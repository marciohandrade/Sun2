#include "StdAfx.h"
#include ".\agentlog.h"
#include <AgentServer.h>
#include <time.h>

AgentLog::AgentLog()
{
	m_pTmpBuffer[0] = '\0';
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
}

//////////////////////////////////////////////////////////////////////////
// 세션 로그 

//
//void AgentLog::UserLoginFail( UserSession * pTempUser )
//{
//	if( !IsRun() || !pTempUser )		return;
//
//	m_SessionData.Init();
//	m_SessionData.SetBaseData( CONN_LOGIN_FAILD, pTempUser->GetUserID(), NULL, pTempUser->GetIP()/*모듈의IP사용*/ );	
//	m_SessionData.SetLogoutType(pTempUser->GetLogoutType());
//
//	m_LogMng.Serialize( m_SessionData );
//
//}
// 로그인 
void AgentLog::SetRoomKey( SessionData & rSessionData, UserSession * pUserSession )
{
	if( pUserSession->GetUserType() == UT_TEMP_USER ) return;
	User * pUser = (User*)pUserSession;
	if( 0 != pUser->GetZoneKey() )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Room:%u", pUser->GetZoneKey() );
		m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
		rSessionData.AddAddInfo(m_pTmpBuffer);
	}
}
void AgentLog::UserLogin( UserSession * pUser )
{
	if( !IsRun() || !pUser )		return;

	m_SessionData.Init();

	m_SessionData.SetLogType( SESSION_LOGIN );
    m_SessionData.SetAccountID( pUser->GetUserID(), ID_MAX_LEN );
	m_SessionData.SetCharName( pUser->GetCharName(), MAX_CHARNAME_LENGTH );
	m_SessionData.SetUserIP( pUser->GetClientIP(), MAX_IPADDRESS_SIZE );
	m_SessionData.SetLoginTime(pUser->GetLoginTime(), MAX_TIMEDATA_SIZE);

	m_LogMng.Serialize( m_SessionData );
}

#ifdef _NA_0_20110915_CLIENT_MAC_ADDRESS
void AgentLog::UserLogin( UserSession * pUser, const char* mac_address )
{
    if( !IsRun() || !pUser )		return;

    m_SessionData.Init();

    m_SessionData.SetLogType( SESSION_LOGIN );
    m_SessionData.SetAccountID( pUser->GetUserID(), ID_MAX_LEN );
    m_SessionData.SetCharName( pUser->GetCharName(), MAX_CHARNAME_LENGTH );
    m_SessionData.SetUserIP( pUser->GetClientIP(), MAX_IPADDRESS_SIZE );
    m_SessionData.SetLoginTime(pUser->GetLoginTime(), MAX_TIMEDATA_SIZE);

    char temp[MAX_PATH];
    sprintf(temp, "MAC : %s", mac_address ? mac_address : "");
    m_SessionData.AddAddInfo(temp);

    m_LogMng.Serialize( m_SessionData );
}
#endif

// 로그아웃
void AgentLog::UserLogout(  UserSession * pUserSession )
{
	if( !IsRun() || !pUserSession )		return;

	m_SessionData.Init();

	m_SessionData.SetLogType( SESSION_LOGOUT );
	m_SessionData.SetAccountID( pUserSession->GetUserID(), ID_MAX_LEN );
	m_SessionData.SetCharName( pUserSession->GetCharName(), MAX_CHARNAME_LENGTH );
	m_SessionData.SetUserIP( pUserSession->GetClientIP(), MAX_IPADDRESS_SIZE );

#ifdef _NA_008765_20160405_DISCONNECTION_REASON_LOG
	RC::eCONNECTION_RESULT LogOutType = pUserSession->GetLogoutType2();
	if(LogOutType == RC::RC_DISCONNECT_NORMAL)
	{
		// 정상적일 경우, 세션에 대한 로그아웃 타입이 있기때문에 기존 형식을 따름
		m_SessionData.SetLogoutType(eLOGOUT_NORMAL);
	}
	else
	{
		// eLOGOUT_NORMAL 이외에는 모두 비정상적 접속 끊김이다...
		// 비 정상적일 경우
		m_SessionData.SetLogoutType(LogOutType);
	}
#else
	m_SessionData.SetLogoutType(pUserSession->GetLogoutType());
#endif //(_NA_008765_20160405_DISCONNECTION_REASON_LOG)

	m_SessionData.SetLoginTime(pUserSession->GetLoginTime(),  MAX_TIMEDATA_SIZE);

	time_t cur_time;
	util::TimeSync::time( &cur_time );
	DWORD dwUsedTime = (DWORD)abs(difftime(cur_time, pUserSession->GetLoginTime_t())/60);
	m_SessionData.SetUsedTime( dwUsedTime );

	this->SetRoomKey(m_SessionData, pUserSession);

	m_LogMng.Serialize( m_SessionData );
}
// 4. 캐릭터 접속
void AgentLog::ConnectChar( User * pUser )
{
	if( !IsRun() || !pUser )		return;

	SUNLOG( eFULL_LOG, "[%s]캐릭터접속", pUser->GetCharName() );
	m_SessionData.Init();

	m_SessionData.SetLogType( SESSION_CHAR_CONNECT );
	m_SessionData.SetAccountID( pUser->GetUserID(), ID_MAX_LEN );
	m_SessionData.SetCharName( pUser->GetCharName(), MAX_CHARNAME_LENGTH );
	m_SessionData.SetUserIP( pUser->GetClientIP(), MAX_IPADDRESS_SIZE );
	m_SessionData.SetLoginTime(pUser->GetLoginTime(), MAX_TIMEDATA_SIZE);

	m_LogMng.Serialize( m_SessionData );
}

// 4. 캐릭터 접속 종료 : 캐릭터 선택창으로 돌아 올 때
void AgentLog::DisconnectChar( User * pUser )
{
	if( !IsRun() || !pUser )		return;

	SUNLOG( eFULL_LOG, "[%s]캐릭터접속해제", pUser->GetCharName() );

	m_SessionData.Init();

	m_SessionData.SetLogType( SESSION_CHAR_DISCONNECT );
	m_SessionData.SetAccountID( pUser->GetUserID(), ID_MAX_LEN );
	m_SessionData.SetCharName( pUser->GetCharName(), MAX_CHARNAME_LENGTH );
	m_SessionData.SetUserIP( pUser->GetClientIP(), MAX_IPADDRESS_SIZE );
	m_SessionData.SetLoginTime(pUser->GetLoginTime(), MAX_TIMEDATA_SIZE);

	if( 0 != pUser->GetCharLoginTime_t() )
	{
		time_t cur_time;
		util::TimeSync::time( &cur_time );
		DWORD dwUsedTime = (DWORD)abs(difftime(cur_time, pUser->GetCharLoginTime_t())/60);
		m_SessionData.SetUsedTime( dwUsedTime );
	}

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, _T("Class:%u"), pUser->GetCharClass());
    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_SessionData.AddAddInfo(m_pTmpBuffer);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

	m_LogMng.Serialize( m_SessionData );
}

#ifdef _NA_20100802_SPEED_HACK_DETECTION
// 스피드핵 검사 로그
void AgentLog::LogSpeedHackSuspect(User* user, TCHAR* additional_info)
{
    if(!IsRun() || !user)	{
        return;
    }

    SUNLOG( eCRITICAL_LOG, "[%s][%s] Speedhack suspected : %s", \
        user->GetUserID(), user->GetCharName(), additional_info );

    m_SessionData.Init();

    m_SessionData.SetLogType( SESSION_SPEED_HACK_SUSPECT );
    m_SessionData.SetAccountID( user->GetUserID(), ID_MAX_LEN );
    m_SessionData.SetCharName( user->GetCharName(), MAX_CHARNAME_LENGTH );
    m_SessionData.SetUserIP( user->GetClientIP(), MAX_IPADDRESS_SIZE );
    m_SessionData.SetLoginTime(user->GetLoginTime(), MAX_TIMEDATA_SIZE);

	m_SessionData.AddAddInfo(additional_info);

    m_LogMng.Serialize( m_SessionData );
}

// 스피드핵 검사 판정에 의한 강제 접속 종료
void AgentLog::LogSpeedHackSuspectDisconnection(User* user)
{
    if(!IsRun() || !user)	{
        return;
    }

    SUNLOG( eCRITICAL_LOG, "[%s][%s] Speedhack suspected : Forced disconnection!", \
        user->GetUserID(), user->GetCharName() );

    m_SessionData.Init();

    m_SessionData.SetLogType( SESSION_SPEED_HASK_SUSPECT_DISCONN );
    m_SessionData.SetAccountID( user->GetUserID(), ID_MAX_LEN );
    m_SessionData.SetCharName( user->GetCharName(), MAX_CHARNAME_LENGTH );
    m_SessionData.SetUserIP( user->GetClientIP(), MAX_IPADDRESS_SIZE );
    m_SessionData.SetLoginTime(user->GetLoginTime(), MAX_TIMEDATA_SIZE);

    if( 0 != user->GetCharLoginTime_t() )
    {
        time_t cur_time;
        util::TimeSync::time( &cur_time );
        DWORD dwUsedTime = (DWORD)abs(difftime(cur_time, user->GetCharLoginTime_t())/60);
        m_SessionData.SetUsedTime( dwUsedTime );
    }

    m_LogMng.Serialize( m_SessionData );
}
#endif // _NA_20100802_SPEED_HACK_DETECTION

//////////////////////////////////////////////////////////////////////////
// 액션로그
//1. 캐릭터 생성 / 삭제
void AgentLog::CreateCharacter(
    const bool is_create, User* user, const TCHAR* char_name, const TCHAR* add_info)
{
    if (IsRun() == FALSE || user == NULL)
    {
        return;
    }
	
    const int log_type = is_create ? ACT_CREATE_CHAR : ACT_DELETE_CHAR;
	
    m_ActionData.Init();
	m_ActionData.SetLogType(log_type);
	m_ActionData.SetAccountID(user->GetUserID(), ID_MAX_LEN);
	m_ActionData.SetCharName(char_name, MAX_CHARNAME_LENGTH);
	m_ActionData.SetUserIP(user->GetClientIP(), MAX_IPADDRESS_SIZE);

    if (add_info != NULL)
    {
        m_ActionData.AddAddInfo(const_cast<TCHAR*>(add_info));
    }

	m_LogMng.Serialize(m_ActionData);
}

// 2. 마을로 이동
void AgentLog::MoveVillage( User* pUser, WORD wFromMapCode, WORD wToMapCode, BOOL bRoom )
{
	if( !IsRun() || !pUser )	return;

	
	int LogType = 0;
	if( bRoom == TRUE )			LogType = ACT_MOVE_ROOM;
	else						LogType = ACT_MOVE_FIELD;

	m_ActionData.Init();

	m_ActionData.SetLogType( LogType );
	m_ActionData.SetAccountID( pUser->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pUser->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pUser->GetClientIP(), MAX_IPADDRESS_SIZE );

	m_ActionData.SetMapCode( (short)pUser->GetSelectedCharMapCode() );

	//이동하려는 곳의 맵코드..
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "BMap:%u, AMap:%u", wFromMapCode, wToMapCode );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ActionData );
}

// 리벤지 대상으로 워프
void AgentLog::TrackRevenge( User* pMoveUser, User* pTrackUser, WzVector &vCurPos, WzVector &vDestPos )
{
	if( !IsRun() || !pMoveUser || !pTrackUser )	return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_TRACK_REVENGE );
	m_ActionData.SetAccountID( pMoveUser->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pMoveUser->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pMoveUser->GetClientIP(), MAX_IPADDRESS_SIZE );

	m_ActionData.SetMapCode( (short)pMoveUser->GetSelectedCharMapCode() );
	m_ActionData.SetMapX( (short)vCurPos.x );
	m_ActionData.SetMapY( (short)vCurPos.y );
	m_ActionData.SetMapZ( (short)vCurPos.z );

	// 추적대상의 정보 및 좌표
/*
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",TrackUserID:%s,CharName:%s,ToMapCode:%u,X:%0.3f,Y:%0.3f,Z:%0.3f", 
		pTrackUser->GetUserID(), pTrackUser->GetSelectedCharName(), pTrackUser->GetSelectedCharMapCode(), vDestPos.x, vDestPos.y, vDestPos.z );
		*/
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",TrkUsrID:%s,ChrName:%s,ToMapCd:%u,X:%0.3f,Y:%0.3f,Z:%0.3f", 
		pTrackUser->GetUserID(), pTrackUser->GetSelectedCharName(), pTrackUser->GetSelectedCharMapCode(), vDestPos.x, vDestPos.y, vDestPos.z );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ActionData );
}

// 좌표이동 아이템을 이용한 포탈
void AgentLog::Portal( User* pMoveUser, MAPCODE ToMapCode, WzVector &vCurPos, WzVector &vDestPos )
{
	if( !IsRun() || !pMoveUser )	return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_PORTAL_BY_ITEM );
	m_ActionData.SetAccountID( pMoveUser->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pMoveUser->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pMoveUser->GetClientIP(), MAX_IPADDRESS_SIZE );

	m_ActionData.SetMapCode( (short)pMoveUser->GetSelectedCharMapCode() );
	m_ActionData.SetMapX( (short)vCurPos.x );
	m_ActionData.SetMapY( (short)vCurPos.y );
	m_ActionData.SetMapZ( (short)vCurPos.z );

	// 추적대상의 정보 및 좌표
//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",ToMapCode:%u,X:%0.3f,Y:%0.3f,Z:%0.3f", ToMapCode, vDestPos.x, vDestPos.y, vDestPos.z );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",ToMapCd:%u,X:%0.3f,Y:%0.3f,Z:%0.3f", ToMapCode, vDestPos.x, vDestPos.y, vDestPos.z );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ActionData );
}

// GM 캐릭터 정보
void AgentLog::ReqCharInfo( User* pGM, MSG_CG_GM_CHARINFO_ACK& msg )
{
	if( !IsRun() || !pGM )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_CHARACTER_INFO );
	m_AdminData.SetAccountID( pGM->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pGM->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pGM->GetClientIP(), MAX_IPADDRESS_SIZE );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Room:%u,ID:%s,LV:%u,Char:%u,Map:%u,X:%0.3f,Y:%0.3f,Z:%0.3f,Money:%I64u,Guild:%s", 
												msg.m_RoomKey,
												msg.m_pszAccountID, 
												msg.m_wLevel, 
												msg.m_eCharType, 
												msg.m_MapCode,
												msg.m_vCurPos.x,
												msg.m_vCurPos.y,
												msg.m_vCurPos.z,
												msg.m_Money, 
												msg.m_szGuildName );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_AdminData );
}


// GM 접속종료
void AgentLog::DisconnectCmd( User* pGM, User* pFindUser )
{
	if( !IsRun() || !pGM || !pFindUser )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_DISCONNECT );
	m_AdminData.SetAccountID( pGM->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pGM->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pGM->GetClientIP(), MAX_IPADDRESS_SIZE );
	m_AdminData.SetToCharName( pFindUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH);

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Room:%u,ID:%s,LV:%u,Class:%u,Map:%u", 
												pFindUser->GetZoneKey(),
												pFindUser->GetUserID(),
												pFindUser->GetSelectedCharLV(),
												pFindUser->GetSelectedCharClass(),
												pFindUser->GetSelectedCharMapCode()
												);	
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_AdminData );
}

void AgentLog::LogGM( int nLogType, User * pGM, User * pFindUser )
{
	if( !IsRun() || !pGM || !pFindUser )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( nLogType );
	m_AdminData.SetAccountID( pGM->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pGM->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pGM->GetClientIP(), MAX_IPADDRESS_SIZE );

	m_AdminData.SetMapCode( (WORD)pGM->GetSelectedCharMapCode() );

	if( pFindUser )
	{
		m_AdminData.SetToCharName( pFindUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH );

		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Room:%u,ID:%s,LV:%u,Class:%u,Map:%u", 
			pFindUser->GetZoneKey(),
			pFindUser->GetUserID(),
			pFindUser->GetSelectedCharLV(),
			pFindUser->GetSelectedCharClass(),
			pFindUser->GetSelectedCharMapCode()
			);	
		m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
		m_AdminData.AddAddInfo( m_pTmpBuffer );
	}

	m_LogMng.Serialize( m_AdminData );
}

void AgentLog::LogDominationFieldEnterCount(int log_type, User* user, BYTE count, MAPCODE map_code)
{
    if (!IsRun() || !user)		
    {
        return;
    }

    m_ActionData.Init();

    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID(user->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(user->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(user->GetClientIP(), MAX_IPADDRESS_SIZE);
    m_ActionData.SetMapCode(map_code);

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Guild : %u, Map : %u, Count : %d"
               ,user->GetSelectedCharGuildGuid(), map_code, count);

    m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
    m_ActionData.AddAddInfo( m_pTmpBuffer );

    m_LogMng.Serialize( m_ActionData );
}

void AgentLog::WriteCharRename(User* user, SLOTIDX slot_index, const TCHAR* pszName)
{
    if( !IsRun() || !user )
        return;

    m_ActionData.Init();
    m_ActionData.SetAccountID( user->GetUserID(), ID_MAX_LEN );
    m_ActionData.SetCharName( user->GetCharName(slot_index), MAX_CHARNAME_LENGTH );
    m_ActionData.SetUserIP( user->GetClientIP(), MAX_IPADDRESS_SIZE );
    m_ActionData.SetLogType( ACT_CHARACTER_NAME_CHANGE );

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "NewName=%s", pszName);

    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
void AgentLog::LogWzDat( int log_type, const TCHAR* add_info /*= NULL*/ )
{
    if( !IsRun())
        return;

    m_ActionData.Init();
    m_ActionData.SetLogType( log_type );
    m_ActionData.SetAccountID( "WzDat", ID_MAX_LEN );
    m_ActionData.SetCharName( "WzDat", MAX_CHARNAME_LENGTH );

    if (add_info != NULL)
    {
        m_ActionData.AddAddInfo((TCHAR*)add_info);
    }

    m_LogMng.Serialize( m_ActionData );
}
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG