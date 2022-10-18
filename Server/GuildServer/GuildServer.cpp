#include "stdafx.h"
#include <io.h>
#include <IOCPServer.h>
#include <NetworkObject.h>


//=========================================================================================================
// GuildServer Include Files
//

#include "GuildServer.h"
#include "PacketHandler/PacketHandler.h"

#include <TempServerSession.h>
#include <Sessions/DBProxyServerSession.h>
#include <Sessions/MasterServerSession.h>
#include <Sessions/GameServerSession.h>

// Internal Function Headers
#include "Guild/GuildManager.h"
#include "GuildScriptParserManager.h"
#include <GuildRankInfoParser.h>

#include <ServerBuildVersion.h>
#include "DominationManager.h"

//#define temp_for_test

#define CHECK_PARAM_NUMBER( variable_type, variable, szSectionName, keyName, defValue )		\
	variable_type variable = 0;																\
	__READ_CONFIG_NUMBER( variable, szSectionName, keyName, defValue, GetDescFileName() );	\
	if( variable == defValue ) return FALSE
#define CHECK_PARAM_STRING( variable, szSectionName, keyName, defValue )		\
	char variable [128] = {0,};																\
	__READ_CONFIG_STRING( variable, szSectionName, keyName, defValue, GetDescFileName() );	\
	if( variable == defValue ) return FALSE


#include <boost/shared_array.hpp>


/// @brief 환경 설정 파일에서 문자열 값을 안전하게 읽어온다.
/// @param section_name 섹션 이름
/// @param key_name 키값 이름
/// @param default_value 기본값
/// @param ini_file_name 환경설정 파일 이름
/// @param min_read_count 읽어올 문자열의 최소 개수
/// @param max_read_count 읽어올 문자열의 최대 개수
/// @return 정상적으로 읽어 왔으면 읽어온 문자열, 아니면 NULL값을 반환한다.
boost::shared_array<TCHAR> ReadConfigStringFromIni(
    const TCHAR* section_name, 
    const TCHAR* key_name, 
    const TCHAR* default_value, 
    const TCHAR* ini_file_name, 
    int min_read_count = 0,  
    int max_read_count = 1000
    )
{
    boost::shared_array<TCHAR> read_string_ptr;
    
    // 입력값을 검사한다.
    if (NULL == section_name || NULL == key_name || NULL == ini_file_name || min_read_count < 0 || 
        min_read_count > max_read_count)
    {
        return read_string_ptr;
    }

    read_string_ptr.reset(new TCHAR[MAX_PATH]);

    // 먼저 읽어올 문자열 길이를 검사한다.
    int read_character_count = ::GetPrivateProfileString(section_name, key_name, default_value, 
        read_string_ptr.get(), MAX_PATH, ini_file_name);    
    if (read_character_count < min_read_count || read_character_count > max_read_count)
    {
        read_string_ptr.reset(NULL);
        return read_string_ptr;
    }

    read_string_ptr.reset(new TCHAR[read_character_count+1]);

    // 읽어온다.
    DWORD new_read_character_count = ::GetPrivateProfileString(section_name, key_name, default_value, 
        read_string_ptr.get(), MAX_PATH, ini_file_name);

    // 성공적으로 읽어 왔는지 검사한다.
    if (new_read_character_count != read_character_count)
    {
        read_string_ptr.reset(NULL);
        return read_string_ptr;
    }

    return read_string_ptr;
} // ReadConfigStringFromIni


GuildServer::GuildServer()
{
	SetPhaseStatus( PHASE_UNKNOWN );
	ConnectSessions.Init();

	m_pPoolRouter = new SERVER_SESSION_ROUTER;
	SERVER_SESSION_ROUTER& rRouter = GetPoolRouter();

	rRouter.Register<AgentServerSession>(AGENT_SERVER);
	rRouter.Register<DBProxyServerSession>(GAME_DBPROXY);
	rRouter.Register<WorldServerSession>(WORLD_SERVER);
	rRouter.Register<MasterServerSession>(MASTER_SERVER);
	rRouter.Register<GameServerSession>(GAME_SERVER);
	//rRouter.Register<TempServerSessionEx>(TEMP_SERVER);

	rRouter.Initialize( AGENT_SERVER, 5, 1 );
	rRouter.Initialize( GAME_DBPROXY, 1, 1 );
	rRouter.Initialize( WORLD_SERVER, 1, 1 );
	rRouter.Initialize( MASTER_SERVER, 1, 1 );
	//rRouter.Initialize( TEMP_SERVER, 10, 5 );
	rRouter.Initialize( GAME_SERVER, 15, 5 );

	m_TempServerSessionPool.Initialize( 10, 5 );
	TempServerSession::SetPool( &m_TempServerSessionPool );
	// Register Object Code
}

GuildServer::~GuildServer()
{
	SetPhaseStatus(PHASE_UNKNOWN);
	ConnectSessions.Init();

	SAFE_DELETE( m_pPoolRouter );
}


BOOL	GuildServer::Init()
{
	InitializeLogger();

	MessageOut( eCRITICAL_LOG, "*** GuildServer Start *** " );
	MessageOut( eCRITICAL_LOG, "*** GuildServer Server Version = %d ***", SERVER_RELEASE_BUILD_NO );

	// (서비스/콘솔)모드 제어 확인할 것.
	__READ_CONFIG_NUMBER( m_bServiceMode, "COMMON", "SERVICE_MODE", 0, GUILD_SERVER_INFO_FILE_NAME );

	WorkLoadManager::CreateInstance();
	PacketHandler::RegisterHandler();

	if( !Init_ScriptData( this->IsServerInfoFromFile() ) )
	{
		P_FAILED;
		return FALSE;
	}
	P_SUCCESS;

	DISPMSG(  "Creating GuildLog File..." );
	g_pLOG = new GuildLog();
	CHECK_PARAM_STRING( pszGameLogfilePath,		"LOGFILE", "WOPS_LOGFILE_PATH", NULL );
	if( _access( pszGameLogfilePath, 0 ) == -1 )		//폴더가 존재하지 않으면..
	{
		if( !CreateDirectory( pszGameLogfilePath, NULL) )
			printf( "CreateDirectory Fail, path = %s \n", pszGameLogfilePath  );
	}
	TCHAR szBinPath[MAX_PATH], szTextPath[MAX_PATH];
	_sntprintf( szBinPath, MAX_PATH, "%s\\\\GUILD", pszGameLogfilePath );
	_sntprintf( szTextPath, MAX_PATH, "%s\\\\GUILD_TEXT", pszGameLogfilePath );
	CHECK_PARAM_NUMBER( BYTE, byWopsLogOption, "LOGFILE", "WOPS_LOGFILE_OPTION", 0xFF );
#ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
	TCHAR szFileName[MAX_PATH];
	_sntprintf( szFileName, MAX_PATH, "%d_SUN_GUILD", NATION_BUILD_VERSION );
	if( !g_pLOG->Init( byWopsLogOption, szFileName, _T(szBinPath), _T(szTextPath) ) )
#else
	if( !g_pLOG->Init( byWopsLogOption, _T("SUN_GUILD"), _T(szBinPath), _T(szTextPath) ) )
#endif
	{ P_FAILED; }
	else
	{ P_SUCCESS; }

#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    StartServer();
#endif

	return TRUE;
}

VOID	GuildServer::Release()
{
	WorkLoadManager::DestroyInstance();
	PacketHandler::ReleaseHandler();

	MessageOut( eCRITICAL_LOG, "*** GuildServer Close *** " );

	if( m_pGuildScriptParserManager )
	{
		m_pGuildScriptParserManager->Release();
		m_pGuildScriptParserManager->DestroyInstanceEx();
		SAFE_DELETE( m_pGuildScriptParserManager );
	}
    DominationManager::DestroyInstance();
	SAFE_DELETE(g_pSunLog);
	SAFE_DELETE(g_pLOG);
}

BOOL	GuildServer::Update( DWORD tick )
{
	// <2> Server Session 관리
	WorkLoadManager::Update();

	// SunLog Update 위치나 종료시점의 로그 쓰기 등을 고려해서 재설정할 것.
	g_pSunLog->Update();

	if(m_GuildDestroyTimer.IsExpired())
		g_GuildManager.UpdateForGuildDestroy();

    DominationManager::Instance()->LoadDominationInfo();
	return TRUE;
}

// <IMPLEMENTATION><SERVERFRAME><INTERFACE>
ServerSession*	GuildServer::AllocServerSession( eSERVER_TYPE serverType )
{
	ServerSession* pServerSession = NULL;

	if( TEMP_SERVER != serverType )
	{
		SERVER_SESSION_ROUTER& rRouter = GetPoolRouter();
		if( BATTLE_SERVER == serverType || FIELD_SERVER == serverType )
			serverType = GAME_SERVER;

		pServerSession = (ServerSession*)rRouter.Alloc( serverType );

		switch( serverType )
		{
		case GAME_DBPROXY:
			ConnectSessions.SessionDBProxy() = (DBProxyServerSession*)pServerSession;
			break;
		case MASTER_SERVER:
            ConnectSessions.SessionMaster() = (MasterServerSession*)pServerSession;
			break;
		case WORLD_SERVER:
            ConnectSessions.SessionWorld() = (WorldServerSession*)pServerSession;
			break;
		}
	}
	else
	{
		pServerSession = (ServerSession*)TempServerSession::ALLOC();
	}

	if( !pServerSession )
	{
		ASSERT(!"존재하지 않는 서버타입입니다."); return NULL;
	}

	pServerSession->Init( this );
	return pServerSession;
}

// <IMPLEMENTATION><SERVERFRAME><INTERFACE>
VOID			GuildServer::FreeServerSession( ServerSession* pServerSession )
{
	if( !pServerSession )
		return;

	eSERVER_TYPE serverType = pServerSession->GetServerType();
	pServerSession->Release();

	if( TEMP_SERVER != serverType )
	{
		SERVER_SESSION_ROUTER& rRouter = GetPoolRouter();
		eSERVER_TYPE serverType = pServerSession->GetServerType();

		if( BATTLE_SERVER == serverType || FIELD_SERVER == serverType )
			serverType = GAME_SERVER;

		rRouter.Free( (DWORD)serverType, pServerSession );
	}
	else
	{
		TempServerSession::FREE( (TempServerSession*)pServerSession );
	}
}

VOID			GuildServer::SendToAllGameServers( MSG_BASE* pMsg, WORD wSize )
{
	const DWORD cdwBitMask = (DWORD)( (1<<GAME_SERVER) | (1<<FIELD_SERVER) | (1<<BATTLE_SERVER) );

	ServerFrame::SERVER_MAP& rSERVERMAP = GetSessionList();

	ServerFrame::SERVER_MAP::iterator it = rSERVERMAP.begin();
	ServerFrame::SERVER_MAP::iterator end = rSERVERMAP.end();

	for( ; it != end ; ++it )
	{
		ServerSession* pSession = it->second;
		if( ((DWORD)(1<<pSession->GetServerType())) & cdwBitMask )
		{
			pSession->SendPacket( pMsg, wSize );
		}
	}
}

BOOL			GuildServer::ConnectTo( eSERVER_TYPE serverType )
{
	ServerSession* pSession = NULL;
	switch( serverType )
	{
	case GAME_DBPROXY:
		pSession = ConnectSessions.SessionDBProxy(); break;
	case WORLD_SERVER:
		pSession = ConnectSessions.SessionWorld(); break;
	}

	if( !pSession ) return FALSE;

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( pSession->IsConnecting() || pSession->IsConnected() )
	{
		ASSERT( !"중복 Connect 시도" );
		return FALSE;
	}

	Connect(pSession);

	return TRUE;

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( ! IsServerInfoFromFile() )
	{
		if( (MASTER_SERVER != serverType)
			&& (GetPhaseStatus() != PHASE_SERVICE) )
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

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

//
//
//
//
//

#include <PacketStruct_ServerCommon.h>

VOID	GuildServer::StartServer()
{
	// <해야 할 일>
	//
	// <NOTICE>
	//   이것이 호출되었다는 의미는 필요한 모든 데이터를 받았다는 의미다.
	// <THINKING>

	GuildServer* pGuildServer = GuildServer::GetMainFrame();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 현 시점 PHASE_EXCHANGE일 것이다.
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;
	pGuildServer->ConnectSessions.SendToMaster( &msgNTF, sizeof(msgNTF) );
#else
    if (pGuildServer->IsServerInfoFromFile())
#endif //
    {
        pGuildServer->Listen(SERVER_IOHANDLER);
    };
}


BOOL GuildServer::InitializeLogger()
{
    // 로그 파일 옵션을 읽는다.
	BYTE sun_log_option = 0;
	__READ_CONFIG_NUMBER(sun_log_option, TEXT("LOGFILE"), TEXT("SUN_LOGFILE_OPTION"), 0, 
        TEXT(GUILD_SERVER_INFO_FILE_NAME));
	
    // 로그 파일 레벨을 읽는다.
    BYTE sun_log_file_level = 0;
    __READ_CONFIG_NUMBER(sun_log_file_level, TEXT("LOGFILE"), TEXT("SUN_LOGFILE_LEVEL"), 0, 
        TEXT(GUILD_SERVER_INFO_FILE_NAME));
	
    // 로그 파일 경로를 읽는다.
    typedef boost::shared_array<TCHAR> CharSmartPtr;
    const CharSmartPtr& sun_log_file_path_ptr = ReadConfigStringFromIni(TEXT("LOGFILE"), 
        TEXT("SUN_LOGFILE_PATH"), NULL, GUILD_SERVER_INFO_FILE_NAME, 3);
    if (!sun_log_file_path_ptr)
    {
        DISPMSG(TEXT("Fail to loading 'SUN_LOGFILE_PATH' from ini file.\n"));
        return FALSE;
    }

    // 로그를 초기화 한다.
	DISPMSG( "SUNLOG.Guild Initializing...\t" );

    g_pSunLog = new CSunLog;	
    BOOL is_success = g_pSunLog->Init(sun_log_option, sun_log_file_level, sun_log_file_path_ptr.get(), 
        TEXT("Guild"));
	if (is_success)	
    { 
        P_SUCCESS; 
    }
	else		
    { 
        P_FAILED; 
    }

	return is_success;
}

BOOL	GuildServer::Init_ScriptData( BOOL bReadFromFile )
{
	m_pGuildScriptParserManager = new GuildScriptParserManager;
	m_pGuildScriptParserManager->InitParserPool( NULL );
	m_pGuildScriptParserManager->RegisterParser();

	#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		return TRUE;
	#else
		return m_pGuildScriptParserManager->Load();
	#endif
}

//
//=======================================================================================================
BOOL	GuildServer::sConnectSession::SendToMaster( MSG_BASE* pMsg, WORD wSize )
{
	if( m_pMasterServerSession && m_pMasterServerSession->IsConnected() )
	{
		return m_pMasterServerSession->SendPacket( pMsg, wSize );
	}

	return FALSE;
}

BOOL	GuildServer::sConnectSession::SendToDBProxy( MSG_BASE* pMsg, WORD wSize )
{
	if( m_pDBProxyServerSession && m_pDBProxyServerSession->IsConnected() )
	{
		return m_pDBProxyServerSession->SendPacket( pMsg, wSize );
	}

	return FALSE;
}

BOOL	GuildServer::sConnectSession::SendToWorld( MSG_BASE* pMsg, WORD wSize )
{
	if( m_pWorldServerSession && m_pWorldServerSession->IsConnected() )
	{
		return m_pWorldServerSession->SendPacket( pMsg, wSize );
	}

	return FALSE;
}
//=======================================================================================================
//

//
// Callback Handlers
NetworkObject*	GuildServer::CreateServerSideAcceptedObject()
{
	return GuildServer::GetMainFrame()->AllocServerSession( TEMP_SERVER );
}

VOID			GuildServer::DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject )
{
	GuildServer::GetMainFrame()->FreeServerSession( (ServerSession*)pNetworkObject );
}

VOID			GuildServer::DestroyServerSideConnectedObject( NetworkObject* pNetworkObject )
{

}


BOOL	GuildServer::KeyEvent( CHAR ch )
{
	switch(ch)
	{
	case ' ':
		DisplayServerInfo();
		break;
	}

	return TRUE;
}

VOID GuildServer::DisplayServerInfo()
{
	g_GuildManager.DisplayGuildInfo();
}


//==================================================================================================
//==================================================================================================
//==================================================================================================

