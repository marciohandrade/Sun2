#pragma once

#include <Singleton.h>

// 친구 관리 위해 include
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

	// 친구 관련
	BOOL					AddNewFriend( DWORD FriendCharGuid, User *pUser );	// 새로 추가된 친구를 목록에 추가
	BOOL					AddFriend( User* pUser );		// 내 친구들 목록 추가하고, pUser에게 온라인인 친구 정보 넣어준다
	VOID					RemoveNewFriend( DWORD FriendCharGuid, User *pUser );	// 새로 삭제한 친구를 목록에서 제거
	VOID					RemoveFriend( User* pUser );	// 내 친구들 목록 제거한다.

	// 리벤지 관련
	BOOL					AddNewRevenge( DWORD RevengeCharGuid, User *pUser );	// 새로 추가된 복수 대상을 목록에 추가
	BOOL					AddRevenge( User* pUser );		// 내 복수 대상들 목록 추가하고, pUser에게 온라인인 리벤지 정보 넣어준다
	VOID					RemoveNewRevenge( DWORD RevengeCharGuid, User *pUser );	// 새로 삭제한 리벤지 대상을 목록에서 제거
	VOID					RemoveRevenge( User* pUser );	// 내 리벤지 목록 제거한다.

	// 암호화 관련
	VOID					AddCryptoUser( User *pUser );		// 암호화 대상 추가
	VOID					UpdateCryptoUser();					// 암호화 대기 중에 응답 없이 게임을 진행하면 불법 유저로 간주하고 끊어줘야 한다

	// 채팅 금지 관련
	VOID					InsertChatBlockUser( User *pUser, DWORD dwRemainTime );
	BOOL					RemoveChatBlockUser(User *pUser);
	VOID					UpdateChatBlockUser();


	// 친구/리벤지/차단 관련
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
	CharNameUserList		*m_pCharNameUserList;	// Add와 Find에서 반드시 대문자로 바꾸어 사용한다 - 유의할 것

	FRIEND_USER_MAP			m_mapUserFriend;	// 친구 맺은 사용자 정보 전달용 맵
	FRIEND_USER_MAP			m_mapUserRevenge;	// 복수 대상인 사용자 정보 전달용 맵

	CRYPTO_USER_VECTOR		m_vecUserCrypt;		// 암호 키 변경 대상인 유저들 보관용 맵
	CHATBLOCK_USER_MAP		m_mapChatBlocked;	// 채팅금지된 사용자 목록

	util::CMemoryPoolFactory<STRUCT_FRIEND_INFO>	*m_pFriendPool;
	util::CMemoryPoolFactory<STRUCT_BLOCK_INFO>		*m_pBlockPool;
	util::CMemoryPoolFactory<STRUCT_REVENGE_INFO>	*m_pRevengePool;
};
