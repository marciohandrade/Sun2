#include "StdAfx.h"
#include ".\vendorslotcontainer.h"
#include ".\VendorSlot.h"
#include ".\Player.h"
#include ".\ItemManager.h"
#include <SCItemSlotContainer.h>
#include <StructInPacket.h>


VendorSlotContainer::VendorSlotContainer(void)
{
	Init( MAX_PERSONAL_VENDOR_SLOT_NUM, SI_VENDOR_BUY );
}

VendorSlotContainer::~VendorSlotContainer(void)
{
	DestroyVendor();
}

SCSlot*		VendorSlotContainer::CreateSlot()
{
	return new VendorSlot;
}

void VendorSlotContainer::DestroyVendor()
{
	ClearAll();

    for (PLAYER_HASH::iterator it = observer_players_.begin(); it != observer_players_.end(); ++it)
    {
        Player* const observer = it->second;
        if (obj_util::IsValid(observer) == false)
        {
            continue;
        }
        observer->SetVendorContainer(NULL);
        if(observer->GetBehave() != PLAYER_BEHAVE_VENDOR_OBSERVER_STATE)
        {
            continue;
        }
        observer->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
    }
	observer_players_.clear();

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    m_SlotCodeHash.clear();
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
}

//==================================================================================================
// NOTE: f110531.4L, added interface to support broadcast
void VendorSlotContainer::SendPacketToObservers(
    Player* const actor, MSG_BASE_FORWARD* const msg_base, const uint16_t msg_size) const
{
    if (obj_util::IsValid(actor) == false)
    {
        return;
    }

    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.Init(msg_base, msg_size);
    uint16_t* const user_begin = msg_brd.GetUsersStartPtr();

    for (PLAYER_HASH::const_iterator it = observer_players_.begin(); 
        it != observer_players_.end(); ++it)
    {
        Player* const observer = it->second;
        if (obj_util::IsValid(observer) == false)
        {
            continue;
        }
        const Object::Key object_key = observer->GetObjectKey();
        const DWORD bound_check = msg_brd.AddUser(user_begin, object_key);
        if (bound_check == msg_brd.MAX_USER_COUNT) 
        {
            break;
        }
    };

    if (msg_brd.number_of_users) 
    {
        actor->SendPacket(&msg_brd, msg_brd.GetSendingSize());
    }
}
//==================================================================================================

bool VendorSlotContainer::CreateVendor(
    Player* const establisher, 
    const util::StrArrayForVendorTitle& vendor_title, 
    const ESTABLISHER_VENDOR_TOTAL_INFO& vendor_total_info)
{
    if (obj_util::IsValid(establisher) == false)
    {
        return false;
    }

	//초기화
	ClearAll();

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    m_SlotCodeHash.clear();
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH

	if( !observer_players_.empty() )
	{
		SUNLOG( eCRITICAL_LOG, _T("VendorView List size is not Zero { %u }"), observer_players_.size() );	
	}

	// 맴버 초기화
	established_player_ = establisher;

	SCSlotContainer* const inventory = establisher->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
	
	// 상점창에 VendorSlot Item 채움
	bool result = true;
	
	for (POSTYPE i = 0 ; i < vendor_total_info.m_Count; ++i)
	{
        const ESTABLISHER_VENDOR_TOTAL_INFO::SLOT_TYPE& slot = 
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
            vendor_total_info.sell_items_[i].m_Slot;
#else
            vendor_total_info.m_Slot[i];
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

        const POSTYPE org_pos = slot.m_OrgPos;
		if (inventory->ValidPos(org_pos) == false)
        {
            return false;
        }
        if (inventory->IsEmpty(org_pos))
        {
            return false;
        }
        if (inventory->IsLocked(org_pos))
        {
            return false;
        }
        
        const POSTYPE ven_pos = slot.m_VendorPos;
		if (!ValidPos(ven_pos) || !IsEmpty(ven_pos))
        {
            return false;
        }
        
		SCItemSlot& item_slot = (SCItemSlot&)inventory->GetSlot(org_pos);
        if (item_slot.IsUserBlocked())
        {
            return false;
        }
        if (item_slot.GetLaxLock()->IsLock())
        {
            return false;
        }
        if (item_slot.IsPossessionItem() &&
            item_slot.GetIdentifyPossession() &&
            (!item_slot.IsDestroyItem()))
        {
            return false;
        }
#ifdef _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE
        if (item_slot.IsProgressTime())
        {
            return false;
        }
#endif
        if (item_slot.CanTradeSell(eITEM_TRADESELL_TRADE) == false) 
        {
            return false;
        }
        if (item_slot.CanTradeSell(eITEM_TRADESELL_USED_ITEM) == false &&
            item_slot.IsProgressTime())
        {
            return false;
        }

		VendorSlot vender_slot;
		vender_slot.Create(item_slot, slot.m_Money);

		if (InsertSlot(ven_pos, vender_slot) != RC::RC_INSERT_SLOT_SUCCESS) 
		{
			result = false;
			break;
		}
	}
    
    SetTitle(vendor_title);
	
	return result;
}

bool VendorSlotContainer::ModifySlot(const POSTYPE /*pos*/, const VENDOR_ITEMSLOT& vendor_slot)
{
    const Player* const establisher = GetEstablisher();
    if (establisher == NULL)
    {
        return false;
    }
    
	SCSlotContainer* const inventory = establisher->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
	const POSTYPE org_pos = vendor_slot.m_OrgPos;
	if (!inventory->ValidPos(org_pos))
    {
        return false;
    }
    if (inventory->IsEmpty(org_pos))
    {
        return false;
    }
    if (inventory->IsLocked(org_pos))
    {
        return false;
    }
    
	SCItemSlot& org_item_slot = (SCItemSlot&)inventory->GetSlot(org_pos);
    if (org_item_slot.CanTradeSell(eITEM_TRADESELL_TRADE) == false) 
    {
        return false;
    }
    if (org_item_slot.CanTradeSell(eITEM_TRADESELL_USED_ITEM) == false &&
        org_item_slot.IsProgressTime())
    {
        return false;
    }

	const POSTYPE vendor_pos = vendor_slot.m_VendorPos;
	if (!ValidPos(vendor_pos)) 
    {
        return false;
    }
        
	VendorSlot slot;
	slot.Create(org_item_slot, vendor_slot.m_Money);
	if (IsEmpty(vendor_pos))
	{
		if (InsertSlot(vendor_pos, slot) == RC::RC_INSERT_SLOT_SUCCESS)
        {
            return true;
        }
		else
        {
            return false;
        }
	}

	UpdateSlot(vendor_pos, slot);

	return true;
}

BOOL VendorSlotContainer::IsLocked( POSTYPE AtPos )
{
	ASSERT( AtPos < GetMaxSlotNum() );
	if( IsEmpty(AtPos) )			return FALSE;

	return ((VendorSlot&)(GetSlot(AtPos))).IsLocked();
}

VOID VendorSlotContainer::SetLock( POSTYPE AtPos, BOOL val )
{
	ASSERT( AtPos < GetMaxSlotNum() );
	if( IsEmpty(AtPos) )			return;

	((VendorSlot&)(GetSlot(AtPos))).SetLock(val);
}

void VendorSlotContainer::AddObserver(Player* const observer)
{
    if (obj_util::IsValid(observer) == false)
    {
        return;
    }

	const DWORD dwObjectKey = observer->GetObjectKey();
	PLAYER_HASH_ITR itFIND = observer_players_.find( dwObjectKey );
	Player* pTmpPlayer = (itFIND != observer_players_.end()) ? itFIND->second : NULL;

	if( pTmpPlayer )
	{
		SUNLOG( eCRITICAL_LOG, _T("[ADD] Already Exist Vendor Observer List [%s:%u]<=[%s:%u]"),
			pTmpPlayer->GetCharName(), pTmpPlayer->GetBehave(), observer->GetCharName(), observer->GetBehave() );
	}
	else
	{
		observer_players_.insert( std::make_pair( dwObjectKey, observer ) );
	}
}

void VendorSlotContainer::RemoveObserver(const Player* const observer)
{
    if (observer == NULL)
    {
        return;
    }
    const Object::Key obj_key = observer->GetObjectKey();
	observer_players_.erase(obj_key);
}

BOOL VendorSlotContainer::IsEmpty( POSTYPE AtPos )
{
	return ( GetSlot(AtPos).GetSerial() == 0 );
}


#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
void VendorSlotContainer::OnInsert(SCSlot& IN rSlotIn)
{
    SLOTCODE_HASH::_Pairib pairib = m_SlotCodeHash.insert( SLOTCODE_HASH::value_type(rSlotIn.GetCode(), 1) );
    if( pairib.second == false )
    {
        ++(pairib.first->second);
    }
}

void VendorSlotContainer::OnDelete(SCSlot& IN rSlotIn)
{
    SLOTCODE_HASH::iterator iter = m_SlotCodeHash.find( rSlotIn.GetCode() );
    if( iter != m_SlotCodeHash.end() )
    {
        if( iter->second <= 1 )
            m_SlotCodeHash.erase( iter );
        else
            --(iter->second);
    }
}

BOOL VendorSlotContainer::HasItem( SLOTCODE ItemCode )
{
    SLOTCODE_HASH::iterator iter = m_SlotCodeHash.find( ItemCode );
    return ( iter != m_SlotCodeHash.end() );
}

#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
