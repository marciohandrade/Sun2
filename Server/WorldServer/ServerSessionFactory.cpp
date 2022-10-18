#include "stdafx.h"
#include "ServerSessionFactory.h"
#include <Macro.h>
#include "TempServerSession.h"
#include "BattleServerSession.h"
#include "FieldServerSession.h"
#include "GameDBProxySession.h"
#include "AccountDBProxySession.h"
#include "AgentServerSession.h"
#include "MasterServerSession.h"
#include "GuildServerSession.h"

ServerSessionFactory::ServerSessionFactory()
{
	m_pTempServerPool		= new util::CMemoryPoolFactory<TempServerSession>;
	m_pBattleServerPool		= new util::CMemoryPoolFactory<BattleServerSession>;
	m_pFieldServerPool		= new util::CMemoryPoolFactory<FieldServerSession>;
	m_pGameDBProxyPool		= new util::CMemoryPoolFactory<GameDBProxySession>;
	m_pAccountDBProxyPool	= new util::CMemoryPoolFactory<AccountDBProxySession>;
	m_pAgentServerPool		= new util::CMemoryPoolFactory<AgentServerSession>;
	m_pMasterServerPool		= new util::CMemoryPoolFactory<MasterServerSession>;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	m_pGuildServerPool		= new util::CMemoryPoolFactory<GuildServerSession>;
#endif
	// 여러개가 될 수 있으므로 개수 조절이 필요!!!
	m_pTempServerPool->Initialize( 10, 1 );
	m_pBattleServerPool->Initialize( 1, 1 );
	m_pFieldServerPool->Initialize( 1, 1 );
	m_pGameDBProxyPool->Initialize( 1, 1 );
	m_pAccountDBProxyPool->Initialize( 1, 1 );
	m_pAgentServerPool->Initialize( 5, 1 );
	m_pMasterServerPool->Initialize( 1, 1 );
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	m_pGuildServerPool->Initialize( 1, 1 );
#endif
}

ServerSessionFactory::~ServerSessionFactory()
{
	delete m_pTempServerPool;
	delete m_pBattleServerPool;
	delete m_pFieldServerPool;
	delete m_pGameDBProxyPool;
	delete m_pAccountDBProxyPool;
	delete m_pAgentServerPool;
	delete m_pMasterServerPool;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	delete m_pGuildServerPool;
#endif
}

ServerSession * ServerSessionFactory::AllocServerSession( eSERVER_TYPE eServerType )
{
	ServerSession *pServerSession = NULL;

	switch( eServerType )
	{
	case TEMP_SERVER:
		pServerSession = (ServerSession*)m_pTempServerPool->Alloc();
		break;
	case BATTLE_SERVER:
		pServerSession = (ServerSession*)m_pBattleServerPool->Alloc();
		break;
	case FIELD_SERVER:
		pServerSession = (ServerSession*)m_pFieldServerPool->Alloc();
		break;
	case GAME_DBPROXY:
		pServerSession = (ServerSession*)m_pGameDBProxyPool->Alloc();
		break;
	case ACCOUNT_DBPROXY:
		pServerSession = (ServerSession*)m_pAccountDBProxyPool->Alloc();
		break;
	case AGENT_SERVER:
		pServerSession = (ServerSession*)m_pAgentServerPool->Alloc();
		break;
	case MASTER_SERVER:
		pServerSession = (ServerSession*)m_pMasterServerPool->Alloc();
		break;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	case GUILD_SERVER:
		pServerSession = (ServerSession*)m_pGuildServerPool->Alloc();
		break;
#endif
	}

	assert( pServerSession != NULL && "Invalid server type Allocation" );

	if(!pServerSession)
	{
		MessageOut(eCRITICAL_LOG, "Invalid Server Type Allocation...");
		return NULL;
	}

	pServerSession->Init();

	return pServerSession;
}

BOOL ServerSessionFactory::FreeServerSession( ServerSession * pServerSession )
{
	pServerSession->Release();

	switch( pServerSession->GetServerType() )
	{
	case TEMP_SERVER:
		return m_pTempServerPool->Free( (TempServerSession*)pServerSession );
	case BATTLE_SERVER:
		return m_pBattleServerPool->Free( (BattleServerSession*)pServerSession );
	case FIELD_SERVER:
		return m_pFieldServerPool->Free( (FieldServerSession*)pServerSession );
	case GAME_DBPROXY:
		return m_pGameDBProxyPool->Free( (GameDBProxySession*)pServerSession );
	case ACCOUNT_DBPROXY:
		return m_pAccountDBProxyPool->Free( (AccountDBProxySession*)pServerSession );
	case AGENT_SERVER:
		return m_pAgentServerPool->Free( (AgentServerSession*)pServerSession );
	case MASTER_SERVER:
		return m_pMasterServerPool->Free( (MasterServerSession*)pServerSession );
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	case GUILD_SERVER:
		return m_pGuildServerPool->Free( (GuildServerSession*)pServerSession );
#endif
	default:
		{
			assert( !"잘못된 타입의 서버세션을 Free할 수 없다" );
		}
	}
	return FALSE;
}