#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionEtherWeapon) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionEtherWeapon::ChargeEtherBullet(SCItemSlot& rBulletSlot)
{
    //changed rule by __NA_1119_CHANGE_ETHER_EXPEND_STYLE
    // 소비 방식이 단발에서 연발로 바뀌어 사용 안함
    return RC::RC_ITEM_FAILED;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionEtherWeapon::EquipEtherDeviceToWeapon(
    POSTYPE device_item_pos, POSTYPE target_pos)
{
    // (CHANGES) (f100805.3L) arrangment and prevent use of an ether device
    // intended an etheria item or a shell item.
    SCSlotContainer* const item_container = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (!root_->ValidPos(SI_INVENTORY, device_item_pos)) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    if (!root_->ValidPos(SI_INVENTORY, target_pos)) {
        return RC::RC_ITEM_INVALIDPOS;
    };
    //
    SCItemSlot& device_item_slot = \
        static_cast<SCItemSlot&>(item_container->GetSlot(device_item_pos));
    const BASE_ITEMINFO* device_item_info = device_item_slot.GetItemInfo();
    // check the ether device
    if (device_item_info->m_Code == 0 || device_item_info->m_wType != eITEMTYPE_ETHER_DEVICE) {
        // skip check number '(device_item_slot.GetNum() == 0)'
        return RC::RC_ITEM_INVALID_TYPE;
    }
    //
    SCItemSlot& target_item_slot = \
        static_cast<SCItemSlot&>(item_container->GetSlot(target_pos));
    const BASE_ITEMINFO* target_item_info = target_item_slot.GetItemInfo();
    const nsSlot::ItemRuleInfo& item_rule_info = target_item_slot.GetItemRuleInfo();
    if (item_rule_info.is_weapon == false ||
        // (NOTE) other item operations have data field to control possible
        // which is related to 'TradeSell' and ''
        (item_rule_info.is_etheria || item_rule_info.is_shell))
    {
        return RC::RC_ITEM_ETHER_DEVICE_USE_ONLY_TO_WEAPON;
    }
    // is already the weapon item is junctioned with the ether device
    if (target_item_slot.IsEquipEtherDevice()) {
        return RC::RC_ITEM_ALREADY_EQUIP_ETHER_DEVICE;
    }
    //
    RC::eITEM_RESULT item_result = root_->player_->CanUseItem(device_item_info);
    if (item_result != RC::RC_ITEM_SUCCESS) {
        return item_result;
    }
    //------------
    // equip
    target_item_slot.SetEquipEtherDevice(true);
    //
    ;{
        //쿨타임
        WORD cool_time_type = device_item_info->m_wCoolTimeType;
        root_->player_->ResetUseItem(cool_time_type);
    };
    //로그
    GAMELOG->LogItemEx(ITEM_USE, root_->player_, device_item_slot, target_item_slot);
    //(유료 아이템) 삭제
    root_->Lose(item_container, device_item_pos, 1);
    //
    return RC::RC_ITEM_SUCCESS;
}
