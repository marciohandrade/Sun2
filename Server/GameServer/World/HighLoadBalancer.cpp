#include "stdafx.h"
#include "HighLoadBalancer.h"
//
#include <StreamHashKey.h>
//
#include "GameZone.h"

// @history: waverix, refactoring, changed by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
namespace nsGrid {
;

struct HLB_SequentialNode : public util::__LList<HLB_SequentialNode>
{
    typedef HLB_BufferInfo::NetMsgSizeType NetMsgSizeType;
    NetMsgSizeType msg_size;
    // Next
    // User defined message .... (with msg_size)
};

struct HLB_Buffer
{
    HLB_Buffer(const DWORD input_hlb_key)
        : HLBKEY(hlb_key)
        , hlb_key(input_hlb_key)
    {
        region_header->size = 0;
        util::LList::Init(&sequential_root);
    }
    // Integrity check (&HLBKEY == &m_HLBKey)
    //
    const DWORD& HLBKEY;
    DWORD hlb_key;
    HLB_SequentialNode sequential_root;
#pragma pack(push, 1)
    WAR_MSG_HDR region_header[1];
    uint8_t region_stream[HighLoadBalancer::kMaxRegionStreamSize];
    //
#pragma pack(pop)

};

//==================================================================================================
//  Sync Message Format (v0200)
//
//  -----------------------------------------------------------------------------
//  |      MSG_BASE      | WAR_MSG_HDR |   MSGSUB_SYNC_MESSAGEs
//  |  HLBKEY | m_HLBKey | ulong(size) |   byte stream[xxxx] ...
//  -----------------------------------------------------------------------------
//  MSGSUB_SYNC_MESSAGEs
//  -----------------------------------------------------------------------------
//  | __LListNode |   Size   | TypeCode |
//  | prev | next | uint16_t |   BYTE   |
//  -----------------------------------------------------------------------------
//                         |-> MSGSUB_(SYNC, SKILL, ...)

}; // end of namespace

//==================================================================================================
//  Sequential Message Format (v0100)
//
//  -----------------------------------------------------------------------------
//  | WAR_MSG_HDR | NORMAL_MESSAGEs or SEQUENTIAL_STREAM
//  | ulong(size) | byte stream[xxxx] ...
//  -----------------------------------------------------------------------------
//  NORMAL_MESSAGEs
//  -----------------------------------------------------------------------------
//  |   Size   |  header  | body |
//  | uint16_t | MSG_BASE | .... |
//  -----------------------------------------------------------------------------
//  SEQUENTIAL_STREAM buffer format
//  -----------------------------------------------------------------------------
//  | sequential_root | ... | LinkedListNode | NetMsgSizeType | UserMessage |
//  -----------------------------------------------------------------------------
//==================================================================================================
// EP2 removed
// Post Action Synchronization Message Stream Format (v0.1.0.0)
//  SectorIdx | Size |   MSGSUB_xxx    |  SectorIdx | Size |   MSGSUB_xxx    |    ...
//      0~    |  > 0 |  TAG_  | BODY_  |      0~    |  > 0 |  TAG_  | BODY_  |    ...


#ifdef BUF_ALIGN_PTR
    #error "this macro should be used as local declaration only"
#endif
#define BUF_ALIGN_PTR(__size)    (((__size) + ((sizeof(void*)) - 1)) & (~((sizeof(void*)) - 1)))

void HLB_BufferInfo::AppendTimeSyncMsg(MSG_BASE* const msg, const WORD msg_size)
{
    HLB_SequentialNode* root = sequential_root_;

    WAR_MSG_HDR::MSG_SIZE_T header_offset = region_header_->size;
    ;                       header_offset = BUF_ALIGN_PTR(header_offset);
    //const uint16_t size = GetSize();
    WAR_MSG_HDR::MSG_SIZE_T next_offset = header_offset + sizeof(*root) + msg_size;
    //
    if (next_offset >= HighLoadBalancer::kMaxRegionStreamSize)
    {
        HighLoadBalancer::Instance()->Update(game_field_, true);
        // recalculate
        header_offset = region_header_->size;
        header_offset = BUF_ALIGN_PTR(header_offset);
        next_offset = header_offset + sizeof(*root) + msg_size;
    };
    //
    HLB_SequentialNode* const dest = reinterpret_cast<HLB_SequentialNode*>(
        reinterpret_cast<uint8_t*>(region_header_ + 1) + header_offset);
    ;{
        util::LList::Init(dest);
        dest->msg_size = msg_size;
    };
    ;{
        CopyMemory((dest + 1), msg, msg_size);
    };
    ;{
        region_header_->size = next_offset;
        //
        util::LList::AddPrev(dest, root);
    };
}

void HLB_BufferInfo::AppendTimeSyncMsgArray(const DWORD number_of_messages,
    BYTE** const IN message_array, WORD* const message_size_array)
{
    WAR_MSG_HDR::MSG_SIZE_T total_size = 0;
    if (DWORD loop = number_of_messages)
    {
        NetMsgSizeType* size_it = message_size_array;
        while (loop--) {
            total_size += *size_it++;
        }
    };
    BOOST_STATIC_ASSERT(static_cast<NetMsgSizeType>(-1) > static_cast<NetMsgSizeType>(0) &&
                        static_cast<NetMsgSizeType>(-1) == USHRT_MAX);
    if (total_size == 0 || total_size > USHRT_MAX) {
        return;
    }
    //-----------------------------------------------
    HLB_SequentialNode* root = sequential_root_;

    WAR_MSG_HDR::MSG_SIZE_T header_offset = region_header_->size;
    ;                       header_offset = BUF_ALIGN_PTR(header_offset);
    //const uint16_t size = GetSize();
    WAR_MSG_HDR::MSG_SIZE_T next_offset = header_offset + sizeof(*root) + total_size;
    //
    if (next_offset >= HighLoadBalancer::kMaxRegionStreamSize)
    {
        HighLoadBalancer::Instance()->Update(game_field_, true);
        // recalculate
        header_offset = region_header_->size;
        header_offset = BUF_ALIGN_PTR(header_offset);
        next_offset = header_offset + sizeof(*root) + total_size;
    };
    //
    HLB_SequentialNode* const dest = reinterpret_cast<HLB_SequentialNode*>(
        reinterpret_cast<uint8_t*>(region_header_ + 1) + header_offset);
    ;{
        util::LList::Init(dest);
        dest->msg_size = static_cast<NetMsgSizeType>(total_size);
    };
    if (DWORD loop = number_of_messages)
    {
        uint8_t* msg_it = reinterpret_cast<uint8_t*>(dest + 1);
        NetMsgSizeType* input_size_it = message_size_array;
        uint8_t** input_msg_it = message_array;
        while (loop--) {
            CopyMemory(msg_it, *input_msg_it++, *input_size_it);
            msg_it += *input_size_it++;
        }
    };
    ;{
        region_header_->size = next_offset;
        //
        util::LList::AddPrev(dest, root);
    };
}

//==================================================================================================
// HighLoadBalancer
//

HighLoadBalancer* HighLoadBalancer::instance_static_ = 0;

void HighLoadBalancer::CreateInstance()
{
    if (instance_static_ == NULL) {
        instance_static_ = new HighLoadBalancer;
    };
}

void HighLoadBalancer::DestroyInstance()
{
    SAFE_DELETE(instance_static_);
}

//--------------------------------------------------------------------------------------------------


HighLoadBalancer::~HighLoadBalancer()
{
    FOREACH_CONTAINER(MSG_STREAM_BUFFER_POOLLIST::value_type& pool_node, msg_buffer_pool_,
                      MSG_STREAM_BUFFER_POOLLIST)
    {
        nsGrid::HLB_Buffer* hlb_buffer = pool_node;
        TAllocFree(hlb_buffer);
    }

    FOREACH_CONTAINER(MSG_STREAM_BUFFER_INDEXER::value_type& index_node, msg_buffer_indexer_,
                      MSG_STREAM_BUFFER_INDEXER)
    {
        nsGrid::HLB_Buffer* hlb_buffer = index_node.second;
        if (hlb_buffer) {
            TAllocFree(hlb_buffer);
        }
    }
}


//--------------------------------------------------------------------------------------------------


void
HighLoadBalancer::RegisterBuffer(GameField* const game_field)
{
    const ulong hash_key = util::StreamHashKey::GetMemoryHashKey(&game_field, sizeof(game_field));
    HLB_Buffer*& hlb_buffer = msg_buffer_indexer_[hash_key]; // reference
    if (hlb_buffer != NULL) {
        return;
    };

    const bool is_pool_empty = msg_buffer_pool_.empty();
    hlb_buffer = is_pool_empty ? (HLB_Buffer*)TAllocMalloc(sizeof(*hlb_buffer))
                               : msg_buffer_pool_.front();
    new (hlb_buffer) HLB_Buffer(hash_key);

    HLB_BufferInfo*& field_hlb_buffer = game_field->m_pHLB; // reference
    if (field_hlb_buffer == NULL) {
        field_hlb_buffer = TAllocNew(HLB_BufferInfo);
    }
    ;{
        field_hlb_buffer->game_field_ = game_field;
        field_hlb_buffer->region_header_ = hlb_buffer->region_header;
        field_hlb_buffer->sequential_root_ = &hlb_buffer->sequential_root;
    };
    if (is_pool_empty == false) {
        msg_buffer_pool_.pop_front();
    }
}


//--------------------------------------------------------------------------------------------------


void
HighLoadBalancer::UnRegisterBuffer(GameField* const game_field)
{
    const ulong hash_key = util::StreamHashKey::GetMemoryHashKey(&game_field, sizeof(game_field));
    MSG_STREAM_BUFFER_INDEXER::iterator it = msg_buffer_indexer_.find(hash_key);
    if (it == msg_buffer_indexer_.end())
    { // game_field->m_pSyncStreamRoot은 여기서만 제어하는 것을 전제한다. 따라서 그냥 0설정
        game_field->m_pHLB = NULL;
        return;
    }

    // found
    HLB_BufferInfo*& field_hlb_buffer = game_field->m_pHLB;
    if (field_hlb_buffer) {
        TAllocDelete(HLB_BufferInfo, field_hlb_buffer);
    }
    field_hlb_buffer = 0;
    //  남겨진 정보 보낼까도 생각해 봤지만, 필드가 종료되는 시점이다...
    // 유저가 있을 리 없잖아? 아니 없어야 한다.
    //  (부연) - 더 이상 의미 없는 정보일 가능성이 높다.

    ;{
        HLB_Buffer* const hlb_buffer = it->second;
        msg_buffer_indexer_.erase(it);
        msg_buffer_pool_.push_back(hlb_buffer);
    };
}


//--------------------------------------------------------------------------------------------------


void
HighLoadBalancer::RecyclingBuffer(GameField* const pGameField)
{
    HLB_BufferInfo* const hlb_buffer = pGameField->m_pHLB;
    if (hlb_buffer)
    {
        hlb_buffer->region_header_->size = 0;
        util::LList::Init(hlb_buffer->sequential_root_);
    }
}


void HighLoadBalancer::Update(GameField* const game_field, const BOOLEAN need_flush)
{
    __UNUSED(need_flush);
    HLB_BufferInfo* const hlb_buffer = game_field->m_pHLB;
    if (hlb_buffer == NULL) {
        return;
    }
    WAR_MSG_HDR* const region_header = hlb_buffer->region_header_;
    //const BOOLEAN need_flush_internal
    //    = need_flush ?  true
    //                 :  (kMaxRegionStreamAlarmSize < region_header->size);
    //if (need_flush_internal == false) {
    //    return;
    //}

    //
    // #1: send time sync buffer
    HLB_SequentialNode* const sequential_root = hlb_buffer->sequential_root_;
    HLB_SequentialNode* seq_it = static_cast<HLB_SequentialNode*>(sequential_root->next);
    for ( ; seq_it != sequential_root; seq_it = static_cast<HLB_SequentialNode*>(seq_it->next))
    {
        g_pGameServer->SendToServer(AGENT_SERVER,
            reinterpret_cast<MSG_BASE*>(seq_it + 1), seq_it->msg_size);
    };
    // #2: notify region events
    game_field->SectorGridRef.Update(game_field);
    //
    ;{  // clear
        region_header->size = 0;
        util::LList::Init(sequential_root);
    };
    return;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
