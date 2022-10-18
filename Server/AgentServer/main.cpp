#include "stdafx.h"

#include <Stackwalker.h>
#include <SolarSimpleConsole.h>
#include <SolarDump.h>

#include <AgentServer.h>
#include <UserSessions/UserManager.h>

//==================================================================================================
// @desc �޸� ���� ������ ��� ����
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Agent.txt"
// @desc ������ �޸� ���� ��� ���ϸ�
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "./memory_leak_Agent_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================


//===============================================================================================
// <FUNCTION><PROTOTYPEs>
extern VOID CallBackCommand( CHAR* szMessage );
VOID		ErrorMsg_n_Pause( CHAR* errmsg );
static VOID _Init_Routine();

extern "C"
{ 
	int WinMainCRTStartup();
}

//===============================================================================================
// <MODULE><VARIABLEs>
static SolarSimpleConsole*	s_pConsole = NULL;

//===============================================================================================
// <ENTRY POINT>
int SunEntryPoint()
{
//    SharedMemoryPool::CreateInstance();
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
	WaverixTestEntry();
#endif
	return WinMainCRTStartup();
}

#ifdef _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY
static SolarDump	solarDUMP(".", MiniDumpWithFullMemory);
#else  // _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY
static SolarDump	solarDUMP(".");
#endif // _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY

// <ENTRY POINT><2nd>
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
    __UNUSED((hInstance, hPrevInstance, lpCmdLine, nCmdShow));
	//CoInitializeEx( NULL, COINIT_MULTITHREADED );
	//CoInitialize( NULL );

	StartMemoryLeakCheck();
	BreakPointOnMemoryLeak(63);

#ifdef _DEBUG
	//	InitAllocCheck();//Memory Leak Check Start
#endif

	_Init_Routine();

	// ��� �μ� �Ľ�
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
		ErrorMsg_n_Pause( "Invalid Argument!\n" );

	DISPMSG(	\
		"=============================================================================\n"	\
		"                    Agent Server Initialization Start!\n"	\
		"=============================================================================\n");
    WAVERIX_WRAPPER((WAVERIX_LC_CALL0("waverix_log_fasttest")));
	// ���� �ʱ�ȭ
    AgentServer* agent_server = AgentServer::CreateInstance();
	if( !agent_server->DoInit(
							AGENT_SERVER_INFO_FILE_NAME,
							AgentServer::CreateServerSideAcceptedObject,
							AgentServer::DestroyServerSideAcceptedObject,
							AgentServer::DestroyServerSideConnectedObject,
							AgentServer::CreateClientSideAcceptedObject,
							AgentServer::DestroyClientSideAcceptedObject,
							AgentServer::DestroyClientSideConnectedObject
							/*NULL,
							NULL,
							NULL*/
							) )
		ErrorMsg_n_Pause( "[WinMain] Server Initialization Failed!\n" );

	MessageOut( eFULL_LOG, "============================================================================= " );
	MessageOut( eFULL_LOG, "              SUN Online Agent Server Initialization Complete." );
	MessageOut( eFULL_LOG, "============================================================================= " );
	//
	// <RUNNING>
	agent_server->DoProcess();
	agent_server->DoRelease();
	agent_server->DoFinalize();
	// ServerFrame�� ����ϸ鼭 ���乮�� �ذ� ����
	// Release������ ���ο� Instance �� �����ϴ� �͵��� ���� ���⼭ �����ϱ�� �Ѵ�.
	//s_pAgentServer->Finalize();
    AgentServer::DestroyInstance();

	// ���� ����
	DISPMSG( "[WinMain] Server is Terminated...\n" );

	// �ܼ�â ����
	s_pConsole->Release();
	SAFE_DELETE(s_pConsole);

#ifdef _DEBUG
	//DeInitAllocCheck();//Memory Leak Check End
#endif

	return 0;
}


static VOID		ErrorHandler_new()
{
	printf( "Can't Alloc Memory\n" );
	if( g_pSunLog )
	{
		MessageOut( eCRITICAL_LOG, _T("Can't Alloc Memory\n") );
		g_pSunLog->Flush( CSunLog::cOUTPUT_BOTH );
	}

	__debugbreak();
}

#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
#include <signal.h>
VOID		signal_redirect( INT signo )
{
	__debugbreak();
}
#endif


static VOID _Init_Routine()
{
#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
	signal( SIGABRT, signal_redirect );
	_set_error_mode( _OUT_TO_STDERR );
#endif
	std::set_new_handler( ErrorHandler_new );

	// �ܼ�â �ʱ�ȭ
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= TRUE;
	cdesc.bUsingRedirect		= TRUE;
	s_pConsole = new SolarSimpleConsole;
	s_pConsole->Init( &cdesc );
}

VOID		ErrorMsg_n_Pause( CHAR* errmsg )
{
	DISPMSG( errmsg );
	system( "pause" );
	__debugbreak();
}




