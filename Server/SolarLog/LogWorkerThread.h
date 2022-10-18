#ifndef __LOG_WORKER_THREAD_H__
#define __LOG_WORKER_THREAD_H__


#pragma once

//=======================================================================================================================
/// Overlapped ������� Log�� File Write�ϴ� �α� ��Ŀ Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 11 ( ������ Day ^^ )
	@remarks
			- Overlapped File I/O
			- Buffer Switch�� lock�Ǿ� ���ٸ��� �ð��� �ּ�ȭ
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