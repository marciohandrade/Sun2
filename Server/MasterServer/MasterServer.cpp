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

// ���� �������� ����
#include "Protocol_ServerCommon.h"
#include "PacketStruct_ServerCommon.h"
#include "PacketStruct_MO.h"

// ���� ���
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

// ��� ����
#include <StatisticsManager.h>

//=================================================================================================
// �ݹ��Լ�
//=================================================================================================
NetworkObject* CreateAcceptedObject()
{
	ServerSession *pServerSession = ServerSessionFactory::Instance()->AllocServerSession( TEMP_SERVER );
	return (NetworkObject*)pServerSession;
}

//=================================================================================================
// �ݹ��Լ�
//=================================================================================================
VOID DestroyAcceptedObject( NetworkObject *pNetworkObject )
{
	ServerSessionFactory::Instance()->FreeServerSession( (ServerSession*)pNetworkObject );
}

//=================================================================================================
// �ݹ��Լ�
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

	// INI ���� �Ľ�
	if( !MasterServerInfoParser::Instance()->ParseINI( "MasterServer.ini" ) )
	{
		printf( "MasterServer.ini parsing failed.\n" );
		return FALSE;
	}

	// ���� ���
#ifdef _DEBUG
	DisplayDebugInfo( GetServerType(), 0, TRUE, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, SERVER_RELEASE_BUILD_NO, 0, 0, 1 );
#else
	DisplayDebugInfo( GetServerType(), 0, FALSE, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, SERVER_RELEASE_BUILD_NO, 0, 0, 1 );
#endif

	//SUN Log File �ʱ�ȭ
	g_pSunLog = new CSunLog;
	SUNLOG_INFO* pSunLogInfo = MasterServerInfoParser::Instance()->GetSunLogInfo();
	if(g_pSunLog->Init(pSunLogInfo->bySunLogOption, (BYTE)pSunLogInfo->wSunLogfileLevel,
		pSunLogInfo->pszSunLogfilePath, "Master") == false)
	{
		//SunLog�� ������ �ʽ��ϴ�.
	}

	// IO ���� �ν��Ͻ� ���� �� �ʱ�ȭ
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

	// GM List �ε�
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
	descOUTSIDE.fnCreateAcceptedObject		= CreateAcceptedObject;		//�ٸ� ������ ��ü�� ��
	descOUTSIDE.fnDestroyAcceptedObject		= DestroyAcceptedObject;	//�ٸ� ������ ��ü�� ��
	descOUTSIDE.fnDestroyConnectedObject	= DestroyConnectedObject;

	m_pIOCPServer->Init( desc, 2 );

	//// GM List �ε�
	//GMList::Instance()->Load( "data\\GMList.txt" );

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	// ��Ŷ �ڵ鷯 ���
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

	// DB Proxy ���� ���� ���� ����	
	m_pAccountDBProxySession->SetAddr( MasterServerInfoParser::Instance()->GetAccountDBProxyIP(),
									  MasterServerInfoParser::Instance()->GetAccountDBProxyPort() );
	printf( "Account DB proxy address(%s:%d)\n", MasterServerInfoParser::Instance()->GetAccountDBProxyIP(),
												MasterServerInfoParser::Instance()->GetAccountDBProxyPort() );
	// Op Server ���� ���� ����
	m_pOpServerSession->SetAddr( MasterServerInfoParser::Instance()->GetOpServerIP(),
									MasterServerInfoParser::Instance()->GetOpServerPort() );

	MessageOut( eCRITICAL_LOG, "Op server address(%s:%d)", MasterServerInfoParser::Instance()->GetOpServerIP(),
											MasterServerInfoParser::Instance()->GetOpServerPort() );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	PacketHandler::Instance()->RegisterHandler_SM();
	m_pSolarAuthServerSession = (SolarAuthServerSession*)ServerSessionFactory::Instance()->AllocServerSession( SOLARAUTH_SERVER );
	// Solar Auth ���� ���� ���� ����	
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

	// ���� ��ȭ�� ���� �ʱ�ȭ
	m_pServerMoneyInfoTable = new util::SolarHashTable<SERVER_MONEY_INFO*>;
	m_pServerMoneyInfoTable->Initialize( 100 );

	// ���� ���� ���� �ʱ�ȭ
	m_bExitServer = FALSE;
	m_eExitType = eEXIT_NONE;
#ifdef _WHATTHEHELL_AGENTSHUTDOWN_20060524
	m_pExitAgentTable = new util::SolarHashTable<SERVER_KEY*>;
	m_pExitAgentTable->Initialize( 100 );
#endif
	m_pExitServerTable = new util::SolarHashTable<SERVER_KEY*>;
	m_pExitServerTable->Initialize( 1000 );		// �ִ� 1000�� ���� ���� ����?
	m_pExitDBServerTable = new util::SolarHashTable<SERVER_KEY*>;
	m_pExitDBServerTable->Initialize( 100 );	// �ִ� 100�� ���� ���� ����?
	m_ExitTimer.SetTimer( MasterServerInfoParser::Instance()->GetExitInterval() );	// ���� ���� ��� �ð�
	m_ExitTimer.DisableCheckTime();

	// ��ȭ�� �ֱ� �ʱ�ȭ
	m_MoneyTimer.SetTimer( MasterServerInfoParser::Instance()->GetStatisticsInterval() );

	return TRUE;
}

VOID MasterServer::StartListen()
{
	// DB Proxy�� ����� �Ŀ� ���� ����
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

	// Account DB pxory ������ DB ������ ���� ���� ������ ���� ���� �Ŵ����� �߰�
	SERVER_KEY serverKey = ServerInfoManager::Instance()->GetServerKey( ACCOUNT_DBPROXY, (char*)m_pAccountDBProxySession->GetConnectIP().c_str() );
	if( !ServerSessionManager::Instance()->FindServer( serverKey ) )
	{
		// ���� ������ ������Ʈ �ϴ� ��� �� �߰����� �ʰԲ� ����Ű�� ã�ƺ� �Ŀ� �߰�
		m_pAccountDBProxySession->SetServerKey( serverKey );
		ServerSessionManager::Instance()->AddServer( m_pAccountDBProxySession );
	}

	// Op Server ������ DB ������ ���� ���� ������ ���� ���� �Ŵ����� �߰�
	SERVER_KEY opServerKey = ServerInfoManager::Instance()->GetServerKey( OP_SERVER, (char*)m_pOpServerSession->GetConnectIP().c_str() );
	if( !ServerSessionManager::Instance()->FindServer( opServerKey ) )
	{
		// Op Server�� ������ �Ǿ��ִ� ��� �߰�!
		if( m_pOpServerSession->IsConnected() )
		{
			m_pOpServerSession->SetServerKey( opServerKey );
			ServerSessionManager::Instance()->AddServer( m_pOpServerSession );
		}
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// Solar Auth Server ������ DB ������ ���� ���� ������ ���� ���� �Ŵ����� �߰�
	SERVER_KEY solarAuthServerKey = ServerInfoManager::Instance()->GetServerKey( SOLARAUTH_SERVER, (char*)m_pSolarAuthServerSession->GetConnectIP().c_str() );
	if( !ServerSessionManager::Instance()->FindServer( solarAuthServerKey ) )
	{
		// ���� ������ ������Ʈ �ϴ� ��� �� �߰����� �ʰԲ� ����Ű�� ã�ƺ� �Ŀ� �߰�
		m_pSolarAuthServerSession->SetServerKey( solarAuthServerKey );
		ServerSessionManager::Instance()->AddServer( m_pSolarAuthServerSession );
	}
#endif
}

VOID MasterServer::Run()
{
	// Account DB Proxy�� ���� �õ�
//	ConnectToServer( m_pAccountDBProxySession, (char*)m_pAccountDBProxySession->GetConnectIP().c_str(), m_pAccountDBProxySession->GetConnectPort() );

	// Op Server�� ���ӽõ�
//	ConnectToServer( m_pOpServerSession, (char*)m_pOpServerSession->GetConnectIP().c_str(), m_pOpServerSession->GetConnectPort() );

	// ���� ����
	while( !m_bShutdown )
	{
		Sleep( 10 );

		if( !Update() ) break;
	}
}

BOOL MasterServer::Update()
{
	// I/O ���� ������Ʈ
	m_pIOCPServer->Update();

	UpdateFPS();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	WorkLoadManager::Update();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	ServerSessionManager::Instance()->Update();

	g_pSunLog->Update();

	// �ܼ�ó��
	if( ProcessConsole() == FALSE )
	{
		return FALSE;	// ���� ����
	}

	if( m_ReconnectTimer.IsExpired() )
	{
		MaintainConnection();
	}

	// �� ó��
	if( m_MoneyTimer.IsExpired() )
	{
		SendMoneyReq();
	}

	return TRUE;
}

VOID MasterServer::MaintainConnection()
{
	// ������ ������ �������̸� ����
	if( m_bShutdown )						return;

	// Account DB proxy�� ������ ������ ������ �ٽ� ������ �õ��Ѵ�.
	if( !m_pAccountDBProxySession->IsConnected() && !m_pAccountDBProxySession->GetConnectIP().empty() )
	{
		ConnectToServer( m_pAccountDBProxySession, (char*)m_pAccountDBProxySession->GetConnectIP().c_str(), m_pAccountDBProxySession->GetConnectPort() );
	}
/*
	// Ȥ�� ���� ������ OpServer�� NULL�̸�
	if( ServerSessionManager::Instance()->GetOpServer() == NULL )
	{
		// m_pOpServerSession�� �����ش�
		ServerSessionManager::Instance()->AddServer( m_pOpServerSession );
	}
*/
	// Op Server�� ������ ������ ������ ������	
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

	// �������� ����� WOPS���� �޾Ҵ��� check�ϰ� �ʿ��� ���� ���� ����
	CheckExitServer();
//	if( CheckExitServer() )
//	{
//		Shutdown();
//	}
}

#include <PacketStruct_MX.h>

// ��� ���� ���� DB���� �о������ ��û
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
	// ���� ��ɵ� ���� �����ش�
	MSG_GM_OPERATION_FUNCRESTRICT_CMD	funcMsg;

	MasterServerInfoParser::Instance()->GetFuncRestrictInfo( (BYTE*)&funcMsg );

	// ���� �־�� �����ش�
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
			// ���͹� ���� ���ε�
			MasterServerInfoParser::Instance()->ReloadInterval();
			m_ExitTimer.Reset();
			m_ExitTimer.SetTimer( MasterServerInfoParser::Instance()->GetExitInterval() );	// ���� ���� ��� �ð�
			m_ExitTimer.DisableCheckTime();

			// ��ȭ�� �ֱ� �ʱ�ȭ
			m_MoneyTimer.Reset();
			m_MoneyTimer.SetTimer( MasterServerInfoParser::Instance()->GetStatisticsInterval() );
			MessageOut( eCRITICAL_LOG, "Interval Info Reload[Exit:%u][Statistics:%u]", m_ExitTimer.GetIntervalTime(), 
																						m_MoneyTimer.GetIntervalTime() );
			break;
		case 'R':
			if( MasterServerInfoParser::Instance()->IsRestrictInfoFromFile() )
			{
				MasterServerInfoParser::Instance()->LoadFuncRestrictInfo();

				// ���� ��ɵ� ���� ������ �����ش�
				MSG_GM_OPERATION_FUNCRESTRICT_CMD	funcMsg;

				MasterServerInfoParser::Instance()->GetFuncRestrictInfo( (BYTE*)&funcMsg );

				// ���� �־�� �����ش�
				if( funcMsg.byRestrictNum )
				{
					ServerSessionManager::Instance()->SendGameServer( (BYTE*)&funcMsg, funcMsg.GetSize() );
				}
				MessageOut( eCRITICAL_LOG, "Reload FuncRestrict Setting AND Broadcast" );
			}
			else	// DB���� �о���� ��� DB�� ���� ��ɵ� ���� ��û
			{
				SendRestrictReq();
			}

#ifdef _TEST_ABUSE
			// �׽�Ʈ�� �ڵ� - ����� ���¡ ������
			MSG_MO_RTTG_ABUSING_NTF	sendMsg;
			SERVER_KEY				serverKey;

			serverKey.Set( 1, 2, 3, 4 );
			GetDateTime_YYMMDDHHMMSS( sendMsg.m_dw64Time );
			sendMsg.m_dwUserGuid	= 3724;
			sendMsg.m_dwCharGuid	= 12938;
			sendMsg.m_ServerKey		= serverKey;
			sendMsg.m_byAbuseType	= 1;

			wsprintf( sendMsg.m_ptszDesc, "���� �׽�Ʈ ������~[3724][12938][1,2,3,4][3]" ); 
			sendMsg.m_byLength		= _tcslen(sendMsg.m_ptszDesc);

			ServerSession *pOpServer = GetOpServer();
			if( !pOpServer )
			{
				MessageOut( eCRITICAL_LOG, "RTTA Abusing ���� ����:No RTTA[Type:%u][UserGuid:%u][CharGuid:%u][Desc:%u]", sendMsg.m_byAbuseType, 
																														sendMsg.m_dwUserGuid,
																														sendMsg.m_dwCharGuid,
																														sendMsg.m_ptszDesc );
				return FALSE;
			}

			pOpServer->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );

			MessageOut( eCRITICAL_LOG, "RTTA Abusing Alarm:No RTTA[Type:%u][UserGuid:%u][CharGuid:%u]", sendMsg.m_byAbuseType, 
																												sendMsg.m_dwUserGuid,
																												sendMsg.m_dwCharGuid );

			// ����� ���� ������ ���ε�
//			SendRestrictReq();
#endif




			// RTTA ��� ���µ� �о ���� - �� �ڵ�� �ϴ� ����
//			MasterServerInfoParser::Instance()->LoadRTTARestrictInfo();
//			MessageOut( eCRITICAL_LOG, "Reload RTTARestrict Setting" );
			break;
/*
		case 'S':
			//�׽�Ʈ - ����� ���� ���� ��û

			SERVER_KEY exitKey;
			exitKey.Set( 0, 255, 0, 0 );
			ServerSessionManager::Instance()->AddExitServerList( exitKey );

			SetExitServer();	// ���� ��� ����!
			SetExitKey( 0, 0 );
			SetExitType( eEXIT_AGENTSTART );

			//���� ����� ����������(�ʵ�/��Ʋ/������Ʈ->DB������) �̷������ �ϹǷ�, ���� ���� ���� ���� ó���ؾ� �Ѵ�
			MessageOut( eFULL_LOG, "���� ���� ����� ����[W:%u/C:%u/T:%u/I:%u]", exitKey.GetWorldID(), 
																					exitKey.GetChannelID(),
																					exitKey.GetServerType(), 
																					exitKey.GetServerID() );
			break;
		case 'D':
			// �׽�Ʈ-�������� ��û
			// ���� �������� ��� Agent �������� �� ���������� �����Ѵ�
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

	// �� �Ⱦ ���� ���� �ַ�� �����ش�
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

	assert( !"�׷� ���� Ÿ���� �����" );

	return FALSE;	
}

/*
VOID MasterServer::SetMoney( SERVER_KEY serverkey, ULONGLONG &ui64GetMoney, ULONGLONG &ui64LoseMoney, ULONGLONG &ui64UserMoney )
{
	SERVER_MONEY_INFO *pServer = m_pServerMoneyInfoTable->GetData( serverkey );
	if(pServer == NULL)
	{
		// ������ �����
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
		// ������ ����
		pServer->ui64TotalGetMoney += ui64GetMoney;
		pServer->ui64TotalLoseMoney += ui64LoseMoney;
		pServer->ui64TotalUserMoney = ui64UserMoney;	// ��� ���ϸ� �ȵȴ�
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

// �����ų ���� �߰�
BOOL MasterServer::AddExitServer( SERVER_KEY sServerKey )
{
	SERVER_KEY *pServerKey = new SERVER_KEY;

	pServerKey->m_dwKey = sServerKey.m_dwKey;

	return m_pExitServerTable->Add( pServerKey, pServerKey->m_dwKey );
}

// �����ų ���� ����
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

// �����ų ���� �߰�
BOOL MasterServer::AddExitDBServer( SERVER_KEY sServerKey )
{
	SERVER_KEY *pServerKey = new SERVER_KEY;

	pServerKey->m_dwKey = sServerKey.m_dwKey;

	return m_pExitDBServerTable->Add( pServerKey, pServerKey->m_dwKey );
}

// �����ų ���� ����
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
//		SetExitType( FALSE );	// �� ���´�
		return;
	}
//	SetExitType( FALSE );	// �� ���´�
	MessageOut( eFULL_LOG, "RemoveExitDBServer [%u/%u/%u/%u]", serverKey.GetWorldID(), 
															serverKey.GetChannelID(), 
															serverKey.GetServerType(), 
															serverKey.GetServerID() );
}

BOOL MasterServer::CheckExitServer()
{
/*
	// �׽�Ʈ �ڵ� - 2�ʿ� �ѹ��� ��¥ ���弭�� ������ ������
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
			MessageOut( eCRITICAL_LOG, "RTTA ������ ���� ����:No Data" );	
			return FALSE;
		}

		for( BYTE byWorld = 1; byWorld < 6; byWorld++ )
		{
			if( rand() % 2 )
			{
				continue;
			}

			// ��Ʋ�� ����
			battleMsg.byCount = 3;

			// ����
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
				MessageOut( eCRITICAL_LOG, "RTTA�� ������ ����: W(%u)C(%u)V(%u/%u)B(%u/%u)", sendMsg.dwWorldID, 
																					sendMsg.dwChannelID, 
																					sendMsg.dwVillageCount, 
																					sendMsg.dwVillagePCRoomCount,
																					sendMsg.dwBattleZoneCount,
																					sendMsg.dwBattleZonePCRoomCount );
				dwTotalVillageCount += sendMsg.dwVillageCount;
				dwTotalVillagePCRoomCount += sendMsg.dwVillagePCRoomCount;
				dwTotalBattleZoneCount += sendMsg.dwBattleZoneCount;
				dwTotalBattleZonePCRoomCount += sendMsg.dwBattleZonePCRoomCount;

				// ��Ʋ��
				battleMsg.ChannelInfo[i].byWorldID = byWorld;
				battleMsg.ChannelInfo[i-1].byChannelID = i;
				battleMsg.ChannelInfo[i-1].byWorldID = byWorld;
				memset( &battleMsg.ChannelInfo[i].dwZoneCount, 0, sizeof(DWORD) * eZONETYPE_MAX );
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_VILLAGE] = sendMsg.dwVillageCount;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_FIELD] = sendMsg.dwVillagePCRoomCount;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_HUNTING] = sendMsg.dwBattleZonePCRoomCount;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_MISSION] = sendMsg.dwBattleZoneCount - 100;
				battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_PVP] = 100;
				_snprintf(szMsg, sizeof(szMsg), "��Ʋ�� ���� : W(%d)C(%d)V(%u)H(%u)M(%u)PVP(%u)", battleMsg.ChannelInfo[i-1].byWorldID, 
																							battleMsg.ChannelInfo[i-1].byChannelID, 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_VILLAGE], 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_HUNTING], 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_MISSION], 
																							battleMsg.ChannelInfo[i-1].dwZoneCount[eZONETYPE_PVP] );

				SUNLOG->InsertMessage( szMsg, eMIDDLE_LOG );
			}
			MessageOut( eCRITICAL_LOG, "���� ���� Ȯ��: W(%u)V(%u/%u)B(%u/%u)", sendMsg.dwWorldID, 
																	dwTotalVillageCount, 
																	dwTotalVillagePCRoomCount,
																	dwTotalBattleZoneCount,
																	dwTotalBattleZonePCRoomCount );

			pOpServer->Send( (BYTE*)&battleMsg, battleMsg.GetSize() );

			dwLastTime = dwThisTime;
		}
	}
	// �׽�Ʈ �ڵ� ��
*/

	// ���� ���� ���μ��������� Ȯ���Ѵ�
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
		case eEXIT_AGENTSTART:	// ������Ʈ ���� ��Ŷ ������
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
		case eEXIT_AGENTWAIT:	// ������Ʈ ���� ��Ŷ ������ ���
		{
			if( m_pExitAgentTable->GetDataNum() )
			{
				m_eExitType = eEXIT_GAMESTART;
				MessageOut( eFULL_LOG, "Send AgentExit Complete" );
				break;
			}

			// ���� �ð� �̻� ���� ������ ���� ���� ó��
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
						// ���� ����
						sendMsg.dwError = 1;
						sendMsg.m_ServerKey = *pExitServerKey;
						sendMsg.dwKey = GetExitKey();
						sendMsg.i64WopsKey = GetExit64Key();

						if( !m_bExitFailed )
						{
							pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );
							m_bExitFailed = TRUE;
						}

						// �̹� ó���� ����Ű�� ����
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
		case eEXIT_GAMESTART:	// ���� ���� ��Ŷ ������
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
		case eEXIT_GAMEWAIT:	// ���� ���� ��Ŷ ������ ���
		{
			if( !m_pExitServerTable->GetDataNum() )
			{
				m_eExitType = eEXIT_DBPROXY;
				MessageOut( eFULL_LOG, "Send ServerExit Complete" );
				break;
			}

			// ���� �ð� �̻� ���� ������ ���� ���� ó��
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
						// ���� ����
						sendMsg.dwError = 1;
						sendMsg.m_ServerKey = *pExitServerKey;
						sendMsg.dwKey = GetExitKey();
						sendMsg.i64WopsKey = GetExit64Key();

						if( !m_bExitFailed )
						{
							pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );

							m_bExitFailed = TRUE;
						}

						// �̹� ó���� ����Ű�� ����
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
		case eEXIT_DBPROXY:		// DB ���� ��Ŷ ������
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
		case eEXIT_DBPROXYWAIT:	// DB ���� ��Ŷ ������ ���
		{
			if( !m_pExitDBServerTable->GetDataNum() )
			{
				m_eExitType = eEXIT_END;
				MessageOut( eFULL_LOG, "Send DBServerExit Complete" );
				break;
			}

			// ���� �ð� �̻� ���� ������ ���� ���� ó��
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
						// ���� ����
						sendMsg.dwError = 1;
						sendMsg.m_ServerKey = *pExitServerKey;
						sendMsg.dwKey = GetExitKey();
						sendMsg.i64WopsKey = GetExit64Key();

						if( !m_bExitFailed )
						{
							pOpServer->Send( (BYTE*)&sendMsg, sizeof(MSG_MO_RTTG_SERVERSHUTDOWN_ANS) );

							m_bExitFailed = TRUE;
						}

						// �̹� ó���� ����Ű�� ����
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
			// ����Ǿ��ٴ� ������ RTTA�� ������
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
		default:				// ����
		{
			break;
		}
	}
/*
		if( m_ExitTimer.IsExpired() )
		{
			// 10�� �̻� ���� ������ ���� ��Ŷ ����
			MessageOut( eCRITICAL_LOG, "ExitServer Error - [%u/%u/%u/%u] No Response", m_ExitServerKey.GetWorldID(), 
																					m_ExitServerKey.GetChannelID(), 
																					m_ExitServerKey.GetServerType(), 
																					m_ExitServerKey.GetServerID() );

			// ������ �˷��ְ� ���� ������ �Ѿ��
			SERVER_KEY *pExitServerKey = m_pExitServerTable->GetHeadData();

			if( pExitServerKey )
			{
				// ���� ����
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

				// �̹� ó���� ����Ű�� ����
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
		// ������ ������ ���̺� ��� �ִ��� Ȯ��
		if( m_pExitServerTable->GetDataNum() > 0 )
		{
			// �տ� �ϳ� �����
			util::SolarHashTable<SERVER_KEY *>::iterator iter = m_pExitServerTable->begin();
			SERVER_KEY ExitServerKey = *(*iter);

			ServerSession *pSession = ServerSessionManager::Instance()->FindServer( ExitServerKey );

			if( !pSession )
			{
				//�̹� ����
				m_pExitServerTable->erase( iter );	// �����͸� �����
				if( 0 == m_pExitServerTable->GetDataNum() )
				{
					// ������ ���� �� ���� ������ ��~
					m_bExitServer = FALSE;
				}
			}
			else	// ���� ������ ����
			{
				// DB Proxy �������
				if( pSession->GetServerType() == GAME_DBPROXY || ACCOUNT_DBPROXY )
				{
					// �ٸ� ���� �����ϴ� �ð� ���� ����ϱ� ���� 5�� �ش�
					if( dwLastTick + 5000 > dwTickCount )
					{
						return FALSE;
					}
					dwLastTick = GetTickCount();
				}
				// ���� Ÿ�Կ� ����
				MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ sendMsg;

				sendMsg.m_byCategory		= 0;
				sendMsg.m_byProtocol		= SERVERCOMMON_SERVERSHUTDOWN_REQ;

				pSession->Send( (BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ) );
				SetExitType( TRUE );	// ���� ��û ���´�!
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

			// �� ������~
		}
	}

	if( IsExitServer() && !IsExitType() && m_pExitServerTable->GetDataNum() == 0 )
	{
		// ���� ���� ��Ȳ�̸�!
		SetExitServer( FALSE );
		// ����Ǿ��ٴ� ������ RTTA�� ������
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

		// �� ������~
        SetExitServer( FALSE );

		// ���� ��ü ������ �ڱ� �ڽŵ� �����Ѵ�
		if( m_ExitServerKey.GetWorldID() == 0 )
		{
			return TRUE;
		}
	}
*/
	return FALSE;
}

// ��� �ִ� ���� ��Ͽ� �߰�
VOID MasterServer::AddServerMap( SERVER_KEY &serverKey )
{
	WORLD_MAP_ITER	iter = m_WorldMap.find( serverKey.GetWorldID() );

	// �ش� ���尡 ������ �߰�
	if( iter == m_WorldMap.end() )
	{
		CHANNEL_MAP	channelMap;

		channelMap.clear();

		// ä�ο� ���� Ű�� �߰�
		SERVERKEY_VEC	serverKeyVec;

		serverKeyVec.clear();

		serverKeyVec.push_back( serverKey );

		channelMap.insert( CHANNEL_MAP_PAIR( serverKey.GetChannelID(), serverKeyVec ) );
		m_WorldMap.insert( WORLD_MAP_PAIR( serverKey.GetWorldID(), channelMap ) );
	}
	// �ش� ������ ä�� �� ������ �ű⿡ ������ �߰�
	else
	{
		CHANNEL_MAP &channelMap = (*iter).second;

		CHANNEL_MAP_ITER iterChannel = channelMap.find( serverKey.GetChannelID() );

		// �ش� ä���� ���� ������ �߰�
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

// ���Ӽ������� ��ȭ�� ���� ó��
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
		MessageOut( eCRITICAL_LOG, "RTTA ��ȭ�� ���� �Ұ���:No OpServer" );
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

			if( !byChannelID ) continue;	// 0�� ä���� ������ �ʴ´�

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

						// �ʿ��� ��踸 ����
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

						// WOPS�� �� �Ѱ� ������ �ʱ�ȭ
						pGameServer->ClearMoney();
						break;
					}
					default:
					{
						continue;
					}
				}
			}

			// �� ä�� ��ȭ�� �� �������� ����
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

// ��Ÿ ��� ����
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
		MessageOut( eCRITICAL_LOG, "RTTA ��ȭ�� ���� �Ұ���:No OpServer" );
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

			if( !byChannelID ) continue;	// 0�� ä���� ������ �ʴ´�

			memset( sendMsg.ui64Statitics, 0, sizeof(sendMsg.ui64Statitics) );	// ������ �� �ʱ�ȭ(ä�κ�)

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

						// ��� ��� ����
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
									// ���� ��ȭ�� ������ ���ԵǴ� ������ �� ���� �ʿ� ����
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

			// �� ä�� �� �� ������� ����
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

// â�� ��ȭ��
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
		MessageOut( eCRITICAL_LOG, "RTTA ��ȭ�� ���� �Ұ���:No OpServer" );
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

			if( !byChannelID ) continue;	// 0�� ä���� ������ �ʴ´�

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

						// WOPS�� �� �Ѱ� ������ �ʱ�ȭ
						pGameServer->ClearWarehouseMoney();
						break;
					}
					default:
					{
						continue;
					}
				}
			}

			// �� ä�� ��ȭ�� �� �������� ����
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
	// ��� ���Ӽ����� ����~
	std::vector<ServerSession*>				gameList;
	std::vector<ServerSession*>::iterator	iter;

	gameList = ServerSessionManager::Instance()->GetGameServerList();

	if( gameList.empty() )
	{
		// ��� ������ �� ���� ����
		return;
	}

//	MSG_GM_OPERATION_USERMONEY_REQ	sendMsg;
	MSG_GM_OPERATION_STATISTICS_REQ	sendMsg;
	ServerSession *pGameServer = NULL;

	for( iter = gameList.begin(); iter < gameList.end(); iter++ )
	{
		// ����~
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

	// ����� ��� �����鿡�� PHASE_SHIFT_CMD :: PHASE_EXCHANGE ����� ������.
	// ����� �������� ������ ��û�� ���̴�.

	// ���� �ڵ� <060802>
	ServerSessionManager::Instance()->Send_PHASE_SHIFT_CMD_AllBroadcast( PHASE_EXCHANGE, PHASE_SERVICE );


	// ���� ���� �ϸ�, PHASE_SERVICE ���¿� �ִ� ���� �Ͻ������� PHASE_EXCHANGE��
	// ��ȯ�Ǿ� ������ ��ȯ�� �� ���̴�. ������, ��� ���� ���� ������ Control Flag����
	// �о���̰� �ٽ� SERVICE �� ��ȯ�ǰ� ����.
	// �����Ͱ� �����͸� �о���̱� ���� ��û�� ���� ��� ���°� �� �ٵ�, �� ��쵵 ����ް�
	// �� ���̴�.
	// ���� ������ �����ϴ� �����, ���� PHASE_EXCHANGE ����� ���� �����͸� �ް� �� ���̴�.

	// �Ϸ� ����� �� ���, Field, Battle, DB ����� �ٷ� PHASE_SERVICE �� ��ȯ�Ѵ�.
	// Agent, World ���� Front �迭 �Ǵ� �ٸ� ��������� �������� �ִ� ���
	// �ʰ� PHASE_SERVICE�� ��ȯ��Ų��. <- ��� ã�ƾ� �ϴ� ��, �����ؾ� �ϴ����� �Ǵ��� ��.
}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
