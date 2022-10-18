#pragma once
#ifndef WORLDSERVER_WORLD_LOG_H
#define WORLDSERVER_WORLD_LOG_H
//==================================================================================================
/// WorldLog
/**
    @history
            - 2006.5.15 : [taiyo] ���� ���� �� ������ ���ڿ� ó�� ����
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
    // ä�� �α�
    // 1. �Ϲ� ä��
    void WriteNormalChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* pszChatMsg, DWORD MaxSize);
    // 2. ��ġ��
    void WriteShoutChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* szChatMsg, DWORD MaxSize);
    // 3. ��Ƽ ä��
    void WritePartyChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* szChatMsg, DWORD MaxSize);
    // 4. ��� ä��
    void WriteGuildChat(ns_world::WorldUser* user, int nChannelID,
        ns_world::WorldGuild* pGuild, TCHAR* szChatMsg, DWORD MaxSize);
    // 5. �ӼӸ�
    void WriteWhisperChat(ns_world::WorldUser* user, int nChannelID,
        ns_world::WorldUser* target_user, TCHAR* szChatMsg, DWORD MaxSize);
    // 6. ģ�� ä��(1:1 ä��)
    void WriteFriendChat(ns_world::WorldUser* user, int nChannelID,
        ns_world::WorldUser* friend_user, TCHAR* szChatMsg, DWORD MaxSize);
    // 7. ä�� ä��(ä�ù� ����)
    void WriteChannelChat(ns_world::WorldUser* user, int nChannelID,
        TCHAR* ptszChatMsg, DWORD dwMaxSize);

    //////////////////////////////////////////////////////////////////////////
    // ���� �α�
    // 1. ���� �ζ� ���� �α�(�α�Ÿ��, �ɸ��� ������, ���ӷζ� ȸ��, ��÷���, ��÷�ݾ�)
    void LogSettleHeimLotto(int log_type,
        ns_world::WorldUser* user, CHARGUID char_guid,
        WORD draw_num, BYTE winner_grade, MONEY amount_cost);
    //////////////////////////////////////////////////////////////////////////
    // �׼� �α�
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