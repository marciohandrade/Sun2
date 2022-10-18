#ifndef __SERVER_SESSION_FACTORY_H__
#define __SERVER_SESSION_FACTORY_H__

#pragma once

#include <MemoryPoolFactory.h>
#include <Singleton.h>

#include "ServerSessionManager.h"

using namespace util;

class TempServerSession;
class MasterServerSession;
class ServerSession;



class ServerSessionFactory : public Singleton<ServerSessionFactory>
{
public:
	ServerSessionFactory();
	~ServerSessionFactory();

	SERVER_SESSION_DELEGATOR			AllocServerSession( eSERVER_TYPE eServerType );
	BOOL								FreeServerSession( SERVER_SESSION_DELEGATOR pServerSession );


private:

	CMemoryPoolFactory<TempServerSession>*			m_pTempServerPool;
	CMemoryPoolFactory<MasterServerSession>*		m_pMasterServerPool;
};


#endif // __SERVER_SESSION_FACTORY_H__