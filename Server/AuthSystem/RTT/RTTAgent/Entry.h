#pragma once

#include <eXtreme/net/ServerNetwork.h>

#include "ManagerRTTS.h"

class Entry : public eXtreme::Thread
{
	Singleton( Entry );

public:
	static	bool						OnServiceStart();
	static	void						OnServiceStop();


			bool						Initialize();
			void						Destroy();

			void						ServiceStartUp();
			void						ServiceTerminate();


			char						m_agentIPAddr[20], m_serverIPAddr[20];
			DWORD						m_agentPortNo, m_serverPortNo;

			DWORD						GetVersion(){ return m_dwVersion; };
			void						SetVersion(DWORD dwVersion){ m_dwVersion = dwVersion; };
			
protected:
	virtual	void						Run();

private:
	static	void						ProcCrashHandling( LPCTSTR stackWalkLog, LPCTSTR dumpFileName );
	static	LPCTSTR						GenerateDmpName();


			bool						LoadServerInfo();


			eXtreme::ServerNetwork		m_networkSkel;
			eXtreme::ServerNetwork		m_networkRTTS;

			ManagerRTTS					m_managerRTTS;
			bool						m_serviceRun;
			HANDLE						m_eventKill;

			DWORD						m_dwVersion;
};