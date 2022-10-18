#include "stdafx.h"
#include "./DBCharacter.h"

#include <TimeConverter.h>
#include <SolarDateTime.h>

#include <SerializeStream.h>
#include <VersionControl.h>
#include <SCItemSlot.h>

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#else
#include <AreaConquest.h>
#endif

#include <ServerStruct.h>
#include <PacketStruct_DG.h>
#include <WayPoint.h>
#include <CollectedList.h>

#include "DBUser.h"
#include "DBProxyServerEx.h"
#include "Services/GuildManagerInDBP.h"
#include "ProxyNodes/DBQueryProxyManager.h"

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    #include "Services/SkillCoolTimeStorage.h"
#endif

//==================================================================================================
// (WAVERIX) (090506) (REVISION) v0.1 - 코드 정리
// (CHANGES) (f100416.5L) (WAVERIX) fit it coding style

DBCharacter::DBCharacter(void) : is_released_(false)
{
    player_info_ = new BASE_PLAYERINFO;
    ZeroMemory(player_info_, sizeof(*player_info_));
    // (f100804.6L) remove DB_SI_~ enumeration
    // because ambigous enumeration type has caused an invalid usage
    // - types of SI_WAREHOUSE and SI_GUILDWAREHOUSE don't use in a db character
    way_point_ = new WayPoint;
    char_way_point_ = new CharWayPoint;
    collected_list_ = new CollectedList;
    ReleaseGuildWarehouse();

    db_user_ = NULL;

    //----------------------------------------------------------------------------------------------
    // NOTE: f100928.2L, forced clear memory. should be don't declare non-pod fields in this ranges.
    ZeroMemory(pod_fields_start_, static_cast<int>(pod_fields_end_ - pod_fields_start_));
    /*
    // removed by f100928.2L
    delete_checked_ = 0;
    delete_date_[0] = '\0';

    db_last_updated_tick_ = 0;
    login_time_ = 0;                // 캐릭터별 로그인 시간
    play_time_after_last_updated_ = 0;
    event_last_updated_time_ = 0;           // 마지막 업데이트한 시각(이벤트용)
    event_use_time_ = 0;
    event_use_time_normal_ = 0;   //__NA000000_090506_EVENT_TYPE_MODIFICATION__

    state_total_info_.m_Count = 0;

#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    repurchase_items_.m_Count = 0;    
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496
    Invitatedfriend_count_.friend_count_ = 0;

    ZeroMemory(&state_gm_info_, sizeof(state_gm_info_));
    */
    /*
    // removed by f100928.2L
    accumulated_exp_ = 0;
    group_high_member_ = NULL;
    */
    //__KR_001355_20090608_GROUPINGSYSTEM_DATA
    state_high_member_ = GROUP_HAVE_NONE;
    state_low_member_ = GROUP_HAVE_NONE;
    //
}

DBCharacter::~DBCharacter(void)
{
    SAFE_DELETE(way_point_);
    SAFE_DELETE(char_way_point_);
    SAFE_DELETE(collected_list_);
    // (f100804.5L)
    SAFE_DELETE(player_info_);
    is_released_ = true;
}

void
DBCharacter::Clear()
{
    db_query_node_.Init(NULL, NULL);
    ZeroMemory(player_info_, sizeof(*player_info_));
    way_point_->Clear();
    char_way_point_->Clear();
    collected_list_->Clear();
    ReleaseGuildWarehouse();

    db_user_ = NULL;
    //----------------------------------------------------------------------------------------------
    // NOTE: f100928.2L, forced clear memory. should be don't declare non-pod fields in this ranges.
    ZeroMemory(pod_fields_start_, static_cast<int>(pod_fields_end_ - pod_fields_start_));
    /*
    // removed by f100928.2L
    delete_checked_ = 0;
    delete_date_[0] = '\0';

    db_last_updated_tick_ = 0;
    login_time_ = 0;                // 캐릭터별 로그인 시간
    play_time_after_last_updated_ = 0;
    event_last_updated_time_ = 0;           // 마지막 업데이트한 시각(이벤트용)
    event_use_time_ = 0;
    event_use_time_normal_ = 0;   //__NA000000_090506_EVENT_TYPE_MODIFICATION__

    state_total_info_.m_Count = 0;

    ZeroMemory(&state_gm_info_, sizeof(state_gm_info_));

    inventory_flag_stream_ = 0;
    temp_password_ = NULL;

    log_counter_ = 0;
    */
    /*
    // removed by f100928.2L
    accumulated_exp_ = 0;
    group_high_member_ = NULL;
    */
    // __KR_001355_20090608_GROUPINGSYSTEM_DATA
    // NOTE: I think so good without a value, the zero value is neat style.
    state_high_member_ = GROUP_HAVE_NONE;
    state_low_member_ = GROUP_HAVE_NONE;
    extra_info_.queried_ = false;
    extra_info_.modified_ = false;
#ifdef _NA_006372_20130108_BADGE_ITEM
    extra_info_.toggle_item_info_.init();
#endif //_NA_006372_20130108_BADGE_ITEM
    extra_info_.daily_quest_reset_item_use_time = 0;

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    user_extra_info_.queried_ = false;
    user_extra_info_.modified_ = false;
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    user_extra_info_.attendance_infos.init(0, 0, 
        eATTENDANCE_SUPERPOSITION, eATTENDANCE_INFO_NOT_ATTEND);
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#endif // _NA_007932_20150106_USER_EXTRA_INFO
}

void
DBCharacter::OnInit(DBUser* db_user)
{
    Clear();

    db_user_ = db_user; 
    db_query_node_.Init(db_user, this);
    is_released_ = false;
}

void
DBCharacter::OnRelease()
{
    Clear();
    is_released_ = true;
}

void
DBCharacter::OnConnect()
{
    // 캐릭터별 로그인 시간
    util::TimeSync::time(&login_time_);

    play_time_after_last_updated_ = login_time_;
    // 마지막 업데이트한 시각(이벤트용)
    event_last_updated_time_ = login_time_;

    // 캐릭터가 접속해 있을때만 업데이트
    db_last_updated_tick_ = GetTickCount();
    // (NOTE) declare character database data query processor at this point.
    // (f100731.3L) ... temporary point
    nsQuery::DBQueryProxyManager* const query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    query_proxy_manager->Request_Char_Data_All(db_user_, this);
}

void
DBCharacter::OnDisconnect()
{
    login_time_ = 0;
    play_time_after_last_updated_ = 0;
    db_last_updated_tick_ = 0;
}

int
DBCharacter::GetEventUseTime() const
{   // (WAVERIX) (090513) move from header to code file
    int eventUseTime = db_user_->IsPCRoom() ? event_use_time_ : event_use_time_normal_;
    return eventUseTime;
}

void
DBCharacter::OnDelete()
{
    delete_checked_ = 3;
    DWORD64 current_date=0;
    //"YYYYMMDDHHMM"
    util::GetDateTime_YYYYMMDDHHMM(current_date);
    //"2006-10-23 10:00"
    util::GetSQLDateTime_YYYYMMDDHHMM(current_date, delete_date_);
}

void
DBCharacter::OnRecover()
{
    delete_checked_ = 0;
    delete_date_[0] = '\0';
}

void
DBCharacter::SendAllDataToGameServer()
{
    // @history
    //  - (f100527.4L) (WAVERIX)
    //  - f100826.1L, waverix
	//_NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT
    const size_t kMaxBufferSize = sizeof(MSG_DG_CHARINFO_CHAR_ITEM_CMD); // 이 멤버함수에서 가장 사이즈가 큰 패킷
    static uint8_t busy_access_buffer_[16000];
    BOOST_STATIC_ASSERT(sizeof(busy_access_buffer_) > kMaxBufferSize);  // busy_access_buffer_는 패킷보다 사이즈가 커야 한다.
    // f101021.2L
    //DBUser* const db_user = GetParentDBUser();
    //const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = this->GetCharGuid();
    //
    int total = 0; // (f100527.3L) change type
    ;{ // setup and send 'MSG_DG_CHARINFO_CHAR_ITEM_CMD'
        MSG_DG_CHARINFO_CHAR_ITEM_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_CHARINFO_CHAR_ITEM_CMD;
        SerializeCharInfo(msg_cmd.m_CharacterInfo, SERIALIZE_LOAD);
        //
        // (f100527.4L)
        _ITEM_TOTAL_INFO* const item_block = &msg_cmd.m_ItemInfo;
        item_block->Clear();
        //
        total = 0; // (f100527.3L) change type
        SerializeItemStreamLoad(SI_EQUIPMENT, \
                                &item_block->m_Slot[total], &item_block->m_EquipCount);
        //
        total += item_block->m_EquipCount; // change offset
        SerializeItemStreamLoad(SI_INVENTORY, \
                                &item_block->m_Slot[total], &item_block->m_InvenCount);
        //
        total += item_block->m_InvenCount;
        // CHANGES: f100928.2L, remove a tempinventory store routine
        //
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };
    ;{
        //MSG_DG_CHARINFO_SKILL_LIST_CMD msg_cmd;
    };
    ;{
        MSG_DG_CHARINFO_SKILL_PERK_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_CHARINFO_SKILL_PERK_CMD;
        msg_cmd.char_guid_ = char_guid;
        this->db_query_node_.LoadPerk(&msg_cmd.container_);
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };
    ;{
        //---------------------------------------------
        // DG_CHARINFO_SKILL_CMD
        //---------------------------------------------
        MSG_DG_CHARINFO_SKILL_CMD& msg_cmd = *new (busy_access_buffer_) MSG_DG_CHARINFO_SKILL_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.m_ChannelID = 0;
        db_query_node_.LoadSkill(&msg_cmd.m_SkillInfo);
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    ;{
        MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD& msg_cmd = *new (busy_access_buffer_) MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.m_ChannelID = 0;
        SkillCoolTimeStorage::Instance()->LoadSkillCoolTime(char_guid, &(msg_cmd.skill_cool_time));
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };
#endif

    ;{
        //---------------------------------------------
        // DG_CHARINFO_QUICK_CMD
        //--------------------------------------------
        MSG_DG_CHARINFO_QUICK_CMD& msg_cmd = *new (busy_access_buffer_) MSG_DG_CHARINFO_QUICK_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.m_ChannelID = 0;
        db_query_node_.LoadQuick(&msg_cmd.m_QuickInfo);
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };
    ;{
        //---------------------------------------------
        // DG_CHARINFO_QUEST_CMD
        //---------------------------------------------
        MSG_DG_CHARINFO_QUEST_CMD& msg_cmd = *new (busy_access_buffer_) MSG_DG_CHARINFO_QUEST_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.m_ChannelID = 0;
        db_query_node_.LoadQuest(&msg_cmd);
        db_query_node_.LoadMission(&msg_cmd);
      #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
        SerializeACStream(msg_cmd.m_pACStream, MAX_ACSTREAM_SIZE, SERIALIZE_LOAD);
      #endif
        db_user_->SendPacketToGameServer(&msg_cmd, sizeof(msg_cmd));
    };
    ;{
        MSG_DG_CHARINFO_EXTRA_CMD& msg_cmd = *new (busy_access_buffer_) MSG_DG_CHARINFO_EXTRA_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.m_ChannelID = 0;
        msg_cmd.m_LoginTime_t = 0; //< 현재 사용 안함

        SerializeStateInfo(msg_cmd.m_TotalInfo, msg_cmd.m_GMStateInfo, SERIALIZE_LOAD);

        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };
    ;{
        //printf("SendAll %08X %08X\n", db_user_->WarehouseFlagStream(), InventoryFlagStream());
        MSG_DG_WAREHOUSE_FLAG_STREAM_CMD msg_cmd_warehouse;
        msg_cmd_warehouse.m_dwFlagStream = db_user_->WarehouseFlagStream();
        msg_cmd_warehouse.m_EncodedSSN.CopyFrom(db_user_->SSN.GetBUFFER());
        db_user_->SendPacketToGameServer(&msg_cmd_warehouse, sizeof(msg_cmd_warehouse));

        MSG_DG_ITEM_INVENTORY_FLAG_STREAM_CMD msg_cmd_inventory;
        msg_cmd_inventory.m_dwFlagStream = InventoryFlagStream();
        db_user_->SendPacketToGameServer(&msg_cmd_inventory, sizeof(msg_cmd_inventory));
    };
    ;{
    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        MSG_DG_OTHER_FATIGUE_CMD msg_cmd;
        msg_cmd.m_wPlayTime = db_user_->GetPlayTime();
        msg_cmd.m_wOfflineTime = db_user_->GetOfflineTime();
        db_user_->SendPacketToGameServer(&msg_cmd, sizeof(msg_cmd));
    #endif //
    };

    // NOTE, change to a late sequence order
    // because this information has low priority related to a character initialization
    ;{
    #ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
        MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.m_ChannelID = 0;
        msg_cmd.m_RepurchasItem.m_Count = 0;
        SerializeRepurchasItemInfo(msg_cmd.m_RepurchasItem, SERIALIZE_LOAD);
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    #endif //
    };
    ;{  //_KR_0_20091021_FIRENDINVITATION_EXTEND
        MSG_DG_CHARINFO_INVITATEDFRIEND_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_CHARINFO_INVITATEDFRIEND_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.channel_id_ = 0;
        SerializeFriendListInfo(msg_cmd.friend_list_, SERIALIZE_LOAD);
        db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
    };

	;{
	#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		//_NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT
        static MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD ac_buffer;
		MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD& msg_cmd = *new (&ac_buffer) MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD;
        msg_cmd.char_guid_ = char_guid;
        msg_cmd.channel_id_ = 0;
		SerializeAchievementInfo(msg_cmd.achievement_info_, SERIALIZE_LOAD);
		db_user_->SendPacketToGameServer(&msg_cmd, msg_cmd.GetSize());
	#endif //
	};

    // (NOTE) (f100422.5L) (WAVERIX) moved to last send position, \
    // because this message has client event
    ;{
        MSG_DG_EVENT_USETIME_CMD msg_cmd;
        msg_cmd.m_EventUseTime = GetEventUseTime();
        db_user_->SendPacketToGameServer(&msg_cmd, sizeof(msg_cmd));
    };	
    ;{
        MSG_DG_CHARACTER_EXTRA_INFO_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_CHARACTER_EXTRA_INFO_CMD;
#ifdef _NA_006372_20130108_BADGE_ITEM
        memcpy(&msg_cmd.toggle_item_info_, &extra_info_.toggle_item_info_, \
            sizeof(extra_info_.toggle_item_info_));
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        memcpy(&msg_cmd.battle_ground_record_infos_, &extra_info_.battle_ground_record_infos_, \
                sizeof(extra_info_.battle_ground_record_infos_));
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        memcpy(&msg_cmd.goldrush_record_infos_, &extra_info_.goldrush_record_infos_, \
            sizeof(extra_info_.goldrush_record_infos_));
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
        msg_cmd.daily_quest_reset_item_use_time = extra_info_.daily_quest_reset_item_use_time;
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        msg_cmd.minigame_bingo_infos = extra_info_.minigame_bingo_infos;
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
        db_user_->SendPacketToGameServer(&msg_cmd, sizeof(msg_cmd));
    };
#ifdef _NA_007086_20140318_MONSTERBOOK
    ;{
        MSG_DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD;
        memcpy(&msg_cmd.monster_book, &monster_book_, sizeof(monster_book_));
        db_user_->SendPacketToGameServer(&msg_cmd, sizeof(msg_cmd));
    };
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    ;{
        MSG_DG_USER_EXTRA_INFO_CMD& msg_cmd = \
            *new (busy_access_buffer_) MSG_DG_USER_EXTRA_INFO_CMD;
        // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        memcpy(&msg_cmd.attendance_infos, &user_extra_info_.attendance_infos,
            sizeof(user_extra_info_.attendance_infos));
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        db_user_->SendPacketToGameServer(&msg_cmd, sizeof(msg_cmd));
    };
#endif // _NA_007932_20150106_USER_EXTRA_INFO
}

CHAR*
DBCharacter::GetInventoryPassword()
{
    if (GetInfo()) {
        return GetInfo()->m_InventoryPwd;
    }
    return NULL;
}

void
DBCharacter::SetInventoryPassword(CHAR psPassword[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH])
{
    if (GetInfo()) {
        strncpy(GetInfo()->m_InventoryPwd, psPassword, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH);
    }
}

//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
// update all inventory insert rule configures
void DBCharacter::update_inventory_config(const nsSlot::InventoryConfig& configure)
{
    inventory_config_ = configure;
    for (nsSlot::InventoryConfig::SetType* it = inventory_config_.configures;
         it != &inventory_config_.configures[_countof(inventory_config_.configures)];
         ++it)
    {
        *it = (*it) & configure.eInsertRule_All;
    }
}
// update separated inventory insert rule configure
bool DBCharacter::update_inventory_config_tab(int tab_index,
                                              const nsSlot::InventoryConfig::SetType set_value)
{
    if (!FlowControl::FCAssert(tab_index < _countof(inventory_config_.configures))) {
        return false;
    }
    nsSlot::InventoryConfig::SetType* slot = &inventory_config_.configures[tab_index];
    *slot = set_value & inventory_config_.eInsertRule_All;
    return true;
}


bool
DBCharacter::AlarmInvalidState(TCHAR* pszPath)
{
    // 같은 캐릭터의 로그를 4번 이상 남기지는 않는다.
    if (log_counter_ >= 4) {
        return FALSE;
    }
    BOOLEAN bValidState = player_info_->m_UserGuid &&
        player_info_->m_CharGuid &&
        player_info_->m_sStrength &&
        player_info_->m_sDexterity &&
        player_info_->m_sVitality &&
        player_info_->m_sInteligence &&
        player_info_->m_sSpirit;

    if (bValidState == 0)
    {
        DWORD eDBState = eDB_STATE_MAX;
        if (db_user_)
            eDBState = db_user_->GetDBState();

        SUNLOG(eCRITICAL_LOG,
            "[AlarmInvalidState] Path[%s] UserDBState[%d] "
            "CharName[%s] UserGuid[%d] CharGuid[%d] "
            "Stat[%d/%d/%d/%d/%d]",
            pszPath, eDBState,
            player_info_->m_tszCharName, player_info_->m_UserGuid, player_info_->m_CharGuid,
            player_info_->m_sStrength, player_info_->m_sDexterity, player_info_->m_sVitality,
            player_info_->m_sInteligence, player_info_->m_sSpirit);

        ++log_counter_;

        return TRUE;
    }

    return FALSE;
}

CTime DBCharacter::GetDailyQuestResetTime()
{
    return extra_info_.daily_quest_reset_item_use_time;
}

void DBCharacter::SetDailyQuestResetTime( CTime _daily_quest_reset_item_use_time )
{
    extra_info_.daily_quest_reset_item_use_time = _daily_quest_reset_item_use_time;
}

void DBCharacter::SetUserExtraInfoQuery( bool queried )
{
	user_extra_info_.queried_ = queried;
}

bool DBCharacter::GetUserExtraInfoQuery()
{
	return user_extra_info_.queried_;
}

void DBCharacter::SetUserExtraInfoModify( bool modified )
{
	user_extra_info_.modified_ = modified;
}

bool DBCharacter::GetUserExtraInfoModify()
{
	return user_extra_info_.modified_;
}
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
AttendanceInfo& DBCharacter::GetAttendanceInfos()
{
	return user_extra_info_.attendance_infos;
}
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
