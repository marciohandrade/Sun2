#pragma once

#include <eXtreme/net/ServerNetwork.h>

#include "SessionMngStub.h"
#include "SessionMngSkel.h"

class Entry
{
	Singleton( Entry );

public:
			void						Destroy();
			void						Join();

private:
			eXtreme::ServerNetwork		m_networkSkel;
			eXtreme::ServerNetwork		m_networkStub;

			SessionMngSkel				m_managerSkel;
			SessionMngStub				m_managerStub;
};