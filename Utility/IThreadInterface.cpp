#include "UtilityCommon.h"
#include <windows.h>
#include ".\ithreadinterface.h"
#include <process.h>
#include <assert.h>

namespace util 
{


unsigned int __stdcall IThreadInterface::m_fnThread( void * pt )
{
	IThreadInterface * pthis = (IThreadInterface *)pt;

	DWORD rt = pthis->run();

	_endthread();

	return rt;
}

IThreadInterface::IThreadInterface()
	:	m_hThread ( INVALID_HANDLE_VALUE )
{

}

IThreadInterface::~IThreadInterface()
{
	if( INVALID_HANDLE_VALUE != m_hThread )
	{
		CloseHandle( m_hThread );
		m_hThread = INVALID_HANDLE_VALUE;
	}

}

VOID IThreadInterface::StartThread( BOOL bSuspend )
{
	assert( INVALID_HANDLE_VALUE == m_hThread );
	if( INVALID_HANDLE_VALUE == m_hThread )
	{
		unsigned threadId = 0;
		m_hThread = (HANDLE)_beginthreadex( NULL, 0, m_fnThread, (void *)this, ( bSuspend ? CREATE_SUSPENDED : 0 ), &threadId );
	}
}

VOID IThreadInterface::SuspendThread()
{
	::SuspendThread( m_hThread );
}

VOID IThreadInterface::ResumeThread()
{
	::ResumeThread( m_hThread );
}

}