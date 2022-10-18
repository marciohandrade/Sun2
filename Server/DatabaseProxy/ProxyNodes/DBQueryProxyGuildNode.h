#pragma once
#ifndef DBPROXYSERVER_DBQUERYPROXY_GUILD_NODE_H
#define DBPROXYSERVER_DBQUERYPROXY_GUILD_NODE_H

#include "./DBQueryProxyNode.h"

//==================================================================================================
// @history
//  |10.10.10|waverix|created, 
//                   |to support reliable guild data db synchronization with life-time management

namespace nsQuery {
;
class DBQueryProxyManager;
}; //end of namespace


#include "./DBQueryProxyItemSlotNode.h"

class GuildWarehouseSelectQuery;
class GuildWarehouseUpdateQuery;
class GuildManagerInDBP;

namespace nsQuery {
;

struct GuildWarehouseUpdateNode
{
    //
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
    };
    //
    uint8_t record_changed_;
    GuildWarehouseUpdateQuery* query_;
    GuildWarehouseResultRecord commited_data_;
    GuildWarehouseResultRecord latest_data_;
    //
    void UpdateAndCompareData();
};

//==================================================================================================

class DBQueryProxyGuildNode : public DBQueryProxyNode
{
public:
    static const CHARGUID kOpenLockerUserGuidForBlock = -1;
    static const CHARGUID kOpenLockerCharGuidForBlock = kOpenLockerUserGuidForBlock;
    // r100928.5L
    enum eState {
        eState_None = 0,
        eState_Loaded_Warehouse = 0x00000002,
        eState_Loaded_WareItems = 0x00000004,
        //
    };
    // r100928.5L
    enum eTransaction {
        eTrans_None = eState_None,
        //
        eTrans_Select_Warehouse  = eState_Loaded_Warehouse, //0x00000002
        eTrans_Select_WareItems  = eState_Loaded_WareItems, //0x00000004
        //
    };

    enum eRequest {
        eReq_None               = eTrans_None,
        eReq_Release            = 0x00000001,
        //
    };

    DBQueryProxyGuildNode();
    //~DBQueryProxyGuildNode(); // to support pending list management
    void Init(GUILDGUID guild_guid);
    bool Update(ChildServerSession* server_session, USERGUID user_guid, CHARGUID char_guid);
    // properties
    GUILDGUID GetGuildGuid() const;
    bool IsWarehouseLoaded() const;
    bool IsTransaction() const;
    bool NeedUpdateCheck() const;
    MONEY GetWarehouseMoney() const;
    uint8_t GetSlotTabNum() const;
    USERGUID GetOpenLocker(CHARGUID* char_guid_if_locked) const;
    // update properties
    void AcquireOpenLock(USERGUID locker_user_guid, CHARGUID locker_char_guid);
    void ReleaseOpenLock();
    void SetSlotTabNum(uint8_t number_of_slot_tabs);
    void SetWarehouseMoney(const MONEY& money);
    //----------------------------------------------------------------------------------------------
    // must be check 'IsWarehouseLoaded' first
    // r100928.4L, list = { guild_warehouse, }
    // G->D update stream
    bool StoreItemSlotInfo(SLOTIDX slot_type,
                           const ITEMSLOTEX_INSERVER* data_list, const POSTYPE number_of_slots);
    // D->G send stream
    bool LoadItemSlotInfo(SLOTIDX slot_type,
                          ITEMSLOTEX_INSERVER* data_list, POSTYPE* result_number_of_slots) const;
    //
    //----------------------------------------------------------------------------------------------
    // DB result handlers
    // - db result to compatible the custom interface
    bool OnSetGuildWarehouseBaseInfo(const GuildWarehouseSelectQuery& query_result);
    bool DBResult(const GuildWarehouseUpdateQuery& query_result);
    bool DBResult(const Query_ItemSlotInfo_Select& query_result);
    bool DBResult(const Query_ItemSlotInfo_Update& query_result);

private:
    struct UpdateArgs {
        ChildServerSession* server_session;
        USERGUID user_guid;
        CHARGUID char_guid;
    };
    //
    void Release(); // call by GuildManagerInDBP
    bool BeginTransaction(eTransaction transaction);
    bool EndTransaction(eTransaction transaction, bool success);
    bool UpdateWarehouse(const UpdateArgs& update_args);
    bool UpdateGuildLevelItemSlots(const UpdateArgs& update_args);

    ITimeout lifetime_timeout_; // the life-time after a delete user operation
    // NOTE: f101010.3L, the below block is processed by operation of fill zero memory.
    // WARNING: should be don't declare non-pod fields in this ranges.
    char pod_field_start_[1];
    //
    GUILDGUID guild_guid_;
    USERGUID locker_user_guid_; // this is the one who open a guild warehouse.
    CHARGUID locker_char_guid_; // this is the one who open a guild warehouse.
    bool update_check_; // if the field value is true, do update routines
    uint8_t number_of_slot_tabs_;
    GuildWarehouseUpdateNode warehouse_update_node_;
    GuildWarehouseSlotUpdateTable warehouse_table_;
    //
    char pod_field_end_[1];
    //
    friend class DBQueryProxyManager;
    friend class GuildManagerInDBP;
};

}; //end of namespace 'nsQuery'

//==================================================================================================
// implementation

namespace nsQuery {
;

// properties
inline GUILDGUID DBQueryProxyGuildNode::GetGuildGuid() const {
    return guild_guid_;
}

inline bool DBQueryProxyGuildNode::IsWarehouseLoaded() const {
    bool wareitems_loaded = (shared_.state & this->eState_Loaded_Warehouse) &&
                            (shared_.state & this->eState_Loaded_WareItems);
    return wareitems_loaded;
}

inline bool DBQueryProxyGuildNode::IsTransaction() const {
    bool is_transaction = (shared_.transaction & this->eTrans_Select_Warehouse) ||
                          (shared_.transaction & this->eTrans_Select_WareItems);
    return is_transaction;
}

inline bool DBQueryProxyGuildNode::NeedUpdateCheck() const {
    return update_check_;
}

inline MONEY DBQueryProxyGuildNode::GetWarehouseMoney() const {
    return warehouse_update_node_.latest_data_.money_;
}

inline uint8_t DBQueryProxyGuildNode::GetSlotTabNum() const {
    //return warehouse_update_node_.latest_data_.number_of_slot_tabs_;
    return number_of_slot_tabs_;
}

inline CHARGUID DBQueryProxyGuildNode::GetOpenLocker(CHARGUID* char_guid_if_locked) const {
    if (char_guid_if_locked) {
        *char_guid_if_locked = locker_char_guid_;
    }
    return locker_user_guid_;
}

// update properties
inline void DBQueryProxyGuildNode::SetSlotTabNum(uint8_t number_of_slot_tabs) {
    //warehouse_update_node_.latest_data_.number_of_slot_tabs_ = number_of_slot_tabs;
    number_of_slot_tabs_ = number_of_slot_tabs;
    update_check_ = true;
}

inline void DBQueryProxyGuildNode::AcquireOpenLock(USERGUID locker_user_guid,
                                                   CHARGUID locker_char_guid)
{
    locker_user_guid_ = locker_user_guid;
    locker_char_guid_ = locker_char_guid;
}

inline void DBQueryProxyGuildNode::ReleaseOpenLock() {
    locker_user_guid_ = locker_char_guid_ = 0;
}

inline void DBQueryProxyGuildNode::SetWarehouseMoney(const MONEY& money) {
    warehouse_update_node_.latest_data_.money_ = money;
    update_check_ = true;
}

}; //end of namespace 'nsQuery'


//==================================================================================================

#endif //DBPROXYSERVER_DBQUERYPROXY_GUILD_NODE_H