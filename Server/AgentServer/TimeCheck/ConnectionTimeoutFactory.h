#ifndef __CONNECTION_TIMEOUT_FACTORY_H__
#define __CONNECTION_TIMEOUT_FACTORY_H__

#pragma once

#include <MemoryPoolFactory.h>

class ConnectionTimeout;
class ConnectionTimeoutFactory
{
public:
	ConnectionTimeoutFactory();
	virtual ~ConnectionTimeoutFactory();

	VOID										InitPool( DWORD dwPoolNum );
	VOID										ReleasePool();
	VOID										SetConnectionTimeoutObjectNum( DWORD dwPoolNum );
	ConnectionTimeout *							AllocConnection();
	VOID										FreeConnection( ConnectionTimeout * p );
	
private:
	// pool
	util::CMemoryPoolFactory<ConnectionTimeout> m_ConnectionPool;
};

extern ConnectionTimeoutFactory g_ConnTimeoutFactory;
#endif // __CONNECTION_TIMEOUT_FACTORY_H__