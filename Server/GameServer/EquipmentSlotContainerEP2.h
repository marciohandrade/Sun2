#ifndef GAMESERVER_EQUIP_SLOTCONTAINER_EP2_H
#define GAMESERVER_EQUIP_SLOTCONTAINER_EP2_H
#pragma once
// EquipmentSlotContainer.h: interface for the CEquipmentSlotContainer class.
//
//==================================================================================================
/// CEquipmentSlotContainer class
/**
@author
    Kim Min Wook < taiyo@webzen.com >   
@since
    2004. 8. 11~
@history
    - 2006/2/15 : 아이템제한처리때문에 스텟up아이템을 내리기위해 CanUnEquip() 함수추가
                => DeleteSlot()의 리턴값(BOOL) 추가, 실패할 때 예외(nak) 처리
    - 2006/5/12 : 제한수치가 맞지 않는 아이템이면 탈착 가능하게!
    - ~~~
    - 2010/3/29 : waverix, refactoring to support the changed item attribute calculator
*/
//==================================================================================================
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================

#include "SCItemSlotContainer.h"
#include "ItemAttrCalculator.h"

class Player;
class SCItemSlot;

namespace nsSlot {
;

struct EquipExistences
{
    uint8_t equipped[MAX_EQUIPMENT_SLOT_NUM];
};

}; //end of namespace

//==================================================================================================

class CEquipmentSlotContainer : public SCItemSlotContainer,
                                public IEquipmentRestriction
{
public:
    // NOTES: f110422.2L, added constants reference information about default renderable equips.
    static const bool kDefaultRenderableEquipItems[MAX_EQUIPMENT_SLOT_NUM];
    //
    CEquipmentSlotContainer();
    virtual ~CEquipmentSlotContainer();
    // SCSlotContainer inherited interfaces
    virtual void ClearAll();
    // 
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* result_deleted_slot);

    virtual void OnInsert(SCSlot& IN slot);
    virtual void OnDelete(SCSlot& IN slot);
    virtual void OnUpdate(SCSlot& IN slot, int changed_num); // CHANGES: f110603.5L, added to support wings control

    virtual BOOL ValidState();
    //--------------------------------------------------
    void SetPlayer(Player* player) { player_ = player; }
    // IEquipmentRestriction interface
    virtual SCItemSlotContainer* GetEquipmentSlotContainer();
    virtual ns_formula::EquipSlotsManageInfo* GetEquipmentManageInfo();
    // check contents level enabled equip condition (no check stats+level)
    virtual bool CheckEquipDefaultValidationWithoutStats(
        POSTYPE at_pos, const SCItemSlot& IN check_slot);
    //
    bool CanEquip(POSTYPE at_pos, const SCItemSlot& IN check_slot);
    bool ValidLimitStats(const SCItemSlot& item_slot) const;
    //
    void CheckupActiveEquipItemAll();
    bool OnChangedEquipItemDurability(const SCItemSlot& item_slot);
#if SUN_CODE_BACKUP
    void SendSetItemBrd(BOOL bFullOption);
#endif
    // CONTENTS: _NA_0_20100722_AUTO_SYSTEM
    POSTYPE GetPosEquipByItemType(eITEMTYPE find_type, POSTYPE start_pos, POSTYPE end_pos) const;
    bool IsEquipEventItem(BYTE event_code) const;
    // NOTE: f110411.2L, check whether the item is activated. you must check position.
    bool IsActivated(POSTYPE item_pos) const;
    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    bool IsEquipped(POSTYPE item_pos) const;
    // CHANGES: f110422.2L, refresh CEquipmentSlotContainer::equip_existences_
    void RefreshEquipItemsCache();
    //
    // CHANGES: f110420.5L, constant information is added whether this item is renderable equipment.
    static bool IsDefaultRenderableEquipItem(POSTYPE pos);
private:
    //----------------------------------------------------------------------------------------------
    // fields...
    Player* player_;
    ns_formula::EquipSlotsManageInfo equip_slots_manage_info_;
    nsSlot::EquipExistences equip_existences_;
    //
    WAVERIX_TEST_BLOCK_IN_CLASS(CEquipmentSlotContainer);
};

//==================================================================================================
// inline implements

// CHANGES: f110420.5L, constant information is added whether this item is renderable equipment.
inline bool CEquipmentSlotContainer::IsDefaultRenderableEquipItem(POSTYPE pos)
{
    if (bool in_of_ranges = (pos < _countof(kDefaultRenderableEquipItems))) {
        return kDefaultRenderableEquipItems[pos];
    };
    return false;
}

// NOTE: f110411.2L, check whether the item is activated. you must check position.
inline bool CEquipmentSlotContainer::IsActivated(POSTYPE item_pos) const
{
    if (bool out_ranges = (item_pos >= this->GetMaxSlotNum())) {
        return false;
    };
    return equip_slots_manage_info_.slots[item_pos].activated;
}

//_NA_007079_BUG_WEAPON_INACTIVATE_STATE
inline bool CEquipmentSlotContainer::IsEquipped( POSTYPE item_pos ) const
{
    if (bool out_ranges = (item_pos >= this->GetMaxSlotNum())) {
        return false;
    };
    return equip_existences_.equipped[item_pos];
}
//==================================================================================================
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
//==================================================================================================
#endif //GAMESERVER_EQUIP_SLOTCONTAINER_EP2_H
