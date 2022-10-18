#include "UtilityCommon.h"
#include "CriticalSection.h"

namespace util
{

CCriticalSection::CCriticalSection( DWORD dwSpinCount )
{
	if( 0 != dwSpinCount )
		::InitializeCriticalSectionAndSpinCount( &m_cs, dwSpinCount );
	else
		::InitializeCriticalSection( &m_cs );
}

CCriticalSection::~CCriticalSection()
{
	::DeleteCriticalSection( &m_cs );
}

void CCriticalSection::Lock()
{
	::EnterCriticalSection( &m_cs );
}

void CCriticalSection::Unlock(void)
{
	::LeaveCriticalSection( &m_cs );
}



CSyncCriticalSection::CSyncCriticalSection( CCriticalSection & cs) : m_cs( cs )
{
	Lock();
}

CSyncCriticalSection::~CSyncCriticalSection()
{
	Unlock();
}

void CSyncCriticalSection::Lock()
{
	m_cs.Lock();
	m_bLocked = TRUE;
}

void CSyncCriticalSection::Unlock(void)
{
	if( m_bLocked )
	{
		m_bLocked = FALSE;
		m_cs.Unlock();
	}
}

}