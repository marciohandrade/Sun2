#include "stdafx.h"
#include "./SolarDatabaseInDBP.h"

//==================================================================================================

SolarDatabaseInDBP::SolarDatabaseInDBP()
    : requested_release_(false)
    , query_list_(NULL)
    , initialized_(false)
    , connected_(false)
{
}

SolarDatabaseInDBP::~SolarDatabaseInDBP()
{
}

bool SolarDatabaseInDBP::Initialize(const SolarDatabaseDesc& desc, bool use_pending_pool)
{
    bool success = solar_database_.Initialize(desc);
    SetConnectState(success);
    if (success && use_pending_pool)
    {
        const util::WxRingBuffer::eFlagBase sync_flag = util::WxRingBuffer::eFlag_Null;
        ulong pending_pool_size = (desc.query_pool_size << 1);
        ;     pending_pool_size = pending_pool_size > kPendingPoolSize
                                ? pending_pool_size : kPendingPoolSize;
        const ulong buffer_size = sizeof(QueryResult*) * pending_pool_size;
        query_list_ = new util::WxRingBuffer(sync_flag, buffer_size);
    }
    initialized_ = success;
    return success;
}

void SolarDatabaseInDBP::Release()
{
    assert(requested_release_ == false);
    requested_release_ = true;
    if (initialized_ == false) {
        return;
    };
#if SUN_CODE_BACKUP
    // NOTE: it is handled in single logic thread
    // for the lifetime management of a solardatabase module
    while (bool need_update = solar_database_.IsRemainsQueries())
    {
        Sleep(100);
        solar_database_.Update();
    };
#endif
    // release stable
    solar_database_.Release();
    SetConnectState(false);
    // NOTE: the query pending list is not controlled. it's only controlled under 'Query'
    SAFE_DELETE(query_list_);
}

bool SolarDatabaseInDBP::DBQuery(Query* query)
{
    // NOTE: f101103.x pooling. the main thread is stopped until flush queue
    // if main logic is entered at this point.
    // for example, when the dbproxy server is disconnected with database.
    // NOTE: (additional) this logic has no thread event handling because of meanless
    // CHANGES: f101126.1L, changes to non-blocking process
    // because it's possible network buffer problem while blocking.
    if (query_list_ == NULL)
    {
        bool queried = solar_database_.DBQuery(query);
        if (queried) {
            return true;
        }
    }
    else if (query_list_->Enqueue(reinterpret_cast<uint8_t*>(&query), sizeof(query)))
    {
        return UpdateInterQuery();
    }
    ASSERT(!"can't update anymore, memory leaks!");
    return false;
}

void SolarDatabaseInDBP::Update()
{
    if (query_list_) {
        UpdateInterQuery();
    }
    return solar_database_.Update();
}

// CHANGES: f101126.1L
bool SolarDatabaseInDBP::UpdateInterQuery()
{
    assert(query_list_);
    Query* query = NULL;
    while (query_list_->Peek(reinterpret_cast<uint8_t*>(&query), sizeof(query)))
    {
        bool queried = solar_database_.DBQuery(query);
        if (queried == false) {
            return false;
        }
        query_list_->Dequeue(NULL, sizeof(query));
    }
    return true;
}

// NOTE: f110628.1L, added the specialied busy update query counter to support a load banalcing
int SolarDatabaseInDBP::GetNumberOfPendingQueries() const
{
    if (query_list_ == NULL) {
        return 0;
    };
    util::RingBufferInfo buffer_info;
    query_list_->WritableInfo(&buffer_info);
    DWORD buffer_size = query_list_->BufferLength();
    ;     buffer_size -= (buffer_info.first.size + buffer_info.second.size);
    int queries = buffer_size / sizeof(Query*);
    return queries;
}