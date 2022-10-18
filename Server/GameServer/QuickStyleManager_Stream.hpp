#pragma once

//==================================================================================================
/*
// CHANGES, f100826.2L, removed

#include <SCQuickStyleSlot.h>
#include <SCQuickStyleSlotContainer.h>

template< class STYLE_TOTAL_INFO_TYPE >
VOID QuickStyleManager::SerializeStyleTotalInfo( STYLE_TOTAL_INFO_TYPE & StyleInfo, eSERIALIZE eType )
{
	if( eType == SERIALIZE_LOAD )
	{
		StyleInfo.m_Count = 0;
		STYLE_TOTAL_INFO_TYPE::SLOT_TYPE * pSlot = StyleInfo.m_Slot;
		POSTYPE total = 0;
		for(POSTYPE i=0;i<STYLE_TOTAL_INFO_TYPE::MAX_SLOT_NUM;++i)
		{
			if( !m_pSlotMGR->IsEmpty(SI_STYLE, i) )
			{
				pSlot[total].m_Pos = i;
				SerializeStyleStream(pSlot[total].m_Pos, &pSlot[total].m_Stream, SERIALIZE_LOAD);
				++total;
				++StyleInfo.m_Count;
			}
		}
	}
	else if( eType == SERIALIZE_STORE )
	{
		ASSERT( GetQuickStyleSlotContainer()->GetSlotNum() == 0 );
		STYLE_TOTAL_INFO_TYPE::SLOT_TYPE * pSlot = StyleInfo.m_Slot;
		POSTYPE start = 0;
		POSTYPE total = StyleInfo.m_Count;
		ASSERT( StyleInfo.m_Count <= STYLE_TOTAL_INFO_TYPE::MAX_SLOT_NUM );
		for(POSTYPE i=start;i<total;++i)
			SerializeStyleStream(pSlot[i].m_Pos, &pSlot[i].m_Stream,SERIALIZE_STORE);
	}
}

template< class STYLESTREAM_TYPE >
VOID QuickStyleManager::SerializeStyleStream( POSTYPE pos, STYLESTREAM_TYPE * IN pStream, eSERIALIZE eType )
{
	if( eType == SERIALIZE_LOAD )
	{
		SCQuickStyleSlot & rSlot = static_cast<SCQuickStyleSlot &>(GetQuickStyleSlotContainer()->GetSlot(pos));
		rSlot.CopyOut(*pStream);
	}
	else if( eType == SERIALIZE_STORE )
		((SCQuickStyleSlotContainer*)GetQuickStyleSlotContainer())->InsertSlot( pos, *pStream );
}

*/