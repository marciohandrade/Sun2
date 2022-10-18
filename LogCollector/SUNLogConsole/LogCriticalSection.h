/*------------------------------------------------------------------------------
 * LogCriticalSection.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _LOG_CRITICALSECTION_H
#define _LOG_CRITICALSECTION_H

class LogCriticalSection
{
public:
	LogCriticalSection();
	~LogCriticalSection();

	int Enter();
	int Leave();

private:
	BOOL			 m_initialized;
	CRITICAL_SECTION m_CriticalSection;
	int Init();
};

#endif //_LOG_CRITICALSECTION_H
