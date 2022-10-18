#include "StdAfx.h"
#include ".\waitinguserlist.h"
#include "User.h"
#include <vector>

WaitingUserList::WaitingUserList(void)
{
}

WaitingUserList::~WaitingUserList(void)
{
}

VOID WaitingUserList::Init()
{
}

VOID WaitingUserList::Release()
{
	m_mapUsers.clear();
	m_mapGuidUsers.clear();
}

VOID WaitingUserList::Update()
{
	User *pUser;
	WAITING_MAP_ITER it;

	STLX_VECTOR<User*> eraseList;

	// 일정 시간안에 접속되지 않은 유져들은 대기 리스트에서 삭제한다.
	for( it = m_mapUsers.begin(); it != m_mapUsers.end(); ++it )
	{
		pUser = it->second;
		if( pUser->IsTimeout() )
		{
			eraseList.push_back( pUser );
		}
	}

	if( !eraseList.empty() )
	{
		STLX_VECTOR<User*>::iterator it_erase;
		for( it_erase = eraseList.begin(); it_erase != eraseList.end(); ++it_erase )
		{
			pUser = *it_erase;
			RemoveUser( pUser->GetAuthID() );
			MessageOut(eCRITICAL_LOG,  "Timeout and Removed From WaitingUserList(%d)", pUser->GetGUID() );
		}
		eraseList.clear();
	}
}

VOID WaitingUserList::AddUser( DWORD dwAuthUserID, User *pUser )
{	
	WAITING_MAP_ITER it;

	// 이미 맵에 있는 경우 제거
	if( FindUserWithAuthID( dwAuthUserID ) )
	{
		RemoveUser( dwAuthUserID );
	}

	// 타임아웃 설정
	pUser->StartTimeCheck();
	
	// dwAuthUserID를 키로 맵에 추가
	m_mapUsers.insert( WAITING_MAP_PAIR( dwAuthUserID, pUser ) );
	
	// Guid를 키로 맵에 추가
	assert( pUser->GetGUID() != 0 );
	m_mapGuidUsers.insert( WAITING_MAP_PAIR( pUser->GetGUID(), pUser ) );
}

VOID WaitingUserList::RemoveUser( DWORD dwAuthUserID )
{
	WAITING_MAP_ITER it = m_mapUsers.find( dwAuthUserID );
	User *pUser;
	if( it != m_mapUsers.end() )
	{
		pUser = it->second;
		m_mapUsers.erase( it );

		// guid map에서도 제거
		it = m_mapGuidUsers.find( pUser->GetGUID() );
		if( it != m_mapGuidUsers.end() )
		{
			m_mapGuidUsers.erase( it );
		}
	}
}

User* WaitingUserList::FindUserWithAuthID( DWORD dwAuthUserID )
{
	WAITING_MAP_ITER it = m_mapUsers.find( dwAuthUserID );

	if( it != m_mapUsers.end() )
	{
		return it->second;
	}

	return NULL;
}

User* WaitingUserList::FindUserWithGuid( DWORD dwGuid )
{
	WAITING_MAP_ITER it = m_mapGuidUsers.find( dwGuid );

	if( it != m_mapGuidUsers.end() )
	{
		return it->second;
	}

	return NULL;
}