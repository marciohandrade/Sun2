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
	char		abPath[MAX_PATH];				//  ���μ����� ���� ���
	char		fileName[100];					//  ���̳ʸ� �̹��� �̸�
	char		serviceName[20];				//  ���� ��ϸ�(isService == true�� ��쿡�� ��ȿ)
	bool		isService;						//  true : Service App

	bool		isActive;						//  ���� ������������ �˻��ϴ� flag
} MONITOR_PROCESS_INFO;

typedef struct
{
	char		abPath[MAX_PATH];				//  ���μ����� ���� ���
	char		fileName[100];					//  ���̳ʸ� �̹��� �̸�
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