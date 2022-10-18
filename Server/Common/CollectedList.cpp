#include "StdAfx.h"
#include "CollectedList.h"

void CollectedList::Add( WzID CollectionID, WORD wCount )
{
	COLLECTED_HASH_IT it = m_Hash.find( CollectionID );
	if( it == m_Hash.end() )
		m_Hash[CollectionID] = wCount;
	else
		wCount = (it->second += wCount);
}

void CollectedList::Delete(WzID CollectionID, WORD wCount )
{
	COLLECTED_HASH_IT it = m_Hash.find( CollectionID );
	if( it != m_Hash.end() )
	{
		int nCount = wCount - it->second;
		
		if( 1 > nCount )
			m_Hash.erase(it);
	}
}


WORD CollectedList::GetCount(WzID CollectionID)
{
	COLLECTED_HASH_IT it = m_Hash.find( CollectionID );
	if( it != m_Hash.end() )
		return it->second;

	return 0;
}

void CollectedList::SerializeStream( COLLECTED_TOTAL_INFO & IN rTotalInfo, eSERIALIZE eType )
{
	if( eType == SERIALIZE_STORE )
	{
		Clear();

		for( int i = 0 ; i < rTotalInfo.m_Count ; ++i )
			Add(rTotalInfo.m_Slot[i].m_CollectedID,rTotalInfo.m_Slot[i].m_wCount);
	}
	else if( eType == SERIALIZE_LOAD )
	{
		BYTE count = 0;
		COLLECTED_HASH_IT it = m_Hash.begin(), end = m_Hash.end();
		while( it != end )
		{
			rTotalInfo.m_Slot[count].m_CollectedID	= it->first;
			rTotalInfo.m_Slot[count++].m_wCount		= it->second;
			++it;
		}
		rTotalInfo.m_Count = count;
	}
}
