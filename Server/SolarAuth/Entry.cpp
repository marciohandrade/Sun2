//=============================================================================================================================
/// SolarAuthServer Main Entry
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

#include "stdafx.h"

#include <SolarSimpleConsole.h>
#include <Stackwalker.h>

#include <ISolarConsole_Define.h>
#include <ISolarConsole_Guid.h>
#include <ISolarConsole.h>
#include <SUNConsole.h>

#include <SolarDump.h>
#include <ServiceApp.h>
#include <FileParser.h>

//============================================================
//
#include "SolarAuthServer.h"
#include "Sessions/ServerSession.h"
#include "Sessions/ServerSessionManager.h"
//
//============================================================
// MACROs
//#define __DEBUG_CHECK_ALLOCCHECK

//============================================================
// Module Varialbes
static SolarAuthServer*		s_pSolarAuthServer	= NULL;

// <초기화 하는데 사용되는 함수 집합들 -임시 모음->
#include "Entry.SubFunc.cxx"
//
//============================================================

//
// <프로그램 시작점>
//
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	SolarDump sSolarDump;

	StartMemoryLeakCheck();
	//BreakPointOnMemoryLeak(3228);
	//BreakPointOnMemoryLeak(2509);
#if defined(_DEBUG) && defined(__DEBUG_CHECK_ALLOCCHECK)
	InitAllocCheck();//Memory Leak Check Start
#endif

	if( !InitRuntimeEnvionment() )
		return 0;

	DISPMSG(	\
		"============================================================================="	\
		"                 SolarAuthServer Initialization Start!\n" \
		"=============================================================================\n");

	// 서버 초기화
	s_pSolarAuthServer = new SolarAuthServer();
	if( !s_pSolarAuthServer->DoInit(
								"./SolarAuthServer.ini",
								SolarAuthServer::CreateServerSideAcceptedObject,
								SolarAuthServer::DestroyServerSideAcceptedObject,
								SolarAuthServer::DestroyServerSideConnectedObject,
								NULL,
								NULL,
								NULL
								) )
	{
		DISPMSG( "[WinMain] Server Initialization Failed!\n" );
		system( "pause" );
		return 0;
	}

	SUNLOG( eFULL_LOG,
		"============================================================================="	\
		"           SUN Online SolarAuth Server Initialization Complete.\n" \
		"=============================================================================\n" );

	//
	// <RUNNING>
	switch( s_pSolarAuthServer->GetServiceMode() )
	{
	case eCONSOLE_MODE:
		{
			s_pSolarAuthServer->DoProcess();
			s_pSolarAuthServer->DoRelease();
			// ServerFrame을 상속하면서 생긴문제 해결 목적
			// Release이후의 새로운 Instance 를 생성하는 것들은 따로 여기서 해제하기로 한다.
			s_pSolarAuthServer->Finalize();
			SAFE_DELETE(s_pSolarAuthServer);

			// 서버 종료
			DISPMSG( "[WinMain] Server is terminated...\n" );

			g_SimpleConsole.Release();
		}
		break;

	case eSERVICE_MODE:
		{
			DISPMSG( "[WinMain] <<<<< Service Mode >>>>>\n" );
			//__START_SERVICE( SolarAuthServiceApp );
		}
		break;
	}

#if defined(_DEBUG) && defined(__DEBUG_CHECK_ALLOCCHECK)
	DeInitAllocCheck();//Memory Leak Check End
#endif

	return 0;
}

SolarAuthServer*	SolarAuthServer::GetMainFrame()
{
	return s_pSolarAuthServer;
}

ServerSession*	SolarAuthServer::Handler_AllocServerSession( eSERVER_TYPE serverType )
{
	ServerSession* pServerSession = (ServerSession*)Handler_AllocServerSession_NoInit( serverType );
	if( pServerSession )
	{
		pServerSession->Init( s_pSolarAuthServer );
	}

	return pServerSession;
}

VOID			SolarAuthServer::Handler_FreeServerSession( ServerSession* pServerSession )
{
	if( pServerSession )
	{
		pServerSession->Release();
		Handler_FreeServerSession_NoRelease( pServerSession );
	}
}

NetworkObject*	SolarAuthServer::Handler_AllocServerSession_NoInit( eSERVER_TYPE serverType )
{
	NetworkObject* pSession = NULL;

	if( TEMP_SERVER == serverType )
		pSession = (NetworkObject*)s_pSolarAuthServer->m_TempServerPool.Alloc();
	else if( MASTER_SERVER == serverType )
		pSession = (NetworkObject*)s_pSolarAuthServer->m_MasterServerPool.Alloc();
	else if( ACCOUNT_DBPROXY == serverType )
		pSession = (NetworkObject*)s_pSolarAuthServer->m_ADBPServerPool.Alloc();
	else
	{
		SUNLOG( eFULL_LOG,  "[ServerSessionFactory::AllocServerSession] 잘못된 서버 타입이다. 풀에서 할당할 수 없습니다." );
	}

	return pSession;
}

VOID			SolarAuthServer::Handler_FreeServerSession_NoRelease( NetworkObject* pNetworkObject )
{
	if( !pNetworkObject )
	{
		SUNLOG( eCRITICAL_LOG, "NULL로 ServerSession 해제하려고 한다." );
		return;
	}

	ServerSession* pServerSession = (ServerSessionEx*)pNetworkObject;
	eSERVER_TYPE serverType = pServerSession->GetServerType();

	if( TEMP_SERVER == serverType )
		s_pSolarAuthServer->m_TempServerPool.Free( static_cast<TempServerSessionEx*>(pServerSession) );
	else if( MASTER_SERVER == serverType )
		s_pSolarAuthServer->m_MasterServerPool.Free( static_cast<MasterServerSession*>(pServerSession) );
	else if( ACCOUNT_DBPROXY == serverType )
		s_pSolarAuthServer->m_ADBPServerPool.Free( static_cast<AccountDBProxySession*>(pServerSession) );
	else
	{
		SUNLOG( eFULL_LOG,  "[ServerSessionFactory::FreeServerSession] 잘못된 타입의 서버세션을 Free할 수 없다" );
	}
}

///------------------------------------------------------------------------------------------------
/// 서버쪽 콜백
NetworkObject*	SolarAuthServer::CreateServerSideAcceptedObject()
{
	SERVER_SESSION_DELEGATOR pServerSession =
		(ServerSessionEx*)SolarAuthServer::Handler_AllocServerSession_NoInit( TEMP_SERVER );

	((ServerSession*)pServerSession())->Init( s_pSolarAuthServer );

	return (NetworkObject*)pServerSession();
}

VOID			SolarAuthServer::DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject )
{
	SolarAuthServer::Handler_FreeServerSession_NoRelease( pNetworkObject );
}

VOID			SolarAuthServer::DestroyServerSideConnectedObject( NetworkObject* pNetworkObject )
{
	__UNUSED(pNetworkObject);	
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//
// SolarAuthServiceApp Implementation
//

//#ifdef _DEBUG
//__DECLARE_SERVICE( SolarAuthServiceApp, "AgentServer_d" );
//#else
//__DECLARE_SERVICE( SolarAuthServiceApp, "AgentServer" );
//#endif

DWORD		SolarAuthServiceApp::OnInit ( DWORD argc, LPTSTR* argv, DWORD* specificError )
{
	__UNUSED(argc);
	__UNUSED(argv);
	__UNUSED(specificError);
	return 0; 
}

VOID		SolarAuthServiceApp::Update()
{
	s_pSolarAuthServer->DoProcess();
	s_pSolarAuthServer->DoRelease();
	SAFE_DELETE( s_pSolarAuthServer );
	g_SimpleConsole.Release();
}

VOID		SolarAuthServiceApp::OnStop()
{
	//s_pSolarAuthServer->Shutdown();
}

VOID		SolarAuthServiceApp::OnShutDown()
{
	printf("SHUT DOWN");
}

