#ifndef GAMESERVER_ITEMMANAGER_UPDATER_H
#define GAMESERVER_ITEMMANAGER_UPDATER_H

class ItemManager;

//==================================================================================================
namespace nsSlot {
;
//==================================================================================================
// Sub-Manager of the ItemManager about item durability
class ItemManagerUpdateImpl
{
private:
    static const DWORD SELECTED_DEFAULTITEM = 0;
    static const DWORD SELECTED_TIMEEXPIRATIONITEM = 10000;
public:
    ItemManagerUpdateImpl();
    // called by ItemManager::Init
    ItemManagerUpdateImpl(ItemManager* item_manager);
    //
    // 중복 시리얼 체크 위한 코드
    bool IsDupSerialCode(DWORD64 world_serial_key) const;
    void AddSerialCode(eSERIAL_ACTION serial_action,
                       SLOTIDX slot_index, POSTYPE pos_in_slots,
                       DWORD64 world_serial_key);
    void RemoveSerialCode(SLOTIDX slot_index, POSTYPE pos_in_slots, DWORD64 world_serial_key);

    // item_slot: slot in container
    void AddExpirationItem(SCItemSlot& item_slot);
    // item_slot: slot in container
    void RemoveExpirationItem(SCItemSlot& item_slot);

#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    void AddImmediatelyUseItem(SCItemSlot& item_slot);
    void RemoveImmediatelyUseItem(SCItemSlot& item_slot);
    bool IsImmediatelyUseItem(SCItemSlot& item_slot);
    bool UpdateImmediatelyUseItem();
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE

private:
    // returns: SELECTED_DEFAULTITEM|SELECTED_TIMEEXPIRATIONITEM|eCHARGE_SUB_TYPE
    DWORD GetExpirationItemType(const SCItemSlot& item_slot) const;
    //
    bool _UpdateExpirationItem(); // ChargeItem, etc Time Expiration Items
    bool _UpdateEtcItem();
    //
    bool DeleteItemOnUpdate(SLOTIDX slot_index, POSTYPE pos_in_slots,
                            SCItemSlot& item_slot,
                            uint8_t reason_based_on_item_result,
                            uint16_t reason_based_on_game_log,
                            SCItemSlot* const deleted_item_slot);
    bool ChangeItemOnUpdate(SLOTIDX slot_index, POSTYPE pos_in_slots,
                            SCItemSlot& item_slot,
                            bool enabled_game_logging,
                            uint16_t reason_based_on_game_log);
    //
    bool SetupPostItemOptionOnSerialIssued(SCItemSlot& issue_item_slot);
    bool SetupPostItemEventOnDuplicatedSerialIssued(
        eSERIAL_ACTION serial_action,
        const SLOTIDX slot_index, const POSTYPE pos_in_slots,
        const DWORD64 world_serial_key);
    //
    ItemManager* const root_;
    //
    friend class ItemManager;
    //
    __DISABLE_COPY(ItemManagerUpdateImpl);
};

//==================================================================================================

inline nsSlot::ItemManagerUpdateImpl::ItemManagerUpdateImpl()
    : root_(NULL)
{
}

inline nsSlot::ItemManagerUpdateImpl::ItemManagerUpdateImpl(ItemManager* item_manager)
    : root_(item_manager)
{
}

}; //end of namespace

//==================================================================================================
#endif //GAMESERVER_ITEMMANAGER_UPDATER_H