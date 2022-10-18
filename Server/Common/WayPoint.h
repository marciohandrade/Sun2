#pragma once

#include <hash_map>
#include <StructInPacket.h>

// 포탈 NPC의 웨이포인트 리스트에 등록되는 정보들.
// 이전 기획에서 플레이어가 이동하여 클릭한 NPC의 맵 정보를 저장하고, 포탈을 열수 있는 방식이었으나,
// 현재 포탈은 전체를 보여주고 이용 요금으로 갈수 있는 방식으로 변경. 
// 2008/10/23일 현재 쓰이고 있지 않음.
class ChkLoaded
{
public:
	ChkLoaded(void) : m_IsLoaded(false) {}
	~ChkLoaded(void) {}

	bool			IsLoaded() { return m_IsLoaded; }
	void			SetLoaded( bool v ) { m_IsLoaded = v; }

protected:
	bool	m_IsLoaded;
};

typedef STLX_HASH_MAP<MAPCODE, MAPCODE>		WP_HASH;
typedef WP_HASH::iterator					WP_HASH_IT;

class WayPoint : public ChkLoaded
{
public:
	WayPoint(void);
	~WayPoint(void);

	void			Clear();
	BOOL			IsExist( MAPCODE MapCode );
	BOOL			Insert( MAPCODE MapCode );

	int				GetCount() { return (int)m_WPHash.size(); }

	void			Serialize( WAYPOINT_TOTAL_INFO & INOUT rInfo, eSERIALIZE se );

private:
	WP_HASH m_WPHash; 
};

typedef STLX_HASH_MAP<DWORD, WAYPOINTSLOT>		CWP_HASH;
typedef CWP_HASH::iterator						CWP_HASH_IT;

class CharWayPoint : public ChkLoaded
{
public:
	CharWayPoint(void) {}
	~CharWayPoint(void) {}

	void			Clear();
	BOOL			IsExist( DWORD Index );
	BOOL			Insert( WAYPOINTSLOT stWayPoint );
	BOOL			Delete( DWORD Index );

	WAYPOINTSLOT	GetWayPointSlot( DWORD Index );
	int				GetCount() { return (int)m_CWPHash.size(); }

	void			Serialize( CHAR_WAYPOINT_TOTAL_INFO & INOUT rInfo, eSERIALIZE se );

private:
	CWP_HASH m_CWPHash;
};
