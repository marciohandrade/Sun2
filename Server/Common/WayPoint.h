#pragma once

#include <hash_map>
#include <StructInPacket.h>

// ��Ż NPC�� ��������Ʈ ����Ʈ�� ��ϵǴ� ������.
// ���� ��ȹ���� �÷��̾ �̵��Ͽ� Ŭ���� NPC�� �� ������ �����ϰ�, ��Ż�� ���� �ִ� ����̾�����,
// ���� ��Ż�� ��ü�� �����ְ� �̿� ������� ���� �ִ� ������� ����. 
// 2008/10/23�� ���� ���̰� ���� ����.
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
