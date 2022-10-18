#pragma once

#include <SCQuickSlot.h>
#include <SCQuickSlotContainer.h>

template< class QUICK_TOTAL_INFO_TYPE >
VOID QuickManager::SerializeQuickTotalInfo( QUICK_TOTAL_INFO_TYPE & QuickInfo, eSERIALIZE eType )
{
	if( eType == SERIALIZE_LOAD )
	{
		QuickInfo.m_Count = 0;
		QUICK_TOTAL_INFO_TYPE::SLOT_TYPE * pSlot = QuickInfo.m_Slot;
		POSTYPE total = 0;
		for(POSTYPE i=0;i<QUICK_TOTAL_INFO_TYPE::MAX_SLOT_NUM;++i)
		{
			if( !IsEmpty(i) )
			{
				pSlot[total].m_Pos = i;
				SerializeQuickStream(pSlot[total].m_Pos, &pSlot[total].m_Stream, SERIALIZE_LOAD);
				++total;
				++QuickInfo.m_Count;
			}
		}
	}
	else if( eType == SERIALIZE_STORE )
	{
		ASSERT( GetQuickSlotContainer()->GetSlotNum() == 0 );
		QUICK_TOTAL_INFO_TYPE::SLOT_TYPE * pSlot = QuickInfo.m_Slot;
		POSTYPE start = 0;
		POSTYPE total = QuickInfo.m_Count;
		ASSERT( QuickInfo.m_Count <= QUICK_TOTAL_INFO_TYPE::MAX_SLOT_NUM );
		for(POSTYPE i=start;i<total;++i)
			SerializeQuickStream(pSlot[i].m_Pos, &pSlot[i].m_Stream,SERIALIZE_STORE);


		// 처음 Quick정보를 받았을 경우!
		// QuickContainer를 iterate하면서 장착창, 인벤토리창의 모든 정보를 SCItemSlot의 QPos을 셋팅한다.
	}
}

template< class QUICKSTREAM_TYPE >
VOID QuickManager::SerializeQuickStream( POSTYPE pos, QUICKSTREAM_TYPE * IN pStream, eSERIALIZE eType )
{
	if( eType == SERIALIZE_LOAD )
	{
		SCQuickSlot & rSlot = static_cast<SCQuickSlot &>(GetQuickSlotContainer()->GetSlot(pos));
		rSlot.CopyOut(*pStream);
	}
	else if( eType == SERIALIZE_STORE )
		((SCQuickSlotContainer*)GetQuickSlotContainer())->InsertSlot( pos, *pStream );
}
