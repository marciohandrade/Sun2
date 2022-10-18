/*------------------------------------------------------------------------------
 * LogCriticalSection.cpp
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "LogCriticalSection.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogCriticalSection::LogCriticalSection()
	:	m_initialized(FALSE)
{
	Init();
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogCriticalSection::~LogCriticalSection()
{
	if (TRUE == m_initialized)
	{
		DeleteCriticalSection(&m_CriticalSection);
		m_initialized = FALSE;
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogCriticalSection::Init()
{
	if (TRUE == m_initialized)
	{
		return 0;
	}

	__try
	{
		InitializeCriticalSection(&m_CriticalSection);
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		_ASSERTE(!"InitializeCriticalSection");
		return -1;
	}

	m_initialized = TRUE;
	return 0;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogCriticalSection::Enter()
{
	_ASSERTE(TRUE == m_initialized);
	if (FALSE == m_initialized)
	{
		_ASSERTE(!"not initialized");
		return -1;
	}

	__try
	{
		::EnterCriticalSection(&m_CriticalSection); 
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		_ASSERTE(!"EnterCriticalSection");
		return -1;
	}
	return 0;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogCriticalSection::Leave()
{
	_ASSERTE(TRUE == m_initialized);
	if (FALSE == m_initialized)
	{
		_ASSERTE(!"not initialized");
		return -1;
	}

	::LeaveCriticalSection(&m_CriticalSection);		
	return 0;
}