//=============================================================================================================================
/// SolarAuthServer class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	@history 
	-  
	*/
//=============================================================================================================================


// SolarAuthServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include <Version.h>
#include <ServerBuildVersion.h>

#include <IOCPServer.h>
#include <NetworkObject.h>
#include <PacketControl/PacketControl.h>

//=========================================================================================================
// SolarAuthServer Include Files
//

#include "SolarAuthServer.h"

#include "Sessions/ServerSession.h"
#include "Sessions/ServerSessionManager.h"

#include "PacketHandler/PacketHandler.h"

#include "Information/ScriptLoadManager.h"

#include <WorkLoad/WorkLoadManager.h>

#include <Information/ReportFromMasters.h>
//
//=========================================================================================================

#define SOLAR_AUTH_SERVER_INFO_FILE_NAME	".\\SolarAuthServer.ini"
//===================================================================================================
//
//

SolarAuthServer::SolarAuthServer()
{
	m_bShutdown		= FALSE;
}

SolarAuthServer::~SolarAuthServer()
{
}

BOOL	SolarAuthServer::Init()
{
	// (����/�ܼ�)��� ���� Ȯ���� ��.
	__READ_CONFIG_NUMBER( m_bServiceMode, "COMMON", "SERVICE_MODE", 0, SOLAR_AUTH_SERVER_INFO_FILE_NAME );
	//UINT value;
	//__READ_CONFIG_NUMBER( value, "COMMON", "S", 0, SOLAR_AUTH_SERVER_INFO_FILE_NAME );
	//m_ServerMode = (eSERVER_MODE)value;
	nsPacket::Environment::Init();

	WorkLoadManager::CreateInstance();

	//-------------------------------------------------------------------------------------------------
	// <4> SunLog
	InitializeLogger();

	ScriptLoadManager* pScriptManager = ScriptLoadManager::Instance();
	pScriptManager->Initialize(
							"SolarAuthServer.EncryptedInfo.ini",		//���� ���� �ٸ� ���� �о���δ�.
							"SolarAuthServer.EncryptedInfo.ini",		//���� ���� �ٸ� ���� �о���δ�.
							"EncryptedScriptList.ini" );

	ERROR_PARSER( pScriptManager->LoadSolarAuthInfo() );
	ERROR_PARSER( pScriptManager->LoadAcceptableMasterServerList() );
	ERROR_PARSER( pScriptManager->LoadData() );
//	ERROR_PARSER( pScriptManager->LoadServerListPerMaster() );

	// ���� ȯ�����κ���? �ƴ� ������ �����κ��� �ش� ������ �о���δ�.
	eCODE_NATION nationCode;	// m_pSolarAuthInfoParser->GetNationCode();
	eCODE_REGION regionCode;	// m_pSolarAuthInfoParser->GetNationCode();
	eMODE_SERVER serverMode;	// m_pSolarAuthInfoParser->GetNationCode();
	PADDING_T padType = 0;

	m_GlobalServerKey.Set( nationCode, regionCode, serverMode, padType );

	//	SetChannelID( pServerEnv->wSolarAuthServerID );	//< ä�� ID
	//m_ServerKey.Set( (BYTE)m_ServerKey.GetWorldID(), (BYTE)pServerEnv->wSolarAuthServerID, (BYTE)GetServerType(), (BYTE)m_ServerKey.GetServerID() );

	//-------------------------------------------------------------------------------------------------
	// <6> Network���� �ʱ�ȭ
	//    SessionManager
	//    Init. Network Engine

	PacketHandler::RegisterHandler();
	ServerSessionManager::Instance();

	m_MasterServerPool.Initialize( 20, 10 );
	m_TempServerPool.Initialize( 20, 10 );
	m_ADBPServerPool.Initialize( 1 );

	//-------------------------------------------------------------------------------------------------
	// <6> ���ÿ��� �����ϴ� ����� �ִ� ��� �ּ� ����
	//

	//-------------------------------------------------------------------------------------------------
	// <7> ��Ŷ ��ȣȭ�� ���� ȯ�� ����
	//    <NOTE>
	//       �̰��� TempServer���� �����ͼ����� �����ް� �� ���, ��Ŷ�� ��ȣȭ �ϴ� ���������� ��ģ��.
/*
	// ��Ŷ ��ȣȭ ���� ���� ����
	if( pServerEnv->CryptographyIsEnabled() )
		User::CryptographyEnabledByEnv();
	if( pServerEnv->CryptographyKeyChangeIsEnabled() )
	{
		User::CryptographyKeyChangeEnabledByEnv( pServerEnv->dwCrypto_KeyChange_Interval );
		UserCrypts::GetCryptUpdateTimer().SetTimer( pServerEnv->dwCrypto_KeyChange_Interval );
	}
*/
	do
	{   //__NA000000_070523_REPORT_TO_SOLARAUTH
		ReportFromMasters* pWorkNode = new ReportFromMasters();
		m_wkReportWorkKey = WorkLoadManager::RegisterTask( pWorkNode );
		if( WorkLoadManager::c_NotExistKey == m_wkReportWorkKey )
		{
			MessageOut( eCRITICAL_LOG, _T("Can't Create Report WorkNode") );
			return FALSE;
		}
	} while(0);

#if !defined(__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__)
	SolarAuthServer::StartServer();
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__

	return TRUE;
}

VOID	SolarAuthServer::Release()
{
	//m_bShutdown = TRUE;

	//SAFE_DELETE(m_pFileParser);
	SAFE_DELETE(g_pSunLog);

	// Ŀ��Ʈ�� �̿��� ���� ��ü ��ȯ
	// ������...

	WorkLoadManager::DestroyInstance();
	PacketHandler::ClosePacketHandler();
}

VOID	SolarAuthServer::Finalize()
{
	ServerSessionManager::DestroyInstance();
	ScriptLoadManager::DestroyInstance();
}

VOID	SolarAuthServer::StartServer()
{
	Listen( SERVER_IOHANDLER );
}

BOOL	SolarAuthServer::Update( DWORD tick )
{
	// <2> Server Session ����
	WorkLoadManager::Update();
//	ServerSessionManager::Instance()->Update();

	// <4> �ֱ��� Ű��ȯ... �ʿ��Ѱ�? Ÿ���� ��.
	//if( User::CryptographyKeyChangeIsEnabledByEnv() )
	//{
	//	if( UserCrypts::GetCryptUpdateTimer().IsExpired( TRUE ) )
	//	{
	//		UserCrypts::Update();
	//	}
	//}

	// SunLog Update ��ġ�� ��������� �α� ���� ���� ����ؼ� �缳���� ��.
	g_pSunLog->Update();

	return TRUE;
}

VOID	SolarAuthServer::DisplayPoolInfo()
{
	//DISPMSG( "================= Pool Info =================\n" );
	//g_ZoneManager.DisplayerZonePoolInfo();
	//g_UserFactory.DisplayPoolInfo();
	//PartyManager::Instance()->DisplayPoolInfo();
}

VOID	SolarAuthServer::DisplayServerInfo()
{
	DISPMSG( "============== SolarAuthServer ==============\n" );
//	DISPMSG( "Frame Per Second		: %d\n"		, m_dwFPS );
	DISPMSG( "Server Connections		: %d\n"	, GetIOCPServer()->GetNumberOfConnections( OUTSIDE_IOHANDLER ) );
	DISPMSG( "Master Server Connections	: %d\n"	, GetIOCPServer()->GetNumberOfConnections( INSIDE_IOHANDLER ) );
}

VOID	SolarAuthServer::DisplayServerListInfo()
{
	int iWritten=0;
	int iServerCounts = GetIOCPServer()->GetNumberOfConnections( OUTSIDE_IOHANDLER );

	const int iMaxStr = 4095;
	const CHAR* strfmtServerInfo = "[%02u] %-24s:  %s:%05u\n";
	const CHAR* strfmtServerIPInfo = "[%02u] %-24s:  %u.%u.%u.%u:%05u\n";
	CHAR strServerInfo[iMaxStr+((int)sizeof(CHAR))];	// 4096

	// UNICODE�� ������ ��
	iWritten += _snprintf( strServerInfo, iMaxStr, "============= Connected MasterServer Session List ==============\n" );

	DWORD count = 0;
	SERVER_SESSION_CONTAINER_DELEGATOR pSessionList = ServerSessionManager::Instance()->GetConnectedServerList();

	SERVER_SESSION_CONTAINER_ITER it, end(pSessionList->end());

	for( it = pSessionList->begin() ; it != end ; ++it )
	{
		SERVER_SESSION_DELEGATOR pServerSession = it->second;
		TCHAR* ptstr = GetServerType2String( pServerSession->GetServerType() );

		WORD wConnPort = pServerSession->GetConnectPort();
		if( wConnPort )
		{
			iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, strfmtServerInfo,
				pServerSession->GetSessionIndex(),
				ptstr, pServerSession->GetConnectIP().c_str(), wConnPort );
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
				pServerSession->GetSessionIndex(),
				ptstr,
				uAddr.byAddrT[0], uAddr.byAddrT[1], uAddr.byAddrT[2], uAddr.byAddrT[3], wPort );
		}
		++count;
	}

	iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, " Connected Session Counts : %3d\n", count );
	iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten, "================================================================\n\n" );


	// ����� ���Ϸ���?
	//iWritten = _snprintf( strfmtServerInfo+iWritten, iMaxStr, strfmtServerInfo, 
	if( iWritten < 0 )	// �׷� ���ɼ��� �ִ°�? ���� ��������...-_-;;;
	{
		SUNLOG( eCRITICAL_LOG, "���� ��� ���� Full" );
	}


	//
	iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten,
		"===================== Session Buffer Info =====================\n" );

	for( it = pSessionList->begin() ; it != end ; ++it )
	{
		SERVER_SESSION_DELEGATOR pServerSession = it->second;

		DWORD dwCurSendBuffer = GetIOCPServer()->GetCurSendBuffer( OUTSIDE_IOHANDLER, pServerSession->GetSessionIndex() );
		DWORD dwCurRecvBuffer = GetIOCPServer()->GetCurRecvBuffer( OUTSIDE_IOHANDLER, pServerSession->GetSessionIndex() );

		iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten,
			"SessionIndex[%d] dwCurSendBuffer[%d] : dwCurRecvBuffer[%d] \n",
			pServerSession->GetSessionIndex(), dwCurSendBuffer, dwCurRecvBuffer );
	}
	iWritten += _snprintf( strServerInfo+iWritten, iMaxStr-iWritten,
		"===============================================================\n" );

	puts( strServerInfo );
}

//VOID	SolarAuthServer::Shutdown() {}


ServerSession*	SolarAuthServer::AllocServerSession( eSERVER_TYPE serverType )
{
	SERVER_SESSION_DELEGATOR pServerSession =
		(ServerSessionEx*)SolarAuthServer::Handler_AllocServerSession_NoInit( serverType );

	if( pServerSession() )
		((ServerSession*)pServerSession())->Init( this );

	return (ServerSession*)pServerSession();
}

VOID			SolarAuthServer::FreeServerSession( ServerSession* pServerSession )
{
	SolarAuthServer::Handler_FreeServerSession_NoRelease( pServerSession );
}



///------------------------------------------------------------------------------------------------
/// Ŭ���̾�Ʈ�� �ݹ� (�̰��� �Ͻ������� �ܺηκ��� ������ ������ �ǹ��Ѵ�.)
//NetworkObject*	SolarAuthServer::CreateClientSideAcceptedObject()
//{
//	ServerSession* pServerSession = ServerSessionFactory::Instance()->AllocServerSession( TEMP_SERVER );
//	return (NetworkObject*)pServerSession;
//}
//
//VOID			SolarAuthServer::DestroyClientSideAcceptedObject( NetworkObject* pNetworkObject )
//{
//	ServerSession* pServerSession = (ServerSession*)pNetworkObject;
//	ServerSessionFactory::Instance()->FreeServerSession( (ServerSession*)pNetworkObject );
//}
//
//VOID			SolarAuthServer::DestroyClientSideConnectedObject( NetworkObject* pNetworkObject )
//{
//	__UNUSED(pNetworkObject);
//}

//
//

BOOL SolarAuthServer::KeyEvent( CHAR ch )
{
	switch(ch)
	{
	case 'L':	//����� ������ ��� �����ֱ�
		{
			DisplayServerListInfo();
		}
		break;
	case 'C':
		{
			//DISPMSG( "============== Client Buffer Info ============================\n" );
			//m_pIOCPServer->Display( INSIDE_IOHANDLER, 5000 );
			//DISPMSG( "===============================================================\n" );
		}
		break;
	case 'S':	//Show Packet Stream in User
		{
			//DisplayPacketStream();
#ifdef	_DEBUG
			//PACKET_CONTROL::SetShowPacketStream( !PACKET_CONTROL::GetShowPacketStream() );
#endif
		}
		break;
	case 'P':
		{	
			DisplayPoolInfo();
		}break;
	case 'Q':
		{
			//MSG_CG_CONNECTION_RESTART_CMD msg;
		}
		break;
	case 'Y':
		{
		}
		break;
	}

	return TRUE;
}


//====================================================================================================================================
// Network Manatain
//

//VOID SolarAuthServer::Listen( eIOHANDLER where )
//{
//	if( BOTH_IOHANDLER == where || OUTSIDE_IOHANDLER == where )
//	{
//		if( m_wOutSideListenPort && !m_pIOCPServer->IsListening( OUTSIDE_IOHANDLER ) )
//		{
//			DISPMSG( "[SolarAuthServer::StartListen]\n\tStarting Out-side listen(%s:%d)...", m_pszOutSideListenIP, m_wOutSideListenPort );
//			if( !m_pIOCPServer->StartListen( OUTSIDE_IOHANDLER, m_pszOutSideListenIP, m_wOutSideListenPort ) )
//			{
//				DISP_FAIL;
//			}
//			else
//			{
//				DISP_OK;
//			}
//		}
//	}
//
//	if( BOTH_IOHANDLER == where || INSIDE_IOHANDLER == where )
//	{
//		if( m_wInSideListenPort && !m_pIOCPServer->IsListening( INSIDE_IOHANDLER ) )
//		{
//			DISPMSG( "[SolarAuthServer::StartListen]\n\tStarting In-side listen(%s:%d)...", m_pszInSideListenIP, m_wInSideListenPort );
//			if( !m_pIOCPServer->StartListen( INSIDE_IOHANDLER, m_pszInSideListenIP, m_wInSideListenPort ) )
//			{
//				DISP_FAIL;
//			}
//			else
//			{
//				DISP_OK;
//			}
//		}
//	}
//}

VOID SolarAuthServer::Connect()
{
	ASSERT( "Ȯ������!" );
	/*
	INT		ServerType			= UNKNOWN_SERVER;
	char	pszSectionName[16]	= {'\0',};
	char	pszConnectIP[32]	= {'\0',};
	WORD	wConnectPort		= 0;
	INT nMax = m_nConnectionNum;

	if( !m_bGetServerInfoFromFile)
	{
	while( 0 < nMax-- )
	{
	_snprintf( pszSectionName, 16, "CONNECTION_%d", nMax );

	__READ_CONFIG_STRING_IP( pszConnectIP, pszSectionName, "IP", NULL );
	__READ_CONFIG_NUMBER( wConnectPort, pszSectionName, "PORT", 0 );
	__READ_CONFIG_NUMBER( ServerType, pszSectionName, "TYPE", 0 );

	ServerSession * pSession = ServerSessionManager::Instance()->AllocServerSession( (eSERVER_TYPE)ServerType );
	ASSERT( pSession && "����Ÿ���� �߸��Ǿ����ϴ�.GuildServer.ini ����!" );

	if( ServerType == GAME_DBPROXY )
	{
	m_pDBProxyServerSession = pSession;
	}
	if( ServerType == MASTER_SERVER )
	{
	m_pMasterServerSession = pSession;
	}
	if( ServerType == WORLD_SERVER )
	{
	m_pWorldServerSession = pSession;
	}

	// Master Server �ƴϸ� ����
	if( ServerType != MASTER_SERVER )
	{
	continue;
	}

	ASSERT( wConnectPort != 0 && " ��Ʈ�� 0�Դϴ�.GuildServer.ini ����!" );

	m_pMasterServerSession->SetAddr( pszConnectIP, wConnectPort );
	Connect( m_pMasterServerSession, pszConnectIP, wConnectPort );

	}
	}
	else
	{
	while( 0 < nMax-- )
	{
	_snprintf( pszSectionName, 16, "CONNECTION_%d", nMax );

	__READ_CONFIG_STRING_IP( pszConnectIP, pszSectionName, "IP", NULL );
	__READ_CONFIG_NUMBER( wConnectPort, pszSectionName, "PORT", 0 );
	__READ_CONFIG_NUMBER( ServerType, pszSectionName, "TYPE", 0 );

	ASSERT( wConnectPort != 0 && " ��Ʈ�� 0�Դϴ�.GuildServer.ini ����!" );

	// Master Server ���� ����!
	if( ServerType == MASTER_SERVER )
	{
	if(m_bGetServerInfoFromFile) continue;
	}

	ServerSession * pSession = ServerSessionManager::Instance()->AllocServerSession( (eSERVER_TYPE)ServerType );
	ASSERT( pSession && "����Ÿ���� �߸��Ǿ����ϴ�.GuildServer.ini ����!" );

	pSession->SetAddr( pszConnectIP, wConnectPort );
	Connect( pSession, pszConnectIP, wConnectPort );

	// TO DO : 

	if( ServerType == GAME_DBPROXY )
	{
	m_pDBProxyServerSession = pSession;
	}
	if( ServerType == MASTER_SERVER )
	{
	m_pMasterServerSession = pSession;
	}
	if( ServerType == WORLD_SERVER )
	{
	m_pWorldServerSession = pSession;
	}
	}
	}
	*/
}

VOID SolarAuthServer::Connect( const ServerSession* pNetObject, CHAR* IN pszIP, const WORD wPort )
{
	ASSERT( "Ȯ������!" );
	if( GetIOCPServer() && pNetObject )
	{
		GetIOCPServer()->Connect( INSIDE_IOHANDLER, (NetworkObject*)pNetObject, pszIP, wPort);
	}
}


//============================================================================================================
// Logger Initializer
//

BOOL	SolarAuthServer::InitializeLogger()
{
	BYTE bySunLogOption = 0;
	BYTE bySunLogFileLevel = 0;
	CHAR pszSunLogFilePath[32];

	__READ_CONFIG_NUMBER( bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, SOLAR_AUTH_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_NUMBER( bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, SOLAR_AUTH_SERVER_INFO_FILE_NAME );
	__READ_CONFIG_STRING( pszSunLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", NULL, SOLAR_AUTH_SERVER_INFO_FILE_NAME );

	DISPMSG( "SUNLOG Initializing...\t" );

	g_pSunLog = new CSunLog;
	BOOL bRet = g_pSunLog->Init( bySunLogOption, bySunLogFileLevel, pszSunLogFilePath, "SolarAuth" );

	if( bRet )
	{
		DISP_OK;
	}
	else
	{
		DISP_FAIL;
	}

	return bRet;
}

