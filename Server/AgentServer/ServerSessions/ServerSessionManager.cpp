#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <Macro.h>

//////////////////////////////////////////////////////////////////////////
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>

#include <ServerSessions/BattleServerSession.h>
#include <Party/PartyManager.h>


ServerSessionManager::ServerSessionManager():	
m_pSessionHashTable( NULL ),
m_pLOFBattleServer( NULL ),
m_pFieldServer(NULL)
{
	m_pSessionHashTable = new util::SolarHashTable<ServerSessionEx*>;
	m_pSessionHashTable->Initialize( _MAX_SESSION_BUCKET_SIZE );
}

ServerSessionManager::~ServerSessionManager()
{
	if( m_pSessionHashTable ) 
	{
		delete m_pSessionHashTable;
		m_pSessionHashTable = NULL;
	}
}

VOID ServerSessionManager::AddServer( ServerSessionEx* pServer )
{
	switch( pServer->GetServerType() )
	{
	case FIELD_SERVER:
		if( NULL == m_pFieldServer )
			m_pFieldServer = pServer;
		break;
	}
	//pServer->Init();
	ASSERT( NULL == m_pSessionHashTable->GetData( pServer->GetSessionIndex() ) );
	m_pSessionHashTable->Add( pServer, pServer->GetSessionIndex() );
}

VOID		ServerSessionManager::DisconnectAllServer()
{
	SESSION_LIST::iterator it;
	for( it = m_pSessionHashTable->begin(); it != m_pSessionHashTable->end() ; ++it )
		(*it)->Disconnect();
}


ServerSessionEx*	ServerSessionManager::FindServer( DWORD dwSessionIndex )
{
	return m_pSessionHashTable->GetData( dwSessionIndex );
}

VOID ServerSessionManager::RemoveServer( ServerSessionEx* pServer )
{
	ASSERT( NULL != m_pSessionHashTable->GetData( pServer->GetSessionIndex() ) );

	//pServer->Release();

	m_pSessionHashTable->Remove( pServer->GetSessionIndex() );

	switch( pServer->GetServerType() )
	{
	case FIELD_SERVER:
		{
			ASSERT(m_pFieldServer && "RemoveServer두번호출되는경우?" );
			if( !m_pFieldServer || //< 필드서버가 끊어질때, 다른 필드 서버가 없을때
				(m_pFieldServer && m_pFieldServer->GetSessionIndex() == pServer->GetSessionIndex()) )
			{
				m_pFieldServer = GetFieldServerSession();
			}
		}
		break;
	}

	
}

VOID ServerSessionManager::Process()
{
	ServerSessionEx* pServer = NULL;
	m_pSessionHashTable->SetFirst();
	while( ( pServer = m_pSessionHashTable->GetNext() ) != NULL )
	{
		pServer->Update();
	}
}

ServerSessionEx* ServerSessionManager::GetFieldServerSession()
{
	SESSION_LIST::iterator it, end( m_pSessionHashTable->end() );
	for( it = m_pSessionHashTable->end() ; it != end ; ++it )
	{
		if( (*it)->GetServerType() == FIELD_SERVER )
			return *it;
	}
	return NULL;
}

BOOL ServerSessionManager::update_LOF_BattleServer()
{
	BattleServerSession* pLOFServer = NULL;
	BattleServerSession* pBServer = NULL;
	DWORD dwMinNum = ULONG_MAX;						//! Max값
	ServerSessionEx* pServer = NULL;
	m_pSessionHashTable->SetFirst();
	while( ( pServer = m_pSessionHashTable->GetNext() ) != NULL )
	{
		if( pServer->GetServerType() == BATTLE_SERVER )
		{
			pBServer = (BattleServerSession*)pServer;
			if( pBServer->GetNumberOfRooms() < dwMinNum )
			{
				pLOFServer = pBServer;
				dwMinNum = pLOFServer->GetNumberOfRooms();
			}
		}
	}
	if( NULL == pLOFServer ) return FALSE;
	m_pLOFBattleServer = pLOFServer;
	return TRUE;
}


DWORD ServerSessionManager::Incr_LOF_BattleServer()
{
    if (!update_LOF_BattleServer())
        return 0;

    m_pLOFBattleServer->increasingRoom();
    return m_pLOFBattleServer->GetSessionIndex();
}

VOID ServerSessionManager::Decr_LOF_BattleServer( DWORD dwSessionIndex )
{
	ServerSessionEx* pSession = FindServer( dwSessionIndex );
	if( NULL == pSession )
	{
		BOOL rt = update_LOF_BattleServer();
		SUNLOG( eCRITICAL_LOG, "[ServerSessionManager::Decr_LOF_BattleServer] Battle 서버세션이 없다.update_LOF_BattleServer[rt:%d]",rt);
		return;
	}
	ASSERT( (pSession)->GetServerType() == BATTLE_SERVER );
	if( (pSession)->GetServerType() != BATTLE_SERVER )
		return ;

	BattleServerSession* pBattle = (BattleServerSession*)pSession;
	pBattle->decreasingRoom();
	BOOL rt = update_LOF_BattleServer();
	ASSERT( rt == TRUE );
}
DWORD ServerSessionManager::GetBattleServerSessionNum()
{
	DWORD count = 0;
	SESSION_ITERATOR it, end(m_pSessionHashTable->end());
	for( it = m_pSessionHashTable->begin() ; it != end ; ++it )
	{
		if( (*it)->GetServerType() == BATTLE_SERVER )
			++count;
	}
	return count;
}

