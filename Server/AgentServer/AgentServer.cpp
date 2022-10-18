#include "stdafx.h"
#include "AgentServer.h"

#include <io.h>

#include <PacketStruct_ServerCommon.h>
#include <ServerOptionParser.h>

#ifdef _APPLY_NHN_FILTERFING
    #include <IAF_Export.h>	//lib 사용	
#endif

//
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/MasterServerSession.h>
#include <ServerSessions/DBProxyServerSession.h>
#include <ServerSessions/AuthAgentServerSession.h>
#include <ServerSessions/WorldServerSession.h>
//
#include <PacketHandler/PacketHandler.h>
//
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>
#include <TimeCheck/HeartbeatManager.h>
//
#include <Zone/ZoneManager.h>
#include <Zone/WarControlManager.h>
#include <Zone/PortalManager.h>
//

#include <AgentParty/AgentPartyManager.h>

#include "BattleOpenTimeManager.h"
#include <Function/GuildRanking/GuildRanking.h>

//////////////////////////////////////////////////////////////////////////
// 임시 includes
#include "TimeCheck/ConnectionTimeoutFactory.h"
#include "TimeCheck/TimeKeeper.h"
#include "TimeCheck/ConnectionTimeout.h"
#include "Function/AgentServer.Timer.WorkLoad.h"

//
//////////////////////////////////////////////////////////////////////////

#include "Function/AgentScriptParserManager.h"

#include <ProhibitionWordParser.h>
#ifdef _NA_0_20110329_GAMEOPTION_SETTING
#include "Function/GameOptionManager.h"
#endif //_NA_0_20110329_GAMEOPTION_SETTING

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/AgentChaosZoneManager.h>
#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#include <TimeCheck/BattleGroundOpenTimeManager.h>
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#include <ChaosZoneSystem/TeamBattleGroundManager.h> //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include <ChaosZoneSystem/FreePVPManager.h>
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <ChaosZoneSystem/SonnenScheinManager.h>
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include <ChaosZoneSystem/OneDaysGuildMissionManager.h>
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

#if USING_GAMEGUARD
#include "GameGuard/GameGuard.h"
#endif //USING_GAMEGUARD

// <DEFINITIONs>
#define DISP_OK		SUNLOG( eFULL_LOG,  " [OK]" );
#define DISP_FAIL	SUNLOG( eFULL_LOG,  " Failed!" );

AgentServer* AgentServer::instance_ = NULL;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//	<MOLA 5th Update Routine + HACK SHIELD>
//	__NA000630_070817_MOLA_5th_UPDATE
sCLIENT_INIT_PRE_ENTRY_WRAP		s_SECURE_INIT_PRE_ENTRY;

sCLIENT_INIT_PRE_ENTRY_WRAP*	AgentServer::GetGResource()
{
	return &s_SECURE_INIT_PRE_ENTRY;
}

#if USING_HACKSHIELD
AHNHS_SERVER_HANDLE				AgentServer::GetHANDLEofHS()
{
	return s_SECURE_INIT_PRE_ENTRY.GetHANDLEofHS();
}
#elif USING_XIGNCODE
#elif USING_FROST
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
// <CODEs><IMPLEMENTATION>
//
//

AgentServer::AgentServer()
{
	m_pServerPoolRouter = NULL;

	Init_ClearFields();

	
	do
	{	// Connect Sessions
		m_pServerPoolRouter = new SERVER_SESSION_ROUTER;
		SERVER_SESSION_ROUTER& rRouter = GetServerPoolRouter();

		rRouter.Register<AuthAgentServerSession>(AUTH_AGENT_SERVER);
		rRouter.Register<DBProxyServerSession>(GAME_DBPROXY);
		rRouter.Register<WorldServerSession>(WORLD_SERVER);
		rRouter.Register<MasterServerSession>(MASTER_SERVER);
		rRouter.Register<BattleServerSession>(BATTLE_SERVER);
		rRouter.Register<FieldServerSession>(FIELD_SERVER);
		rRouter.Register<TempServerSessionEx>(TEMP_SERVER);

		rRouter.Initialize( AUTH_AGENT_SERVER, 1, 1 );
		rRouter.Initialize( GAME_DBPROXY, 1, 1 );
		rRouter.Initialize( WORLD_SERVER, 1, 1 );
		rRouter.Initialize( MASTER_SERVER, 1, 1 );
		rRouter.Initialize( TEMP_SERVER, 10, 5 );
		rRouter.Initialize( FIELD_SERVER, 2, 2 );
		rRouter.Initialize( BATTLE_SERVER, 5, 5 );
	} while(0);
}

AgentServer::~AgentServer()
{
	Init_ClearFields();
}

AgentServer* AgentServer::CreateInstance()
{
    instance_ = new AgentServer();
    return instance_;
}

void AgentServer::DestroyInstance()
{
    SAFE_DELETE(instance_);
}

VOID	AgentServer::Init_ClearFields()
{
	SetPhaseStatus( PHASE_UNKNOWN );
	//m_bServiceMode;
	//m_skServerKey;
	SetDBProxyServerState( SERVER_NORMAL );
    db_connector_index_ = 0;

	memset( m_pServerSessions, 0, sizeof(m_pServerSessions) );

	m_bShutDownProcess = FALSE;
	//m_TimerUserCountReport;
	//m_TimerServerInfoLog;		// 서버정보의 로그를 남기는 타이머.

	memset( m_pszWorldServerClientIP, 0, sizeof(m_pszWorldServerClientIP) );
	m_wWorldServerClientPort = 0;

	SAFE_DELETE( m_pServerPoolRouter );
}

BOOL	AgentServer::Init()
{
	//////////////////////////////////////////////////////////////////////////
	// <환경설정 정보 읽기>
	//

	// (서비스/콘솔)모드 제어 확인할 것.
	DWORD dwShutdownDelayTime = 0;
	__READ_CONFIG_NUMBER( m_bServiceMode, "COMMON", "SERVICE_MODE", 0, AGENT_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_NUMBER( dwShutdownDelayTime, "ATTRIBUTES", "SHUTDOWN_DELAY_TIME", SHUTDOWN_NOACTION_DELAY_TIME, AGENT_SERVER_INFO_FILE_NAME );
	SERVER_EXIT.TotalDelayTime() = dwShutdownDelayTime;
	SERVER_EXIT.BlockUserOnRecvDelay() = 5*1000;

	DISPMSG( "SHUTDOWN_DELAY_TIME:%u\n", dwShutdownDelayTime );
	ASSERT( dwShutdownDelayTime > 0 );
	if( !(dwShutdownDelayTime > 0) )
		return FALSE;

	if( !Init_PoolInfo() )
		return FALSE;

    ;{
        // (CHANGES) (f100424.2L) change the server key configuration that the non-master mode has \
        // same channel based on the server key. the attribute value of the 'AGENTSERVER_ID' \
        // will be represented by the server key read from the agent server configuration file that\
        // to support the delvelopment mode multi-channel (1).
        if (this->IsServerInfoFromFile())
        {
            int channel_id = 0;
            __READ_CONFIG_NUMBER(channel_id, "SERVER_ADDINFO", "AGENTSERVER_ID", 0,
                                 AGENT_SERVER_INFO_FILE_NAME);
            SERVER_KEY change_key = GetKey();
            change_key.m_ChannelID = static_cast<BYTE>(channel_id);
            SetKey(change_key);
            UpdateAppTitle();
        }
    };

	//
	CHAR pszWorldClientIP[16];
	WORD dwWorldClientPort;
	__READ_CONFIG_NUMBER( dwWorldClientPort, "SERVER_ADDINFO", "WORLDSERVER_CLIENT_PORT", 0, AGENT_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_STRING_IP( pszWorldClientIP, "SERVER_ADDINFO", "WORLDSERVER_CLIENT_IP", NULL, AGENT_SERVER_INFO_FILE_NAME );

	SetWorldServerClientAddr( pszWorldClientIP, dwWorldClientPort );

	__READ_CONFIG_NUMBER( m_bPVPMode, "SERVER_ADDINFO", "PVP_MODE", 1, AGENT_SERVER_INFO_FILE_NAME );
	//	SetChannelID( pServerEnv->wAgentServerID );	//< 채널 ID
	//m_ServerKey.Set( (BYTE)m_ServerKey.GetWorldID(), (BYTE)pServerEnv->wAgentServerID, (BYTE)GetServerType(), (BYTE)m_ServerKey.GetServerID() );

	Init_Logger();

	// Agent 서버 시작 로그 
	SUNLOG( eCRITICAL_LOG, "*** AgentServer Start ***" );
	SUNLOG( eCRITICAL_LOG, "*** AgentServer Server Version = %d ***", SERVER_RELEASE_BUILD_NO );

#ifdef _NA_20100802_SPEED_HACK_DETECTION
    TickSpeedChecker::LoadSettings(_T(".\\AgentServer.TickSpeedChecker.ini"));

    if (TickSpeedChecker::IsGloballyEnabled())
    {
        CHAR log_file_path[256];
        __READ_CONFIG_STRING(log_file_path, "LOGFILE", "SUN_LOGFILE_PATH", NULL, AGENT_SERVER_INFO_FILE_NAME);

        TickSpeedChecker::CreateLogFile(log_file_path);
    }
#endif

	//////////////////////////////////////////////////////////////////////////
	PacketHandler::RegisterHandler();
	WorkLoadManager::CreateInstance();
	UserManager::CreateInstance();
	//
	// (WAVERIX) (NOTE)
	//ServerSessionManager::CreateInstance();
	//

	//-------------------------------------------------------------------------------------------------
	// 유저 관련 정보 초기화
	//-------------------------------------------------------------------------------------------------
	g_ConnTimeoutFactory.InitPool( 5000 );
	g_TimeKeeper.InitTimeKeeper( 5000 );
    // (WAVERIX) (090520) (CHANGES) 스크립트 로딩 이후로 처리 시점 변경
	//g_ZoneManager.CreateCharSelect();

	// 파티매니저 초기화
	AgentPartyManager::Instance()->Initialize( 1000 );

	if( !Init_ScriptData( IsServerInfoFromFile() ) )
	{
		P_FAILED;
		return FALSE;
	}
	P_SUCCESS;

	m_TimerUserCountReport.SetTimer( 10000 );

	if(Init_SecureLogic() == FALSE)
        return FALSE;

    // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager::Instance()->Init();

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    TeamBattleGroundManager::Instance()->Init();
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SonnenScheinManager::Instance()->Init();
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	OneDaysGuildMissionManager::Instance()->Init();
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

	// 서버 정보 로그 초기화~
	// ServerInfoLogCycle
#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
	StartServer();
#endif

	do
	{
		PROCESS_AGENTSERVER_PERIODIC_WORKLOAD* pWorkNode = new PROCESS_AGENTSERVER_PERIODIC_WORKLOAD();
		// (KEYWORD:CCU)
		m_wkPeriodicWorkKey = WorkLoadManager::RegisterTask( pWorkNode );
	} while(0);

	m_BattleJoinTimer.SetTimer( 30000 );
	m_RegisterTimer.SetTimer( 1000 );

	return TRUE;
}

VOID	AgentServer::Release()
{
	SetPhaseStatus( PHASE_UNKNOWN );
	PacketHandler::ReleaseHandler();
#if USING_HACKSHIELD
	AHNHS_SERVER_HANDLE hHANDLEofHS = s_SECURE_INIT_PRE_ENTRY.GetHANDLEofHS();
	_AhnHS_CloseServerHandle( hHANDLEofHS );
	s_SECURE_INIT_PRE_ENTRY.SetHANDLEofHS( ANTICPX_INVALID_HANDLE_VALUE );
#elif USING_XIGNCODE
    ns_secure::XignCode_Uninitialize();
#elif USING_FROST
#elif USING_GAMEGUARD
    GameGuard::Instance()->release();
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Release GameGuard!");
#endif
	if( m_pAgentScriptParserManager )
	{
		m_pAgentScriptParserManager->Release();
		m_pAgentScriptParserManager->DestroyInstanceEx();
		SAFE_DELETE( m_pAgentScriptParserManager );
	}

	//-------------------------------------------------------------------------------------------------
	// 유저 관련 정보 해제
	//-------------------------------------------------------------------------------------------------
	g_TimeKeeper.ReleaseTimeKeeper();
	g_ConnTimeoutFactory.ReleasePool();

	//UserManager::Instance()->ReleaseAllUser();

	SUNLOG( eCRITICAL_LOG, "*** AgentServer Close ***" );
}

VOID	AgentServer::DoFinalize()
{
	WorkLoadManager::DestroyInstance();

	ServerSessionManager::DestroyInstance();
    PortalManager::DestroyInstance(); // UserManager와 AgentPartyManager를 사용하기 때문에 둘보다 먼저 해제한다.
    UserManager::DestroyInstance();

	// 유저 OnDisconnect시 LeaveParty를 해야하므로 PartyManager 해제는 유저처리보다 나중에 한다.
	AgentPartyManager::Instance()->Release();	AgentPartyManager::DestroyInstance();

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
    GameOptionManager::DestroyInstance();
#endif

	ServerOptionParser::Instance()->Release();	ServerOptionParser::DestroyInstance();

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    AgentChaosZoneManager::DestroyInstance();
    BattleGroundManager::Instance()->Release(); BattleGroundManager::DestroyInstance();
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    
    //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    TeamBattleGroundManager::Instance()->Release();
    TeamBattleGroundManager::DestroyInstance();
    
    //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    FreePVPManager::DestroyInstance();

	SUNLOG( eCRITICAL_LOG, "*** AgentServer Close ***" );

	// 게임로그
	SAFE_DELETE(g_pLOG);
	// SUNLOG
	SAFE_DELETE(g_pSunLog);
	SAFE_DELETE(g_pSecureLog);
	SAFE_DELETE(g_pSunConnLog);
    SAFE_DELETE(g_pSecureModuleLog);
}

BOOL	AgentServer::Update( DWORD dwTick )
{
	// Shutdown Process
	// ...
	if( SERVER_EXIT.IsShutDown() )
		return FALSE;
    // TODO: make to general timer
    const util::ServerTimeInfoPerFrame& time_info = this->GetTimeInfoPerFrame();
	WorkLoadManager::Update();

	// timeout 관리 업데이트
	g_TimeKeeper.Update();
	g_ZoneManager.Process();
#ifdef _NA_004517_20120312_GAME_OPTION_TRANSACTION
    GameOptionManager::Instance()->Update();
#endif // _NA_004517_20120312_GAME_OPTION_TRANSACTION
    
	if( g_pSunLog )		g_pSunLog->Update();
	if( g_pSecureLog )	g_pSecureLog->Update();
	if( g_pSunConnLog )	g_pSunConnLog->Update();
    if (g_pSecureModuleLog) {
        g_pSecureModuleLog->Update();
    }
    g_BattleOpenTimeMng.Update(time_info);

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    g_GuildRankingSystem.UpdateGuildRanking();
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    BattleGroundOpenTimeManager::Instance()->Update(time_info);
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

#if USING_DUMMY_CLIENT
    test_unit::UpdateDummyClient();
#endif
	return TRUE;
}

VOID	AgentServer::DisplayPoolInfo()
{
	DISPMSG( "================= Pool Info =================\n" );
	g_ZoneManager.DisplayerZonePoolInfo();
}

BOOL AgentServer::KeyEvent( CHAR ch )
{
	switch(ch)
	{
	case 0x1B:
		SERVER_EXIT.StartServerExitProcess();
		return FALSE;
	case ' ':
		g_ZoneManager.Display();
		AgentPartyManager::Instance()->Display();
        //_NA_000000_20150507_TIME_DISPLAY
        DisplayCurrentServerTime();
		break;
#if USING_DUMMY_CLIENT
    case 'D':
        test_unit::StartDummyClient();
        break;
#endif
	case 'P': DisplayPoolInfo(); break;
	case 'U':
		break;
	case 'S':
#ifdef	_DEBUG
		PACKET_CONTROL::SetShowPacketStream( !PACKET_CONTROL::GetShowPacketStream() );
#endif
		break;
	case 'T':
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
		WaverixTestUnit();
#endif
		break;
	}

	return TRUE;
}

BOOL	AgentServer::SendToServerByIndex( eSERVER_TYPE stServerType, MSG_BASE* pMsg, WORD wSize )
{
	BOOL bProcess = (BOOL)(MAX_SERVER > stServerType);
	ServerSessionEx* pSession = NULL;

	if( bProcess )
		pSession = m_pServerSessions[stServerType];

	if( bProcess )
	{
		if( pSession && pSession->IsConnected() )
			return pSession->SendPacket( pMsg, wSize );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, _T("잘못된 서버타입으로 패킷 전송시도 %d"), stServerType );
	}

	return FALSE;
}

BOOL	AgentServer::SendToAuthAgentServer( BYTE* pMsg, WORD wSize )
{
	ServerSessionEx* pSession = m_pServerSessions[AUTH_AGENT_SERVER];
	if( pSession && pSession->IsConnected() )
		return pSession->Send( (BYTE*)pMsg, wSize );

	return FALSE;
}

class SendToGameServerOp
{
private:	MSG_BASE*	m_pMsg;
			WORD		m_wSize;

public:		SendToGameServerOp( MSG_BASE* pMsg, WORD wSize ) : m_pMsg(pMsg), m_wSize(wSize) {}
			VOID	operator() ( ServerSessionEx* pServerSession )
			{
				pServerSession->SendPacket( m_pMsg, m_wSize );
			}
};


VOID	AgentServer::SendToGameServer( MSG_BASE* pMsg, WORD wSize )
{
	SendToGameServerOp op( pMsg, wSize );
	ServerSessionManager::Instance()->Foreach( op );
}

BOOL	AgentServer::IsConnectedTo( eSERVER_TYPE eServerType )
{
	if( MAX_SERVER > eServerType )
	{
		ServerSessionEx* pSession = m_pServerSessions[eServerType];
		if( pSession )
			return pSession->IsConnected();

		SUNLOG( eCRITICAL_LOG, _T("Undefined ConnectionType") );
		return FALSE;
	}

	SUNLOG( eCRITICAL_LOG, _T("Undefined ServerType.") );
	return FALSE;
}

BOOL	AgentServer::ConnectTo( eSERVER_TYPE serverType )
{
	ServerSession* pSession = NULL;
	switch( serverType )
	{
	case MASTER_SERVER:
	case GAME_DBPROXY:
	case AUTH_AGENT_SERVER:
	case WORLD_SERVER:
		pSession = m_pServerSessions[serverType];
		break;
	}

	if( !pSession ) return FALSE;

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	if( pSession->IsConnecting() || pSession->IsConnected() )
	{
		ASSERT( !"중복 Connect 시도" );
		return FALSE;
	}

	Connect( pSession );

	return TRUE;
#else //
	if( ! IsServerInfoFromFile() )
	{
		if( (MASTER_SERVER != serverType) && (GetPhaseStatus() != PHASE_SERVICE) )
			return FALSE;
	}

	if( !( pSession->IsConnecting() || pSession->IsConnected() ) )
	{
		if( (MASTER_SERVER != serverType)
			&& (! IsServerInfoFromFile() )
			&& (GetPhaseStatus() != PHASE_SERVICE) )
			return FALSE;

		Connect( pSession );
		return TRUE;
	}

	ASSERT( !"중복 Connect 시도" );
	return FALSE;
#endif //

}

ServerSession*	AgentServer::AllocServerSession( eSERVER_TYPE serverType )
{
	ServerSession* pSession = (ServerSession*)m_pServerPoolRouter->Alloc( (DWORD)serverType );
	if( pSession )
	{
		//GAME_DBPROXY:
		//WORLD_SERVER:
		//MASTER_SERVER:
		//GUILD_SERVER:
		//AUTH_AGENT_SERVER:
		m_pServerSessions[serverType] = (ServerSessionEx*)pSession;
		pSession->Init( this );

		return pSession;
	}

//	ASSERT( !"존재하지 않는 서버타입입니다." );
	return NULL;
}

VOID			AgentServer::FreeServerSession( ServerSession* pServerSession )
{
	((ServerSession*)pServerSession)->Release();
	eSERVER_TYPE serverType = pServerSession->GetServerType();
	m_pServerPoolRouter->Free( (DWORD)serverType, pServerSession );
}

VOID			AgentServer::StartServerSideListen()
{
	Listen( SERVER_IOHANDLER );
}

VOID			AgentServer::StartClientSideListen()
{
	Listen( CLIENT_IOHANDLER );
}

//------------------------------------------------------------------------------------------------
// 클라이언트 콜백
//------------------------------------------------------------------------------------------------
NetworkObject*	AgentServer::CreateClientSideAcceptedObject()
{
	return UserManager::Instance()->AllocUser( UT_TEMP_USER );
}

VOID	AgentServer::DestroyClientSideAcceptedObject( NetworkObject* pNetworkObject )
{
	UserManager::Instance()->FreeUser( (UserSession *)pNetworkObject );
}

VOID	AgentServer::DestroyClientSideConnectedObject( NetworkObject* pNetworkObject )
{
	ASSERT( !"들어 올 수 없음" );
}

//------------------------------------------------------------------------------------------------
// 서버 콜백
//------------------------------------------------------------------------------------------------
NetworkObject*	AgentServer::CreateServerSideAcceptedObject()
{
	ServerSession* pServerSession = GetInstance()->AllocServerSession( TEMP_SERVER );

	return pServerSession;
}

VOID	AgentServer::DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject )
{
	ServerSession * pServerSession = (ServerSession *)pNetworkObject;
	GetInstance()->FreeServerSession( (ServerSession *)pNetworkObject );
}

VOID	AgentServer::DestroyServerSideConnectedObject( NetworkObject* pNetworkObject )
{
	// Nothing to do

	//ServerSession *pServerSession = (ServerSession*)pNetworkObject;
	//ServerSessionFactory::Instance()->FreeServerSession( (ServerSession *)pNetworkObject );
}

//------------------------------------------------------------------------------------------------
// 콘솔 메시지 콜백
//------------------------------------------------------------------------------------------------
VOID	CallBackCommand( CHAR* szMessage )
{
	switch(szMessage[0])
	{
	case 'c':
		{
			//DISPMSG( "[CallBackCommand ]Server Session :(%d) \n", AgentServer::GetInstance()->m_pIOCPServer->GetNumberOfConnections(	AgentServer::SERVER_IOHANDLER ) );
		}
		break;
	case 'i':
		{


		}
		break;
	}
}

// 모든 데이터 로딩이 완료되었을 경우 콜백으로 호출될 것이다.
VOID AgentServer::StartServer()
{
	// <해야 할 일>
	// 1. FieldServer에 접속되어 있는지?
	// 2. 네트워크를 통해 필요한 모든 데이터를 받았는지? <-- 여기 들어왔다는 것은 파일로부터 읽지 않았다는 의미
	// 3. <1,2>의 조건을 만족하면, CreateVillage를 호출한다.
	//    그런데, FieldServer::init에 의하면 접속될 때마다 CreateVillage를 호출하는 데... 맞나?
	//    한번만 호출하도록 해보자.
	//
	// <NOTICE>
	//   이것이 호출되었다는 의미는 필요한 모든 데이터를 받았다는 의미다.
	//   따라서 FieldServer의 접속 여부와 CreateVillage의 호출 여부가 될 것이다.
	//   계속해서 감시할 필요가 있다. <필요인자: g_AgentServer, AgentServer::GetInstancee()->m_FieldServer>
	// <THINKING>
	//   FieldServer가 끊어졌을 경우... 일단 재접될 것이며, 이때는...
	AgentServer* const pAgentServer = AgentServer::GetInstance();

	if( !WarControlManager::GetWorkKey() )
	{
		WarControlManager* pWorkNode = new WarControlManager();
		const WORK_KEY wKey = WorkLoadManager::RegisterTask( pWorkNode );
		WarControlManager::SetWorkKey( wKey );
	}

    // (WAVERIX) (090520) (CHANGES) 스크립트 로딩 이후로 처리 시점 변경
    // (WAVERIX) (090712) (BUG-FIX) SolarAuth 연결 구조일 경우, 데이터 수신 완료 콜백으로 이것이
    // 호출된다. 따라서 재 초기화가 발생한다. 해당 문제 수정.
    static BOOLEAN s_bInited = 0;
    if(s_bInited == 0)
    {
        g_ZoneManager.CreateCharSelect();
    #ifdef __NA000000_081029_STATIC_ZONE_MATCHING_TABLE__
        g_ZoneManager.CreateConstStaticZoneTable();
    #endif
    #ifdef _APPLY_NHN_FILTERFING
        int result = IAF::LoadAbuseDB();
        // error handling?
        if(result != 0) {
            SUNLOG(eCRITICAL_LOG, "Fail to load IAF LoadAbuseDB %d", result);
        }
        else {
            SUNLOG(eCRITICAL_LOG, "Success to load IAF");
        }
    #endif
        s_bInited = true;
    }

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 현 시점 PHASE_EXCHANGE일 것이다.
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;
	pAgentServer->SessionMasterServer()->Send( (BYTE*)&msgNTF, sizeof(msgNTF) );
	//ServerSessionManager::Instance()->RegisterEvent_OnAcceptFieldServer( TRUE );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

//////////////////////////////////////////////////////////////////////////

BOOL	AgentServer::Init_PoolInfo()
{
	sPOOL_SIZE_INFO rINFO;
	switch( GetMode() )
	{
	case eSERVER_LIVE:
		rINFO.dwUserPoolBase = 3500;
		rINFO.dwUserPoolExtend = 100;
		rINFO.dwTempUserPoolBase = 1000;
		rINFO.dwTempUserPoolExtend = 100;
		break;
	case eSERVER_TESTLIVE:
		rINFO.dwUserPoolBase = 1000;
		rINFO.dwUserPoolExtend = 100;
		rINFO.dwTempUserPoolBase = 100;
		rINFO.dwTempUserPoolExtend = 100;
		break;
	default:
		rINFO.dwUserPoolBase = 10;
		rINFO.dwUserPoolExtend = 10;
		rINFO.dwTempUserPoolBase = 10;
		rINFO.dwTempUserPoolExtend = 10;
		break;
	}
	BOOL bRet = ObjectPoolRouter::Instance()->InitializeByPoolInfo( &rINFO );
	if( bRet )	DISPMSG( "INIT OBJPOOL SUCCESS\n" );
	else
	{
		DISPMSG( "INIT OBJPOOL FAIL\n" );
		return FALSE;
	}

	return TRUE;
}

BOOL AgentServer::Init_ScriptData( BOOL bReadFromFile )
{
	// 스크립트 초기화 순서에 따라 영향을 받는 스크립트가 존재하므로 이 순서를 지켜주자.
	m_pAgentScriptParserManager = new AgentScriptParserManager;
	m_pAgentScriptParserManager->RegisterParser();
	m_pAgentScriptParserManager->InitParserPool( NULL );
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__		//솔라오스할때는 Load를 나중에 한다.
	return TRUE;
#else
	return m_pAgentScriptParserManager->Load();
#endif

}

BOOL AgentServer::Init_Logger()
{
	//-------------------------------------------------------------------------------------------------
	// 로그 파일 정보 초기화
	//-------------------------------------------------------------------------------------------------

	BYTE bySunLogOption, bySunLogFileLevel;
	CHAR pszLogFilePath[256];

	__READ_CONFIG_NUMBER( bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, AGENT_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_NUMBER( bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, AGENT_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_STRING( pszLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", NULL, AGENT_SERVER_INFO_FILE_NAME );

	//1. AgentLog
	g_pSunLog = new CSunLog;
	if( g_pSunLog->Init(bySunLogOption, bySunLogFileLevel, pszLogFilePath, "Agent") == false )
		DISPMSG(  "SUNLOG Initialize Failed...\n" );

	//2. 보안로그
	g_pSecureLog = new SecureLog();
	if( g_pSecureLog->Init(bySunLogOption, bySunLogFileLevel, pszLogFilePath, "Secure", FALSE) )
	{
		g_pSecureLog->RegisterHandler( SecureLog::WriteMessage_Network );

		// 초기화 되었을 때, 수행
		//MessageSecure( eCRITICAL_LOG, _T("HELLO") );
		//g_pSecureLog->Update();
		//g_pSecureLog->Flush();
	}

	//3. ConnLog
	g_pSunConnLog = new CSunLog;
	if( g_pSunConnLog->Init(bySunLogOption, bySunLogFileLevel, pszLogFilePath, "AgentConn") == false )
		DISPMSG(  "SUNCONNLOG Initialize Failed...\n" );
#if USING_XIGNCODE
    g_pSecureModuleLog = new CSunLog;
    if (g_pSecureModuleLog->Init(bySunLogOption, bySunLogFileLevel, pszLogFilePath, "XignCode") == false)
    {
        DISPMSG("XignCodeLogger Initialize Failed...\n");
    }
#endif
	BYTE byLogOption;
	__READ_CONFIG_NUMBER( byLogOption, "LOGFILE", "WOPS_LOGFILE_OPTION", 0, AGENT_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_STRING( pszLogFilePath, "LOGFILE", "WOPS_LOGFILE_PATH", NULL, AGENT_SERVER_INFO_FILE_NAME );

	DISPMSG(  "Creating GameLog File...\n" );
	g_pLOG = new AgentLog;
	if( _access( pszLogFilePath, 0 ) == -1 )
	{
		if( !CreateDirectory(pszLogFilePath, NULL) )
			printf( "CreateDirectory Fail, path = %s \n", pszLogFilePath  );
	}

	TCHAR szBinPath[MAX_PATH], szTextPath[MAX_PATH];
	strncpy(szBinPath, pszLogFilePath, MAX_PATH);
	strcat(szBinPath, "\\\\AGENT");
	strncpy(szTextPath, pszLogFilePath, MAX_PATH);
	strcat(szTextPath, "\\\\AGENT_TEXT");

#ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
	TCHAR szFileName[MAX_PATH];
	_sntprintf( szFileName, MAX_PATH, "%d_SUN_AGENT", NATION_BUILD_VERSION );
	if( !g_pLOG->Init( byLogOption, szFileName, _T(szBinPath), _T(szTextPath) ) )
#else
	if( !g_pLOG->Init( byLogOption, _T("SUN_AGENT"), _T(szBinPath), _T(szTextPath) ) )
#endif
		DISPMSG(  "GameLog File Failed...\n" );
	//	else
	//DISP_OK;

	return TRUE;
}

//BOOL	AgentServer::CheckServerMode()
//{
//	SERVER_ENV* pEnv = m_pFileParser->GetServerEnv();
//	if( !pEnv )	return FALSE;
//
//	//라이브나 테섭이면 무조건 0, 1이여야 한다.
//	if( pEnv->byServerStatus == 3 || pEnv->byServerStatus == 4 )	//라이브(3)나 테섭(4)이면..
//	{
//		if( (pEnv->byFieldServerType == 0) && (pEnv->byBattleServerType == 1) )
//			return TRUE;
//
//		return FALSE;
//	}
//
//	return TRUE;
//}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
BOOL	AgentServer::Init_SecureLogic()
{
	//	MOLA LIBRARY Initialize
	CryptManager::Init1st();

	//--------------------------------------------------------------------------------------------
	// <BEGIN> 패킷 암호화 관련 설정
	//
	// <070110, Waverix> 적용으로... 그 주기에 달려있다.
	do
	{
		//eCRYPTOGRAPHY_KEY_CHANGE_TIMEOUT
		DWORD dwServerMode = (DWORD)GetMode();
		//printf( "CHECK ROUTINE Init_SecureLogic\n" );
		RandomTimeUtil::Initialize( dwServerMode );

		SecureManager* pWorkNode = new SecureManager();
		pWorkNode->SetInformation( RandomTimeUtil::SECURE_MANAGER_UPDATE_INTERVAL(), RandomTimeUtil::USER_KEYCHANGE_TIMEOUT() );
		WorkLoadManager::RegisterTask( pWorkNode );
	} while(0);

	//
	// <END> 
	//--------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------
	// Heartbeat처리 관련
	do
	{
		//--------------------------------------------------------------------------------------------
		// Heartbeat처리 관련
		PROCESS_HEARTBEAT_VALUATION* pWorkNode = new PROCESS_HEARTBEAT_VALUATION;
		sHEARTBEAT_VALUATION_INFO sINFO;
		sINFO.m_dwCheckUserValuationPeriod = 30*RandomTimeUtil::SECOND;	// 30초마다 전체 유저들을 대상으로 한 평가 수행
		//sINFO.m_dwHeartbeatTick = 500;					//HEARTBEAT_TICK;
		//sINFO.m_dwValuationPeriod = 500*30;			//HEARTBEAT_TICK*30;
		sINFO.m_dwHeartbeatTick = HEARTBEAT_TICK;
		sINFO.m_dwValuationPeriod = HEARTBEAT_TICK*30;
		sINFO.m_bUseArmorKitHeartbeat = FALSE;
		pWorkNode->SetInformation( &sINFO );
		WorkLoadManager::RegisterTask( pWorkNode );

	} while(0);
#if USING_HACKSHIELD && !defined(_DEBUG)
    // init HackShield
	do {
		// filepath 읽어들이는 경로 변경 가능하게 설정할 것
		// hsb파일 경로

		CHAR tmp[512];
		CHAR pszFilePath[512];

		GetCurrentDirectory( sizeof(tmp), tmp );
		_snprintf( pszFilePath, sizeof(pszFilePath), "%s%s", tmp, "\\AntiCpx.hsb" );

		//CHAR* pszFilePath =
			//"D:/PROJECT/SUN.MOLA/ForeignVersion/Server/_bin/GameClient_Debug.exe.hsb";
		//	"S:\\AntiCpx.hsb";
			//"./AntiCpX.hsb";

		printf( "path %s\n", pszFilePath );
		AHNHS_SERVER_HANDLE hHANDLEofHS = _AhnHS_CreateServerObject( pszFilePath );
		if( ANTICPX_INVALID_HANDLE_VALUE == hHANDLEofHS )
		{
			MessageOut( eCRITICAL_LOG, _T("Can't Create HackShield Instance") );
			return FALSE;
		}

		s_SECURE_INIT_PRE_ENTRY.SetHANDLEofHS( hHANDLEofHS );
	} while(0);
#elif USING_XIGNCODE && !defined(_DEBUG)
    if (ns_secure::XignCode_Initilize() == false) {
        DISPMSG( "Can't Create SignCode Instance\n" );
        return false;
    }
#elif USING_FROST
#elif USING_GAMEGUARD
    UINT32 gameguard_result = GameGuard::Instance()->init();
    if (gameguard_result != ERROR_SUCCESS) {
        DISPMSG( "GameGuard Initialize Failed!\n" );
        return false;
    }
    else {
        DISPMSG( "GameGuard Initialize Success!\n" );
    }
#endif


#ifdef __WAVERIX_SECURITY_TEST__
	static DWORD dwKeyInc = 100;
	for( DWORD i=0 ; i<100 ; ++i )
	{
		User* pUser = (User*)UserManager::Instance()->AllocUser( UT_USER );
		pUser->SetAuthID( dwKeyInc++ );
		pUser->SetUserKey( dwKeyInc++ );
		//g_pUser->GetHeartbeatChecker()->SetEnable( TRUE );
		UserCrypts::AddCryptedUser( pUser );
		UserManager::Instance()->AddUser( pUser );
	}
#endif //__WAVERIX_SECURITY_TEST__



	return TRUE;
}

//_NA_000000_20150507_TIME_DISPLAY
VOID AgentServer::DisplayCurrentServerTime()
{
    SYSTEMTIME	cur_time;
    util::TimeSync::GetLocalTime(&cur_time);
    __time64_t timevalue;
    util::TimeSync::_time64(&timevalue);
    SUNLOG(eCRITICAL_LOG, "CurServerTime_timevalue: %d", timevalue);
    SUNLOG(eCRITICAL_LOG, "CurServerTime: [%d.%d.%d] [%d:%d:%d]", cur_time.wYear, 
           cur_time.wMonth, cur_time.wDay, cur_time.wHour, cur_time.wMinute, cur_time.wSecond);
}


//==================================================================================================
//==================================================================================================
//==================================================================================================
