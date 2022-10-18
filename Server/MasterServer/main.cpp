#include "stdafx.h"

#include <SolarSimpleConsole.h>
#include <SolarDump.h>

#include <Sessions/ServerSessionEx.h>
#include <Sessions/ServerSessionManager.h>

#include "MasterServerEx.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Master.txt"
// @desc 공유된 메모리 누수 출력 파일명
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "./memory_leak_Master_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================

//
//
//============================================================
//
extern "C"
{
	int WinMainCRTStartup();
}

int SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
	return WinMainCRTStartup();
}

//============================================================
// Module Varialbes
static MasterServerEx*		s_pMasterServerEx	= NULL;

// Global
SolarSimpleConsole	g_Console;

MasterServerEx*		MasterServerEx::GetMainFrame()
{
	return s_pMasterServerEx;
}


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	SolarDump dp;

	// 콘솔창 초기화
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	// 명령 인수 파싱
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
		return 0;
	}

	DISPMSG(	\
		"============================================================================="	\
		"                    Master Server Initialization Start!\n" \
		"=============================================================================\n");

	// 서버 초기화
	s_pMasterServerEx = new MasterServerEx();
	if( !s_pMasterServerEx->DoInit(
								"./MasterServer.ini",
								MasterServerEx::CreateServerSideAcceptedObject,
								MasterServerEx::DestroyServerSideAcceptedObject,
								MasterServerEx::DestroyServerSideConnectedObject,
								MasterServerEx::CreateServerSideAcceptedObject,
								MasterServerEx::DestroyServerSideAcceptedObject,
								MasterServerEx::DestroyServerSideConnectedObject
								/*NULL,
								NULL,
								NULL*/
								) )
	{
		DISPMSG( "[WinMain] Server Initialization Failed!\n" );
		system( "pause" );
		return 0;
	}

	MessageOut( eFULL_LOG, "============================================================================= " );
	MessageOut( eFULL_LOG, "              SUN Online Master Server Initialization Complete." );
	MessageOut( eFULL_LOG, "============================================================================= " );

	//
	// <RUNNING>
	s_pMasterServerEx->DoProcess();
	s_pMasterServerEx->DoRelease();
	// ServerFrame을 상속하면서 생긴문제 해결 목적
	// Release이후의 새로운 Instance 를 생성하는 것들은 따로 여기서 해제하기로 한다.
	s_pMasterServerEx->Finalize();
	SAFE_DELETE(s_pMasterServerEx);

	// 서버 종료
	DISPMSG( "[WinMain] Server is terminated...\n" );

	// 콘솔창 종료
	g_Console.Release();

	return 0;
}

//
//
//

ServerSession*	MasterServerEx::Handler_AllocServerSession( eSERVER_TYPE serverType )
{
	ServerSession* pServerSession = (ServerSession*)Handler_AllocServerSession_NoInit( serverType );
	if( pServerSession )
	{
		pServerSession->Init( s_pMasterServerEx );
	}

	return pServerSession;
}

VOID			MasterServerEx::Handler_FreeServerSession( ServerSession* pServerSession )
{
	if( pServerSession )
	{
		pServerSession->Release();
		Handler_FreeServerSession_NoRelease( pServerSession );
	}
}

NetworkObject*	MasterServerEx::Handler_AllocServerSession_NoInit( eSERVER_TYPE serverType )
{
	NetworkObject* pSession =
		(NetworkObject*)ServerSessionManager::AllocNewSession( serverType );

	if( !pSession )
	{
		MessageOut( eCRITICAL_LOG,  "[ServerSessionFactory::AllocServerSession] 잘못된 서버 타입이다. 풀에서 할당할 수 없습니다." );
	}

	return pSession;
}

VOID			MasterServerEx::Handler_FreeServerSession_NoRelease( NetworkObject* pNetworkObject )
{
	if( !pNetworkObject )
	{
		MessageOut( eCRITICAL_LOG, "NULL로 ServerSession 해제하려고 한다." );
		return;
	}

	ServerSession* pServerSession = (ServerSession*)pNetworkObject;

	if( !ServerSessionManager::FreeSession( pServerSession ) )
	{
		MessageOut( eFULL_LOG,  "[ServerSessionFactory::FreeServerSession] 잘못된 타입의 서버세션을 Free할 수 없다" );
	}
}

///------------------------------------------------------------------------------------------------
/// 서버쪽 콜백
NetworkObject*	MasterServerEx::CreateServerSideAcceptedObject()
{
	ServerSessionEx* pServerSession =
		(ServerSessionEx*)MasterServerEx::Handler_AllocServerSession_NoInit( TEMP_SERVER );

	((ServerSession*)pServerSession)->Init( s_pMasterServerEx );

	return (NetworkObject*)pServerSession;
}

VOID			MasterServerEx::DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject )
{
	MasterServerEx::Handler_FreeServerSession( (ServerSession*)pNetworkObject );
}

VOID			MasterServerEx::DestroyServerSideConnectedObject( NetworkObject* pNetworkObject )
{
	__UNUSED(pNetworkObject);	
}




