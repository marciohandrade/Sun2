
#include "stdafx.h"

#include "ServerSession.h"
#include "ServerSessionManager.h"

#include "../Information/ScriptLoadManager.h"
#include "../Information/SolarAcceptableMasterServerListLoader.h"


//private:
// <INTERNAL><INTERFACEs>
ServerSessionManager::ServerSessionManager()
{
	_Initialize();
}

ServerSessionManager::~ServerSessionManager()
{
	SAFE_DELETE( AcceptedSessions.REFERENCE() );
	SAFE_DELETE( ConnectedSessions.REFERENCE() );
}

VOID				ServerSessionManager::_Initialize()
{
	AcceptedSessions.REFERENCE()	= new SERVER_SESSION_MAP();
	ConnectedSessions.REFERENCE()	= new SERVER_SESSION_CONTAINER();
	m_EmptySession.REFERENCE()		= NULL;

	ScriptLoadManager* pManager = ScriptLoadManager::Instance();
	pManager->LoadAcceptableMasterServerList();

}

VOID				ServerSessionManager::AddServer( SERVER_SESSION_DELEGATOR pServer )
{
	DWORD dwKey = (DWORD)(__int64)pServer();
	SERVER_SESSION_CONTAINER_ITER it = ConnectedSessions->find( dwKey );
	if( it != ConnectedSessions->end() )
	{
		ASSERT( !"중복된 서버가 추가되려고 하고 있다." );
		return;
	}

	ConnectedSessions->insert(
							ConnectedSessions->end(),
							SERVER_SESSION_CONTAINER_PAIR( dwKey, pServer )
							);
}

VOID				ServerSessionManager::RemoveServer( SERVER_SESSION_DELEGATOR pServer )
{
	DWORD dwKey = (DWORD)(__int64)pServer();
	SERVER_SESSION_CONTAINER_ITER it = ConnectedSessions->find( dwKey );
	if( it == ConnectedSessions->end() )
	{
		ASSERT( !"존재하지 않는 서버를 삭제하려고 하고 있다." );
		return;
	}

	ConnectedSessions->erase( it );
}

SERVER_SESSION_DELEGATOR&
ServerSessionManager::FindServer( SERVER_SESSION_DELEGATOR pServer )
{
	DWORD dwKey = (DWORD)(__int64)pServer();
	SERVER_SESSION_MAP_ITER it = ConnectedSessions->find( dwKey );
	if( it == ConnectedSessions->end() )
		return m_EmptySession;

	return it->second;
}

VOID				ServerSessionManager::Update()
{
	for_each(
		ConnectedSessions->begin(),
		ConnectedSessions->end(),
		bind1st(mem_fun(&ServerSessionManager::_UpdateSession), this)
		);
}


// 접속되어 있는 마스터서버들에게 브로드캐스팅
VOID				ServerSessionManager::SendToMasterServers( BYTE* pMsg, WORD wSize )
{
}


VOID				ServerSessionManager::_UpdateSession( SERVER_SESSION_PAIR node )
{
	SERVER_SESSION_DELEGATOR pSession = node.second;
	if( pSession != NULL )
		pSession->Update();
}

VOID				ServerSessionManager::_DeleteSession( SERVER_SESSION_PAIR node )
{
}



