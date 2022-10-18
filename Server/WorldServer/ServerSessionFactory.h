#ifndef _SERVER_SESSION_FACTORY_H_
#define _SERVER_SESSION_FACTORY_H_

#include <MemoryPoolFactory.h>
#include <Singleton.h>
#include <const.h>

class ServerSession;
class TempServerSession;
class BattleServerSession;
class FieldServerSession;
class GameDBProxySession;
class AccountDBProxySession;
class AgentServerSession;
class MasterServerSession;
class GuildServerSession;

class ServerSessionFactory : public util::Singleton<ServerSessionFactory>
{
public:
	ServerSessionFactory();
	~ServerSessionFactory();

	ServerSession*								AllocServerSession( eSERVER_TYPE eServerType );
	BOOL										FreeServerSession( ServerSession * pServerSession );

private:
	util::CMemoryPoolFactory<TempServerSession>		*m_pTempServerPool;
	util::CMemoryPoolFactory<BattleServerSession>		*m_pBattleServerPool;
	util::CMemoryPoolFactory<FieldServerSession>		*m_pFieldServerPool;
	util::CMemoryPoolFactory<GameDBProxySession>		*m_pGameDBProxyPool;	
	util::CMemoryPoolFactory<AccountDBProxySession>	*m_pAccountDBProxyPool;	
	util::CMemoryPoolFactory<AgentServerSession>		*m_pAgentServerPool;	
	util::CMemoryPoolFactory<MasterServerSession>		*m_pMasterServerPool;	
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	util::CMemoryPoolFactory<GuildServerSession>		*m_pGuildServerPool;
#endif
};


#endif // __SERVER_SESSION_FACTORY_H__