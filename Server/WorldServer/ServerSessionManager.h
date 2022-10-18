#ifndef __SERVER_SESSION_MANAGER_H__
#define __SERVER_SESSION_MANAGER_H__


#pragma once

class ServerSession;

#include <Singleton.h>
#include <map>

typedef STLX_MAP<DWORD, ServerSession*>				SERVER_SESSION_MAP;
typedef STLX_MAP<DWORD, ServerSession*>::iterator	SERVER_SESSION_MAP_ITER;
typedef std::pair<DWORD, ServerSession*>			SERVER_SESSION_PAIR;

class ServerSessionManager : public util::Singleton<ServerSessionManager>
{
public:
	ServerSessionManager();
	~ServerSessionManager();

	VOID							AddServer( ServerSession * pServer );
	VOID							RemoveServer( DWORD dwSessionIndex );
	ServerSession *					FindServer( DWORD dwSessionIndex );
	VOID							Update();

	ServerSession*					GetMasterServer() { return m_pMasterServer; }
	ServerSession*					GetGameDBProxy() { return m_pGameDBProxy; }
	ServerSession*					GetGuildServer();
	ServerSession*					FindAgentServer( DWORD dwChannelID );

	inline SERVER_SESSION_MAP&		GetServerSession(){ return m_mapSessions; }

private:
	SERVER_SESSION_MAP				m_mapSessions;
	ServerSession					*m_pMasterServer;
	ServerSession					*m_pGameDBProxy;
};

#endif // __SERVER_SESSION_MANAGER_H__