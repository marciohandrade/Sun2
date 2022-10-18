#pragma once
#ifndef GAMESERVER_SLOT_MANAGER_H
#define GAMESERVER_SLOT_MANAGER_H

#include "ItemTypeList.h"
#include "ItemManager_Types.h"

//==================================================================================================
// @history
//  - ~11.04.06
//  - 11.04.06, waverix, code arrangement and refactoring (f110406.6L)
//
//==================================================================================================

class SlotManager
{
public:
    static const bool kForeachContinueCondition = false;
    //
    SlotManager();
    ~SlotManager();

    void Init(Player* player);
    void Release();

    SCSlotContainer* GetSlotContainer(SLOTIDX slot_index) const;
    bool IsEmpty(SLOTIDX slot_index, POSTYPE pos_in_slot) const;
    bool ValidState() const;

    void UnbindGuildWarehouseContainer();
    void BindGuildWarehouseContainer(CGuildWarehouseSlotContainer* guild_warehouse_container);

    // CHANGES: f110406.6L, foreach routines using functor
    bool ForeachSlot(SLOTIDX slot_index,
                     const ns_functor::ISlotForeachFunctor* functor) const;
    bool ForeachSlot(SLOTIDX slot_index,
                     const ns_functor::IItemSlotForeachFunctor* functor) const;

private:
    struct ImplData; // internal data structure

    bool ValidPos(
        const SLOTIDX slot_index, const POSTYPE pos_in_slot, const bool check_empty = true) const;
    bool ValidContainer(SLOTIDX slot_index) const;

    // fields...
    Player* player_;
    SlotManager::ImplData* const data_;
    SCSlotContainer* slot_container_list_[SI_MAX];
    //
    friend class ItemManager;
    friend class SkillSlotManager;
    friend class QuickManager;
    //
    __DISABLE_COPY(SlotManager);
};

//==================================================================================================

#endif //GAMESERVER_SLOT_MANAGER_H