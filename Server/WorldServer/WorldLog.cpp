#include "StdAfx.h"
#include ".\worldlog.h"
#include "User.h"
#include "WorldGuild.h"

WorldLog::WorldLog(void)
{
	for(int i = 0; i < (CHAT_CHANNEL - CHAT_NORMAL) + 1; i++)
	{
		m_bLogAble[i] = FALSE;
	}
}

WorldLog::~WorldLog(void)
{
}

VOID WorldLog::SetLogAble()
{
	// 설정 파일에서 Log 여부를 읽는다
	m_bLogAble[0] = GetPrivateProfileInt( "CHATLOG", "CHAT_NORMAL", 0, ".\\WorldServer.ini" );
	m_bLogAble[1] = GetPrivateProfileInt( "CHATLOG", "CHAT_SHOUT", 0, ".\\WorldServer.ini" );
	m_bLogAble[2] = GetPrivateProfileInt( "CHATLOG", "CHAT_PARTY", 0, ".\\WorldServer.ini" );
	m_bLogAble[3] = GetPrivateProfileInt( "CHATLOG", "CHAT_GUILD", 0, ".\\WorldServer.ini" );
	m_bLogAble[4] = GetPrivateProfileInt( "CHATLOG", "CHAT_PRIVATE", 0, ".\\WorldServer.ini" );
	m_bLogAble[5] = GetPrivateProfileInt( "CHATLOG", "CHAT_FRIEND", 0, ".\\WorldServer.ini" );
	m_bLogAble[6] = GetPrivateProfileInt( "CHATLOG", "CHAT_CHANNEL", 0, ".\\WorldServer.ini" );
}

//////////////////////////////////////////////////////////////////////////
// 채팅 로그 
// 1. 일반 채팅
void WorldLog::WriteNormalChat( User* pUser, int nChannelID, TCHAR* pszMsg, DWORD MaxSize )
{
	if( !IsRun() || !IsLogAble( CHAT_NORMAL ) )		return;
	if( !pUser )	return;
	
	m_ChatData.Init();

	m_ChatData.SetLogType( CHAT_NORMAL );
	m_ChatData.SetAccountID( pUser->GetAccountID(), ID_MAX_LEN );
	m_ChatData.SetCharName( pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	m_ChatData.SetUserIP( pUser->GetAuthIP(), MAX_IPADDRESS_SIZE );

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID() );

	m_ChatData.SetContext( pszMsg , MaxSize);
	
	m_LogMng.Serialize( m_ChatData );
}


// 2. 외치기
void WorldLog::WriteShoutChat( User* pUser, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize)
{
	if( !IsRun() || !IsLogAble( CHAT_SHOUT ) )		return;
	if( !pUser )	return;

	m_ChatData.Init();

	m_ChatData.SetLogType( CHAT_SHOUT );
	m_ChatData.SetAccountID( pUser->GetAccountID(), ID_MAX_LEN );
	m_ChatData.SetCharName( pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	m_ChatData.SetUserIP( pUser->GetAuthIP(), MAX_IPADDRESS_SIZE );

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID() );

	m_ChatData.SetContext( szChatMsg , MaxSize );

	m_LogMng.Serialize( m_ChatData );
}


// 3. 파티 채팅
void WorldLog::WritePartyChat( User* pUser, int nChannelID, TCHAR* szChatMsg, DWORD MaxSize )
{
	if( !IsRun() || !IsLogAble( CHAT_PARTY ) )		return;
	if( !pUser )	return;

	m_ChatData.Init();

	m_ChatData.SetLogType( CHAT_PARTY );
	m_ChatData.SetAccountID( pUser->GetAccountID(), ID_MAX_LEN );
	m_ChatData.SetCharName( pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	m_ChatData.SetUserIP( pUser->GetAuthIP(), MAX_IPADDRESS_SIZE );

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID() );

	m_ChatData.SetContext( szChatMsg, MaxSize );

	m_LogMng.Serialize( m_ChatData );
}

// 4. 길드 채팅
void WorldLog::WriteGuildChat( User* pUser, int nChannelID, WorldGuild* pGuild, TCHAR* szChatMsg, DWORD MaxSize)
{
	if( !IsRun() || !IsLogAble( CHAT_GUILD ) )		return;

	m_ChatData.Init();

	m_ChatData.SetLogType( CHAT_GUILD );
	m_ChatData.SetAccountID( pUser->GetAccountID(), ID_MAX_LEN );
	m_ChatData.SetCharName( pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	m_ChatData.SetUserIP( pUser->GetAuthIP(), MAX_IPADDRESS_SIZE );

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID() );
	m_ChatData.SetListenUserGuid( pGuild->GetGuildGuid() );	

	m_ChatData.SetListenCharName( const_cast<TCHAR*>( pGuild->GetGuildName() ), MAX_GUILDNAME_LENGTH ) ;
	m_ChatData.SetContext( szChatMsg, MaxSize );

	m_LogMng.Serialize( m_ChatData );

}

// 5. 귓속말
void WorldLog::WriteWhisperChat( User* pUser, int nChannelID, User* pTargetUser, TCHAR* szChatMsg, DWORD MaxSize )
{
	if( !IsRun() || !IsLogAble( CHAT_PRIVATE ) )		return;

	m_ChatData.Init();

	m_ChatData.SetLogType( CHAT_PRIVATE );
	m_ChatData.SetAccountID( pUser->GetAccountID(), ID_MAX_LEN );
	m_ChatData.SetCharName( pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	m_ChatData.SetUserIP( pUser->GetAuthIP(), MAX_IPADDRESS_SIZE );

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID() );
	m_ChatData.SetListenUserGuid( pTargetUser->GetGUID() );	

	m_ChatData.SetListenCharName( const_cast<TCHAR*>( pTargetUser->GetCharName().c_str()), MAX_CHARNAME_LENGTH  );
	m_ChatData.SetContext( szChatMsg, MaxSize );

	m_LogMng.Serialize( m_ChatData );
}

// 6. 친구 채팅
void WorldLog::WriteFriendChat( User* pUser, int nChannelID, User* pFriend, TCHAR* szChatMsg, DWORD MaxSize )
{
	if( !IsRun() || !IsLogAble( CHAT_FRIEND ) )		return;

	m_ChatData.Init();

	m_ChatData.SetLogType( CHAT_FRIEND );
	m_ChatData.SetAccountID( pUser->GetAccountID(), ID_MAX_LEN );
	m_ChatData.SetCharName( pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	m_ChatData.SetUserIP( pUser->GetAuthIP(), MAX_IPADDRESS_SIZE );

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID()  );
	m_ChatData.SetListenUserGuid( pFriend->GetGUID() );	

	m_ChatData.SetListenCharName( const_cast<TCHAR*>( pFriend->GetCharName().c_str() ), MAX_CHARNAME_LENGTH );
	m_ChatData.SetContext( szChatMsg, MaxSize );

	m_LogMng.Serialize( m_ChatData );
}

void WorldLog::WriteChannelChat(User* pUser, int nChannelID, TCHAR* ptszChatMsg, DWORD dwMaxSize)
{
	if(!IsRun() || !IsLogAble(CHAT_CHANNEL))	return;

	m_ChatData.Init();

	m_ChatData.SetLogType(CHAT_CHANNEL);
	m_ChatData.SetAccountID(pUser->GetAccountID(), ID_MAX_LEN);
	m_ChatData.SetCharName(pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH);
	m_ChatData.SetUserIP(pUser->GetAuthIP(), MAX_IPADDRESS_SIZE);

	m_ChatData.SetMapCode( pUser->GetMapCode() );
	m_ChatData.SetChannel( nChannelID );
	m_ChatData.SetRoomNo( pUser->GetZoneKey() );

	m_ChatData.SetUserGuid( pUser->GetGUID()  );

	m_ChatData.SetContext( ptszChatMsg, dwMaxSize );

	m_LogMng.Serialize( m_ChatData );
}

//////////////////////////////////////////////////////////////////////////
// 하임 로그

//하임로또 정산 로그
void WorldLog::LogSettleHeimLotto(int log_type, User* user, CHARGUID char_guid, 
                                  WORD draw_num, BYTE winner_grade, MONEY amount_cost)
{
    if (!IsRun()) {
        return;
    }
    assert(log_type >= MONEY_TOTAL && log_type < MONEY_MAX);

    m_MoneyData.Init();
    m_MoneyData.SetLogType(log_type);

    if (user != NULL)
    {
        m_MoneyData.SetAccountID(user->GetAccountID(), ID_MAX_LEN);
        m_MoneyData.SetCharName(user->GetCharName().c_str(), MAX_CHARNAME_LENGTH);
        m_MoneyData.SetUserIP(user->GetAuthIP(), MAX_IPADDRESS_SIZE );
    }

    if (log_type == MONEY_HEIM_LOTTO_SETTLE_RESULT)
    {
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "CharId:%d,Round:%d,Grade:%d,Money:%I64u",
                   char_guid, draw_num, winner_grade, amount_cost);
        m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
        m_MoneyData.AddAddInfo(m_pTmpBuffer);
    }
    m_LogMng.Serialize(m_MoneyData);
}

//////////////////////////////////////////////////////////////////////////
// 액션 로그
// implemented by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
void WorldLog::LogAction(int log_type, User* user, const TCHAR* char_name, const TCHAR* additional_info)
{
    if (!IsRun()) {
        return;
    }
    assert(log_type >= ACT_CREATE_CHAR && log_type < ACT_MAX);

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);

    if (user != NULL)
    {
        m_ActionData.SetAccountID(user->GetAccountID(), ID_MAX_LEN);
        m_ActionData.SetUserIP(user->GetAuthIP(), MAX_IPADDRESS_SIZE);
        m_ActionData.SetCharName(user->GetCharName().c_str(), MAX_CHARNAME_LENGTH);
        m_ActionData.SetMapCode(user->GetMapCode());
    }
    else if (char_name != NULL)
    {
        m_ActionData.SetCharName(char_name, MAX_CHARNAME_LENGTH);
    }

    if (additional_info != NULL) {
        m_ActionData.AddAddInfo(const_cast<TCHAR*>(additional_info));
    }
    m_LogMng.Serialize(m_ActionData);
}

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
void WorldLog::LogNotification( NotificationInfo& notification )
{
    if (!IsRun()) {
        return;
    }

    m_ActionData.Init();
    m_ActionData.SetLogType(ACT_NOTIFICATION_MESSAGE);
    m_ActionData.SetCharName(notification.m_char_name, MAX_CHARNAME_LENGTH);
    m_ActionData.SetMapCode(notification.m_map_code);

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Type:%d, ItemCode:%d(%d) Time:%d", 
        notification.m_notification_type, notification.m_item_code,
        notification.m_optione_value1, notification.m_time);
    m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize(m_ActionData);
}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
