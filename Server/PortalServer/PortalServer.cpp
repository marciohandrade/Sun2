#include "stdafx.h"
#include "PortalServer.h"
#include <ServerSession.h>
#include <ServerBuildVersion.h>
#include "PortalScriptParserManager.h"

PortalServer g_PortalServer;
NetworkObject *	PortalServer::DoAllocServerSession()
{
	return (NetworkObject *)g_PortalServer.AllocServerSession(TEMP_SERVER);
}
VOID PortalServer::DoFreeServerSession( NetworkObject * pNetObject )
{
	g_PortalServer.FreeServerSession((ServerSession*)pNetObject);
}

PortalServer::PortalServer()
:	m_pDBProxySession(NULL)
,	m_pMasterSession(NULL)
,	m_WEEKDAY_NUMBER_OF_RANKING_PERIOD(0)
{
	SetPhaseStatus( PHASE_UNKNOWN );
	m_pPortalScriptParserManager = NULL;
}

BOOL PortalServer::SendDBProxy( MSG_BASE * pMsg, WORD wSize )
{
	return m_pDBProxySession->Send( (BYTE*)pMsg, wSize );
}

BOOL PortalServer::SendMaster( MSG_BASE * pMsg, WORD wSize )
{
	return m_pMasterSession->Send( (BYTE*)pMsg, wSize );
}

BOOL PortalServer::IsDBProxyConnected()
{
	if( m_pDBProxySession && m_pDBProxySession->IsConnected() )
		return TRUE;
	return FALSE;
}

BOOL PortalServer::IsMasterConnected()
{
	if( m_pMasterSession && m_pMasterSession->IsConnected() )
		return TRUE;
	return FALSE;
}

ServerSession * PortalServer::AllocServerSession( eSERVER_TYPE type )
{
	ServerSession * pServerSession = NULL;
	switch( type )
	{
	case FIELD_SERVER:
	case BATTLE_SERVER:
	case GAME_SERVER:
		pServerSession = GameServerSession::ALLOC(); break;
	case TEMP_SERVER:	pServerSession = TempServerSession::ALLOC(); break;
	case AGENT_SERVER:	pServerSession = AgentServerSession::ALLOC(); break;
	case MASTER_SERVER:	
		{
			if( m_pMasterSession != NULL )
			{
				SUNLOG( eCRITICAL_LOG, "Master가 Connect할 수 있는 개수가 넘었습니다." );
				return NULL;
			}
			pServerSession = MasterServerSession::ALLOC();
			m_pMasterSession = (MasterServerSession*)pServerSession;
		}break;
	case GAME_DBPROXY:
		{
			if( m_pDBProxySession != NULL )
			{
				SUNLOG( eCRITICAL_LOG, "DB가 Connect할 수 있는 개수가 넘었습니다." );
				return NULL;
			}
			pServerSession = DBProxyServerSession::ALLOC(); 
			m_pDBProxySession = (DBProxyServerSession*)pServerSession;
		}break;
	default: { ASSERT(!"존재하지 않는 서버타입입니다."); return NULL; }
	}

	pServerSession->Init( this );
	return pServerSession;
}

void PortalServer::FreeServerSession( ServerSession * pSession )
{
	pSession->Release();
	switch( pSession->GetServerType() )
	{
	case FIELD_SERVER:
	case BATTLE_SERVER:
	case GAME_SERVER:
		GameServerSession::FREE( (GameServerSession *)pSession ); break;
	case TEMP_SERVER:
		TempServerSession::FREE( (TempServerSession *)pSession ); break;
	case MASTER_SERVER:
		MasterServerSession::FREE( (MasterServerSession *)pSession ); m_pMasterSession = NULL; break;
	case AGENT_SERVER:
		AgentServerSession::FREE( (AgentServerSession *)pSession ); break;
	case GAME_DBPROXY:
		DBProxyServerSession::FREE( (DBProxyServerSession *)pSession ); m_pDBProxySession = NULL; break;
	default:
		{
			ASSERT( !"서버타입이 존재하지 않습니다." );
		}
	}
}

BOOL PortalServer::InitializeLogger()
{
	BYTE	bySunLogOption = 0,
			bySunLogFileLevel = 0;
	CHAR	pszSunLogFilePath[32];

	__READ_CONFIG_NUMBER( bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, GetDescFileName() );
	__READ_CONFIG_NUMBER( bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, GetDescFileName() );
	__READ_CONFIG_STRING( pszSunLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", NULL, GetDescFileName() );

	DISPMSG( "SUNLOG.Portal Initializing...\t" );

	g_pSunLog = new CSunLog;
	BOOL bRet = g_pSunLog->Init( bySunLogOption, bySunLogFileLevel, pszSunLogFilePath, "Portal" );

	g_pSunLog->SetFileOutDelayOption( FALSE );

	SUNLOG( eCRITICAL_LOG, "*** PortalServer Start ***" );
	SUNLOG( eCRITICAL_LOG, "*** PortalServer Server Version = %d ***", SERVER_RELEASE_BUILD_NO );

	if( bRet )	{ DISPMSG( " [OK]\n" ); }
	else		{ DISPMSG( " Failed!\n" ); }

	return bRet;
}

BOOL PortalServer::Init()
{
	if( !InitializeLogger() )
	{
		DISPMSG(" Create SunLog Failed.." );
		return FALSE;
	}

	m_AgentServerSessionPool.Initialize(20);
	m_DBProxyServerSessionPool.Initialize(2);
	m_GameServerSessionPool.Initialize(100);
	m_TempServerSessionPool.Initialize(100);
	m_MasterServerSessionPool.Initialize(4);

	AgentServerSession::SetPool(&m_AgentServerSessionPool);
	DBProxyServerSession::SetPool(&m_DBProxyServerSessionPool);
	GameServerSession::SetPool(&m_GameServerSessionPool);
	TempServerSession::SetPool(&m_TempServerSessionPool);
	MasterServerSession::SetPool(&m_MasterServerSessionPool);

	WorkLoadManager::CreateInstance();

	Init_ScriptData( IsServerInfoFromFile() );

	int m_SettledYear, m_SettledMonth, m_SettledDay, m_PeriodDay, m_SuspendTime;
	__READ_CONFIG_NUMBER( m_SuspendTime, "ATTRIBUTES", "RANKING_SUSPEND_TIME", 0, GetDescFileName() );
	__READ_CONFIG_NUMBER( m_PeriodDay, "ATTRIBUTES", "RANKING_PERIOD_DAY", 0, GetDescFileName() );
	__READ_CONFIG_NUMBER( m_SettledYear, "ATTRIBUTES", "RANKING_SETTLED_YEAR", 0, GetDescFileName() );
	__READ_CONFIG_NUMBER( m_SettledMonth, "ATTRIBUTES", "RANKING_SETTLED_MONTH", 0, GetDescFileName() );
	__READ_CONFIG_NUMBER( m_SettledDay, "ATTRIBUTES", "RANKING_SETTLED_DAY", 0, GetDescFileName() );
	ASSERT( m_SuspendTime > 0 && m_PeriodDay > 0 && 
		m_SettledYear > 0 && m_SettledMonth > 0 && m_SettledDay > 0 );

#ifdef _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT
    // 정산 시점을 suspend time만큼 뒤로 밀어 앞뒤로 주어지는 중단 시간이 정시에 시작하도록 변경
    int minute = m_SuspendTime;
#else
    int minute = 0;
#endif
	g_RankingMgr.InitComputation( m_SettledYear, m_SettledMonth, m_SettledDay, 0, minute, 0, m_PeriodDay, m_SuspendTime );

#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    StartServer();
#endif
	return TRUE;
}

BOOL PortalServer::Update( DWORD tick )
{
	g_RankingMgr.Update();

	WorkLoadManager::Update();

	if( g_pSunLog ) g_pSunLog->Update();

	return TRUE;
}

void PortalServer::Release()
{
	g_RankingMgr.Release();

	SetPhaseStatus( PHASE_UNKNOWN );
	WorkLoadManager::DestroyInstance();

	if( m_pPortalScriptParserManager )
	{
		m_pPortalScriptParserManager->Release();
		m_pPortalScriptParserManager->DestroyInstanceEx();
		SAFE_DELETE( m_pPortalScriptParserManager );
	}

	SAFE_DELETE(g_pSunLog);
}


BOOL PortalServer::KeyEvent( char ch )
{
	switch(ch)
	{
	case 'P': break;//DisplayPoolInfo(); break;
	case 'K':
#ifdef _DEBUG
		g_RankingMgr.DisplayRanking();
#endif
		g_RankingMgr.Display();
		break;
	}

	return TRUE;
}

ServerSession * PortalServer::GetSession( eSERVER_TYPE type )
{
	switch(type)
	{
	case GAME_DBPROXY:
		return m_pDBProxySession;
	case MASTER_SERVER:
		return m_pMasterSession;
	}
	return NULL;
}

BOOL PortalServer::ConnectTo( eSERVER_TYPE type )
{
	ServerSession * pSession = NULL;
	switch(type)
	{
	case GAME_DBPROXY:
		pSession = m_pDBProxySession; break;
	case MASTER_SERVER:
		pSession = m_pMasterSession; break;
	}

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( !pSession ) return FALSE;

	if( pSession->IsConnecting() || pSession->IsConnected() )
	{
		ASSERT( !"중복 Connect 시도" );
		return FALSE;
	}

	Connect(pSession);

	return TRUE;

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( !pSession )
		return FALSE;

	if( ! IsServerInfoFromFile() )
	{
		if( (MASTER_SERVER != type)
			&& (GetPhaseStatus() != PHASE_SERVICE) )
			return FALSE;
	}

	if( !( pSession->IsConnecting() || pSession->IsConnected() ) )
	{
		if( (MASTER_SERVER != type)
			&& (! IsServerInfoFromFile() )
			&& (GetPhaseStatus() != PHASE_SERVICE) )
			return FALSE;

		Connect( pSession );
		return TRUE;
	}

	ASSERT( !"중복 Connect 시도" );
	return FALSE;

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

BOOL PortalServer::Init_ScriptData( BOOL bReadFromFile )
{
	m_pPortalScriptParserManager = new PortalScriptParserManager;
	m_pPortalScriptParserManager->RegisterParser();
	m_pPortalScriptParserManager->InitParserPool( NULL );

	#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		return TRUE;
	#else
		return m_pPortalScriptParserManager->Load();
	#endif
}

#include <PacketStruct_ServerCommon.h>
// 모든 데이터 로딩이 완료되었을 경우 콜백으로 호출될 것이다.
VOID	PortalServer::StartServer()
{
	// <해야 할 일>
	//
	// <NOTICE>
	//   이것이 호출되었다는 의미는 필요한 모든 데이터를 받았다는 의미다.
	// <THINKING>

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 현 시점 PHASE_EXCHANGE일 것이다.
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;
	g_PortalServer.SendMaster( &msgNTF, sizeof(msgNTF) );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}


//==================================================================================================
//==================================================================================================
//==================================================================================================

