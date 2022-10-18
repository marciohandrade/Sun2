#include "StdAfx.h"
#include ".\gameserverex.h"
#include <ServerSession.h>
#include "FileParser.h"


#include <ServerGlobal.h>

//custom
#include "ServerOptionParser.h"
#include "ObjectFactory.h"
#include "AbilityFactory.h"
#include "SummonManager.h"
#include "StatisticsManager.h"
#include "GameZoneManager.h"
#include "UnitGroupManager.h"
#include "QuestManager_Concrete.h"
#include "DropManager.h"
#include "PacketHandler/PacketHandler.h"
#include "ResrcManager.h"
#include "TradeManager.h"
#include "GMCmdManager.h"
#include "SunEventInfoParser.h"
#include "PeakTimeEvent.h"
#include "GameGuildManager.h"
#include "QuestInfoManagerEx.h"
#include "RatioManager.h"
#include "FunctionalRestrictManager.h"
#include "GameShopManager.h"
#include "PerfCheck.h"
#include "RewardManager.h"
#include <ItemTypeList.h>
#include <DropRatioInfoList.h>
#include <MovementChecker.h>
#include <ServerOptionParserEx.h>
#include <ServerBuildVersion.h>

#include "GameParty/NewGamePartyManager.h"
#include "GameDominationManager.h"
#include ".\FriendInvitation\GFriendInvitationController.h"
#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
#include "./SkillSystem/SkillCoolTimeAccessor.h"
#endif
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop/GameShop.Manager.h"
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#include "VendorManager.h"
#include <ExtraNPCManager.h>

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
#include "MonsterInvasionEvent.h"
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingGradeManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _DEBUG
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/tinyxml_d.lib")
#else
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/tinyxml.lib")
#endif //

GameServerEx * g_pGameServer = NULL;

NetworkObject *	GameServerEx::DoAllocServerSession()
{
	return (NetworkObject *)g_pGameServer->AllocServerSession(TEMP_SERVER);
}
VOID GameServerEx::DoFreeServerSession( NetworkObject * pNetObject )
{
	g_pGameServer->FreeServerSession((ServerSession*)pNetObject);
}

BOOL GameServerEx::SendToServer( eSERVER_TYPE type, MSG_BASE * pMsg, WORD wSize )
{
	if( !m_pSession[type] ) return FALSE;
	return m_pSession[type]->Send( (BYTE*)pMsg, wSize );
}

BOOL GameServerEx::IsConnected( eSERVER_TYPE type )
{
	if( !m_pSession[type] ) return FALSE;

	return m_pSession[type]->IsConnected();
}

VOID GameServerEx::SetServerAddr( eSERVER_TYPE type, char *pszIP, WORD wPort )
{
	if( !m_pSession[type] ) return ;
	m_pSession[type]->SetAddr(  pszIP, wPort );
}

ServerSession * GameServerEx::GetSession( eSERVER_TYPE type )
{
	return m_pSession[type];
}
BOOL GameServerEx::ConnectTo( eSERVER_TYPE type )
{
#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( !m_pSession[type] ) return FALSE;

	if( m_pSession[type]->IsConnecting() || m_pSession[type]->IsConnected() )
	{
		ASSERT( !"중복 Connect 시도" );
		return FALSE;
	}

	Connect(m_pSession[type]);

	return TRUE;

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	ServerSession* pSession = m_pSession[type];
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

ServerSession * GameServerEx::AllocServerSession( eSERVER_TYPE type )
{
	ServerSession* pServerSession = NULL;
	if( TEMP_SERVER == type )
	{
		pServerSession = TempServerSession::ALLOC();
		pServerSession->Init( this );
		return pServerSession;
	}

	if( m_pSession[type] == NULL )
	{
		switch( type )
		{
    #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
		case PORTAL_SERVER:	pServerSession = m_pSession[type] = PortalServerSession::ALLOC(); break;
    #endif
    #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
		case GUILD_SERVER:	pServerSession = m_pSession[type] = GuildServerSession::ALLOC(); break;
    #endif
		case AGENT_SERVER:	pServerSession = m_pSession[type] = AgentServerSession::ALLOC(); break;
		case MASTER_SERVER:	pServerSession = m_pSession[type] = MasterServerSession::ALLOC(); break;
		case GAME_DBPROXY:
            pServerSession = m_pSession[type] = GameDBProxySession::ALLOC();
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
            m_pSession[GUILD_SERVER] = pServerSession;
    #endif
            break;
    #if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
		case SHOP_SERVER:	pServerSession = m_pSession[type] = ShopServerSession::ALLOC(); break;
    #endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

		default: { ASSERT(!"Not Existed ServerType"); return NULL; }
		}

		// dispatch
		if( pServerSession )
		{
			pServerSession->Init( this );
			return pServerSession;
		}
	}

	// <WARNING!>
	//   error handling
	//
	//   upperbound를 검사하지 않는 구조로 바뀌었으므로
	//   앞선 m_pSession[type] == NULL 루틴에서 통과하는 형태가 존재할 것이다.
	//   접속시 서버 타입 검사를 반드시 수행하고 작업에 임해야 할 것이다.
	//   그렇지 않으면 잘못된 연산 또는 공격에 의한 서버 크래쉬 현상이
	//   발생할 것이다. - 최소한 아래 루틴만큼은 바운트 체크를 하자.
	if( (DWORD)MAX_SERVER > (DWORD)type )
	{
		TCHAR* pSessionName = GetServerType2String(type);
		SUNLOG( eCRITICAL_LOG, _T("[%16s]가 Connect할 수 있는 개수가 넘었습니다."), pSessionName );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, _T("잘못된 서버 타입입니다.") );
	}
	return NULL;
}

void GameServerEx::FreeServerSession( ServerSession * pSession )
{
	pSession->Release();
	eSERVER_TYPE type = pSession->GetServerType();
	switch( type )
	{
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	case GUILD_SERVER:
		GuildServerSession::FREE( (GuildServerSession *)pSession ); m_pSession[type] = NULL; break;
#endif
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	case PORTAL_SERVER:
		PortalServerSession::FREE( (PortalServerSession *)pSession ); m_pSession[type] = NULL; break;
#endif
	case TEMP_SERVER:
		TempServerSession::FREE( (TempServerSession *)pSession ); break;
	case MASTER_SERVER:
		MasterServerSession::FREE( (MasterServerSession *)pSession ); m_pSession[type] = NULL; break;
	case AGENT_SERVER:
		AgentServerSession::FREE( (AgentServerSession *)pSession ); m_pSession[type] = NULL; break;
	case GAME_DBPROXY:
		GameDBProxySession::FREE( (GameDBProxySession *)pSession );
        m_pSession[type] = NULL;
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
        m_pSession[GUILD_SERVER] = NULL;
    #endif
        break;

    #if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
	case SHOP_SERVER:
		ShopServerSession::FREE( (ShopServerSession *)pSession ); m_pSession[type] = NULL; break;
    #endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

	default:
		{
			ASSERT( !"서버타입이 존재하지 않습니다." );
		}
	}
}


GameServerEx::GameServerEx()
:	m_bConnectWorldServer(FALSE)
,	m_hDll3DTerrain(NULL)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
, battle_ground_log_(NULL)
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
, mondschein_log_ (NULL)
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG
#ifdef _NA_008334_20150608_SONNENSCHEIN
, sonnenschein_log_ (NULL)
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
, max_damage_log (NULL)
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG
#ifdef _NA_008706_20160229_TEXTLOG_PK
, battle_monitor_log_ (NULL)
#endif //_NA_008706_20160229_TEXTLOG_PK
{
	ZeroMemory(m_pSession, sizeof(ServerSession *)*MAX_SERVER);

	SetPhaseStatus( PHASE_UNKNOWN );

	m_bPVPMode = 0;
}

GameServerEx::~GameServerEx()
{
    for (int i = 0; i < _countof(m_pSession); ++i)
    {
        ServerSession* server_session = m_pSession[i];
        if (server_session != NULL)
        {
            FreeServerSession(server_session);
        }
    }
}


BOOL	GameServerEx::GetServerTypeStr( TCHAR* pszServerType, int nSize )
{
	ZeroMemory( pszServerType, nSize );

	if( GetServerType() == BATTLE_SERVER )
		strncpy( pszServerType, "Battle", nSize );
	else if( GetServerType() == FIELD_SERVER )
		strncpy( pszServerType, "Field", nSize );
	else
		return FALSE;

	return TRUE;
}


BOOL	GameServerEx::CreateSunLog()
{
	BYTE	bySunLogOption = 0, bySunLogFileLevel = 0;
	TCHAR	szSunLogFilePath[256];	//__READ_CONFIG_STRING에서 내부적으로 크기가 256으로 정해져 있다.
	TCHAR	szServerType[MAX_PATH];
	DWORD	dwServerInfoCycle = 0;
	__READ_CONFIG_NUMBER( bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, GetDescFileName() );
	__READ_CONFIG_NUMBER( bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, GetDescFileName() );
	__READ_CONFIG_STRING( szSunLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", "", GetDescFileName() );
	__READ_CONFIG_NUMBER( dwServerInfoCycle, "LOGFILE", "SERVERINFO_CYCLE", 0, GetDescFileName() );

	GetServerTypeStr( szServerType, MAX_PATH );

	g_pSunLog = new CSunLog;
	if( !g_pSunLog->Init( bySunLogOption, bySunLogFileLevel, szSunLogFilePath, szServerType ) )
		return FALSE;

	g_pSunLog->SetFileOutDelayOption( FALSE );

	SUNLOG( eCRITICAL_LOG, "*** %s Server Start ***", szServerType );
	SUNLOG( eCRITICAL_LOG, "*** %s Server Version = %d ***", szServerType, SERVER_RELEASE_BUILD_NO );

    SUNLOG( eDEV_LOG, "*** log level test ***");
    

	//2. 보안로그
	g_pSecureLog = new SecureLog();
	if( g_pSecureLog->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "Secure", FALSE) )
	{
		g_pSecureLog->RegisterHandler( SecureLog::WriteMessage_Network );
	}
	else
	{
		P_FAILED;
	}

    if (GetServerType() == FIELD_SERVER)
    {
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
        mondschein_log_ = new CSunLog;
        if (mondschein_log_->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "Mondschein") == false) {
            DISPMSG( "SUNCONNLOG(MONDSCHIEN) Initialize Failed... \n");
        }
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
        battle_monitor_log_ = new CSunLog;
        if (battle_monitor_log_->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "BattleMonitor") == false) {
            DISPMSG( "SUNCONNLOG(BATTLEMONITOR) Initialize Failed... \n");
        }
#endif //_NA_008706_20160229_TEXTLOG_PK
    }

    if (GetServerType() == BATTLE_SERVER)
    {
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        //3. 전장로그 배틀 서버에서 로그를 생성..
        battle_ground_log_ = new CSunLog;
        if( battle_ground_log_->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "BattleGround") == false )
            DISPMSG(  "SUNCONNLOG Initialize Failed...\n" );
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
        //Max Damage Log
        max_damage_log = new CSunLog;
        if( max_damage_log->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "MaxDamage") == false )
            DISPMSG(  "SUNCONNLOG(max_damage_log) Initialize Failed...\n" );
#endif //_NA_008299_20150519_MAX_DAMAGE_LOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
        sonnenschein_log_ = new CSunLog;
        if (sonnenschein_log_->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "SonnenSchein") == false) {
            DISPMSG( "SUNCONNLOG(SONNENSCHEIN) Initialize Failed... \n");
        }
#endif //_NA_008334_20150608_SONNENSCHEIN
    }

	if( dwServerInfoCycle == 0 )
		m_ServerInfoLogTimer.DisableCheckTime();
	else
		m_ServerInfoLogTimer.SetTimer( dwServerInfoCycle*1000*60 );

#ifdef _DEBUG

    {
        const int AverageRenderInfoSize = sizeof(PLAYER_RENDER_INFO_EX) -
                                          sizeof(STATE_TOTAL_INFO) +
                                          sizeof(STATE_INFO) * 15;
        SUNLOG(eCRITICAL_LOG, "[Simulation Max PlayerRenderSize] %d",
               AverageRenderInfoSize * MSG_CG_SYNC_ALLPLAYERS_CMD::_MAX_PLAYER_INFO_SIZE);
    }

#endif //_DEBUG

    return TRUE;
}

bool GameServerEx::LoadGameShopConfiguration()
{
#if !defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
    return false;
#endif
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    return false;
#else
  #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    char section_name[32 + 1];
    //
    const char* file_name = GetDescFileName();
    int number_of_connections = 0;
    __READ_CONFIG_NUMBER(number_of_connections,
        "SERVER_IOHANDLER", "CONEECTION_NUM", 0, file_name);
    int index = 0;
    for (; index < number_of_connections; ++index)
    {
        _snprintf(section_name, _countof(section_name), "CONNECTION_%d", index);
        int server_type = UNKNOWN_SERVER;
        __READ_CONFIG_NUMBER(server_type, section_name, "TYPE", 0, file_name);
        if (server_type != SHOP_SERVER) {
            continue;
        };

        char ip_address[MAX_IPADDRESS_SIZE + 1];
        WORD connect_port = 0;
		__READ_CONFIG_STRING_IP(ip_address, section_name, "IP", NULL, file_name);
        ip_address[_countof(ip_address) - 1] = '\0';
		__READ_CONFIG_NUMBER(connect_port, section_name, "PORT", 0, file_name);

        GameShop::Manager* const shop_manager = GameShop::Manager::Instance();
        shop_manager->Initialize(ip_address, connect_port);
        break;
    };

    if (index == number_of_connections) {
        return false;
    };

    GameShop::Manager* const shop_manager = GameShop::Manager::Instance();
    shop_manager->StartService();
    return true;
  #else
    return false;
  #endif
#endif
};

BOOL GameServerEx::Init()
{
#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
	if( !CreateSunLog() )
	{
		DISPMSG(" Create SunLog Failed.." );
		return FALSE;
	}
#endif
    db_connector_index_ = 0;

	WorkLoadManager::CreateInstance();

	//Check ServerMode

	m_AgentServerSessionPool.Initialize(20, 0, "AgentServerSession", AssertionLogger4Pool_Server);
	m_DBProxyServerSessionPool.Initialize(2, 0, "GameDBProxySession", AssertionLogger4Pool_Server);
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	m_GuildServerSessionPool.Initialize(2, 0, "GuildServerSession", AssertionLogger4Pool_Server);
#endif
	m_MasterServerSessionPool.Initialize(2, 0, "MasterServerSession", AssertionLogger4Pool_Server);
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	m_PortalServerSessionPool.Initialize(2, 0, "PortalServerSession", AssertionLogger4Pool_Server);
#endif
#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
	m_ShopServerSessionPool.Initialize(2, 0, "ShopServerSession", AssertionLogger4Pool_Server);
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

    m_TempServerSessionPool.Initialize(1, 0, "TempServerSession", AssertionLogger4Pool_Server);
	
	AgentServerSession::SetPool(&m_AgentServerSessionPool);
	GameDBProxySession::SetPool(&m_DBProxyServerSessionPool);
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	GuildServerSession::SetPool(&m_GuildServerSessionPool);
#endif
	MasterServerSession::SetPool(&m_MasterServerSessionPool);
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	PortalServerSession::SetPool(&m_PortalServerSessionPool);
#endif
	TempServerSession::SetPool(&m_TempServerSessionPool);

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
	ShopServerSession::SetPool(&m_ShopServerSessionPool);
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
	//-------------------------------------------------------------------------------------------------------------
	// 3D엔진팀 지형엔진DLL, 수치엔진DLL 로딩
#ifdef _DEV_TAIYO_PATHENGINE_CODE
#ifdef _DEBUG
	char *lpsz3DTerrainDLL = "3DTerrainPE_sd.dll";
	char *lpszNumericDLL = "Numeric_d.dll";
#else
	char *lpsz3DTerrainDLL = "3DTerrainPE_s.dll";
	char *lpszNumericDLL = "Numeric.dll";
#endif
#else //< DEV_TAIYO_PATHENGINE_CODE
#ifdef _DEBUG
	char *lpsz3DTerrainDLL = "3DTerrain_sd.dll";
	char *lpszNumericDLL = "Numeric_d.dll";
#else
	char *lpsz3DTerrainDLL = "3DTerrain_s.dll";
	char *lpszNumericDLL = "Numeric.dll";
#endif
#endif //< DEV_TAIYO_PATHENGINE_CODE
	DISPMSG(  "Loading \"%s\"...", lpsz3DTerrainDLL );
	m_hDll3DTerrain = Load3DTerrainLibrary( lpsz3DTerrainDLL, &m_fnContactTerrain );
	if ( !m_hDll3DTerrain )
	{
		P_FAILED;
		return FALSE;
	}
	P_SUCCESS;

	m_pLoadHelper = (CLoadHelper *)g_pGameServer->m_fnContactTerrain( CT_NEW_MAPLOADER, 0 );
#endif

//스크립트 로드......
	if( !ParserInit() )
		return FALSE;

	//-------------------------------------------------------------------------------------------------------------


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	//-------------------------------------------------------------------------------------------------------------
	DISPMSG(  "Register PacketHandler..." );
	PacketHandler::Instance();
	P_SUCCESS;
	//-------------------------------------------------------------------------------------------------------------

	if( g_pSunLog )	g_pSunLog->SetFileOutDelayOption( TRUE );

	return TRUE;

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	// 파서를 제외한 서버로직 초기화
	LogicInit();

	if( g_pSunLog )	g_pSunLog->SetFileOutDelayOption( TRUE );

	return TRUE;
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

BOOL GameServerEx::Update( DWORD dwDeltaTick )
{
    // f110805.3L, f100920.3L, change a time control to support multi-time utilization
    const util::ServerTimeInfoPerFrame& time_info = this->GetTimeInfoPerFrame();
    //WAVERIX_TIME_CHECK_INIT_START();
    MovementChecker::Update(time_info.delta_tick_);
    // CHANGES: f110214.2L, added heartbeat control
    nsAI::NpcHeartbeat::Update(&time_info);

    WorkLoadManager::Update();

    g_GameZoneManager.Update(time_info.delta_tick_);

    // (WAVERIX) (090517) (NOTE) 이 루틴은 PlayerManager보다 먼저 처리되도록 한다.
    NewGamePartyManager::Instance()->Update();
    PlayerManager::Instance()->Update(&time_info);

    g_DropManager.Process();

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    g_GameShopManager.Update();
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    if (GameDominationManager* const domination_manager = GameDominationManager::Instance())
    {
        if (domination_manager->DominationTimerIsExpired()) {
            domination_manager->LoadProcess();
        };
    };

    if (m_ServerInfoLogTimer.IsExpired())
    {
        DisplayAppInfo(true);
        DisplayServerInfo(true);
    };
#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
    g_pSunLog->Update();
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (battle_ground_log_)
        battle_ground_log_->Update();
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
    if (mondschein_log_) {
        mondschein_log_->Update();
    }
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG
#ifdef _NA_008334_20150608_SONNENSCHEIN
    if (sonnenschein_log_) {
        sonnenschein_log_->Update();
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
    if (max_damage_log)
    {
        max_damage_log->Update();
    }
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
    if (battle_monitor_log_)
    {
        battle_monitor_log_->Update();
    }
#endif //_NA_008706_20160229_TEXTLOG_PK

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    GameShop::Manager::Instance()->Update(dwDeltaTick);
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    HonorSystem::Instance()->Update();
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    g_VendorManager.Update();
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SunRankingGradeManager::Instance()->Update();
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

    g_nPerfCounter++;

    return true;
}

void GameServerEx::Release()
{
	SetPhaseStatus( PHASE_UNKNOWN );
	WorkLoadManager::DestroyInstance();

	g_GameZoneManager.Release();
	g_GameGuildManager.Release();

	g_ResrcMgr.Destroy();
	//	g_MapAllocator.Release();
	g_TradeManager.Release();

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
	g_Ratio.Release();
#endif // _NA_000251_20100727_SOCKET_SYSTEM

	PacketHandler::DestroyInstance();
	SummonManager::Instance()->Release();				
	PlayerManager::DestroyInstance();

    GameDominationManager::DestroyInstance();

	NewGamePartyManager::Instance()->Release();			NewGamePartyManager::DestroyInstance();
    ObjectFactory::Instance()->Release();				
    
	SummonManager::DestroyInstance();
	PeakTimeEvent::DestroyInstance();

    GFriendInvitationController::DestroyInstance();

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    SkillCoolTimeAccessor::DestroyInstance();
#endif
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    GameShop::Manager::DestroyInstance();
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    HonorSystem::DestroyInstance();
#endif //_NA_003027_20111013_HONOR_SYSTEM
    g_VendorManager.Destroy();

	//QuestManager를 Release 하기 전에는 QuestInfoManager를 Unload하면 안된다.
	//QuestManager의 Release는 	PlayerManager::DestroyInstance() 에서 이루어진다.
	g_QuestInfoManager.Unload();
	g_DropManager.Release();
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    g_ExtraNPCManager.release();
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

    ParserRelease();

	HighLoadBalancer::DestroyInstance();

#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
	g_pGameServer->m_fnContactTerrain( CT_DELETE_MAPLOADER, m_pLoadHelper );

	if( m_hDll3DTerrain )			
	{
		FreeLibrary( m_hDll3DTerrain );
	}
#endif

	SUNLOG( eCRITICAL_LOG, "*** Server Close ***" );

	SAFE_DELETE(g_pLOG);
	SAFE_DELETE(g_pSunLog);
	SAFE_DELETE(g_pSecureLog);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SAFE_DELETE(battle_ground_log_);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
    SAFE_DELETE(mondschein_log_);
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SAFE_DELETE(sonnenschein_log_);
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
    SAFE_DELETE(max_damage_log);
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG
#ifdef _NA_008706_20160229_TEXTLOG_PK
    SAFE_DELETE(battle_monitor_log_);
#endif //_NA_008706_20160229_TEXTLOG_PK
}

VOID	GameServerEx::Finalize()
{
	PlayerManager::DestroyInstance();
	ServerOptionParserEx::DestroyInstance();
	NewGamePartyManager::DestroyInstance();
    ObjectFactory::DestroyInstance();
}


//
VOID GameServerEx::ReloadData()
{
//	// 리로딩 하지 못하는 것!!!
//	//MapInfoParser::Instance()->Reload();
//	//TriggerParser::Instance()->Reload();
//
//	// 모든 파서 리로딩
//
//	ItemInfoParser::Instance()->Reload("data\\WeaponItemInfo.txt");
//	ItemInfoParser::Instance()->Reload("data\\ArmorItemInfo.txt");
//	ItemInfoParser::Instance()->Reload("data\\WasteItemInfo.txt");
//	ItemInfoParser::Instance()->Reload("data\\AccessoryItemInfo.txt");
//
//	NPCInfoParser::Instance()->Reload();
//	//	CharInfoParser::Instance()->Reload();
//	ShopInfoParser::Instance()->Reload();
//	SkillInfoParser::Instance()->Reload();
//	RegenParser::Instance()->Reload();
//	GroupParser::Instance()->Reload();
//	StateInfoParser::Instance()->Reload();
//	//ItemOptionParser::Instance()->Reload();
//	ItemCompositeParser::Instance()->Reload();
//	//RareItemDropParser::Instance()->Reload();
//	//ItemDropParser::Instance()->Reload();
//
//	MissionRewardParser::Instance()->Reload();
//	FormulaRatioParser::Instance()->Reload();
//	StyleQuickRegistInfoParser::Instance()->Reload();
//	AIParamParser::Instance()->Reload();
//	ServerOptionParser::Instance()->Reload();
//	EnchantParser::Instance()->Reload();
//	//	GMList::Instance()->Reload(); => 리로드 제대로 안됨!! 준석이는 버그 수정할 것!!
//
//	ExpValueInfoParser::Instance()->Reload();
//	CHuntingRewardParser::Instance()->Reload();
//	RewardInfoList::Instance()->Reload();
//
//	g_Ratio.Release();
//	g_Ratio.Init();
//
//	// 부가적으로 리로딩해야 하는 것
//	g_DropManager.Reload();
//
//	ServerOptionParser::Instance()->Reload();
//
//	PKScriptParser::Instance()->Reload();
//
}

VOID GameServerEx::DisplayServerInfo( BOOL bLog )
{
	if( bLog )
		SUNLOG( eCRITICAL_LOG, "Player Connections		: %d", PlayerManager::Instance()->GetNumberOfPlayers() );	
	else
		DISPMSG(  "Player Connections		: %d\n", PlayerManager::Instance()->GetNumberOfPlayers() );	

	if( m_pSession[AGENT_SERVER] && m_pSession[GAME_DBPROXY] )
	{
		DWORD dwCurAgentSendBuffer = GetIOCPServer()->GetCurSendBuffer( SERVER_IOHANDLER, m_pSession[AGENT_SERVER]->GetSessionIndex() );
		DWORD dwCurAgentRecvBuffer = GetIOCPServer()->GetCurRecvBuffer( SERVER_IOHANDLER, m_pSession[AGENT_SERVER]->GetSessionIndex() );
		DWORD dwCurDBProxySendBuffer = GetIOCPServer()->GetCurSendBuffer( SERVER_IOHANDLER, m_pSession[GAME_DBPROXY]->GetSessionIndex() );
		DWORD dwCurDBProxyRecvBuffer = GetIOCPServer()->GetCurRecvBuffer( SERVER_IOHANDLER, m_pSession[GAME_DBPROXY]->GetSessionIndex() );

		if( dwCurAgentSendBuffer >= 10000 || dwCurAgentRecvBuffer >= 10000 ||
			dwCurDBProxySendBuffer >= 10000 || dwCurDBProxyRecvBuffer >= 10000 )
		{
			if( bLog )
			{
				SUNLOG( eCRITICAL_LOG, "Agent   dwCurSendBuffer[%d]	: dwCurRecvBuffer[%d]", dwCurAgentSendBuffer, dwCurAgentRecvBuffer );
				SUNLOG( eCRITICAL_LOG, "DBProxy dwCurSendBuffer[%d]	: dwCurRecvBuffer[%d]", dwCurDBProxySendBuffer, dwCurDBProxyRecvBuffer );
			}
			else
			{
				DISPMSG(  "Agent   dwCurSendBuffer[%d]	: dwCurRecvBuffer[%d] \n", dwCurAgentSendBuffer, dwCurAgentRecvBuffer );
				DISPMSG(  "DBProxy dwCurSendBuffer[%d]	: dwCurRecvBuffer[%d] \n", dwCurDBProxySendBuffer, dwCurDBProxyRecvBuffer );
			}
		}
	}

	if( bLog )
		SUNLOG( eCRITICAL_LOG, "FUNCTION RESTRICT VALUE		: %u", g_FunctionalRestrictMgr.GetRestrictFunction() );
	else
		DISPMSG(  "FUNCTION RESTRICT VALUE		: %u\n", g_FunctionalRestrictMgr.GetRestrictFunction() );

	//g_ZoneManager.zone_print();
	g_GameZoneManager.Display( bLog );
	g_GameGuildManager.DisplayerGuildInfo( bLog );

	NewGamePartyManager::Instance()->Display();
}

VOID GameServerEx::ReloadAttributes()
{
	// 기능 제한 설정
	DWORD AbuseFunctionRestrict = 0xFFFFFFFF;
	__READ_CONFIG_NUMBER( AbuseFunctionRestrict, "RELOAD_ATTRIBUTES", "ABUSE_FUNCTION_RESTRICT_VALUE", 0xFFFFFFFF, GetDescFileName() );
	if( 0xFFFFFFFF != AbuseFunctionRestrict )
	{
		SUNLOG( eCRITICAL_LOG, "FUCTION_RESTRICT_VALUE=%u", AbuseFunctionRestrict );
		g_FunctionalRestrictMgr.SetRestrictFunction( AbuseFunctionRestrict );
	}

	//// 로그 레벨 설정
	BYTE SunLogLevel = 0xFF;
	__READ_CONFIG_NUMBER( SunLogLevel, "RELOAD_ATTRIBUTES", "SUN_LOG_LEVEL", 0xFF, GetDescFileName() );
	if( 0xFF != SunLogLevel )
	{
		if( g_pSunLog )
		{
			g_pSunLog->SetLogLevel( SunLogLevel );
			SUNLOG( eCRITICAL_LOG, "SUN_LOG_LEVEL=%u", SunLogLevel );
		}
	}
}


BOOL	GameServerEx::CheckServerMode( eSERVER_MODE ServerMode, eSERVER_TYPE ServerType, BYTE byServerPoolType )
{
	if( byServerPoolType > 3 )	return FALSE;

	//1. FIELD, BATTLE_SERVER_TYPE CHECK (반드시 0 ,1이어야 한다.)
	if( ServerMode == eSERVER_LIVE || ServerMode == eSERVER_TESTLIVE )	//라이브나 테스트 라이브면..
	{
		if( ServerType == FIELD_SERVER && byServerPoolType == 0 )
			return TRUE;
		else if( ServerType == BATTLE_SERVER && byServerPoolType == 1 )
			return TRUE;
		else
			return FALSE;
	}

	//라이브나, 테스트 라이브가 아니면 어떤 설정도 OK..
	return TRUE;
}

VOID GameServerEx::LogicInit()
{
	SERVER_POOL_OPTION& stServerPoolOptionInfo = ServerOptionParser::Instance()->GetPoolInfo();

	//-------------------------------------------------------------------------------------------------------------
	// Object Pool 초기화
	DISPMSG(  "Creating object factories..." );
    ObjectFactory* const object_factory = ObjectFactory::Instance();
    object_factory->Init( 
        stServerPoolOptionInfo.m_wPlayerPool, 
        stServerPoolOptionInfo.m_wNPCPool, 
        stServerPoolOptionInfo.m_wItemPool,
        stServerPoolOptionInfo.m_wMapObjectPool, 
        stServerPoolOptionInfo.m_wMapNPCPool, 
        stServerPoolOptionInfo.m_wMerchantPool, 
        100 
    );	
	P_SUCCESS;
	//-------------------------------------------------------------------------------------------------------------

	// 로직 최기화
	DISPMSG(  "Initializing Logic..." );
	AbilityFactory::Instance()->Init( stServerPoolOptionInfo.m_wAbilityPool );
	NewGamePartyManager::Instance()->Initialize( stServerPoolOptionInfo.m_wPartyPool );
	SummonManager::Instance()->Init( stServerPoolOptionInfo.m_wSummonPool );	// 소환은 몇 명 정도가 할까?...몬스터도 포함해서!
	g_StatisticsManager.Init();

    GameZoneManager::ReservedZonePool(20); // zone의 풀 초기화

	UnitRegenManager::CreatePool( 1000 );
	UnitGroupManager::CreatePool( stServerPoolOptionInfo.m_wObjGroupPool );

    QuestManager_Concrete::Init(20);
	g_DropManager.Init(stServerPoolOptionInfo.m_wItemPool);
	g_TradeManager.Init( stServerPoolOptionInfo.m_wTradePool );

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
	g_Ratio.Init();
#endif // _NA_000251_20100727_SOCKET_SYSTEM

	PeakTimeEvent::Instance()->Init();
	g_RewardManager.Init( ItemTypeList::Instance(), DropRatioInfoList::Instance() );
    g_VendorManager.Initialize();
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    g_ExtraNPCManager.initialize();
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    MonsterInvasionEvent::Instance()->Init();
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

	P_SUCCESS;
	//-------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------
	DISPMSG(  "Register PacketHandler..." );
	PacketHandler::Instance();
	P_SUCCESS;
	//-------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------
	DISPMSG(  "Initializing map allocator..." );
#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
	ResrcManager::SetPoolSize( stServerPoolOptionInfo.m_wMapPool, stServerPoolOptionInfo.m_wSectorPool );
	g_ResrcMgr.Establish();
#endif
    // _NA_0_20110613_GAME_SHOP_RENEWAL
    if (this->IsServerInfoFromFile()) {
        LoadGameShopConfiguration();
    };

	P_SUCCESS;
}

// __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
BOOL GameServerEx::NotifyEXPAcquisition(CHARGUID charGuid, eEXP_ACQUISITION_TYPE type, EXPTYPE amount)
{
    MSG_AG_STATUS_GROUP_EXP_ACQUISITION_CMD sendMsg;

    sendMsg.m_CharGuid = charGuid;
    sendMsg.m_byAcquisitionType = type;
    sendMsg.m_Amount = amount;

    return SendToServer(AGENT_SERVER, &sendMsg, sizeof(sendMsg));
}

// implemented by '__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION'
BOOL GameServerEx::NotifyHeimAcquisition(CHARGUID charGuid, eHEIM_ACQUISITION_TYPE type, MONEY amount)
{
    // 다른 서버에서 하임 획득 정보를 활용할 수 있도록 AgentServer로 통지
    MSG_AG_STATUS_HEIM_ACQUISITION_NTF noti;
    noti.m_CharGuid = charGuid;
    noti.m_byAcquisitionType = type;
    noti.m_Amount = amount;
    return SendToServer(AGENT_SERVER, &noti, sizeof(noti));
}

// implemented by '__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION'
// changes input arguments by _NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
BOOL GameServerEx::NotifyHeimConsumption(CHARGUID charGuid, eHEIM_CONSUMPTION_TYPE type, MONEY amount, int lottoAccumRatio)
{
    // 다른 서버에서 하임 소비 정보를 활용할 수 있도록 AgentServer로 통지
    MSG_AG_STATUS_HEIM_CONSUMPTION_NTF noti;
    noti.m_CharGuid = charGuid;
    noti.m_byConsumptionType = type;
    noti.m_Amount = amount;
    noti.m_LottoAccumRatio = lottoAccumRatio; //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    return SendToServer(AGENT_SERVER, &noti, sizeof(noti));
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
bool GameServerEx::MessageBattleGroundOut( eSUN_LOG_LEVEL log_level, const TCHAR* format, ... )
{
    bool result = false;
    va_list args;
    va_start(args, format); 

    if (battle_ground_log_)
    {
        result = CSunLog::LOGGINGwithARG(battle_ground_log_,
            CSunLog::cOUTPUT_FILE,CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);

    return result;
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
BOOL GameServerEx::MessageMondscheinOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, format); 

    if (mondschein_log_)
    {
        result = CSunLog::LOGGINGwithARG(mondschein_log_,
            CSunLog::cOUTPUT_FILE,CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);

    return result;
}
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
BOOL GameServerEx::MessageSonnenScheinOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, format); 

    if (sonnenschein_log_)
    {
        result = CSunLog::LOGGINGwithARG(sonnenschein_log_,
            CSunLog::cOUTPUT_FILE,CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);

    return result;
}
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
BOOL GameServerEx::MessageMaxDamageOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, format); 

    if (max_damage_log)
    {
        result = CSunLog::LOGGINGwithARG(max_damage_log,
            CSunLog::cOUTPUT_FILE,CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);

    return result;
}
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
BOOL GameServerEx::MessageBattleMonitorOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, format); 

    if (battle_monitor_log_)
    {
        result = CSunLog::LOGGINGwithARG(battle_monitor_log_,
            CSunLog::cOUTPUT_FILE,CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);

    return result;
}
#endif //_NA_008706_20160229_TEXTLOG_PK

//==================================================================================================
//==================================================================================================
//==================================================================================================

