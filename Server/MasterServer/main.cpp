#include "stdafx.h"

#include <SolarSimpleConsole.h>
#include <SolarDump.h>

#include <Sessions/ServerSessionEx.h>
#include <Sessions/ServerSessionManager.h>

#include "MasterServerEx.h"

//==================================================================================================
// @desc �޸� ���� ������ ��� ����
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Master.txt"
// @desc ������ �޸� ���� ��� ���ϸ�
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

	// �ܼ�â �ʱ�ȭ
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	// ��� �μ� �Ľ�
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

	// ���� �ʱ�ȭ
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
	// ServerFrame�� ����ϸ鼭 ���乮�� �ذ� ����
	// Release������ ���ο� Instance �� �����ϴ� �͵��� ���� ���⼭ �����ϱ�� �Ѵ�.
	s_pMasterServerEx->Finalize();
	SAFE_DELETE(s_pMasterServerEx);

	// ���� ����
	DISPMSG( "[WinMain] Server is terminated...\n" );

	// �ܼ�â ����
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
		MessageOut( eCRITICAL_LOG,  "[ServerSessionFactory::AllocServerSession] �߸��� ���� Ÿ���̴�. Ǯ���� �Ҵ��� �� �����ϴ�." );
	}

	return pSession;
}

VOID			MasterServerEx::Handler_FreeServerSession_NoRelease( NetworkObject* pNetworkObject )
{
	if( !pNetworkObject )
	{
		MessageOut( eCRITICAL_LOG, "NULL�� ServerSession �����Ϸ��� �Ѵ�." );
		return;
	}

	ServerSession* pServerSession = (ServerSession*)pNetworkObject;

	if( !ServerSessionManager::FreeSession( pServerSession ) )
	{
		MessageOut( eFULL_LOG,  "[ServerSessionFactory::FreeServerSession] �߸��� Ÿ���� ���������� Free�� �� ����" );
	}
}

///------------------------------------------------------------------------------------------------
/// ������ �ݹ�
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




