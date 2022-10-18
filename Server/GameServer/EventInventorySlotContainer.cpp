#include "StdAfx.h"
#include ".\eventinventoryslotcontainer.h"
#include ".\EventItemSlot.h"
#include "Player.h"
#include <ItemInfoParser.h>

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

EventInventorySlotContainer::EventInventorySlotContainer(void)
{
	Init( MAX_EVENT_INVENTORY_SLOT_NUM, SI_EVENT_INVENTORY );
	m_eEventIvenState		= EVENT_INVEN_STATE_UNAVAILABLE;
	m_eEventIvenTransaction = EVENT_INVEN_TRANS_END;
}

EventInventorySlotContainer::~EventInventorySlotContainer(void)
{
}

SCSlot * EventInventorySlotContainer::CreateSlot()
{
	return new EventItemSlot;
}

VOID EventInventorySlotContainer::InsertSlot( POSTYPE AtPos, EVENTSLOTEX & IN rSlotInfo )
{
	if( rSlotInfo.m_ItemCode == 0 )		return;
	if( rSlotInfo.m_ItemSeq == 0 )		return;
	if( rSlotInfo.m_ItemCount == 0 )	return;

	EventItemSlot& rSlot = (EventItemSlot&)GetSlot(AtPos);
	rSlot.Serialize( rSlotInfo, SERIALIZE_STORE );
	rSlot.SetPos( AtPos );
	rSlot.SetSlotIdx( GetSlotIdx() );
	++m_nSlotNum;
	ASSERT( m_nSlotNum <= GetMaxSlotNum() );
}

VOID EventInventorySlotContainer::ClearAll()
{
	SCItemSlotContainer::ClearAll();
	Close();
}

VOID EventInventorySlotContainer::Close()
{
	m_eEventIvenState		= EVENT_INVEN_STATE_UNAVAILABLE;
	m_eEventIvenTransaction = EVENT_INVEN_TRANS_END;
}

RC::eITEM_RESULT EventInventorySlotContainer::Open()
{
	m_eEventIvenState = EVENT_INVEN_STATE_AVAILABLE;

	return Select();
}

RC::eITEM_RESULT EventInventorySlotContainer::Select()
{
	if( IsEventTransaction() ) return RC::RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION; /*RC::RC_EVENT_ALREADY_ANOTHER_TRANSACTION*/;
	SetEventTransaction( EVENT_INVEN_TRANS_BEGIN_SELECT );

	return RC::RC_ITEM_EVENT_SUCCESS /*RC::RC_EVENT_SUCCESS*/;
}

RC::eITEM_RESULT EventInventorySlotContainer::MoveItemToInventory( POSTYPE AtPos, DWORD & OUT ItemSeqOut )
{
	if( IsEventTransaction() ) return RC::RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION; /*RC::RC_EVENT_ALREADY_ANOTHER_TRANSACTION*/;
	SetEventTransaction( EVENT_INVEN_TRANS_BEGIN_SELECT );

	if( AtPos >= GetMaxSlotNum() )
	{
		SUNLOG( eCRITICAL_LOG, _T("[Abuse]Hacking User:%s,Pos:%u"), m_pPlayer->GetCharName(), AtPos );
		return RC::RC_ITEM_EVENT_FAILED /*RC::RC_EVENT_FAILED*/;
	}

	if( IsEmpty( AtPos) )
		return RC::RC_ITEM_NOTEXIST_ITEM/* RC::RC_EVENT_NOTEXIST_EVENTITEM*/;

	EventItemSlot & rSlot = (EventItemSlot &)GetSlot( AtPos );

	if( !ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode()) )
		return RC::RC_ITEM_INVALID_VALUE /*RC::RC_EVENT_INVALID_ITEMCODE*/;

	SCItemSlot ItemSlot;
	ItemSlot.SetCode(rSlot.GetCode());
	m_TransactionInfo.m_ItemCode	= rSlot.GetCode();
	m_TransactionInfo.m_ItemCount	= rSlot.GetNum();
	m_TransactionInfo.m_DateTime	= 0;
	switch( ItemSlot.GetItemInfo()->m_ChargeSubType )
	{
	case eCHARGE_SUB_FIXED_AMOUNT:
	case eCHARGE_SUB_FIXED_AMOUNT_EX:
	case eCHARGE_SUB_FIXED_QUANTITY:
	case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
	case eCHARGE_SUB_FATE:
	case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_RIDER_WITH_PART:
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
		{
			m_TransactionInfo.m_DateTime = rSlot.GetDateTime()*60;
		}
		break;
	}

	if( RC::RC_ITEM_SUCCESS != m_pPlayer->GetItemManager()->ReserveSpaceAllKindOfItem( 
		1, &m_TransactionInfo.m_ItemCode, &m_TransactionInfo.m_ItemCount, &m_TransactionInfo.m_DateTime, 
		NULL, NULL, NULL, 
		MAX_LOCKED_INFO_NUM, m_TransLockedInfo, &m_LockedCount ) )
	{
		return RC::RC_ITEM_ISNOTEMPTYSLOT /*RC::RC_EVENT_FULL_INVENTORY*/;
	}

	ItemSeqOut = static_cast<DWORD>(rSlot.GetSerial());

	return RC::RC_ITEM_EVENT_SUCCESS /*RC::RC_EVENT_SUCCESS*/;
}



//////////////////////////////////////////////////////////////////////////
// 결과값을 받아서 처리하는 부분
//////////////////////////////////////////////////////////////////////////
VOID EventInventorySlotContainer::OnSelect( BOOL bSuccess, EVENT_ITEM_TOTAL_INFO_EX * IN pTotalInfo )
{
	ASSERT( IsEventTransaction() );
	SetEventTransaction( EVENT_INVEN_TRANS_END );

	if( bSuccess )
	{
		if( GetSlotNum() > 0 ) SCItemSlotContainer::ClearAll();

		for( int i = 0 ; i < pTotalInfo->m_Count ; ++i )
		{
			InsertSlot( i, pTotalInfo->m_Slot[i] );
		}
	}
	else
	{

	}
}

VOID EventInventorySlotContainer::OnMoveItemToInventory( BOOL bSuccess, POSTYPE atPos, JUST_INVENTORY_TOTAL_INFO * OUT pTotalInfoOut )
{
	ASSERT( IsEventTransaction() );
	SetEventTransaction( EVENT_INVEN_TRANS_END );

	if( bSuccess )
	{
		EventItemSlot EventSlot;
		DeleteSlot( atPos, &EventSlot );

		ASSERT( EventSlot.GetNum() != 0 );
        const KindOfItemTag item_tag = { item_tag.Tag_None };
        m_pPlayer->GetItemManager()->ObtainReservedAllKindOfItem(\
            item_tag, m_TransLockedInfo, m_LockedCount, pTotalInfoOut);
		
		SCItemSlot ItemSlot;
		ItemSlot.SetCode(EventSlot.GetCode());
		switch( ItemSlot.GetItemInfo()->m_ChargeSubType )
		{
		case eCHARGE_SUB_FIXED_AMOUNT:
		case eCHARGE_SUB_FIXED_AMOUNT_EX:
		case eCHARGE_SUB_FIXED_QUANTITY:
		case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
		case eCHARGE_SUB_FATE:
		case eCHARGE_SUB_COORDINATE_EX:
        case eCHARGE_SUB_RIDER_WITH_PART:
        case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
        case eCHARGE_SUB_FIXED_AMOUNT_EX2:
			{
				ItemSlot.SetDateTime( EventSlot.GetDateTime()*60 );
			}
            break;
		}
		// 로케트 베터리 이벤트 로그
		GAMELOG->LogItemEx2( ITEM_EVENT_ROCKET, m_pPlayer, ItemSlot, 0, EventSlot.GetNum(), 
			EventSlot.GetSerial(), EventSlot.GetCode(), EventSlot.GetNum() );

#ifdef _NA_001990_ACSYSTEM_ADD
        const BASE_ITEMINFO* item_info = ItemSlot.GetItemInfo();
        int item_count = EventSlot.GetNum();
        if (item_info->m_wACCode)
            m_pPlayer->GetACManager()->ProcessItemObtain(item_info->m_Code, item_info->m_wACCode, 
                item_info->m_byACReferenceID, item_count);
#endif //_NA_001990_ACSYSTEM_ADD
	}
	else
	{
	}
}

VOID EventInventorySlotContainer::Serialize( EVENT_ITEM_TOTAL_INFO & rTotalInfo, eSERIALIZE se )
{
	if( GetSlotNum() > MAX_EVENT_INVENTORY_SLOT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[EventInventorySlotContainer] Invalid MaxSlotNum[%d/%d]", GetSlotNum(), MAX_EVENT_INVENTORY_SLOT_NUM );
		return;
	}

	if( se == SERIALIZE_LOAD )
	{
		rTotalInfo.m_Count = GetSlotNum();

		for( POSTYPE i = 0 ; i < GetSlotNum() ; ++i )
		{
			EventItemSlot & rSlot = (EventItemSlot&)GetSlot(i);
			rTotalInfo.m_Slot[i].m_ItemCode		= rSlot.GetCode();
			rTotalInfo.m_Slot[i].m_ItemCount	= rSlot.GetNum();
			rTotalInfo.m_Slot[i].m_DateTime		= rSlot.GetDateTime();
		}
	}
	else
	{
		ASSERT( NULL );
	}
}













