// EquipmentSlotContainer.cpp: implementation of the CEquipmentSlotContainer class.
//
//==================================================================================================
#include "stdafx.h"
#include "EquipmentSlotContainerEP2.h"

//==================================================================================================
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================

#include <ItemInfoParser.h>
#include <ItemFormula.h>

#include <SCItemSlot.h>
#include <SCQuickSlot.h>

#include "Player.h"
#include "GameField.h"

#include "SkillSlotManager.h"
#include "QuickManager.h"
#include "QuickSlotContainer.h"
#include "ItemManager.h"
#include "StatusManager.h"

//==================================================================================================

BOOST_STATIC_ASSERT(eEQUIPCONTAINER_MAX == MAX_EQUIPMENT_SLOT_NUM);
// NOTES: f110422.2L, added constants reference information about default renderable equips.
// NOTES: old-comments, 엘리트 아이템 풀셋일 경우 효과 추가 (2007.08.03 - 김창현, 전보환)
//                      = { eEQUIPCONTAINER_PROTECTOR, eEQUIPCONTAINER_BELT, eEQUIPCONTAINER_SHIRTS }
const bool CEquipmentSlotContainer::kDefaultRenderableEquipItems[MAX_EQUIPMENT_SLOT_NUM] =
{
    true,  // eEQUIPCONTAINER_WEAPON      = 0,
    true,  // eEQUIPCONTAINER_ARMOR       = 1,
    true,  // eEQUIPCONTAINER_PROTECTOR   = 2,
    true,  // eEQUIPCONTAINER_HELMET      = 3,
    true,  // eEQUIPCONTAINER_PANTS       = 4,
    true,  // eEQUIPCONTAINER_BOOTS       = 5,
    true,  // eEQUIPCONTAINER_GLOVE       = 6,
    true,  // eEQUIPCONTAINER_BELT        = 7,
    true,  // eEQUIPCONTAINER_SHIRTS      = 8,
#ifdef _NA_002935_20110704_ITEM_SCORE 
    true,  // eEQUIPCONTAINER_RING1       = 9,
    true,  // eEQUIPCONTAINER_RING2       = 10,
    true,  // eEQUIPCONTAINER_NECKLACE    = 11,
#else
    false, // eEQUIPCONTAINER_RING1       = 9,
    false, // eEQUIPCONTAINER_RING2       = 10,
    false, // eEQUIPCONTAINER_NECKLACE    = 11,
#endif
    false, // eEQUIPCONTAINER_BOW         = 12,
    true,  // eEQUIPCONTAINER_SACCESSORY1 = 13,
    true,  // eEQUIPCONTAINER_SACCESSORY2 = 14,
    true,  // eEQUIPCONTAINER_SACCESSORY3 = 15,
    true,  // eEQUIPCONTAINER_CHARGE1     = 16,
    true,  // eEQUIPCONTAINER_CHARGE2     = 17,
    true,  // eEQUIPCONTAINER_CHARGE3     = 18,
    true,  // eEQUIPCONTAINER_CHARGE4     = 19,
    true,  // eEQUIPCONTAINER_CHARGE5     = 20,
#ifdef _NA_002935_20110704_ITEM_SCORE
    true,  // eEQUIPCONTAINER_PC_ROOM1    = 21,
    true,  // eEQUIPCONTAINER_PC_ROOM2    = 22,
    true,  // eEQUIPCONTAINER_PC_ROOM3    = 23,
#else
    false, // eEQUIPCONTAINER_PC_ROOM1    = 21,
    false, // eEQUIPCONTAINER_PC_ROOM2    = 22,
    false, // eEQUIPCONTAINER_PC_ROOM3    = 23,
#endif
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace nsSlot {
;

// CHANGES: f110422.2L, ChangedRenderableItemsNotifier. when items are changed attributes,
// renderable equip items may occur multiple changes.
struct ChangedRenderableItemsNotifier
{
    enum eCtrls { eCtrl_None, eCtrl_Activated, eCtrl_Inactivated, eCtrl_Counts };

    ChangedRenderableItemsNotifier(
        const nsSlot::EquipExistences& input_original_existences,
        const ns_formula::EquipSlotsManageInfo* input_original_states,
        Player* player);
    bool OnChangeEquipments(const nsSlot::EquipExistences& input_changed_existences,
                            const ns_formula::EquipSlotsManageInfo* input_changed_states);
    bool Broadcast(Player* player, const SCItemSlotContainer::SLOT_LINEAR_LIST& slot_list);
    //
    bool validated;
    EquipExistences original_existences;
    uint8_t original_activates[MAX_EQUIPMENT_SLOT_NUM];
    uint8_t need_notification_list[MAX_EQUIPMENT_SLOT_NUM]; // ref: eCtrls

private:
    ChangedRenderableItemsNotifier();
    //
    __DISABLE_COPY(ChangedRenderableItemsNotifier);
};

}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================

CEquipmentSlotContainer::CEquipmentSlotContainer()
    : player_(NULL)
{
    Init(MAX_EQUIPMENT_SLOT_NUM, SI_EQUIPMENT);
    ZeroMemory(&equip_existences_, sizeof(equip_existences_));
}

CEquipmentSlotContainer::~CEquipmentSlotContainer()
{
}

void CEquipmentSlotContainer::ClearAll()
{
    SCItemSlotContainer::ClearAll();
    equip_slots_manage_info_.ClearAll();
    ZeroMemory(&equip_existences_, sizeof(equip_existences_));
}

//==================================================================================================

// CHANGES: f110422.2L, refresh CEquipmentSlotContainer::equip_existences_
void CEquipmentSlotContainer::RefreshEquipItemsCache()
{
    ZeroMemory(&equip_existences_, sizeof(equip_existences_));

    const SCItemSlotContainer::SLOT_LINEAR_LIST& slot_list = GetSlotList();
    SCItemSlotContainer::SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
    const SCItemSlotContainer::SLOT_LINEAR_LIST::const_iterator end = slot_list.end();
    for (int pos = 0; it != end; ++pos, ++it)
    {
        SCItemSlot* const item_slot = static_cast<SCItemSlot*>(it->slot);
        if (item_slot->GetItemCode() == 0) {
            continue;
        };

        equip_existences_.equipped[pos] = true;
        item_slot->OnEquip();
    }
}

//==================================================================================================

RC::eSLOT_INSERT_RESULT
CEquipmentSlotContainer::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    SASSERT(player_ != NULL, "[CEquipmentSlotContainer::InsertSlot] player_ == NULL");

    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    bool prev_weapon_activated = true;
    if (IsActivated(eEQUIPCONTAINER_WEAPON) == false) {
        prev_weapon_activated = false;
    }
    if (CanEquip(at_pos, static_cast<SCItemSlot&>(slot)) == false) {
        return RC::RC_INSERT_SLOT_CAN_NOT_EQUIP;
    };
    // CHANGES: f110422.2L
    nsSlot::ChangedRenderableItemsNotifier g2c_notifier(
        equip_existences_, &equip_slots_manage_info_, player_);
    //
    RC::eSLOT_INSERT_RESULT insert_result = SCSlotContainer::InsertSlot(at_pos, slot);
    if (insert_result != RC::RC_INSERT_SLOT_SUCCESS) {
        return insert_result;
    }
    equip_existences_.equipped[at_pos] = true;
    //
    SCItemSlot& current_slot = static_cast<SCItemSlot&>(GetSlot(at_pos));
    if (current_slot.OnEquip()) {
        GAMELOG->LogItem(ITEM_CHARGE_FIRST_EQUIP, player_, &current_slot);
    }
    if (SCQuickSlotContainer* quick_container =
            player_->GetQuickManager()->GetQuickSlotContainer())
    {
        quick_container->UpdateQuickSlot(current_slot.GetSlotIdx(), current_slot.GetPos(),
            current_slot.GetItemCode(), SI_EQUIPMENT, at_pos, current_slot.GetItemCode());
    };
    //
    bool pc_room_status = (player_->CanApplyPCBangBenefit(true) != false);
    ItemAttrCalculator item_calculator(player_->GetPlayerAttr(), this);
    item_calculator.Equip2(current_slot, pc_room_status,
                           true, player_->GetLevel());
    // CHANGES: f110422.2L
    g2c_notifier.OnChangeEquipments(equip_existences_, &equip_slots_manage_info_);
    // current state
    // current_slot changes = { empty -> slot }

    // 무기가 변경/비활성화 되었을 경우 스타일 셋팅과 패시브 적용이 달라진다.
    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    SCItemSlot& weapon_slot = static_cast<SCItemSlot&>(GetSlot(eEQUIPCONTAINER_WEAPON));

    if (at_pos == eEQUIPCONTAINER_WEAPON) 
    {
        if (IsActivated(eEQUIPCONTAINER_WEAPON) == true && weapon_slot.GetDura() != 0) {
            player_->GetSkillSlotManager()->AddWeapon(current_slot.GetItemCode());
        }
    }
    else if (prev_weapon_activated == false && IsActivated(eEQUIPCONTAINER_WEAPON) == true)
    {
        // 다른 장비의 변경으로 무기 활성/비활성이 변경되었을 경우도 AddWeapon을 호출한다
        player_->GetSkillSlotManager()->AddWeapon(weapon_slot.GetItemCode());
    }

    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            player_->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddSerialCode(eSERIAL_EQUIPMENT, GetSlotIdx(), current_slot.GetPos(),
                                         current_slot.GetWorldSerial());
    };
    // CHANGES: f110521.3L, f110520.3L, when the equipments of player changed,
    // the relation of equipments may occur wings controls.
    IVehicleEvent::OnEvent(player_, IVehicleEvent::ChangeEquips());
    // CHANGES: f110422.2L
    g2c_notifier.Broadcast(player_, GetSlotList());
    //
    player_->GetPlayerAttr().Display();
    //
    return insert_result;
}

BOOL CEquipmentSlotContainer::DeleteSlot(POSTYPE at_pos, SCSlot* result_deleted_slot)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));

    SCItemSlot temp_delete_item_slot;
    // CHANGES: f110422.2L
    nsSlot::ChangedRenderableItemsNotifier g2c_notifier(
        equip_existences_, &equip_slots_manage_info_, player_);
    //
    if (SCSlotContainer::DeleteSlot(at_pos, &temp_delete_item_slot) == false) {
        return false;
    };
    equip_existences_.equipped[at_pos] = false;
    //
    temp_delete_item_slot.OnUnEquip();
    //
    if (result_deleted_slot != NULL) {
        static_cast<SCItemSlot*>(result_deleted_slot)->Copy(temp_delete_item_slot);
    };
    //
    ItemAttrCalculator item_calculator(player_->GetPlayerAttr(), this);

    bool pc_room_status = (player_->CanApplyPCBangBenefit(true) != false);
    item_calculator.Unequip2(&temp_delete_item_slot, at_pos,
        pc_room_status, true, player_->GetLevel());

    // CHANGES: f110422.2L
    g2c_notifier.OnChangeEquipments(equip_existences_, &equip_slots_manage_info_);

    // 무기가 변경/비활성화 되었을 경우 스타일 셋팅과 패시브 적용이 달라진다.
    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    if (at_pos == eEQUIPCONTAINER_WEAPON ||
        (IsEquipped(eEQUIPCONTAINER_WEAPON) == true && IsActivated(eEQUIPCONTAINER_WEAPON) == false)) {
        player_->GetSkillSlotManager()->DelWeapon();
    };

    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        player_->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveSerialCode(GetSlotIdx(), at_pos,
            temp_delete_item_slot.GetWorldSerial());
    };
    //
    if (const BASE_ITEMINFO* item_info = temp_delete_item_slot.GetItemInfo())
    {
        // 벗은 아이템이 버프이벤트 아이템일 경우면
        if (BYTE event_code = item_info->m_byEventCode)
        {
            // 같은 종류의 이벤트 아이템을 입고 있지 않다면 상태 해제
            if (IsEquipEventItem(event_code) == false) {
                player_->GetStatusManager()->RemoveEventStatus(event_code);
            }
        }
    }
    // CHANGES: f110521.3L, f110520.3L, when the equipments of player changed,
    // the relation of equipments may occur wings controls.
    IVehicleEvent::OnEvent(player_, IVehicleEvent::ChangeEquips());
    // CHANGES: f110422.2L
    g2c_notifier.Broadcast(player_, GetSlotList());
    //
    player_->GetPlayerAttr().Display();
    //
    return true;
}

BOOL CEquipmentSlotContainer::ValidState()
{   // 예) PK시 드랍되는 ITEM이 장비일 경우... CanUnEquip에서 사용하면 안된다.
    // 패킷에서 사용할 수 있도록 하자.
    // CHANGES: f091202.x, WOPS:5334, 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    ItemManager::TransactionClosure::value_type transaction =
        player_->GetItemManager()->GetCurrentTransactionAction();
    if (transaction == ItemManager::TransactionClosure::eTransType_None)
    {
        ProtectorInventory& protector = player_->GetWarehouseInventoryProtector().INVENTORY();
        if (protector.IsUsable() == false) {
            return false;
        }
    }
    return SCItemSlotContainer::ValidState();
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// IEquipmentRestriction interface
SCItemSlotContainer* CEquipmentSlotContainer::GetEquipmentSlotContainer()
{
    return this;
}

ns_formula::EquipSlotsManageInfo* CEquipmentSlotContainer::GetEquipmentManageInfo()
{
    return &equip_slots_manage_info_;
}

// IEquipmentRestriction interface
// check contents level enabled equip condition (no check stats+level)
bool CEquipmentSlotContainer::CheckEquipDefaultValidationWithoutStats(
    POSTYPE at_pos, const SCItemSlot& IN item_slot)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    if (StatusManager* status_manager = player_->GetStatusManager()) {
        // 드래곤 변신상태에서는 장착슬롯을 이용할 수 없다.
        if (status_manager->Status.IsDragonTransforming()) {
#ifdef _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS
#else
            return false;
#endif // _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS            
        }
    }
    //----------------------------------------------------------------------------------------------
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    // CHANGES: use BASE_ITEMINFO interface, ref eITEM_MATERIAL_TYPE + for rider info
    if (item_slot.CanEquip() == false) {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    const BASE_PLAYERINFO* const player_info = player_->GetCharInfo();
#ifdef _NA_002050_20110216_ADD_GENDER
    // 장착 가능한 성별인지 검사한다.
    if (item_info->IsCanUseGender(player_info->gender) == false) {
        return false;
    }
#endif
    //----------------------------------------------------------------------------------------------
    const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(player_info->m_byClassCode);
    // CHANGES: f110422.2L, changes to easy check, class별 장착 여부
    if (item_info->CheckValidEquipClass(char_type) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Invalid char_type on check equip|Class=%d|",
               char_type);
        return false;
    };
    //----------------------------------------------------------------------------------------------
    switch (const eEQUIPCONTAINER_POS equip_pos =
                static_cast<eEQUIPCONTAINER_POS>(item_info->m_wEqPos))
    {
        //-------------------------------------------------
    case eEQUIPCONTAINER_RING1:
    case eEQUIPCONTAINER_RING2:
        if (at_pos != eEQUIPCONTAINER_RING1 &&
            at_pos != eEQUIPCONTAINER_RING2)
        {
            return false;
        }
        break;
        //-------------------------------------------------
    case eEQUIPCONTAINER_SACCESSORY1:
    case eEQUIPCONTAINER_SACCESSORY2:
    case eEQUIPCONTAINER_SACCESSORY3:
        if (at_pos != eEQUIPCONTAINER_SACCESSORY1 &&
            at_pos != eEQUIPCONTAINER_SACCESSORY2 &&
            at_pos != eEQUIPCONTAINER_SACCESSORY3)
        {
            return false;
        }
    #ifdef _NA001955_110210_WING_COSTUME_ACCESSORY_EXTENTION
    #else
        if (char_type != eCHAR_ELEMENTALIST &&
            (at_pos == eEQUIPCONTAINER_SACCESSORY2 || at_pos == eEQUIPCONTAINER_SACCESSORY3))
        {
            return false;
        }
    #endif
        break;
        //-------------------------------------------------
    case eEQUIPCONTAINER_CHARGE1:
    case eEQUIPCONTAINER_CHARGE2:
    case eEQUIPCONTAINER_CHARGE3:
    case eEQUIPCONTAINER_CHARGE4:
    case eEQUIPCONTAINER_CHARGE5:
        if (at_pos != eEQUIPCONTAINER_CHARGE1 &&
            at_pos != eEQUIPCONTAINER_CHARGE2 &&
            at_pos != eEQUIPCONTAINER_CHARGE3 &&
            at_pos != eEQUIPCONTAINER_CHARGE4 &&
            at_pos != eEQUIPCONTAINER_CHARGE5)
        {
                return false;
        }

        if (at_pos == eEQUIPCONTAINER_CHARGE4 &&
            player_info->m_ExtraEquipCashSlotCount < 1)
        {
            return false;
        }

        if (at_pos == eEQUIPCONTAINER_CHARGE5 &&
            player_info->m_ExtraEquipCashSlotCount < 2)
        {
            return false;
        }
        break;
        //-------------------------------------------------
    case eEQUIPCONTAINER_PC_ROOM1:
    case eEQUIPCONTAINER_PC_ROOM2:
    case eEQUIPCONTAINER_PC_ROOM3:  
        if (at_pos != eEQUIPCONTAINER_PC_ROOM1 &&
            at_pos != eEQUIPCONTAINER_PC_ROOM2 &&
            at_pos != eEQUIPCONTAINER_PC_ROOM3)
        {
            return false;
        }

        if (player_->CanApplyPCBangBenefit(true) == false) {
            return false;
        }
        break;
        //-------------------------------------------------
    default:
        if (item_info->m_wEqPos != at_pos) {
            return false;
        }
        break;
        //-------------------------------------------------
    }
    //----------------------------------------------------------------------------------------------
    // CHANGES: f110429.2L, roll-backed. using external equipment limitation conditions can check
    // whether this item enable equips.
    if (equip_slots_manage_info_.CanEquipmentsDependency(item_slot) == false) {
        return false;
    };
    return true;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

bool CEquipmentSlotContainer::CanEquip(POSTYPE at_pos, const SCItemSlot& IN item_slot)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
    ItemManager::TransactionClosure::value_type transaction =
        player_->GetItemManager()->GetCurrentTransactionAction();
    if (transaction == ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation) {
        return true;
    };
    //
    if (ValidLimitStats(item_slot) == false) {
        return false;
    };
    if (CheckEquipDefaultValidationWithoutStats(at_pos, item_slot) == false) {
        return false;
    };

    return true;
}

// Checking list = { STR, DEX, VIT, INT, SPR, Level }
bool CEquipmentSlotContainer::ValidLimitStats(const SCItemSlot& item_slot) const
{
    if (ItemAttrCalculator::CanEquipmentValidStats(item_slot,
            player_->GetPlayerAttr(), player_->GetLevel()) == false)
    {
        return false;
    };
    return true;
}

void CEquipmentSlotContainer::CheckupActiveEquipItemAll()
{
    // CHANGES: f110422.2L
    nsSlot::ChangedRenderableItemsNotifier g2c_notifier(
        equip_existences_, &equip_slots_manage_info_, player_);

    bool pc_room_status = (player_->CanApplyPCBangBenefit(true) != false);
    ItemAttrCalculator item_calculator(player_->GetPlayerAttr(), this);
    item_calculator.EquipAll(pc_room_status, true, player_->GetLevel());
    // CHANGES: f110422.2L
    g2c_notifier.OnChangeEquipments(equip_existences_, &equip_slots_manage_info_);
    // CHANGES: f110521.3L, f110520.3L, when the equipments of player changed,
    // the relation of equipments may occur wings controls.
    IVehicleEvent::OnEvent(player_, IVehicleEvent::ChangeEquips());
    g2c_notifier.Broadcast(player_, GetSlotList());
}

bool CEquipmentSlotContainer::OnChangedEquipItemDurability(const SCItemSlot& item_slot)
{
    const SLOTIDX slot_index = item_slot.GetSlotIdx();
    if (FlowControl::FCAssert(slot_index == SI_EQUIPMENT) == false) {
        return false;
    };
    const POSTYPE pos = item_slot.GetPos();
    if (FlowControl::FCAssert(pos < MAX_EQUIPMENT_SLOT_NUM) == false) {
        return false;
    };
    // CHANGES: f110422.2L
    nsSlot::ChangedRenderableItemsNotifier g2c_notifier(
        equip_existences_, &equip_slots_manage_info_, player_);
    //
    bool pc_room_status = (player_->CanApplyPCBangBenefit(true) != false);
    ItemAttrCalculator item_calculator(player_->GetPlayerAttr(), this);
    bool processed = item_calculator.ChangeItemDurabilityStatus(item_slot, pos,
        pc_room_status, true, player_->GetLevel());
    // CHANGES: f110422.2L
    g2c_notifier.OnChangeEquipments(equip_existences_, &equip_slots_manage_info_);
    // CHANGES: f110521.3L, f110520.3L, when the equipments of player changed,
    // the relation of equipments may occur wings controls.
    IVehicleEvent::OnEvent(player_, IVehicleEvent::ChangeEquips());
    g2c_notifier.Broadcast(player_, GetSlotList());
    //

    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    if (pos == eEQUIPCONTAINER_WEAPON && item_slot.GetDura() == 0) {
        player_->GetSkillSlotManager()->DelWeapon();
    }
    return processed;
}

void CEquipmentSlotContainer::OnInsert(SCSlot& IN slot)
{
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            player_->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddExpirationItem(static_cast<SCItemSlot&>(slot));
    };
    OnUpdate(slot, 1);
}

void CEquipmentSlotContainer::OnDelete(SCSlot& IN slot)
{
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            player_->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveExpirationItem(static_cast<SCItemSlot&>(slot));
    };
}

// CHANGES: f110603.5L, added to support wings control
void CEquipmentSlotContainer::OnUpdate(SCSlot& IN slot, int changed_num)
{
    __UNUSED(changed_num);
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot);
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_wType == eITEMTYPE_WINGS && item_slot.IsUserBlocked())
    {
        ePLAYER_STATE player_state = static_cast<ePLAYER_STATE>(player_->GetState());
        if (player_state == PLAYER_CHARINFO_FIRST_REQUESTED ||
            player_state == PLAYER_CHARINFO_REQUESTED)
        {
            item_slot.SetUserBlocked(false);
        };
    };
}

//_NA_0_20100722_AUTO_SYSTEM
POSTYPE CEquipmentSlotContainer::GetPosEquipByItemType(
    eITEMTYPE find_type, POSTYPE start_pos, POSTYPE end_pos) const
{
    const POSTYPE kFailedResult = eEQUIPCONTAINER_INVALID;
    if (FlowControl::FCAssert(start_pos < end_pos && end_pos <= eEQUIPCONTAINER_MAX) == false) {
        return kFailedResult;
    };
    //
    const SLOT_LINEAR_LIST& slot_list = GetSlotList();
    SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
    const SLOT_LINEAR_LIST::const_iterator end = it + end_pos;
    it += start_pos;
    for (POSTYPE pos = start_pos; it != end; ++pos, ++it)
    {
        const SCItemSlot* item_slot = static_cast<SCItemSlot*>(it->slot);
        if (item_slot->GetItemCode() == 0) {
            continue;
        };
        const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
        if (item_info->m_wType == find_type) {
            return pos;
        };
    }
    return kFailedResult;
}

bool CEquipmentSlotContainer::IsEquipEventItem(BYTE event_code) const
{
    return equip_slots_manage_info_.IsEquipEventItem(event_code);
}
//==================================================================================================
//==================================================================================================
// CHANGES: f110422.2L, ChangedRenderableItemsNotifier. when items are changed attributes,
// renderable equip items may occur multiple changes.
//==================================================================================================

nsSlot::ChangedRenderableItemsNotifier::ChangedRenderableItemsNotifier(
    const nsSlot::EquipExistences& input_original_existences,
    const ns_formula::EquipSlotsManageInfo* input_original_states,
    Player* player)
{
    this->validated = false;
    if (player->GetField() == NULL) {
        return;
    };
    ZeroMemory(this, sizeof(*this));
    this->validated = true;
    //
    this->original_existences = input_original_existences;
    //
    uint8_t* dest_it = this->original_activates;
    for (const ns_formula::EquipSlotsManageInfo::Node* it = input_original_states->slots;
            it != &input_original_states->slots[_countof(input_original_states->slots)];
            ++it, ++dest_it)
    {
        *dest_it = it->activated;
    }
};

bool nsSlot::ChangedRenderableItemsNotifier::OnChangeEquipments(
    const nsSlot::EquipExistences& input_changed_existences,
    const ns_formula::EquipSlotsManageInfo* input_changed_states)
{
    if (this->validated == false) {
        return false;
    };
    uint8_t* exist_l = this->original_existences.equipped;
    const uint8_t* exist_r = input_changed_existences.equipped;
    uint8_t* activates_l = this->original_activates;
    const ns_formula::EquipSlotsManageInfo::Node* activates_r = input_changed_states->slots;
    uint8_t* notification = need_notification_list;

    for (int pos = 0; pos < _countof(original_activates);
            ++pos, ++exist_l, ++exist_r, ++activates_l, ++activates_r, ++notification)
    {
        if (CEquipmentSlotContainer::kDefaultRenderableEquipItems[pos] == false) {
            continue;
        };
        //
        if (*exist_l != *exist_r)
        {
            if (*exist_l < *exist_r) { // inserted
                if (activates_r->activated) {
                    *notification = eCtrl_Activated;
                    continue;
                };
            }
            else { // deleted
                *notification = eCtrl_Inactivated;
                continue;
            };
        }
        else
        {
            const uint8_t right_activated = (activates_r->activated ? 1 : 0);
            if (*activates_l != right_activated)
            {
                // changes to activates
                if (*activates_l < right_activated) {
                    *notification = eCtrl_Activated;
                    continue;
                }
                // changes to inactivates
                else {
                    *notification = eCtrl_Inactivated;
                    continue;
                };
            }
        };
    } //end 'for'
    //
    return true;
};
//
bool nsSlot::ChangedRenderableItemsNotifier::Broadcast(Player* player,
    const SCItemSlotContainer::SLOT_LINEAR_LIST& slot_list)
{
    if (validated == false) {
        return false;
    };
    //
    MSG_CG_CHARINFO_APPREARANCE_LIST_BRD msg_act;
    MSG_CG_CHARINFO_DISAPPREARANCE_LIST_BRD msg_inact;

    if (uint16_t player_key = static_cast<uint16_t>(player->GetObjectKey()))
    {
        msg_act.player_key = player_key;
        msg_act.number_of_items = 0;
        msg_inact.player_key = player_key;
        msg_inact.number_of_items = 0;
#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
        CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
            player->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
        const ns_formula::EquipSlotsManageInfo* const equip_manage_info = \
            equip_container->GetEquipmentManageInfo();
        const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = \
            equip_manage_info->item_score_info;

        msg_act.total_item_score_ = item_score_info.GetTotalScore();
        msg_inact.total_item_score_ = item_score_info.GetTotalScore();
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE
    };

    MSG_CG_CHARINFO_APPREARANCE_LIST_BRD::Node* act_it = msg_act.nodes;
    MSG_CG_CHARINFO_DISAPPREARANCE_LIST_BRD::Node* inact_it = msg_inact.nodes;

    const SCItemSlotContainer::SLOT_LINEAR_LIST::const_iterator begin_it = slot_list.begin();
    for (const uint8_t* it = this->need_notification_list;
         it != &this->need_notification_list[_countof(need_notification_list)];
         ++it)
    {
        BOOST_STATIC_ASSERT(eCtrl_None == 0);
        const POSTYPE pos = static_cast<POSTYPE>(it - this->need_notification_list);
        const eCtrls ctrl = static_cast<eCtrls>(*it);
        ;;;; if (ctrl == this->eCtrl_None) {
            continue;
        }
        else if (ctrl == this->eCtrl_Activated) {
            SCItemSlotContainer::SLOT_LINEAR_LIST::const_iterator slot_it =
                begin_it + pos;
            const SCItemSlot* item_slot = static_cast<const SCItemSlot*>(slot_it->slot);
            item_slot->CopyOut(act_it->render_item);
            act_it->pos = pos;
            ++act_it;
        }
        else if (ctrl == this->eCtrl_Inactivated) {
            inact_it->pos = pos;
            ++inact_it;
        }
        else {
            assert(!"unexpected type");
        };
    }; //end 'foreach need_notification_list'
    //
    msg_act.number_of_items = static_cast<uint8_t>(act_it - msg_act.nodes);
    msg_inact.number_of_items = static_cast<uint8_t>(inact_it - msg_inact.nodes);
    // TODO: after version 1102, 'MSG_CG_CHARINFO_APPREARANCE_BRD' and
    // 'MSG_CG_CHARINFO_DISAPPREARANCE_BRD' will be removed.
    if (msg_act.number_of_items)
    {
        player->SendPacketAround(&msg_act, msg_act.GetSize(), false);
    };
    if (msg_inact.number_of_items)
    {
        player->SendPacketAround(&msg_inact, msg_inact.GetSize(), false);
    };

    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================



//==================================================================================================
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
//==================================================================================================
