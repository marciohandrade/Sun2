#pragma once
#ifndef WORLDSERVER_WORLD_LOG_H
#define WORLDSERVER_WORLD_LOG_H
//==================================================================================================
/// WorldLog
/**
    @history
            - 2006.5.15 : [taiyo] 버그 수정 및 위험한 문자열 처리 수정
*/
//==================================================================================================
#include <BaseLog.h>
#include <ChatData.h>
#include <MoneyData.h>
#include <ActionData.h>

//==================================================================================================

namespace ns_world {
;
class WorldGuild;

class WorldLog : public BaseLog
{
private:
    bool enabled_log_categories_[CHAT_CHANNEL - CHAT_NORMAL + 1];
    ChatData    chat_log_data_;
    MoneyData   money_log_data_;
    ActionData  action_log_data_; //_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    TCHAR temp_buffer_[MAX_ADDINFO_SIZE + 1];

public:
    WorldLog();
    ~WorldLog();

    bool IsLogAble(eLOG_CHAT eChatType) const;
    void SetLogAble();

    //////////////////////////////////////////////////////////////////////////
    // 채팅 로그
    // 1. 일반 채팅
    void WriteNormalChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* pszChatMsg, DWORD MaxSize);
    // 2. 외치기
    void WriteShoutChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* szChatMsg, DWORD MaxSize);
    // 3. 파티 채팅
    void WritePartyChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* szChatMsg, DWORD MaxSize);
    // 4. 길드 채팅
    void WriteGuildChat(ns_world::WorldUser* user, int nChannelID,
        ns_world::WorldGuild* pGuild, TCHAR* szChatMsg, DWORD MaxSize);
    // 5. 귓속말
    void WriteWhisperChat(ns_world::WorldUser* user, int nChannelID,
        ns_world::WorldUser* target_user, TCHAR* szChatMsg, DWORD MaxSize);
    // 6. 친구 채팅(1:1 채팅)
    void WriteFriendChat(ns_world::WorldUser* user, int nChannelID,
        ns_world::WorldUser* friend_user, TCHAR* szChatMsg, DWORD MaxSize);
    // 7. 채널 채팅(채팅방 생성)
    void WriteChannelChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* ptszChatMsg, DWORD dwMaxSize);

    //////////////////////////////////////////////////////////////////////////
    // 하임 로그
    // 1. 하임 로또 정산 로그(로그타입, 케릭터 고유값, 하임로또 회차, 당첨등수, 당첨금액)
    void LogSettleHeimLotto(int log_type,
        ns_world::WorldUser* user, CHARGUID char_guid,
        WORD draw_num, BYTE winner_grade, MONEY amount_cost);
    //////////////////////////////////////////////////////////////////////////
    // 액션 로그
    // implemented by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    void LogAction(int log_type,
        ns_world::WorldUser* user, const TCHAR* char_name,
        const TCHAR* additional_info = NULL);
};

}; //end of namespace

//==================================================================================================

inline bool ns_world::WorldLog::IsLogAble(eLOG_CHAT eChatType) const
{
    return enabled_log_categories_[eChatType - CHAT_NORMAL];
}


#endif //WORLDSERVER_WORLD_LOG_H