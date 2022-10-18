#include "stdafx.h"
#include <conio.h>
#include <IOCPServer.h>
#include <GMList.h>
#include "MasterServer.h"
#include "ServerSession.h"
#include "AccountDBProxySession.h"
#include "OpServerSession.h"
#include "PacketHandler.h"
#include "ServerSessionFactory.h"
#include "ServerSessionManager.h"
#include "ServerInfoManager.h"
#include "MasterServerInfoParser.h"

// 종료 프로토콜 위해
#include "Protocol_ServerCommon.h"
#include "PacketStruct_ServerCommon.h"
#include "PacketStruct_MO.h"

// 버전 찍기
#include <Version.h>
#include <ServerBuildVersion.h>
#include <ServerGlobal.h>

#include "GameServerSession.h"
#include <PacketStruct_GM.h>

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
#include "SolarAuthServerSession.h"
#endif

//#define _TEST_ABUSE

#ifdef _TEST_ABUSE
#include <SolarDateTime.h>
#endif

// 통계 위해
#include <StatisticsManager.h>

//=================================================================================================
// 콜백함수
//=================================================================================================
NetworkObject* CreateAcceptedObject()
{
	ServerSession *pServerSession = ServerSessionFactory::Instance()->AllocServerSession( TEMP_SERVER );
	return (NetworkObject*)pServerSession;
}

//=================================================================================================
// 콜백함수
//=================================================================================================
VOID DestroyAcceptedObject( NetworkObject *pNetworkObject )
{
	ServerSessionFactory::Instance()->FreeServerSession( (ServerSession*)pNetworkObject );
}

//=================================================================================================
// 콜백함수
//=================================================================================================
VOID DestroyConnectedObject( NetworkObject *pNetworkObject )
{
}

MasterServer::MasterServer()
{
	m_pIOCPServer				= NULL;
	m_bShutdown					= FALSE;
	m_pAccountDBProxySession	= NULL;
	m_pOpServerSession			= NULL;

	m_pServerMoneyInfoTable		= NULL;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	SetPhaseStatus(PHASE_UNKNOWN);
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

MasterServer::~MasterServer()
{
	if( m_pIOCPServer )				delete m_pIOCPServer;
	if( m_pAccountDBProxySession )	delete m_pAccountDBProxySession;
	if( m_pOpServerSession )		delete m_pOpServerSession;

	ASSERT( m_pServerMoneyInfoTable );
}

VOID MasterServer::Release()
{
	m_pIOCPServer->Shutdown();

	if( m_pAccountDBProxySession )
	{
		m_pAccountDBProxySession->Release();
	}
	if( m_pOpServerSession )
	{
		m_pOpServerSession->Release();
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	WorkLoadManager::DestroyInstance();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	GMList::Instance()->Release();

	m_pServerMoneyInfoTable->RemoveAll();
	SAFE_DELETE( m_pServerMoneyInfoTable );

	m_bExitServer = FALSE;
	m_eExitType = eEXIT_NONE;
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
	m_pExitAgentTable->RemoveAll();
	SAFE_DELETE( m_pExitAgentTable );
#endif
	m_pExitServerTable->RemoveAll();
	SAFE_DELETE( m_pExitServerTable );
	m_pExitDBServerTable->RemoveAll();
	SAFE_DELETE( m_pExitDBServerTable );
	SAFE_DELETE(g_pSunLog);
}

BOOL MasterServer::Init()
{
#ifdef _TEST_ABUSE
	OLD_SERVER_KEY	oldKey;

	SERVER_KEY testKey;

	testKey.Set(1,2,3,4);
	oldKey = testKey;
#endif

	// INI 파일 파싱
	if( !MasterServerInfoParser::Instance()->ParseINI( "MasterServer.ini" ) )
	{
		printf( "MasterServer.ini parsing failed.\n" );
		return FALSE;
	}

	// 버전 찍기
#ifdef _DEBUG
	DisplayDebugInfo( GetServerType(), 0, TRUE, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, SERVER_RELEASE_BUILD_NO, 0, 0, 1 );
#else
	DisplayDebugInfo( GetServerType(), 0, FALSE, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, SERVER_RELEASE_BUILD_NO, 0, 0, 1 );
#endif

	//SUN Log File 초기화
	g_pSunLog = new CSunLog;
	SUNLOG_INFO* pSunLogInfo = MasterServerInfoParser::Instance()->GetSunLogInfo();
	if(g_pSunLog->Init(pSunLogInfo->bySunLogOption, (BYTE)pSunLogInfo->wSunLogfileLevel,
		pSunLogInfo->pszSunLogfilePath, "Master") == false)
	{
		//SunLog를 만들지 않습니다.
	}

	// IO 엔진 인스턴스 생성 및 초기화
	m_pIOCPServer = new IOCPServer;

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	IOHANDLER_DESC desc;
	desc.dwIoHandlerKey				= IOHANDLER_KEY;
	desc.dwMaxAcceptSession			= MasterServerInfoParser::Instance()->GetIoDesc()->dwMaxAcceptSession;
	desc.dwMaxConnectSession		= MasterServerInfoParser::Instance()->GetIoDesc()->dwMaxConnectSession;
	desc.dwSendBufferSize			= MasterServerInfoParser::Instance()->GetIoDesc()->dwSendBufferSize;
	desc.dwRecvBufferSize			= MasterServerInfoParser::Instance()->GetIoDesc()->dwRecvBufferSize;
	desc.dwTimeOut					= MasterServerInfoParser::Instance()->GetIoDesc()->dwTimeOut;
	desc.dwMaxPacketSize			= MasterServerInfoParser::Instance()->GetIoDesc()->dwMaxPacketSize;
	desc.dwNumberOfIoThreads		= MasterServerInfoParser::Instance()->GetIoDesc()->dwNumberOfIoThreads;
	desc.dwNumberOfAcceptThreads	= MasterServerInfoParser::Instance()->GetIoDesc()->dwNumberOfAcceptThreads;
	desc.dwNumberOfConnectThreads	= MasterServerInfoParser::Instance()->GetIoDesc()->dwNumberOfConnectThreads;
	desc.fnCreateAcceptedObject		= CreateAcceptedObject;
	desc.fnDestroyAcceptedObject	= DestroyAcceptedObject;
	desc.fnDestroyConnectedObject	= DestroyConnectedObject;

	m_pIOCPServer->Init( &desc, 1 );

	// GM List 로딩
	//GMList::Instance()->Load( "data\\GMList.txt" );

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	INI_IOHANDLER_DESC* pIO_DESC = NULL;
	IOHANDLER_DESC desc[2];

	IOHANDLER_DESC& descINSIDE = desc[INSIDE_IOHANDLER];

	pIO_DESC = MasterServerInfoParser::Instance()->GetIoDesc();

	descINSIDE.dwIoHandlerKey				= INSIDE_IOHANDLER;
	descINSIDE.dwMaxAcceptSession			= pIO_DESC->dwMaxAcceptSession;
	descINSIDE.dwMaxConnectSession			= pIO_DESC->dwMaxConnectSession;
	descINSIDE.dwSendBufferSize				= pIO_DESC->dwSendBufferSize;
	descINSIDE.dwRecvBufferSize				= pIO_DESC->dwRecvBufferSize;
	descINSIDE.dwTimeOut					= pIO_DESC->dwTimeOut;
	descINSIDE.dwMaxPacketSize				= pIO_DESC->dwMaxPacketSize;
	descINSIDE.dwNumberOfIoThreads			= pIO_DESC->dwNumberOfIoThreads;
	descINSIDE.dwNumberOfAcceptThreads		= pIO_DESC->dwNumberOfAcceptThreads;
	descINSIDE.dwNumberOfConnectThreads		= pIO_DESC->dwNumberOfConnectThreads;
	descINSIDE.fnCreateAcceptedObject		= CreateAcceptedObject;
	descINSIDE.fnDestroyAcceptedObject		= DestroyAcceptedObject;
	descINSIDE.fnDestroyConnectedObject		= DestroyConnectedObject;

	pIO_DESC = MasterServerInfoParser::Instance()->GetIoDescOutside();

	IOHANDLER_DESC& descOUTSIDE = desc[OUTSIDE_IOHANDLER];
	descOUTSIDE.dwIoHandlerKey				= OUTSIDE_IOHANDLER;
	descOUTSIDE.dwMaxAcceptSession			= pIO_DESC->dwMaxAcceptSession;
	descOUTSIDE.dwMaxConnectSession			= pIO_DESC->dwMaxConnectSession;
	descOUTSIDE.dwSendBufferSize			= pIO_DESC->dwSendBufferSize;
	descOUTSIDE.dwRecvBufferSize			= pIO_DESC->dwRecvBufferSize;
	descOUTSIDE.dwTimeOut					= pIO_DESC->dwTimeOut;
	descOUTSIDE.dwMaxPacketSize				= pIO_DESC->dwMaxPacketSize;
	descOUTSIDE.dwNumberOfIoThreads			= pIO_DESC->dwNumberOfIoThreads;
	descOUTSIDE.dwNumberOfAcceptThreads		= pIO_DESC->dwNumberOfAcceptThreads;
	descOUTSIDE.dwNumberOfConnectThreads	= pIO_DESC->dwNumberOfConnectThreads;
	descOUTSIDE.fnCreateAcceptedObject		= CreateAcceptedObject;		//다른 것으로 대체할 것
	descOUTSIDE.fnDestroyAcceptedObject		= DestroyAcceptedObject;	//다른 것으로 대체할 것
	descOUTSIDE.fnDestroyConnectedObject	= DestroyConnectedObject;

	m_pIOCPServer->Init( desc, 2 );

	//// GM List 로딩
	//GMList::Instance()->Load( "data\\GMList.txt" );

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	// 패킷 핸들러 등록
	PacketHandler::Instance()->RegisterHandler_DM();
	PacketHandler::Instance()->RegisterHandler_MX();
	PacketHandler::Instance()->RegisterHandler_BM();
	PacketHandler::Instance()->RegisterHandler_FM();
	PacketHandler::Instance()->RegisterHandler_GM();
	PacketHandler::Instance()->RegisterHandler_AM();
	PacketHandler::Instance()->RegisterHandler_MW();
	PacketHandler::Instance()->RegisterHandler_MO();

	m_pAccountDBProxySession = (AccountDBProxySession*)ServerSessionFactory::Instance()->AllocServerSession( ACCOUNT_DBPROXY );
	m_pOpServerSession = (OpServerSession*)ServerSessionFactory::Instance()->AllocServerSession( OP_SERVER );

	// DB Proxy 서버 연결 정보 세팅	
	m_pAccountDBProxySession->SetAddr( MasterServerInfoParser::Instance()->GetAccountDBProxyIP(),
									  MasterServerInfoParser::Instance()->GetAccountDBProxyPort() );
	printf( "Account DB proxy address(%s:%d)\n", MasterServerInfoParser::Instance()->GetAccountDBProxyIP(),
												MasterServerInfoParser::Instance()->GetAccountDBProxyPort() );
	// Op Server 연결 정보 세팅
	m_pOpServerSession->SetAddr( MasterServerInfoParser::Instance()->GetOpServerIP(),
									MasterServerInfoParser::Instance()->GetOpServerPort() );

	MessageOut( eCRITICAL_LOG, "Op server address(%s:%d)", MasterServerInfoParser::Instance()->GetOpServerIP(),
											MasterServerInfoParser::Instance()->GetOpServerPort() );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	PacketHandler::Instance()->RegisterHandler_SM();
	m_pSolarAuthServerSession = (SolarAuthServerSession*)ServerSessionFactory::Instance()->AllocServerSession( SOLARAUTH_SERVER );
	// Solar Auth 서버 연결 정보 세팅	
	m_pSolarAuthServerSession->SetAddr( MasterServerInfoParser::Instance()->GetSolarAuthServerIP(),
		MasterServerInfoParser::Instance()->GetSolarAuthServerPort() );
	printf( "SolarAuthServer IP address(%s:%d)\n", MasterServerInfoParser::Instance()->GetSolarAuthServerIP(),
		MasterServerInfoParser::Instance()->GetSolarAuthServerPort() );

	WorkLoadManager::CreateInstance();
	
	sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_CODE_KEY(LIST_SIZE)] =
	{
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ACCESSORYITEMINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ACSCRIPT), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(AICOMMONINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(AITYPEINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ARMORITEMINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ARMORTYPEDAMAGEINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(COMPOSITE_NPC), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(DEC1), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(DEC2), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(DEVCOMMAND), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(DROPRATIOINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ENC1), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ENC2), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ENCHANT), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(EXPVALUEINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(EXTRASTONEOPTIONINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(FATIGUEOPTION), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(FIELD), NULL),
		//SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(FILELIST), NULL),
		//	break;
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(FORMULARATIO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(GMCOMMAND), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(GMLIST), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(GROUP), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(INITCHARINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(INITCHARINFOLIST), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMCOMPOSELIST), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMCOMPOSITEINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMCOMPOSITERESULTINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMCRYSTALIZELIST), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMDROPINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMDROPPERGRADE), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMTYPEDROPRATIO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(ITEMTYPELIST), NULL),
		//SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(LIMITEDMAPPORT), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(MAPENTERANCEPORTAL), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(MATCHINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(MISSIONREWARDINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(NOTICE_SAMPLE), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(NPCEXTRAINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(NPCINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(PKSCRIPT), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(QUESTINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(RANKOPTIONINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(RAREITEMDROPINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(REGEN), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(REGEN_POS), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(REQUIREFIELDLEVELINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(REWARDINFOLIST), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SHOPINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SHOUTRANGE), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SKILLINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SOCKETOPTIONINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(STATEINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(STYLEINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(STYLEQUICKREGISTINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SUMMON), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SUNEVENTINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(SUPERUSERLIST), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(THEMOLAC), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(THEMOLAD), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(THEMOLAS), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(VSHUNTINGREWARDINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(WASTEITEMINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(WEAPONITEMINFO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(WORLD), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(FIELDWMO), NULL),
		SCRIPT_DATA_INFO_SET(SCRIPT_CODE_KEY(FIELDWUT), NULL),
		SCRIPT_DATA_INFO_SET_EMPTY(),
	};
	ScriptDataManager::RegisterDataList( pNodeTable );
	ScriptDataManager::RegisterCallbackAllDataLoaded( MasterServer::StartDataService );
#endif
											

	m_ReconnectTimer.SetTimer( 1000 );

	// 서버 통화량 정보 초기화
	m_pServerMoneyInfoTable = new util::SolarHashTable<SERVER_MONEY_INFO*>;
	m_pServerMoneyInfoTable->Initialize( 100 );

	// 서버 종료 정보 초기화
	m_bExitServer = FALSE;
	m_eExitType = eEXIT_NONE;
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
	m_pExitAgentTable = new util::SolarHashTable<SERVER_KEY*>;
	m_pExitAgentTable->Initialize( 100 );
#endif
	m_pExitServerTable = new util::SolarHashTable<SERVER_KEY*>;
	m_pExitServerTable->Initialize( 1000 );		// 최대 1000개 서버 종료 가능?
	m_pExitDBServerTable = new util::SolarHashTable<SERVER_KEY*>;
	m_pExitDBServerTable->Initialize( 100 );	// 최대 100개 서버 종료 가능?
	m_ExitTimer.SetTimer( MasterServerInfoParser::Instance()->GetExitInterval() );	// 종료 응답 대기 시간
	m_ExitTimer.DisableCheckTime();

	// 통화량 주기 초기화
	m_MoneyTimer.SetTimer( MasterServerInfoParser::Instance()->GetStatisticsInterval() );

	return TRUE;
}

VOID MasterServer::StartListen()
{
	// DB Proxy에 연결된 후에 리슨 시작
	if( !m_pIOCPServer->IsListening( INSIDE_IOHANDLER ) )
	{
		char szMasterServerIP[MAX_IPADDRESS_SIZE+1]={0,};
		WORD wMasterServerPort;

		strncpy( szMasterServerIP, MasterServerInfoParser::Instance()->GetMasterServerIP(), MAX_IPADDRESS_SIZE );
		wMasterServerPort = MasterServerInfoParser::Instance()->GetMasterServerPort();

		if( !m_pIOCPServer->StartListen( INSIDE_IOHANDLER, szMasterServerIP, wMasterServerPort ) )
		{
			MessageOut( eCRITICAL_LOG, "*** Listen failed. ***" );
			Shutdown();
		}
		else
		{
			MessageOut( eCRITICAL_LOG, "Listen started...(%s:%d)", szMasterServerIP, wMasterServerPort );
		}
	}

	// Account DB pxory 세션은 DB 정보를 받은 지금 시점에 서버 세션 매니져에 추가
	SERVER_KEY serverKey = ServerInfoManager::Instance()->GetServerKey( ACCOUNT_DBPROXY, (char*)m_pAccountDBProxySession->GetConnectIP().c_str() );
	if( !ServerSessionManager::Instance()->FindServer( serverKey ) )
	{
		// 서버 정보를 업데이트 하는 경우 또 추가하지 않게끔 서버키로 찾아본 후에 추가
		m_pAccountDBProxySession->SetServerKey( serverKey );
		ServerSessionManager::Instance()->AddServer( m_pAccountDBProxySession );
	}

	// Op Server 세션은 DB 정보를 받은 지금 시점에 서버 세션 매니져에 추가
	SERVER_KEY opServerKey = ServerInfoManager::Instance()->GetServerKey( OP_SERVER, (char*)m_pOpServerSession->GetConnectIP().c_str() );
	if( !ServerSessionManager::Instance()->FindServer( opServerKey ) )
	{
		// Op Server가 연결이 되어있는 경우 추가!
		if( m_pOpServerSession->IsConnected() )
		{
			m_pOpServerSession->SetServerKey( opServerKey );
			ServerSessionManager::Instance()->AddServer( m_pOpServerSession );
		}
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// Solar Auth Server 세션은 DB 정보를 받은 지금 시점에 서버 세션 매니져에 추가
	SERVER_KEY solarAuthServerKey = ServerInfoManager::Instance()->GetServerKey( SOLARAUTH_SERVER, (char*)m_pSolarAuthServerSession->GetConnectIP().c_str() );
	if( !ServerSessionManager::Instance()->FindServer( solarAuthServerKey ) )
	{
		// 서버 정보를 업데이트 하는 경우 또 추가하지 않게끔 서버키로 찾아본 후에 추가
		m_pSolarAuthServerSession->SetServerKey( solarAuthServerKey );
		ServerSessionManager::Instance()->AddServer( m_pSolarAuthServerSession );
	}
#endif
}

VOID MasterServer::Run()
{
	// Account DB Proxy에 접속 시도
//	ConnectToServer( m_pAccountDBProxySession, (char*)m_pAccountDBProxySession->GetConnectIP().c_str(), m_pAccountDBProxySession->GetConnectPort() );

	// Op Server에 접속시도
//	ConnectToServer( m_pOpServerSession, (char*)m_pOpServerSession->GetConnectIP().c_str(), m_pOpServerSession->GetConnectPort() );

	// 메인 루프
	while( !m_bShutdown )
	{
		Sleep( 10 );

		if( !Update() ) break;
	}
}

BOOL MasterServer::Update()
{
	// I/O 엔진 업데이트
	m_pIOCPServer->Update();

	UpdateFPS();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	WorkLoadManager::Update();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	ServerSessionManager::Instance()->Update();

	g_pSunLog->Update();

	// 콘솔처리
	if( ProcessConsole() == FALSE )
	{
		return FALSE;	// 서버 종료
	}

	if( m_ReconnectTimer.IsExpired() )
	{
		MaintainConnection();
	}

	// 돈 처리
	if( m_MoneyTimer.IsExpired() )
	{
		SendMoneyReq();
	}

	return TRUE;
}

VOID MasterServer::MaintainConnection()
{
	// 마스터 서버가 종료중이면 리턴
	if( m_bShutdown )						return;

	// Account DB proxy와 연결이 끊어져 있으면 다시 접속을 시도한다.
	if( !m_pAccountDBProxySession->IsConnected() && !m_pAccountDBProxySession->GetConnectIP().empty() )
	{
		ConnectToServer( m_pAccountDBProxySession, (char*)m_pAccountDBProxySession->GetConnectIP().c_str(), m_pAccountDBProxySession->GetConnectPort() );
	}
/*
	// 혹시 무슨 이유로 OpServer가 NULL이면
	if( ServerSessionManager::Instance()->GetOpServer() == NULL )
	{
		// m_pOpServerSession과 맞춰준다
		ServerSessionManager::Instance()->AddServer( m_pOpServerSession );
	}
*/
	// Op Server와 연결이 끊어져 있으면 재접속	
	if( !m_pOpServerSession->IsConnected() && !m_pOpServerSession->GetConnectIP().empty() )
	{
		ConnectToServer( m_pOpServerSession, (char*)m_pOpServerSession->GetConnectIP().c_str(), m_pOpServerSession->GetConnectPort() );
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	if( !m_pSolarAuthServerSession->IsConnected() && !m_pSolarAuthServerSession->GetConnectIP().empty() )
	{
		ConnectToServerOutside( m_pSolarAuthServerSession, (char*)m_pSolarAuthServerSession->GetConnectIP().c_str(), m_pSolarAuthServerSession->GetConnectPort() );
	}
#endif

	// 정상종료 명령을 WOPS에서 받았는지 check하고 필요한 종료 과정 수행
	CheckExitServer();
//	if( CheckExitServer() )
//	{
//		Shutdown();
//	}
}

#include <PacketStruct_MX.h>

// 기능 제한 정보 DB에서 읽어오도록 요청
BOOL MasterServer::SendRestrictReq()
{
	if( m_pAccountDBProxySession && m_pAccountDBProxySession->IsConnected() )
	{
		MasterServerInfoParser::Instance()->ClearFuncRestrictInfo();

		MSG_MX_FUNCRESTRICT_RELOAD_REQ		sendMsg;

		m_pAccountDBProxySession->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );

		return TRUE;
	}

	return FALSE;
}

VOID MasterServer::SendRestrictAns()
{
	// 금지 기능들 정보 날려준다
	MSG_GM_OPERATION_FUNCRESTRICT_CMD	funcMsg;

	MasterServerInfoParser::Instance()->GetFuncRestrictInfo( (BYTE*)&funcMsg );

	// 내용 있어야 보내준다
	if( funcMsg.byRestrictNum )
	{
		ServerSessionManager::Instance()->SendGameServer( (BYTE*)&funcMsg, funcMsg.GetSize() );
	}
}

BOOL MasterServer::ProcessConsole()
{
	if( kbhit() )
	{
		char ch = getch();
		ch = toupper(ch);

		switch(ch)
		{
		case 0x1b:
			return FALSE;

		case ' ':
			DisplayServerInfo();
			break;

		case 'U':
			ServerInfoManager::Instance()->Clear();
			m_pAccountDBProxySession->SendReqServerInfo();
			break;
		case 'S':
			// 인터벌 정보 리로드
			MasterServerInfoParser::Instance()->ReloadInterval();
			m_ExitTimer.Reset();
			m_ExitTimer.SetTimer( MasterServerInfoParser::Instance()->GetExitInterval() );	// 종료 응답 대기 시간
			m_ExitTimer.DisableCheckTime();

			// 통화량 주기 초기화
			m_MoneyTimer.Reset();
			m_MoneyTimer.SetTimer( MasterServerInfoParser::Instance()->GetStatisticsInterval() );
			MessageOut( eCRITICAL_LOG, "Interval Info Reload[Exit:%u][Statistics:%u]", m_ExitTimer.GetIntervalTime(), 
																						m_MoneyTimer.GetIntervalTime() );
			break;
		case 'R':
			if( MasterServerInfoParser::Instance()->IsRestrictInfoFromFile() )
			{
				MasterServerInfoParser::Instance()->LoadFuncRestrictInfo();

				// 금지 기능들 정보 있으면 날려준다
				MSG_GM_OPERATION_FUNCRESTRICT_CMD	funcMsg;

				MasterServerInfoParser::Instance()->GetFuncRestrictInfo( (BYTE*)&funcMsg );

				// 내용 있어야 보내준다
				if( funcMsg.byRestrictNum )
				{
					ServerSessionManager::Instance()->SendGameServer( (BYTE*)&funcMsg, funcMsg.GetSize() );
				}
				MessageOut( eCRITICAL_LOG, "Reload FuncRestrict Setting AND Broadcast" );
			}
			else	// DB에서 읽어오는 경우 DB에 금지 기능들 정보 요청
			{
				SendRestrictReq();
			}

#ifdef _TEST_ABUSE
			// 테스트용 코드 - 가라로 어뷰징 보낸다
			MSG_MO_RTTG_ABUSING_NTF	sendMsg;
			SERVER_KEY				serverKey;

			serverKey.Set( 1, 2, 3, 4 );
			GetDateTime_YYMMDDHHMMSS( sendMsg.m_dw64Time );
			sendMsg.m_dwUserGuid	= 3724;
			sendMsg.m_dwCharGuid	= 12938;
			sendMsg.m_ServerKey		= serverKey;
			sendMsg.m_byAbuseType	= 1;

			wsprintf( sendMsg.m_ptszDesc, "가라 테스트 데이터~[3724][12938][1,2,3,4][3]" ); 
			sendMsg.m_byLength		= _tcslen(sendMsg.m_ptszDesc);

			ServerSession *pOpServer = GetOpServer();
			if( !pOpServer )
			{
				MessageOut( eCRITICAL_LOG, "RTTA Abusing 보고 실패:No RTTA[Type:%u][UserGuid:%u][CharGuid:%u][Desc:%u]", sendMsg.m_byAbuseType, 
																														sendMsg.m_dwUserGuid,
																														sendMsg.m_dwCharGuid,
																														sendMsg.m_ptszDesc );
				return FALSE;
			}

			pOpServer->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );

			MessageOut( eCRITICAL_LOG, "RTTA Abusing Alarm:No RTTA[Type:%u][UserGuid:%u][CharGuid:%u]", sendMsg.m_byAbuseType, 
																												sendMsg.m_dwUserGuid,
																												sendMsg.m_dwCharGuid );

			// 가라로 서버 정보도 리로드
//			SendRestrictReq();
#endif




			// RTTA 허용 상태도 읽어서 리턴 - 이 코드는 일단 봉인
//			MasterServerInfoParser::Instance()->LoadRTTARestrictInfo();
//			MessageOut( eCRITICAL_LOG, "Reload RTTARestrict Setting" );
			break;
/*
		case 'S':
			//테스트 - 가라로 서버 종료 요청

			SERVER_KEY exitKey;
			exitKey.Set( 0, 255, 0, 0 );
			ServerSessionManager::Instance()->AddExitServerList( exitKey );

			SetExitServer();	// 종료 모드 돌입!
			SetExitKey( 0, 0 );
			SetExitType( eEXIT_AGENTSTART );

			//서버 종료는 순차적으로(필드/배틀/에이전트->DB프락시) 이루어져야 하므로, 서버 종료 모드로 들어가서 처리해야 한다
			MessageOut( eFULL_LOG, "정상 종료 가라로 수신[W:%u/C:%u/T:%u/I:%u]", exitKey.GetWorldID(), 
																					exitKey.GetChannelID(),
																					exitKey.GetServerType(), 
																					exitKey.GetServerID() );
			break;
		case 'D':
			// 테스트-접속종료 요청
			// 현재 접속중인 모든 Agent 서버에게 이 프로토콜을 전달한다
			MSG_AM_OPERATION_CHAR_DISCONNECT_SYN	sendMsg;

			sendMsg.m_dwUserGuid = 4342;

			SERVER_SESSION_VEC agentList = ServerSessionManager::Instance()->GetAgentServerList();

			if( agentList.empty() )
			{
				MessageOut( eCRITICAL_LOG, "MO_RTTG_USERDISCONNECT Error - No Agent Server 4342" );
				return TRUE;
			}

			SERVER_SESSION_VEC::iterator iter;
			ServerSession *pAgentServer=NULL;

			for( iter = agentList.begin(); iter != agentList.end(); iter++ )
			{
				pAgentServer = (*iter);
				pAgentServer->Send( (BYTE*)&sendMsg, sizeof(MSG_AM_OPERATION_CHAR_DISCONNECT_SYN) );
			}

			MessageOut( eFULL_LOG, "USERDISCONNECT Send 4342" );
			break;
*/
		}
	}

	return TRUE;
}

VOID MasterServer::ConnectToServer( NetworkObject *pNetworkObject, char *pszIP, WORD wPort )
{
	m_pIOCPServer->Connect( INSIDE_IOHANDLER, pNetworkObject, pszIP, wPort );
}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

VOID MasterServer::ConnectToServerOutside( NetworkObject *pNetworkObject, char *pszIP, WORD wPort )
{
	m_pIOCPServer->Connect( OUTSIDE_IOHANDLER, pNetworkObject, pszIP, wPort );
}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

VOID MasterServer::UpdateFPS()
{
	// FPS Check
	static int cnt = 0;
	static DWORD prevTick = GetTickCount();

	cnt++;

	DWORD curTick = GetTickCount();

	if( prevTick + 1000 < curTick )
	{
		m_dwFPS = cnt;
		//printf( "%d FPS\n", cnt );
		prevTick	= curTick;
		cnt			= 0;

		if( m_dwFPS < 10 )
		{
			DISPMSG( "============================================================\n" );
			DISPMSG( "============================================================\n" );
			DISPMSG( "[MasterServer::UpdateFPS] FPS Warning!!! [%d] \n", m_dwFPS );
			DISPMSG( "============================================================\n" );
			DISPMSG( "============================================================\n" );
		}
	}
}

VOID MasterServer::DisplayServerInfo()
{
	printf( "======== Master Server ========\n" );
	printf( " Frame per Second: %d\n", m_dwFPS );
	printf( " Total Connection: %d\n", m_pIOCPServer->GetNumberOfConnections( INSIDE_IOHANDLER ) );
	if( !m_pAccountDBProxySession->IsConnected() )
	{
		printf( ">> Warning: Not connected to account DB proxy server.\n" );
	}
	if( !m_pOpServerSession->IsConnected() )
	{
		printf( ">> Warning: Not connected to operating server(RTTA).\n" );
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	if( !m_pSolarAuthServerSession->IsConnected() )
	{
		printf( ">> Warning: Not connected to Solar Auth Server.\n" );
	}
#endif

	// 맵 훑어서 서버 정보 주루룩 읊어준다
	WORLD_MAP_ITER		iterWorld;
	CHANNEL_MAP			mapChannel;
	CHANNEL_MAP_ITER	iterChannel;
	SERVERKEY_VEC		vecServer;
	SERVERKEY_VEC_ITER	iterServer;
	SERVER_KEY			serverKey;

	DISPMSG( "===== World Info =====\n" );
	for( iterWorld = m_WorldMap.begin(); iterWorld != m_WorldMap.end(); ++iterWorld )
	{
		DISPMSG( "======= World %u =======\n", (*iterWorld).first );
		mapChannel = (*iterWorld).second;

		for( iterChannel = mapChannel.begin(); iterChannel != mapChannel.end(); ++iterChannel )
		{
#ifdef _DEBUG
			DISPMSG( "====== Channel %u ======\n", (*iterChannel).first );
			vecServer = (*iterChannel).second;

			for( iterServer = vecServer.begin(); iterServer != vecServer.end(); ++iterServer )
			{
				serverKey = (*iterServer);

				DISPMSG( "%s\n", GetServerType2String( static_cast<eSERVER_TYPE>(serverKey.GetServerType()) ) );
			}
#else
			vecServer = (*iterChannel).second;
			DISPMSG( "=== Channel %u: %u ===\n", (*iterChannel).first, vecServer.size() );
#endif
//			printf( "========================\n" );
		}
	}
	DISPMSG( "========================\n" );
}

BOOL MasterServer::IsConnectedTo( eSERVER_TYPE eServerType )
{
	switch( eServerType )
	{
	case ACCOUNT_DBPROXY:
		return m_pAccountDBProxySession->IsConnected();
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	case SOLARAUTH_SERVER:
		return m_pSolarAuthServerSession->IsConnected();
#endif
	}

	assert( !"그런 서버 타입은 없어요" );

	return FALSE;	
}

/*
VOID MasterServer::SetMoney( SERVER_KEY serverkey, ULONGLONG &ui64GetMoney, ULONGLONG &ui64LoseMoney, ULONGLONG &ui64UserMoney )
{
	SERVER_MONEY_INFO *pServer = m_pServerMoneyInfoTable->GetData( serverkey );
	if(pServer == NULL)
	{
		// 없으면 만들어
		pServer = new SERVER_MONEY_INFO;
		pServer->byWorldID = serverkey.GetWorldID();
		pServer->byChannelID = serverkey.GetChannelID();
		pServer->byServerType = serverkey.GetServerType();
		pServer->byServerID = serverkey.GetServerID();
		pServer->ui64TotalGetMoney = ui64GetMoney;
		pServer->ui64TotalLoseMoney = ui64LoseMoney;
		pServer->ui64TotalUserMoney = ui64UserMoney;

		m_pServerMoneyInfoTable->Add( pServer, serverkey );
	}
	else
	{
		// 있으면 더해
		pServer->ui64TotalGetMoney += ui64GetMoney;
		pServer->ui64TotalLoseMoney += ui64LoseMoney;
		pServer->ui64TotalUserMoney = ui64UserMoney;	// 얘는 더하면 안된다
	}
}

BOOL MasterServer::GetMoney( SERVER_KEY serverkey, ULONGLONG &ui64GetMoney, ULONGLONG &ui64LoseMoney, ULONGLONG &ui64UserMoney )
{
	SERVER_MONEY_INFO *pServer = m_pServerMoneyInfoTable->GetData( serverkey );
	if( pServer == NULL )
	{
		return FALSE;
	}

	ui64GetMoney = pServer->ui64TotalGetMoney;
	ui64LoseMoney = pServer->ui64TotalLoseMoney;
	ui64UserMoney = pServer->ui64TotalUserMoney;

	return TRUE;
}

VOID MasterServer::RemoveMoney( SERVER_KEY serverkey )
{
	SERVER_MONEY_INFO *pServer = m_pServerMoneyInfoTable->GetData( serverkey );
	if( pServer == NULL )
	{
		return;
	}

	m_pServerMoneyInfoTable->Remove( serverkey );
}

DWORD MasterServer::GetMoneyInfoNum()
{
	return m_pServerMoneyInfoTable->GetDataNum();
}

VOID MasterServer::SetMoneyInfoFirst()
{
	m_pServerMoneyInfoTable->SetFirst();
}

SERVER_MONEY_INFO* MasterServer::GetMoneyInfoNext()
{
	return m_pServerMoneyInfoTable->GetNext();
}
*/

#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
BOOL MasterServer::AddExitAgentServer( SERVER_KEY sServerKey )
{
	SERVER_KEY *pServerKey = new SERVER_KEY;

	pServerKey->dwKey = sServerKey.dwKey;

	return m_pExitAgentTable->Add( pServerKey, pServerKey->dwKey );
}

VOID MasterServer::RemoveExitAgentServer( SERVER_KEY serverKey )
{
	SERVER_KEY *pServerKey = m_pExitAgentTable->GetData( serverKey.dwKey );

	if( pServerKey )
	{
		m_pExitAgentTable->Remove( serverKey.dwKey );
		delete pServerKey;
		MessageOut( eFULL_LOG, "RemoveExitAgentServer - delete [%u/%u]", serverKey.GetWorldID(), serverKey.GetChannelID() );
		return;
	}

	MessageOut( eFULL_LOG, "RemoveExitAgentServer Error - No Exit ServerKey[%u/%u]", serverKey.GetWorldID(), serverKey.GetChannelID() );
}

#endif

// 종료시킬 서버 추가
BOOL MasterServer::AddExitServer( SERVER_KEY sServerKey )
{
	SERVER_KEY *pServerKey = new SERVER_KEY;

	pServerKey->m_dwKey = sServerKey.m_dwKey;

	return m_pExitServerTable->Add( pServerKey, pServerKey->m_dwKey );
}

// 종료시킬 서버 삭제
VOID MasterServer::RemoveExitServer( SERVER_KEY serverKey )
{
	SERVER_KEY *pServerKey = m_pExitServerTable->GetData( serverKey.m_dwKey );

	if( pServerKey )
	{
		m_pExitServerTable->Remove( serverKey.m_dwKey );
		delete pServerKey;
		MessageOut( eFULL_LOG, "RemoveExitServer - delete [%u/%u/%u/%u]", serverKey.GetWorldID(), 
																		serverKey.GetChannelID(), 
																		serverKey.GetServerType(), 
																		serverKey.GetServerID() );
		return;
	}

	MessageOut( eFULL_LOG, "RemoveExitServer Error - No Exit ServerKey[%u/%u/%u/%u]", serverKey.GetWorldID(), 
																						serverKey.GetChannelID(), 
																						serverKey.GetServerType(), 
																						serverKey.GetServerID() );
}

// 종료시킬 서버 추가
BOOL MasterServer::AddExitDBServer( SERVER_KEY sServerKey )
{
	SERVER_KEY *pServerKey = new SERVER_KEY;

	pServerKey->m_dwKey = sServerKey.m_dwKey;

	return m_pExitDBServerTable->Add( pServerKey, pServerKey->m_dwKey );
}

// 종료시킬 서버 삭제
VOID MasterServer::RemoveExitDBServer( SERVER_KEY serverKey )
{
	SERVER_KEY *pServerKey = m_pExitDBServerTable->GetData( serverKey.m_dwKey );

	if( pServerKey )
	{
		m_pExitDBServerTable->Remove( serverKey.m_dwKey );
		delete pServerKey;
		MessageOut( eFULL_LOG, "RemoveExitDBServer - delete [%u/%u/%u/%u]", serverKey.GetWorldID(), 
																		serverKey.GetChannelID(), 
																		serverKey.GetServerType(), 
																		serverKey.GetServerID() );
//		SetExitType( FALSE );	// 다 보냈다
		return;
	}
//	SetExitType( FALSE );	// 다 보냈다
	MessageOut( eFULL_LOG, "RemoveExitDBServer [%u/%u/%u/%u]", serverKey.GetWorldID(), 
															serverKey.GetChannelID(), 
															serverKey.GetServerType(), 
															serverKey.GetServerID() );
}

BOOL MasterServer::CheckExitServer()
{
/*
	// 테스트 코드 - 2초에 한번씩 가짜 월드서버 동접을 보낸다
	static DWORD dwLastTime = GetTickCount();
	DWORD dwThisTime = GetTickCount();

	if( dwThisTime >= dwLastTime + 8000 )
	{
		MSG_MO_RTTG_USERCOUNT_NTF	sendMsg;
		DWORD						dwTotalVillageCount = 0, dwTotalVillagePCRoomCount = 0;
		DWORD						dwTotalBattleZoneCount = 0, dwTotalBattleZonePCRoomCount = 0;

		MSG_MO_RTTG_BATTLEZONEINFO_ANS	battleMsg;
		TCHAR szMsg[512];

		ServerSession *pOpServer = GetOpServer();
		if( !pOpServer )
		{
			MessageOut( eCRITICAL_LOG, "RTTA 동접수 보고 실패:No Data" );	
			return FALSE;
		}

		for( BYTE byWorld = 1; byWorld < 6; byWorld++ )
		{
			if( rand() % 2 )
			{
				continue;
			}

			// 배틀존 상태
			battleMsg.byCount = 3;

			// 동접
			sendMsg.dwWorldID = byWorld;

			dwTotalVillageCount = 0;
			dwTotalVillagePCRoomCount = 0;
			dwTotalBattleZoneCount = 0;
			dwTotalBattleZonePCRoomCount = 0;

			for(BYTE i=1; i<=3; i++)
			{
				sendMsg.dwChannelID = i;
				sendMsg.dwVillageCount = i * 300;
				sendMsg.dwVillagePCRoomCount = i * 200;
				sendMsg.dwBattleZoneCount = i * 200;
				sendMsg.dwBattleZonePCRoomCount = i * 300;

				pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_USERCOUNT_NTF) );
				MessageOut( eCRITICAL_LOG, "RTTA로 동접수 보고: W(%u)C(%u)V(%u/%u)B(%u/%u)", sendMsg.dwWorldID, 
																					sendMsg.dwChannelID, 
																					sendMsg.dwVillageCount, 
																					sendMsg.dwVillagePCRoomCount,
																					sendMsg.dwBattleZoneCount,
																					sendMsg.dwBattleZonePCRoomCount );
				dwTotalVillageCount += sendMsg.dwVillageCount;
				dwTotalVillagePCRoomCount += sendMsg.dwVillagePCRoomCount;
				dwTotalBattleZoneCount += sendMsg.dwBattleZoneCount;
				dwTotalBattleZonePCRoomCount += sendMsg.dwBattleZonePCRoomCount;

				// 배틀존
				battleMsg.ChannelInfo[i].byWorldID = byWorld;
				battleMsg.ChannelInfo[i-1].byChannelID = i;
				battleMsg.ChannelInfo[i-1].byWorldID = byWorld;
				memset( &battleMsg.ChannelInfo[i].dwZoneCount, 0, sizeof(DWORD) * eZONETYPE_MAX );
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_VILLAGE] = sendMsg.dwVillageCount;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_FIELD] = sendMsg.dwVillagePCRoomCount;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_HUNTING] = sendMsg.dwBattleZonePCRoomCount;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_MISSION] = sendMsg.dwBattleZoneCount - 100;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_PVP] = 100;
				_snprintf(szMsg, sizeof(szMsg), "배틀존 상태 : W(%d)C(%d)V(%u)H(%u)M(%u)PVP(%u)", battleMsg.ChannelInfo[i-1].byWorldID, 
																							battleMsg.ChannelInfo[i-1].byChannelID, 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_VILLAGE], 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_HUNTING], 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_MISSION], 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_PVP] );

				SUNLOG->InsertMessage( szMsg, eMIDDLE_LOG );
			}
			MessageOut( eCRITICAL_LOG, "월드 동접 확인: W(%u)V(%u/%u)B(%u/%u)", sendMsg.dwWorldID, 
																	dwTotalVillageCount, 
																	dwTotalVillagePCRoomCount,
																	dwTotalBattleZoneCount,
																	dwTotalBattleZonePCRoomCount );

			pOpServer->Send( (BYTE*)&battleMsg, battleMsg.GetSize() );

			dwLastTime = dwThisTime;
		}
	}
	// 테스트 코드 끝
*/

	// 정상 종료 프로세스중인지 확인한다
	if( !IsExitServer() )
	{
		return FALSE;
	}

//	static DWORD dwLastTick = GetTickCount();
//	DWORD dwTickCount = GetTickCount();
	SERVERKEY_HASH_ITR	iter;
	ServerSession *pServerSession = NULL;
	SERVER_KEY ExitServerKey;
	MSG_MO_RTTG_SERVERSHUTDOWN_ANS sendMsg;

	switch( m_eExitType )
	{
		case eEXIT_NONE:
		{
			m_bExitFailed = FALSE;
			break;
		}
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
		case eEXIT_AGENTSTART:	// 에이전트 종료 패킷 보낸다
		{
			MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ sendMsg;

			sendMsg.m_byCategory		= 0;
			sendMsg.m_byProtocol		= SERVERCOMMON_SERVERSHUTDOWN_REQ;

			for( iter = m_pExitAgentTable->begin(); iter != m_pExitAgentTable->end(); ++iter )
			{
				ExitServerKey = *(*iter);
				pServerSession = ServerSessionManager::Instance()->FindServer( ExitServerKey );
				if( pServerSession )
				{
					pServerSession->Send( (BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ) );
				}
				else
				{
					m_pExitAgentTable->erase( iter );
					break;
				}
				m_eExitType = eEXIT_AGENTWAIT;
				m_ExitTimer.Reset();
				m_ExitTimer.EnableCheckTime();
				MessageOut( eFULL_LOG, "Send AgentExit Message" );
			}
			break;
		}
		case eEXIT_AGENTWAIT:	// 에이전트 종료 패킷 보내고 대기
		{
			if( m_pExitAgentTable->GetDataNum() )
			{
				m_eExitType = eEXIT_GAMESTART;
				MessageOut( eFULL_LOG, "Send AgentExit Complete" );
				break;
			}

			// 일정 시간 이상 응답 없으면 전부 오류 처리
			if( m_ExitTimer.IsExpired() )
			{
				ServerSession*	pOpServer = GetOpServer();
				SERVER_KEY*		pExitServerKey = NULL;

				if( !pOpServer )
				{
					MessageOut( eCRITICAL_LOG, "RTTA ServerShutDown TimeOut Error(Agent) - No RTTA Server" );
					return FALSE;
				}

				while( m_pExitAgentTable->GetDataNum() )
				{
					pExitServerKey = m_pExitAgentTable->GetHeadData();

					if( pExitServerKey )
					{
						// 오류 전송
						sendMsg.dwError = 1;
						sendMsg.m_ServerKey = *pExitServerKey;
						sendMsg.dwKey = GetExitKey();
						sendMsg.i64WopsKey = GetExit64Key();

						if( !m_bExitFailed )
						{
							pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );
							m_bExitFailed = TRUE;
						}

						// 이미 처리한 서버키는 삭제
						MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN DB TimeOut(Agent)[%u/%u]", pExitServerKey->GetWorldID(), 
																								pExitServerKey->GetChannelID() );
						delete pExitServerKey;
					}
					else
					{
						break;
					}
				}
				MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN TimeOut Error Return(Agent)" );
				m_ExitTimer.DisableCheckTime();
				m_pExitAgentTable->clear();
			}
			break;
		}
#endif
		case eEXIT_GAMESTART:	// 게임 종료 패킷 보낸다
		{
			MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ sendMsg;

			sendMsg.m_byCategory		= 0;
			sendMsg.m_byProtocol		= SERVERCOMMON_SERVERSHUTDOWN_REQ;

			for( iter = m_pExitServerTable->begin(); iter != m_pExitServerTable->end(); ++iter )
			{
				ExitServerKey = *(*iter);
				pServerSession = ServerSessionManager::Instance()->FindServer( ExitServerKey );
				if( pServerSession )
				{
					pServerSession->Send( (BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ) );
				}
				else
				{
					m_pExitServerTable->erase( iter );
					break;
				}
			}

			m_eExitType = eEXIT_GAMEWAIT;
			m_ExitTimer.Reset();
			m_ExitTimer.EnableCheckTime();
			MessageOut( eFULL_LOG, "Send ServerExit Message" );
			break;
		}
		case eEXIT_GAMEWAIT:	// 게임 종료 패킷 보내고 대기
		{
			if( !m_pExitServerTable->GetDataNum() )
			{
				m_eExitType = eEXIT_DBPROXY;
				MessageOut( eFULL_LOG, "Send ServerExit Complete" );
				break;
			}

			// 일정 시간 이상 응답 없으면 전부 오류 처리
			if( m_ExitTimer.IsExpired() )
			{
				ServerSession*	pOpServer = GetOpServer();
				SERVER_KEY*		pExitServerKey = NULL;

				if( !pOpServer )
				{
					MessageOut( eCRITICAL_LOG, "RTTA ServerShutDown TimeOut Error - No RTTA Server");
					return FALSE;
				}

				while( m_pExitServerTable->GetDataNum() )
				{
					pExitServerKey = m_pExitServerTable->GetHeadData();;

					if( pExitServerKey )
					{
						// 오류 전송
						sendMsg.dwError = 1;
						sendMsg.m_ServerKey = *pExitServerKey;
						sendMsg.dwKey = GetExitKey();
						sendMsg.i64WopsKey = GetExit64Key();

						if( !m_bExitFailed )
						{
							pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );

							m_bExitFailed = TRUE;
						}

						// 이미 처리한 서버키는 삭제
						MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN DB TimeOut[%u/%u/%u/%u]", pExitServerKey->GetWorldID(), 
																							pExitServerKey->GetChannelID(),
																							pExitServerKey->GetServerType(),
																							pExitServerKey->GetServerID() );
						m_pExitServerTable->Remove( pExitServerKey->m_dwKey );
						delete pExitServerKey;
					}
					else
					{
						break;
					}
				}
				MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN TimeOut Error Return" );
				m_ExitTimer.DisableCheckTime();
				m_pExitServerTable->clear();
			}
			break;
		}
		case eEXIT_DBPROXY:		// DB 종료 패킷 보낸다
		{
			MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ sendMsg;

			sendMsg.m_byCategory		= 0;
			sendMsg.m_byProtocol		= SERVERCOMMON_SERVERSHUTDOWN_REQ;

			for( iter = m_pExitDBServerTable->begin(); iter != m_pExitDBServerTable->end(); ++iter )
			{
				ExitServerKey = *(*iter);
				pServerSession = ServerSessionManager::Instance()->FindServer( ExitServerKey );
				if( pServerSession )
				{
					pServerSession->Send( (BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ) );
				}
				else
				{
					m_pExitDBServerTable->erase( iter );
				}
			}

			m_eExitType = eEXIT_DBPROXYWAIT;
			MessageOut( eFULL_LOG, "Send DBServerExit Message" );
			m_ExitTimer.Reset();
			m_ExitTimer.EnableCheckTime();
			break;
		}
		case eEXIT_DBPROXYWAIT:	// DB 종료 패킷 보내고 대기
		{
			if( !m_pExitDBServerTable->GetDataNum() )
			{
				m_eExitType = eEXIT_END;
				MessageOut( eFULL_LOG, "Send DBServerExit Complete" );
				break;
			}

			// 일정 시간 이상 응답 없으면 전부 오류 처리
			if( m_ExitTimer.IsExpired() )
			{
				ServerSession*	pOpServer = GetOpServer();
				SERVER_KEY*		pExitServerKey = NULL;

				if( !pOpServer )
				{
					MessageOut( eCRITICAL_LOG, "RTTA ServerShutDown TimeOut Error - No RTTA Server");
					return FALSE;
				}

				while( m_pExitDBServerTable->GetDataNum() )
				{
					pExitServerKey = m_pExitDBServerTable->GetHeadData();

					if( pExitServerKey )
					{
						// 오류 전송
						sendMsg.dwError = 1;
						sendMsg.m_ServerKey = *pExitServerKey;
						sendMsg.dwKey = GetExitKey();
						sendMsg.i64WopsKey = GetExit64Key();

						if( !m_bExitFailed )
						{
							pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );

							m_bExitFailed = TRUE;
						}

						// 이미 처리한 서버키는 삭제
						MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN DB TimeOut[%u/%u/%u/%u]", pExitServerKey->GetWorldID(), 
																							pExitServerKey->GetChannelID(),
																							pExitServerKey->GetServerType(),
																							pExitServerKey->GetServerID() );
						m_pExitDBServerTable->Remove( pExitServerKey->m_dwKey );
						delete pExitServerKey;
					}
					else
					{
						break;
					}
				}

				MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN DB TimeOut Error Return" );
				m_ExitTimer.DisableCheckTime();
				m_pExitDBServerTable->clear();
			}
			break;
		}
		case eEXIT_END:
		{
			// 종료되었다는 응답을 RTTA에 보낸다
			sendMsg.dwError = 0;
			sendMsg.dwKey = GetExitKey();
			sendMsg.i64WopsKey = GetExit64Key();

			ServerSession* pOpServer = GetOpServer();

			if( !pOpServer )
			{
				MessageOut( eCRITICAL_LOG, "RTTA ServerShutDown_ACK Error - No RTTA Server");
				return FALSE;
			}

			if( !m_bExitFailed )
			{
				pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );

				m_bExitFailed = TRUE;
			}

			m_eExitType = eEXIT_NONE;
			m_bExitServer = FALSE;
			m_ExitServerKey.Set( 0, 0, 0, 0 );
			m_ExitTimer.DisableCheckTime();
			MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN_ACK Return" );
			break;
		}
		default:				// 에러
		{
			break;
		}
	}
/*
		if( m_ExitTimer.IsExpired() )
		{
			// 10초 이상 응답 없으면 종료 패킷 오류
			MessageOut( eCRITICAL_LOG, "ExitServer Error - [%u/%u/%u/%u] No Response", m_ExitServerKey.GetWorldID(), 
																					m_ExitServerKey.GetChannelID(), 
																					m_ExitServerKey.GetServerType(), 
																					m_ExitServerKey.GetServerID() );

			// 오류를 알려주고 다음 서버로 넘어간다
			SERVER_KEY *pExitServerKey = m_pExitServerTable->GetHeadData();

			if( pExitServerKey )
			{
				// 오류 전송
				MSG_MO_RTTG_SERVERSHUTDOWN_ANS sendMsg;

				sendMsg.dwError = 1;
				sendMsg.m_ServerKey = *pExitServerKey;
				sendMsg.dwKey = GetExitKey();

				ServerSession* pOpServer = ServerSessionManager::Instance()->GetOpServer();

				if( !pOpServer )
				{
					MessageOut( eCRITICAL_LOG, "RTTA ServerShutDown TimeOut Error - No RTTA Server");
					return FALSE;
				}

				pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );

				// 이미 처리한 서버키는 삭제
				m_pExitServerTable->Remove( pExitServerKey->dwKey );
				delete pExitServerKey;

				MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN TimeOut Error Return" );
			}
			SetExitType( FALSE );
			dwLastTick = GetTickCount();
		}
	}
	else
	{
		// 종료할 서버가 테이블에 들어 있는지 확인
		if( m_pExitServerTable->GetDataNum() > 0 )
		{
			// 앞에 하나 지운다
			util::SolarHashTable<SERVER_KEY *>::iterator iter = m_pExitServerTable->begin();
			SERVER_KEY ExitServerKey = *(*iter);

			ServerSession *pSession = ServerSessionManager::Instance()->FindServer( ExitServerKey );

			if( !pSession )
			{
				//이미 종료
				m_pExitServerTable->erase( iter );	// 데이터만 지운다
				if( 0 == m_pExitServerTable->GetDataNum() )
				{
					// 종료할 서버 다 종료 했으면 끝~
					m_bExitServer = FALSE;
				}
			}
			else	// 서버 있으면 종료
			{
				// DB Proxy 서버라면
				if( pSession->GetServerType() == GAME_DBPROXY || ACCOUNT_DBPROXY )
				{
					// 다른 서버 종료하는 시간 동안 대기하기 위해 5초 준다
					if( dwLastTick + 5000 > dwTickCount )
					{
						return FALSE;
					}
					dwLastTick = GetTickCount();
				}
				// 서버 타입에 따라
				MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ sendMsg;

				sendMsg.m_byCategory		= 0;
				sendMsg.m_byProtocol		= SERVERCOMMON_SERVERSHUTDOWN_REQ;

				pSession->Send( (BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ) );
				SetExitType( TRUE );	// 종료 요청 보냈다!
				MessageOut( eFULL_LOG, "Send ServerExit Message [%u/%u/%u/%u]", pSession->GetServerKey().GetWorldID(), 
																				pSession->GetServerKey().GetChannelID(), 
																				pSession->GetServerKey().GetServerType(), 
																				pSession->GetServerKey().GetServerID() );

				m_ExitTimer.Reset();
				dwLastTick = GetTickCount();
			}
		}
		else
		{
			m_bExitServer = FALSE;

			// 다 끝났다~
		}
	}

	if( IsExitServer() && !IsExitType() && m_pExitServerTable->GetDataNum() == 0 )
	{
		// 완전 종료 상황이면!
		SetExitServer( FALSE );
		// 종료되었다는 응답을 RTTA에 보낸다
		MSG_MO_RTTG_SERVERSHUTDOWN_ANS sendMsg;

		sendMsg.dwError = 0;
		sendMsg.dwKey = GetExitKey();

		ServerSession* pOpServer = ServerSessionManager::Instance()->GetOpServer();

		if( !pOpServer )
		{
			MessageOut( eCRITICAL_LOG, "RTTA ServerShutDown_ACK Error - No RTTA Server");
			return FALSE;
		}

		pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );
		MessageOut( eFULL_LOG, "RTTA SERVERSHUTDOWN_ACK Return" );

		// 다 끝났다~
        SetExitServer( FALSE );

		// 서버 전체 종료라면 자기 자신도 종료한다
		if( m_ExitServerKey.GetWorldID() == 0 )
		{
			return TRUE;
		}
	}
*/
	return FALSE;
}

// 살아 있는 서버 목록에 추가
VOID MasterServer::AddServerMap( SERVER_KEY &serverKey )
{
	WORLD_MAP_ITER	iter = m_WorldMap.find( serverKey.GetWorldID() );

	// 해당 월드가 없으면 추가
	if( iter == m_WorldMap.end() )
	{
		CHANNEL_MAP	channelMap;

		channelMap.clear();

		// 채널에 서버 키도 추가
		SERVERKEY_VEC	serverKeyVec;

		serverKeyVec.clear();

		serverKeyVec.push_back( serverKey );

		channelMap.insert( CHANNEL_MAP_PAIR( serverKey.GetChannelID(), serverKeyVec ) );
		m_WorldMap.insert( WORLD_MAP_PAIR( serverKey.GetWorldID(), channelMap ) );
	}
	// 해당 월드의 채널 맵 있으면 거기에 데이터 추가
	else
	{
		CHANNEL_MAP &channelMap = (*iter).second;

		CHANNEL_MAP_ITER iterChannel = channelMap.find( serverKey.GetChannelID() );

		// 해당 채널의 벡터 없으면 추가
		if( iterChannel == channelMap.end() )
		{
			SERVERKEY_VEC	serverKeyVec;

			serverKeyVec.clear();

			serverKeyVec.push_back( serverKey );

			channelMap.insert( CHANNEL_MAP_PAIR( serverKey.GetChannelID(), serverKeyVec ) );
		}
		else
		{
			SERVERKEY_VEC &serverKeyVec = (*iterChannel).second;

			serverKeyVec.push_back( serverKey );
		}
	}
}

VOID MasterServer::RemoveServerMap( SERVER_KEY &serverKey )
{
	WORLD_MAP_ITER iterWorld;

	iterWorld = m_WorldMap.find( serverKey.GetWorldID() );

	if( iterWorld != m_WorldMap.end() )
	{
		CHANNEL_MAP_ITER &iterChannel = (*iterWorld).second.find( serverKey.GetChannelID() );

		if( iterChannel != (*iterWorld).second.end() )
		{
			SERVERKEY_VEC_ITER iterServer;

			SERVERKEY_VEC &vecServer = (*iterChannel).second;

			for( iterServer = vecServer.begin(); iterServer != vecServer.end(); ++iterServer )
			{
				if( (*iterServer) == serverKey )
				{
					vecServer.erase( iterServer );
					MessageOut( eFULL_LOG, "Remove Server[%u/%u/%u/%u]", serverKey.GetWorldID(),
																		serverKey.GetChannelID(),
																		serverKey.GetServerType(),
																		serverKey.GetServerID() );
					break;
				}
			}
			if( vecServer.empty() )
			{
				(*iterWorld).second.erase( iterChannel );
			}
		}
		if( (*iterWorld).second.empty() )
		{
			m_WorldMap.erase( iterWorld );
		}
	}
}

// 게임서버들의 통화량 전달 처리
VOID MasterServer::CheckServerMoney()
{
	WORLD_MAP_ITER		iterWorld;
	CHANNEL_MAP_ITER	iterChannel;
	SERVERKEY_VEC_ITER	iterServer;
	GameServerSession*	pGameServer = NULL;
	ULONGLONG			ui64TempUserMoney;
	MSG_MO_RTTG_USERMONEY_ANS		sendMsg;
	BYTE				byWorldID, byChannelID;

	ServerSession *pOpServer = GetOpServer();
	if( !pOpServer )
	{
		MessageOut( eCRITICAL_LOG, "RTTA 통화량 보고 불가능:No OpServer" );
		return;
	}

	for( iterWorld = m_WorldMap.begin(); iterWorld != m_WorldMap.end(); ++iterWorld )
	{
		byWorldID = (*iterWorld).first;

		if(! byWorldID )
			continue;

		CHANNEL_MAP& mapChannel = (*iterWorld).second;
		for( iterChannel = mapChannel.begin(); iterChannel != mapChannel.end(); ++iterChannel )
		{
			byChannelID = (*iterChannel).first;

			if( !byChannelID ) continue;	// 0번 채널은 보내지 않는다

			SERVERKEY_VEC& vecServer = (*iterChannel).second;

			sendMsg.CirculateMoney = 0;
			sendMsg.ui64TotalUserMoney = 0;
			sendMsg.ui64TotalGetMoney = 0;
			sendMsg.ui64TotalLoseMoney = 0;
			sendMsg.ui64WarehouseAveMoney = 0;
			sendMsg.ui64WarehousePlusMoney = 0;
			sendMsg.ui64WarehouseMinusMoney = 0;
			sendMsg.JackpotMoney = 0;
			sendMsg.usJackpotCnt = 0;

			for( iterServer = vecServer.begin(); iterServer != vecServer.end(); ++iterServer )
			{
				SERVER_KEY& serverKey = (*iterServer);

				switch( serverKey.GetServerType() )
				{
					case GAME_SERVER:
					case FIELD_SERVER:
					case BATTLE_SERVER:
					{
						pGameServer = (GameServerSession*)ServerSessionManager::Instance()->FindServer( serverKey );
						if( !pGameServer )
						{
							vecServer.erase( iterServer );
							continue;
						}

						ui64TempUserMoney = pGameServer->GetAveUserMoney();
						sendMsg.ui64TotalUserMoney = min( sendMsg.ui64TotalUserMoney + ui64TempUserMoney, ULLONG_MAX );

						ui64TempUserMoney = pGameServer->GetAveWarehouseMoney();
						sendMsg.ui64WarehouseAveMoney = min( sendMsg.ui64WarehouseAveMoney + ui64TempUserMoney, ULLONG_MAX );

						// 필요한 통계만 수집
						for( BYTE byCnt = 0; byCnt < eSTATISTIC_MAX; byCnt++ )
						{
							ui64TempUserMoney = pGameServer->GetStatisticsType( byCnt );
							switch( byCnt )
							{
								case eSTATISTIC_MONEY_CIRCULATION:
								{
									sendMsg.CirculateMoney = min( sendMsg.CirculateMoney + ui64TempUserMoney, ULLONG_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
								case eSTATISTIC_MONEY_PLUS:
								{
									sendMsg.ui64TotalGetMoney = min( sendMsg.ui64TotalGetMoney + ui64TempUserMoney, ULLONG_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
								case eSTATISTIC_MONEY_MINUS:
								{
									sendMsg.ui64TotalLoseMoney = min( sendMsg.ui64TotalLoseMoney + ui64TempUserMoney, ULLONG_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
								case eSTATISTIC_MONEY_WAREHOUSE_PLUS:
								{
									sendMsg.ui64WarehousePlusMoney = min( sendMsg.ui64WarehousePlusMoney + ui64TempUserMoney, ULLONG_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
								case eSTATISTIC_MONEY_WAREHOUSE_MINUS:
								{
									sendMsg.ui64WarehouseMinusMoney = min( sendMsg.ui64WarehouseMinusMoney + ui64TempUserMoney, ULLONG_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
								case eSTATISTIC_JACKPOT_COUNT:
								{
									sendMsg.usJackpotCnt = min( sendMsg.usJackpotCnt + ui64TempUserMoney, USHRT_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
								case eSTATISTIC_JACKPOT_MONEY:
								{
									sendMsg.JackpotMoney = min( sendMsg.JackpotMoney + ui64TempUserMoney, ULLONG_MAX );
									pGameServer->ClearStatistics( byCnt );
									break;
								}
							}
						}

						// WOPS에 값 넘겨 줬으니 초기화
						pGameServer->ClearMoney();
						break;
					}
					default:
					{
						continue;
					}
				}
			}

			// 한 채널 통화량 다 구했으면 보고
			sendMsg.byWorldID = byWorldID;
			sendMsg.byChannelID = byChannelID;
			sendMsg.byServerType = 0;
			sendMsg.byServerID = 0;

			pOpServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
#ifdef _DEBUG
			MessageOut( eFULL_LOG, "UserMoney Send[%u/%u][Get:%I64u][Lose:%I64u][User:%I64u]", sendMsg.byWorldID,
																					sendMsg.byChannelID,
																					sendMsg.ui64TotalGetMoney,
																					sendMsg.ui64TotalLoseMoney,
																					sendMsg.ui64TotalUserMoney );

			MessageOut( eFULL_LOG, "WareHouseMoney Send[%u/%u][Get:%I64u][Lose:%I64u][User:%I64u]", sendMsg.byWorldID,
																					sendMsg.byChannelID,
																					sendMsg.ui64WarehousePlusMoney,
																					sendMsg.ui64WarehouseMinusMoney,
																					sendMsg.ui64WarehouseAveMoney );
			MessageOut( eFULL_LOG, "JackpotMoney Send[%u/%u][Cnt:%u][Money:%I64u]", sendMsg.byWorldID,
																					sendMsg.byChannelID,
																					sendMsg.usJackpotCnt, sendMsg.JackpotMoney );
#endif
		}
	}
}

// 기타 통계 집계
VOID MasterServer::CheckStatistics()
{
	WORLD_MAP_ITER		iterWorld;
	CHANNEL_MAP_ITER	iterChannel;
	SERVERKEY_VEC_ITER	iterServer;
	GameServerSession*	pGameServer = NULL;
	MSG_MO_RTTG_GAMESTATISTICS_ANS		sendMsg;
	BYTE				byWorldID, byChannelID;
	ULONGLONG			ui64Temp = 0;

	ServerSession *pOpServer = GetOpServer();
	if( !pOpServer )
	{
		MessageOut( eCRITICAL_LOG, "RTTA 통화량 보고 불가능:No OpServer" );
		return;
	}

	for( iterWorld = m_WorldMap.begin(); iterWorld != m_WorldMap.end(); ++iterWorld )
	{
		byWorldID = (*iterWorld).first;

		if(! byWorldID )
			continue;

		CHANNEL_MAP& mapChannel = (*iterWorld).second;
		for( iterChannel = mapChannel.begin(); iterChannel != mapChannel.end(); ++iterChannel )
		{
			byChannelID = (*iterChannel).first;

			if( !byChannelID ) continue;	// 0번 채널은 보내지 않는다

			memset( sendMsg.ui64Statitics, 0, sizeof(sendMsg.ui64Statitics) );	// 집계할 값 초기화(채널별)

			SERVERKEY_VEC& vecServer = (*iterChannel).second;

			for( iterServer = vecServer.begin(); iterServer != vecServer.end(); ++iterServer )
			{
				SERVER_KEY& serverKey = (*iterServer);

				switch( serverKey.GetServerType() )
				{
					case GAME_SERVER:
					case FIELD_SERVER:
					case BATTLE_SERVER:
					{
						pGameServer = (GameServerSession*)ServerSessionManager::Instance()->FindServer( serverKey );
						if( !pGameServer )
						{
							vecServer.erase( iterServer );
							continue;
						}

						// 모든 통계 수집
						for( BYTE byCnt = 0; byCnt < eSTATISTIC_MAX; byCnt++ )
						{
							switch( byCnt )
							{
								case eSTATISTIC_MONEY_CIRCULATION:
								case eSTATISTIC_MONEY_PLUS:
								case eSTATISTIC_MONEY_MINUS:
								case eSTATISTIC_MONEY_WAREHOUSE_PLUS:
								case eSTATISTIC_MONEY_WAREHOUSE_MINUS:
								case eSTATISTIC_JACKPOT_COUNT:
								case eSTATISTIC_JACKPOT_MONEY:
								{
									// 기존 통화량 정보에 포함되는 값들은 또 넣을 필요 없다
									break;
								}
								default:
								{
									ui64Temp = pGameServer->GetStatisticsType( byCnt );
									pGameServer->ClearStatistics( byCnt );
									sendMsg.ui64Statitics[byCnt] = min( sendMsg.ui64Statitics[byCnt] + ui64Temp, ULLONG_MAX );
									break;
								}
							}
						}
					}
				}
			}

			// 한 채널 값 다 모았으면 전송
			sendMsg.byWorldID = byWorldID;
			sendMsg.byChannelID = byChannelID;
			sendMsg.byServerType = 0;
			sendMsg.byServerID = 0;

			pOpServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
#ifdef _DEBUG
			MessageOut( eFULL_LOG, "GameStatistics Send[%u/%u]", sendMsg.byWorldID, sendMsg.byChannelID );
#endif
		}
	}
}

// 창고 통화량
VOID MasterServer::CheckServerWarehouseMoney()
{
	WORLD_MAP_ITER		iterWorld;
	CHANNEL_MAP_ITER	iterChannel;
	SERVERKEY_VEC_ITER	iterServer;
	GameServerSession*	pGameServer = NULL;
	ULONGLONG			ui64TotalGetMoney, ui64TotalLoseMoney, ui64TotalUserMoney;
	ULONGLONG			ui64TempGetMoney, ui64TempLoseMoney, ui64TempUserMoney;
	MSG_MO_RTTG_WAREHOUSEMONEY_ANS		sendMsg;
	BYTE				byWorldID, byChannelID;

	ServerSession *pOpServer = GetOpServer();
	if( !pOpServer )
	{
		MessageOut( eCRITICAL_LOG, "RTTA 통화량 보고 불가능:No OpServer" );
		return;
	}

	for( iterWorld = m_WorldMap.begin(); iterWorld != m_WorldMap.end(); ++iterWorld )
	{
		byWorldID = (*iterWorld).first;

		if(! byWorldID )
			continue;

		CHANNEL_MAP& mapChannel = (*iterWorld).second;
		for( iterChannel = mapChannel.begin(); iterChannel != mapChannel.end(); ++iterChannel )
		{
			ui64TotalGetMoney = 0;
			ui64TotalLoseMoney = 0;
			ui64TotalUserMoney = 0;

			byChannelID = (*iterChannel).first;

			if( !byChannelID ) continue;	// 0번 채널은 보내지 않는다

			SERVERKEY_VEC& vecServer = (*iterChannel).second;

			for( iterServer = vecServer.begin(); iterServer != vecServer.end(); ++iterServer )
			{
				SERVER_KEY& serverKey = (*iterServer);

				switch( serverKey.GetServerType() )
				{
					case GAME_SERVER:
					case FIELD_SERVER:
					case BATTLE_SERVER:
					{
						pGameServer = (GameServerSession*)ServerSessionManager::Instance()->FindServer( serverKey );
						if( !pGameServer )
						{
							vecServer.erase( iterServer );
							continue;
						}
						ui64TempGetMoney = pGameServer->GetTotalGetMoney();
						ui64TempLoseMoney = pGameServer->GetTotalLoseMoney();
						ui64TempUserMoney = pGameServer->GetAveUserMoney();
						if( 0xffffffffffffffff - ui64TotalGetMoney < ui64TempGetMoney )
						{
							ui64TotalGetMoney = 0xffffffffffffffff;
						}
						else
						{
							ui64TotalGetMoney += pGameServer->GetTotalGetMoney();
						}
						if( 0xffffffffffffffff - ui64TotalLoseMoney < ui64TempLoseMoney )
						{
							ui64TotalLoseMoney = 0xffffffffffffffff;
						}
						else
						{
							ui64TotalLoseMoney += ui64TempLoseMoney;
						}
						if( 0xffffffffffffffff - ui64TotalUserMoney < ui64TempUserMoney )
						{
							ui64TotalUserMoney = 0xffffffffffffffff;
						}
						else
						{
							ui64TotalUserMoney += ui64TempUserMoney;
						}

						// WOPS에 값 넘겨 줬으니 초기화
						pGameServer->ClearWarehouseMoney();
						break;
					}
					default:
					{
						continue;
					}
				}
			}

			// 한 채널 통화량 다 구했으면 보고
			sendMsg.byWorldID = byWorldID;
			sendMsg.byChannelID = byChannelID;
			sendMsg.byServerType = 0;
			sendMsg.byServerID = 0;

			sendMsg.ui64AveMoney = ui64TotalUserMoney;
			sendMsg.ui64TotalPlusMoney = ui64TotalGetMoney;
			sendMsg.ui64TotalMinusMoney = ui64TotalLoseMoney;

			pOpServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
#ifdef _DEBUG
			MessageOut( eFULL_LOG, "WareHouseMoney Send[%u/%u][Get:%u][Lose:%u][User:%u]", sendMsg.byWorldID,
																					sendMsg.byChannelID,
																					sendMsg.ui64TotalPlusMoney,
																					sendMsg.ui64TotalMinusMoney,
																					sendMsg.ui64AveMoney );
#endif
		}
	}
}

VOID MasterServer::SendMoneyReq()
{
	// 모든 게임서버에 전달~
	std::vector<ServerSession*>				gameList;
	std::vector<ServerSession*>::iterator	iter;

	gameList = ServerSessionManager::Instance()->GetGameServerList();

	if( gameList.empty() )
	{
		// 비어 있으면 줄 값이 없다
		return;
	}

//	MSG_GM_OPERATION_USERMONEY_REQ	sendMsg;
	MSG_GM_OPERATION_STATISTICS_REQ	sendMsg;
	ServerSession *pGameServer = NULL;

	for( iter = gameList.begin(); iter < gameList.end(); iter++ )
	{
		// 전달~
		pGameServer = (*iter);

		pGameServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
	}
}


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

VOID MasterServer::StartDataService()
{
	MessageOut( eCRITICAL_LOG, "All Data Loaded!\n" );

	//#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__TEST__
	//	MasterServer::Instance()->StartListen();
	//#endif

	// 연결된 모든 서버들에게 PHASE_SHIFT_CMD :: PHASE_EXCHANGE 명령을 보낸다.
	// 연결된 서버들은 데이터 요청할 것이다.

	// 이전 코드 <060802>
	ServerSessionManager::Instance()->Send_PHASE_SHIFT_CMD_AllBroadcast( PHASE_EXCHANGE, PHASE_SERVICE );


	// 위와 같이 하면, PHASE_SERVICE 상태에 있는 경우라도 일시적으로 PHASE_EXCHANGE로
	// 전환되어 데이터 교환을 할 것이다. 하지만, 모두 읽은 경우라도 지정된 Control Flag만을
	// 읽어들이고 다시 SERVICE 로 전환되게 하자.
	// 마스터가 데이터를 읽어들이기 전에 요청할 경우는 대기 상태가 될 텐데, 이 경우도 적용받게
	// 될 것이다.
	// 이후 별도로 접속하는 경우라면, 개별 PHASE_EXCHANGE 명령을 통해 데이터를 받게 될 것이다.

	// 완료 명령이 올 경우, Field, Battle, DB 등등은 바로 PHASE_SERVICE 로 전환한다.
	// Agent, World 등의 Front 계열 또는 다른 서버들과의 연관도가 있는 경우
	// 늦게 PHASE_SERVICE로 전환시킨다. <- 어떻게 찾아야 하는 지, 감시해야 하는지를 판단할 것.
}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
