#include "stdafx.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include <assert.h>

#include <ServerStruct.h>

#include <WorldServer.h>

ServerSessionManager::ServerSessionManager()
{
	m_pMasterServer	= NULL;
	m_pGameDBProxy = NULL;
}

ServerSessionManager::~ServerSessionManager()
{
}

VOID ServerSessionManager::AddServer( ServerSession * pServer )
{
	assert( FindServer( pServer->GetSessionIndex() ) == NULL );
	if( FindServer( pServer->GetSessionIndex() ) )
	{
		SUNLOG( eCRITICAL_LOG, "[AddServer] Session(%u) already exist!", pServer->GetSessionIndex() );
		return;
	}

	m_mapSessions.insert( SERVER_SESSION_PAIR( pServer->GetSessionIndex(), pServer ) );

	switch( pServer->GetServerType() )
	{
	case MASTER_SERVER:
		m_pMasterServer = pServer;	break;
	case GAME_DBPROXY:
		m_pGameDBProxy = pServer;	break;
	}

#ifdef _JP_0_20101123_PMS
    WorldServer::Instance()->SetPMSSessionCount();
#endif
}

VOID ServerSessionManager::RemoveServer( DWORD dwSessionIndex )
{
	SERVER_SESSION_MAP_ITER it = m_mapSessions.find( dwSessionIndex );
	assert( it != m_mapSessions.end() );
	if( it == m_mapSessions.end() )
	{
		SUNLOG( eCRITICAL_LOG, "[RemoveServer] Session(%u) doesn't exist!", dwSessionIndex );
		return;
	}

	ServerSession *pServer = it->second;
	m_mapSessions.erase( it );

	switch( pServer->GetServerType() )
	{
	case MASTER_SERVER:
		m_pMasterServer = NULL;		break;
	case GAME_DBPROXY:
		m_pGameDBProxy = NULL;		break;
	}
#ifdef _JP_0_20101123_PMS
    WorldServer::Instance()->SetPMSSessionCount();
#endif
}

ServerSession *	ServerSessionManager::FindServer( DWORD dwSessionIndex )
{
	SERVER_SESSION_MAP_ITER it = m_mapSessions.find( dwSessionIndex );

	if( it == m_mapSessions.end() ) return NULL;

	return it->second;
}

VOID ServerSessionManager::Update()
{
	SERVER_SESSION_MAP_ITER it;

	for( it = m_mapSessions.begin(); it != m_mapSessions.end(); ++it )
	{
		it->second->Update();
	}
}

ServerSession* ServerSessionManager::GetGuildServer()
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    return WorldServer::Instance()->GetGameDBProxy();
#else
	SERVER_SESSION_MAP_ITER it;

	for( it = m_mapSessions.begin(); it != m_mapSessions.end(); ++it )
	{
		if( it->second->GetServerType() == GUILD_SERVER )
		{
			return it->second;
		}
	}

	return NULL;
#endif
}

ServerSession* ServerSessionManager::FindAgentServer( DWORD dwChannelID )
{
	SERVER_SESSION_MAP_ITER it;

	for( it = m_mapSessions.begin(); it != m_mapSessions.end(); ++it )
	{
		if( it->second->GetServerType() == AGENT_SERVER )
		{
			SERVER_KEY ServerKey = it->second->GetServerKey();

			if( ServerKey.GetChannelID() == dwChannelID )
			{
				return it->second;
			}
		}
	}

	return NULL;
}
