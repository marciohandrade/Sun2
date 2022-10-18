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

			char						m_serverIPAddr[20];
			DWORD						m_serverPortNo;

protected:
	virtual	void						Run();

private:
			bool						LoadServerInfo();


			eXtreme::ServerNetwork		m_networkRTTS;

			ManagerRTTS					m_managerRTTS;
			HANDLE						m_eventKill;
};