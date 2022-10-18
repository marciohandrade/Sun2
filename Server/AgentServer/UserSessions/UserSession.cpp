#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
// <SHARED>
#include <PacketControl/PacketControl.h>

#include <ServerOptionParserEx.h>

//////////////////////////////////////////////////////////////////////////
// <AGENT SERVER>
#include "UserSession.h"
#include "UserManager.h"
#include <PacketHandler/PacketHandler.h>
#include <ServerSessions/ServerSessionEx.h>
#include <ServerSessions/AuthAgentServerSession.h>
#include <ServerSessions/ServerSessionManager.h>

#include <AgentServer.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UserSession::UserSession() :
				m_bDisconnectProcessing( FALSE ),
				m_eDisconnectReason( RC::RC_DISCONNECT_NORMAL ),
				m_dwSessionIndex( 0 ),
				//m_dwServerSessionIndex( 0 ),
				//m_pLinkedServer( NULL ),
				m_dwReservedValue( 0 ),
                m_AuthUserType(AUTH_USER_DEFAULT),
				m_PCRoomSts( 0 ),
				m_bySts( 0 )
{
    m_dwPremiumService = 0;
}

UserSession::~UserSession()
{
}

VOID	UserSession::Init()
{
	m_bDisconnectProcessing	= FALSE;
	m_eDisconnectReason		= RC::RC_DISCONNECT_NORMAL;

    ZeroMemory(&user_account_info_, sizeof(user_account_info_));

	m_dwReservedValue   = 0;
    m_AuthUserType      = AUTH_USER_DEFAULT;
	m_PCRoomSts			= 0;
	m_bySts				= 0;
}

VOID	UserSession::Release()
{
}

//////////////////////////////////////////////////////////////////////////
// <NETWORK OBJECT FORWARDING CONTROL>
BOOL	UserSession::SendToDBPServer( MSG_BASE* pMsg, WORD wSize )
{
	pMsg->m_dwKey = GetUserKey();

	SHOW_PACKET_STREAM( pMsg, wSize, "A2D" );

	return AgentServer::GetInstance()->SendToDBPServer( pMsg, wSize );
}

BOOL	UserSession::SendToWorldServer( MSG_BASE* pMsg, WORD wSize )
{
	pMsg->m_dwKey = GetUserKey();

	SHOW_PACKET_STREAM( pMsg, wSize, "A2W" );

	return AgentServer::GetInstance()->SendToWorldServer( pMsg, wSize );
}

//////////////////////////////////////////////////////////////////////////
// <NETWORK OBJECT><INHERITANCE>

VOID	UserSession::OnLogString( CHAR* pszLog )
{
	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_CONNECTION,
		_T("[UserSession:T%u] %s"), GetUserType(), pszLog );
}


VOID	UserSession::OnDisconnect()
{
	UserManager::Instance()->RemoveUser( GetUserKey() );
}

VOID	UserSession::OnAccept( DWORD dwNetworkIndex )
{
	SetSessionIndex( dwNetworkIndex );

	// Don't add UserManager
}

VOID	UserSession::OnDisconnectInfoToAuth()
{
	AgentServer* pAgentServer = AgentServer::GetInstance();
	AuthAgentServerSession * pAuthAgent = pAgentServer->SessionAuthAgentServer();
	ASSERT( pAuthAgent != NULL );

	if( pAuthAgent && pAuthAgent->IsConnected() )
	{
		MSG_GA_AUTH_PC_LOGOUT_CMD msgLOGOUT;
		msgLOGOUT.m_dwAuthUserID = GetAuthID();
		msgLOGOUT.m_byLogoutType = GetLogoutType();
        CHECK_ID_LENGTH_BUFFER(msgLOGOUT.m_szAccountID);
		strncpy(msgLOGOUT.m_szAccountID, GetUserID(), _countof(msgLOGOUT.m_szAccountID));
        msgLOGOUT.m_szAccountID[_countof(msgLOGOUT.m_szAccountID) - 1] = '\0';

		if( GetCharName() )
			strncpy(msgLOGOUT.m_szPCName, GetCharName(), _countof(msgLOGOUT.m_szPCName));
		else
			msgLOGOUT.m_szPCName[0] = '\0';

		msgLOGOUT.m_wPCLevel = GetCharLV();
		msgLOGOUT.m_byPCClass = GetCharClass();

		pAuthAgent->Send( (BYTE*)&msgLOGOUT, sizeof(msgLOGOUT) );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[UserSession::OnDisconnectToAuth] AuthAgent와 연결이 끊어져 있습니다!!!! 로그인 불가!!!!!" );
	}

	//로그아웃 로그
	GAMELOG->UserLogout( this );
}

BOOL	UserSession::SendPacket( MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking )
{
    __UNUSED(bUsePacking); // User object only used.
	return Send( (BYTE*)pMsg, wSize );
}

//////////////////////////////////////////////////////////////////////////
// <NETWORK OBJECT DISPOSING CONTROL>

VOID	UserSession::DisconnectUser( BOOL bGracefulDisconnect )
{
	DisconnectProcessing() = TRUE;

	MSG_CG_CONNECTION_DISCONNECT_CMD msgCMD;
	msgCMD.m_dwErrorCode = (DWORD)GetDisconnectCode();
	SendPacket( &msgCMD, sizeof(msgCMD) );

	// Game Server에서 뒤늦게 종료 메시지가 오게 될 것이다.
	// 하지만 OnDisconnect이 먼저 이뤄지게 되므로...
	// 여기에 대한 적절한 해결책을 제시해 보자.
	NetworkObject::Disconnect( bGracefulDisconnect );

	// 로그의 순서를 바꾼다.
    SUNCONNLOG( eCRITICAL_LOG, _T("ForcedDisconnectUser[Reason:(%u)][UID:(%s)][IP:(%s)][GUID:(%d)]"), 
        GetDisconnectCode(), GetUserID(), GetClientIP(), GetUserGUID() );
	SetDisconnectCode( RC::RC_DISCONNECT_UNKNOWN );
}


eAUTH_LOGOUT_REASON	// Return Type
UserSession::GetAuthLogoutType()
{
	switch( GetDisconnectCode() )
	{
	case RC::RC_DISCONNECT_NORMAL:
		return AUTH_LOGOUT_NORMAL;
	//	return AUTH_LOGOUT_USER_NOT_FOUND;
	//	return AUTH_LOGOUT_HACKING_USER;
	case RC::RC_DISCONNECT_INCORRECT_AUTH_KEY:
		return AUTH_LOGOUT_WRONG_AUTHKEY;
	case RC::RC_DISCONNECT_INCORRECT_VERSION:
		return AUTH_LOGOUT_WRONG_VERSION;
	default:
		return AUTH_LOGOUT_ETC;
	}
}

eLOG_SESSION_LOGOUT_TYPE	// Return Type
UserSession::GetLogoutType()
{
	switch( GetDisconnectCode() )
	{
	case RC::RC_DISCONNECT_NORMAL:
		SUNLOG( eDEV_LOG, _T("[UserSession::GetLogoutType] RC_DISCONNECT_NORMAL") );
		return eLOGOUT_NORMAL;
	//	return eLOGOUT_HACK;
	//	return eLOGOUT_EXPIRED;
	default:
		return eLOGOUT_ABNORMAL;
	}
}

#ifdef _NA_008765_20160405_DISCONNECTION_REASON_LOG
RC::eCONNECTION_RESULT
UserSession::GetLogoutType2()
{
	return GetDisconnectCode();
}
#endif //(_NA_008765_20160405_DISCONNECTION_REASON_LOG)

BOOL UserSession::CanApplyPCBangBenefit( BOOL bNationCheck )
{
	if( bNationCheck )
	{
		if( ServerOptionParserEx::Instance()->GetServerOption().m_byPCBangApplyBenefit == FALSE )
			return FALSE;
	}

#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    if (GetPremiumService() > 0) {
        return TRUE;
    }
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    DWORD pcbang_id = GetReservedValue();
    return (m_PCRoomSts == 0 && pcbang_id);
#else
	return (BOOL)(!m_PCRoomSts && m_dwReservedValue);
#endif
}

//!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
void UserSession::SetPremiumService( DWORD premiumservice )
{
    m_dwPremiumService = premiumservice;
}

DWORD UserSession::GetPremiumService()
{
    return m_dwPremiumService;
}
//~! _NA_006413_20130201_PREMIUMSERVICE_ITEM














