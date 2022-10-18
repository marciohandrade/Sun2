#pragma once

#include <hash_map>
#include <Linkedlist.h>

class User;
class FieldInfo;
class Channnel;

typedef STLX_HASH_MAP<DWORD, User*>			USER_MAP;
typedef USER_MAP::iterator					USER_MAP_ITER;
typedef std::pair<DWORD, User*>				USER_MAP_PAIR;

// 기본적인 Zone 정의
class BaseZone
{
public:
	BaseZone(void);
	virtual ~BaseZone(void);

	VOID					Init();
	virtual VOID			Release();

	virtual BOOL			AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey=0 )=0;
	virtual BOOL			RemoveUser( DWORD dwGuid )=0;
	virtual BOOL			Send( BYTE *pMsg, WORD wSize )=0;

	VOID					AddLinkedZone( BaseZone *pZone );
	virtual inline VOID		SendToAll( BYTE *pMsg, WORD wSize )
	{
		Send( pMsg, wSize );
		SendLinkedZone( pMsg, wSize );
	}

	inline VOID				SetZoneType( eZONETYPE type ) { m_zoneType = type; }
	inline eZONETYPE		GetZoneType() { return m_zoneType; }
	inline VOID				SetKey( DWORD key ) { m_dwKey = key; }
	inline DWORD			GetKey() { return m_dwKey; }
	inline DWORD			GetNumberOfUsers() { return (DWORD)m_mapUsers.size(); }
	virtual VOID			SendToMe( BYTE *pMsg, WORD wSize )=0;
	inline VOID				SetMapCode( MAPCODE mapCode ) { m_MapCode = mapCode; }
	inline MAPCODE			GetMapCode() { return m_MapCode; }

    //add by __NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__ runtime community block/release
	void SetBlockZone(BOOL value) { m_bBlockZone = value; }
	BOOL IsBlockZone() { return m_bBlockZone; }

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    inline VOID             SetRoomKey(const DWORD key)   { room_key_ = key; }
    inline const DWORD      GetRoomKey()                  { return room_key_;}
#endif    // NA_004694_20120412_1202_PARTY_ROOM_KEY

protected:
	virtual VOID			SendLinkedZone( BYTE *pMsg, WORD wSize )=0;

public:
	USER_MAP m_mapUsers;
	eZONETYPE m_zoneType;
	util::CLinkedlist<BaseZone*> m_LinkedZone;
	DWORD m_dwKey;
	MAPCODE m_MapCode;
	BOOL m_bBlockZone;
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    DWORD room_key_;
#endif    // NA_004694_20120412_1202_PARTY_ROOM_KEY
};

// 일반적인 채팅 채널 처리용
class ChatZone : public BaseZone
{
public:
	ChatZone();
	virtual ~ChatZone();

	virtual BOOL			AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey=0 );
	virtual BOOL			RemoveUser( DWORD dwGuid );
	virtual BOOL			Send( BYTE *pMsg, WORD wSize );
	virtual VOID			SendToMe( BYTE *pMsg, WORD wSize ) { Send( pMsg, wSize ); }

	util::CLinkedlist<BaseZone*> &GetLinkedZoneList() { return m_LinkedZone; }

protected:
	virtual VOID			SendLinkedZone( BYTE *pMsg, WORD wSize );
};

// 시야 범위 처리용
class ViewPortZone : public BaseZone
{
public:
	ViewPortZone();
	virtual ~ViewPortZone();

	BOOL					AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey=0 );
	BOOL					RemoveUser( DWORD dwGuid );
	BOOL					Send( BYTE *pMsg, WORD wSize );
	VOID					SendToMe( BYTE *pMsg, WORD wSize );
	VOID					SetParentVillage( BaseZone* pZone ) { m_pParentVillage = pZone; }

	VOID					SendToAll( BYTE *pMsg, WORD wSize );

protected:
	VOID					SendLinkedZone( BYTE *pMsg, WORD wSize ) { __UNUSED((pMsg, wSize)); }

	BaseZone*				m_pParentVillage;	// 자신이 속한 마을의 ChatZone 인스턴스 포인터
};

typedef STLX_HASH_MAP<DWORD, ViewPortZone*>		VIEWPORT_MAP;
typedef VIEWPORT_MAP::iterator					VIEWPORT_MAP_ITER;
typedef std::pair<DWORD, ViewPortZone*>			VIEWPORT_MAP_PAIR;

typedef STLX_HASH_MAP<DWORD, BaseZone*>			BATTLE_MAP;
typedef BATTLE_MAP::iterator					BATTLE_MAP_ITER;
typedef std::pair<DWORD, BaseZone*>				BATTLE_MAP_PAIR;

// 마을 채널 처리용
class VillageZone : public ChatZone
{
public:
	VillageZone();
	virtual ~VillageZone();

	VOID					Init( MAPCODE MapCode, FieldInfo *pFieldInfo );
	VOID					Release();

	BOOL					AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey=0 );
	BOOL					RemoveUser( DWORD dwGuid );
	BOOL					Send( BYTE *pMsg, WORD wSize );
	VOID					SendToMe( BYTE *pMsg, WORD wSize );
//	inline MAPCODE			GetMapCode() { return m_MapCode; }
	DWORD					GetFieldCode();

	VOID					AddBattleLinkedZone( ChatZone *pZone );
	VOID					RemoveBattleLinkedZone( ChatZone *pZone );

protected:
	VOID					SendLinkedZone( BYTE *pMsg, WORD wSize );

//	MAPCODE					m_MapCode;
	FieldInfo				*m_pFieldInfo;
	VIEWPORT_MAP			m_mapZone;
	BATTLE_MAP				m_mapBattle;
};

// 파티/길드/채널 채팅 처리용
class CommChatZone : public ChatZone
{
public:
	CommChatZone();
	virtual ~CommChatZone();

	VOID					Release();

	BOOL					AddUser( DWORD dwGuid, User *pUser, DWORD dwRoomKey=0 );
	BOOL					RemoveUser( DWORD dwGuid );
	BYTE					GetPartyVoiceChatInfo( BYTE *pStatus, User *pTarget );

	inline TCHAR*			GetChatRoomName() { return m_ptszChatRoomName; }
	VOID					SetChatRoomName( TCHAR* ptszName );

	BOOL					GetFirstUserName( TCHAR* ptszName );
	BOOL					GetNextUserName( TCHAR* ptszName );

private:
	TCHAR					m_ptszChatRoomName[MAX_ROOMTITLE_LENGTH+1];	// 채팅방 이름
	USER_MAP_ITER			m_iterUserName;
};
