#include "stdafx.h"
#include "./HanAuthQueryManager.h"

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

#include <WxRingBuffer.h> //utility
#include <process.h>

//reference : "__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__"

namespace nsHanAuth {

HanAuthQueryManager*
HanAuthQueryManager::instance_s_ = 0;

const DWORD
Message::MaxMessageSize = max(sizeof(Message_Close),
                          max(sizeof(Message_Sync),
                          max(sizeof(Message_Authentication),
                          max(sizeof(Message_AuthenticationResult),
                          0))));

void
HanAuthQueryManager::CreateInstance()
{
    if(instance_s_ == 0)
        instance_s_ = new HanAuthQueryManager();
}

void
HanAuthQueryManager::DestroyInstance()
{
    if(instance_s_ != 0)
        delete instance_s_;
    instance_s_ = 0;
}

HanAuthQueryManager::HanAuthQueryManager()
    : query_thread_handle_(INVALID_HANDLE_VALUE)
    , query_io_handle_(INVALID_HANDLE_VALUE)
    , closing_state_(eClosing_None)
    , m_pQueryKeyGenerator(new QueryKeyGenerator)
    , m_QuerySlots(new QuerySlot[QuerySlotCounts])
    , m_QueryRequestQueue(new util::WxRingBuffer(
        util::WxRingBuffer::eSync_Mask, QueryQueueSize))
    , m_QueryResultQueue(new util::WxRingBuffer(
        util::WxRingBuffer::eSync_Mask, QueryQueueSize))
{
    BOOST_STATIC_ASSERT(INVALID_INDEX_ID + 1 == 1 && 1 < QuerySlotCounts);
    m_pQueryKeyGenerator->Create(1, QuerySlotCounts);
    ZeroMemory(m_QuerySlots, sizeof(QuerySlot) * QuerySlotCounts);
}

HanAuthQueryManager::~HanAuthQueryManager()
{
    while(true) {
        DWORD result = WaitForMultipleObjectsEx(1, &query_thread_handle_, TRUE, INFINITE, TRUE);
        if(result == WAIT_IO_COMPLETION)
            continue;
        else
            break;
    }
    delete m_QuerySlots;
    delete m_pQueryKeyGenerator;
    delete m_QueryRequestQueue;
    delete m_QueryResultQueue;
}

//--------------------------------------------------------------------------------------------------
// action:
//  thread 0
// result:
//  BOOLEAN
BOOLEAN
HanAuthQueryManager::Init(GAME_ID_BLOCK szGameId, int nServiceCode)
{
    // (WAVERIX) (090722) (NOTE) 유저 관련된 정보를 'HanAuthQueryManager::Init'에서 수행하지 말 것.
    //assert(instance_s_);
    struct ErrorGroup {
        enum eError {
            eError_Success,
            eError_HanAuthFail,
            eError_SystemFail,
        };
        BOOLEAN is_succeed;
        int error_group;
    } error;

    error.is_succeed = true;
    error.error_group = error.eError_Success;

    NORTHA_DEBUGLOG(__FUNCTION__" mode %d\n", _AUTH_NORTHA_TEST_MODE);
    int hanAuth_Result = HanAuthInit(szGameId, nServiceCode);
    NORTHA_DEBUGLOG("HanAuthInit %d\n", hanAuth_Result);
#if _AUTH_NORTHA_TEST_MODE == _AUTH_NORTHA_TEST_MODE__NOTDEV__
    if(error.is_succeed && hanAuth_Result != HAN_AUTHSVR_OK)
        error.is_succeed = false, error.error_group = error.eError_HanAuthFail;
#endif

    if(error.is_succeed) {
        instance_s_->query_io_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 1);
        if(instance_s_->query_io_handle_ == INVALID_HANDLE_VALUE)
            error.is_succeed = false, error.error_group = error.eError_SystemFail;
    }

    if(error.is_succeed) {
        instance_s_->query_thread_handle_ = (HANDLE)_beginthreadex(0, 0, &QueryThread, 0, 0, 0);
        if(instance_s_->query_thread_handle_ == INVALID_HANDLE_VALUE)
            error.is_succeed = false, error.error_group = error.eError_SystemFail;
    }

    if(error.is_succeed) {
        instance_s_->sync_timer_.Configure(ITimerBase::LOOP, 5 * 1000);
        instance_s_->NotifySyncMessage();
        //PostQueuedCompletionStatus(instance_s_->query_io_handle_, 0, (ULONG_PTR)instance_s_, 0);
    }

    if(error.is_succeed == 0) {
        const char* error_group_msg = 0;
        if(error.error_group == error.eError_HanAuthFail) {
            error_group_msg = "HanGameAuthInit Failed";
            error.error_group = hanAuth_Result;
        } else {
            error_group_msg = "HanGameAuthSystemInit Failed";
        }

        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][%d][%s]", error.error_group, error_group_msg);
    }
    NORTHA_DEBUGLOG(__FUNCTION__" error.is_succeed - %d\n", error.is_succeed);
    return error.is_succeed;
}

//--------------------------------------------------------------------------------------------------
// action:
//  thread 0
// arguments:
//  callback시 필요 정보 - QuerySlot
INDEX_ID_T
HanAuthQueryManager::AllocSlot(const QuerySlot& callback_data)
{
    //assert(instance_s_);
    INDEX_ID_T new_key = instance_s_->m_pQueryKeyGenerator->GetKey();
    if(new_key == INVALID_INDEX_ID)
        return INVALID_INDEX_ID;

    INDEX_ID_T slot_index = new_key - 1;
    QuerySlot& slot = instance_s_->m_QuerySlots[slot_index];
    slot = callback_data;
    slot.index_ = slot_index;

    return new_key;
}

BOOLEAN
HanAuthQueryManager::ReleaseSlot(INDEX_ID_T query_slot_index)
{
    if(query_slot_index == INVALID_INDEX_ID ||
       query_slot_index > QuerySlotCounts)
        return FALSE;

    INDEX_ID_T slot_index = query_slot_index - 1;
    QuerySlot& slot = instance_s_->m_QuerySlots[slot_index];
    slot.Clear();
    instance_s_->m_pQueryKeyGenerator->RestoreKey(query_slot_index);

    return TRUE;
}

BOOLEAN
HanAuthQueryManager::GetSlot(INDEX_ID_T query_slot_index, QuerySlot* slot)
{
    if(query_slot_index == INVALID_INDEX_ID ||
       query_slot_index > QuerySlotCounts)
    {
        slot->Clear();
        return FALSE;
    }

    INDEX_ID_T slot_index = query_slot_index - 1;
    const QuerySlot& rSlot = instance_s_->m_QuerySlots[slot_index];
    *slot = rSlot;

    return TRUE;
}

BOOLEAN
HanAuthQueryManager::QueryAuthentication(INDEX_ID_T query_slot_index,
                                         MEMBER_ID_BLOCK szMemberId, AUTH_STRING_BLOCK szAuthString)
{
    NORTHA_DEBUGLOG(__FUNCTION__"%d\n", 0);
    //assert(instance_s_ && query_slot_index != INVALID_INDEX_ID);
    if(INVALID_INDEX_ID == query_slot_index ||
       QuerySlotCounts  <  query_slot_index)
       return FALSE;

    if(instance_s_->closing_state_ != eClosing_None)
        return FALSE;

    Message_Authentication msg;
    ZeroMemory(&msg, sizeof(msg));

    strncpy(msg.member_id_, szMemberId, _countof(msg.member_id_));
    msg.member_id_[_countof(msg.member_id_) - 1] = '\0';
    strncpy(msg.auth_string_, szAuthString, _countof(msg.auth_string_));
    msg.auth_string_[_countof(msg.auth_string_) - 1] = '\0';

    msg.length_ = sizeof(msg);
    msg.interface_type_ = msg.MessageID;
    msg.query_index_ = query_slot_index;

    BOOLEAN queried = instance_s_->m_QueryRequestQueue->Enqueue((BYTE*)&msg, sizeof(msg));
    NORTHA_DEBUGLOG(__FUNCTION__" enqueue %d\n", queried);
    return queried;
}

void
HanAuthQueryManager::NotifySyncMessage()
{
    Message_Sync msg;
    msg.interface_type_ = msg.MessageID;
    msg.length_ = sizeof(msg);
    msg.query_index_ = m_pQueryKeyGenerator->GetKey();
    msg.check_start_tick_ = GetTickCount();
    m_QueryRequestQueue->Enqueue((BYTE*)&msg, sizeof(msg));

//#if WAVERIX_AUTH_TEST_CODE
//    static int notified_count = 0;
//    if((++notified_count % 10) == 0)
//    {
//        Message_Close msg;
//        msg.interface_type_ = msg.MessageID;
//        msg.length_ = sizeof(msg);
//        msg.query_index_ = INVALID_INDEX_ID;
//        m_QueryRequestQueue->Enqueue((BYTE*)&msg, sizeof(msg));
//    }
//#endif
}

void
HanAuthQueryManager::ReplySyncMessage(const Message_Sync* msg)
{
    DWORD diff_tick = msg->check_reply_tick_ - msg->check_start_tick_;
    m_pQueryKeyGenerator->RestoreKey(msg->query_index_);
}

BOOLEAN
HanAuthQueryManager::Update(IHanAuthCallback* callback_interface)
{
    //assert(instance_s_ && callback_interface);
    // query thread -> main
    HanAuthQueryManager* const this_ = instance_s_;
    static Message_ChunkBlock chunk_block;
    util::WxRingBuffer* const pReceiver = this_->m_QueryResultQueue;

    if(this_->sync_timer_.IsExpired())
        this_->NotifySyncMessage();

    while(pReceiver->Peek((BYTE*)&chunk_block, sizeof(Message)) != 0)
    {
        DWORD received_length = chunk_block.length_;
        if(received_length > Message::MaxMessageSize)
            return false;

        DWORD expected_length = 0;

    #define HANAUTH_MSG_NODE(type)  \
        case type::MessageID: expected_length = sizeof(type); break

        switch(chunk_block.interface_type_)
        {
        HANAUTH_MSG_NODE(Message_Close);
        HANAUTH_MSG_NODE(Message_Sync);
        //HANAUTH_MSG_NODE(Message_Authentication);
        HANAUTH_MSG_NODE(Message_AuthenticationResult);
        };

    #undef HANAUTH_MSG_NODE

        if(expected_length == 0)
        {
            SUNLOG(eCRITICAL_LOG,
                   "["__FUNCTION__"] Invalid Message Received { Length %u, Type %u }",
                   received_length, chunk_block.interface_type_);
            return false;
        }

        if(pReceiver->Dequeue((BYTE*)&chunk_block, expected_length) == 0)
            return true;

        BOOLEAN need_continue = true;

        switch(chunk_block.interface_type_)
        {
        case Message_Close::MessageID:
            need_continue = MessageProc_Message_Close((Message_Close*)&chunk_block);
            break;
        case Message_Sync::MessageID:
            need_continue = MessageProc_Message_Sync((Message_Sync*)&chunk_block);
            break;
        //case Message_Authentication::MessageID:
        case Message_AuthenticationResult::MessageID:
            {
                Message_AuthenticationResult* msg_result =
                    (Message_AuthenticationResult*)&chunk_block;
                need_continue =
                    MessageProc_Message_AuthenticationResult(msg_result, callback_interface);
            }
            break;
        };
    }
    return TRUE;
}

unsigned int __stdcall
HanAuthQueryManager::QueryThread(void* parameter)
{
    __UNUSED(parameter);
    const HANDLE iocp_handle = instance_s_->query_io_handle_;

    DWORD number_of_transferred_byte = 0;
    ULONG_PTR completion_key = 0;
    LPOVERLAPPED overlapped = 0;
    DWORD error_result = ERROR_SUCCESS;

    static Message_ChunkBlock chunk_block;
    util::WxRingBuffer* const pSender = instance_s_->m_QueryResultQueue;
    util::WxRingBuffer* const pReceiver = instance_s_->m_QueryRequestQueue;

    while(true)
    {
        BOOLEAN q_result = GetQueuedCompletionStatus(iocp_handle,
                                                     &number_of_transferred_byte,
                                                     (PULONG_PTR)&completion_key,
                                                     &overlapped,
                                                     300);

        //if(completion_key == 0)
        //    break;

        while(pReceiver->Peek((BYTE*)&chunk_block, sizeof(Message)) != 0)
        {
            DWORD received_length = chunk_block.length_;
            if(received_length > Message::MaxMessageSize)
                return false; // close process

            DWORD expected_length = 0;

        #define HANAUTH_MSG_NODE(type)  \
            case type::MessageID: expected_length = sizeof(type); break

            switch(chunk_block.interface_type_)
            {
                HANAUTH_MSG_NODE(Message_Close);
                HANAUTH_MSG_NODE(Message_Sync);
                HANAUTH_MSG_NODE(Message_Authentication);
                //HANAUTH_MSG_NODE(Message_AuthenticationResult);
            };

        #undef HANAUTH_MSG_NODE

            if(expected_length == 0)
            {
                //SUNLOG(eCRITICAL_LOG,
                //       "["__FUNCTION__"] Invalid Message Received { Length %u, Type %u }",
                //       received_length, chunk_block.interface_type_);
                __asm { int 3 };
                return false;
            }

            if(pReceiver->Dequeue((BYTE*)&chunk_block, expected_length) == 0)
                return true;

            BOOLEAN need_continue = true;

            switch(chunk_block.interface_type_)
            {
            case Message_Close::MessageID:
                {
                    Message_Close msg;
                    msg.length_ = sizeof(msg);
                    msg.interface_type_ = chunk_block.interface_type_;
                    msg.query_index_ = chunk_block.query_index_;
                    need_continue = false;
                    pSender->Enqueue((BYTE*)&msg, sizeof(msg));
                };
                break;
            case Message_Sync::MessageID:
                {
                    Message_Sync* msg_sync = (Message_Sync*)&chunk_block;
                    Message_Sync msg;
                    msg.length_ = sizeof(msg);
                    msg.interface_type_ = chunk_block.interface_type_;
                    msg.query_index_ = chunk_block.query_index_;
                    msg.check_start_tick_ = msg_sync->check_start_tick_;
                    msg.check_reply_tick_ = GetTickCount();
                    pSender->Enqueue((BYTE*)&msg, sizeof(msg));
                };
                break;
                //case Message_Authentication::MessageID:
            case Message_Authentication::MessageID:
                {
                    Message_Authentication* msg_auth = (Message_Authentication*)&chunk_block;
                    need_continue = MessageProc_Message_Authentication(msg_auth);
                }
                break;
            };

            if(need_continue == 0)
                return 0;
        }
    };

    return 0;
}


BOOLEAN
HanAuthQueryManager::MessageProc_Message_Close(const Message_Close* query_msg)
{
    instance_s_->closing_state_ = eClosing_Closed;
    return false;
}

BOOLEAN
HanAuthQueryManager::MessageProc_Message_Sync(const Message_Sync* query_msg)
{
    // for time sync
    instance_s_->ReplySyncMessage(query_msg);
    return true;
}

BOOLEAN
HanAuthQueryManager::MessageProc_Message_Authentication(const Message_Authentication* query_msg)
{
    NORTHA_DEBUGLOG(__FUNCTION__" mode %d\n", _AUTH_NORTHA_TEST_MODE);
    // in query thread
    char result[SIZE_OUTBUFFER] = { 0, };
    int ret = HAN_AUTHSVR_OK;
#if _AUTH_NORTHA_TEST_MODE < _AUTH_NORTHA_TEST_MODE__NOTDEV__
    if(GetPrivateProfileInt("CONFIG_INFO", "SkipHanAuthForSvr", 1, "./HanAuthUserTestInfo.ini") == 0)
#endif
    {
        NORTHA_DEBUGLOG(__FUNCTION__" real query HanAuthForSvr {%s}\n", &query_msg->member_id_[0]);
        NORTHA_DEBUGLOG(__FUNCTION__" real query HanAuthForSvr {%s}\n", &query_msg->auth_string_[0]);
        ret = HanAuthForSvr(const_cast<char*>(&query_msg->member_id_[0]),
                            const_cast<char*>(&query_msg->auth_string_[0]),
                            result, _countof(result), 0);
    }

    NORTHA_DEBUGLOG(__FUNCTION__" step #1 %d\n", ret);
    Message_AuthenticationResult msg;
    ZeroMemory(&msg, sizeof(msg));

    msg.length_ = sizeof(msg);
    msg.interface_type_ = msg.MessageID;
    msg.query_index_ = query_msg->query_index_;
    msg.result_code_ = ret;
    if(ret == HAN_AUTHSVR_OK)
    {
    #if _AUTH_NORTHA_TEST_MODE < _AUTH_NORTHA_TEST_MODE__NOTDEV__
        if(GetPrivateProfileInt("CONFIG_INFO", "SkipHanAuthForSvr", 1, "./HanAuthUserTestInfo.ini"))
        {
            NORTHA_DEBUGLOG(__FUNCTION__" step #2.1 skip step {%s} \n", &query_msg->member_id_[0]);
            strncpy(msg.user_id_, &query_msg->member_id_[0], _countof(msg.user_id_));
            msg.user_id_[_countof(msg.user_id_) - 1] = '\0';
        }
        else
    #endif
        {
            NORTHA_DEBUGLOG(__FUNCTION__" step #2.2 get step %d \n", 0);
            // 다음 로직은 무조건 성공하도록 유도하자.
            if(HAN_AUTHSVR_OK != HanAuthGetUserInfo(result, 1, msg.user_id_, _countof(msg.user_id_)))
            {
                __asm { int 3 };
                return false;
            }
        }
    }

    instance_s_->m_QueryResultQueue->Enqueue((BYTE*)&msg, sizeof(msg));
    return true;
}

BOOLEAN
HanAuthQueryManager::MessageProc_Message_AuthenticationResult(
    const Message_AuthenticationResult* query_msg, IHanAuthCallback* callback_interface)
{
    NORTHA_DEBUGLOG(__FUNCTION__" mode %d\n", _AUTH_NORTHA_TEST_MODE);
    // in main thread by query thread result
    // query_manager-> AuthDB Query process with query_index information
    callback_interface->AuthResult(query_msg->query_index_,
                                   query_msg->result_code_, query_msg->user_id_);

    return true;
}



}; //end of namespace 'nsHanAuth'

#endif //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
