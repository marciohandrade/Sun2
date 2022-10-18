#ifndef _HANAUTH_QUERY_MANAGER_H
#define _HANAUTH_QUERY_MANAGER_H

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

#include <HanAuth/HanAuthForSvr.h>
#include "./HanAuthStruct.h"
#include <ObjKeyGenerator.h>
#include <ITimeout.h>

//==================================================================================================
//
//
namespace util {

class WxRingBuffer;

};

//==================================================================================================
//
//
namespace nsHanAuth {

//==================================================================================================
//
//
struct QuerySlot
{
    INDEX_ID_T  index_;
    DWORD       session_index_;
    DWORD       front_id_;
    char client_ip_[IP_MAX_LEN];
    char local_ip_[IP_MAX_LEN];

    inline void SetInfo(INDEX_ID_T index, DWORD session_index, DWORD front_id,
                        char client_ip[IP_MAX_LEN], char local_ip[IP_MAX_LEN]);
    inline BOOLEAN IsValid(INDEX_ID_T index) const { return index_ == index; }
    inline void Clear() { ZeroMemory(this, sizeof(*this)); }
};

inline void
QuerySlot::SetInfo(INDEX_ID_T index, DWORD session_index, DWORD front_id,
                   char client_ip[IP_MAX_LEN], char local_ip[IP_MAX_LEN])
{
    index_ = index;
    session_index_ = session_index;
    front_id_ = front_id;

    strncpy(client_ip_, client_ip, _countof(client_ip_));
    client_ip_[_countof(client_ip_) - 1] = '\0';

    strncpy(local_ip_, local_ip, _countof(local_ip_));
    local_ip_[_countof(local_ip_) - 1] = '\0';
}

//==================================================================================================
//
struct IHanAuthCallback
{
    virtual void AuthResult(INDEX_ID_T query_slot_index, int hanAuth_result,
                            const char user_id[ID_MAX_LEN + 1] /* etc ... */) = 0;
};

//==================================================================================================
//
//
class HanAuthQueryManager
{
protected:
    HanAuthQueryManager();
    virtual ~HanAuthQueryManager();

public:
    static void CreateInstance();
    static void DestroyInstance();

    // action:
    //  thread 0
    // result:
    //  BOOLEAN
    static BOOLEAN Init(GAME_ID_BLOCK szGameId, int nServiceCode);

    // action:
    //  thread 0
    // arguments:
    //  callback시 필요 정보 - QuerySlot
    static INDEX_ID_T AllocSlot(const QuerySlot& callback_data);
    static BOOLEAN ReleaseSlot(INDEX_ID_T query_slot_index);
    static BOOLEAN GetSlot(INDEX_ID_T query_slot_index, QuerySlot* slot);

    static BOOLEAN QueryAuthentication(INDEX_ID_T query_slot_index,
                                       MEMBER_ID_BLOCK szMemberId, AUTH_STRING_BLOCK szAuthString);
    static BOOLEAN Update(IHanAuthCallback* callback_interface);

private:
    static unsigned int __stdcall QueryThread(void* parameter);

    void NotifySyncMessage();
    void ReplySyncMessage(const Message_Sync* msg);

#define HANAUTH_MSG_NODE(type)  \
    static BOOLEAN MessageProc_##type(const type* msg);

    static BOOLEAN MessageProc_Message_Close(const Message_Close* msg);
    static BOOLEAN MessageProc_Message_Sync(const Message_Sync* msg);
    static BOOLEAN MessageProc_Message_Authentication(const Message_Authentication* msg);
    static BOOLEAN MessageProc_Message_AuthenticationResult(const Message_AuthenticationResult* msg,
                                                            IHanAuthCallback* callback_interface);

#undef HANAUTH_MSG_NODE

    static const INDEX_ID_T QuerySlotCounts = 12000;
    static const DWORD QueryQueueSize = 12000 * 4;
    typedef util::CObjKeyGenerator<INDEX_ID_T> QueryKeyGenerator;
    HANDLE query_thread_handle_, query_io_handle_;
    enum eClosingStatus { eClosing_None, eClosing_Closing, eClosing_Closed, };
    BOOLEAN closing_state_;
    ITimerBase sync_timer_;
    QueryKeyGenerator* const m_pQueryKeyGenerator;
    QuerySlot* const m_QuerySlots;
    util::WxRingBuffer* const m_QueryRequestQueue;  // main -> query thread
    util::WxRingBuffer* const m_QueryResultQueue;   // query thread -> main

    static HanAuthQueryManager* instance_s_;
};

}; //end of namespace 'nsHanAuth'

#endif //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

#endif //_HANAUTH_QUERY_MANAGER_H


