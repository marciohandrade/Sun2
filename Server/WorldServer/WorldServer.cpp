#include "stdafx.h"
#include "WorldServer.h"
#ifdef _APPLY_NHN_FILTERFING
    #include <IAF_Export.h>	//lib 사용	
#endif
#include "PacketHandler.h"
#include "AccountDBProxySession.h"
#include "ServerSessionFactory.h"
#include "ServerSessionManager.h"
#include "WorldServerInfoParser.h"
#include "UserFactory.h"
#include "User.h"
#include "WaitingUserList.h"
#include "UserManager.h"
#include "ZoneFactory.h"
#include "ChannelManager.h"
#include <conio.h>
#include <IOCPServer.h>
#include <iostream>
#include <process.h>
#include <PacketStruct_CW.h>
#include <io.h>
//#include <GMList.h>

// 버전 찍기
#include <Version.h>
#include <ServerBuildVersion.h>
#include <ServerGlobal.h>

#include <PacketStruct_ServerCommon.h>

#include <MapInfoParser.h>

// 채널 동접 찍기 테스트용
#include "Channel.h"
#include "ChannelManager.h"

// 채널 채팅용
#include "ChatRoomManager.h"
#include "WorldScriptParserManager.h"
#include <ProhibitionWordParser.h>

#include "Lotto/Lotto.h"
#include "FriendInvitation/FriendInvitationListProc.h"
#include "FriendInvitation/FriendInvitationLevelChangedProc.h"
#include "FriendInvitation/WFriendInvitationController.h"

//=================================================================================================
// 클라쪽 콜백함수
//=================================================================================================
NetworkObject* CreateClientSideAcceptedObject()
{
	return UserFactory::Instance()->Alloc( TEMP_USER );
}

VOID DestroyClientSideAcceptedObject( NetworkObject *pNetworkObject )
{
	UserFactory::Instance()->Free( (User*)pNetworkObject );
}

VOID DestroyClientSideConnectedObject( NetworkObject *pNetworkObject )
{
	assert( 0 );
}

//=================================================================================================
// 서버쪽 콜백함수
//=================================================================================================
NetworkObject* CreateServerSideAcceptedObject()
{
	ServerSession *pServerSession = ServerSessionFactory::Instance()->AllocServerSession( TEMP_SERVER );
	return (NetworkObject*)pServerSession;
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject )
{
	ServerSessionFactory::Instance()->FreeServerSession( (ServerSession*)pNetworkObject );
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject )
{
}

WorldServer::WorldServer()
{
	m_pIOCPServer				= NULL;
	m_bShutdown					= FALSE;
	m_pMasterServer				= NULL;
	m_pGameDBProxy				= NULL;
	m_ServerKey					= 0;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	SetPhaseStatus( PHASE_UNKNOWN );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	m_pWorldScriptParserManager = NULL;
    // (f100611.3L) change to use sync timer on server
    util::TimeSync::Command_Standalone();
}

WorldServer::~WorldServer()
{
	if( m_pIOCPServer )				delete m_pIOCPServer;
	if( m_pMasterServer )			delete m_pMasterServer;
	if( m_pGameDBProxy )			delete m_pGameDBProxy;
}

VOID WorldServer::Release()
{
	MessageOut( eCRITICAL_LOG, "*** WorldServer Close ***" );
    // (f100724.1L) remove a memory leak point
    WFriendInvitationController::DestroyInstance();
    Lotto::GetInstance().Shutdown();

	SetPhaseStatus( PHASE_UNKNOWN );
	WorkLoadManager::DestroyInstance();

	if( m_pIOCPServer ) m_pIOCPServer->Shutdown();
	PacketHandler::Instance()->DestroyInstance();

	UserFactory::Instance()->Release();
	UserFactory::Instance()->DestroyInstance();

	ChatZoneFactory::Instance()->Release();
	ChatZoneFactory::Instance()->DestroyInstance();

	VillageZoneFactory::Instance()->Release();
	VillageZoneFactory::Instance()->DestroyInstance();

	ViewPortZoneFactory::Instance()->Release();
	ViewPortZoneFactory::Instance()->DestroyInstance();

	ChatRoomManager::Instance()->Release();
	ChatRoomManager::Instance()->DestroyInstance();

	UserManager::Instance()->Release();
	UserManager::Instance()->DestroyInstance();

	WaitingUserList::Instance()->Release();
	WaitingUserList::Instance()->DestroyInstance();

	/*ChannelManager::Instance()->Release();
	ChannelManager::Instance()->DestroyInstance();*/

	if( m_pMasterServer )
	{
		m_pMasterServer->Release();
		ServerSessionFactory::Instance()->FreeServerSession( (ServerSession*)m_pMasterServer );
		m_pMasterServer = NULL;
	}

	if( m_pGameDBProxy )
	{
		m_pGameDBProxy->Release();
		ServerSessionFactory::Instance()->FreeServerSession( (ServerSession*)m_pGameDBProxy );
		m_pGameDBProxy = NULL;
	}

	ServerSessionManager::Instance()->DestroyInstance();
	ServerSessionFactory::Instance()->DestroyInstance();

	if( m_pWorldScriptParserManager )
	{
		m_pWorldScriptParserManager->Release();
		m_pWorldScriptParserManager->DestroyInstanceEx();
		SAFE_DELETE( m_pWorldScriptParserManager );
	}

	WorldServerInfoParser::Instance()->DestroyInstance();

	WorldServer::Instance()->m_fnContactTerrain( CT_DELETE_MAPLOADER, m_pLoadHelper );

	if( m_hDll3DTerrain )			
	{
		FreeLibrary( m_hDll3DTerrain );
	}

	SAFE_DELETE(g_pLOG);
	SAFE_DELETE(g_pSunLog);
	SAFE_DELETE(g_pCountLog);

	SAFE_DELETE(m_pIOCPServer);
}

VOID WorldServer::UpdateServerTitle()
{
#ifdef _DEBUG
	DisplayDebugInfo(GetServerType(),
                     (BYTE)WorldServerInfoParser::Instance()->GetServerStatus(),
                     TRUE,
                     C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO,
                     SERVER_RELEASE_BUILD_NO,
                     (BYTE)GetWorldID(), (BYTE)m_ServerKey.GetServerID() );
#else
	DisplayDebugInfo(GetServerType(),
                     (BYTE)WorldServerInfoParser::Instance()->GetServerStatus(),
                     FALSE,
                     C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO,
                     SERVER_RELEASE_BUILD_NO,
                     (BYTE)GetWorldID(), (BYTE)GetChannelID(), (BYTE)m_ServerKey.GetServerID() );
#endif
}

BOOL WorldServer::Init()
{
	// INI 파일 파싱
	if( !WorldServerInfoParser::Instance()->ParseINI( "WorldServer.ini" ) )
	{
		MessageOut(eCRITICAL_LOG,   "WorldServer.ini parsing failed." );
		return FALSE;
	}

	if( WorldServerInfoParser::Instance()->IsServerInfoFromFile() )
	{
		m_ServerKey.Set( (BYTE)WorldServerInfoParser::Instance()->GetWorldID(), 0, 0, 1 );
	}

	UpdateServerTitle();

	WorkLoadManager::CreateInstance();

	// Log 생성
	if( !CreateGameLog() || !CreateSunLog() )
		return FALSE;

	MessageOut( eCRITICAL_LOG, "*** WorldServer Start ***" );
	MessageOut( eCRITICAL_LOG, "*** WorldServer Server Version = %d ***", SERVER_RELEASE_BUILD_NO );

	//맵 로딩용 라이브러리 함수 로드
#ifndef _DEBUG
	// DLL 로딩
	char *lpszDLL = "3DTerrain_s.dll";
#else
	// DLL 로딩
	char *lpszDLL = "3DTerrain_sd.dll";
#endif

	m_hDll3DTerrain = Load3DTerrainLibrary( lpszDLL, &m_fnContactTerrain);
	if ( !m_hDll3DTerrain)
	{
		MessageOut( eCRITICAL_LOG, "dll loading failed.\n" );
		return FALSE;
	}

	m_pLoadHelper = (CLoadHelper *)WorldServer::Instance()->m_fnContactTerrain( CT_NEW_MAPLOADER, 0 );


	INI_IOHANDLER_DESC *pDesc;
	
	// IO 엔진 인스턴스 생성 및 초기화
	m_pIOCPServer = new IOCPServer;
	IOHANDLER_DESC desc[2];
	pDesc = WorldServerInfoParser::Instance()->GetClientIoDesc();
	desc[0].dwIoHandlerKey				= CLIENT_IOHANDLER;
	desc[0].dwMaxAcceptSession			= pDesc->dwMaxAcceptSession;
	desc[0].dwMaxConnectSession			= pDesc->dwMaxConnectSession;
	desc[0].dwSendBufferSize			= pDesc->dwSendBufferSize;
	desc[0].dwRecvBufferSize			= pDesc->dwRecvBufferSize;
	desc[0].dwTimeOut					= pDesc->dwTimeOut;
	desc[0].dwMaxPacketSize				= pDesc->dwMaxPacketSize;
	desc[0].dwNumberOfIoThreads			= pDesc->dwNumberOfIoThreads;
	desc[0].dwNumberOfAcceptThreads		= pDesc->dwNumberOfAcceptThreads;
	desc[0].dwNumberOfConnectThreads	= pDesc->dwNumberOfConnectThreads;
	desc[0].fnCreateAcceptedObject		= CreateClientSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyClientSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyClientSideConnectedObject;
	pDesc = WorldServerInfoParser::Instance()->GetServerIoDesc();
	desc[1].dwIoHandlerKey				= SERVER_IOHANDLER;
	desc[1].dwMaxAcceptSession			= pDesc->dwMaxAcceptSession;
	desc[1].dwMaxConnectSession			= pDesc->dwMaxConnectSession;
	desc[1].dwSendBufferSize			= pDesc->dwSendBufferSize;
	desc[1].dwRecvBufferSize			= pDesc->dwRecvBufferSize;
	desc[1].dwTimeOut					= pDesc->dwTimeOut;
	desc[1].dwMaxPacketSize				= pDesc->dwMaxPacketSize;
	desc[1].dwNumberOfIoThreads			= pDesc->dwNumberOfIoThreads;
	desc[1].dwNumberOfAcceptThreads		= pDesc->dwNumberOfAcceptThreads;
	desc[1].dwNumberOfConnectThreads	= pDesc->dwNumberOfConnectThreads;
	desc[1].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[1].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[1].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;
	m_pIOCPServer->Init( desc, 2 );

	UserFactory::Instance()->Init( desc[0].dwMaxAcceptSession );
	ChatZoneFactory::Instance()->Init();
	VillageZoneFactory::Instance()->Init();
	ViewPortZoneFactory::Instance()->Init();
	UserManager::Instance()->Init();
	WaitingUserList::Instance()->Init();

	m_pWorldScriptParserManager = new WorldScriptParserManager;
	m_pWorldScriptParserManager->RegisterParser();
	m_pWorldScriptParserManager->InitParserPool( NULL );
	#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		m_pWorldScriptParserManager->Load();
	#endif

	// FieldInfo 읽는 루틴이 들어가 있으므로...
	ChannelManager::Instance()->Init();

	m_pGameDBProxy = (ServerSession*)ServerSessionFactory::Instance()->AllocServerSession( GAME_DBPROXY );

	if( WorldServerInfoParser::Instance()->IsServerInfoFromFile() )
	{
		// 게임DB프락시 서버 연결 정보 세팅	
		m_pGameDBProxy->SetAddr( WorldServerInfoParser::Instance()->GetGameDBProxyIP(),
								WorldServerInfoParser::Instance()->GetGameDBProxyPort() );
		MessageOut(eCRITICAL_LOG,   "GameDBProxy server address(%s:%d)", WorldServerInfoParser::Instance()->GetGameDBProxyIP(),
												WorldServerInfoParser::Instance()->GetGameDBProxyPort() );
	}

	// 패킷 핸들러 등록
	PacketHandler::Instance()->RegisterPackets();

	m_pMasterServer = (ServerSession*)ServerSessionFactory::Instance()->AllocServerSession( MASTER_SERVER );

	// 마스터 서버 연결 정보 세팅	
	m_pMasterServer->SetAddr( WorldServerInfoParser::Instance()->GetMasterServerIP(),
							  WorldServerInfoParser::Instance()->GetMasterServerPort() );
	MessageOut(eCRITICAL_LOG,   "Master server address(%s:%d)", WorldServerInfoParser::Instance()->GetMasterServerIP(),
											  WorldServerInfoParser::Instance()->GetMasterServerPort() );

	m_ReconnectTimer.SetTimer( 1000 );
	m_UserManagerTimer.SetTimer( 60000 );	// 사용자 매니저용 타이머

	m_dwShoutInterval = WorldServerInfoParser::Instance()->GetShoutInterval();

	// 채널 채팅 초기화
	ChatRoomManager::Instance()->Init();
	CryptManager::Init1st();

#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    StartWorldServer();
#endif
	return TRUE;
}

VOID WorldServer::StartListen()
{
	TCHAR szListenIP[16]={0,};

	if( !m_pIOCPServer->IsListening( CLIENT_IOHANDLER ) )
	{
		INI_IOHANDLER_DESC *pDesc = WorldServerInfoParser::Instance()->GetClientIoDesc();

		// INI_IOHANDLER_DESC의 IP 주소 길이가 15글자라 XXX.XXX.XXX.XXX 와 같은 IP의 경우 NULL문자 빠지는 오류 발생 가능하여 수정-성현창
		_tcsncpy( szListenIP, pDesc->szIP, MAX_IPADDRESS_SIZE );
		if( !m_pIOCPServer->StartListen( CLIENT_IOHANDLER, szListenIP, pDesc->wPort ) )
		{
			MessageOut(eCRITICAL_LOG,   "Error! Client-side Listen failed!(%s:%d)", szListenIP, pDesc->wPort );
			m_bShutdown = TRUE;
		}
		else
		{
			DISPMSG( "Client-side listen started...(%s:%d)\n", szListenIP, pDesc->wPort );
		}
	}

	if( !m_pIOCPServer->IsListening( SERVER_IOHANDLER ) )
	{
		INI_IOHANDLER_DESC *pDesc = WorldServerInfoParser::Instance()->GetServerIoDesc();

		// INI_IOHANDLER_DESC의 IP 주소 길이가 15글자라 XXX.XXX.XXX.XXX 와 같은 IP의 경우 NULL문자 빠지는 오류 발생 가능하여 수정-성현창
		_tcsncpy( szListenIP, pDesc->szIP, MAX_IPADDRESS_SIZE );

		if( !m_pIOCPServer->StartListen( SERVER_IOHANDLER, szListenIP, pDesc->wPort ) )
		{
			MessageOut(eCRITICAL_LOG,   "Error! Server-side Listen failed!(%s:%d)", szListenIP, pDesc->wPort );
			m_bShutdown = TRUE;
		}
		else
		{
			DISPMSG( "Server-side listen started...(%s:%d)\n", szListenIP, pDesc->wPort );
		}
	}
}

VOID WorldServer::StartListen( char *szIP, WORD wPort, char *szInnerIP, WORD wInnerPort )
{
	if( !m_pIOCPServer->IsListening( CLIENT_IOHANDLER ) )
	{
		if( !m_pIOCPServer->StartListen( CLIENT_IOHANDLER, szIP, wPort ) )
		{
			MessageOut(eCRITICAL_LOG,   "Error! Client-side Listen failed!(%s:%d)", szIP, wPort );
			m_bShutdown = TRUE;
		}
		else
		{
			MessageOut(eCRITICAL_LOG,   "Client-side listen started...(%s:%d)", szIP, wPort );
		}
	}

	if( !m_pIOCPServer->IsListening( SERVER_IOHANDLER ) )
	{
		if( !m_pIOCPServer->StartListen( SERVER_IOHANDLER, szInnerIP, wInnerPort ) )
		{
			MessageOut(eCRITICAL_LOG,   "Error! Server-side Listen failed!(%s:%d)", szInnerIP, wInnerPort );
			m_bShutdown = TRUE;
		}
		else
		{
			MessageOut(eCRITICAL_LOG,   "Server-side listen started...(%s:%d)", szInnerIP, wInnerPort );
		}
	}
}

VOID WorldServer::Run()
{
	if( WorldServerInfoParser::Instance()->IsServerInfoFromFile() )
	{
		ConnectToServer( m_pGameDBProxy, (char*)m_pGameDBProxy->GetConnectIP().c_str(), m_pGameDBProxy->GetConnectPort() );

		WorldServer::Instance()->StartListen();
	}
	else
	{
		// 마스터 서버에 접속
		ConnectToServer( m_pMasterServer, (char*)m_pMasterServer->GetConnectIP().c_str(), m_pMasterServer->GetConnectPort() );
	}

	// 메인 루프
	while( !m_bShutdown )
	{
		Sleep( 1 );

		if( !Update() ) break;
	}
}

BOOL WorldServer::Update()
{
	// I/O 엔진 업데이트
	m_pIOCPServer->Update();

	WorkLoadManager::Update();

	UpdateFPS();

	// 콘솔 처리
	if( !ProcessConsole() )
	{
		return FALSE;
	}

	// 서버 세션 업데이트
	ServerSessionManager::Instance()->Update();

	WaitingUserList::Instance()->Update();

	ChannelManager::Instance()->Update();

    Lotto::GetInstance().Update();
    
    // 리커넥트 체크
	if( m_ReconnectTimer.IsExpired() )
	{
		MaintainConnection();
	}

	if( m_UserManagerTimer.IsExpired() )
	{
	// 암호화 설정한 유저들 타임아웃 체크도 여기서 하자
		UserManager::Instance()->UpdateCryptoUser();
		UserManager::Instance()->UpdateChatBlockUser();
	}

	g_pSunLog->Update();
	g_pCountLog->Update();

#ifdef _JP_0_20101123_PMS
    pms_manager_.Run(); 
#endif

	return TRUE;
}

VOID WorldServer::MaintainConnection()
{
	// 월드 서버가 종료중이면 리턴
	if( m_bShutdown )						return;

	if( !m_pGameDBProxy->IsConnected() )
	{
		ConnectToServer( m_pGameDBProxy, (char*)m_pGameDBProxy->GetConnectIP().c_str(), m_pGameDBProxy->GetConnectPort() );
	}

	// 마스터 서버와 연결이 끊어져 있으면 다시 접속을 시도한다.
	if( WorldServerInfoParser::Instance()->IsServerInfoFromFile() == FALSE )
	{
		if( !m_pMasterServer->IsConnected() )
		{
			ConnectToServer( m_pMasterServer, (char*)m_pMasterServer->GetConnectIP().c_str(), m_pMasterServer->GetConnectPort() );
		}
	}
	else
	{
		if( !m_pGameDBProxy->IsConnected() && !m_pGameDBProxy->GetConnectIP().empty() )
		{
			ConnectToServer( m_pGameDBProxy, (char*)m_pGameDBProxy->GetConnectIP().c_str(), m_pGameDBProxy->GetConnectPort() );
		}
	}
}

BOOL WorldServer::ProcessConsole()
{
	if (_kbhit())
	{
		char ch = static_cast<char>(_getch());
		ch = toupper(ch);

		switch(ch)
		{
		case 0x1b:
			MessageOut( eCRITICAL_LOG, "ESC키에 의한 서버종료시작" );
			return FALSE;

		case ' ':
			DisplayServerInfo();
			break;

		case 'C':
			ChannelManager::Instance()->DisplayChannelInfo();
			ChatRoomManager::Instance()->DisplayChatRoomInfo();
			break;

		case 'N':
			if( WorldServerInfoParser::Instance()->GetFileNotice() )
			{
				ReadAndSendNotice();
			}
			break;
		case 'R':		// 채팅로그 옵션 리로드
			GAMELOG->SetLogAble();
			MessageOut( eCRITICAL_LOG, "Chatting Log Option Reloaded." );
			WorldServerInfoParser::Instance()->ReloadShoutInterval();
			m_dwShoutInterval = WorldServerInfoParser::Instance()->GetShoutInterval();
			MessageOut( eCRITICAL_LOG, "Shouting Interval Option Reloaded[%u]", m_dwShoutInterval );
			WorldServerInfoParser::Instance()->ReloadFileNotice();
			MessageOut( eCRITICAL_LOG, "Notice From File Option Reloaded[%u]", WorldServerInfoParser::Instance()->GetFileNotice() );
			break;
		case 'L':	
			DisplayServerListInfo();
			break;
		case 'T':
			break;
#ifdef _JP_0_20101123_PMS
        case 'J':
            pms_manager_.DisplayPMSInfo();
            break;
#endif
		}
	}

	return TRUE;
}

VOID WorldServer::ReadAndSendNotice()
{
	char szNotice[MAX_NOTICE_LEN+1];
	ZeroMemory( szNotice, sizeof(szNotice) );

	// 파일에서 공지 읽기
	FILE *fp = fopen( "Notice.txt", "rt" );
	if( !fp )
	{
		MessageOut(eCRITICAL_LOG,   "Can't Open File Notice.txt " );
		return;
	}
	fread( szNotice, sizeof(char), MAX_NOTICE_LEN, fp );
	fclose( fp );

	MSG_CW_NOTICE_BRD noticeMsg;
	noticeMsg.m_byCategory	= CW_CHAT;
	noticeMsg.m_byProtocol	= CW_NOTICE_BRD;
	noticeMsg.m_byRed = 255;
	noticeMsg.m_byGreen = 255;
	noticeMsg.m_byBlue = 255;
    // CHANGES: f101210.2L, check boundary and consider length with null-terminated position
    if (size_t str_len = strlen(szNotice) + 1)
    {
        // NOTE: always true, because wLen need consider length with null-terminated position
        strncpy(noticeMsg.szMsg, szNotice, _countof(noticeMsg.szMsg));
        noticeMsg.szMsg[_countof(noticeMsg.szMsg) - 1] = '\0';
        str_len = min(_countof(noticeMsg.szMsg), str_len);
        noticeMsg.wLen = static_cast<uint16_t>(str_len);
    }

	// 전체 공지
	UserManager::Instance()->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

	MessageOut(eCRITICAL_LOG,   ">> WorldNotice(File): %s", szNotice );
}	

VOID WorldServer::ConnectToServer( NetworkObject *pNetworkObject, char *pszIP, WORD wPort )
{
	m_pIOCPServer->Connect( SERVER_IOHANDLER, pNetworkObject, pszIP, wPort );
}

VOID WorldServer::SetGameDBProxyServerAddr( TCHAR* tszDBProxyIP, WORD wDBProxyPort )
{
	m_pGameDBProxy->SetAddr( tszDBProxyIP, wDBProxyPort );
}

VOID WorldServer::ConnectToGameDBProxyServer()
{
	ConnectToServer( m_pGameDBProxy, const_cast<char*>(m_pGameDBProxy->GetConnectIP().c_str()), m_pGameDBProxy->GetConnectPort() );
}

VOID WorldServer::SendGameDBProxy( BYTE *pMsg, WORD wSize )
{
	if( !m_pGameDBProxy )
	{
		MessageOut( eCRITICAL_LOG, "SendGameDB Error - No GameDBProxy" );
		return;
	}

	m_pGameDBProxy->Send( pMsg, wSize );
}

ServerSession* WorldServer::GetGameDBProxy() const
{
    return m_pGameDBProxy;
}

VOID WorldServer::ConnectToDBProxyServer()
{
	//MessageOut(eCRITICAL_LOG,   "Trying to connect to DB proxy server..." );
	//m_pAccountDBProxySession->TryToConnect();	
}

VOID WorldServer::UpdateFPS()
{
	// FPS Check
	static int cnt = 0;
	static DWORD prevTick = GetTickCount();

	cnt++;

//	DWORD curTick = GetTickCount();
	// 글로벌 틱 업데이트
	m_dwGlobalTick = GetTickCount();

	if( prevTick + 1000 < m_dwGlobalTick )
	{
		m_dwFPS = cnt;
		//MessageOut(eCRITICAL_LOG,   "%d FPS", cnt );
		prevTick	= m_dwGlobalTick;
		cnt			= 0;

		if( m_dwFPS < 10 )
		{
			DISPMSG( "============================================================\n" );
			DISPMSG( "============================================================\n" );
			DISPMSG( "[WorldServer::UpdateFPS] FPS Warning!!! [%d] \n", m_dwFPS );
			DISPMSG( "============================================================\n" );
			DISPMSG( "============================================================\n" );
		}
	}
}

VOID WorldServer::DisplayServerInfo()
{
	DISPMSG( "======== World Server ========\n" );
	DISPMSG( " Frame per Second: %d\n", m_dwFPS );
	DISPMSG( " Client Connection: %d  GuidList(((  %d  )))  CharNameList(%d)\n",
			m_pIOCPServer->GetNumberOfConnections( CLIENT_IOHANDLER ),
			UserManager::Instance()->GetNumberOfGuidUsers(),
			UserManager::Instance()->GetNumberOfCharNameUsers()	);
			
	DISPMSG( " Server Connection: %d\n", m_pIOCPServer->GetNumberOfConnections( SERVER_IOHANDLER ) );
	DISPMSG( ">> C: 채널정보  N: 파일공지\n" );
	if( !WorldServerInfoParser::Instance()->IsServerInfoFromFile() )
	{
		if( !m_pMasterServer->IsConnected() )
		{
			DISPMSG( ">> Warning: Not connected to master server.\n" );
		}
	}
	if( !m_pGameDBProxy->IsConnected() )
	{
		DISPMSG( ">> Warning: Not connected to GameDBProxy server.\n" );
	}
}

BOOL WorldServer::IsConnectedTo( eSERVER_TYPE eServerType )
{
	switch( eServerType )
	{
	case MASTER_SERVER:
		return m_pMasterServer->IsConnected();
	case GAME_DBPROXY:
		return m_pGameDBProxy->IsConnected();
	}

	assert( !"그런 서버 타입은 없어요" );

	return FALSE;	
}

BOOL	WorldServer::CreateSunLog()
{
	//SUN Log File 초기화
	SUNLOG_INFO* pSunLogInfo = WorldServerInfoParser::Instance()->GetSunLogInfo();
	if( !pSunLogInfo )		return FALSE;

	DISPMSG( "Creating SunLog File...\n" );
	g_pSunLog = new CSunLog;
	if(g_pSunLog->Init(pSunLogInfo->bySunLogOption, (BYTE)pSunLogInfo->wSunLogfileLevel,
		pSunLogInfo->pszSunLogfilePath, "World") == false)
	{
		delete g_pSunLog;
		return FALSE;
	}

	DISPMSG( "Creating CountLog File...\n" );
	g_pCountLog = new CSunLog;
	if(g_pCountLog->Init(1, (BYTE)pSunLogInfo->wSunLogfileLevel,
		pSunLogInfo->pszSunLogfilePath, "Count") == false)
	{
		delete g_pCountLog;
		return FALSE;
	}

	return TRUE;
}

BOOL	WorldServer::CreateGameLog()
{
	//-------------------------------------------------------------------------------------------------
	// WOPS 로그 파일 정보 초기화
	//-------------------------------------------------------------------------------------------------
	DISPMSG(  "Creating GameLog File...\n" );
	char *szLogPath = WorldServerInfoParser::Instance()->GetLogFilePath();
	if( _access( szLogPath, 0 ) == -1 )
	{
		if( !CreateDirectory(szLogPath, NULL) )
			DISPMSG( "CreateDirectory Fail, path = %s \n", szLogPath  );
	}
	TCHAR szBinPath[MAX_PATH], szTextPath[MAX_PATH];
	strncpy( szBinPath, szLogPath, MAX_PATH );
	strcat( szBinPath, "\\\\WORLD" );
	strncpy( szTextPath, szLogPath, MAX_PATH );
	strcat( szTextPath, "\\\\WORLD_TEXT" );

	g_pLOG = new WorldLog;
	GAMELOG->SetLogAble();
	BYTE byLogOption = WorldServerInfoParser::Instance()->GetLogFileOption();

#ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
	TCHAR szFileName[MAX_PATH];
	_sntprintf( szFileName, MAX_PATH, "%d_SUN_WORLD", NATION_BUILD_VERSION );
	if( !g_pLOG->Init( byLogOption, szFileName, _T(szBinPath), _T(szTextPath) ) )
#else
	if( !g_pLOG->Init( byLogOption, _T("SUN_WORLD"), _T(szBinPath), _T(szTextPath) ) )
#endif
	{
		DISPMSG(  "GameLog File Failed...\n" );
		delete g_pLOG;
		return FALSE;
	}

	return TRUE;
}


VOID	WorldServer::StartWorldServer()
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ChannelManager* pChannelManager = ChannelManager::Instance();
	if( !pChannelManager->LoadFieldInfo() )
	{
		MessageOut( eCRITICAL_LOG, "FieldInfo Load Failed!");
	}

#ifdef _APPLY_NHN_FILTERFING
    static BOOLEAN s_bInited = 0;
    if(s_bInited == 0)
    {
        int result = IAF::LoadAbuseDB();
        // error handling?
        if(result != 0)
        {
            SUNLOG(eCRITICAL_LOG, "Fail to load IAF LoadAbuseDB %d", result);
        }
        s_bInited = true;
    }
#endif

	// 현 시점 PHASE_EXCHANGE일 것이다.
	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;

	WorldServer::Instance()->GetMasterServerSession()->Send( (BYTE*)&msgNTF, sizeof(msgNTF) );
#endif
}

VOID WorldServer::DisplayServerListInfo()
{
	int iWritten=0;

	const int iMaxStr = 4095;
	const CHAR* strfmtServerInfo = "[%02u] %-24s:  %s:%05u\n";
	const CHAR* strfmtServerIPInfo = "[%02u] %-24s:  %u.%u.%u.%u:%05u\n";
	CHAR strServerInfo[iMaxStr+((int)sizeof(CHAR))];	// 4096

	// UNICODE에 주의할 것
	iWritten += _snprintf( strServerInfo, iMaxStr, "============== WorldServer Connected Server List ==============\n" );

	DWORD count = 0;

	SERVER_SESSION_MAP mapSession = ServerSessionManager::Instance()->GetServerSession();
	SERVER_SESSION_MAP_ITER itr;

    for(itr = mapSession.begin(); itr != mapSession.end(); ++itr)	
	{
		ServerSession *pServerSession = itr->second;

		TCHAR* ptstr = GetServerType2String( pServerSession->GetServerType() );
		WORD wPort = pServerSession->GetConnectPort();

		if(wPort)
		{
			iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, strfmtServerInfo,
				pServerSession->GetSessionIndex(), ptstr, pServerSession->GetConnectIP().c_str(), wPort );
		}
		else
		{
			union
			{
				DWORD dwAddr;
				BYTE byAddrT[4];
			} uAddr;
			uAddr.dwAddr = pServerSession->GetIPAddress();
			WORD wPort = pServerSession->GetPort();			

			iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, strfmtServerIPInfo,
				pServerSession->GetSessionIndex(), ptstr,
				uAddr.byAddrT[0], uAddr.byAddrT[1], uAddr.byAddrT[2], uAddr.byAddrT[3], wPort );
		}

		++count;
	}

	iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, " Connected Server Counts : %3d\n", count );
	iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, "===============================================================\n\n" );

	puts( strServerInfo );
}

#ifdef _JP_0_20101123_PMS
VOID WorldServer::SetPMSSessionCount()
{
    pms_manager_.SetSessioCount(
        m_pIOCPServer->GetNumberOfConnections( CLIENT_IOHANDLER ) +
        m_pIOCPServer->GetNumberOfConnections( SERVER_IOHANDLER )
        );
}

VOID WorldServer::SetPMSChannelCount(DWORD channel_count)
{
    pms_manager_.SetChannelCount(channel_count);
}

BOOL PMSAnnounceRequest::OnAnnounceReq(LPCSTR lpszMsg)
{
    MSG_CW_NOTICE_BRD noticeMsg;
    noticeMsg.m_byCategory	= CW_CHAT;
    noticeMsg.m_byProtocol	= CW_NOTICE_BRD;
    noticeMsg.m_byRed = 255;
    noticeMsg.m_byGreen = 255;
    noticeMsg.m_byBlue = 255;
    // CHANGES: f101210.2L, check boundary and fixed try to copy null-message
    if (size_t str_len = strlen(lpszMsg) + 1)
    {
        // NOTE: always true, because wLen need consider with null-terminated position
        strncpy(noticeMsg.szMsg, lpszMsg, _countof(noticeMsg.szMsg));
        noticeMsg.szMsg[_countof(noticeMsg.szMsg) - 1] = '\0';
        str_len = min(_countof(noticeMsg.szMsg), str_len);
        noticeMsg.wLen = static_cast<uint16_t>(str_len);
    }

    // 전체 공지
    UserManager::Instance()->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

    MessageOut(eCRITICAL_LOG,   ">> WorldNotice(PMS): %s", noticeMsg.szMsg );

    return TRUE;
}
#endif

//==================================================================================================
//==================================================================================================
//==================================================================================================

