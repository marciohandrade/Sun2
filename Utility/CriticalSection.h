#ifndef __CRITICALSECTION_H__
#define __CRITICALSECTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 
#endif
#include <windows.h>
#include "CommonDef.h"

namespace util
{
	
class CCriticalSection 
{
public:
	CCriticalSection( DWORD dwSpinCount = 0 );
	virtual ~CCriticalSection();

	void Lock();
	void Unlock(void);
	
private:
	CRITICAL_SECTION m_cs;
	
};


class CSyncCriticalSection
{
public:
	CSyncCriticalSection( CCriticalSection & cs );
	virtual ~CSyncCriticalSection();

	void Lock();
	void Unlock(void);
	
private:
	BOOL m_bLocked;
	CCriticalSection & m_cs;
	
	__DISABLE_COPY(CSyncCriticalSection);
};



}//End of namespace util



#endif // __CRITICALSECTION_H__
