#ifndef GAMESERVER_ITEMMANAGER_DURABILITY_H
#define GAMESERVER_ITEMMANAGER_DURABILITY_H

class ItemManager;

//==================================================================================================
namespace nsSlot {
;
//==================================================================================================
// Sub-Manager of the ItemManager about item durability
class DurabilityImpl
{
public:
    DurabilityImpl();
    // called by ItemManager::Init
    DurabilityImpl(ItemManager* item_manager);
    //
    void DecreaseDurabilityUsingReason(eDecDuraType decrease_dura_type,
                                       eEQUIPCONTAINER_POS equip_pos);
    // NOTE: this method use internal controller instead of 'ValidPos'.
    bool DecreaseDurabilityWithRanges(const DWORD damage,
        eEQUIPCONTAINER_POS start_equip_pos, eEQUIPCONTAINER_POS last_equip_pos);
    bool DecreaseDurability(const DWORD damage, eEQUIPCONTAINER_POS equip_pos);

    bool DecreaseDuraAll(float decrease_ratio);
    RC::eITEM_RESULT Repair(bool total_repairs, SLOTIDX slot_index, POSTYPE slot_pos);
    // NOTE: the usage case of eITEMTYPE_REPAIR item.
    // if the result is fails, any items have been repaired.
    bool RepairAllUsingByRepairsItem();

    // work in EquipContainer, _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    void UpdateItemAttributesOnChangesEquippedItemStatus(const SCItemSlot& equipped_item);
    // NOTE: activated by GM command. __NA_001062_20080623_STRENGTH_PENALTY
    bool ChangeDurabilityForGM(eEQUIPCONTAINER_POS equip_pos, DURATYPE durability);
private:
    // requirements: 'equip_pos' is predicated already test using 'ValidPos'
    bool DeleteEquipByDura(POSTYPE equip_pos);
    bool NotifyChangeDurability(POSTYPE equip_pos, SLOTCODE item_code, DURATYPE durability);
    //
    ItemManager* const root_;
    //
    __DISABLE_COPY(DurabilityImpl);
};

//==================================================================================================
inline DurabilityImpl::DurabilityImpl()
    : root_(NULL)
{
}

inline DurabilityImpl::DurabilityImpl(ItemManager* item_manager)
    : root_(item_manager)
{
}

inline bool DurabilityImpl::DecreaseDurability(const DWORD damage, eEQUIPCONTAINER_POS equip_pos)
{
    return DecreaseDurabilityWithRanges(damage, equip_pos, equip_pos);
};

//==================================================================================================
}; //end of namespace
//==================================================================================================

#endif //GAMESERVER_ITEMMANAGER_DURABILITY_H