#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionIdentity) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT
nsSlot::ItemFunctionIdentity::IdentifyFateItem(POSTYPE target_pos)
{
    SCSlotContainer* inventory = root_->GetItemSlotContainer(SI_INVENTORY);

    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;    
    }
    if (root_->ValidPos(SI_INVENTORY, target_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;  
    }
    SCItemSlot& target_item = static_cast<SCItemSlot&>(inventory->GetSlot(target_pos));
    //if (!target_item.GetItemInfo()) return RC::RC_ITEM_NOINFO; <- assert(not null)

    // (NOTE) Fate item specification : IsFateItem(), GetFateIdentify(���� ����)
    if (target_item.IsFateItem() == false) {
        return RC::RC_ITEM_NOT_FATE_ITEM;
    }
    if (target_item.GetFateIdentify()) {
        return RC::RC_ITEM_ALREADY_IDENTYFIED_FATE_ITEM;
    }
    // (NOTE) DB transaction lock
    target_item.SetLock(true);

    DWORD expiration_time = 0; // per day based
    if (target_item.CalFateDateTime(expiration_time) == false)
    {
        target_item.SetLock(false);
        return RC::RC_ITEM_FAILED;
    }

    // CHANGES: f110428.x, removed block, __NA002610_110428_REMOVE_INFINITY_USING_LUCKY
    if (FlowControl::FCAssert(expiration_time <= 90) == false) { // 3rd index, 90��
        return RC::RC_ITEM_INVALIDSTATE;
    };
#if SUN_CODE_BACKUP
    if (FlowControl::FCAssert(expiration_time <= INFINITE_ITEM_DAY) == false) {
        expiration_time = INFINITE_ITEM_DAY; //���� �������̶�...
    }
    if (expiration_time == INFINITE_ITEM_DAY)
    { // �������̰� ���̺� �����̸� DB���� ����
        eSERVER_MODE server_mode = g_pGameServer->GetMode();
        if (server_mode == eSERVER_LIVE ||
            server_mode == eSERVER_DEV ||
            server_mode == eSERVER_INTRAOFFICE)
        {
            //�� ���� ���� ����.
            return RC::RC_ITEM_WIN_PRIZE_INFINITY_ITEM;
        }
    }
#endif
    //������ �������� �ƴϸ� �ٷ� ���� (�׼��� �����ѵ� ����)
    if (target_item.CalFateInfo() == false)
    {
        target_item.SetLock(false);
        return RC::RC_ITEM_FAILED;
    }
    //                                             1day
    target_item.SetDateTime(expiration_time * (24 * 60 * 60));
    target_item.SetFatePeriod(static_cast<WORD>(expiration_time));

    GAMELOG->LogItem(ITEM_IDENTIFY_LUCKY_ITEM, root_->player_, &target_item);

    target_item.SetLock(false);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
// implemented by __NA_1247_POSSESSION_ITEM
RC::eITEM_RESULT nsSlot::ItemFunctionIdentity::IdentifyPossessionItem(
    POSTYPE ItemPos, ITEMSTREAMEX& OUT rItemStream)
{
    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);
    if (!root_->ValidPos(SI_INVENTORY, ItemPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rPossessionItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
    if (!rPossessionItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //Ÿ���� �ͼ� �������ΰ�?
    if (!rPossessionItemSlot.GetItemInfo()->m_bPossessionItem)
        return RC::RC_ITEM_INVALID_TYPE;

    //�ͼ��� �� �������̸� �ͼ��� �� ����.
    if (rPossessionItemSlot.GetIdentifyPossession())
        return RC::RC_ITEM_INVALID_TYPE;

    rPossessionItemSlot.SetIdentifyPossession(true);
    //�α�
    GAMELOG->LogItem(ITEM_IDENTIFY_POSSESSION_ITEM, root_->player_, &rPossessionItemSlot);

    rPossessionItemSlot.CopyOut(rItemStream);

    return RC::RC_ITEM_SUCCESS;
}

