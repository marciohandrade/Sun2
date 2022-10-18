#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#pragma warning(disable: 4121)

#include "./SolarDatabase.h"
#include "./IDBCInterface.h"
#include "./IODBC.h"
//#include ".\IOLEDB.h"
//#include ".\ISQLLIB.h"
#include "./QueryExecuteProcessor.h"
#include "./QueryResult.h"

#pragma warning(pop)

//==================================================================================================

SolarDatabase::SolarDatabase()
{
    ZeroMemory(&inter_data_, sizeof(inter_data_));
}

SolarDatabase::~SolarDatabase()
{
}

//==================================================================================================

bool SolarDatabase::IsRemainsQueries() const
{
    bool remains = false;
    remains = (inter_data_.query_list_->IsEmpty() == false);
    if (remains) {
        return true;
    };
    remains = (inter_data_.query_result_list_->IsEmpty() == false);
    if (remains) {
        return true;
    };
    return false;
}

int SolarDatabase::GetBufferedQueryNum() const
{
    int queries = 0;
    util::RingBufferInfo buffer_info;
    ;{
        inter_data_.query_list_->WritableInfo(&buffer_info);
        DWORD buffer_size = inter_data_.query_list_->BufferLength();
        ;     buffer_size -= (buffer_info.first.size + buffer_info.second.size);
        queries += buffer_size / sizeof(Query*);
    };
    ;{
        inter_data_.query_result_list_->ReadableInfo(&buffer_info);
        DWORD buffer_size = (buffer_info.first.size + buffer_info.second.size);
        queries += buffer_size / sizeof(Query*);
    };
    return queries;
}

//==================================================================================================

void SolarDatabase::Release()
{
    inter_data_.is_end_process_ = true;

    SAFE_DELETE(inter_data_.thread_pool_);

    /// 스레드가 파괴된 후 : 스레드에서 사용하기때문에
    CloseHandle(inter_data_.qep_wakeup_event_);

    SAFE_DELETE(inter_data_.query_list_);
    SAFE_DELETE(inter_data_.query_result_list_);

    /// 스레드가 다 죽은 후 : Thread에서 DBCInstance 사용하기 때문에
    //m_pDBCInstance->Disconnect();
    //m_pDBCInstance->Release();
    if (inter_data_.dbc_instance_array_)
    {
        for (WORD i = 0; i < inter_data_.query_processor_num_; ++i)
        {
            delete inter_data_.dbc_instance_array_[i];
            inter_data_.dbc_instance_array_[i] = NULL;
        }
        delete [] inter_data_.dbc_instance_array_;
        inter_data_.dbc_instance_array_ = NULL;
    }
}

bool SolarDatabase::Initialize(const SolarDatabaseDesc& desc)
{
    ASSERT(!inter_data_.dbc_instance_array_);

    ASSERT(!IsBadReadPtr(desc.database_ip, sizeof(desc.database_ip)));
    ASSERT(!IsBadReadPtr(desc.database_name, sizeof(desc.database_name)));
    ASSERT(!IsBadReadPtr(desc.user_name, sizeof(desc.user_name)));
    ASSERT(!IsBadReadPtr(desc.user_password, sizeof(desc.user_password)));


    ASSERT(!IsBadReadPtr(desc.fnptr_error_message, sizeof(desc.fnptr_error_message)));
    ASSERT(!IsBadReadPtr(desc.fnptr_error_message_ex, sizeof(desc.fnptr_error_message_ex)));
    ASSERT(!IsBadReadPtr(desc.fnptr_query_result, sizeof(desc.fnptr_query_result)));

    inter_data_.fnptr_error_message_ = desc.fnptr_error_message;
    inter_data_.fnptr_query_result_ = desc.fnptr_query_result;
    // CHANGES: f101022.3L, change to single thread specification,
    // multi-threaded query processor in a solar database can't support a FIFO order.
    inter_data_.query_processor_num_ = 1; //desc.query_processor_num;

    const int query_processor_num = inter_data_.query_processor_num_;
    switch (desc.database_module_type)
    {
    case DBCInterfaceType_ODBC:
        {
            inter_data_.dbc_instance_array_ = new IDBCInterface*[query_processor_num];
            for (int i = 0; i < query_processor_num; ++i) {
                inter_data_.dbc_instance_array_[i] = new IODBC;
            }
        }
        break;
        /*
        case DBCInterfaceType_OLEDB:
            {
                dbc_instance_array_ = new IDBCInterface*[query_processor_num_];
                for (WORD i = 0; i < query_processor_num_; ++i)
                dbc_instance_array_[i] = new IOLEDB;
            }
            break;
        case DBCInterfaceType_SQLLIB:
            {
                dbc_instance_array_ = new IDBCInterface*[query_processor_num_];
                for (WORD i = 0; i < query_processor_num_; ++i)
                dbc_instance_array_[i] = new ISQLLIB;
            }
            break;
        */
    default:
        {
            ASSERT(!"Not Exist database_module_type");
            Release();
            return false;
        }
    }
    //
    const DWORD login_timeout = 3; // seconds
    //desc.login_timeout = 3;
    for (int i = 0; i < query_processor_num; ++i)
    {
        IDBCInterface* idbc_interface = inter_data_.dbc_instance_array_[i];
        if (!idbc_interface->Init(i, desc.database_ip, desc.database_name,
                                  desc.user_name, desc.user_password,
                                  inter_data_.fnptr_error_message_, desc.fnptr_error_message_ex))
        {
            return false;
        }
        if (idbc_interface->Connect(login_timeout) == false) {
            return false;
        }
    }

    /// 각종 데이터 초기화
    inter_data_.qep_wakeup_event_ = CreateEvent(NULL, false, false, NULL);
    /// 각종 자료구조 초기화
    ;{
        const util::WxRingBuffer::eFlagBase sync_flag = util::WxRingBuffer::eSync_Mask;
        const DWORD modified_num = desc.query_pool_size <= 1 ? 2 : desc.query_pool_size;
        const DWORD buffer_size = sizeof(QueryResult*) * modified_num;
        inter_data_.query_list_ = new util::WxRingBuffer(sync_flag, buffer_size);
        inter_data_.query_result_list_ = new util::WxRingBuffer(sync_flag, buffer_size);
    };

    inter_data_.thread_pool_ = new util::ThreadPool;
    inter_data_.thread_pool_->SetAutoRelease(true);
    util::IThreadInterface* pThread = NULL;
    for (int i = 0; i < query_processor_num; ++i)
    {
        pThread = new QueryExecuteProcessor(*this, inter_data_.dbc_instance_array_[i]);
        inter_data_.thread_pool_->AddThread(pThread, true);
    }

    inter_data_.thread_pool_->BatchStartThread();

    inter_data_.is_end_process_ = false;

    return true;
}

bool SolarDatabase::ReConnect(WORD db_index)
{
    if (db_index < inter_data_.query_processor_num_)
    {
        IDBCInterface* idbc_interface = inter_data_.dbc_instance_array_[db_index];
        idbc_interface->Disconnect();
        return idbc_interface->Connect(1);
    }

    return false;
}

bool SolarDatabase::DBQuery(Query* query)
{
    ASSERT(query);
    /// 데이터베이스 모듈을 파괴중일때 쿼리를 날릴 수 없다.
    if (inter_data_.is_end_process_) {
        return false;
    }

    bool enqueued = (inter_data_.query_list_->Enqueue(\
        reinterpret_cast<uint8_t*>(&query), sizeof(query)) != false);

    SetEvent(inter_data_.qep_wakeup_event_);

    return enqueued;
}

//////////////////////////////////////////////////////////////////////////
/// 로직 구현 부분에서 계속 호출 되어야 하는 함수
//////////////////////////////////////////////////////////////////////////
void SolarDatabase::Update()
{
    ASSERT(this);
    /// DB 쿼리 결과값 리턴 
    if (inter_data_.query_result_list_ == NULL ||
        inter_data_.query_result_list_->IsEmpty())
    {
        return;
    }

    const bool kInfinite_loop = true;
    QueryResult* query_result = NULL;
    while (kInfinite_loop)
    {
        query_result = NULL;
        if (inter_data_.query_result_list_->Dequeue(\
                reinterpret_cast<uint8_t*>(&query_result), sizeof(query_result)) == false)
        {
            break;
        }

        inter_data_.fnptr_query_result_(query_result->GetIndex(), query_result);
    }
}

//==================================================================================================
