#include "stdafx.h"
#include "DBQueryProxyNode.h"
#include "DBQueryProxyManager.h"
//==================================================================================================

// implemented by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
#include "Services/NewCharacterItemProvider.h"
#include "QueryObjects/DBHandler.h"
//==================================================================================================
#include <TinyXml/tinyxml.h>

namespace nsQuery {
;

static void CompileTimeChecker(int value)
{
    __TOUCHED(&CompileTimeChecker);
    DBQueryProxyCharNode node;
    switch (value)
    {
    // r100928.2L
    case node.eState_None:
    case node.eState_Loaded_EquipInfo:
    case node.eState_Loaded_InvenInfo:
    case node.eState_Loaded_SkillInfo:
    case node.eState_Loaded_QuickInfo:
    case node.eState_Loaded_PerkInfo:
    case node.eState_Loaded_QuestInfo:
    case node.eState_Loaded_BattleRecords:
    case node.eState_Loaded_CharStateInfo:
    case node.eState_Loaded_Achievement: // C_NA_0_20100520_ACSYSTEM_CHANGED
    case node.eState_Loaded_MissionInfo:
        break;
    };

    switch (value)
    {
    // r100928.2L
    case node.eTrans_None:
    case node.eTrans_Select_EquipInfo:
    case node.eTrans_Select_InvenInfo:
    case node.eTrans_Select_SkillInfo:
    case node.eTrans_Select_QuickInfo:
    case node.eTrans_Select_PerkInfo:
    case node.eTrans_Select_QuestInfo:
    case node.eTrans_Select_BattleRecords:
    case node.eTrans_Select_CharStateInfo:
    case node.eTrans_Select_Achievement: // C_NA_0_20100520_ACSYSTEM_CHANGED
    case node.eTrans_Select_MissionInfo:
    //
    case node.eTrans_Update_CharInfo:
    case node.eTrans_Update_InvenInfo:
    case node.eTrans_Update_CharState:
        break;
    }

    switch (value)
    {
    case node.eReq_None:
    case node.eReq_Release:
    case node.eReq_Update_CharInfo:
    case node.eReq_Update_InvenInfo:
    case node.eReq_Update_CharState:
        break;
    }
}

DBQueryProxyNode::CharacterNode::CharacterNode()
    : db_user_(NULL)
    , db_character_(NULL)
{
}

void DBQueryProxyNode::CharacterNode::Init(DBUser* db_user, DBCharacter* db_character)
{
    db_user_ = db_user;
    db_character_ = db_character;
    const bool clear_case = (db_character == NULL);
    if (clear_case)
    {
        // r100928.2L
        perk_table_.clear();
        char_state_table_.clear();
        ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
        /*
        ZeroMemory(&equip_table_, sizeof(equip_table_));
        ZeroMemory(&inven_table_, sizeof(inven_table_));
        ZeroMemory(&skill_table_, sizeof(skill_table_));
        ZeroMemory(&quick_table_, sizeof(quick_table_));
        ZeroMemory(&quest_table_, sizeof(quest_table_));
        ZeroMemory(&battle_records_table_, sizeof(battle_records_table_));
        */
    }
    // last process
    DBQueryProxyNode::Init();
}

bool DBQueryProxyNode::CharacterNode::BeginTransaction(eTransaction transaction)
{
    const bool is_checked = (shared_.transaction & transaction) != 0;

    BOOST_STATIC_ASSERT(\
        (CharacterNode::eTrans_Update_CharInfo  == CharacterNode::eReq_Update_CharInfo) &&
        (CharacterNode::eTrans_Update_InvenInfo == CharacterNode::eReq_Update_InvenInfo) &&
        (CharacterNode::eTrans_Update_CharState == CharacterNode::eReq_Update_CharState));
    // r100928.2L
    switch (transaction)
    {
    //case this->eTrans_Select_CharInfo :
    //case this->eTrans_Select_InvenInfo:
    //case this->eTrans_Select_CharState:
    //case this->eTrans_Select_Warehouse:
    // (NOTE) one time load process
    case this->eTrans_Select_EquipInfo:
    case this->eTrans_Select_InvenInfo:
    case this->eTrans_Select_SkillInfo:
    case this->eTrans_Select_QuickInfo:
    case this->eTrans_Select_PerkInfo:
    case this->eTrans_Select_QuestInfo:
    case this->eTrans_Select_BattleRecords:
    case this->eTrans_Select_CharStateInfo:
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    case this->eTrans_Select_Achievement:
#endif
    case this->eTrans_Select_MissionInfo:
    case this->eTrans_Select_CharacterExtraInfo: //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    case this->eTrans_Select_UserExtraInfo:
#endif // _NA_007932_20150106_USER_EXTRA_INFO
        if (!FlowControl::FCAssert(is_checked == false)) {
            return false;
        }
        break;
    //
    case this->eTrans_Update_CharInfo :
    case this->eTrans_Update_InvenInfo:
    case this->eTrans_Update_CharState:
    //case this->eTrans_Update_Warehouse:
        // (f100603.5L) add a clear routine by a latest data application mechanism
        shared_.request &= ~transaction;
        if (!FlowControl::FCAssert(is_checked == false)) {
            return false;
        }
        break;
    //
    default:
        ASSERT(!"unexpected transaction value");
        return false;
    }
    //
    shared_.transaction |= transaction;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::EndTransaction(eTransaction transaction, bool success)
{
    if (!FlowControl::FCAssert(shared_.transaction & transaction)) {
        return false;
    }
    switch (transaction)
    {
    // r100928.2L
    case this->eTrans_Select_EquipInfo:
        success ? shared_.state |= this->eState_Loaded_EquipInfo
                : shared_.state &= this->eState_Loaded_EquipInfo;
        break;
    case this->eTrans_Select_InvenInfo:
        success ? shared_.state |= this->eState_Loaded_InvenInfo
                : shared_.state &= this->eState_Loaded_InvenInfo;
        break;
    case this->eTrans_Select_SkillInfo:
        success ? shared_.state |= this->eState_Loaded_SkillInfo
                : shared_.state &= this->eState_Loaded_SkillInfo;
        break;
    case this->eTrans_Select_QuickInfo:
        success ? shared_.state |= this->eState_Loaded_QuickInfo
                : shared_.state &= this->eState_Loaded_QuickInfo;
        break;
    case this->eTrans_Select_PerkInfo:
        success ? shared_.state |= this->eState_Loaded_PerkInfo
                : shared_.state &= this->eState_Loaded_PerkInfo;
        break;
    case this->eTrans_Select_QuestInfo:
        success ? shared_.state |= this->eState_Loaded_QuestInfo
                : shared_.state &= this->eState_Loaded_QuestInfo;
        break;
    case this->eTrans_Select_BattleRecords:
        success ? shared_.state |= this->eState_Loaded_BattleRecords
                : shared_.state &= this->eState_Loaded_BattleRecords;
        break;
    case this->eTrans_Select_CharStateInfo:
        success ? shared_.state |= this->eState_Loaded_CharStateInfo
                : shared_.state &= this->eState_Loaded_CharStateInfo;
        break;
  #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    case this->eTrans_Select_Achievement:
        success ? shared_.state |= this->eState_Loaded_Achievement
                : shared_.state &= this->eState_Loaded_Achievement;
        break;
  #endif
    case this->eTrans_Select_MissionInfo:
        success ? shared_.state |= this->eState_Loaded_MissionInfo
                : shared_.state &= this->eState_Loaded_MissionInfo;
        break;
    case this->eTrans_Select_CharacterExtraInfo: //_NA_006372_20130108_BADGE_ITEM
        success ? shared_.state |= this->eState_Loaded_CharacterExtraInfo
                : shared_.state &= this->eState_Loaded_CharacterExtraInfo;
        break;
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    case this->eTrans_Select_UserExtraInfo:
        success ? shared_.state |= this->eState_Loaded_UserExtraInfo
                : shared_.state &= this->eState_Loaded_UserExtraInfo;
        break;
#endif // _NA_007932_20150106_USER_EXTRA_INFO

    case this->eTrans_Update_CharInfo :
    case this->eTrans_Update_InvenInfo:
    case this->eTrans_Update_CharState:
    //case this->eTrans_Update_Warehouse:
        break;
    //
    default:
        ASSERT(!"unexpected transaction value");
        return false;
    }
    // processed
    shared_.transaction &= ~transaction;
    shared_.loaded = ((shared_.state & this->eState_All_SubData_Loaded) ==
                      this->eState_All_SubData_Loaded);
    return true;
}

void DBQueryProxyNode::CharacterNode::RequestEnterGame(const MSG_DG_CHARINFO_QUERY_SYN* recv_msg,
                                                       ChildServerSession* server_session)
{
    shared_.request |= this->eReq_EnterGamePending;
    request_info_.session = server_session;
    request_info_.message = *recv_msg;
}

bool DBQueryProxyNode::CharacterNode::ProcessRequestEvent(const ProxyTransaction& transaction,
                                                          bool success)
{
    __UNUSED((transaction, success)); //reserved
    if (this->loaded() == false) {
        return false;
    }

    if (shared_.request & this->eReq_EnterGamePending)
    {
        shared_.request &= this->eReq_EnterGamePending;
        DBUser* db_user = db_character_->GetParentDBUser();
        if (db_user == NULL)
        {
            return true;    // 강제..transaction success..
        }
        DBCharacter* connected_char = db_user->GetConnectedChar();
        if (connected_char != db_character_) {
            return false;
        }
        DBQueryProxyManager* const query_proxy_manager = DBQueryProxyManager::Instance();
        query_proxy_manager->SendCharInfoToGameOnAllCharDataLoaded(db_user, db_character_);
        //
        MSG_DG_CHARINFO_QUERY_ACK msg_ack;
        db_user->SendPacketToGameServer(&msg_ack, sizeof(msg_ack));
    };
    return true;
}

bool DBQueryProxyNode::CharacterNode::OnCreateCharacter()
{
    ;{  // initialize an equipments information
        equip_table_._OperateAll(equip_table_.eOper_ClearAllToDefault |
                                 equip_table_.eOper_CopyAllLatestDataToCommitedData,
                                 equip_table_.records_,
                                 equip_table_.kMaxNumberOfSlots,
                                 equip_table_.kSlotIndex);
        shared_.state |= this->eState_Loaded_EquipInfo;
    };
    ;{
        inven_table_._OperateAll(inven_table_.eOper_ClearAllToDefault |
                                 inven_table_.eOper_CopyAllLatestDataToCommitedData,
                                 inven_table_.records_,
                                 inven_table_.kMaxNumberOfSlots,
                                 inven_table_.kSlotIndex);
        shared_.state |= this->eState_Loaded_InvenInfo;
    };
    if (const nsSlot::NewCharacterItemProvider* provider = \
            nsSlot::NewCharacterItemProvider::Instance())
    {
        const BASE_PLAYERINFO* player_info = db_character_->GetInfo();
        ITEMSLOTEX_INSERVER making_inventory[MAX_INVENTORY_SLOT_NUM];
        const int number_of_aligned = provider->MakeStream(\
            static_cast<eCHAR_TYPE>(player_info->m_byClassCode), making_inventory);
        this->StoreItemSlotInfo(inven_table_.kSlotIndex,
                                making_inventory, static_cast<POSTYPE>(number_of_aligned));
    };
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::Update(eUpdateType update_type)
{
    bool changed_events = false;
    // r100928.2L
    if (update_type & this->eUpdate_ItemSlotInfo) {
        bool cleared = UpdateItemSlotInfo();
        if (cleared == false) {
            changed_events = true;
        }
    }
    if (update_type & this->eUpdate_SlotInfo) {
        bool cleared = UpdateSlotInfo();
        if (cleared == false) {
            changed_events = true;
        }
    }
    if (update_type & this->eUpdate_Perk) {
        bool cleared = UpdatePerk();
        if (cleared == false) {
            changed_events = true;
        }
    }
    if (update_type & this->eUpdate_Quest) {
        bool cleared = UpdateQuest();
        if (cleared == false) {
            changed_events = true;
        }
    }
    if (update_type & this->eUpdate_BattleRecords) {
        bool cleared = UpdateBattleRecords();
        if (cleared == false) {
            changed_events = true;
        }
    }
    if (update_type & this->eUpdate_CharStateInfo) {
        bool cleared = UpdateCharStates();
        if (cleared == false) {
            changed_events = true;
        }
    }
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    if (update_type & this->eUpdate_Achievement) {
        bool cleared = UpdateAchievement();
        if (cleared == false) {
            changed_events = true;
        }
    }
#endif
    if (update_type & this->eUpdate_MissionInfo) {
        bool cleared = UpdateMissionInfo();
        if (cleared == false) {
            changed_events = true;
        }
    }
    if (update_type & this->eUpdate_CharacterExtraInfo) {
        bool cleared = UpdateCharacterExtraInfo();
        if (cleared == false) {
            changed_events = true;
        }
    }
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    if (update_type & this->eUpdate_UserExtraInfo) {
        bool cleared = UpdateUserExtraInfo();
        if (cleared == false) {
            changed_events = true;
        }
    }
#endif // _NA_007932_20150106_USER_EXTRA_INFO

    if (changed_events) {
        return false;
    }
    return true;
}

//!~ _NA_006372_20130108_BADGE_ITEM
bool DBQueryProxyNode::CharacterNode::DBResult(const Query_CharacterExtraInfo_Select& query_result)
{
    const Query_CharacterExtraInfo_Select::sQueryResult* record_it = query_result.result_records_;
    int result_count = query_result.GetResultRowNum();
    //
    for (int i = 0; i < result_count; i++)
    {
        TiXmlDocument character_info_ex_doc;
        character_info_ex_doc.Parse(record_it[i].extra_info_data_.extra_info_data_);
        switch(record_it[i].extra_info_data_.extra_info_type_)
        {
            case kExtraType_ToggleItemInfo:
            {
#ifdef _NA_006372_20130108_BADGE_ITEM
                TiXmlElement* toggle_item_elem = character_info_ex_doc.FirstChildElement("TOGGLE_ITEM");
                if (toggle_item_elem == NULL) {
                    break;
                }

                ToggleItemInfos& toggle_item_infos = db_character_->GetToggleItemInfo();
                //
                for (TiXmlElement* elem = toggle_item_elem->FirstChildElement("ITEM_INFO");
                    elem; elem = elem->NextSiblingElement())
                {
                    int toggle_item_type, is_equiped, item_pos, item_code;
                    elem->QueryIntAttribute("TYPE",  &toggle_item_type);
                    elem->QueryIntAttribute("EQUIP", &is_equiped);
                    elem->QueryIntAttribute("POS",   &item_pos);
                    elem->QueryIntAttribute("CODE",  &item_code);

                    //printf("%d, %d, %d, %d\n", toggle_item_type, is_equiped, item_pos, item_code);
                    if (toggle_item_type >= kToggleItem_Badge && toggle_item_type < kToggleItemMax)
                    {
                        nsSlot::TOGGLE_ITEMS* toggle_item_info = \
                            &toggle_item_infos.toggle_item_info_[toggle_item_type];
                        toggle_item_info->toggle_item_type_ = (ToggleItemType)toggle_item_type;
                        toggle_item_info->is_equiped_       = (is_equiped ? true : false);
                        toggle_item_info->item_pos_         = item_pos;
                        toggle_item_info->item_code_        = item_code;
                    }
                }
#endif //_NA_006372_20130108_BADGE_ITEM
            } break;

            case kExtraType_BattleGroundRecordInfo:
            {
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                BattleGroundRecordInfos& battle_ground_record_infos = \
                                                        db_character_->GetBattleGroundRecordInfos();

                int index = 0;
                for (TiXmlElement* elem = character_info_ex_doc.FirstChildElement("BATTE_GROUND_RECORD");
                    elem; elem = elem->NextSiblingElement())
                {
                    int map_code;
                    int total_count;
                    int win_count;
                    int draw_count;
                    int lose_count;
                    int dis_count;
                    int kill_count;
                    int death_count;
                    int max_series_kill;
                    int strongpoint_attack;
                    int strongpoint_defense;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                    int assist_count=0;
#endif  //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

                    elem->QueryIntAttribute("MAPCODE", &map_code);
                    elem->QueryIntAttribute("TOTAL", &total_count);
                    elem->QueryIntAttribute("WIN", &win_count);
                    elem->QueryIntAttribute("DRAW", &draw_count);
                    elem->QueryIntAttribute("LOSE", &lose_count);
                    elem->QueryIntAttribute("DIS", &dis_count);
                    elem->QueryIntAttribute("KILL", &kill_count);
                    elem->QueryIntAttribute("DEATH", &death_count);
                    elem->QueryIntAttribute("MAX_KILL", &max_series_kill);
                    elem->QueryIntAttribute("ATTACK", &strongpoint_attack);
                    elem->QueryIntAttribute("DEFENSE", &strongpoint_defense);
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                    elem->QueryIntAttribute("ASSIST", &assist_count);
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                    
                    if (index >= BattleGroundRecordInfos::kRecordMaxCount)
                    {
                        continue;
                    }
                    BattleGroundRecord* record = \
                        &battle_ground_record_infos.battle_ground_record[index++];
                    if (record == NULL)
                    {
                        continue;
                    }

                    record->map_code = map_code;
                    record->total_count = total_count;
                    record->win_count = win_count;
                    record->draw_count = draw_count;
                    record->lose_count = lose_count;
                    record->dis_count = dis_count;
                    record->kill_count = kill_count;
                    record->death_count = death_count;
                    record->max_series_kill = max_series_kill;
                    record->strongpoint_attack = strongpoint_attack;
                    record->strongpoint_defense = strongpoint_defense;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                    record->assist_count = assist_count;
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

                }


#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            }
            break;
            case kExtraType_DailyQuestResetTime:
            {
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
                TiXmlElement* daily_quest_reset_time_elem = \
                    character_info_ex_doc.FirstChildElement("DAILY_QUEST_RESET_TIME");
                if (daily_quest_reset_time_elem == NULL) {
                    db_character_->SetDailyQuestResetTime(0);
                }
                else 
                {
                    const char* datetime_string = daily_quest_reset_time_elem->Attribute("TIME");
                    if (datetime_string)
                    {
                        tm s_tm;
                        sscanf(datetime_string, "%4d-%2d-%2d %2d:%2d:%2d",
                            &s_tm.tm_year, &s_tm.tm_mon, &s_tm.tm_mday, 
                            &s_tm.tm_hour, &s_tm.tm_min, &s_tm.tm_sec);

                        CTime daily_quest_reset_time( \
                            s_tm.tm_year, s_tm.tm_mon, s_tm.tm_mday, 
                            s_tm.tm_hour, s_tm.tm_min, s_tm.tm_sec);

                        db_character_->SetDailyQuestResetTime(daily_quest_reset_time);
                    }
                }
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
            } break;
            case kExtraType_GoldRushRecordInfo:
            {
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
                GoldRushRecordInfos& goldrush_record_infos = db_character_->GetGoldRushRecordInfos();
                int index = 0;
                for (TiXmlElement* elem = character_info_ex_doc.FirstChildElement("GOLDRUSH_RECORD");
                    elem; elem = elem->NextSiblingElement())
                {
                    int map_code;
                    int total_count;
                    int win_count;
                    int draw_count;
                    int lose_count;
                    int dis_count;
                    int kill_count;
                    int death_count;
                    int max_series_kill;
                    int assist_count=0;

                    elem->QueryIntAttribute("MAPCODE", &map_code);
                    elem->QueryIntAttribute("TOTAL", &total_count);
                    elem->QueryIntAttribute("WIN", &win_count);
                    elem->QueryIntAttribute("DRAW", &draw_count);
                    elem->QueryIntAttribute("LOSE", &lose_count);
                    elem->QueryIntAttribute("DIS", &dis_count);
                    elem->QueryIntAttribute("KILL", &kill_count);
                    elem->QueryIntAttribute("DEATH", &death_count);
                    elem->QueryIntAttribute("MAX_KILL", &max_series_kill);
                    elem->QueryIntAttribute("ASSIST", &assist_count);

                    if (index >= BattleGroundRecordInfos::kRecordMaxCount)
                    {
                        continue;
                    }
                    GoldRushRecord* record = \
                        &goldrush_record_infos.goldrush_record[index++];
                    if (record == NULL)
                    {
                        continue;
                    }

                    record->map_code = map_code;
                    record->total_count = total_count;
                    record->win_count = win_count;
                    record->draw_count = draw_count;
                    record->lose_count = lose_count;
                    record->dis_count = dis_count;
                    record->kill_count = kill_count;
                    record->death_count = death_count;
                    record->max_series_kill = max_series_kill;
                    record->assist_count = assist_count;
                }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
            }break;
            case kExtraType_MiniGameBingoInfo:
                {
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
                    MiniGameBingoInfos& minigame_bingo_info = db_character_->GetMiniGameBingoInfos();
                    TiXmlElement* minigame_bingo_elem = character_info_ex_doc.FirstChildElement("MINIGAME_BINGO");
                    if (minigame_bingo_elem == NULL) {
                        break;
                    }

                    int bingo_event_index = 0;
                    minigame_bingo_elem->QueryIntAttribute("INDEX", &bingo_event_index);
                    minigame_bingo_info.bingo_event_index = bingo_event_index;

                    int i = 0;
                    for (TiXmlElement* elem = minigame_bingo_elem->FirstChildElement("BINGO");
                        elem; elem = elem->NextSiblingElement(), ++i)
                    {
                        int bingo_number, bingo_state;
                        elem->QueryIntAttribute("NUMBER", &bingo_number);
                        elem->QueryIntAttribute("STATE", &bingo_state);
                        minigame_bingo_info.m_bingo_infos[i].bingo_number = bingo_number;

                        if (bingo_state >= eMINIGAME_BINGO_STATE_EMPTY
                            && bingo_state <= eMINIGAME_BINGO_STATE_FREE)
                        {
                            minigame_bingo_info.m_bingo_infos[i].bingo_state =
                                static_cast<eMINIGAME_BINGO_STATE>(bingo_state);
                        }
                    }
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
                }break;
        }
    };

    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_CharacterExtraInfo_Update& query_result)
{
    const Query_CharacterExtraInfo_Update::sQueryParam& parameters = query_result.parameters_;
    if ((parameters.extra_info_data_.extra_info_type_ + 1) == kExtraType_Max) {
        db_character_->SetExtraInfoQueried(false);
        db_character_->SetExtraInfoModified(false);
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::SelectCharacterExtraInfo()
{
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL) {
        return true;    // 강제..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    DBQueryProxyCharNode* const char_node = db_character_->db_query_node();
    //
    STRING_SAFER_QUERY64 query_buffer;
    query_buffer.MakeString(_T("{?=call S_CharacterExtraInfo_Select (?)}"));

    Query_CharacterExtraInfo_Select* query = Query_CharacterExtraInfo_Select::ALLOC();
    query->SetQuery(query_buffer);
    query->SetUserKey(user_guid);
    query->set_char_guid(char_guid);
    // setup parameters
    query->parameters_.char_guid_ = char_guid;
    //
    db_user->DBQuery(DG_CHARINFO, DG_CHARACTER_EXTRA_INFO_DBR, query);
    char_node->BeginTransaction(char_node->eTrans_Select_CharacterExtraInfo);

    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateCharacterExtraInfo()
{
    const bool loaded = (shared_.state & this->eState_Loaded_CharacterExtraInfo) != 0;
    if (loaded == false) {
        return true;
    };
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL) {
        return true;    // 강제..transaction success..
    }

    if (db_character_->GetExtraInfoModified() == false || 
        db_character_->GetExtraInfoQueried() == true) {
        return true;
    }

    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };

    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();

    int character_extra_info_type[] = {
        kExtraType_ToggleItemInfo, //_NA_006372_20130108_BADGE_ITEM
        kExtraType_BattleGroundRecordInfo, // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        kExtraType_DailyQuestResetTime, // _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
        kExtraType_GoldRushRecordInfo,     // _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        kExtraType_MiniGameBingoInfo,      // _NA_008404_20150712_MINIGAME_SYSTEM
        0,
    };
    //
    for (int* it = &character_extra_info_type[0]; (*it) != 0; ++it)
    {
        int extra_info_type = (*it);
        const char* xml_string = NULL;
        TiXmlPrinter printer;
        switch(extra_info_type)
        {
            case kExtraType_ToggleItemInfo:
            {
#ifdef _NA_006372_20130108_BADGE_ITEM
                TiXmlDocument doc;
                TiXmlElement* toggle_info = new TiXmlElement("TOGGLE_ITEM");
                doc.LinkEndChild(toggle_info);

                ToggleItemInfos& toggle_item_info = db_character_->GetToggleItemInfo();

                for (nsSlot::TOGGLE_ITEMS* it2 = toggle_item_info.begin(); \
                    it2 != toggle_item_info.end(); ++it2)
                {
                    TiXmlElement* item_info = new TiXmlElement("ITEM_INFO");
                    item_info->SetAttribute("TYPE",  it2->toggle_item_type_);
                    item_info->SetAttribute("EQUIP", it2->is_equiped_);
                    item_info->SetAttribute("POS",   it2->item_pos_);
                    item_info->SetAttribute("CODE",  it2->item_code_);

                    toggle_info->LinkEndChild(item_info);
                }

                doc.Accept(&printer);
                xml_string = printer.CStr();
#endif //_NA_006372_20130108_BADGE_ITEM
            } break;

            case kExtraType_BattleGroundRecordInfo:
                {
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    TiXmlDocument doc;
                    BattleGroundRecordInfos& battle_ground_record_infos = \
                                                        db_character_->GetBattleGroundRecordInfos();

                    BattleGroundRecord* itr = battle_ground_record_infos.begin();
                    for (; itr != battle_ground_record_infos.end(); ++itr)
                    {
                        TiXmlElement* battle_ground_record = new TiXmlElement("BATTE_GROUND_RECORD");
                        battle_ground_record->SetAttribute("MAPCODE", itr->map_code);
                        battle_ground_record->SetAttribute("TOTAL", itr->total_count);
                        battle_ground_record->SetAttribute("WIN", itr->win_count);
                        battle_ground_record->SetAttribute("DRAW", itr->draw_count);
                        battle_ground_record->SetAttribute("LOSE", itr->lose_count);
                        battle_ground_record->SetAttribute("DIS", itr->dis_count);
                        battle_ground_record->SetAttribute("KILL", itr->kill_count);
                        battle_ground_record->SetAttribute("DEATH", itr->death_count);
                        battle_ground_record->SetAttribute("MAX_KILL", itr->max_series_kill);
                        battle_ground_record->SetAttribute("ATTACK", itr->strongpoint_attack);
                        battle_ground_record->SetAttribute("DEFENSE", itr->strongpoint_defense);
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                        battle_ground_record->SetAttribute("ASSIST", itr->assist_count);
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

                        doc.LinkEndChild(battle_ground_record);
                    }

                    doc.Accept(&printer);
                    xml_string = printer.CStr();
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                }
                break;
            case kExtraType_DailyQuestResetTime:
            {
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
                TiXmlDocument doc;
                TiXmlElement* daily_quest_reset_time_elem = new TiXmlElement("DAILY_QUEST_RESET_TIME");

                CTime daily_quest_reset_time = db_character_->GetDailyQuestResetTime();

                char datetime_string[MAX_DATETIME_SIZE+1] = { 0, };
                util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( \
                    util::GetDateTime_YYYYMMDDHHMMSS(daily_quest_reset_time.GetTime())*1000, datetime_string);
                
                daily_quest_reset_time_elem->SetAttribute("TIME", datetime_string);

                doc.LinkEndChild(daily_quest_reset_time_elem);
                doc.Accept(&printer);
                xml_string = printer.CStr();
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
            } break;
            case kExtraType_GoldRushRecordInfo:
            {
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
                TiXmlDocument doc;
                GoldRushRecordInfos& goldrush_record_infos = \
                    db_character_->GetGoldRushRecordInfos();

                GoldRushRecord* itr = goldrush_record_infos.begin();
                for (; itr != goldrush_record_infos.end(); ++itr)
                {
                    TiXmlElement* goldrush_record = new TiXmlElement("GOLDRUSH_RECORD");
                    goldrush_record->SetAttribute("MAPCODE", itr->map_code);
                    goldrush_record->SetAttribute("TOTAL", itr->total_count);
                    goldrush_record->SetAttribute("WIN", itr->win_count);
                    goldrush_record->SetAttribute("DRAW", itr->draw_count);
                    goldrush_record->SetAttribute("LOSE", itr->lose_count);
                    goldrush_record->SetAttribute("DIS", itr->dis_count);
                    goldrush_record->SetAttribute("KILL", itr->kill_count);
                    goldrush_record->SetAttribute("DEATH", itr->death_count);
                    goldrush_record->SetAttribute("MAX_KILL", itr->max_series_kill);
                    goldrush_record->SetAttribute("ASSIST", itr->assist_count);

                    doc.LinkEndChild(goldrush_record);
                }
                doc.Accept(&printer);
                xml_string = printer.CStr();
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
            }break;
            case kExtraType_MiniGameBingoInfo:
            {
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
                MiniGameBingoInfos& minigame_bingo_info = db_character_->GetMiniGameBingoInfos();

                TiXmlDocument doc;
                TiXmlElement* minigame_bingo_elem = new TiXmlElement("MINIGAME_BINGO");
                minigame_bingo_elem->SetAttribute("INDEX",  minigame_bingo_info.bingo_event_index);
                doc.LinkEndChild(minigame_bingo_elem);

                for (int i = 0; i < MINIGAME_BINGO_LENGTH; ++i)
                {
                    TiXmlElement* bingo_elem = new TiXmlElement("BINGO");
                    bingo_elem->SetAttribute("NUMBER",  minigame_bingo_info.m_bingo_infos[i].bingo_number);
                    bingo_elem->SetAttribute("STATE", minigame_bingo_info.m_bingo_infos[i].bingo_state);

                    minigame_bingo_elem->LinkEndChild(bingo_elem);
                }

                doc.Accept(&printer);
                xml_string = printer.CStr();
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
            }break;
        }

        if (xml_string == NULL) {
            continue;
        }
        //
        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString(_T("{?=call S_CharacterExtraInfo_Update (?,?,?)}"));
        Query_CharacterExtraInfo_Update* const query = Query_CharacterExtraInfo_Update::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);
        query->set_char_guid(char_guid);
        // setup parameters
        if (Query_CharacterExtraInfo_Update::sQueryParam* parameters = &query->parameters_)
        {
            parameters->char_guid_ = char_guid;
            parameters->extra_info_data_.extra_info_type_ = extra_info_type;
            memset(parameters->extra_info_data_.extra_info_data_, 0, \
                sizeof(parameters->extra_info_data_.extra_info_data_));
            ASSERT(strlen(xml_string) < sizeof(parameters->extra_info_data_.extra_info_data_));
            
            strncpy(parameters->extra_info_data_.extra_info_data_, xml_string, \
                sizeof(parameters->extra_info_data_.extra_info_data_));
        }
        db_user->DBQuery(DG_CHARINFO, DG_CHARACTER_EXTRA_INFO_DBR, query);
        
        db_character_->SetExtraInfoQueried(true);
    };

    return true;
}
//~! _NA_006372_20130108_BADGE_ITEM

bool DBQueryProxyNode::CharacterNode::QueryNext(ProxyTransaction::eTransaction last_transaction)
{
    switch(last_transaction)
    {
        case ProxyTransaction::eTrans_CharacterExtraInfo_Select:
#ifdef _NA_007086_20140318_MONSTERBOOK
        {
            SelectCharacterMonsterBook();
        } break;
        case ProxyTransaction::eTrans_CharacterMonsterBook_Select:
#endif //_NA_007086_20140318_MONSTERBOOK
        {
            const nsQuery::ProxyTransaction transaction = {
                ProxyTransaction::eTrans_CharacterExtraInfo_Select
            };
            EndTransaction(eTrans_Select_CharacterExtraInfo, true);
            ProcessRequestEvent(transaction, true);
        } break;
    }

    return true;
}

#ifdef _NA_007086_20140318_MONSTERBOOK
bool DBQueryProxyNode::CharacterNode::SelectCharacterMonsterBook()
{
	DBUser* const db_user = db_character_->GetParentDBUser();
	if (db_user == NULL) {
		return true;    // 강제..transaction success..
	}
	const USERGUID user_guid = db_user->GetUserGuid();
	const CHARGUID char_guid = db_character_->GetCharGuid();
	DBQueryProxyCharNode* const char_node = db_character_->db_query_node();

	//
	STRING_SAFER_QUERY64 query_buffer;
	query_buffer.MakeString(_T("{?=call S_CharacterMonsterBook_Select (?)}"));

	Query_CharacterMonsterBook_Select* query = Query_CharacterMonsterBook_Select::ALLOC();
	query->SetQuery(query_buffer);
	query->SetUserKey(user_guid);
	query->set_char_guid(char_guid);
	// setup parameters
	query->parameters_.char_guid_ = char_guid;
	//
	db_user->DBQuery(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_DBR, query);

	return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult( const Query_CharacterMonsterBook_Select& query_result )
{
    const Query_CharacterMonsterBook_Select::sQueryResult* record_it = query_result.result_records_;
    int result_count = query_result.GetResultRowNum();

    MonsterBook& monsterbook = db_character_->GetMonsterBook();
    monsterbook.clear();
    //
    for (int i = 0; i < result_count; i++)
    {
        monsterbook.monster_code[monsterbook.monster_code_count++] = record_it[i].monster_code;
    };

    QueryNext(ProxyTransaction::eTrans_CharacterMonsterBook_Select);

    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateCharacterMonsterBook(DWORD monster_code)
{
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL) {
        return true;    // 강제..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    DBQueryProxyCharNode* const char_node = db_character_->db_query_node();

    //
    STRING_SAFER_QUERY64 query_buffer;
    query_buffer.MakeString(_T("{?=call S_CharacterMonsterBook_Update (?, ?)}"));

    Query_CharacterMonsterBook_Update* query = Query_CharacterMonsterBook_Update::ALLOC();
    query->SetQuery(query_buffer);
    query->SetUserKey(user_guid);
    query->set_char_guid(char_guid);
    // setup parameters
    query->parameters_.char_guid_ = char_guid;
    query->parameters_.monster_code_ = monster_code;
    //
    db_user->DBQuery(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_DBR, query);

    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult( const Query_CharacterMonsterBook_Update& query_result )
{
    if (query_result.parameters_.result_ == 0)
    {
        DBUser* db_user = db_character_->GetParentDBUser();
        if (db_user == NULL)
        {
            return true;    // 강제..transaction success..
        }
        DBCharacter* connected_char = db_user->GetConnectedChar();
        if (connected_char != db_character_) {
            return false;
        }

        DWORD monster_code = query_result.parameters_.monster_code_;

        MonsterBook& monsterbook = db_character_->GetMonsterBook();
        if (monsterbook.find(monster_code) == 0) {
            monsterbook.push_back(monster_code);

            MSG_DG_CHARACTER_MONSTERBOOK_ADD_ACK msg;
            msg.monster_code = monster_code;
            db_user->SendPacketToGameServer(&msg, sizeof(msg));
        }
    }
    return true;
}
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
bool DBQueryProxyNode::CharacterNode::SelectUserExtraInfo()
{
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL) {
        return true;    // 강제..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    DBQueryProxyCharNode* const char_node = db_character_->db_query_node();
    
    STRING_SAFER_QUERY64 query_buffer;
    query_buffer.MakeString(_T("{?=call S_UserExtraInfo_Select (?)}"));

    Query_UserExtraInfo_Select* query = Query_UserExtraInfo_Select::ALLOC();
    query->SetQuery(query_buffer);
    query->SetUserKey(user_guid);
    query->set_char_guid(char_guid);
    
    query->parameters_.user_guid_ = user_guid;
    
    db_user->DBQuery(DG_CHARINFO, DG_USER_EXTRA_INFO_DBR, query);
    char_node->BeginTransaction(char_node->eTrans_Select_UserExtraInfo);

    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_UserExtraInfo_Select& query_result)
{
    const Query_UserExtraInfo_Select::sQueryResult* record_it = query_result.result_records_;
    int result_count = query_result.GetResultRowNum();
    

    for (int i = 0; i < result_count; i++)
    {
        TiXmlDocument user_info_ex_doc;
        user_info_ex_doc.Parse(record_it[i].extra_info_data_.extra_info_data_);
        switch(record_it[i].extra_info_data_.extra_info_type_)
        {
        // UserExtraInfo 추가시 수정
        case kUserExtraType_AttendanceInfo:
            {
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
                TiXmlElement* attendance_elem = user_info_ex_doc.FirstChildElement("ATTENDANCE");
                if (attendance_elem == NULL) {
                    break;
                }

                AttendanceInfo& attendance_infos = db_character_->GetAttendanceInfos();
                
                TiXmlElement* base_info_elem = attendance_elem->FirstChildElement("BASE_INFO");
                if (base_info_elem != NULL)
                {
                    int year, month, type;

                    base_info_elem->QueryIntAttribute("YEAR", &year);
                    base_info_elem->QueryIntAttribute("MONTH", &month);
                    base_info_elem->QueryIntAttribute("TYPE", &type);

                    if (type < eATTENDANCE_CALENDAR || type >= eATTENDANCE_MAX)
                    {
                        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]invalid attendance type(%d)", type);
                        type = eATTENDANCE_SUPERPOSITION;
                    }
                    
                    attendance_infos.init(year, month, static_cast<eATTENDANCE_TYPE>(type), eATTENDANCE_INFO_NOT_ATTEND);
                }

                int day = 0;
                for (TiXmlElement* elem = attendance_elem->FirstChildElement("DAY");
                    elem; elem = elem->NextSiblingElement(), ++day)
                {
                    int info;
                    elem->QueryIntAttribute("INFO", &info);

                    if (info >= eATTENDANCE_INFO_NOT_ATTEND && info < eATTENDANCE_INFO_MAX
                        && day >= 0 && day < AttendanceInfo::kRecordMaxCount)
                    {
                        attendance_infos.SetAttendance(day, static_cast<eATTENDANCE_INFO>(info));
                    }
                    else
                    {
                        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]invalid attendance info(%d) or day(%d)", info, day);
                    }
                }
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
            }
            break;
        default:
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a user extra info type(%d) in user (%u)|"),
                __FUNCTION__, record_it[i].extra_info_data_.extra_info_type_, query_result.GetUserKey());
            break;
        }
    };

    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_UserExtraInfo_Update& query_result)
{
    const Query_UserExtraInfo_Update::sQueryParam& parameters = query_result.parameters_;
    if ((parameters.extra_info_data_.extra_info_type_ + 1) == kUserExtraType_Max) {
        db_character_->SetUserExtraInfoQuery(false);
        db_character_->SetUserExtraInfoModify(false);
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateUserExtraInfo()
{
    const bool loaded = (shared_.state & this->eState_Loaded_UserExtraInfo) != 0;
    if (loaded == false) {
        return true;
    };
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL) {
        return true;    // 강제..transaction success..
    }

    if (db_character_->GetUserExtraInfoModify() == false || 
        db_character_->GetUserExtraInfoQuery() == true) {
            return true;
    }

    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };

    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();


    int user_extra_info_type[] = {
        kUserExtraType_AttendanceInfo,          // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        0,
    };
    //
    for (int* it = &user_extra_info_type[0]; (*it) != 0; ++it)
    {
        int extra_info_type = (*it);
        const char* xml_string = NULL;
        TiXmlPrinter printer;
        switch(extra_info_type)
        {
        // UserExtraInfo 추가시 수정
        case kUserExtraType_AttendanceInfo:
            {
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM 
                TiXmlDocument doc;
                TiXmlElement* attendance_info_elem = new TiXmlElement("ATTENDANCE");
                doc.LinkEndChild(attendance_info_elem);

                AttendanceInfo& attendance_infos = db_character_->GetAttendanceInfos();

                {
                    TiXmlElement* base_info = new TiXmlElement("BASE_INFO");
                    base_info->SetAttribute("YEAR", attendance_infos.GetYear());
                    base_info->SetAttribute("MONTH", attendance_infos.GetMonth());
                    base_info->SetAttribute("TYPE", attendance_infos.GetType());

                    attendance_info_elem->LinkEndChild(base_info);
                }

                for (eATTENDANCE_INFO* it = attendance_infos.begin(); \
                    it != attendance_infos.end(); ++it)
                {
                    TiXmlElement* day_info = new TiXmlElement("DAY");
                    day_info->SetAttribute("INFO", *it);

                    attendance_info_elem->LinkEndChild(day_info);
                }

                doc.Accept(&printer);
                xml_string = printer.CStr();
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
            }break;
        default:
            {
                SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a user extra info type(%d) in user guid(%u)|"),
                    __FUNCTION__, extra_info_type, user_guid);
                break;
            }break;
        }

        if (xml_string == NULL) {
            continue;
        }
        //
        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString(_T("{?=call S_UserExtraInfo_Update (?,?,?)}"));
        Query_UserExtraInfo_Update* const query = Query_UserExtraInfo_Update::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);
        query->set_char_guid(char_guid);
        // setup parameters
        if (Query_UserExtraInfo_Update::sQueryParam* parameters = &query->parameters_)
        {
            parameters->user_guid_ = user_guid;
            parameters->extra_info_data_.extra_info_type_ = extra_info_type;
            memset(parameters->extra_info_data_.extra_info_data_, 0, \
                sizeof(parameters->extra_info_data_.extra_info_data_));
            ASSERT(strlen(xml_string) < sizeof(parameters->extra_info_data_.extra_info_data_));

            strncpy(parameters->extra_info_data_.extra_info_data_, xml_string, \
                sizeof(parameters->extra_info_data_.extra_info_data_));
        }
        db_user->DBQuery(DG_CHARINFO, DG_USER_EXTRA_INFO_DBR, query);
        db_character_->SetUserExtraInfoQuery(true);
    };

    return true;
}
#endif // _NA_007932_20150106_USER_EXTRA_INFO
}; //end of namespace 'nsQuery'

//==================================================================================================
