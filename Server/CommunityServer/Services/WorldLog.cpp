#include "WorldServerPch.hxx"
#include "./WorldLog.h"

#include "Objects/WorldUser.h"
#include "Services/Guild/WorldGuild.h"

ns_world::WorldLog* ns_world::world_log = NULL;

ns_world::WorldLog::WorldLog()
{
    ZeroMemory(enabled_log_categories_, sizeof(enabled_log_categories_));
}

ns_world::WorldLog::~WorldLog()
{
}

void ns_world::WorldLog::SetLogAble()
{
    // 설정 파일에서 Log 여부를 읽는다
    enabled_log_categories_[0] = !!GetPrivateProfileInt("CHATLOG", "CHAT_NORMAL", 0, ".\\WorldServer.ini");
    enabled_log_categories_[1] = !!GetPrivateProfileInt("CHATLOG", "CHAT_SHOUT", 0, ".\\WorldServer.ini");
    enabled_log_categories_[2] = !!GetPrivateProfileInt("CHATLOG", "CHAT_PARTY", 0, ".\\WorldServer.ini");
    enabled_log_categories_[3] = !!GetPrivateProfileInt("CHATLOG", "CHAT_GUILD", 0, ".\\WorldServer.ini");
    enabled_log_categories_[4] = !!GetPrivateProfileInt("CHATLOG", "CHAT_PRIVATE", 0, ".\\WorldServer.ini");
    enabled_log_categories_[5] = !!GetPrivateProfileInt("CHATLOG", "CHAT_FRIEND", 0, ".\\WorldServer.ini");
    enabled_log_categories_[6] = !!GetPrivateProfileInt("CHATLOG", "CHAT_CHANNEL", 0, ".\\WorldServer.ini");
}

//////////////////////////////////////////////////////////////////////////
// 채팅 로그 
// 1. 일반 채팅
void ns_world::WorldLog::WriteNormalChat(
    ns_world::WorldUser* user, int nChannelID, TCHAR* pszMsg, DWORD MaxSize)
{
    if (user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_NORMAL)) {
        return;
    };
    
    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_NORMAL);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);

    chat_log_data_.SetContext(pszMsg , MaxSize);
    
    m_LogMng.Serialize(chat_log_data_);
}

// 2. 외치기
void ns_world::WorldLog::WriteShoutChat(
    ns_world::WorldUser* user, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize)
{
    if (user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_SHOUT)) {
        return;
    };

    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_SHOUT);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);

    chat_log_data_.SetContext(szChatMsg , MaxSize);

    m_LogMng.Serialize(chat_log_data_);
}


// 3. 파티 채팅
void ns_world::WorldLog::WritePartyChat(
    ns_world::WorldUser* user, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize)
{
    if (user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_PARTY)) {
        return;
    };

    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_PARTY);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);

    chat_log_data_.SetContext(szChatMsg, MaxSize);

    m_LogMng.Serialize(chat_log_data_);
}

// 4. 길드 채팅
void ns_world::WorldLog::WriteGuildChat(
    ns_world::WorldUser* user, int nChannelID,
    ns_world::WorldGuild* guild, TCHAR* szChatMsg, DWORD MaxSize)
{
    if (user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_GUILD)) {
        return;
    };

    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_GUILD);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);
    chat_log_data_.SetListenUserGuid(guild->GetGuildGuid());   

    chat_log_data_.SetListenCharName(const_cast<TCHAR*>(guild->GetGuildName()), MAX_GUILDNAME_LENGTH);
    chat_log_data_.SetContext(szChatMsg, MaxSize);

    m_LogMng.Serialize(chat_log_data_);

}

// 5. 귓속말
void ns_world::WorldLog::WriteWhisperChat(ns_world::WorldUser* user, int nChannelID,
    ns_world::WorldUser* target_user, TCHAR* szChatMsg, DWORD MaxSize)
{
    if (user == NULL || target_user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_PRIVATE)) {
        return;
    };

    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_PRIVATE);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);
    chat_log_data_.SetListenUserGuid(target_user->GetUserGuid());

    chat_log_data_.SetListenCharName(\
        const_cast<TCHAR*>(target_user->GetCharName()), MAX_CHARNAME_LENGTH);
    chat_log_data_.SetContext(szChatMsg, MaxSize);

    m_LogMng.Serialize(chat_log_data_);
}

// 6. 친구 채팅
void ns_world::WorldLog::WriteFriendChat(ns_world::WorldUser* user, int nChannelID,
    ns_world::WorldUser* friend_user, TCHAR* szChatMsg, DWORD MaxSize)
{
    if (user == NULL || friend_user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_FRIEND)) {
        return;
    };

    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_FRIEND);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);
    chat_log_data_.SetListenUserGuid(friend_user->GetUserGuid());

    chat_log_data_.SetListenCharName(\
        const_cast<TCHAR*>(friend_user->GetCharName()), MAX_CHARNAME_LENGTH);
    chat_log_data_.SetContext(szChatMsg, MaxSize);

    m_LogMng.Serialize(chat_log_data_);
}

void ns_world::WorldLog::WriteChannelChat(
    ns_world::WorldUser* user, int nChannelID, TCHAR* ptszChatMsg, DWORD dwMaxSize)
{
    if (user == NULL) {
        return;
    };
    if (!IsRun() || !IsLogAble(CHAT_CHANNEL)) {
        return;
    };

    chat_log_data_.Init();

    chat_log_data_.SetLogType(CHAT_CHANNEL);
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    chat_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
    chat_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
    chat_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);

    chat_log_data_.SetMapCode(char_info.map_code);
    chat_log_data_.SetChannel(nChannelID);
    chat_log_data_.SetRoomNo(char_info.zone_key);

    chat_log_data_.SetUserGuid(user_info.user_guid);

    chat_log_data_.SetContext(ptszChatMsg, dwMaxSize);

    m_LogMng.Serialize(chat_log_data_);
}

//////////////////////////////////////////////////////////////////////////
// 하임 로그

//하임로또 정산 로그
void ns_world::WorldLog::LogSettleHeimLotto(
    int log_type, ns_world::WorldUser* user, CHARGUID char_guid,
    WORD draw_num, BYTE winner_grade, MONEY amount_cost)
{
    if (!IsRun()) {
        return;
    }
    assert(log_type >= MONEY_TOTAL && log_type < MONEY_MAX);

    money_log_data_.Init();
    money_log_data_.SetLogType(log_type);

    if (user != NULL)
    {
        const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
        const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
        money_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
        money_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
        money_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);
    }

    if (log_type == MONEY_HEIM_LOTTO_SETTLE_RESULT)
    {
        _sntprintf(temp_buffer_, MAX_ADDINFO_SIZE, "CharId:%d,Round:%d,Grade:%d,Money:%I64u",
                   char_guid, draw_num, winner_grade, amount_cost);
        temp_buffer_[_countof(temp_buffer_) - 1] = '\0';
        money_log_data_.AddAddInfo(temp_buffer_);
    }
    m_LogMng.Serialize(money_log_data_);
}

//////////////////////////////////////////////////////////////////////////
// 액션 로그
// implemented by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
void ns_world::WorldLog::LogAction(
    int log_type, ns_world::WorldUser* user, const TCHAR* char_name, const TCHAR* additional_info)
{
    if (!IsRun()) {
        return;
    }
    assert(log_type >= ACT_CREATE_CHAR && log_type < ACT_MAX);

    action_log_data_.Init();
    action_log_data_.SetLogType(log_type);

    if (user != NULL)
    {
        const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
        const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
        action_log_data_.SetAccountID(user_info.account_id, ID_MAX_LEN);
        action_log_data_.SetUserIP(user_info.ip_address, MAX_IPADDRESS_SIZE);
        action_log_data_.SetCharName(char_info.char_name, MAX_CHARNAME_LENGTH);
        action_log_data_.SetMapCode(char_info.map_code);
    }
    else if (char_name != NULL)
    {
        action_log_data_.SetCharName(char_name, MAX_CHARNAME_LENGTH);
    }

    if (additional_info != NULL) {
        action_log_data_.AddAddInfo(const_cast<TCHAR*>(additional_info));
    }
    m_LogMng.Serialize(action_log_data_);
}
