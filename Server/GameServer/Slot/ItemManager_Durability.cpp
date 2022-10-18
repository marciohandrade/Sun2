#include "stdafx.h"
#include "./ItemManager_Durability.h"
#include "ItemManager.h"

//==================================================================================================
#include <ItemInfoParser.h>
#include <ItemAttrCalculator.h>

#include "ChaoState.h"

#include "EquipmentSlotContainer.h"
//_NA_007079_BUG_WEAPON_INACTIVATE_STATE
#include "SkillSlotManager.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::DurabilityImpl) == sizeof(ItemManager*));

// NOTE: this object support the unified transaction
// that is converted from duplicated decrease durability transactions.
struct InterDurabilityTransaction
{
    static const ItemManager::TransactionClosure::value_type kTransType =
        ItemManager::TransactionClosure::eTransType_ChangeDura;
    InterDurabilityTransaction(ItemManager* ptr)
        : root_(ptr)
        , transaction_(NULL)
    {
        ItemManager::TransactionClosure::value_type trans_type =
            root_->GetCurrentTransactionAction();
        if (trans_type == ItemManager::TransactionClosure::eTransType_None) {
            transaction_ = new (buffer_) ItemManager::TransactionClosure(root_, kTransType);
        }
    };
    ~InterDurabilityTransaction()
    {
        if (transaction_) {
            transaction_->~TransactionClosure();
        }
    }
private:
    //
    ItemManager* const root_;
    ItemManager::TransactionClosure* transaction_;
    char buffer_[sizeof(ItemManager::TransactionClosure)];
    //
    __DISABLE_COPY(InterDurabilityTransaction);
};

}; //end of namespace
//==================================================================================================

// work in EquipContainer
void nsSlot::DurabilityImpl::UpdateItemAttributesOnChangesEquippedItemStatus(
    const SCItemSlot& equipped_item)
{
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    SLOTIDX slot_index = equipped_item.GetSlotIdx();
    if (slot_index != SI_EQUIPMENT) {
        return;
    };
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));

    ns_object::ChangedValuesNotifier notifier(root_->player_);
    equip_container->OnChangedEquipItemDurability(equipped_item);
#else //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    __UNUSED(equipped_item);
    root_->UpdateAttrForEquipItem();
#endif
}

bool nsSlot::DurabilityImpl::NotifyChangeDurability(
    POSTYPE equip_pos, SLOTCODE item_code, DURATYPE durability)
{
    MSG_CG_ITEM_CHANGE_DURA_CMD msg_cmd;
    msg_cmd.m_EquipPos = equip_pos;
    msg_cmd.m_ItemCode = item_code;
    msg_cmd.m_Dura = durability;
    return (root_->player_->SendPacket(&msg_cmd, sizeof(msg_cmd)) != false);
}

bool nsSlot::DurabilityImpl::DeleteEquipByDura(POSTYPE equip_pos)
{
    if (root_->player_ == NULL || root_->player_->GetField() == NULL) {
        return false;
    };
    //
    InterDurabilityTransaction transaction(root_);
    //
    if (root_->ValidPos(SI_EQUIPMENT, equip_pos) == false) {
        return false;
    };
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(equip_container->GetSlot(equip_pos));
    // acceptable conditions = { durability = 0, armors, not lucky }
    if (const nsSlot::ItemRuleInfo* item_rule_info = &item_slot.GetItemRuleInfo())
    {
#ifdef _NA_0_20111130_DISTROY_WEAPON
        if (!(item_rule_info->is_armor || item_rule_info->is_weapon))
            return false;
        if (item_rule_info->is_fate)
            return false;
#else
        if (item_rule_info->is_armor == false || item_rule_info->is_fate) {
            return false;
        };
#endif
        DURATYPE durability = item_slot.GetDura();
        if (durability != 0) {
            return false;
        };
    };
    //
    ;{
        int deleted_ratio = item_slot.IsDivine() ? 5 : 10; // percentage
        int dice_value = RandomNumberGenerator::GetRandomNumberRange(1, 100);
        if (dice_value > deleted_ratio) {
            return false; // wait to next turn
        };
    };
    // ACTION: delete item that is not durable
    ;{
        GAMELOG->LogItem(ITEM_DELETE_CAUSE_DURA, root_->player_,
                         &item_slot, 0, item_slot.GetNum());
        // notify to client about the item deletion by decreasing durability
        MSG_CG_ITEM_DESTROY_DURA_CMD msg_cmd;
        msg_cmd.m_SlotIndex = SI_EQUIPMENT;
        msg_cmd.m_SlotPos = equip_pos;
        root_->player_->SendPacket(&msg_cmd, sizeof(msg_cmd));
        // NOTE: side-effect = { total item attributes calculation }
        equip_container->DeleteSlot(equip_pos, NULL);
    };
    //-----------------------------------------------------------
    // WARNING: current state = { corrupted data 'item_slot' }
    //-----------------------------------------------------------

    return true;
}


bool nsSlot::DurabilityImpl::ChangeDurabilityForGM(
    eEQUIPCONTAINER_POS equip_pos, DURATYPE durability)
{
#if !defined(__NA_001062_20080623_STRENGTH_PENALTY)
    return false;
#endif
#ifdef __NA_001062_20080623_STRENGTH_PENALTY
    InterDurabilityTransaction transaction(root_);
    //
    if (!root_->ValidPos(SI_EQUIPMENT, equip_pos)) {
        return false;
    }
    // 공격자의 무기 내구도 감소
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(equip_container->GetSlot(equip_pos));

    if (!item_slot.WithDura()) {
        return false;
    }
    // CHANGES: f080821.x, lst1024, 최대 내구도 이상일때 최대내구도로 셋팅.
    DURATYPE modified_durability = item_slot.GetMaxDura();
    ;        modified_durability = min(modified_durability, durability);
    //
    item_slot.SetDura(modified_durability);
    NotifyChangeDurability(equip_pos, item_slot.GetItemCode(), modified_durability);
    UpdateItemAttributesOnChangesEquippedItemStatus(item_slot);
    //
    return true;
#endif
}

void nsSlot::DurabilityImpl::DecreaseDurabilityUsingReason(
    eDecDuraType decrease_dura_type, eEQUIPCONTAINER_POS equip_pos)
{
    InterDurabilityTransaction transaction(root_);
    //
    if (!root_->ValidPos(SI_EQUIPMENT, equip_pos)) {
        return;
    }
    // 공격자의 무기 내구도 감소
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(equip_container->GetSlot(equip_pos));

    if (!item_slot.WithDura()) {
        return;
    }
    const BYTE current_durability = item_slot.GetDura();
    __UNUSED(current_durability);

    if (item_slot.DecreasingDura(decrease_dura_type))
    {
        const DURATYPE changed_durability = item_slot.GetDura();
#ifdef _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY
#else
        // 방어구 내구도가 0인데 다시 선택되는 경우 소멸된다.
        if (changed_durability == 0 && DeleteEquipByDura(equip_pos)) {
            // 
            return;
        };
#endif //_NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY
        NotifyChangeDurability(equip_pos, item_slot.GetItemCode(), changed_durability);
    #if !defined(__NA_001062_20080623_STRENGTH_PENALTY)
        if (changed_durability == 0)
    #endif
        {
            UpdateItemAttributesOnChangesEquippedItemStatus(item_slot);
        };
    }
}

// NOTE: this method use internal controller that skip the inventory lock instead of 'ValidPos'.
bool nsSlot::DurabilityImpl::DecreaseDurabilityWithRanges(
    const DWORD damage,
    eEQUIPCONTAINER_POS start_equip_pos, eEQUIPCONTAINER_POS last_equip_pos)
{
    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
    if (FlowControl::FCAssert(start_equip_pos <= last_equip_pos &&
                              start_equip_pos < eEQUIPCONTAINER_MAX) == false)
    {
        return false;
    };
    // change a last position to make easy query. this last position change to end position
    eEQUIPCONTAINER_POS end_equip_pos
        = ((last_equip_pos + 1) < eEQUIPCONTAINER_MAX)
        ?    static_cast<eEQUIPCONTAINER_POS>(last_equip_pos + 1)
        :    eEQUIPCONTAINER_MAX;
    //
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));

    const SLOT_LINEAR_LIST& slot_list = equip_container->GetSlotList();
    assert(slot_list.size() >= static_cast<SLOT_LINEAR_LIST::size_type>(end_equip_pos));
    SLOT_LINEAR_LIST::const_iterator it = slot_list.begin() + start_equip_pos;
    const SLOT_LINEAR_LIST::const_iterator end = it + end_equip_pos;

    int number_of_modified_items = 0;
    for (POSTYPE equip_pos = start_equip_pos; it != end; ++equip_pos, ++it)
    {
        SCItemSlot* item_slot = static_cast<SCItemSlot*>(it->slot);
        SLOTCODE item_code = item_slot->GetItemCode();
        if (item_code == 0) {
            continue;
        };
        // NOTE: 'IsLocked' is set game rules, and 'IsBlocked' is forced by abusing control
        if (item_slot->IsLocked() || item_slot->IsBlocked()) {
            continue; // no check LaxLock
        };
        if (item_slot->WithDura() == false) {
            continue;
        };
        //
        if (item_slot->StoreDamageNDecreaseDura(damage) == false) {
            continue;
        };

        const DURATYPE changed_durability = item_slot->GetDura();
        // NOTE: 방어구 내구도가 0인데 다시 선택되는 경우 소멸된다.
#ifdef _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY
#else
        if (changed_durability == 0)
        {
            if (DeleteEquipByDura(equip_pos)) {
                // WARNING: it's possible that 'item_slot' is corrupted
                ++number_of_modified_items;
                continue;
            };
        };
#endif //_NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY
        
        NotifyChangeDurability(equip_pos, item_code, changed_durability);
        UpdateItemAttributesOnChangesEquippedItemStatus(*item_slot);
        //
        ++number_of_modified_items;
    };
    // DEBUG_INFO: number_of_modified_items
    return true;
}

bool nsSlot::DurabilityImpl::DecreaseDuraAll(float decrease_ratio)
{
    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
    //
    if (decrease_ratio == 0.0f) {
        return false;
    };
    // PC방인 경우 내구도 CanApplyPCBangBenefit감소량 50%
    if (root_->player_->CanApplyPCBangBenefit(true)) {
        decrease_ratio *= 0.5f;
    }
    //
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));

    const SLOT_LINEAR_LIST& slot_list = equip_container->GetSlotList();
    SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
    const SLOT_LINEAR_LIST::const_iterator end = slot_list.end();

    int number_of_modified_items = 0;
    bool update_all = false;
    for (POSTYPE equip_pos = 0; it != end; ++equip_pos, ++it)
    {
        SCItemSlot* item_slot = static_cast<SCItemSlot*>(it->slot);
        SLOTCODE item_code = item_slot->GetItemCode();
        if (item_code == 0) {
            continue;
        };
        if (item_slot->WithDura() == false) {
            continue;
        };
        // NOTE: 'IsLocked' is set game rules, and 'IsBlocked' is forced by abusing control
        if (item_slot->IsLocked() || item_slot->IsBlocked()) {
            continue; // no check LaxLock
        };

        const DURATYPE durability = item_slot->GetDura();
#ifdef _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY
#else
        if (durability == 0)
        {
            if (DeleteEquipByDura(equip_pos)) {
                ++number_of_modified_items;
                continue;
            };
        };
#endif //_NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY

        DURATYPE decreasing_dura = static_cast<DURATYPE>(item_slot->GetMaxDura() * decrease_ratio);
        ;        decreasing_dura = max(1, decreasing_dura);
        DURATYPE changed_dura = ((durability > decreasing_dura)
                              ?  (durability - decreasing_dura)
                              :  (0));
        // assertion (decreasing_dura != 0)
        item_slot->SetDura(changed_dura);
        NotifyChangeDurability(equip_pos, item_code, changed_dura);
        if (changed_dura == durability) { // SetDura(0)
            update_all = true;
        }
        else {
        #ifdef __NA_001062_20080623_STRENGTH_PENALTY
            if (update_all == false) {
                UpdateItemAttributesOnChangesEquippedItemStatus(*item_slot);
            };
        #endif
        }
        ++number_of_modified_items;
    };
    // DEBUG_INFO: number_of_modified_items

#ifdef _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD
    //그냥 무조건 업데이터 한다.
    root_->UpdateItemAttributes(true, false);
#else
    if (update_all)
    {
    #ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
        root_->UpdateItemAttributes(true, false);
    #else
        root_->UpdateAttrForEquipItem();
    #endif
    }
    else 
    {
        // already processed in loop
    };
#endif //_NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD


    return true;
}


RC::eITEM_RESULT
nsSlot::DurabilityImpl::Repair(bool total_repairs, SLOTIDX slot_index, POSTYPE slot_pos)
{
    Player* const player = root_->player_;
    //----------------------------------------------------------------------------------------------
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_REPAIR_ITEM)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    };
    if (player->GetChaoState().CanUseFuction(CHAO_USE_STORE) == false) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    };
    if (player->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    };
    //----------------------------------------------------------------------------------------------
    if (total_repairs) {
        slot_index = SI_EQUIPMENT;
    };
    //----------------------------------------------------------------------------------------------
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    if (total_repairs == false && root_->ValidPos(slot_index, slot_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    };
    //----------------------------------------------------------------------------------------------
    MONEY cost_of_total_repairs = 0ULL;
    SCItemSlotContainer* const item_container = root_->GetItemSlotContainer(slot_index);

    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
    const SLOT_LINEAR_LIST& slot_list = item_container->GetSlotList();

    //----------------------------------------------------------------------------------------------
    if (total_repairs == false)
    {
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(*slot_list[slot_pos].slot);
        if (item_slot.IsDestroyItem()) {
            return RC::RC_ITEM_CANNOT_REPAIR_ITEM;
        }
        if (item_slot.WithDura() == false) {
            return RC::RC_ITEM_CANNOT_REPAIR_ITEM;
        };

        DURATYPE durability = item_slot.GetDura();
        DURATYPE max_durability = item_slot.GetMaxDura();
        if (durability == max_durability) {
            return RC::RC_ITEM_DONT_NEED_TO_REPAIR;
        };
        if (item_slot.IsLimited()) {
            if (item_slot.DecreasingMaxDura() == false) {
                return RC::RC_ITEM_CANNOT_REPAIR_ITEM;
            }
        };
        cost_of_total_repairs += item_slot.GetPriceForRepair();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 상점 수리 하임 증감 어트리뷰트 적용
        cost_of_total_repairs = player->GetAttr().GetValueAppliedAttr(cost_of_total_repairs,\
            eATTR_SHOP_REPAIR_HEIM_RATIO);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        // 수리비가 있는지 체크
        if (player->GetMoney() < cost_of_total_repairs) {
            return RC::RC_ITEM_HAVENOTMONEY;
        }
        //
        item_slot.SetDura(max_durability);
        GAMELOG->LogItem(ITEM_DURA, player, &item_slot, cost_of_total_repairs);
        //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
        // 수리하기 전의 내구도가 0이었다면, 수리 후 AddWeapon함수를 호출하여 스타일을 변경시킨다
        if (slot_pos == eEQUIPCONTAINER_WEAPON && durability == 0) {
            player->GetSkillSlotManager()->AddWeapon(item_slot.GetItemCode());
        }
        //
        UpdateItemAttributesOnChangesEquippedItemStatus(item_slot);
        //
    }
    //----------------------------------------------------------------------------------------------
    else
    //----------------------------------------------------------------------------------------------
    {   //total_repairs != false
        SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
        const SLOT_LINEAR_LIST::const_iterator end = slot_list.end();
        for ( ; it != end; ++it)
        {
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(*it->slot);
            if (item_slot.GetItemCode() == 0) {
                continue;
            };
            if (item_slot.IsDestroyItem()) {
                continue;
            }
            if (!item_slot.WithDura()) {
                continue;
            }
            if (item_slot.GetDura() == item_slot.GetMaxDura()) {
                continue;
            }
            if (item_slot.IsLimited())
            {               
                if (item_slot.GetLimitedDuraMax() <= 2) {
                    continue;
                }
            }
            //
            cost_of_total_repairs += item_slot.GetPriceForRepair();
        }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 상점 수리 하임 증감 어트리뷰트 적용
        cost_of_total_repairs = player->GetAttr().GetValueAppliedAttr(cost_of_total_repairs,\
            eATTR_SHOP_REPAIR_HEIM_RATIO);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        // check whether the player have money enough
        if (player->GetMoney() < cost_of_total_repairs) {
            return RC::RC_ITEM_HAVENOTMONEY;
        }
        // do repair
        RepairAllUsingByRepairsItem();
        //
    };
    //----------------------------------------------------------------------------------------------
    // result arrangement
    ;{
        player->MinusMoney(cost_of_total_repairs);
        GAMELOG->LogMoney(MONEY_SPEND_REPAIR, player, cost_of_total_repairs);
        g_pGameServer->NotifyHeimConsumption(player->GetCharGuid(),
                                             eHEIM_CONSUMPTION_REPAIR,
                                             cost_of_total_repairs);
    };
    //
    return RC::RC_ITEM_SUCCESS;
}

// NOTE: the usage case is it of eITEMTYPE_REPAIR item or it of a NPC total repair request.
// if the result is fails, any items have been repaired.
bool nsSlot::DurabilityImpl::RepairAllUsingByRepairsItem()
{
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;

    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));

    const SLOT_LINEAR_LIST& slot_list = equip_container->GetSlotList();
    SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
    const SLOT_LINEAR_LIST::const_iterator end = slot_list.end();

    int number_of_repairs = 0;
    for ( ; it != end; ++it)
    {
        SCItemSlot* item_slot = static_cast<SCItemSlot*>(it->slot);
        if (item_slot->GetItemCode() == 0) {
            continue;
        };

        if (item_slot->IsBlocked()) {
            continue;
        };
        if (item_slot->WithDura() == false) {
            continue;
        };

        if (item_slot->IsLimited())
        {
            if (item_slot->DecreasingMaxDura() == false) {
                continue;
            };
        }

        const DURATYPE cur_durability = item_slot->GetDura();
        const DURATYPE max_durability = item_slot->GetMaxDura();
        if (cur_durability == max_durability) {
            continue;
        };
        //
        item_slot->SetDura(max_durability);
        GAMELOG->LogItem(ITEM_DURA, root_->player_, item_slot, 0);
        //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
        // 수리하기 전의 내구도가 0이었다면, 수리 후 AddWeapon함수를 호출하여 스타일을 변경시킨다
        POSTYPE item_pos = item_slot->GetPos();
        if (item_pos == eEQUIPCONTAINER_WEAPON && cur_durability == 0) {
            root_->player_->GetSkillSlotManager()->AddWeapon(item_slot->GetItemCode());
        }
        //
        ++number_of_repairs;
        //
    };
    //
    if (number_of_repairs == 0) {
        return false;
    };
    // success
    root_->UpdateItemAttributes(true, false);
    //
    return true;
    //
#else //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    CEquipmentSlotContainer* const equip_container =
        static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));

    ItemAttrCalculator itemCalc(root_->player_->GetPlayerAttr(), *equip_container);

    bool repaired = false;
    // 수리한다.
    const POSTYPE number_of_max_slots = equip_container->GetMaxSlotNum();
    DURATYPE current_durability = 0;
    for (POSTYPE i = 0 ; i < number_of_max_slots; ++i)
    {
        if (equip_container->IsEmpty(i)) {
            continue;
        };
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(equip_container->GetSlot(i));
        if (item_slot.IsBlocked()) {
            continue;   //< 블럭된 아이템일경우 수리안함
        };
        if (!item_slot.WithDura()) {
            continue;
        };
        current_durability = item_slot.GetDura();
        if (current_durability == item_slot.GetMaxDura()) {
            continue;
        };
        GAMELOG->LogItem(ITEM_DURA, root_->player_, &item_slot, 0);

        if (item_slot.IsLimited())
        {
            if (!item_slot.DecreasingMaxDura()) {
                continue;
            }
        }

        item_slot.SetDura(item_slot.GetMaxDura());

        repaired = true;
    }

    if (repaired) {
        root_->UpdateAttrForEquipItem();
    };
    return repaired;
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
}

//==================================================================================================
