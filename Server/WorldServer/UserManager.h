#pragma once

#include <Singleton.h>

// ģ�� ���� ���� include
#include <map>
#include <vector>

#include <MemoryPoolFactory.h>

class User;

struct STRUCT_FRIEND_INFO;
struct STRUCT_BLOCK_INFO;
struct STRUCT_REVENGE_INFO;

typedef STLX_MAP<DWORD, User*>							FRIEND_ATOM_MAP;
typedef FRIEND_ATOM_MAP::iterator						FRIEND_ATOM_MAP_ITER;
typedef std::pair< DWORD, User* >						FRIEND_ATOM_MAP_PAIR;

typedef FRIEND_ATOM_MAP									CHATBLOCK_USER_MAP;
typedef FRIEND_ATOM_MAP_ITER							CHATBLOCK_USER_MAP_ITER;
typedef FRIEND_ATOM_MAP_PAIR							CHATBLOCK_USER_MAP_PAIR;

typedef STLX_MAP<DWORD, FRIEND_ATOM_MAP>				FRIEND_USER_MAP;
typedef FRIEND_USER_MAP::iterator						FRIEND_USER_MAP_ITER;
typedef std::pair< DWORD, FRIEND_ATOM_MAP >				FRIEND_USER_MAP_PAIR;

typedef STLX_VECTOR<User*>								CRYPTO_USER_VECTOR;
typedef CRYPTO_USER_VECTOR::iterator					CRYPTO_USER_VECTOR_ITER;

class User;
class GuidUserList;
class CharNameUserList;

class UserManager : public util::Singleton<UserManager>
{
	friend class Channel;

public:
	UserManager(void);
	~UserManager(void);

	VOID					Init();
	VOID					Release();
	BOOL					SetUserCharGuid( DWORD dwGuid, DWORD dwCharGuid );
	BOOL					SetUserCharName( DWORD dwGuid, std::string strCharName );
	BOOL					UnSetCharGuid( DWORD dwGuid );
	BOOL					UnsetUserCharName( DWORD dwGuid );
	User*					FindUser( DWORD dwGuid );
	User*					FindUser( std::string strCharName );
	User*					FindUserByCharGuid( DWORD CharGuid );
	VOID					SendToAll( BYTE *pMsg, WORD wSize );

	DWORD					GetNumberOfGuidUsers();
	DWORD					GetNumberOfCharNameUsers();

	// ģ�� ����
	BOOL					AddNewFriend( DWORD FriendCharGuid, User *pUser );	// ���� �߰��� ģ���� ��Ͽ� �߰�
	BOOL					AddFriend( User* pUser );		// �� ģ���� ��� �߰��ϰ�, pUser���� �¶����� ģ�� ���� �־��ش�
	VOID					RemoveNewFriend( DWORD FriendCharGuid, User *pUser );	// ���� ������ ģ���� ��Ͽ��� ����
	VOID					RemoveFriend( User* pUser );	// �� ģ���� ��� �����Ѵ�.

	// ������ ����
	BOOL					AddNewRevenge( DWORD RevengeCharGuid, User *pUser );	// ���� �߰��� ���� ����� ��Ͽ� �߰�
	BOOL					AddRevenge( User* pUser );		// �� ���� ���� ��� �߰��ϰ�, pUser���� �¶����� ������ ���� �־��ش�
	VOID					RemoveNewRevenge( DWORD RevengeCharGuid, User *pUser );	// ���� ������ ������ ����� ��Ͽ��� ����
	VOID					RemoveRevenge( User* pUser );	// �� ������ ��� �����Ѵ�.

	// ��ȣȭ ����
	VOID					AddCryptoUser( User *pUser );		// ��ȣȭ ��� �߰�
	VOID					UpdateCryptoUser();					// ��ȣȭ ��� �߿� ���� ���� ������ �����ϸ� �ҹ� ������ �����ϰ� ������� �Ѵ�

	// ä�� ���� ����
	VOID					InsertChatBlockUser( User *pUser, DWORD dwRemainTime );
	BOOL					RemoveChatBlockUser(User *pUser);
	VOID					UpdateChatBlockUser();


	// ģ��/������/���� ����
	STRUCT_FRIEND_INFO*		AllocFriendInfo();
	VOID					FreeFriendInfo( STRUCT_FRIEND_INFO *pFriendInfo );
	STRUCT_REVENGE_INFO*	AllocRevengeInfo();
	VOID					FreeRevengeInfo( STRUCT_REVENGE_INFO *pRevengeInfo );
	STRUCT_BLOCK_INFO*		AllocBlockInfo();
	VOID					FreeBlockInfo( STRUCT_BLOCK_INFO *pBlockInfo );

	BOOL					SetUserCharLevel(DWORD dwGuid, LEVELTYPE CharLevel);
    BOOL                    SetUserCharClass(DWORD dwGuid, BYTE byClass);
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BOOL                    SetUserCharGender(DWORD guid, BYTE gender);
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

private:
	VOID					AddUser( DWORD dwGuid, User *pUser );
	VOID					RemoveUser( DWORD dwGuid );

	GuidUserList			*m_pGuidUserList;
	GuidUserList			*m_pCharGuidList;
	CharNameUserList		*m_pCharNameUserList;	// Add�� Find���� �ݵ�� �빮�ڷ� �ٲپ� ����Ѵ� - ������ ��

	FRIEND_USER_MAP			m_mapUserFriend;	// ģ�� ���� ����� ���� ���޿� ��
	FRIEND_USER_MAP			m_mapUserRevenge;	// ���� ����� ����� ���� ���޿� ��

	CRYPTO_USER_VECTOR		m_vecUserCrypt;		// ��ȣ Ű ���� ����� ������ ������ ��
	CHATBLOCK_USER_MAP		m_mapChatBlocked;	// ä�ñ����� ����� ���

	util::CMemoryPoolFactory<STRUCT_FRIEND_INFO>	*m_pFriendPool;
	util::CMemoryPoolFactory<STRUCT_BLOCK_INFO>		*m_pBlockPool;
	util::CMemoryPoolFactory<STRUCT_REVENGE_INFO>	*m_pRevengePool;
};
