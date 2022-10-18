#pragma once
#ifndef DBPROXY_DBCHARARACTER_H
#define DBPROXY_DBCHARARACTER_H

//==================================================================================================
/// DBCharacter class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >   
    @since
        2006. 6. 26~
    @description
        - DBUser에서 분리 : 리팩토링
    @history
        - 09.05.13|waverix|코드정리
        - 10.04.16|waverix|fit in coding style
        - 10.04.18|waverix|reorder data fields based on member size and access rates
*/
//==================================================================================================

#include <QuestDefine.h>
#include <SCStream.h>
#include <SCSlotHeader.h>


#include "QueryObjects/StructInQuery.h"
#include "ProxyNodes/DBQueryProxyNode.h"

//==================================================================================================

// (f100804.6L) remove DB_SI_~ enumeration
// because of ambigous enumeration type has caused an invalid usage
/*
enum
{
    DB_SI_TEMPINVENTORY = 0,
    DB_SI_INVENTORY,
    DB_SI_EQUIPMENT,
    DB_SI_SKILL,
    DB_SI_QUICK,
    DB_SI_STYLE,
    DB_SI_MAX,
};
*/

class DBUser;
class WayPoint;
class CharWayPoint;
class Query_Char_Update;
class Query_Char_Update_Inventory;
class Query_Warehouse_Update;
class GuildWarehouseInfo;
class CollectedList;
struct SERVER_CHARACTER_PART;

//==================================================================================================

class DBCharacter
{
    enum {
        _DBP_UPDATE_CYCLE_DELAY = 1000 * 60 * 5, //< 임시 : 5분 딜레이
    };
public:
    DBCharacter(void);
    virtual	~DBCharacter(void);

    BASE_PLAYERINFO* GetInfo() const;
    void SetInfo(BASE_PLAYERINFO* player_info);
    CHARGUID GetCharGuid() const;
    const TCHAR* GetCharName() const;
    void SetCharName(char* char_name);
    LEVELTYPE GetLV() const;
    BYTE GetClassCode() const;
    // (f100730.4L) export field...
    DBUser* GetParentDBUser() const;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN      GetGameChuUSN() const;
    VOID            SetGameChuUSN(GAMECHUUSN gamechu_usn);
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY         GetBetaKey() const;
    VOID            SetBetakey(BETAKEY betakey);
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
#endif
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	inline DBCharacter_Achievements* GetAchievementInfo();
	void SerializeAchievementInfo(Character_Achievements& achievement_info, eSERIALIZE eType);
#endif
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    void SerializeRepurchasItemInfo(REPURCHASITEM_TOTAL_INFO& RepurchasItems, eSERIALIZE eType);
#endif //
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    void SerializeFriendListInfo(CHAR_INVITATEDFRIEND_INFOs& Invitatedfriend_count,
                                 eSERIALIZE serialize_type);
    void SerializeCharInfo(BASE_PLAYERINFO& rCharInfo, eSERIALIZE eType);
    void SerializeMissionStream(BYTE* pMissionStream, WORD wSize, eSERIALIZE se);
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
    void SerializeACStream(BYTE* pStream, WORD wSize, eSERIALIZE se);
#endif

    void SerializeStateInfo(STATE_DETAIL_TOTAL_INFO& INOUT rStateTotalInfo,
                            STATE_GM_INFO& INOUT rStateGMInfo, eSERIALIZE se);

    // CHANGES, f100817.2L, split a composed SerializeItemStream method to it of load and it of store
    void SerializeItemStreamLoad(SLOTIDX slot_index,
                                 ITEMSLOTEX_INSERVER* INOUT slot_array,
                                 POSTYPE* result_number_of_slots);
    void SerializeItemStreamStore(SLOTIDX slot_index,
                                  const ITEMSLOTEX_INSERVER* slot_array, POSTYPE number_of_slots);
    
    void SetExtraInfoQueried(bool queried) { extra_info_.queried_ = queried; };
    bool GetExtraInfoQueried() { return extra_info_.queried_; };
    void SetExtraInfoModified(bool modified) { extra_info_.modified_ = modified; };
    bool GetExtraInfoModified() { return extra_info_.modified_; };
#ifdef _NA_006372_20130108_BADGE_ITEM
    ToggleItemInfos& GetToggleItemInfo() { return extra_info_.toggle_item_info_; };
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundRecordInfos& GetBattleGroundRecordInfos() { return extra_info_.battle_ground_record_infos_; }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    GoldRushRecordInfos& GetGoldRushRecordInfos() { return extra_info_.goldrush_record_infos_; }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    CTime GetDailyQuestResetTime();
    void  SetDailyQuestResetTime(CTime _daily_quest_reset_item_use_time);
#ifdef _NA_007086_20140318_MONSTERBOOK
    MonsterBook& GetMonsterBook() { return monster_book_; };
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    MiniGameBingoInfos& GetMiniGameBingoInfos() { return extra_info_.minigame_bingo_infos; }
#endif //_NA_008404_20150712_MINIGAME_SYSTEM

    // 디비에서 캐릭터 정보를 가져 왔을 때
    void OnInit(DBUser* db_user);
    void OnRelease();

    void OnConnect();
    void OnDisconnect();

    void Updatea();
    //void UpdateByGuildWarehouse(GuildWarehouseInfo*);
    void ReleaseGuildWarehouse();
    void BindGuildWarehouse(GuildWarehouseInfo* p);
    GuildWarehouseInfo* GetGuildWarehouse() const;

    void Serialize(SERVER_CHARACTER_PART& part, eSERIALIZE se);
    bool OnSetInfo(const sQUERY_CHARACTER* queried_char_info);
    // (CHANGES) (f100324.1L) (WAVERIX) add interface for inventory stream alignment function \
    // detached from 'OnSetInfo'.
    // (WARNING) don't call directly on the current version (100324)
    bool OnSetInfoInventory(const sQUERY_CHARACTER* queried_char_info);
    // f100928.2L, this routine support a nested data synchronization on all loaded data
    bool SynchronizeDataBeforeSendCharInfo(SERVER_CHARACTER_PART* const player_part);
    bool UpdateCharInfo();
    bool UpdateInventory();
    bool UpdateSubData(); // skill & perk, ...
    //
    void MakeQuery_Char_Update(Query_Char_Update* query);
    void MakeQuery_Char_Update_Inventory(Query_Char_Update_Inventory* query);
    void SendAllDataToGameServer();

    WayPoint* GetWayPoint() const;
    CharWayPoint* GetCharWayPoint() const;
    CollectedList* GetCollectedList() const;
    int GetEventUseTime() const;
    void   SetLoginTime(time_t t);
    time_t GetLoginTime() const;
    void OnDelete();
    void OnRecover();
    bool IsDeleted() const;

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    void SetReNameStatus(const int rename_status);
    int GetReNameStatus() const;
#endif

    DWORD&  InventoryFlagStream();
    char*   GetInventoryPassword();
    void    SetInventoryPassword(char psPassword[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH]);
    inline sPASSWORD_BLOCK_IN_DBPROXY* GetTempInventoryPassword() const;
    inline void SetTempInventoryPassword(sPASSWORD_BLOCK_IN_DBPROXY* temp_password_block);

    bool AlarmInvalidState(TCHAR* pszPath);	// 캐릭터 상태가 이상한 경우(수치이상) 로그를 남긴다.
    // __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
    EXPTYPE     GetAccumEXP() const;
    void        SetAccumEXP(EXPTYPE AccumEXP);
    EXPTYPE     AddAccumEXP(EXPTYPE AccumEXP);
    CHARGUID    GetHighMemberGuid() const;
    void        SetHighMemberGuid(CHARGUID GroupHighMember);
    eGROUP_STAT GetStatHighMember() const;
    void        SetStatHighMember(eGROUP_STAT eStatHighMember);
    eGROUP_STAT GetStatLowMember() const;
    void        SetStatLowMember(eGROUP_STAT eStatLowMember);
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
    // (CHANGES) (f100324.3L) default added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
    const nsSlot::InventoryConfig& inventory_config() const;
    // update all inventory insert rule configures
    void update_inventory_config(const nsSlot::InventoryConfig& configure);
    // update separated inventory insert rule configure
    bool update_inventory_config_tab(int tab_index, const nsSlot::InventoryConfig::SetType set_value);
    nsQuery::DBQueryProxyCharNode* db_query_node();
    const nsQuery::DBQueryProxyCharNode* db_query_node() const;

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    BYTE* GetACRawStream() const;
#endif
    bool IsReleased() const
    {
        return is_released_;
    }

private:
    void Clear();

    //----------------------------------------------------------------------------------------------
    DBUser* db_user_; // (CHANGES) (f100324.2L) change to first member
    BASE_PLAYERINFO* player_info_;
    // (NOTE) types of SI_WAREHOUSE and SI_GUILDWAREHOUSE don't use in a db character
    WayPoint* way_point_;
    CharWayPoint* char_way_point_;
    CollectedList* collected_list_;
    nsQuery::DBQueryProxyCharNode db_query_node_;
    GuildWarehouseInfo* my_guild_warehouse_;
    bool is_released_;

    //----------------------------------------------------------------------------------------------
    // NOTE: f100928.2L, the below block is processed by operation of fill zero memory.
    // WARNING: should be don't declare non-pod fields in this ranges.
    char pod_fields_start_[1];
    BYTE    delete_checked_;
    TCHAR   delete_date_[MAX_SMALLDATETIME_SIZE+1];
    //
    // Playtime (in PCroom or Normal) 타임관련 맴버
    DWORD   db_last_updated_tick_; // DB 업데이트 처리
    time_t  login_time_;           // 캐릭터별 로그인 시간 
    time_t  play_time_after_last_updated_; // 캐릭터별 마지막 플레이 시간(매 업데이트 시마다 초기화)
    time_t  event_last_updated_time_; // 마지막 업데이트한 시각(이벤트용)
    int     event_use_time_;
    int     event_use_time_normal_; //__NA000000_090506_EVENT_TYPE_MODIFICATION__
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    int     rename_status_; //1: 캐릭터명 변경 가능, 1이외의값: 변경 불가능
#endif
    //
    // (NOTE) related to inventory control
    DWORD inventory_flag_stream_;
    sPASSWORD_BLOCK_IN_DBPROXY* temp_password_;
    //
    // __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
    EXPTYPE  accumulated_exp_;   //그룹 시스템에서 경험치를 쌓는 역할을 하는 변수
    CHARGUID group_high_member_; //그룹 맺기 시스템에 경험치를 쌓기 위해 저장되어야 하는 그룹 상위 멤버 Guid
    eGROUP_STAT state_high_member_; //그룹의 상위 멤버 상태
    eGROUP_STAT state_low_member_;  //그룹의 하위 멤버 상태
    // };
    int log_counter_;
    //
    // (CHANGES) (f100324.3L) default added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
    nsSlot::InventoryConfig inventory_config_;
    //
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	//typedef STLX_HASH_MAP<WORD, AchievementPart>		 ACHIEVEMENT_PART_MAP;    
	DBCharacter_Achievements	achievement_info_;	
#endif
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    REPURCHASITEM_TOTAL_INFO repurchase_items_;
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    CHAR_INVITATEDFRIEND_INFOs Invitatedfriend_count_;
    //
    BYTE mission_stream_[MAX_MISSIONSTREAM_SIZE];
    BYTE ac_stream_[MAX_ACSTREAM_SIZE];
    STATE_GM_INFO state_gm_info_;

    struct
    {
#ifdef _NA_006372_20130108_BADGE_ITEM
        ToggleItemInfos toggle_item_info_;
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        BattleGroundRecordInfos battle_ground_record_infos_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        GoldRushRecordInfos goldrush_record_infos_;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
        CTime daily_quest_reset_item_use_time;
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        MiniGameBingoInfos minigame_bingo_infos;
#endif //_NA_008404_20150712_MINIGAME_SYSTEM
        bool queried_;
        bool modified_;
    } extra_info_;
#ifdef _NA_007086_20140318_MONSTERBOOK
    MonsterBook monster_book_;
#endif //_NA_007086_20140318_MONSTERBOOK
    //

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
public:
    void SetUserExtraInfoQuery(bool queried);
    bool GetUserExtraInfoQuery();
    void SetUserExtraInfoModify(bool modified);
    bool GetUserExtraInfoModify();
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceInfo& GetAttendanceInfos();
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

private:
    struct USER_EXTRA_INFO
    {
        // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        AttendanceInfo attendance_infos;
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        bool queried_;
        bool modified_;
    } user_extra_info_;
#endif // _NA_007932_20150106_USER_EXTRA_INFO
    char pod_fields_end_[1];// f100928.2L
};

//==================================================================================================
//  implementation in a header

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
inline GAMECHUUSN DBCharacter::GetGameChuUSN() const {
    return player_info_->gamechu_usn_;
};

inline VOID DBCharacter::SetGameChuUSN(GAMECHUUSN gamechu_usn) {
    player_info_->gamechu_usn_ = gamechu_usn;
};
#endif

#ifdef _NA_20100307_BETAKEY_SYSTEM
inline BETAKEY DBCharacter::GetBetaKey() const {
    return player_info_->beta_key_;
};

inline VOID DBCharacter::SetBetakey(BETAKEY betakey) {
    player_info_->beta_key_ = betakey;
};
#endif

// (f100730.4L) export field...
inline DBUser* DBCharacter::GetParentDBUser() const {
    if (impl::IsValidUser(db_user_, __FUNCTION__, __LINE__) == false)
    {
        return NULL;
    }
    return db_user_;
}

inline BASE_PLAYERINFO* DBCharacter::GetInfo() const {
    return player_info_;
}

inline void DBCharacter::SetInfo(BASE_PLAYERINFO* player_info) {
    *player_info_ = *player_info;
}

inline CHARGUID DBCharacter::GetCharGuid() const {
    return player_info_->m_CharGuid;
}

inline const TCHAR* DBCharacter::GetCharName() const {
    return player_info_->m_tszCharName;
}

inline void DBCharacter::SetCharName(char* char_name) {
    strncpy(player_info_->m_tszCharName, char_name, MAX_CHARNAME_LENGTH+1);
    player_info_->m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
}

inline LEVELTYPE DBCharacter::GetLV() const {
    return player_info_->m_LV;
}

inline BYTE DBCharacter::GetClassCode() const {
    return player_info_->m_byClassCode;
}

inline void
DBCharacter::SetLoginTime(time_t t)
{
    login_time_ = t;
}

inline time_t
DBCharacter::GetLoginTime() const
{
    return login_time_;
}

inline void DBCharacter::ReleaseGuildWarehouse() {
    my_guild_warehouse_ = NULL;
}

inline void DBCharacter::BindGuildWarehouse(GuildWarehouseInfo* p) {
    my_guild_warehouse_ = p;
}

inline GuildWarehouseInfo* DBCharacter::GetGuildWarehouse() const {
    return my_guild_warehouse_;
}

inline WayPoint* DBCharacter::GetWayPoint() const {
    return way_point_;
}

inline CharWayPoint* DBCharacter::GetCharWayPoint() const {
    return char_way_point_;
}

inline CollectedList* DBCharacter::GetCollectedList() const {
    return collected_list_;
}

inline bool DBCharacter::IsDeleted() const {
    return delete_checked_ == 3;
}

inline DWORD&  DBCharacter::InventoryFlagStream() {
    return inventory_flag_stream_;
}

inline sPASSWORD_BLOCK_IN_DBPROXY* DBCharacter::GetTempInventoryPassword() const {
    return temp_password_;
}

inline void DBCharacter::SetTempInventoryPassword(sPASSWORD_BLOCK_IN_DBPROXY* temp_password_block) {
    temp_password_ = temp_password_block;
}

inline EXPTYPE DBCharacter::GetAccumEXP() const {
    return min(INT_MAX, accumulated_exp_);
}

inline void DBCharacter::SetAccumEXP(EXPTYPE AccumEXP) {
    accumulated_exp_ = AccumEXP;
}

inline EXPTYPE DBCharacter::AddAccumEXP(EXPTYPE AccumEXP) {
    return accumulated_exp_ = accumulated_exp_ + AccumEXP;
}

inline CHARGUID DBCharacter::GetHighMemberGuid() const {
    return group_high_member_;
}

inline void DBCharacter::SetHighMemberGuid(CHARGUID GroupHighMember) {
    group_high_member_ = GroupHighMember;
}

inline eGROUP_STAT DBCharacter::GetStatHighMember() const {
    return state_high_member_;
}

inline void DBCharacter::SetStatHighMember(eGROUP_STAT eStatHighMember) {
    state_high_member_ = eStatHighMember;
}

inline eGROUP_STAT DBCharacter::GetStatLowMember() const {
    return state_low_member_;
}

inline void DBCharacter::SetStatLowMember(eGROUP_STAT eStatLowMember) {
    state_low_member_ = eStatLowMember;
}

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
inline BYTE* DBCharacter::GetACRawStream() const {
    return const_cast<BYTE*>(ac_stream_);
}
#endif
//////////////////////////////////////////////////////////////////////////

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
inline DBCharacter_Achievements* 
DBCharacter::GetAchievementInfo() 
{
	return &achievement_info_;
}
#endif

// (CHANGES) (f100324.3L) default added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
inline const nsSlot::InventoryConfig& DBCharacter::inventory_config() const
{
    return inventory_config_;
}

//--------------------------------------------------------------------------------------------------

inline nsQuery::DBQueryProxyCharNode*
DBCharacter::db_query_node()
{
    return &db_query_node_;
}

inline const nsQuery::DBQueryProxyCharNode*
DBCharacter::db_query_node() const
{
    return &db_query_node_;
}

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
inline void 
DBCharacter::SetReNameStatus(const int rename_status)
{
    rename_status_ = rename_status;
}

inline int 
DBCharacter::GetReNameStatus() const
{
    return rename_status_;
}
#endif

#endif //DBPROXY_DBCHARARACTER_H

