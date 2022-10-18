#ifndef DBPROXY_DBQUERY_PROXY_NODE_INTER_INL
    #error "can't direct include"
#endif

//==================================================================================================
// @history
//  |10.04.15|waverix|created, 
//                   |to support reliable user data db synchronization with life-time management

namespace nsQuery {
;
class DBQueryProxyManager;
}; //end of namespace

#include "./DBQueryProxyItemSlotNode.h"


namespace nsQuery {
;

//--------------------------------------------------------------------------------------------------

class DBQueryProxyNode::UserNode : public DBQueryProxyNode
{
public:
    static const DWORD kPendingLifeTimeAfterDelete = 30 * 1000; // 30 seconds
    // r100928.3L, r100928.4L
    enum eState {
        eState_None = 0,
        eState_Loaded_CharInfo  = 0x00000002,
        eState_Loaded_InvenInfo = 0x00000004,
        eState_Loaded_PetList   = 0x00000008,
        eState_Loaded_EquipInfo = 0x00000010, // special event based on equipment of players
        eState_Loaded_Warehouse = 0x00000100,
        eState_Loaded_WareItems = 0x00000200,
        //
        eState_All_Character_Loaded = eState_Loaded_CharInfo |
                                      eState_Loaded_InvenInfo |
                                      eState_Loaded_PetList |
                                      eState_Loaded_EquipInfo |
                                      0,
    };
    // r100928.3L, r100928.4L
    enum eTransaction {
        eTrans_None = eState_None,
        //
        eTrans_Select_CharInfo   = eState_Loaded_CharInfo,  //0x00000002
        eTrans_Select_InvenInfo  = eState_Loaded_InvenInfo, //0x00000004
        eTrans_Select_PetList    = eState_Loaded_PetList,   //0x00000008
        eTrans_Select_EquipInfo  = eState_Loaded_EquipInfo, //0x00000010
        eTrans_Select_Warehouse  = eState_Loaded_Warehouse, //0x00000100
        eTrans_Select_WareItems  = eState_Loaded_WareItems, //0x00000200
        //
        //
        eTrans_Update_Warehouse  = eTrans_Select_Warehouse << 16, //0x01000000
        eTrans_Create_Character  = 0x00800000,
        //
    };

    enum eRequest {
        eReq_None               = eTrans_None,
        eReq_Release            = 0x00000001,
        //
        eReq_Update_Warehouse   = eTrans_Update_Warehouse, //0x01000000
        eReq_ConnectPending     = 0x10000000,
    };

    UserNode();
    //~UserNode(); // to support pending list management
    void Init(DBUser* db_user);
    bool Update();
    bool loaded() const;
    void CancelRequest();

    // r100928.4L, list = { warehouse, }
    // G->D update stream
    bool StoreItemSlotInfo(SLOTIDX slot_type,
                           const ITEMSLOTEX_INSERVER* data_list, const POSTYPE number_of_slots);
    // D->G send stream
    bool LoadItemSlotInfo(SLOTIDX slot_type,
                          ITEMSLOTEX_INSERVER* data_list, POSTYPE* result_number_of_slots) const;

    //
    //----------------------------------------------------------------------------------------------
    bool DBResult(const Query_ItemSlotInfo_Select& query_result);
    bool DBResult(const Query_ItemSlotInfo_Update& query_result);

private:
    void Release(); // call by DBQueryProxyManager
    bool BeginTransaction(eTransaction transaction);
    bool EndTransaction(eTransaction transaction, bool success);
    bool UpdateUserLevelItemSlots();
    // call by DBQueryProxyManager
    void RequestConnect(const MSG_AD_CHARINFO_CHARLISTREQ_SYN* recv_msg,
                        ChildServerSession* server_session);
    //
    DBUser* db_user_;
    ITimeout lifetime_timeout_; // the life-time after a delete user operation
    // NOTE: f101008.2L, the below block is processed by operation of fill zero memory.
    // WARNING: should be don't declare non-pod fields in this ranges.
    char pod_field_start_[1];
    //
    WarehouseSlotUpdateTable warehouse_table_;
    //
    struct {
        DWORD registered_tick; // CHANGES: f110614.2L, to prevent connection request hangs
        ChildServerSession* session;
        MSG_AD_CHARINFO_CHARLISTREQ_SYN message;
    } request_info_;
    //
    char pod_field_end_[1];
    //
    friend class DBQueryProxyManager;
};

}; //end of namespace 'nsQuery'

//==================================================================================================
// implementation

namespace nsQuery {
;

inline bool DBQueryProxyNode::UserNode::loaded() const
{
    const_cast<bool&>(shared_.loaded) = \
        ((shared_.state & this->eState_All_Character_Loaded) == this->eState_All_Character_Loaded);
    return shared_.loaded;
}

}; //end of namespace 'nsQuery'

//==================================================================================================

