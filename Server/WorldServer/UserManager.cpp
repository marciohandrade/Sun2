#include "StdAfx.h"
#include ".\usermanager.h"
#include "User.h"
#include "GuidUserList.h"
#include "CharNameUserList.h"
#include <string>
#include <algorithm>
#include <assert.h>

// 친구 위해
#include <struct.h>

// 암호화 유저 유지보수 위해
#include <vector>

#include "Lotto/Lotto.h"

//__NA_000313_20070223_REALTIME_CHAT_BLOCK

#ifdef _JP_0_20101123_PMS
#include <worldserver.h>
#endif

UserManager::UserManager(void)
{
	m_pGuidUserList		= new GuidUserList;
	m_pCharGuidList		= new GuidUserList;
	m_pCharNameUserList = new CharNameUserList;
	m_mapUserFriend.clear();
	m_vecUserCrypt.clear();
	m_mapChatBlocked.clear();
	m_pFriendPool		= new util::CMemoryPoolFactory<STRUCT_FRIEND_INFO>;
	m_pBlockPool		= new util::CMemoryPoolFactory<STRUCT_BLOCK_INFO>;
	m_pRevengePool		= new util::CMemoryPoolFactory<STRUCT_REVENGE_INFO>;
}

UserManager::~UserManager(void)
{
	SAFE_DELETE( m_pGuidUserList );
	SAFE_DELETE( m_pCharGuidList );
	SAFE_DELETE( m_pCharNameUserList );

	FRIEND_USER_MAP_ITER	iter;
	while( !m_mapUserFriend.empty() )
	{
		iter = m_mapUserFriend.begin();
		(*iter).second.clear();
		m_mapUserFriend.erase(iter);
	}
	m_mapUserFriend.clear();
	m_vecUserCrypt.clear();
	m_mapChatBlocked.clear();

	delete m_pFriendPool;
	delete m_pBlockPool;
	delete m_pRevengePool;
}

VOID UserManager::Init()
{
	m_pGuidUserList->Init();
	m_pCharGuidList->Init();
	m_pCharNameUserList->Init();
	m_mapUserFriend.clear();

	//m_pFriendPool->Initialize( 500000, 250000 );
	//m_pBlockPool->Initialize( 500000, 250000 );
	//m_pRevengePool->Initialize( 500000, 25000 );
	m_pFriendPool->Initialize( 50, 50 );
	m_pBlockPool->Initialize( 50, 50 );
	m_pRevengePool->Initialize( 50, 50 );

}

VOID UserManager::Release()
{
	m_pGuidUserList->Release();
	m_pCharGuidList->Release();
	m_pCharNameUserList->Release();
	FRIEND_USER_MAP_ITER	iter;
	while( !m_mapUserFriend.empty() )
	{
		iter = m_mapUserFriend.begin();
		(*iter).second.clear();
		m_mapUserFriend.erase(iter);
	}
	m_mapUserFriend.clear();

	m_pFriendPool->Release();
	m_pBlockPool->Release();
	m_pRevengePool->Release();
}

// FriendCharGuid를 친구로 지정한 pUser 정보 추가
BOOL UserManager::AddNewFriend( DWORD FriendCharGuid, User* pUser )
{
	// 이 사용자의 친구들 목록 추가
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapFriendAtom;

	// 친구가 들어오면 바로 데이터 얻을 수 있도록 내 친구 정보를 등록한다
	iter = m_mapUserFriend.find( FriendCharGuid );
	if( iter != m_mapUserFriend.end() )
	{
		// 데이터 있으면 그 뒤에 추가
		(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
	}
	else
	{
		// 데이터 없으면 새로 만들어 넣는다
		mapFriendAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
		m_mapUserFriend.insert( FRIEND_USER_MAP_PAIR( FriendCharGuid, mapFriendAtom ) );
	}

	// 이 친구가 온라인이라면 친구 인스턴스에 '내가 너를 친구 먹었다!'고 저장시킨다
	// 이후 pFriend가 로그아웃하면 pUser가 알 수 있다
	User* pFriend = FindUserByCharGuid( FriendCharGuid );

	if( pFriend )
	{
		// 이 친구가 나를 차단했다면 알려주지 않는다
		if( !pFriend->FindBlock( pUser->GetCharGuid() ) )
		{
			pFriend->AddOnlineFriend( pUser );
		}
	}

//	MessageOut( eFULL_LOG, "[%s] AddNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid );
	return TRUE;
}

BOOL UserManager::AddFriend( User* pUser )
{
	// 이 사용자의 친구들 목록 추가
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFriendMap()->GetFirstFriend();
#else
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFirstFriend();	
#endif
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapFriendAtom;

	// 친구가 들어오면 바로 데이터 얻을 수 있도록 내 친구 정보를 등록한다
	if( pFriend )
	{
		do
		{
			iter = m_mapUserFriend.find( pFriend->dwFriendGuid );
			if( iter != m_mapUserFriend.end() )
			{
				// 데이터 있으면 그 뒤에 추가
				(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
			}
			else
			{
				// 데이터 없으면 새로 만들어 넣는다
				mapFriendAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
				m_mapUserFriend.insert( FRIEND_USER_MAP_PAIR( pFriend->dwFriendGuid, mapFriendAtom ) );
			}
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
		}while( pFriend = pUser->GetFriendMap()->GetNextFriend() );
#else
		}while( pFriend = pUser->GetNextFriend() );
#endif
		
	}

	// 이미 등록된 정보들 중에 나를 등록한 정보가 있으면 넣어준다
	iter = m_mapUserFriend.find( pUser->GetCharGuid() );

	if( iter != m_mapUserFriend.end() )
	{
		// 있으면 이걸 유저에게 등록
		FRIEND_ATOM_MAP_ITER	iterAtom;

		if( !(*iter).second.empty() )
		{
			for( iterAtom = (*iter).second.begin(); iterAtom != (*iter).second.end(); iterAtom++ )
			{

				// 내가 이 유저를 차단했다면 알려주지 않는다
				if( !pUser->FindBlock( (*iterAtom).second->GetCharGuid() ) )
				{
					pUser->AddOnlineFriend( (*iterAtom).second );
				}
			}
		}
	}

//	MessageOut( eFULL_LOG, "[%s] AddFriend Complete", pUser->GetCharName().c_str() );
	return TRUE;
}

VOID UserManager::RemoveNewFriend( DWORD FriendCharGuid, User *pUser )
{
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP_ITER	iterAtom;

	// 친구 목록 데이터에서 이 사용자의 친구들을 찾는다
	iter = m_mapUserFriend.find( FriendCharGuid );
	if( iter != m_mapUserFriend.end() )
	{
		// 데이터 있으면
		iterAtom = (*iter).second.find( pUser->GetCharGuid() );

		if( iterAtom != (*iter).second.end() )
		{
			// 이놈을 친구 삼은 나의 데이터도 있으면 지워준다!
			(*iter).second.erase( iterAtom );
			// 지우고 났더니 맵이 비었으면 맵도 지워준다!
			if( (*iter).second.empty() )
			{
				m_mapUserFriend.erase( iter );
			}
		}
		else
		{
			// 데이터 없으면 버그
			MessageOut( eCRITICAL_LOG, "[%s] RemoveNewFriend Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), FriendCharGuid );
			return;
		}
	}
	else
	{
		// 데이터 없으면 버그
		MessageOut( eCRITICAL_LOG, "[%s] RemoveNewFriend Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), FriendCharGuid );
		return;
	}

	// 이 친구가 온라인이라면 이쪽에서도 데이터 지워 줘야 한다
	User* pFriend = FindUserByCharGuid( FriendCharGuid );

	if( pFriend )
	{
		pFriend->RemoveOnlineFriend( pUser );
	}

//	MessageOut( eFULL_LOG, "[%s] RemoveNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid );
}

// 친구 목록 업데이트 - 친구 제거!!
VOID UserManager::RemoveFriend( User *pUser )
{
	// 이 사용자의 친구들 정보를 제거한다
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFriendMap()->GetFirstFriend();
#else
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFirstFriend();	
#endif	
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP_ITER	iterAtom;
	User					*pOnlineUser = NULL;

	// 친구 목록 데이터에서 이 사용자의 친구들을 찾는다
	if( pFriend )
	{
		do
		{
			// 친구 정보 있냐?
			iter = m_mapUserFriend.find( pFriend->dwFriendGuid );
			if( iter != m_mapUserFriend.end() )
			{
				// 데이터 있으면
				iterAtom = (*iter).second.find( pUser->GetCharGuid() );

				if( iterAtom != (*iter).second.end() )
				{
					// 이놈을 친구 삼은 나의 데이터도 있으면 지워준다!
					(*iter).second.erase( iterAtom );
					// 지우고 났더니 맵이 비었으면 맵도 지워준다!
					if( (*iter).second.empty() )
					{
						m_mapUserFriend.erase( iter );
					}
					// 이 친구놈이 온라인이면
					if( pFriend->sPacketInfo.bOnline )
					{
						// 실제 접속중인지 찾아서 온라인 체크 풀어준다
						pOnlineUser = FindUserByCharGuid( pFriend->dwFriendGuid );
						if( pOnlineUser )
						{
							pOnlineUser->RemoveOnlineFriend( pUser );
						}
					}
				}
				else
				{
					// 데이터 없으면 버그
					MessageOut( eCRITICAL_LOG, "[%s] RemoveFriend Error - No %s FriendAtomTable", pUser->GetCharName().c_str(), pFriend->sPacketInfo.ptszFriendName );
				}
			}
			else
			{
				// 데이터 없으면 버그
				MessageOut( eCRITICAL_LOG, "[%s] RemoveFriend Error - No %s FriendTable", pUser->GetCharName().c_str(), pFriend->sPacketInfo.ptszFriendName );
				continue;
			}
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
		}while( pFriend = pUser->GetFriendMap()->GetNextFriend() );
#else
		}while( pFriend = pUser->GetNextFriend() );
#endif		
	}

	// User가 삭제되면서 OnlineFriend 정보도 같이 삭제되므로 굳이 이 User 클래스에 있는 m_mapOnlineFriend를 삭제할 필요는 없다
//	MessageOut( eFULL_LOG, "[%s] RemoveFriend Complete", pUser->GetCharName().c_str() );
}

////////////////////////////////////////
// 리벤지 관련
// RevengeCharGuid를 복수 대상으로 지정한 pUser 정보 추가
BOOL UserManager::AddNewRevenge( DWORD RevengeCharGuid, User* pUser )
{
	// 이 사용자의 복수 대상들 목록 추가
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapRevengeAtom;

	// 그놈이 들어오면 바로 데이터 얻을 수 있도록 내 리벤지 정보를 등록한다
	iter = m_mapUserRevenge.find( RevengeCharGuid );
	if( iter != m_mapUserRevenge.end() )
	{
		// 데이터 있으면 그 뒤에 추가
		(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
	}
	else
	{
		// 데이터 없으면 새로 만들어 넣는다
		mapRevengeAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
		m_mapUserRevenge.insert( FRIEND_USER_MAP_PAIR( RevengeCharGuid, mapRevengeAtom ) );
	}

	// 이놈이 온라인이라면 리벤지 인스턴스에 '내가 너를 복수 대상으로 찜했다!'고 저장시킨다
	// 이후 pRevenge가 로그아웃하면 pUser가 알 수 있다
	User* pRevenge = FindUserByCharGuid( RevengeCharGuid );

	if( pRevenge )
	{
		pRevenge->AddOnlineRevenge( pUser );
	}
//	MessageOut( eFULL_LOG, "[%s] AddNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid );
	return TRUE;
}

BOOL UserManager::AddRevenge( User* pUser )
{
	// 이 사용자의 복수 대상들 목록 추가
	STRUCT_REVENGE_INFO*	pRevenge = pUser->GetFirstRevenge();
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapRevengeAtom;

	// 친구가 들어오면 바로 데이터 얻을 수 있도록 내 친구 정보를 등록한다
	if( pRevenge )
	{
		do
		{
			iter = m_mapUserRevenge.find( pRevenge->dwRevengeCharGuid );
			if( iter != m_mapUserRevenge.end() )
			{
				// 데이터 있으면 그 뒤에 추가
				(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
			}
			else
			{
				// 데이터 없으면 새로 만들어 넣는다
				mapRevengeAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
				m_mapUserRevenge.insert( FRIEND_USER_MAP_PAIR( pRevenge->dwRevengeCharGuid, mapRevengeAtom ) );
			}
		}while( pRevenge = pUser->GetNextRevenge() );
	}

	// 이미 등록된 정보들 중에 나를 등록한 정보가 있으면 넣어준다
	iter = m_mapUserRevenge.find( pUser->GetCharGuid() );

	if( iter != m_mapUserRevenge.end() )
	{
		// 있으면 이걸 유저에게 등록
		FRIEND_ATOM_MAP_ITER	iterAtom;

		if( !(*iter).second.empty() )
		{
			for( iterAtom = (*iter).second.begin(); iterAtom != (*iter).second.end(); iterAtom++ )
			{
				pUser->AddOnlineRevenge( (*iterAtom).second );
			}
		}
	}

//	MessageOut( eFULL_LOG, "[%s] AddFriend Complete", pUser->GetCharName().c_str() );
	return TRUE;
}

VOID UserManager::RemoveNewRevenge( DWORD RevengeCharGuid, User *pUser )
{
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP_ITER	iterAtom;

	// 리벤지 목록 데이터에서 이 사용자의 복수 대상을 찾는다
	iter = m_mapUserRevenge.find( RevengeCharGuid );
	if( iter != m_mapUserRevenge.end() )
	{
		// 데이터 있으면
		iterAtom = (*iter).second.find( pUser->GetCharGuid() );

		if( iterAtom != (*iter).second.end() )
		{
			// 이놈을 복수 대상 삼은 나의 데이터도 있으면 지워준다!
			(*iter).second.erase( iterAtom );
			// 지우고 났더니 맵이 비었으면 맵도 지워준다!
			if( (*iter).second.empty() )
			{
				m_mapUserRevenge.erase( iter );
			}
		}
		else
		{
			// 데이터 없으면 버그
			MessageOut( eCRITICAL_LOG, "[%s] RemoveNewRevenge Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), RevengeCharGuid );
			return;
		}
	}
	else
	{
		// 데이터 없으면 버그
		MessageOut( eCRITICAL_LOG, "[%s] RemoveNewRevenge Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), RevengeCharGuid );
		return;
	}

	// 이놈이 온라인이라면 이쪽에서도 데이터 지워 줘야 한다
	User* pRevenge = FindUserByCharGuid( RevengeCharGuid );

	if( pRevenge )
	{
		pRevenge->RemoveOnlineRevenge( pUser );
	}

//	MessageOut( eFULL_LOG, "[%s] RemoveNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid );
}

// 리벤지 목록 업데이트 - 복수 대상 제거!!
VOID UserManager::RemoveRevenge( User *pUser )
{
	// 이 사용자의 복수 대상들 정보를 제거한다
	STRUCT_REVENGE_INFO*	pRevenge = pUser->GetFirstRevenge();
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP_ITER	iterAtom;
	User					*pOnlineUser = NULL;

	// 리벤지 목록 데이터에서 이 사용자의 친구들을 찾는다
	if( pRevenge )
	{
		do
		{
			// 리벤지 정보 있냐?
			iter = m_mapUserRevenge.find( pRevenge->dwRevengeCharGuid );
			if( iter != m_mapUserRevenge.end() )
			{
				// 데이터 있으면
				iterAtom = (*iter).second.find( pUser->GetCharGuid() );

				if( iterAtom != (*iter).second.end() )
				{
					// 이놈을 복수 대상 삼은 나의 데이터도 있으면 지워준다!
					(*iter).second.erase( iterAtom );
					// 지우고 났더니 맵이 비었으면 맵도 지워준다!
					if( (*iter).second.empty() )
					{
						m_mapUserRevenge.erase( iter );
					}
					// 이놈이 온라인이면
					if( pRevenge->sPacketInfo.bOnline )
					{
						// 실제 접속중인지 찾아서 온라인 체크 풀어준다
						pOnlineUser = FindUserByCharGuid( pRevenge->dwRevengeCharGuid );
						if( pOnlineUser )
						{
							pOnlineUser->RemoveOnlineRevenge( pUser );
						}
					}
				}
				else
				{
					// 데이터 없으면 버그
					MessageOut( eCRITICAL_LOG, "[%s] RemoveRevenge Error - No %s FriendAtomTable", pUser->GetCharName().c_str(), pRevenge->sPacketInfo.ptszRevengeName );
				}
			}
			else
			{
				// 데이터 없으면 버그
				MessageOut( eCRITICAL_LOG, "[%s] RemoveRevenge Error - No %s RevengeTable", pUser->GetCharName().c_str(), pRevenge->sPacketInfo.ptszRevengeName );
				continue;
			}
		}while( pRevenge = pUser->GetNextRevenge() );
	}

	// User가 삭제되면서 OnlineFriend 정보도 같이 삭제되므로 굳이 이 User 클래스에 있는 m_mapOnlineFriend를 삭제할 필요는 없다
//	MessageOut( eFULL_LOG, "[%s] RemoveFriend Complete", pUser->GetCharName().c_str() );
}

// 암호화 사용자 추가
VOID UserManager::AddCryptoUser( User *pUser )
{
	m_vecUserCrypt.push_back( pUser );

	pUser->SetCryptTime( GetTickCount() );
}

VOID UserManager::UpdateCryptoUser()
{
	CRYPTO_USER_VECTOR_ITER	iter;
	User					*pUser;
	DWORD					dwTickCount = GetTickCount();

	if( m_vecUserCrypt.empty() )
	{
		return;
	}

	for( iter = m_vecUserCrypt.begin(); iter < m_vecUserCrypt.end(); iter++ )
	{
		pUser = (*iter);

		// 암호화가 끝난 놈이면 여기 넣어둘 필요가 없다
		if( !pUser->IsCrypting() )
		{
			m_vecUserCrypt.erase( iter );
			continue;
		}

		// 시간 오버되면 강제종료
		if( pUser->ISCryptTimeOut( dwTickCount ) )
		{
			MessageOut( eFULL_LOG, "[GUID:%u] User Crypt TimeOut", pUser->GetGUID() );
			pUser->Logout();	// Logout에서 iter도 제거한다
		}
	}
}

// 채팅금지자 목록에 추가
VOID UserManager::InsertChatBlockUser( User *pUser, DWORD dwRemainTime )
{
	m_mapChatBlocked.insert( CHATBLOCK_USER_MAP_PAIR( pUser->GetCharGuid(), pUser ) );

	pUser->SetChatBlock( dwRemainTime );

	MessageOut( eFULL_LOG, "InsertChatBlockTime[%u]", pUser->GetCharGuid() );
}

// 채팅금지자 목록에서 제거
BOOL UserManager::RemoveChatBlockUser( User *pUser )
{
	CHATBLOCK_USER_MAP_ITER	iter = m_mapChatBlocked.find( pUser->GetCharGuid() );

	if( iter != m_mapChatBlocked.end() )
	{
		User *pUser = (*iter).second;

		pUser->FreeChatBlock();
		m_mapChatBlocked.erase( iter );

		MessageOut( eFULL_LOG, "RemoveChatBlockTime[%u]", pUser->GetCharGuid() );
		return TRUE;
	}
	MessageOut( eFULL_LOG, "RemoveChatBlockTime Failed[%u]", pUser->GetCharGuid() );
	return FALSE;
}

// 채팅금지자 상태 업데이트
VOID UserManager::UpdateChatBlockUser()
{
	CHATBLOCK_USER_MAP_ITER	iter;
	User					*pUser;
	STLX_VECTOR<User*>		vecRemoveUser;
	STLX_VECTOR<User*>::iterator	iter_vec;

	vecRemoveUser.clear();

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

	for( iter = m_mapChatBlocked.begin(); iter != m_mapChatBlocked.end(); ++iter )
	{
		pUser = (*iter).second;

		if( !pUser )
		{
			MessageOut( eCRITICAL_LOG, "UserManager ChatBlock Update Error - User NULL" );
			break;
		}

        //__NA_000313_20070223_REALTIME_CHAT_BLOCK
		if( !pUser->UpdateBlockTime() )
		{
			vecRemoveUser.push_back( pUser );
		}
	}

	for( iter_vec = vecRemoveUser.begin(); iter_vec != vecRemoveUser.end(); ++iter_vec )
	{
		pUser = (*iter_vec);
		if( !pUser )
		{
			continue;
		}
		RemoveChatBlockUser( pUser );
	}

	MessageOut( eFULL_LOG, "UpdateChatBlockTime[%u]", m_mapChatBlocked.size() );
}

// 친구 정보 메모리 할당
STRUCT_FRIEND_INFO* UserManager::AllocFriendInfo()
{
	return (STRUCT_FRIEND_INFO*)m_pFriendPool->Alloc();
}

// 친구 정보 메모리 반환
VOID UserManager::FreeFriendInfo( STRUCT_FRIEND_INFO *pFriendInfo )
{
	if( !pFriendInfo )
	{
		MessageOut( eCRITICAL_LOG, "FreeFriendInfo Error - No FriendInfo Pointer" );
		return;
	}
	memset( pFriendInfo, 0, sizeof(STRUCT_FRIEND_INFO) );
	m_pFriendPool->Free( pFriendInfo );
	pFriendInfo = NULL;
}

// 리벤지 정보 메모리 할당
STRUCT_REVENGE_INFO* UserManager::AllocRevengeInfo()
{
	return (STRUCT_REVENGE_INFO*)m_pRevengePool->Alloc();
}

// 리벤지 정보 메모리 반환
VOID UserManager::FreeRevengeInfo( STRUCT_REVENGE_INFO *pRevengeInfo )
{
	if( !pRevengeInfo )
	{
		MessageOut( eCRITICAL_LOG, "FreeRevengeInfo Error - No RevengeInfo Pointer" );
		return;
	}
	memset( pRevengeInfo, 0, sizeof(STRUCT_REVENGE_INFO) );
	m_pRevengePool->Free( pRevengeInfo );
	pRevengeInfo = NULL;
}

// 차단 정보 메모리 할당
STRUCT_BLOCK_INFO* UserManager::AllocBlockInfo()
{
	return (STRUCT_BLOCK_INFO*)m_pBlockPool->Alloc();
}

// 차단 정보 메모리 반환
VOID UserManager::FreeBlockInfo( STRUCT_BLOCK_INFO *pBlockInfo )
{
	if( !pBlockInfo )
	{
		MessageOut( eCRITICAL_LOG, "FreeBlockInfo Error - No FriendInfo Pointer" );
		return;
	}
	memset( pBlockInfo, 0, sizeof(STRUCT_BLOCK_INFO) );
	m_pBlockPool->Free( pBlockInfo );
	pBlockInfo = NULL;
}

VOID UserManager::AddUser( DWORD dwGuid, User *pUser )
{
	m_pGuidUserList->Add( dwGuid, pUser );

	// 캐릭터 이름이 있으면 CharNameUserList에도 추가
	if( !pUser->GetCharName().empty() )
	{
//		string strName = pUser->GetCharName();
//		transform( strName.begin(), strName.end(), strName.begin(), toupper );
		m_pCharNameUserList->Add( pUser->GetCharName(), pUser );
//#ifdef _DEBUG
//		MessageOut( eFULL_LOG, "UserManager::AddUser(Name)[%s]", strName.c_str() );
//#endif
	}

	// 캐릭터 guid 있으면 여기도 추가
	if( pUser->GetCharGuid() )
	{
		m_pCharGuidList->Add( pUser->GetCharGuid(), pUser );

        Lotto::GetInstance().OnPlayerJoin(pUser->GetCharGuid());
	}

}

VOID UserManager::RemoveUser( DWORD dwGuid )
{
	User *pUser = m_pGuidUserList->Find( dwGuid );
	if( pUser )
	{
		m_pGuidUserList->Remove( dwGuid );

		// 캐릭터 이름이 있으면 CharNameUserList에서도 삭제
		if( !pUser->GetCharName().empty() )
		{
			m_pCharNameUserList->Remove( pUser->GetCharName() );
		}

		if( pUser->GetCharGuid() )
		{
			m_pCharGuidList->Remove( pUser->GetCharGuid() );

            Lotto::GetInstance().OnPlayerLeave(pUser->GetCharGuid());
		}
	}
}

BOOL UserManager::SetUserCharGuid( DWORD dwGuid, DWORD dwCharGuid )
{
	User *pUser = m_pGuidUserList->Find( dwGuid );
	if( !pUser )
		return FALSE;

	// 이미 CharGuid리스트에 있는 경우 삭제
	if( m_pCharGuidList->Find( pUser->GetCharGuid() ) )
	{
		m_pCharGuidList->Remove( pUser->GetCharGuid() );

        Lotto::GetInstance().OnPlayerLeave(pUser->GetCharGuid());
        MessageOut( eFULL_LOG,  "Prev Character(%u)  ", pUser->GetCharGuid() );
	}

	// 새 캐릭터 이름 셋팅
	pUser->SetCharGuid( dwCharGuid );
	
	// CharGuid 리스트에 추가
	m_pCharGuidList->Add( dwCharGuid, pUser );

    Lotto::GetInstance().OnPlayerJoin(dwCharGuid);

    MessageOut( eFULL_LOG, "CharGuid(%u)", dwCharGuid );

	return TRUE;
}

BOOL UserManager::SetUserCharName( DWORD dwGuid, std::string strCharName )
{
	User *pUser = m_pGuidUserList->Find( dwGuid );
	if( !pUser ) return FALSE;

	MessageOut( eFULL_LOG,  "Guid(%u)CharGuid(%u)Name(%s)", dwGuid, pUser->GetCharGuid(), pUser->GetCharName().c_str() );

	// 새 캐릭터 이름 셋팅
	pUser->SetCharName( strCharName );

//	// CharName 리스트에 추가
//	transform( strCharName.begin(), strCharName.end(), strCharName.begin(), toupper );

	// 이미 CharName리스트에 있는 경우 삭제
	if( m_pCharNameUserList->Find( strCharName ) )
	{
		m_pCharNameUserList->Remove( strCharName );
		MessageOut( eFULL_LOG,  "Prev Character(%s)  ", pUser->GetCharName().c_str() );
	}
	m_pCharNameUserList->Add( strCharName, pUser );
#ifdef _DEBUG
	MessageOut( eFULL_LOG, "UserManager::AddUser(Name)[%s]", strCharName.c_str() );
#endif
//	m_pCharNameUserList->Add( strCharName, pUser );

	MessageOut(eFULL_LOG,  "Select Character(%s)", pUser->GetCharName().c_str() );

	return TRUE;
}

BOOL UserManager::UnSetCharGuid( DWORD dwGuid )
{
	User *pUser = m_pCharGuidList->Find( dwGuid );
	if( !pUser ) return FALSE;

	// CharGuid 리스트에서 삭제
	m_pCharGuidList->Remove( pUser->GetCharGuid() );

    Lotto::GetInstance().OnPlayerLeave(pUser->GetCharGuid());

	return TRUE;
}

BOOL UserManager::UnsetUserCharName( DWORD dwGuid )
{
	User *pUser = m_pGuidUserList->Find( dwGuid );
	if( !pUser ) return FALSE;

	// CharName 리스트에서 삭제
	m_pCharNameUserList->Remove( pUser->GetCharName() );
	
	return TRUE;
}

User* UserManager::FindUser( DWORD dwGuid )
{
	return m_pGuidUserList->Find( dwGuid );
}

User* UserManager::FindUserByCharGuid( DWORD CharGuid )
{
	return m_pCharGuidList->Find( CharGuid );
}

User* UserManager::FindUser( std::string strCharName )
{
//	transform( strCharName.begin(), strCharName.end(), strCharName.begin(), toupper );
//#ifdef _DEBUG
//	MessageOut( eFULL_LOG, "UserManager::FindUser[%s]", strCharName.c_str() );
//#endif
	return m_pCharNameUserList->Find( strCharName );
}

VOID UserManager::SendToAll( BYTE *pMsg, WORD wSize )
{
	m_pGuidUserList->SendToAll( pMsg, wSize );
}

DWORD UserManager::GetNumberOfGuidUsers()
{
	return m_pGuidUserList->GetNumberOfUsers();
}

DWORD UserManager::GetNumberOfCharNameUsers()
{
	return m_pCharNameUserList->GetNumberOfUsers();
}

BOOL UserManager::SetUserCharLevel(DWORD dwGuid, LEVELTYPE Level)
{
	User *pUser = m_pGuidUserList->Find( dwGuid );
	if( !pUser ) return FALSE;

	// 새 캐릭터 이름 셋팅
	pUser->SetCharLevel(Level);

	return TRUE;
}

BOOL UserManager::SetUserCharClass(DWORD dwGuid, BYTE byClass)
{
    User *pUser = m_pGuidUserList->Find( dwGuid );
    if( !pUser ) return FALSE;

    pUser->SetCharClass( byClass );

    return TRUE;
}

#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
BOOL UserManager::SetUserCharGender(DWORD guid, BYTE gender)
{
    User* user = m_pGuidUserList->Find(guid);
    if(user == NULL) {
        return FALSE; 
    }

    user->SetCharGender(gender);

    return TRUE;
}
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

//__NA_000313_20070223_REALTIME_CHAT_BLOCK