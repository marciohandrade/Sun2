#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/threading/Thread.h>


using namespace eXtreme;

Thread::Thread()
	: m_threadID( 0 ), m_handleThread( INVALID_HANDLE_VALUE )
{
}

Thread::~Thread()
{
	if(m_handleThread!=INVALID_HANDLE_VALUE){
		::CloseHandle(m_handleThread);
	}
}

bool Thread::Start()
{
	if( m_handleThread != INVALID_HANDLE_VALUE )
	{
		::CloseHandle(m_handleThread);
		m_handleThread = INVALID_HANDLE_VALUE;
	}

	//m_handleThread = ::CreateThread( 0, 0, Runner, ( LPVOID )this, 0, &m_threadID );
	unsigned int uiThreadID = 0;
	m_handleThread = (HANDLE)_beginthreadex(0, 0, Runner, (LPVOID)this, 0, &uiThreadID);

	m_threadID = uiThreadID;

	if( m_handleThread == 0 )
	{
		return false;
	}

	return true;
}

bool Thread::Terminate( DWORD exitCode )
{
	if( m_handleThread == INVALID_HANDLE_VALUE )
		return true;

	bool ret				= ( ::TerminateThread( m_handleThread, exitCode ) == TRUE );
	m_handleThread	= INVALID_HANDLE_VALUE;

	return ret;
}

bool Thread::WaitForTerminate( DWORD timeout )
{
	if( m_handleThread == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	return ( ::WaitForSingleObject( m_handleThread, timeout ) == WAIT_OBJECT_0 );
}

unsigned int __stdcall Thread::Runner( LPVOID parameter )
{
	Thread*		instance = ( Thread* )parameter;

	instance->Run();

    _endthreadex(0);
	return 0;
}