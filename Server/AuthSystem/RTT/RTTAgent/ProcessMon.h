#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>

#pragma warning(pop)

#include <eXtreme/threading/Thread.h>
#include <eXtreme/threading/Mutex.h>


#include "enumproc.h"

typedef struct
{
	char		abPath[MAX_PATH];				//  프로세스의 절대 경로
	char		fileName[100];					//  바이너리 이미지 이름
	char		serviceName[20];				//  서비스 등록명(isService == true일 경우에만 유효)
	bool		isService;						//  true : Service App

	bool		isActive;						//  현재 동작중인지를 검사하는 flag
} MONITOR_PROCESS_INFO;

typedef struct
{
	char		abPath[MAX_PATH];				//  프로세스의 절대 경로
	char		fileName[100];					//  바이너리 이미지 이름
} CURRENT_PROCESS_INFO;

class ProcessMon : public eXtreme::Thread
{
	Singleton( ProcessMon );

public:
			void			StartMonitor();
			void			Destroy();


			void			Clear();
			void			AddMonitorProcess( char* abPath, char* fileName, char* serviceName, bool isService );
			bool			CreateAllMonitorProcess();
			bool			CreateMonitorProcess( char* abPath, char* fileName );
			bool			KillMonitorProcess( char* abPath, char* fileName );

			bool			IsRunningProcess( char* abPath, char* fileName );
			void			SetPatchKey( DWORD value )	{	m_patchKey = value;		}
			DWORD			GetPatchKey()				{	return m_patchKey;		}


protected:
	virtual	void			Run();

private:
			void			CheckActiveProcess();
			

			void			SendProcessStatus();


	typedef	std::list		< MONITOR_PROCESS_INFO* >		ListMonProcess;

			ListMonProcess			m_listMonProcess;
			HANDLE					m_eventKill;
			DWORD					m_patchKey;

			
			std::vector		<RUNNING_PROC_INFO>		m_vecProcess;
			eXtreme::Mutex			m_mutexProcess;
};