#ifndef GAMESERVER_ITEMMANAGER_TYPES_H
#define GAMESERVER_ITEMMANAGER_TYPES_H
#pragma once

//==================================================================================================
// @history
//  - |~10/03/24|
//  - |10/03/24|waverix|fit in coding style
//  - |10/03/25|waverix|move to code file related ItemStatSwitch implementation parts

//==================================================================================================

class MATERIALS_FOR_LEVEL;
class SCSlotContainer;
class SCItemSlotContainer;
class SlotManager;
class ItemInfoParser;

class ItemStatSwitch;
class ItemManager;
class Player;

//==================================================================================================

namespace ns_functor {
;

struct ISlotForeachFunctor;
struct IItemSlotForeachFunctor;
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
class DeleteComposeMaterialItem;
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION

}; //end of namespace

//==================================================================================================

enum eSERIAL_ACTION
{
    eSERIAL_LOAD=0,
    eSERIAL_MAKE,
    eSERIAL_WAREHOUSE_START,
    eSERIAL_WAREHOUSE,
    eSERIAL_INVENTORY,
    eSERIAL_TEMPINVENTORY,
    eSERIAL_EQUIPMENT,
};

struct POS_INFO
{
    POSTYPE m_pos;      //< 아이템을 넣을 위치x
    POSTYPE m_num;      //< m_pos[x]에 넣을 개수
};

struct POS_INFO_EX
{
    POSTYPE m_pos;      //< 아이템을 넣을 위치x
    POSTYPE m_num;      //< m_pos[x]에 넣을 개수
    ITEMSTREAMEX ItemStream;
};

//==================================================================================================
// (CHANGES) (f100525.4L) change a data ordering, 16B->12B
struct LOCKED_INFO
{
    SLOTCODE    m_code;
    SLOTIDX     m_SlotIndex;
    POSTYPE     m_pos;      //< 아이템을 넣을 위치x
    POSTYPE     m_num;      //< m_pos[x]에 넣을 개수
    BYTE        m_Enchant;
    BYTE        m_Rank;
    // (f100525.1L) change the specification related to the item customizing,
    // the 'm_ItemType' is the same size as the 'KindOfItemType::value_type'
    KindOfItemType::value_type m_ItemType;
    DWORD       m_DateTime;
};

struct RepurchaseSlot
{
    BOOL m_bIsUse;
    SCItemSlot* m_itemSlot;

    RepurchaseSlot() : 
    m_bIsUse(FALSE), m_itemSlot(NULL) 
    {}
};

//==================================================================================================

// implemented by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_

namespace nsSlot {

enum eItemQueryType
{
    eItemQuery_None,
    eItemQuery_Select,
    eItemQuery_InverseSelect,
    eItemQuery_Max,
};

struct ItemQueryResult {
    union {
        SCSlot* slot;
        int result_of_select; // number of select query, pos_info_array_ has the each position info.
        int result_of_inverse_select; // number of inversed select query, same as 'result_of_select'
    };
};

//--------------------------------------------------------------------------------------------------

class ItemQuery
{
public:
    static const int kMaxNumberOfPosSlots = INVALID_POSTYPE_VALUE;
    struct QueryInfo {
        int number_of_queries;
        int sum_of_queried_items;
        struct Node {
            enum { eSettingGuid_ConstantSlot, eSettingGuid_NonConstantSlot, };
            POSTYPE position;
            nsSlot::InventoryConfig::SetType item_insert_rule; // nsSlot::InventoryConfig::eInsertRule
            SLOTCODE item_code;
            int number_of_items;
            const SCItemSlot* item_slot; // NULL = item code based,
                                         // not NULL = item slot based, aligned item_code
            const BASE_ITEMINFO* item_info; // temp cache field, need null check
            int sum_of_items; // sum of distributed number of items in the result_info_
            int setting_guide;
        } nodes[kMaxNumberOfPosSlots];
    };
    struct ResultInfo {
        // warnings, not serialized result, \
        // if you need total search that must do total iterating search
        int number_of_results;
        int sum_of_selected_items;
        struct Node {
            enum {
                eUpdateGuide_ConstantSlot = 1 << 0,
                eUpdateGuide_NonConstantSlot = 1 << 1,
                eUpdateGuide_Update = 1 << 2,
                eUpdateGuide_Insert = 1 << 3,
            };
            POSTYPE position;
            SLOTCODE container_item_code;
            int number_of_selected_items;
            //// it's calculated cache data which is store each calculated remains
            //int number_of_selected_item_in_container;
            const SCItemSlot* query_item_slot; // if the result succeeds,
                                               // this is aligned from that of a query info \
                                               // to support obtains operation
            SCItemSlot* container_item_slot; // low level editable point, warning dangerous!
            //const BASE_ITEMINFO* query_item_info; // same reason as above
            int update_guide;
        } nodes[kMaxNumberOfPosSlots];
    };

    ItemQuery();
    // slot_index : target container, it is come under the SCItemSlotContainer category
    //              inter-process slot container of player
    static bool MakeQuerySelect(ItemQuery* item_query, Player* player, SLOTIDX slot_index);
    static bool MakeQueryInverseSelect(ItemQuery* item_query, Player* player, SLOTIDX slot_index);

    // target:
    //  - eItemQuery_Select,
    //  - eItemQuery_InverseSelect,
    // select one query data = { ItemCode, ItemSlot }
    // if you want multi items query, use AddArg_ItemCode recursively
    // <returns>
    //  if the function succeeds, the return value is grater equal than 0. (>=0)
    //  if the function fails, the return value is -1. (< 0)
    int AddArg_ItemCode(SLOTCODE item_code, int number_of_items);
    // same comment as above,
    // + The purpose of a inter_slot means nonverifiable slot to prevent duplicated slot copy op.
    int AddArg_ItemSlot(const SCItemSlot& item_slot, int number_of_items,
                        bool non_constant_slot);
    //
    int AddArg_Custom(POSTYPE number_of_kinds,
                      const SLOTCODE* item_code_array, const POSTYPE* item_num_array);
    // (CHANGES) (f100525.4L)
    int AddArg_CustomWithLockInfo(POSTYPE number_of_kinds,
                                  const SLOTCODE* item_code_array, const POSTYPE* item_num_array);

    const QueryInfo& query_info() const;
    const ResultInfo& result_info() const;
    SCItemSlotContainer* item_container() const;
    const KindOfItemTag& GetItemTag() const;
    // (CHANGES) (f100525.4L) the query of a 'InverseSelect' is
    // based on a obtain function directly or indirectly(reserve)
    // this function is to support a long term item transaction like a shop transaction
    void ChangeToFindEmptySlotOnlyMode();
    // CHANGES: f100909.1L, all making item is maked by tag option. this option has 1bit,
    // so that a p-coin purchase tag of a KindOfItemTag is exactly the same way \
    // as an always tradable option like a CanTradeSell(eITEM_TRADESELL_TRADE) is true.
    void ChangeToAcquireItemTagOption(const KindOfItemTag& tag);
    // for Debug routine
    bool CheckResultValidatation_Custom(POSTYPE number_of_kinds,
                                        const SLOTCODE* item_code_array,
                                        const POSTYPE* item_num_array) const;
    bool MakeSerializedStream(INVENTORY_TOTAL_INFO* inven_total_info) const;
    bool MakeSerializedStream(JUST_INVENTORY_TOTAL_INFO* inven_total_info) const;
    bool MakeSerializedStream(INVEN_DELETE_TOTAL_INFO* inven_delete_total_info) const;
    bool MakeSerializedStream(int number_of_kinds,
                              POS_INFO* item_code_array,
                              POSTYPE* upperbound) const;
    // (f100525.4L) set up values of a LOCKED_INFO and lock slots automatically.
    // (WARNING) the result value must be checked with number_of_locked_info
    // after a calling of a 'Query'
    // assert(result_info.number_of_results <= number_of_locked_info)
    bool SetupLockedInfoWithLock(POSTYPE* const number_of_used_locked_info,
                                 const int number_of_locked_info, LOCKED_INFO* const locked_info,
                                 POSTYPE number_of_kinds, const DWORD* datetime_array);
    // (f100607.5L) add the logging function after a result process completed.
    bool WriteToGameLogBasedOnResult(Player* player, int log_type);
private:
    void Clear();
    // If the result is false, can't find item info.
    // If the result is true, the item_info of a QueryInfo::Node is aligned reference.
    bool InterAlignItemInfo(QueryInfo::Node* query_node, \
                            ItemInfoParser* item_parser, int query_node_index, bool sorting_insert);
    // if current operation is queried by the inverse_select query, call the below method.
    bool PrefetchQueryInfoForInverseSelect();
    static bool SetupDefaults(eItemQueryType query_type,
                              ItemQuery* item_query, Player* player, SLOTIDX slot_index);

    eItemQueryType query_type_;
    Player* player_;
    ItemManager* item_manager_;
    SCItemSlotContainer* item_container_;
    SLOTIDX slot_index_;
    bool find_empty_only_; // (CHANGES) (f100525.4L)
    KindOfItemTag option_tag_; // f100909.1L
    //
    QueryInfo query_info_;
    ResultInfo result_info_;
    //
    friend class ItemManager;
    __DISABLE_COPY(ItemQuery);
};

//==================================================================================================

inline ItemQuery::ItemQuery()
    : query_type_(eItemQuery_None)
{
}

inline void ItemQuery::Clear()
{
    ZeroMemory(this, sizeof(*this));
}

inline const ItemQuery::QueryInfo& ItemQuery::query_info() const
{
    return query_info_;
}

inline const ItemQuery::ResultInfo& ItemQuery::result_info() const
{
    return result_info_;
}

inline SCItemSlotContainer* ItemQuery::item_container() const
{
    return item_container_;
}

// (CHANGES) (f100525.4L) the query of a 'InverseSelect' is
// based on a obtain function directly or indirectly(reserve)
// this function is to support a long term item transaction like a shop transaction
inline void ItemQuery::ChangeToFindEmptySlotOnlyMode()
{
    find_empty_only_ = true;
}

inline const KindOfItemTag& ItemQuery::GetItemTag() const
{
    return option_tag_;
}

// CHANGES: f100909.1L, all making item is maked by tag option. this option has 1bit,
// so that a p-coin purchase tag of a KindOfItemTag is exactly the same way \
// as an always tradable option like a CanTradeSell(eITEM_TRADESELL_TRADE) is true.
inline void ItemQuery::ChangeToAcquireItemTagOption(const KindOfItemTag& tag)
{
    option_tag_ = tag;
}

inline bool ItemQuery::MakeQuerySelect(ItemQuery* item_query,
                                         Player* player,
                                         SLOTIDX slot_index)
{
    if (SetupDefaults(eItemQuery_Select, item_query, player, slot_index) == false) {
        return false;
    }
    return true;
}

inline bool ItemQuery::MakeQueryInverseSelect(ItemQuery* item_query,
                                              Player* player,
                                              SLOTIDX slot_index)
{
    if (SetupDefaults(eItemQuery_InverseSelect, item_query, player, slot_index) == false) {
        return false;
    }
    return true;
}

}; //end of namespace

//==================================================================================================

#endif //GAMESERVER_ITEMMANAGER_TYPES_H