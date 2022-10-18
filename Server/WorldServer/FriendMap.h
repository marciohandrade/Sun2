#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING

/*

�ۼ���: ������
������: 2009-11-04

�����Ѽ���: User Ŭ������ ģ�� �� ���� �κ��� �������� �Űܿ´�.

*/

#pragma once

typedef STLX_MAP<DWORD, STRUCT_FRIEND_INFO*>		FRIEND_INFO_MAP;
typedef FRIEND_INFO_MAP::iterator					FRIEND_INFO_MAP_ITER;
typedef std::pair< DWORD, STRUCT_FRIEND_INFO* >		FRIEND_INFO_MAP_PAIR;

class FriendMap
{
public:
	FriendMap(void);
	~FriendMap(void);

public:
	inline BYTE				GetFriendNum()  { return (BYTE)m_mapFriend.size(); }
	inline VOID				clear()			{ m_mapFriend.clear();};
	inline bool				empty()			{ return m_mapFriend.empty();};
	inline FRIEND_INFO_MAP_ITER				begin()			{ return m_mapFriend.begin();};
	inline FRIEND_INFO_MAP_ITER				end()			{ return m_mapFriend.end();};

public:	
	STRUCT_FRIEND_INFO*		GetFirstFriend();
	STRUCT_FRIEND_INFO*		GetNextFriend();
	BOOL					AddFriendInfo( STRUCT_FRIEND_INFO* pFriend );
	STRUCT_FRIEND_INFO*		FindFriend( DWORD dwCharGuid );
	STRUCT_FRIEND_INFO*		FindFriend( TCHAR* ptszFriendName );
	BOOL					RemoveFriend( DWORD dwGuid );
	VOID					init();
	VOID					Clear();

private:
	FRIEND_INFO_MAP			m_mapFriend;				// ģ�� ������ ��
	FRIEND_INFO_MAP_ITER	m_iterNowFriend;	// GetFirst...(), GetNext...() ���� ���.
};

#endif //_NA_0_20091104_FIRENDMAP_REFACTORYING
