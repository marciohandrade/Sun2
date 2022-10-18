#ifndef DBPROXY_DBQUERY_PROXY_NODE_INTER_INL
    #error "can't direct include"
#endif

//==================================================================================================
// @history
//  |10.04.15|waverix|created, 
//                   |to support reliable user data db synchronization with life-time management

//==================================================================================================

namespace nsQuery {
;
class DBQueryProxyManager;

}; //end of namepsace

//==================================================================================================
// (NOTE) the DBQueryProxyCharNode is a container
// to support a database query proxy for all character's sub data.
// a below section should be define a related include files
#include "./DBQueryProxyPerkNode.h"
#include "./DBQueryProxySlotNode.h"
#include "./DBQueryProxyQuestNode.h"
#include "./DBQueryProxyBattleRecordsNode.h"
#include "./DBQueryProxyCharStatesNode.h"
#include "./DBQueryProxyAchievementNode.h" // C_NA_0_20100520_ACSYSTEM_CHANGED
#include "./DBQueryProxyMissionNode.h"

// end of the section for related include files
//==================================================================================================

namespace nsQuery {
;

class DBQueryProxyNode::CharacterNode : public DBQueryProxyNode
{
public:
    enum eState {
        eState_None = 0,
        // KEYWORD:r100928.2L
        eState_Loaded_EquipInfo = 1 << 2,
        eState_Loaded_InvenInfo = 1 << 3,
        eState_Loaded_SkillInfo = 1 << 4,
        eState_Loaded_QuickInfo = 1 << 5,
        eState_Loaded_PerkInfo  = 1 << 6,
        eState_Loaded_QuestInfo = 1 << 7,
        eState_Loaded_BattleRecords = 1 << 8,
        eState_Loaded_CharStateInfo = 1 << 9,
        eState_Loaded_Achievement = 1 << 10, // C_NA_0_20100520_ACSYSTEM_CHANGED
        eState_Loaded_MissionInfo = 1 << 11,
        eState_Loaded_CharacterExtraInfo = 1 << 12, // _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
        eState_Loaded_UserExtraInfo = 1 << 13,
#endif // _NA_007932_20150106_USER_EXTRA_INFO
        eState_All_SubData_Loaded = 0
                                  | eState_Loaded_EquipInfo
                                  | eState_Loaded_InvenInfo
                                  | eState_Loaded_PerkInfo
                                  | eState_Loaded_SkillInfo
                                  | eState_Loaded_QuickInfo
                                  | eState_Loaded_QuestInfo
                                  | eState_Loaded_BattleRecords
                                  | eState_Loaded_CharStateInfo
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
                                  | eState_Loaded_Achievement
#endif
                                  | eState_Loaded_MissionInfo
                                  | eState_Loaded_CharacterExtraInfo
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
                                  | eState_Loaded_UserExtraInfo
#endif // _NA_007932_20150106_USER_EXTRA_INFO
                                   
                                  ,
    };
    enum eTransaction {
        eTrans_None = eState_None,
        // r100928.2L
        eTrans_Select_EquipInfo = eState_Loaded_EquipInfo,
        eTrans_Select_InvenInfo = eState_Loaded_InvenInfo,
        eTrans_Select_SkillInfo = eState_Loaded_SkillInfo,
        eTrans_Select_QuickInfo = eState_Loaded_QuickInfo,
        eTrans_Select_PerkInfo  = eState_Loaded_PerkInfo,
        eTrans_Select_QuestInfo = eState_Loaded_QuestInfo,
        eTrans_Select_BattleRecords = eState_Loaded_BattleRecords,
        eTrans_Select_CharStateInfo = eState_Loaded_CharStateInfo,
        eTrans_Select_Achievement = eState_Loaded_Achievement, // C_NA_0_20100520_ACSYSTEM_CHANGED
        eTrans_Select_MissionInfo = eState_Loaded_MissionInfo,
        eTrans_Select_CharacterExtraInfo = eState_Loaded_CharacterExtraInfo, // _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
        eTrans_Select_UserExtraInfo = eState_Loaded_UserExtraInfo,
#endif // _NA_007932_20150106_USER_EXTRA_INFO
        //
        eTrans_Update_CharInfo  = 1 << 25,
        eTrans_Update_InvenInfo = 1 << 26,
        eTrans_Update_CharState = 1 << 27,
        //
        //eTrans_Update_Warehouse = DBQueryProxyUserNode::eTrans_Select_Warehouse << 16, //0x01000000
    };

    enum eRequest {
        eReq_None               = eState_None,
        eReq_Release            = 0x00000001,
        eReq_Update_CharInfo    = eTrans_Update_CharInfo,  //0x00020000
        eReq_Update_InvenInfo   = eTrans_Update_InvenInfo, //0x00040000
        eReq_Update_CharState   = eTrans_Update_CharState, //0x00080000
        //eReq_Update_Warehouse   = eTrans_Update_Warehouse, //0x01000000
        eReq_EnterGamePending   = 0x20000000,
    };
    //
    enum eUpdateType {
        // r100928.2L
        eUpdate_None     = 0x00000000,
        eUpdate_ItemSlotInfo = 0x00000002,
        eUpdate_SlotInfo = 0x00000004,
        // 0x0002 <- can't use
        eUpdate_Perk     = 0x00000008,
        eUpdate_Quest    = 0x00000010,
        eUpdate_BattleRecords = 0x00000020,
        eUpdate_CharStateInfo = 0x00000040,
        eUpdate_Achievement = 0x00000080, // C_NA_0_20100520_ACSYSTEM_CHANGED
        eUpdate_MissionInfo = 0x00000100,
        eUpdate_CharacterExtraInfo = 0x00000200, // _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
        eUpdate_UserExtraInfo = 0x00000400,
#endif // _NA_007932_20150106_USER_EXTRA_INFO
        eUpdate_All      = eUpdate_ItemSlotInfo
                         | eUpdate_SlotInfo
                         | eUpdate_Perk
                         | eUpdate_Quest
                         | eUpdate_BattleRecords
                         | eUpdate_CharStateInfo
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
                         | eUpdate_Achievement
#endif
                         | eUpdate_MissionInfo
                         | eUpdate_CharacterExtraInfo
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
                         | eUpdate_UserExtraInfo
#endif // _NA_007932_20150106_USER_EXTRA_INFO
                         ,
    };
    //
    CharacterNode();

    void Init(DBUser* db_user, DBCharacter* db_character);
    bool loaded() const; // >= 1101
    // if the result is all clear state, the result is true
    bool Update(eUpdateType update_type);
    const PerkUpdateNode* FindPerkNode(const PerkID& perk_id) const;
    void RequestEnterGame(const MSG_DG_CHARINFO_QUERY_SYN* recv_msg,
                          ChildServerSession* server_session);
    bool ProcessRequestEvent(const ProxyTransaction& transaction, bool success);
    bool OnCreateCharacter();
    // G->D update stream
    void StoreItemSlotInfo(SLOTIDX slot_type,
                           const ITEMSLOTEX_INSERVER* data_list, const POSTYPE number_of_slots);
    // D->G send stream
    void LoadItemSlotInfo(SLOTIDX slot_type,
                          ITEMSLOTEX_INSERVER* data_list, POSTYPE* result_number_of_slots) const;
    //
    // G->D update stream
    void StorePerk(const PerkDataList& data_list);
    // D->G send stream
    void LoadPerk(PerkDataList* data_list) const;
    // 
    // G->D update stream
    void StoreSkill(const SKILL_TOTAL_INFO& data_list);
    // D->G send stream
    void LoadSkill(SKILL_TOTAL_INFO* data_list) const;
    // 
    // G->D update stream
    void StoreQuick(const QUICK_TOTAL_INFO& data_list);
    // D->G send stream
    void LoadQuick(QUICK_TOTAL_INFO* data_list) const;
    // 
    // G->D update stream
    void StoreQuest(const MSG_DG_CHARINFO_QUEST_CMD& quest_block);
    // D->G send stream
    void LoadQuest(MSG_DG_CHARINFO_QUEST_CMD* quest_block) const;
    // 
    // G->D update stream
    void StoreMission(const MSG_DG_CHARINFO_QUEST_CMD& mission_block);
    // D->G send stream
    void LoadMission(MSG_DG_CHARINFO_QUEST_CMD* mission_block) const;
    // 
    // G->D update stream
    void StoreBattleRecords();
    // D->G send stream
    void LoadBattleRecords() const;
    //
    // G->D update stream
    void StoreCharStates(const STATE_DETAIL_TOTAL_INFO& data_list);
    // D->G send stream
    void LoadCharStates(STATE_DETAIL_TOTAL_INFO* data_list) const;
    //----------------------------------------------------------------------------------------------
    // extra command
    void LoadPetItemStream(ITEMSTREAMEX* result_pet_stream) const;
    //----------------------------------------------------------------------------------------------
    bool DBResult(const Query_ItemSlotInfo_Select& query_result);
    bool DBResult(const Query_ItemSlotInfo_Update& query_result);
    //
    // (NOTE) a below method is front-end interface to support a simple control
    // for a various commands. this method return true if an operation transaction is success.
    // a result of an operation should be a self contained result data block.
    // CAUTION! ... in this method, can't store a pointer in a stack or network buffer address.
    // instead of this method has not an asynchronous data control.
    bool ProcessPerkOperation(const PerkControl& req_info,
                              PerkControl* header, PerkControlData* data);
    //
    bool DBResult(const Query_Perk_Select& query_result);
    bool DBResult(const Query_Perk_Insert& query_result);
    bool DBResult(const Query_Perk_Delete& query_result);
    bool DBResult(const Query_Perk_Update& query_result);
    //
    bool DBResult(const Query_SlotInfo_Select& query_result);
    bool DBResult(const Query_SlotInfo_Insert& query_result);
    bool DBResult(const Query_SlotInfo_Update& query_result);
    //
    bool DBResult(const Query_Quest_Select& query_result);
    bool DBResult(const Query_Quest_Update& query_result);
    //
    bool DBResult(const Query_BattleRecords_Select& query_result);
    bool DBResult(const Query_BattleRecords_Update& query_result);
    //
    bool DBResult(const Query_Char_State_Select& query_result);
    bool DBResult(const Query_Char_State_Insert& query_result);
    bool DBResult(const Query_Char_State_Delete& query_result);
    bool DBResult(const Query_Char_State_Update& query_result);
  #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	bool DBResult(const Query_Char_Achievement_Select& query_result);
	bool DBResult(const Query_Char_Achievement_Update& query_result);
  #endif
    //
    bool DBResult(const Query_MissionInfo_Select& query_result);
    bool DBResult(const Query_MissionInfo_Insert& query_result);
    bool DBResult(const Query_MissionInfo_Update& query_result);
    // _NA_006372_20130108_BADGE_ITEM
    bool DBResult(const Query_CharacterExtraInfo_Select& query_result);
    bool DBResult(const Query_CharacterExtraInfo_Update& query_result);
    //
    bool QueryNext(ProxyTransaction::eTransaction last_transaction);
#ifdef _NA_007086_20140318_MONSTERBOOK
    bool SelectCharacterMonsterBook();
    bool UpdateCharacterMonsterBook(DWORD monster_code);
    bool DBResult(const Query_CharacterMonsterBook_Select& query_result);
    bool DBResult(const Query_CharacterMonsterBook_Update& query_result);
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    bool SelectUserExtraInfo();
    bool UpdateUserExtraInfo();
    bool DBResult(const Query_UserExtraInfo_Select& query_result);
    bool DBResult(const Query_UserExtraInfo_Update& query_result);
#endif //_NA_007932_20150106_USER_EXTRA_INFO
private:
    //                  Perk Group
    typedef STLX_HASH_MAP<ulong, PerkUpdateNode> PerkUpdateTable;
    //                   state code
    typedef STLX_HASH_MAP<uint16_t, CharStateUpdateNode> CharStateTable;
    //
    PerkUpdateNode* FindPerkNode(const PerkID& perk_id);
    //void RemovePerk(const PerkID& perk_id);
    bool UpdateItemSlotInfo();
    bool UpdatePerk();
    bool UpdateSlotInfo();
    bool UpdateQuest();
    bool UpdateBattleRecords();
    bool UpdateCharStates();
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	bool UpdateAchievement();
#endif
    bool UpdateMissionInfo(); // NOTE: f101129.5L
    //!~ _NA_006372_20130108_BADGE_ITEM
    bool UpdateCharacterExtraInfo(); 
    bool SelectCharacterExtraInfo();
    //~! _NA_006372_20130108_BADGE_ITEM
    //
    bool BeginTransaction(eTransaction transaction);
    bool EndTransaction(eTransaction transaction, bool success);
    //
    bool SetupDefaultPerkInfo();
    bool SetupDefaultSkillInfo(eCHAR_TYPE char_type);
    bool SetupDefaultQuickInfo();
    static const PerkUpdateTable& _GetDefaultPerkInfoTable();
    static const SkillSlotUpdateTable& _GetDefaultSkillTable(eCHAR_TYPE char_type);
    static const QuickSlotUpdateTable& _GetDefaultQuickTable();
    //
    DBUser* db_user_;
    DBCharacter* db_character_;
    // non-pod fields
    PerkUpdateTable perk_table_;
    CharStateTable char_state_table_;
    // NOTE: f101004.7L, the below block is processed by operation of fill zero memory.
    // WARNING: should be don't declare non-pod fields in this ranges.
    char pod_field_start_[1];
    //
    EquipSlotUpdateTable equip_table_;
    InvenSlotUpdateTable inven_table_;
    ITEMSTREAMEX pet_item_stream_;
    //
    SkillSlotUpdateTable skill_table_;
    QuickSlotUpdateTable quick_table_;
    QuestUpdateTable quest_table_;
    BattleRecordsUpdateTable battle_records_table_;
    MissionUpdateTable mission_table_;
    struct {
        ChildServerSession* session;
        MSG_DG_CHARINFO_QUERY_SYN message;
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

inline bool DBQueryProxyNode::CharacterNode::loaded() const
{
    const_cast<bool&>(shared_.loaded) = \
        ((shared_.state & this->eState_All_SubData_Loaded) == this->eState_All_SubData_Loaded);
    return shared_.loaded;
}

inline PerkUpdateNode*
DBQueryProxyNode::CharacterNode::FindPerkNode(const PerkID& perk_id)
{
    PerkUpdateTable::iterator found = perk_table_.find(perk_id.group_);
    if (found == perk_table_.end()) {
        return NULL;
    }
    PerkUpdateNode* node = &found->second;
    return node;
}

inline const PerkUpdateNode*
DBQueryProxyNode::CharacterNode::FindPerkNode(const PerkID& perk_id) const
{
    PerkUpdateTable::const_iterator found = perk_table_.find(perk_id.group_);
    if (found == perk_table_.end()) {
        return NULL;
    }
    const PerkUpdateNode* node = &found->second;
    return node;
}

}; //end of namespace 'nsQuery'

//==================================================================================================

