#ifndef __LOG_WORKER_THREAD_H__
#define __LOG_WORKER_THREAD_H__


#pragma once

//=======================================================================================================================
/// Overlapped 방식으로 Log를 File Write하는 로그 워커 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 11 ( 빼빼로 Day ^^ )
	@remarks
			- Overlapped File I/O
			- Buffer Switch로 lock되어 가다리는 시간을 최소화
*/
//=======================================================================================================================

#include <IThreadInterface.h>

class SolarLog;

class LogWorkerThread : public util::IThreadInterface
{
public:
	LogWorkerThread( SolarLog & sl );
	~LogWorkerThread();


	virtual VOID					EndThread();


protected:
	virtual DWORD					run();
	VOID							_OPERATE();
	BOOL							_SetEndOfFile();

private:
	HANDLE							m_hEndThread;
	SolarLog &						m_solarlog;
	OVERLAPPED						m_ovl;
	ULARGE_INTEGER					m_readPointer;

	__DISABLE_COPY(LogWorkerThread);
};


#endif // __LOG_WORKER_THREAD_H__