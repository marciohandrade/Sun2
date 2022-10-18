#include "stdafx.h"
#include "ServerInfoManager.h"

#include <Sessions/ServerSessionEx.h>
//
typedef ServerSessionEx		TypeDefinedServerSession;
#include <Include/ServerTree.h>
//
typedef ServerTree<sSERVER_INFO>		ServerTreeINFO;

//
static struct ServerInfoManagerInternal
{
	ServerTreeINFO*				m_pServerTreeInfo;

	ServerTreeINFO&				GetServerInfoTree() { return *m_pServerTreeInfo; }

	VOID						DeleteAll()
	{
		//if( !m_pServerTreeInfo )
		//	return;

		//SAFE_DELETE(m_pServerTreeInfo);
	}
} s_ServerInfoInternal =
{
	NULL,
};

VOID			ServerInfoManager::CreateInstance()
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;
	if( !rINFO.m_pServerTreeInfo )
	{
		rINFO.m_pServerTreeInfo = new ServerTreeINFO;
		return;
	}

	ASSERT( !"중복 호출이 존재합니다." );
}

VOID			ServerInfoManager::DestroyInstance()
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;
	if( rINFO.m_pServerTreeInfo )
	{
		rINFO.DeleteAll();
		SAFE_DELETE( rINFO.m_pServerTreeInfo );
		return;
	}

	ASSERT( !"CreateInstance와의 쌍이 맞지 않아용" );
}

VOID			ServerInfoManager::Clear()
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;
	rINFO.DeleteAll();
}

VOID			ServerInfoManager::AddServerInfo( LPSERVER_INFO lpServerInfo, DWORD dwCount, BOOL bRefresh )
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;
	LPSERVER_INFO lpOffset = lpServerInfo;

	ServerTreeINFO& rTREE = rINFO.GetServerInfoTree();

	for( DWORD inc=0 ; inc<dwCount ; ++inc, ++lpOffset )
	{
		// 일단 서버 정보 맵에 같은 키가 존재하면 안됨
		SERVER_KEY key = lpOffset->ServerKey;

		if( rTREE.FindServer( key ) )
		{
			if( !bRefresh )
			{
				MessageOut( eCRITICAL_LOG,
					_T(">>> Warning!! Duplicated Server Information!! ServerKey (%u:%u:%u:%u) <<<"),
					key.GetWorldID(), key.GetChannelID(), key.GetServerType(), key.GetServerID()
					);
				continue;
			}
			else
			{
				rTREE.RemoveServer( key );
			}
		}

		sSERVER_INFO sServerInfo;
		sServerInfo.m_ServerType = (eSERVER_TYPE)key.GetServerType();
		sServerInfo.m_ServerKey = key;
		memcpy( &sServerInfo.m_ServerInfo, lpOffset, sizeof(SERVER_INFO) );

		rTREE.AddServer( key, &sServerInfo );
	}
}

LPSERVER_INFO	ServerInfoManager::GetServerInfo( SERVER_KEY skKey )
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;

	// 일단 서버 정보 맵에 같은 키가 존재하면 안됨
	ServerTreeINFO& rTREE = rINFO.GetServerInfoTree();

	sSERVER_INFO* pServerInfo = rTREE.FindServer( skKey );

	if( pServerInfo )
		return &(pServerInfo->m_ServerInfo);
	return NULL;
}

// 서버 타입과 IP로 해당 서버 정보의 키값을 얻어낸다.
SERVER_KEY		ServerInfoManager::GetServerKey( eSERVER_TYPE serverType, CHAR* pszIP )
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;
	ServerTreeINFO& rTREE = rINFO.GetServerInfoTree();

	sPTR_SERVER_INFO_ARRAY	ptrARRAY;

	rTREE.GetPtrList( (DWORD)(1<<serverType), &ptrARRAY );

	DWORD dwMaxCount = ptrARRAY.m_dwCount;

	if( !dwMaxCount )
		return 0;

	sSERVER_INFO** pINFO = ptrARRAY.m_pPTRs;
	for( DWORD i=0 ; i<dwMaxCount ; ++i, ++pINFO )
	{
		if( strncmp( (*pINFO)->m_ServerInfo.szInnerIP, pszIP, sizeof((*pINFO)->m_ServerInfo.szInnerIP) ) == 0 )
		{
			return (SERVER_KEY)(*pINFO)->m_ServerKey;
		}
	}

	return 0;
}

VOID			ServerInfoManager::_GetServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pTargetInfo, eSERVER_TYPE serverType )
{
	ServerInfoManagerInternal& rINFO = s_ServerInfoInternal;
	ServerTreeINFO& rTREE = rINFO.GetServerInfoTree();

	ServerTreeINFO::SERVER_NODE_LIST* pNodeList = rTREE.GetServerNodeList( byWorldID, byChannelID );

	if( !pNodeList )
		memset( pTargetInfo, 0, sizeof(SERVER_INFO) );
	else
	{
		ServerTreeINFO::SERVER_NODE_LIST::iterator it = pNodeList->begin();
		ServerTreeINFO::SERVER_NODE_LIST::iterator end = pNodeList->end();

		for( ; it != end ; ++it )
		{
			sSERVER_INFO* pSERVER_INFO = *it;
			eSERVER_TYPE& eServerType = pSERVER_INFO->m_ServerType;
			SERVER_KEY& rServerKey = pSERVER_INFO->m_ServerKey;

			if( serverType == eServerType
				&& rServerKey.GetWorldID() == byWorldID
				&& rServerKey.GetChannelID() == byChannelID )
			{
				memcpy( pTargetInfo, &(pSERVER_INFO->m_ServerInfo), sizeof(SERVER_INFO) );
				break;
			}
		}
	}
}

VOID			ServerInfoManager::GetGameDBProxyInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pGameDBProxyInfo )
{
	//ASSERT( byChannelID == 0 );
	byChannelID = 0;
	_GetServerInfo( byWorldID, byChannelID, pGameDBProxyInfo, GAME_DBPROXY );
}

VOID			ServerInfoManager::GetAgentServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pAgentServerInfo )
{
	_GetServerInfo( byWorldID, byChannelID, pAgentServerInfo, AGENT_SERVER );
}

VOID			ServerInfoManager::GetWorldServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pWorldServerInfo )
{
	//ASSERT( byChannelID == 0 );
	byChannelID = 0;
	_GetServerInfo( byWorldID, byChannelID, pWorldServerInfo, WORLD_SERVER );
}

VOID			ServerInfoManager::GetGuildServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pGuildServerInfo )
{
	//ASSERT( byChannelID == 0 );
	byChannelID = 0;
	_GetServerInfo( byWorldID, byChannelID, pGuildServerInfo, GUILD_SERVER );
}

VOID			ServerInfoManager::GetPortalServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pPortalServerInfo )
{
	//ASSERT( byChannelID == 0 );
	byChannelID = 0;
	_GetServerInfo( byWorldID, byChannelID, pPortalServerInfo, PORTAL_SERVER );
}

VOID			ServerInfoManager::GetAuthAgentServerInfo( LPSERVER_INFO pAuthAgentServerInfo )
{
	_GetServerInfo( 0, 0, pAuthAgentServerInfo, AUTH_AGENT_SERVER );
}

VOID			ServerInfoManager::GetShopServerInfo( LPSERVER_INFO pShopServerInfo )
{
	_GetServerInfo( 0, 0, pShopServerInfo, SHOP_SERVER );
}



