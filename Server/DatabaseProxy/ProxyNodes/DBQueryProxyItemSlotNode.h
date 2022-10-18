#pragma once
#ifndef DBPROXY_DBQUERYPROXY_ITEMSLOTNODE_H
#define DBPROXY_DBQUERYPROXY_ITEMSLOTNODE_H

class Query_ItemSlotInfo_Select;
class Query_ItemSlotInfo_Update;

namespace nsQuery {
;

struct ItemSlotUpdateNode
{
    //
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
    };
    //
    uint8_t record_changed_;
    QueryForCharTransaction* query_;
    ItemSlotResultRecord commited_data_;
    ItemSlotResultRecord latest_data_;
    //
    void UpdateAndCompareData();
};

struct ItemSlotVersionChecker
{
    static const uint32_t kCharSlotTypeMask = (1 << SI_INVENTORY) | (1 << SI_EQUIPMENT);
    static const uint32_t kUserSlotTypeMask = (1 << SI_WAREHOUSE);
    static const uint32_t kUniqueSlotTypeMask = (1 << SI_GUILDWAREHOUSE);
    //
    static bool IsAcceptableItemSlot(SLOTIDX slot_type, ItemSlotVersionChecker* result);
    // NOTE: special operation. make to marked serial key.
    static void SetWorldType(ITEMPART* item_part);
    // internal utility
    static bool ConvertItem(const ItemSlotResultRecord& from_record,
                            ItemSlotResultRecord* const original_record,
                            ItemSlotResultRecord* const converted_record);
    // internal utility
    static bool ConvertItem(const ITEMSTREAMEX& from_stream,
                            ItemSlotResultRecord* converted_record);
    //
    bool is_char_slot_type_;
    bool is_user_slot_type_;
    bool is_unique_slot_type_;
};

struct ItemSlotUpdateTableBase
{
    enum oOper {
        eOper_ClearAllToDefault = 0x01,
        eOper_CopyAllLatestDataToCommitedData = 0x02,
    };
    void _OperateAll(int operation,
                     ItemSlotUpdateNode* record_array,
                     size_t number_of_records,
                     SLOTIDX slot_index);
};

template<SLOTIDX _SlotIndex, int _MaxNumberOfSlots>
struct ItemSlotUpdateTable : public ItemSlotUpdateTableBase
{
    static const SLOTIDX kSlotIndex = _SlotIndex;
    static const int kMaxNumberOfSlots = _MaxNumberOfSlots;
    //
    ItemSlotUpdateNode records_[kMaxNumberOfSlots];
};

typedef ItemSlotUpdateTable<SI_EQUIPMENT, MAX_EQUIPMENT_SLOT_NUM> \
    EquipSlotUpdateTable;
typedef ItemSlotUpdateTable<SI_INVENTORY, MAX_INVENTORY_SLOT_NUM> \
    InvenSlotUpdateTable;
typedef ItemSlotUpdateTable<SI_WAREHOUSE, MAX_WAREHOUSE_SLOT_NUM> \
    WarehouseSlotUpdateTable;
typedef ItemSlotUpdateTable<SI_GUILDWAREHOUSE, MAX_GUILD_WAREHOUSE_SLOT_NUM> \
    GuildWarehouseSlotUpdateTable;

//==================================================================================================
//
class DBQueryProxyItemSlotNode
{
public:
    //
    static bool StoreItemSlotInfo(SLOTIDX slot_type,
                                  const ITEMSLOTEX_INSERVER* data_list,
                                  const POSTYPE number_of_slots,
                                  ItemSlotUpdateNode* const update_node_target_begin,
                                  const POSTYPE max_number_of_slots);
    static bool LoadItemSlotInfo(SLOTIDX slot_type,
                                 ITEMSLOTEX_INSERVER* data_list,
                                 POSTYPE* result_number_of_slots,
                                 const ItemSlotUpdateNode* const update_node_target_begin,
                                 const POSTYPE max_number_of_slots);
    static bool DBResult_Select(const Query_ItemSlotInfo_Select& query_result,
                                ItemSlotUpdateNode* const update_node_target_begin,
                                const POSTYPE max_number_of_slots);
    static bool DBResult_Update(const Query_ItemSlotInfo_Update& query_result,
                                ItemSlotUpdateNode* const update_node_target_begin,
                                const POSTYPE max_number_of_slots);
};

//==================================================================================================


}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_SLOTNODE_H