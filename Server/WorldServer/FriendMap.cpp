#include "StdAfx.h"

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING

#include ".\friendmap.h"
#include "UserManager.h"

FriendMap::FriendMap(void)
{
}

FriendMap::~FriendMap(void)
{
}

VOID FriendMap::init()
{
	Clear();
    // (CHANGES) (100115) (WAVERIX) changes invalid code rule
    m_iterNowFriend = m_mapFriend.end();
}

VOID FriendMap::Clear()
{
	STRUCT_FRIEND_INFO *pFriend;
	if( !empty() )
	{
		FRIEND_INFO_MAP_ITER	iterFriend;
		for( iterFriend = begin(); iterFriend != end(); iterFriend++ )

		{
			pFriend = (*iterFriend).second;
			if( !pFriend ) continue;
			UserManager::Instance()->FreeFriendInfo( pFriend );
		}
		clear();
	}
}

STRUCT_FRIEND_INFO* FriendMap::GetFirstFriend()
{
	//	if( !CheckFriendList() )
	//	{
	//		return NULL;
	//	}

	m_iterNowFriend = m_mapFriend.begin();

	if( m_iterNowFriend == m_mapFriend.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowFriend).second );
}

STRUCT_FRIEND_INFO* FriendMap::GetNextFriend()
{
	if(/* !CheckFriendList() || */m_iterNowFriend == m_mapFriend.end() )
	{
		return NULL;
	}

	m_iterNowFriend++;

	if( m_iterNowFriend == m_mapFriend.end() )
	{
		return NULL;
	}

	return ( (*m_iterNowFriend).second );
}

BOOL FriendMap::AddFriendInfo( STRUCT_FRIEND_INFO* pFriend )
{
	if( GetFriendNum() >= MAX_FRIEND_STAT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[User::AddFriendInfo] OverFlow, FriendGuid = %u" , pFriend->dwFriendGuid ); 
		return FALSE;
	}	

	// 이미 같은 정보가 있으면 에러
	if( FindFriend( pFriend->dwFriendGuid ) != NULL )
	{
		return FALSE;
	}

    STRUCT_FRIEND_INFO *pNowFriend = UserManager::Instance()->AllocFriendInfo();	//new STRUCT_FRIEND_INFO;

    memcpy( pNowFriend, pFriend, sizeof(STRUCT_FRIEND_INFO) );

	m_mapFriend.insert( FRIEND_INFO_MAP_PAIR( pFriend->dwFriendGuid, pNowFriend ) );

	return TRUE;
}

STRUCT_FRIEND_INFO* FriendMap::FindFriend( DWORD dwCharGuid )
{
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.find( dwCharGuid );
	if( iter == m_mapFriend.end() )
	{
		return NULL;
	}

	STRUCT_FRIEND_INFO *pFriendInfo = (*iter).second;

	return pFriendInfo;
}

STRUCT_FRIEND_INFO* FriendMap::FindFriend( TCHAR* ptszFriendName )
{
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.begin();
	if( iter == m_mapFriend.end() )
	{
		return NULL;
	}

	for( iter = m_mapFriend.begin(); iter != m_mapFriend.end(); iter++ )
	{
		STRUCT_FRIEND_INFO *pFriendInfo = (*iter).second;

		if( !_tcsnicmp( pFriendInfo->sPacketInfo.ptszFriendName, ptszFriendName, MAX_CHARNAME_LENGTH ) )
		{
			return pFriendInfo;
		}
	}

	return NULL;
}

BOOL FriendMap::RemoveFriend( DWORD dwGuid )
{
	FRIEND_INFO_MAP_ITER iter = m_mapFriend.find( dwGuid );
	if( iter == m_mapFriend.end() )
	{
		return FALSE;
	}

	STRUCT_FRIEND_INFO *pFriend = (*iter).second;

	//	delete pFriend;
	UserManager::Instance()->FreeFriendInfo( pFriend );
	m_mapFriend.erase(iter);
    return TRUE;
}

#endif //_NA_0_20091104_FIRENDMAP_REFACTORYING