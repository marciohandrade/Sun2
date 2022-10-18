#include "StdAfx.h"
#include "GuidUserList.h"
#include "User.h"
#include <assert.h>

GuidUserList::GuidUserList(void)
{
}

GuidUserList::~GuidUserList(void)
{
}

VOID GuidUserList::Init()
{
}

VOID GuidUserList::Release()
{
	m_mapUsers.clear();
}

VOID GuidUserList::Add( DWORD dwGuid, User *pUser )
{
	// GUID를 키로 유져 추가
	GUSER_MAP_ITER it = m_mapUsers.find( dwGuid );

	if( it != m_mapUsers.end() )
		return;

	m_mapUsers.insert( GUSER_MAP_PAIR( dwGuid, pUser ) );
}

VOID GuidUserList::Remove( DWORD dwGuid )
{
	// GUID를 키로 하는 맵에서 제거
	GUSER_MAP_ITER it = m_mapUsers.find( dwGuid );
	if( it != m_mapUsers.end() )
	{
		m_mapUsers.erase( it );
	}
}

User* GuidUserList::Find( DWORD dwGuid )
{
	GUSER_MAP_ITER it = m_mapUsers.find( dwGuid );

	if( it != m_mapUsers.end() )
	{
		return it->second;
	}

	return NULL;
}

VOID GuidUserList::SendToAll( BYTE *pMsg, WORD wSize )
{
	GUSER_MAP_ITER it;
	User *pUser;
	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		if( pUser )
		{
			pUser->SendPacket( pMsg, wSize );
		}
	}
}