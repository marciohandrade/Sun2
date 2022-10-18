#pragma once

#include <ithreadinterface.h>

class SolarClientNetwork;
class ConnectionAllocator;

class ProcessThread : public util::IThreadInterface
{
public:
	ProcessThread( SolarClientNetwork & net, ConnectionAllocator * pool, HANDLE * phSend, HANDLE * phRecv );
	virtual ~ProcessThread();

	virtual VOID					EndThread();
protected:
	virtual DWORD					run();
	SolarClientNetwork &			m_net;
	HANDLE							m_hEndThreadEvent;
	ConnectionAllocator *			m_pConnectionPool;
	HANDLE *						m_phSendEvent;
	HANDLE *						m_phRecvEvent;
};
