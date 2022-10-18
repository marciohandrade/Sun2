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

    //랭크하향 망치를 사용했는가?
    if (rChargeItemSlot.GetItemInfo()->m_wType != eITEMTYPE_RANK_DOWN ||
        (rChargeItemSlot.GetNum() == 0))
    {
        return RC::RC_ITEM_INVALID_TYPE;
    };
    //쿨타임 체크
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rChargeItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;    

    WORD wCoolTimeType = rChargeItemSlot.GetItemInfo()->m_wCoolTimeType;

    RC::eITEM_RESULT rt = root_->RankDown(TargetPos);

    if (rt != RC::RC_ITEM_SUCCESS)
    {
        return rt;
    }

    //랭크업 망치(유료 아이템) 삭제
    root_->Lose(pContainer, ChargeItemPos, 1);

    //쿨타임
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

    //연금술사의 망치를 사용했는가?
    if (rChargeItemSlot.GetItemInfo()->m_wType != eITEMTYPE_HAMMER_OF_ALCHEMIST ||
        (rChargeItemSlot.GetNum() == 0))
    {
        return RC::RC_ITEM_INVALID_TYPE;    
    };
    //수리 대상이 리미티드 아이템인가?
    if (!rTargetItemSlot.IsLimited())
        return RC::RC_ITEM_NOT_LIMITED_ITEM;

    // 명공의 망치 내구도 체크
    if (!rTargetItemSlot.IsNeedToRepair())
        return RC::RC_ITEM_DONT_NEED_TO_REPAIR;

#ifdef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT
    //수리 가능 횟수 체크 안하는걸로 변경
#else
    //수리 가능 횟수 체크
    if (rTargetItemSlot.GetLimitedRepairCount() <= 0)
        return RC::RC_ITEM_OVER_REPAIR_COUNT;
#endif

    //쿨타임 체크
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rChargeItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    //수리
    rTargetItemSlot.RepairMaxDura();
    rTargetItemSlot.SetDura(rTargetItemSlot.GetMaxDura());  

    GAMELOG->LogItem(ITEM_USE, root_->player_, &rChargeItemSlot); 

    //연금술사의 망치(유료 아이템) 삭제
    root_->Lose(pContainer, ChargeItemPos, 1);

    WORD wCoolTimeType = rChargeItemSlot.GetItemInfo()->m_wCoolTimeType;

    //쿨타임
    root_->player_->ResetUseItem(wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
#ifdef _NA_002050_20110216_ADD_GENDER
// 캐릭터 슬롯 확장 아이템을 사용한다.
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