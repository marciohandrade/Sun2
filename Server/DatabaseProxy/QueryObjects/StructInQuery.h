#ifndef __STRUCT_IN_QUERY_H__
#define __STRUCT_IN_QUERY_H__
#pragma once

#include <QueryResult.h>
#include <SafeMemoryPoolFactory.h>

#include <SCSlotHeader.h>
#include <QuestDefine.h>
#include "QueryStringBuffer.h"


//==================================================================================================
// (NOTE) ...
template<class QueryT>
struct DBResultConvert
{
    //TODO : 가변인자 가능하도록 해야함
    static const int ErrorCode[]; 
    int GetGuildResult(int result_by_db) {
        return QueryT::ErrorCode[result_by_db];
    }
};

class QueryForUser : public QueryResult
{
public:
    void SetUserKey(DWORD user_guid) {
        user_guid_ = user_guid;
    }
    DWORD GetUserKey() const {
        return user_guid_;
    }
    void SetProtocol(BYTE c, BYTE p) {
        SetIndex(MAKEDWORD(WORD(c), WORD(p)));
    }

private: 
    DWORD user_guid_;
};

class QueryForCharTransaction : public QueryForUser
{
public:
    void set_char_guid(DWORD char_guid) { char_guid_ = char_guid; };
    DWORD char_guid() const { return char_guid_; }

    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const = 0;

private:
    DWORD char_guid_;
    //
};

//==================================================================================================
// r101007.3L
struct sQUERY_CHARACTER
{
    USERGUID  m_UserGuid;
    short     m_bySlot;  // uint8_t
    short     m_byClass; // uint8_t
    CHARGUID  m_CharGuid;
    //
    short     m_byHeight; // uint8_t
    short     m_byFace;   // uint8_t
    short     m_byHair;   // uint8_t
    //
    short     m_LV; // LEVELTYPE(WORD)
    //
    short     m_sStrength;
    short     m_sDexterity;
    short     m_sVitality;
    short     m_sInteligence;
    short     m_sSpirit;
    //
    short     m_sSkillStat1;
    short     m_sSkillStat2;
    int       m_UserPoint; // UPTYPE(int)
    //
    int64     m_i64Exp; // EXPTYPE(__int64)
    //
    int       m_iMaxHP;
    int       m_iHP;
    int       m_iMaxMP;
    int       m_iMP;
    //
    int64     m_Money; // MONEY(ULONGLONG)
    //
    int       m_iRemainStat;
    int       m_iRemainSkill;
    // NOTE: DB에서는 int로 가진다 : 스타일 코드가 6만번부터 시작하기 때문에 사용할때는 WORD로 캐스팅해서 사용한다.
    int       m_iSelectStyleCode;
    //
    short     m_byPKState; // uint8_t:3bit
    short     m_byCharState; // uint8_t:7bit
    int64     m_StateTime;
    //
    int       m_iRegion;
    short     m_sX;
    short     m_sY;
    short     m_sZ;
    int       m_iPreRegion;
    //
    int64     m_TitleTime;
    //
    short     m_byInvisibleOpt; // uint8_t
    //
    uint8_t   m_bInventoryLock;
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
    BYTE      m_pACStream[MAX_ACSTREAM_SIZE];
#endif
    //
    short     m_iCHuntingDisCount;
    short     m_iCHuntingVCount;
    short     m_iCHuntingLCount;
    short     m_iCHuntingTotCount;
    //
    short     m_byChaoState; // uint8_t 'ChaoGrade'
    int64     m_i64ChaoTime;
    //
    int       m_iPlayLimitedTime;
    //----------------------------------------------------------------------------------------------
    // NOTE: reference table = { T_PVP }
    int       m_iPVPPoint;
    int       m_iPVPScore;
    short     m_byPVPGrade; // uint8_t
    int       m_iPVPDraw;
    int       m_iPVPSeries;
    int       m_iPVPKill;
    int       m_iPVPDie;
    int       m_iPVPMaxKill;
    int       m_iPVPMaxDie;
    //----------------------------------------------------------------------------------------------
    // NOTE: reference table = { T_GuildInfo, T_GuildMember }
    GUILDGUID m_GuildGuid;
    short     m_eGuildDuty; // uint8_t eGUILD_DUTY
    //----------------------------------------------------------------------------------------------
    // NOTE: reference table = { T_CharUseTime }
    int       m_EventUseTime; // accumulated value of playing time in pc room
    // NOTE: reference table = { T_CharUseTime_General }
    int       m_EventUseTimeNormal; //__NA000000_090506_EVENT_TYPE_MODIFICATION__
    //
    short     m_byDelChk; // uint8_t
    //
    short     m_byExtraInventoryTabCount; // uint8_t
    //
    short     m_ExtraEquipCashSlotCount; // uint8_t
    //
    uint8_t   m_bTradeFlag;
    uint8_t   m_bWhisperFlag;
    short     m_byFreeInitCount; // uint8_t
    short     m_ChangeOfClassStep;
    //
    uint8_t   m_bSummonedPet;
    uint8_t   m_bMAXIntimacy;
    short     m_PetItemPos; // POSTYPE
    int       m_bMAXOptionRemainTime; // ?
    //
    uint8_t   m_bActiveEther;    //__NA_1165_ETHER_DEVICE_SAVE_DB
    short     m_byEtherBulletPos; // POSTYPE
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
    uint8_t   m_iACVersion; //__NA_1288_AC_RENEWAL_FOR_TIME_LINE
#endif
    //{__NA_001290_20090525_SHIELD_SYSTEM
    int       m_iMaxSD;
    int       m_iSD;
    //}
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    short     m_wAccumulateYear;  // WORD:  플레이 시간 적립 년도.
    int       m_dwAccumulateTime; // DWORD: 적립 시간 (값:0000000초)
    short     m_wAccumulatePoint; // WORD:  적립 포인트
#endif
    //
#ifdef _NA_002050_20110216_ADD_GENDER
    uint8_t gender; // 성별
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	short		honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
    
    //_NA_004035_20120227_COSTUME_ITEM
    uint8_t is_equipped_costume; // 코스튬 활성화 여부
    short costume_item_pos; // 코스튬 인벤토리 위치
    int costume_item_code; // 코스튬 아이템 번호
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    uint8_t is_equipped_deco_costume; // 데코 코스튬 활성화 여부
    short deco_costume_item_pos; // 데코 코스튬 인벤토리 위치
    int deco_costume_item_code; // 데코 코스튬 아이템 번호
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME


    //{_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    uint8_t   config_version; // tiny int
    short     config01; // smallint
    short     config02; // smallint
    short     config03; // smallint
    short     config04; // smallint
    short     config05; // smallint
    short     config06; // smallint
    short     config07; // smallint
    short     config08; // smallint
    short     config09; // smallint
    short     config10; // smallint
    //}

#pragma pack(push, 1)
#pragma warning(push)
#pragma warning(disable: 4201)
    union {
        struct {
            char      param_info_char_name_[MAX_CHARNAME_LENGTH];
            char      param_info_char_name_null_terminate_[1];
            char      param_info_title_id_[MAX_TITLEID_LENGTH];
            char      param_info_title_id_null_terminate_[1];
            char      param_info_guild_name_[MAX_GUILDNAME_LENGTH];
            char      param_info_guild_name_null_terminate_[1];
            char      param_info_guild_nick_name_[14];
            char      param_info_guild_nick_name_null_terminate_[1];
            char      param_info_create_time_[MAX_TIMEDATA_SIZE];
            char      param_info_create_time_null_terminate_[1];
            char      param_info_del_date_[MAX_TIMEDATA_SIZE];
            char      param_info_del_date_null_terminate_[1];
            char      param_info_password_[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH];
            char      param_info_password_null_terminate_[1];
        };
        struct {
            char      result_info_char_name_[MAX_CHARNAME_LENGTH + 1];
            char      result_info_title_id_[MAX_TITLEID_LENGTH + 1];
            char      result_info_guild_name_[MAX_GUILDNAME_LENGTH + 1];
            char      result_info_guild_nick_name_[14 + 1];
            char      result_info_create_time_[MAX_TIMEDATA_SIZE + 1];
            char      result_info_del_date_[MAX_TIMEDATA_SIZE + 1];
            char      result_info_password_[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH + 1];
        };
    };
#pragma warning(pop)
#pragma pack(pop)
    //----------------------------------------------------------------------------------------------
    // CHANGES: f101007.4L, add forced boundary checker
    void SetSafeBound()
    {
        assert(param_info_char_name_ == result_info_char_name_ &&
               param_info_title_id_ == result_info_title_id_ &&
               param_info_guild_name_ == result_info_guild_name_ &&
               param_info_guild_nick_name_ == result_info_guild_nick_name_ &&
               param_info_create_time_ == result_info_create_time_ &&
               param_info_del_date_ == result_info_del_date_ &&
               param_info_password_ == result_info_password_);

        param_info_char_name_null_terminate_[0] = 0;
        param_info_title_id_null_terminate_[0] = 0;
        param_info_guild_name_null_terminate_[0] = 0;
        param_info_guild_nick_name_null_terminate_[0] = 0;
        param_info_create_time_null_terminate_[0] = 0;
        param_info_del_date_null_terminate_[0] = 0;
        param_info_password_null_terminate_[0] = 0;        
    }
    /*
    // CHANGES: remove boundary modifier
    inline void SetSafeBound()
    {
        m_tszCharName[_countof(m_tszCharName) - 1]          = _T('\0');
        m_tszTitleID[_countof(m_tszTitleID) - 1]            = _T('\0');
        m_tszGuildName[_countof(m_tszGuildName) - 1]        = _T('\0');
        m_tszGuildNickName[_countof(m_tszGuildNickName) - 1] = _T('\0');
        m_tszCreateTime[_countof(m_tszCreateTime) - 1]      = _T('\0');
        m_tszDelDate[_countof(m_tszDelDate) - 1]            = _T('\0');
    }
    */

    // (WAVERIX) (090513) (CHANGES) 버퍼 초기화 관련
    // (CHANGES) (f100414.1L) removed because of use the pool is supported cleared stream node
    //inline void Clear()
    //{ // no virtual,
    //    ZeroMemory(this, sizeof(*this));
    //}
};

//==================================================================================================
// (CHANGES) (NOTE) changes by __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
// |target=m_AbilityIndex|reference=SkillOption in the GameServer/Skill_Info.h|
struct sQUERY_CHARSTATE
{
    CHARGUID m_CharGuid;
    SHORT    m_StateCode;    // 상태코드
    WORD     m_SkillCode;    // 스킬코드
    BYTE     m_AbilityIndex; // 어빌리티 인덱스
    int      m_RemainTime;   // 남은시간
    SHORT    m_Interval;     // 주기
};


//==================================================================================================
//= namespace nsQuery ==============================================================================
//==================================================================================================

namespace nsQuery {
;
#pragma warning(push)
#pragma warning(disable:4201)

//==================================================================================================
struct PerkResultRecord
{
    int group_code_;
    short level_;
    short link_code_;
    int64 exp_;
};

struct ItemSlotResultRecord
{
    // reference = SI_INVENTORY, SI_EQUIPMENT, reserve = { SI_WAREHOUSE, SI_GUILDWAREHOUSE }
    // read sequence = { slot_type_, pos_, serial_, code_, dura_or_num_, version_, option_stream_ }
    //
    uint8_t slot_type_;
    uint8_t version_;
    short pos_;   // POSTYPE
    short dura_or_num_; // DURATYPE
    int code_;
    __int64 serial_;
    uint8_t option_stream_[sizeof(OPTIONPART)];
};

struct SlotResultRecord
{
    static const int kMaxNumberOfSlots = 8;
    //
    short slot_index_; // reference : SI_SKILL, SI_QUICK, and that.
    short tab_order_;
    union {
        int slot_array_[kMaxNumberOfSlots];
        struct {
            int slot01_;
            int slot02_;
            int slot03_;
            int slot04_;
            int slot05_;
            int slot06_;
            int slot07_;
            int slot08_;
        };
    };
};

struct QuestResultRecord
{
    int quest_version_;
    BYTE quest_stream_progress_[MAX_PROGR_QUESTSTREAM_SIZE];
    BYTE quest_stream_complete_[MAX_COMPL_QUESTSTREAM_SIZE];
};

struct BattleRecordsResultRecord
{
    // PVP
    int pvp_point_;
    int pvp_score_;
    short pvp_grade_; // uint8_t
    int pvp_max_series_;
    int pvp_max_kills_;
    int pvp_max_die_;
    int pvp_number_of_kills_;
    int pvp_number_of_dead_;
    int pvp_number_of_draws_;
    // 경쟁헌팅 전적
    int hunting_number_of_fails_; //< 경쟁헌팅 Dis 횟수
    int hunting_number_of_victories_; //< 경쟁헌팅 승리횟수
    int hunting_number_of_defeats_; //< 경쟁헌팅 패배횟수
    int hunting_number_of_joins_; //< 경쟁헌팅 참여횟수
};

// reference: STATE_DETAIL_INFO
struct CharStateResultRecord
{
    short state_code_;
    int skill_code_;
    // bit spec. idx|option
    //           000|00000
    short ability_index_with_option_; // uint8_t
    int remained_time_; // seconds, infinite time = '-1'
    short interval_;
};

struct GuildWarehouseResultRecord
{
    int64_t money_;
    //uint8_t number_of_slot_tabs_;
};
//

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
struct CharAchievementResultRecord : public Character_Achievement
{
};

// CHANGES: f101112.5L, change query data types,
// you should be use long or short type if a sql data type is smallint on push c-types
struct CharAchievementInputParam //: public CharAchievement_Info
{
    short index_number_;
    uint8_t is_reward_;
    short object_value_[MAX_ACHIEVEMENT_OBJECT];
};
#endif

struct MissionResultRecord
{
    short code; // mission code ranges = (0, 255]
    short state;
    short point;
    short hidden; // not used, reserved field
};

#pragma warning(pop)
}; //end of namespace

//==================================================================================================
//= end of namespace nsQuery =======================================================================
//==================================================================================================


#endif // __STRUCT_IN_QUERY_H__
