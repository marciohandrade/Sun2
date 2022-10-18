#pragma once

#include <eXtreme/net/ServerNetwork.h>

#include "ManagerRTTS.h"

class Entry
{
	Singleton( Entry );

public:
	static	bool						OnStartService();
	static	void						OnStopService();

			void						Join();

private:
			eXtreme::ServerNetwork		m_networkRTTA;
			ManagerRTTS					m_managerRTTA;
};