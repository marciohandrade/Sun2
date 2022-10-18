#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionItemProperty) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
RC::eITEM_RESULT nsSlot::ItemFunctionItemProperty::UseRankDownItem(
    POSTYPE ChargeItemPos, POSTYPE TargetPos)
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ChargeItemPos))
        return RC::RC_ITEM_INVALIDPOS;

    if (!root_->ValidPos(SI_INVENTORY, TargetPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rChargeItemSlot = (SCItemSlot &)pContainer->GetSlot(ChargeItemPos);
    if (!rChargeItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    SCItemSlot & rTargetItemSlot = (SCItemSlot &)pContainer->GetSlot(TargetPos);
    if (!rTargetItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //��ũ���� ��ġ�� ����ߴ°�?
    if (rChargeItemSlot.GetItemInfo()->m_wType != eITEMTYPE_RANK_DOWN ||
        (rChargeItemSlot.GetNum() == 0))
    {
        return RC::RC_ITEM_INVALID_TYPE;
    };
    //��Ÿ�� üũ
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rChargeItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;    

    WORD wCoolTimeType = rChargeItemSlot.GetItemInfo()->m_wCoolTimeType;

    RC::eITEM_RESULT rt = root_->RankDown(TargetPos);

    if (rt != RC::RC_ITEM_SUCCESS)
    {
        return rt;
    }

    //��ũ�� ��ġ(���� ������) ����
    root_->Lose(pContainer, ChargeItemPos, 1);

    //��Ÿ��
    root_->player_->ResetUseItem(wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM


// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionItemProperty::UseHammerOfAlchemist(
    POSTYPE ChargeItemPos, POSTYPE TargetPos)
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ChargeItemPos))
        return RC::RC_ITEM_INVALIDPOS;

    if (!root_->ValidPos(SI_INVENTORY, TargetPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rChargeItemSlot = (SCItemSlot &)pContainer->GetSlot(ChargeItemPos);
    if (!rChargeItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    SCItemSlot & rTargetItemSlot = (SCItemSlot &)pContainer->GetSlot(TargetPos);
    if (!rTargetItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //���ݼ����� ��ġ�� ����ߴ°�?
    if (rChargeItemSlot.GetItemInfo()->m_wType != eITEMTYPE_HAMMER_OF_ALCHEMIST ||
        (rChargeItemSlot.GetNum() == 0))
    {
        return RC::RC_ITEM_INVALID_TYPE;    
    };
    //���� ����� ����Ƽ�� �������ΰ�?
    if (!rTargetItemSlot.IsLimited())
        return RC::RC_ITEM_NOT_LIMITED_ITEM;

    // ����� ��ġ ������ üũ
    if (!rTargetItemSlot.IsNeedToRepair())
        return RC::RC_ITEM_DONT_NEED_TO_REPAIR;

#ifdef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT
    //���� ���� Ƚ�� üũ ���ϴ°ɷ� ����
#else
    //���� ���� Ƚ�� üũ
    if (rTargetItemSlot.GetLimitedRepairCount() <= 0)
        return RC::RC_ITEM_OVER_REPAIR_COUNT;
#endif

    //��Ÿ�� üũ
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rChargeItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    //����
    rTargetItemSlot.RepairMaxDura();
    rTargetItemSlot.SetDura(rTargetItemSlot.GetMaxDura());  

    GAMELOG->LogItem(ITEM_USE, root_->player_, &rChargeItemSlot); 

    //���ݼ����� ��ġ(���� ������) ����
    root_->Lose(pContainer, ChargeItemPos, 1);

    WORD wCoolTimeType = rChargeItemSlot.GetItemInfo()->m_wCoolTimeType;

    //��Ÿ��
    root_->player_->ResetUseItem(wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
#ifdef _NA_002050_20110216_ADD_GENDER
// ĳ���� ���� Ȯ�� �������� ����Ѵ�.
RC::eITEM_RESULT nsSlot::ItemFunctionItemProperty::UseExtendCharSlotItem(
    const SLOTIDX inven_type, const POSTYPE inven_position)
{
    if (inven_type != SI_INVENTORY)
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    MSG_DG_OTHER_EXTEND_CHAR_SLOT_SYN msg;
    {
        msg.Init();
        msg.extend_item_position = inven_position;
    }
    root_->player_->SendToGameDBPServer(&msg, msg.GetSize());

    return RC::RC_ITEM_PROCESS_PENDING;
}
#endif