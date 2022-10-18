#pragma once

#include <hash_map>
#include <string>

class User;

// DWORD(GUID)�� Ű�� �ϴ� ������
typedef STLX_HASH_MAP<DWORD, User*>			GUSER_MAP;
typedef GUSER_MAP::iterator					GUSER_MAP_ITER;
typedef std::pair<DWORD, User*>				GUSER_MAP_PAIR;

class GuidUserList
{
public:
	GuidUserList(void);
	~GuidUserList(void);

	VOID					Init();
	VOID					Release();
	VOID					Add( DWORD dwGuid, User *pUser );
	VOID					Remove( DWORD dwGuid );
	User*					Find( DWORD dwGuid );
	VOID					SendToAll( BYTE *pMsg, WORD wSize );
	inline DWORD			GetNumberOfUsers() { return (DWORD)m_mapUsers.size(); }

private:
	GUSER_MAP				m_mapUsers;		// GUID ������
};