#pragma once

#include <Singleton.h>
#include <hash_map>

class User;

typedef STLX_HASH_MAP<DWORD, User*>			WAITING_USER_MAP;
typedef WAITING_USER_MAP::iterator			WAITING_MAP_ITER;
typedef std::pair<DWORD, User*>				WAITING_MAP_PAIR;

// AuthID를 키로갖는 리스트
class WaitingUserList : public util::Singleton<WaitingUserList>
{
public:
	WaitingUserList(void);
	~WaitingUserList(void);

	VOID					Init();
	VOID					Release();
	VOID					Update();
	VOID					AddUser( DWORD dwAuthUserID, User *pUser );
	VOID					RemoveUser( DWORD dwAuthUserID );
	User*					FindUserWithAuthID( DWORD dwAuthUserID );
	User*					FindUserWithGuid( DWORD dwGuid );

private:
	WAITING_USER_MAP		m_mapUsers;
	WAITING_USER_MAP		m_mapGuidUsers;
};
