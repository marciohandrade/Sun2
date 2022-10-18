#include "StdAfx.h"
#include ".\usermanager.h"
#include "User.h"
#include "GuidUserList.h"
#include "CharNameUserList.h"
#include <string>
#include <algorithm>
#include <assert.h>

// ģ�� ����
#include <struct.h>

// ��ȣȭ ���� �������� ����
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

// FriendCharGuid�� ģ���� ������ pUser ���� �߰�
BOOL UserManager::AddNewFriend( DWORD FriendCharGuid, User* pUser )
{
	// �� ������� ģ���� ��� �߰�
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapFriendAtom;

	// ģ���� ������ �ٷ� ������ ���� �� �ֵ��� �� ģ�� ������ ����Ѵ�
	iter = m_mapUserFriend.find( FriendCharGuid );
	if( iter != m_mapUserFriend.end() )
	{
		// ������ ������ �� �ڿ� �߰�
		(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
	}
	else
	{
		// ������ ������ ���� ����� �ִ´�
		mapFriendAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
		m_mapUserFriend.insert( FRIEND_USER_MAP_PAIR( FriendCharGuid, mapFriendAtom ) );
	}

	// �� ģ���� �¶����̶�� ģ�� �ν��Ͻ��� '���� �ʸ� ģ�� �Ծ���!'�� �����Ų��
	// ���� pFriend�� �α׾ƿ��ϸ� pUser�� �� �� �ִ�
	User* pFriend = FindUserByCharGuid( FriendCharGuid );

	if( pFriend )
	{
		// �� ģ���� ���� �����ߴٸ� �˷����� �ʴ´�
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
	// �� ������� ģ���� ��� �߰�
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFriendMap()->GetFirstFriend();
#else
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFirstFriend();	
#endif
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapFriendAtom;

	// ģ���� ������ �ٷ� ������ ���� �� �ֵ��� �� ģ�� ������ ����Ѵ�
	if( pFriend )
	{
		do
		{
			iter = m_mapUserFriend.find( pFriend->dwFriendGuid );
			if( iter != m_mapUserFriend.end() )
			{
				// ������ ������ �� �ڿ� �߰�
				(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
			}
			else
			{
				// ������ ������ ���� ����� �ִ´�
				mapFriendAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
				m_mapUserFriend.insert( FRIEND_USER_MAP_PAIR( pFriend->dwFriendGuid, mapFriendAtom ) );
			}
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
		}while( pFriend = pUser->GetFriendMap()->GetNextFriend() );
#else
		}while( pFriend = pUser->GetNextFriend() );
#endif
		
	}

	// �̹� ��ϵ� ������ �߿� ���� ����� ������ ������ �־��ش�
	iter = m_mapUserFriend.find( pUser->GetCharGuid() );

	if( iter != m_mapUserFriend.end() )
	{
		// ������ �̰� �������� ���
		FRIEND_ATOM_MAP_ITER	iterAtom;

		if( !(*iter).second.empty() )
		{
			for( iterAtom = (*iter).second.begin(); iterAtom != (*iter).second.end(); iterAtom++ )
			{

				// ���� �� ������ �����ߴٸ� �˷����� �ʴ´�
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

	// ģ�� ��� �����Ϳ��� �� ������� ģ������ ã�´�
	iter = m_mapUserFriend.find( FriendCharGuid );
	if( iter != m_mapUserFriend.end() )
	{
		// ������ ������
		iterAtom = (*iter).second.find( pUser->GetCharGuid() );

		if( iterAtom != (*iter).second.end() )
		{
			// �̳��� ģ�� ���� ���� �����͵� ������ �����ش�!
			(*iter).second.erase( iterAtom );
			// ����� ������ ���� ������� �ʵ� �����ش�!
			if( (*iter).second.empty() )
			{
				m_mapUserFriend.erase( iter );
			}
		}
		else
		{
			// ������ ������ ����
			MessageOut( eCRITICAL_LOG, "[%s] RemoveNewFriend Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), FriendCharGuid );
			return;
		}
	}
	else
	{
		// ������ ������ ����
		MessageOut( eCRITICAL_LOG, "[%s] RemoveNewFriend Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), FriendCharGuid );
		return;
	}

	// �� ģ���� �¶����̶�� ���ʿ����� ������ ���� ��� �Ѵ�
	User* pFriend = FindUserByCharGuid( FriendCharGuid );

	if( pFriend )
	{
		pFriend->RemoveOnlineFriend( pUser );
	}

//	MessageOut( eFULL_LOG, "[%s] RemoveNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid );
}

// ģ�� ��� ������Ʈ - ģ�� ����!!
VOID UserManager::RemoveFriend( User *pUser )
{
	// �� ������� ģ���� ������ �����Ѵ�
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFriendMap()->GetFirstFriend();
#else
	STRUCT_FRIEND_INFO*		pFriend = pUser->GetFirstFriend();	
#endif	
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP_ITER	iterAtom;
	User					*pOnlineUser = NULL;

	// ģ�� ��� �����Ϳ��� �� ������� ģ������ ã�´�
	if( pFriend )
	{
		do
		{
			// ģ�� ���� �ֳ�?
			iter = m_mapUserFriend.find( pFriend->dwFriendGuid );
			if( iter != m_mapUserFriend.end() )
			{
				// ������ ������
				iterAtom = (*iter).second.find( pUser->GetCharGuid() );

				if( iterAtom != (*iter).second.end() )
				{
					// �̳��� ģ�� ���� ���� �����͵� ������ �����ش�!
					(*iter).second.erase( iterAtom );
					// ����� ������ ���� ������� �ʵ� �����ش�!
					if( (*iter).second.empty() )
					{
						m_mapUserFriend.erase( iter );
					}
					// �� ģ������ �¶����̸�
					if( pFriend->sPacketInfo.bOnline )
					{
						// ���� ���������� ã�Ƽ� �¶��� üũ Ǯ���ش�
						pOnlineUser = FindUserByCharGuid( pFriend->dwFriendGuid );
						if( pOnlineUser )
						{
							pOnlineUser->RemoveOnlineFriend( pUser );
						}
					}
				}
				else
				{
					// ������ ������ ����
					MessageOut( eCRITICAL_LOG, "[%s] RemoveFriend Error - No %s FriendAtomTable", pUser->GetCharName().c_str(), pFriend->sPacketInfo.ptszFriendName );
				}
			}
			else
			{
				// ������ ������ ����
				MessageOut( eCRITICAL_LOG, "[%s] RemoveFriend Error - No %s FriendTable", pUser->GetCharName().c_str(), pFriend->sPacketInfo.ptszFriendName );
				continue;
			}
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
		}while( pFriend = pUser->GetFriendMap()->GetNextFriend() );
#else
		}while( pFriend = pUser->GetNextFriend() );
#endif		
	}

	// User�� �����Ǹ鼭 OnlineFriend ������ ���� �����ǹǷ� ���� �� User Ŭ������ �ִ� m_mapOnlineFriend�� ������ �ʿ�� ����
//	MessageOut( eFULL_LOG, "[%s] RemoveFriend Complete", pUser->GetCharName().c_str() );
}

////////////////////////////////////////
// ������ ����
// RevengeCharGuid�� ���� ������� ������ pUser ���� �߰�
BOOL UserManager::AddNewRevenge( DWORD RevengeCharGuid, User* pUser )
{
	// �� ������� ���� ���� ��� �߰�
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapRevengeAtom;

	// �׳��� ������ �ٷ� ������ ���� �� �ֵ��� �� ������ ������ ����Ѵ�
	iter = m_mapUserRevenge.find( RevengeCharGuid );
	if( iter != m_mapUserRevenge.end() )
	{
		// ������ ������ �� �ڿ� �߰�
		(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
	}
	else
	{
		// ������ ������ ���� ����� �ִ´�
		mapRevengeAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
		m_mapUserRevenge.insert( FRIEND_USER_MAP_PAIR( RevengeCharGuid, mapRevengeAtom ) );
	}

	// �̳��� �¶����̶�� ������ �ν��Ͻ��� '���� �ʸ� ���� ������� ���ߴ�!'�� �����Ų��
	// ���� pRevenge�� �α׾ƿ��ϸ� pUser�� �� �� �ִ�
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
	// �� ������� ���� ���� ��� �߰�
	STRUCT_REVENGE_INFO*	pRevenge = pUser->GetFirstRevenge();
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP	mapRevengeAtom;

	// ģ���� ������ �ٷ� ������ ���� �� �ֵ��� �� ģ�� ������ ����Ѵ�
	if( pRevenge )
	{
		do
		{
			iter = m_mapUserRevenge.find( pRevenge->dwRevengeCharGuid );
			if( iter != m_mapUserRevenge.end() )
			{
				// ������ ������ �� �ڿ� �߰�
				(*iter).second.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
			}
			else
			{
				// ������ ������ ���� ����� �ִ´�
				mapRevengeAtom.insert( FRIEND_ATOM_MAP_PAIR( pUser->GetCharGuid(), pUser ) );
				m_mapUserRevenge.insert( FRIEND_USER_MAP_PAIR( pRevenge->dwRevengeCharGuid, mapRevengeAtom ) );
			}
		}while( pRevenge = pUser->GetNextRevenge() );
	}

	// �̹� ��ϵ� ������ �߿� ���� ����� ������ ������ �־��ش�
	iter = m_mapUserRevenge.find( pUser->GetCharGuid() );

	if( iter != m_mapUserRevenge.end() )
	{
		// ������ �̰� �������� ���
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

	// ������ ��� �����Ϳ��� �� ������� ���� ����� ã�´�
	iter = m_mapUserRevenge.find( RevengeCharGuid );
	if( iter != m_mapUserRevenge.end() )
	{
		// ������ ������
		iterAtom = (*iter).second.find( pUser->GetCharGuid() );

		if( iterAtom != (*iter).second.end() )
		{
			// �̳��� ���� ��� ���� ���� �����͵� ������ �����ش�!
			(*iter).second.erase( iterAtom );
			// ����� ������ ���� ������� �ʵ� �����ش�!
			if( (*iter).second.empty() )
			{
				m_mapUserRevenge.erase( iter );
			}
		}
		else
		{
			// ������ ������ ����
			MessageOut( eCRITICAL_LOG, "[%s] RemoveNewRevenge Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), RevengeCharGuid );
			return;
		}
	}
	else
	{
		// ������ ������ ����
		MessageOut( eCRITICAL_LOG, "[%s] RemoveNewRevenge Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), RevengeCharGuid );
		return;
	}

	// �̳��� �¶����̶�� ���ʿ����� ������ ���� ��� �Ѵ�
	User* pRevenge = FindUserByCharGuid( RevengeCharGuid );

	if( pRevenge )
	{
		pRevenge->RemoveOnlineRevenge( pUser );
	}

//	MessageOut( eFULL_LOG, "[%s] RemoveNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid );
}

// ������ ��� ������Ʈ - ���� ��� ����!!
VOID UserManager::RemoveRevenge( User *pUser )
{
	// �� ������� ���� ���� ������ �����Ѵ�
	STRUCT_REVENGE_INFO*	pRevenge = pUser->GetFirstRevenge();
	FRIEND_USER_MAP_ITER	iter;
    FRIEND_ATOM_MAP_ITER	iterAtom;
	User					*pOnlineUser = NULL;

	// ������ ��� �����Ϳ��� �� ������� ģ������ ã�´�
	if( pRevenge )
	{
		do
		{
			// ������ ���� �ֳ�?
			iter = m_mapUserRevenge.find( pRevenge->dwRevengeCharGuid );
			if( iter != m_mapUserRevenge.end() )
			{
				// ������ ������
				iterAtom = (*iter).second.find( pUser->GetCharGuid() );

				if( iterAtom != (*iter).second.end() )
				{
					// �̳��� ���� ��� ���� ���� �����͵� ������ �����ش�!
					(*iter).second.erase( iterAtom );
					// ����� ������ ���� ������� �ʵ� �����ش�!
					if( (*iter).second.empty() )
					{
						m_mapUserRevenge.erase( iter );
					}
					// �̳��� �¶����̸�
					if( pRevenge->sPacketInfo.bOnline )
					{
						// ���� ���������� ã�Ƽ� �¶��� üũ Ǯ���ش�
						pOnlineUser = FindUserByCharGuid( pRevenge->dwRevengeCharGuid );
						if( pOnlineUser )
						{
							pOnlineUser->RemoveOnlineRevenge( pUser );
						}
					}
				}
				else
				{
					// ������ ������ ����
					MessageOut( eCRITICAL_LOG, "[%s] RemoveRevenge Error - No %s FriendAtomTable", pUser->GetCharName().c_str(), pRevenge->sPacketInfo.ptszRevengeName );
				}
			}
			else
			{
				// ������ ������ ����
				MessageOut( eCRITICAL_LOG, "[%s] RemoveRevenge Error - No %s RevengeTable", pUser->GetCharName().c_str(), pRevenge->sPacketInfo.ptszRevengeName );
				continue;
			}
		}while( pRevenge = pUser->GetNextRevenge() );
	}

	// User�� �����Ǹ鼭 OnlineFriend ������ ���� �����ǹǷ� ���� �� User Ŭ������ �ִ� m_mapOnlineFriend�� ������ �ʿ�� ����
//	MessageOut( eFULL_LOG, "[%s] RemoveFriend Complete", pUser->GetCharName().c_str() );
}

// ��ȣȭ ����� �߰�
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

		// ��ȣȭ�� ���� ���̸� ���� �־�� �ʿ䰡 ����
		if( !pUser->IsCrypting() )
		{
			m_vecUserCrypt.erase( iter );
			continue;
		}

		// �ð� �����Ǹ� ��������
		if( pUser->ISCryptTimeOut( dwTickCount ) )
		{
			MessageOut( eFULL_LOG, "[GUID:%u] User Crypt TimeOut", pUser->GetGUID() );
			pUser->Logout();	// Logout���� iter�� �����Ѵ�
		}
	}
}

// ä�ñ����� ��Ͽ� �߰�
VOID UserManager::InsertChatBlockUser( User *pUser, DWORD dwRemainTime )
{
	m_mapChatBlocked.insert( CHATBLOCK_USER_MAP_PAIR( pUser->GetCharGuid(), pUser ) );

	pUser->SetChatBlock( dwRemainTime );

	MessageOut( eFULL_LOG, "InsertChatBlockTime[%u]", pUser->GetCharGuid() );
}

// ä�ñ����� ��Ͽ��� ����
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

// ä�ñ����� ���� ������Ʈ
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

// ģ�� ���� �޸� �Ҵ�
STRUCT_FRIEND_INFO* UserManager::AllocFriendInfo()
{
	return (STRUCT_FRIEND_INFO*)m_pFriendPool->Alloc();
}

// ģ�� ���� �޸� ��ȯ
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

// ������ ���� �޸� �Ҵ�
STRUCT_REVENGE_INFO* UserManager::AllocRevengeInfo()
{
	return (STRUCT_REVENGE_INFO*)m_pRevengePool->Alloc();
}

// ������ ���� �޸� ��ȯ
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

// ���� ���� �޸� �Ҵ�
STRUCT_BLOCK_INFO* UserManager::AllocBlockInfo()
{
	return (STRUCT_BLOCK_INFO*)m_pBlockPool->Alloc();
}

// ���� ���� �޸� ��ȯ
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

	// ĳ���� �̸��� ������ CharNameUserList���� �߰�
	if( !pUser->GetCharName().empty() )
	{
//		string strName = pUser->GetCharName();
//		transform( strName.begin(), strName.end(), strName.begin(), toupper );
		m_pCharNameUserList->Add( pUser->GetCharName(), pUser );
//#ifdef _DEBUG
//		MessageOut( eFULL_LOG, "UserManager::AddUser(Name)[%s]", strName.c_str() );
//#endif
	}

	// ĳ���� guid ������ ���⵵ �߰�
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

		// ĳ���� �̸��� ������ CharNameUserList������ ����
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

	// �̹� CharGuid����Ʈ�� �ִ� ��� ����
	if( m_pCharGuidList->Find( pUser->GetCharGuid() ) )
	{
		m_pCharGuidList->Remove( pUser->GetCharGuid() );

        Lotto::GetInstance().OnPlayerLeave(pUser->GetCharGuid());
        MessageOut( eFULL_LOG,  "Prev Character(%u)  ", pUser->GetCharGuid() );
	}

	// �� ĳ���� �̸� ����
	pUser->SetCharGuid( dwCharGuid );
	
	// CharGuid ����Ʈ�� �߰�
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

	// �� ĳ���� �̸� ����
	pUser->SetCharName( strCharName );

//	// CharName ����Ʈ�� �߰�
//	transform( strCharName.begin(), strCharName.end(), strCharName.begin(), toupper );

	// �̹� CharName����Ʈ�� �ִ� ��� ����
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

	// CharGuid ����Ʈ���� ����
	m_pCharGuidList->Remove( pUser->GetCharGuid() );

    Lotto::GetInstance().OnPlayerLeave(pUser->GetCharGuid());

	return TRUE;
}

BOOL UserManager::UnsetUserCharName( DWORD dwGuid )
{
	User *pUser = m_pGuidUserList->Find( dwGuid );
	if( !pUser ) return FALSE;

	// CharName ����Ʈ���� ����
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

	// �� ĳ���� �̸� ����
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