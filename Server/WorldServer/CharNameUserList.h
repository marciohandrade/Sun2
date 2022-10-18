#pragma once

#include <map>
#include <string>

class User;

// string(CharName)을 키로 하는 유저맵
typedef STLX_MAP<std::string, User*>		CUSER_MAP;
typedef CUSER_MAP::iterator					CUSER_MAP_ITER;
typedef std::pair<std::string, User*>		CUSER_MAP_PAIR;

class CharNameUserList
{
public:
	CharNameUserList(void);
	~CharNameUserList(void);

	VOID					Init();
	VOID					Release();
	VOID					Add( std::string strCharName, User *pUser );
	VOID					Remove( std::string strCharName );
	User*					Find( std::string charName );
//	VOID					SendToAll( BYTE *pMsg, WORD wSize );
	inline DWORD			GetNumberOfUsers() { return (DWORD)m_mapUsers.size(); }

private:
	CUSER_MAP				m_mapUsers;		// CharName 유저맵
};
