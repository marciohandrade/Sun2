// WarehouseSlotContainer.cpp: implementation of the CWarehouseSlotContainer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WarehouseSlotContainer.h"
#include "Player.h"
#include <PacketStruct_DG.h>
#include "ItemManager.h"
#include "FunctionalRestrictManager.h"
#include "StatisticsManager.h"
#include "GameServerEx.h"
#include "ChaoState.h"
#include <SCItemSlot.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarehouseSlotContainer::CWarehouseSlotContainer()
    : m_pPlayer(NULL)
    , m_Money(0)
    , m_bChanged(false)
    , m_bStarted(false)
{
    Init(MAX_WAREHOUSE_SLOT_NUM, SI_WAREHOUSE);
}

CWarehouseSlotContainer::~CWarehouseSlotContainer()
{

}

void CWarehouseSlotContainer::ClearAll()
{
    SCItemSlotContainer::ClearAll();
    m_Money = 0;
    m_bChanged = false;
    m_bStarted = false;
}

BOOL CWarehouseSlotContainer::ValidState()
{
	if( SCItemSlotContainer::ValidState() )
	{
		ProtectorWarehouse& rPROTECT = m_pPlayer->GetWarehouseInventoryProtector().WAREHOUSE();
		if( !rPROTECT.IsUsable() )
			return FALSE;

		return IsEnableService();
	}
	return FALSE;
}

BOOL CWarehouseSlotContainer::IsEnableService()
{
	if( m_pPlayer->GetChaoState().IsChaoState() )
		return FALSE;

	return g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_USE_WAREHOUSE );
}

BOOL CWarehouseSlotContainer::DepositMoney( MONEY PlusMoney ) 
{ 
	if( !IsStarted() || !IsEnableService() ) return FALSE; 

	m_Money += PlusMoney; 

	m_bChanged = true;

//	g_pGameServer->PlusWareMoneyPlus( PlusMoney );
	g_StatisticsManager.PlusStatisticType( eSTATISTIC_MONEY_WAREHOUSE_PLUS, PlusMoney );

	return TRUE;
}

BOOL CWarehouseSlotContainer::WithDrawMoney( MONEY MinusMoney )
{
	if(!IsStarted() || !IsEnableService()) return FALSE;

	if( m_Money < MinusMoney ) return FALSE;

	m_Money -= MinusMoney;

	m_bChanged = true;

//	g_pGameServer->MinusWareMoneyMinus( MinusMoney );
	g_StatisticsManager.PlusStatisticType( eSTATISTIC_MONEY_WAREHOUSE_MINUS, MinusMoney );

	return TRUE;
}

VOID CWarehouseSlotContainer::StartWarehouse()
{
    nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl();
	for(POSTYPE i=0;i<GetMaxSlotNum();++i)
	{
		if( !IsEmpty(i) )
		{
			item_updater_impl->AddSerialCode( eSERIAL_WAREHOUSE_START, GetSlotIdx(), i, ((SCItemSlot&)GetSlot(i)).GetWorldSerial() );
		}
	}
	m_bStarted = true; 
}

VOID CWarehouseSlotContainer::EndWarehouse()
{
    nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl();
	for(POSTYPE i=0;i<GetMaxSlotNum();++i)
	{
		if( !IsEmpty(i) )
		{
			SCItemSlot & rSlot = (SCItemSlot &)GetSlot(i);
			item_updater_impl->RemoveExpirationItem( rSlot );
			item_updater_impl->RemoveSerialCode( GetSlotIdx(), i, rSlot.GetWorldSerial() );
		}
	}
	ClearAll();
}

BOOL CWarehouseSlotContainer::SaveWarehouseToDBP()
{
	// 시작 안되어있으면 return
	if( !IsStarted() )	return FALSE;

	// blocked된 시점에 아이템 변경이 일어나지 않으면, 다시 창고를 열었을 때, 
	// blocked이 안되고, 상점에 팔 수 있기 때문에!
	// 변경이 없으면 return
	//if( !m_bChanged )	return TRUE;

	MSG_DG_CHARINFO_WAREHOUSE_CMD cmsg;
	cmsg.m_dwKey = m_pPlayer->GetUserKey();
    cmsg.char_guid_ = m_pPlayer->GetCharGuid(); // f101021.2L
	cmsg.m_ChannelID = g_pGameServer->GetKey().GetChannelID();
	cmsg.m_WarehouseMoney = GetCurMoney();
	SerializeItemInfo( cmsg.m_ItemInfo, SERIALIZE_LOAD );
	BOOL rt = m_pPlayer->SendToGameDBPServer( &cmsg, cmsg.GetSize() );
	ASSERT( rt && "DB로 보낼 수 없습니다." );

	m_bChanged = false;

	return TRUE;
}

//#include <string>
RC::eSLOT_INSERT_RESULT CWarehouseSlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	if( !IsStarted() || !IsEnableService() )	return RC::RC_INSERT_SLOT_DISABLE_SERVICE;

	RC::eSLOT_INSERT_RESULT rt = SCSlotContainer::InsertSlot( AtPos, rSlot );
	if( RC::RC_INSERT_SLOT_SUCCESS == rt )
	{
		SCItemSlot & rCurSlot = (SCItemSlot &)GetSlot(AtPos);
        if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
        {
            item_updater_impl->AddSerialCode( eSERIAL_WAREHOUSE, GetSlotIdx(), AtPos, rCurSlot.GetWorldSerial() );
        }
		m_bChanged = true;		
	}
	
	return rt;
	
}

BOOL CWarehouseSlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	if( !IsStarted() || !IsEnableService() )	return FALSE;

	SCItemSlot TmpDeleteItemSlot;
	BOOL rt = SCSlotContainer::DeleteSlot( AtPos, &TmpDeleteItemSlot );
	if( rt )
	{
		if( pSlotOut ) 
		{
			((SCItemSlot*)pSlotOut)->Copy(TmpDeleteItemSlot);
		}
		/*
		static char buff[128];

		std::string itemInfo = "창고유저:";
		itemInfo += m_pPlayer->GetCharName();
		itemInfo += ",item삭제:";
		itemInfo += tmpItemSlot.GetItemInfo()->m_pszName;
		itemInfo += "pos(";
		itemInfo += _itoa(AtPos, buff, 10 );
		itemInfo += "),";
		itemInfo += "code(";
		itemInfo += _itoa(tmpItemSlot.GetItemCode(), buff, 10 );
		itemInfo += ")";
		SUNLOG( eFULL_LOG, const_cast<char *>(itemInfo.c_str()) );*/

        if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
                m_pPlayer->GetItemManager()->GetUpdaterImpl())
        {
            item_updater_impl->RemoveSerialCode( GetSlotIdx(), AtPos, TmpDeleteItemSlot.GetWorldSerial() );
        }
		m_bChanged = true;

	}
	return rt;
}

//VOID CWarehouseSlotContainer::UpdateSlot( POSTYPE AtPos, eSlotType type, DBSERIAL serial, SLOTCODE code, POSTYPE quickPos, DURATYPE dura, BYTE level, eUPDATE_BIT upbits )
//{
//	SCSlotContainer::UpdateSlot( AtPos, type, serial, code, quickPos, dura, level, upbits );
//	m_bChanged = true;
//}
//
//VOID CWarehouseSlotContainer::UpdateSlot( POSTYPE AtPos, SCSlot & IN rSlot )
//{
//	SCItemSlotContainer::UpdateSlot( AtPos, rSlot );
//	m_bChanged = true;
//}

// 로그 기록용으로 만듬
VOID CWarehouseSlotContainer::SerializeItemInfo( BYTE * pStream, WORD wSize, eSERIALIZE eType )
{
	ASSERT( wSize >= MAX_WAREHOUSESTREAM_SIZE );

	if( eType == SERIALIZE_LOAD )
	{
		memset( pStream, 0, wSize );
		ITEMSTREAMEX * pStreamEx = (ITEMSTREAMEX *)pStream;

		for(POSTYPE i=0;i<GetMaxSlotNum();++i)
		{
			if( !IsEmpty(i) )
				SCItemSlotContainer::SerializeItemStream(i, &pStreamEx[i], eType);
		}
	}
	else if( eType == SERIALIZE_STORE )
	{
		ASSERT( GetSlotNum() == 0 );
		ITEMSTREAMEX * pStreamEx = (ITEMSTREAMEX *)pStream;

		for(POSTYPE i=0;i<GetMaxSlotNum();++i)
		{
			SCItemSlotContainer::SerializeItemStream(i, &pStreamEx[i], eType);
		}
	}
}
void CWarehouseSlotContainer::OnInsert( SCSlot & IN rSlot )
{
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(rSlot);
    // CHANGES: f110106.3L, added the rule to solve a problem by external GM control
    // in this case, it's possible when a 'pet' or 'rider' item is summoned.
    // a calling case of OnInsert can prevent by other game rules.
    if (item_slot.IsUserBlocked()) {
        item_slot.SetUserBlocked(false);
    };
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddExpirationItem(item_slot);
    }
}
void CWarehouseSlotContainer::OnDelete( SCSlot & IN rSlot )
{
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveExpirationItem( (SCItemSlot &)rSlot );
    }
}

//==================================================================================================
// (f100527.4L) WAREHOUSE_TOTAL_INFO_TYPE = { _WAREHOUSE_TOTAL_INFO }
//template<class WAREHOUSE_TOTAL_INFO_TYPE>
void CWarehouseSlotContainer::SerializeItemInfo(_WAREHOUSE_TOTAL_INFO& total_info,
                                                eSERIALIZE serialize_type)
{
    POSTYPE number_of_max_slots = GetMaxSlotNum();
	if (number_of_max_slots > MAX_WAREHOUSE_SLOT_NUM)
	{
		SUNLOG(eCRITICAL_LOG, _T("[CWarehouseSlotContainer] Invalid MaxSlotNum[%d/%d]"),
               number_of_max_slots, MAX_WAREHOUSE_SLOT_NUM);
		SetMaxSlotNum(MAX_WAREHOUSE_SLOT_NUM);
        number_of_max_slots = GetMaxSlotNum();
	}
    //
	if (serialize_type == SERIALIZE_LOAD)
	{
		total_info.m_Count = 0;
		_WAREHOUSE_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot;
		for (POSTYPE pos = 0; pos < number_of_max_slots; ++pos)
		{
            if (IsEmpty(pos)) {
                continue;
            }
			slot_it->m_Pos = pos;
			SCItemSlotContainer::SerializeItemStream(slot_it->m_Pos, &slot_it->m_Stream,
                                                     serialize_type);
			++total_info.m_Count;
            ++slot_it;
		}
	}
	else if (serialize_type == SERIALIZE_STORE)
	{
		ASSERT(GetSlotNum() == 0);
		_WAREHOUSE_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot;
        for (POSTYPE total_count = total_info.m_Count;
             total_count;
             --total_count, ++slot_it)
        {
            SCItemSlotContainer::SerializeItemStream(slot_it->m_Pos, &slot_it->m_Stream,
                                                     serialize_type);
        }
		m_bChanged = false;
	}
}




