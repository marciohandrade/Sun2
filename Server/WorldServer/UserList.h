#pragma once

#include <Singleton.h>
#include <map>
#include <hash_map>
#include <string>

class User;

// DWORD(GUID)를 키로 하는 유저맵
typedef stdext::hash_map<DWORD, User*>		GUSER_MAP;
typedef GUSER_MAP::iterator					GUSER_MAP_ITER;
typedef std::pair<DWORD, User*>				GUSER_MAP_PAIR;

// string(CharName)을 키로 하는 유저맵
typedef std::map<std::string, User*>		CUSER_MAP;
typedef CUSER_MAP::iterator					CUSER_MAP_ITER;
typedef std::pair<std::string, User*>		CUSER_MAP_PAIR;

// GUID, CharName을 키로 사용하는 유져 맵
class UserList : public util::Singleton<UserList>
{
public:
	UserList();
	virtual ~UserList();

	VOID					Init();
	VOID					Release();
	VOID					Add( User *pUser );
	VOID					Remove( User *pUser );
	User*					Find( DWORD dwGUID );
	User*					Find( std::string charName );
	VOID					SendToAll( BYTE *pMsg, WORD wSize );

private:
	GUSER_MAP				m_gmapUsers;		// GUID 유저맵
	CUSER_MAP				m_cmapUsers;		// CharName 유저맵
};