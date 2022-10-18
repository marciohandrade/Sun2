#include "StdAfx.h"
#include ".\waypoint.h"

WayPoint::WayPoint(void)
{
}

WayPoint::~WayPoint(void)
{
}

void WayPoint::Clear()
{
	m_WPHash.clear();
	m_IsLoaded = false;
}


BOOL WayPoint::IsExist( MAPCODE MapCode )
{
	WP_HASH_IT it = m_WPHash.find( MapCode );
	if( it == m_WPHash.end() )
		return FALSE;
	return TRUE;
}

BOOL WayPoint::Insert( MAPCODE MapCode )
{
	WP_HASH_IT it = m_WPHash.find( MapCode );
	if( it == m_WPHash.end() )
	{
		m_WPHash[MapCode] = MapCode;
		return TRUE;
	}
	return FALSE;
}

void WayPoint::Serialize( WAYPOINT_TOTAL_INFO & INOUT rInfo, eSERIALIZE se )
{
	ASSERT( MAX_MAP_NUM < UCHAR_MAX ); //< m_Count Type is byte
	ASSERT( WAYPOINT_TOTAL_INFO::MAX_SLOT_NUM == MAX_MAP_NUM );
	ASSERT( m_WPHash.size() <= MAX_MAP_NUM );

	if( se == SERIALIZE_STORE )
	{
		MAPCODE MapCode = 0;
		for( int i = 0 ; i < rInfo.m_Count && i < MAX_MAP_NUM ; ++i )
		{
			MapCode = rInfo.m_Slot[i];
			Insert( MapCode );
		}
		m_IsLoaded = true;
	}
	else if( se == SERIALIZE_LOAD )
	{
		if( m_WPHash.size() > MAX_MAP_NUM )
			rInfo.m_Count = MAX_MAP_NUM;
		else
			rInfo.m_Count = (BYTE)m_WPHash.size();

		
		WP_HASH_IT it(m_WPHash.begin()), end(m_WPHash.end());
		for( int i = 0 ; it != end && i < WAYPOINT_TOTAL_INFO::MAX_SLOT_NUM ; ++i, ++it )
			rInfo.m_Slot[i] = it->second;
	}
}


void CharWayPoint::Clear()
{
	m_CWPHash.clear();
	SetLoaded(false);
}

WAYPOINTSLOT CharWayPoint::GetWayPointSlot( DWORD Index )
{
	CWP_HASH_IT it = m_CWPHash.find( Index );
	if( it == m_CWPHash.end() )
	{
		return WAYPOINTSLOT();
	}

	return it->second;
}

BOOL CharWayPoint::IsExist( DWORD Index )
{
	CWP_HASH_IT it = m_CWPHash.find( Index );
	if( it == m_CWPHash.end() )
		return FALSE;
	return TRUE;
}

BOOL CharWayPoint::Insert( WAYPOINTSLOT stWayPoint )
{
	CWP_HASH_IT it = m_CWPHash.find( stWayPoint.m_IdxWayPoint );
	if( it == m_CWPHash.end() )
	{
		m_CWPHash[stWayPoint.m_IdxWayPoint] = stWayPoint;
		return TRUE;
	}
	return FALSE;
}

BOOL CharWayPoint::Delete( DWORD Index )
{
	CWP_HASH_IT it = m_CWPHash.find( Index );
	if( it != m_CWPHash.end() )
	{
		m_CWPHash.erase(it);
		return TRUE;
	}
	return FALSE;
}

void CharWayPoint::Serialize( CHAR_WAYPOINT_TOTAL_INFO & INOUT rInfo, eSERIALIZE se )
{
	ASSERT( MAX_CHAR_WAYPOINT_NUM < UCHAR_MAX ); //< m_Count Type is byte
	ASSERT( CHAR_WAYPOINT_TOTAL_INFO::MAX_SLOT_NUM == MAX_CHAR_WAYPOINT_NUM );
	ASSERT( m_CWPHash.size() <= MAX_CHAR_WAYPOINT_NUM );

	if( se == SERIALIZE_STORE )
	{
		CHAR_WAYPOINT_TOTAL_INFO::SLOT_TYPE Slot;
		for( int i = 0 ; i < rInfo.m_Count && i < MAX_CHAR_WAYPOINT_NUM ; ++i )
		{
			Slot = rInfo.m_Slot[i];
			Insert( Slot );
		}
		SetLoaded(true);
	}
	else if( se == SERIALIZE_LOAD )
	{
		if( m_CWPHash.size() > MAX_CHAR_WAYPOINT_NUM )
			rInfo.m_Count = MAX_CHAR_WAYPOINT_NUM;
		else
			rInfo.m_Count = (BYTE)m_CWPHash.size();

		CWP_HASH_IT it(m_CWPHash.begin()), end(m_CWPHash.end());
		for( int i = 0 ; it != end && i < CHAR_WAYPOINT_TOTAL_INFO::MAX_SLOT_NUM ; ++i, ++it )
			rInfo.m_Slot[i] = it->second;
	}
}

