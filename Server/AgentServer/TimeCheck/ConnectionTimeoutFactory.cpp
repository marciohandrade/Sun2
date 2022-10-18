#include "stdafx.h"
#include ".\connectiontimeoutfactory.h"
#include ".\ConnectionTimeout.h"

ConnectionTimeoutFactory g_ConnTimeoutFactory;

ConnectionTimeoutFactory::ConnectionTimeoutFactory(void)
{
}

ConnectionTimeoutFactory::~ConnectionTimeoutFactory(void)
{
}
VOID ConnectionTimeoutFactory::InitPool( DWORD dwPoolNum )
{
	m_ConnectionPool.Initialize( dwPoolNum, 0, "ConnectionTimeout", AssertionLogger4Pool_Server );
}
VOID ConnectionTimeoutFactory::ReleasePool()
{
	m_ConnectionPool.Release();
}

ConnectionTimeout * ConnectionTimeoutFactory::AllocConnection()
{ 
	return (ConnectionTimeout*)m_ConnectionPool.Alloc(); 
}
VOID ConnectionTimeoutFactory::FreeConnection( ConnectionTimeout * p )
{ 
	m_ConnectionPool.Free( p ); 
}