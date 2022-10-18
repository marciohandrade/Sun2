#pragma once

#include <serverframe.h>
#include "AuthServerSession.h"
#include "GameAgentServerSession.h"
#include <Singleton.h>


enum { AGENT_READY_GAMEAGENT_IPLIST, AGENT_READY_MAX };


class AuthAgentFrame : public ServerFrame, public util::Singleton<AuthAgentFrame>
{
public:
	AuthAgentFrame();
	~AuthAgentFrame();

	static NetworkObject*	DoAllocServerSession();
	static VOID DoFreeServerSession(NetworkObject* pNetObject);
	static VOID DoFreeConnectSession(NetworkObject* pNetObject);

	static NetworkObject*	DoAllocClient_NotUsed() { return NULL; }
	static VOID DoFreeClient_NotUsed(NetworkObject* pNetObject) { __UNUSED(pNetObject); }
	static VOID DoFreeConnect_NotUsed(NetworkObject* pNetObject) { __UNUSED(pNetObject); }

public:
	virtual eSERVER_TYPE	GetServerType() { return AUTH_AGENT_SERVER; }

	BOOL					SendAuthServer(MSG_BASE_INTERNAL* pMsg, WORD wSize);
	BOOL					IsAuthServerConnected();
	BOOL					SendGameAgentServer(MSG_BASE_INTERNAL* pMsg, WORD wSize);
	BOOL					IsGameAgentConnected();

	BOOL					ConnectTo(eSERVER_TYPE type);
	ServerSession*			GetSession(eSERVER_TYPE type);

protected:
	virtual BOOL			Init();
	virtual BOOL			Update(DWORD tick);
	virtual void			Release();

	virtual BOOL			KeyEvent(CHAR ch);
	VOID					DisplayServerInfo();

	virtual ServerSession*  AllocServerSession(eSERVER_TYPE);
	virtual void			FreeServerSession(ServerSession*);

	BOOL					CreateSunLog();

private:
	util::CMemoryPoolFactory<GameAgentServerSession> m_GameAgentServerSessionPool;
	util::CMemoryPoolFactory<AuthServerSession> m_AuthServerSessionPool;

	GameAgentServerSession* m_pGameAgentServerSession;
	AuthServerSession* m_pAuthServerSession;

};


