#include "stdafx.h"
#include "CharNameUserList.h"
#include "User.h"
#include <assert.h>
#include <algorithm>

CharNameUserList::CharNameUserList()
{
}

CharNameUserList::~CharNameUserList()
{
}

VOID CharNameUserList::Init()
{
}

VOID CharNameUserList::Release()
{
	m_mapUsers.clear();
}

VOID CharNameUserList::Add( std::string strCharName, User *pUser )
{
	// 대문자 변환
	transform( strCharName.begin(), strCharName.end(), strCharName.begin(), toupper );

	// CharName을 키로 유져 추가
	CUSER_MAP_ITER it = m_mapUsers.find( strCharName );
#ifdef _DEBUG
	assert( it == m_mapUsers.end() );
#else
	if( it != m_mapUsers.end() )
	{
		m_mapUsers.erase( it );
		MessageOut( eFULL_LOG,  "CharNameUserList::Add - Existed Character Name(%s)  ", strCharName.c_str() );
	}
#endif
	m_mapUsers.insert( CUSER_MAP_PAIR( strCharName, pUser ) );
	//MessageOut(eCRITICAL_LOG,  "캐릭터 추가(%d)(%s)", pUser->GetGUID(), strCharName.c_str() );
}

VOID CharNameUserList::Remove( std::string strCharName )
{
	// 대문자 변환
	transform( strCharName.begin(), strCharName.end(), strCharName.begin(), toupper );

	// CharName을 키로 하는 맵에서 제거
	CUSER_MAP_ITER it = m_mapUsers.find( strCharName );
	if( it != m_mapUsers.end() )
	{
//		User *pUser = it->second;
//		if( !pUser )
//		{
//			MessageOut( eCRITICAL_LOG, "CharNameUserList::Remove Error - No User[%s] In Map", strCharName.c_str() );
//			return;
//		}
		m_mapUsers.erase( it );
		//MessageOut(eCRITICAL_LOG,  "캐릭터 제거(%d)(%s)", pUser->GetGUID(), strCharName.c_str() );
	}
}

User* CharNameUserList::Find( std::string strCharName )
{
	// 대문자 변환
	transform( strCharName.begin(), strCharName.end(), strCharName.begin(), toupper );

	CUSER_MAP_ITER it = m_mapUsers.find( strCharName );

	if( it != m_mapUsers.end() )
	{
		return it->second;
	}

	return NULL;
}
/*
VOID CharNameUserList::SendToAll( BYTE *pMsg, WORD wSize )
{
	CUSER_MAP_ITER it;
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
*/