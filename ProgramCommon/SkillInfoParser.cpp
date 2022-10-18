#include "stdafx.h"
#include "SkillInfoParser.h"

#include <Define.h>
#include <Struct.h>
#include <ItemOptionHeader.h>

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
#include <SkillInventoryStoreParser.h>
#endif


SkillInfoParser::SkillInfoParser() : 
    script_infos_(), 
    script_infos_iterator(script_infos_.begin()), 
    script_infos_const_iterator(script_infos_.begin()), 
    style_infos_(), 
    style_infos_iterator(style_infos_.begin()), 
    style_infos_const_iterator(style_infos_.begin())
{
}

SkillInfoParser::~SkillInfoParser()
{
}

bool SkillInfoParser::_LoadStyle(const bool is_reload)
{
    if (is_reload)
    {
        style_infos_.clear();
    }

#pragma pack(push, 1)
    struct {
        char temp_buffer[32];
        int temp_padding;
    } temp_buffer_block;
#pragma pack(pop)
    typedef char temp_buffer_t[32];
    ZeroMemory(&temp_buffer_block, sizeof(temp_buffer_block));
    temp_buffer_t& temp_buffer = temp_buffer_block.temp_buffer;

    const int max_rows = GetRowSize();
    for (int row = 0; row < max_rows; ++row)
    {
        const int style_code = GetDataInt("StyleCode", row);
        if (style_code <= 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|[%s]|Msg=index(%d) out of ranges|"),
                __FUNCTION__, 
                style_code
            );
            ASSERT(false);
            return false;
        }

        if (GetStyleInfo(style_code) != NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|[%s]|Msg=duplicated index(%d)|"),
                __FUNCTION__, 
                style_code
            );
            ASSERT(false);
            return false;
        }
        
        BASE_STYLEINFO style_info;
        style_info.m_SkillCode = static_cast<SLOTCODE>(style_code);
        style_info.m_byType = ROOT_SKILLINFO::STYLE_TYPE;
        
        style_info.m_SkillClassCode = GetDataWORD("StyleClassCode", row);
        strncpy(
            style_info.m_szStyleName, 
            GetDataString("StyleName", row), 
            _countof(style_info.m_szStyleName)
        );
        style_info.m_szStyleName[_countof(style_info.m_szStyleName) - 1] = '\0';
        style_info.m_dwStyleNCode    = GetDataDWORD("StyleNCode", row);
        style_info.m_dwStyleDescCode = GetDataDWORD("StyleDescCode", row);
        style_info.m_wSkillattribute = GetDataWORD("SkillAttribute", row);
        style_info.m_dwStyleImage    = GetDataDWORD("StyleImage", row);
        style_info.m_wRequireLV = GetDataWORD("RequireLV", row);
        style_info.m_wStyleLV   = GetDataWORD("StyleLV", row);
        style_info.m_wMaxLV     = GetDataWORD("MaxLV", row);
        style_info.m_wOverLV    = GetDataWORD("OverLV", row);

        for (int i = 0; i < _countof(style_info.m_wOverStatclass); ++i)
        {
            _snprintf(temp_buffer, _countof(temp_buffer), "OverStat%dClass", i+1);
            style_info.m_wOverStatclass[i]  = GetDataWORD(temp_buffer, row);
            _snprintf(temp_buffer, _countof(temp_buffer), "OverStat%d", i+1);
            style_info.m_wOverstat[i] = GetDataWORD(temp_buffer, row);
        }

        style_info.m_byRequireSkillPoint = GetDataBYTE("RequireSkillPoint", row);
        style_info.m_byClassDefine = GetDataBYTE("ClassDefine", row);
        style_info.m_iWeaponDefine = GetDataInt("WeaponDefine", row);
        // NOTE: f110304.6L, currently, the 'AttType' column is not used.
        style_info.m_byAttType     = GetDataBYTE("AttType", row);
        style_info.m_fAttRange     = GetDataFloat("AttRange", row);
        style_info.m_byMoveAttack  = GetDataBYTE("MoveAttack", row);
        style_info.m_byStyleCheck  = GetDataBYTE("StyleCheck", row);

        strncpy(style_info.m_wzidCodeFirst.code, GetDataString("CodeFirst", row), 4);
        strncpy(style_info.m_wzidCodeSecond.code, GetDataString("CodeSecond", row), 4);
        strncpy(style_info.m_wzidCodeThird.code, GetDataString("CodeThird", row), 4);
        strncpy(style_info.m_wzidCodeBack.code, GetDataString("CodeBack", row), 4);

        style_info.m_wTimeFirst  = GetDataWORD("TimeFirst", row);
        style_info.m_wTimeSecond = GetDataWORD("TimeSecond", row);
        style_info.m_wTimeThird  = GetDataWORD("TimeThird", row);
        
        for (int i = 0; i < _countof(style_info.m_iAddDamage); ++i)
        {
            BOOST_STATIC_ASSERT(
                _countof(style_info.m_iAddDamage) == _countof(style_info.m_fDamagePercent));
            _snprintf(temp_buffer, _countof(temp_buffer), "AddDamage%d", i+1);
            style_info.m_iAddDamage[i] = GetDataInt(temp_buffer, row);
            _snprintf(temp_buffer, _countof(temp_buffer), "DamagePercent%d", i+1);
            style_info.m_fDamagePercent[i]  = GetDataFloat(temp_buffer, row);
        }

        style_info.m_byAttRangeform   = GetDataBYTE("AttRangeform", row);
        style_info.m_fStyleArea       = GetDataFloat("StyleArea", row);
        style_info.m_fThirdDelay      = GetDataFloat("ThirdDelay", row);
        style_info.m_iAttackRate      = GetDataInt("AttackRate", row);
        style_info.m_iAvoidRate       = GetDataInt("AvoidRate", row);
        style_info.m_iAttackSpeed     = GetDataInt("AttSpeedIncrease", row);
        style_info.m_iBonusDefence    = GetDataInt("DefenceAdd", row);
        style_info.m_iMagicDefenceAdd = GetDataInt("MagicDefenceAdd", row); //마법방어력 증가 814 이거 어디갔냐?(우선 임시로 멤버변수 만들어 넣었다.)
        style_info.m_wCriticalBonus   = GetDataWORD("CriticalBonus", row);
        style_info.m_fDefenseIgnore   = GetDataFloat("DefenceIgnore", row);
        style_info.m_fPierceRate      = GetDataFloat("PierceRate", row);
        style_info.m_fPierceRange     = GetDataFloat("PierceRange", row);
        style_info.m_fStunRate        = GetDataFloat("StunRate", row);
        style_info.m_wStunTime        = GetDataWORD("StunTime", row);
        style_info.m_fKnockBackRate   = GetDataFloat("KnockBackRate", row);
        style_info.m_fKnockBackRange  = GetDataFloat("KnockBackRange", row);
        style_info.m_fDownRate        = GetDataFloat("DownRate", row);
        style_info.m_fDelayReduce     = GetDataFloat("DelayReduce", row);
        style_info.m_fDelayOccur      = GetDataFloat("DelayOccur", row);

        style_info.m_wHPAbsorb    = GetDataWORD("HPAbsorb", row);
        style_info.m_fHPAbsorbPer = GetDataFloat("HPAbsorbPer", row);
        style_info.m_wMPAbsorb    = GetDataWORD("MPAbsorb", row);
        style_info.m_fMPAbsorbPer = GetDataFloat("MPAbsorbPer", row);
        style_info.m_wMaxTargetNo = GetDataWORD("MaxTargetNo", row);

        style_infos_[style_code] = style_info;
    }
    
    if (temp_buffer_block.temp_padding != 0) 
    {
        FASSERT((temp_buffer_block.temp_padding == 0) && "|Msg=occurred buffer overrun!|");
    }
    return true;
}

namespace util { 
namespace internal {

struct AbilityNameTable
{
    AbilityNameTable()
    {
        ZeroMemory(this, sizeof(*this));
    }

    typedef char buffer_t[32];
    buffer_t id_columns[MAX_ABILITY_NUM];
    buffer_t range_columns[MAX_ABILITY_NUM];
    buffer_t rates_columns[MAX_ABILITY_NUM];
    buffer_t option1_columns[MAX_ABILITY_NUM];
    buffer_t option2_columns[MAX_ABILITY_NUM];
    typedef buffer_t parameters_t[BASE_ABILITYINFO::MAX_ABILITY_PARAM_NUM];
    parameters_t parameter_columns[MAX_ABILITY_NUM];
    buffer_t state_id_columns[MAX_ABILITY_NUM];
    
    void BuildString()
    {
        ZeroMemory(this, sizeof(*this));
        //
        for (int i = 0; i < MAX_ABILITY_NUM; ++i) 
        {
            _snprintf(id_columns[i], _countof(id_columns[i]), "AbilityID%d", i + 1);
        }
        for (int i = 0; i < MAX_ABILITY_NUM; ++i) 
        {
            _snprintf(range_columns[i], _countof(range_columns[i]), "Ability%dRangeType", i + 1);
        }
        for (int i = 0; i < MAX_ABILITY_NUM; ++i) 
        {
            _snprintf(rates_columns[i], _countof(rates_columns[i]), "Ability%dSuccessRate", i + 1);
        }
        for (int i = 0; i < MAX_ABILITY_NUM; ++i) 
        {
            _snprintf(option1_columns[i], _countof(option1_columns[i]), "Ability%dOption1", i + 1);
        }
        for (int i = 0; i < MAX_ABILITY_NUM; ++i) 
        {
            _snprintf(option2_columns[i], _countof(option2_columns[i]), "Ability%dOption2", i + 1);
        }
        for (int i = 0; i < MAX_ABILITY_NUM; ++i)
        {
            parameters_t& params = parameter_columns[i];
            for (int param_index = 0; 
                param_index < BASE_ABILITYINFO::MAX_ABILITY_PARAM_NUM; ++param_index)
            {
                _snprintf(
                    params[param_index], 
                    _countof(params[param_index]), 
                    "Ability%dParameter%d", 
                    i + 1, 
                    param_index + 1
                );
            }
        }
        for (int i = 0; i < MAX_ABILITY_NUM; ++i) 
        {
            _snprintf(
                state_id_columns[i], _countof(state_id_columns[i]), "Ability%dParameter5", i + 1);
        }
    }
};

} // internal
} // util

bool SkillInfoParser::_Load(const bool is_reload)
{
    if (is_reload)
    {
        script_infos_.clear();
    }

    util::internal::AbilityNameTable name_table;
    name_table.BuildString();
#pragma pack(push, 1)
    struct {
        char temp_buffer[32];
        int temp_padding;
    } temp_buffer_block;
#pragma pack(pop)
    typedef char temp_buffer_t[32];
    ZeroMemory(&temp_buffer_block, sizeof(temp_buffer_block));
    temp_buffer_t& temp_buffer = temp_buffer_block.temp_buffer;

    const int max_rows = GetRowSize();
    for (int row = 0; row < max_rows; ++row)
    {
        const int skill_code = GetDataInt("SkillCode", row);
        if (skill_code <= 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|스킬 코드 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code
            );
            FASSERT(!"스킬 코드 범위가 올바르지 않습니다.");
            return false;
        }
        
        if (GetSkillInfo(skill_code) != NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|같은 스킬 코드가 이미 존재 합니다.|Row = %d, SkillCode = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code
            );
            FASSERT(!"같은 스킬 코드가 이미 존재 합니다.");
            return false;
        }
        
        const WORD skill_class_code = GetDataWORD("SkillClassCode", row);
        if (skill_class_code == 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|스킬 클래스 코드 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code
            );
            FASSERT(!"스킬 클래스 코드 범위가 올바르지 않습니다.");
            return false;
        }

        SkillScriptInfo script_info;
        script_info.m_SkillCode = static_cast<SLOTCODE>(skill_code);
        script_info.m_byType = ROOT_SKILLINFO::SKILL_TYPE;
        
        script_info.m_SkillClassCode = skill_class_code;
        strncpy(
            script_info.m_szSkillName, 
            GetDataString("SkillName", row), 
            _countof(script_info.m_szSkillName)
        );
        script_info.m_szSkillName[_countof(script_info.m_szSkillName) - 1] = '\0';
        script_info.m_dwSkillNcode = GetDataDWORD("SkillNcode", row);
        script_info.m_dwSkillDesccode = GetDataDWORD("SkillDescCode", row);
        script_info.m_dwSkillIconcode = GetDataDWORD("SkillIconCode", row);
        strncpy(
            script_info.m_szSkillMasterEffectName, 
            GetDataString("SkillEffectName", row),
            _countof(script_info.m_szSkillMasterEffectName)
        );
        script_info.m_szSkillMasterEffectName[_countof(script_info.m_szSkillMasterEffectName) - 1] = '\0';

    #ifdef __NA001126_NEW_SKILL_SCREEN_EFFECT
        script_info.m_bySwayType = GetDataBYTE("SwayType", row);
        script_info.m_dwSwayTime = GetDataDWORD("SwayTime", row);
        script_info.m_bySwayNum = GetDataBYTE("SwayNum", row);
        script_info.m_dwSwaySize = GetDataDWORD("SwaySize", row);
    #endif //__NA001126_NEW_SKILL_SCREEN_EFFECT

        for (int i = 0; i < _countof(script_info.m_iWeaponDefine); ++i)
        {
            BOOST_STATIC_ASSERT(
                _countof(script_info.m_iWeaponDefine) == _countof(script_info.m_wzidAnimationCode));
            _snprintf(temp_buffer, _countof(temp_buffer), "WeaponDefine%d", i+1);
            script_info.m_iWeaponDefine[i] = GetDataInt(temp_buffer, row);
            _snprintf(temp_buffer, _countof(temp_buffer), "AnimationCode%d", i+1);
            strncpy(script_info.m_wzidAnimationCode[i].code, GetDataString(temp_buffer, row), 4);
        }

        script_info.m_CSSyncDelay = GetDataWORD("CSSyncDelay", row);
        strncpy(script_info.m_wzidCastAniCode.code, GetDataString("CastAniCode", row), 4);
        script_info.m_dwFlyingObjectCode = GetDataDWORD("FlyingObjectCode", row);
        script_info.m_wFlyingLifeTime = GetDataWORD("FlyingLifeTime", row);

        strncpy(script_info.m_wzidFieldEffectCode.code, GetDataString("FieldEffect", row), 4);
        script_info.m_dwItemCode = GetDataWORD("Effect", row);          // 효과 이펙트인데? 왠 아이템 코드 뭘까?
        // gs_답변:스킬효과시발동되는이펙트는 사실상 아이템스크립트의
        //         이펙트코드를 참조하기때문에 요코드를써준것임. 헷갈릴만함
        script_info.m_wSkillAttribute = GetDataWORD("SkillAttribute", row);
        script_info.m_wRequireLv = GetDataWORD("RequireLV", row);
        script_info.m_wSkillLv   = GetDataWORD("SkillLV", row);
        script_info.m_wMaxLv     = GetDataWORD("MaxLV", row);
        script_info.m_wOverLv    = GetDataWORD("OverLV", row);

        for (int i = 0; i < _countof(script_info.m_wOverStatclass); ++i)
        {
            BOOST_STATIC_ASSERT(
                _countof(script_info.m_wOverStatclass) == _countof(script_info.m_wOverstat));
            _snprintf(temp_buffer, _countof(temp_buffer), "OverStat%dClass", i+1);
            script_info.m_wOverStatclass[i]  = GetDataWORD(temp_buffer, row);
            _snprintf(temp_buffer, _countof(temp_buffer), "OverStat%d", i+1);
            script_info.m_wOverstat[i] = GetDataWORD(temp_buffer, row);
        }

        script_info.m_bySkillType       = GetDataBYTE("SkillType", row);
        script_info.m_byEtherBulletInfo = GetDataBYTE("EtherBulletInfo", row);
        script_info.m_bySkillUserType   = GetDataBYTE("SkillUserType", row);
        script_info.m_dwClassDefine     = GetDataDWORD("ClassDefine", row);
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
        script_info.m_UpperSkillClass   = GetDataWORD("SkillStatType", row);
        script_info.m_bySkillStatType   = 0;
#else
        script_info.m_bySkillStatType   = GetDataBYTE("SkillStatType", row);
#endif

        for (int i = 0; i < _countof(script_info.m_wRequireSkillStat); ++i)
        {
            _snprintf(temp_buffer, _countof(temp_buffer), "RequireSkillStat%d", i+1);
            script_info.m_wRequireSkillStat[i] = GetDataWORD(temp_buffer, row);
        }

        //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
        script_info.m_RequireUpperSkillLevel  = script_info.m_wRequireSkillStat[0];
        script_info.m_byRequireSkillPoint = GetDataBYTE("RequireSkillPoint", row);
        script_info.m_byTarget          = GetDataBYTE("Target", row);
        script_info.m_byForbiddenTarget = GetDataBYTE("ForbiddenTarget", row);
        script_info.m_wHPSpend       = GetDataWORD("HPSpend", row);
        script_info.m_wMPSpend       = GetDataWORD("MPSpend", row);
        script_info.m_wSkillCasting  = GetDataWORD("SkillCasting", row);
        script_info.m_dwCoolTime     = GetDataDWORD("CoolTime", row);
        script_info.m_wSkillRange    = GetDataWORD("SkillRange", row);
        script_info.m_byAttRangeform = GetDataBYTE("AttRangeform", row);
        script_info.m_wSkillArea     = GetDataWORD("SkillArea", row);
        script_info.m_wChaseRange    = GetDataWORD("ChaseRange", row);
        script_info.m_byMaxTargetNo  = GetDataBYTE("MaxTargetNo", row);
#ifdef _YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212
        script_info.m_btAutoAttack   = GetDataBYTE("AutoAttack", row);
#endif //_YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212
        
        // load abilities
        BASE_ABILITYINFO ability_info;
        for (int i = 0; i < MAX_ABILITY_NUM; ++i)
        {
            const int ability_id = GetDataInt(name_table.id_columns[i], row);
            if (ability_id == eABILITY_INVALID)
            {
                break;
            }
            
            if (ability_id >= eABILITY_MAX)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|어빌리티 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_id
                    );
                //ASSERT(false);
                continue;
            }
            
            ability_info.m_byIndex = static_cast<uint8_t>(i);
            ability_info.m_wAbilityID = static_cast<WORD>(ability_id);
            ability_info.m_byRangeType  = GetDataBYTE(name_table.range_columns[i], row);
            ability_info.m_wSuccessRate = GetDataWORD(name_table.rates_columns[i], row);
            ability_info.m_iOption1 = GetDataInt(name_table.option1_columns[i], row);
            ability_info.m_iOption2 = GetDataInt(name_table.option2_columns[i], row);
            
            for (int j = 0; j < BASE_ABILITYINFO::MAX_ABILITY_PARAM_NUM; ++j) 
            {
                ability_info.m_iParam[j] = GetDataInt(name_table.parameter_columns[i][j], row);
            }

            ability_info.m_wStateID = GetDataWORD(name_table.state_id_columns[i], row);
            // CHANGES: f110309.1L
            if (ability_info.iff_filter.ConfigureIFF_Check(
                &ability_info.iff_filter, ability_info) == false)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|어빌리티 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_id
                );
                ASSERT(false);
            }
            script_info.AddAbility(ability_info);
        }
        script_info.m_bySkillAcquire = GetDataBYTE("SkillAcquire", row);

        // 유효성을 검증한다.
        if (CheckValidity(&script_info, row) == false)
        {
            // 유효하지 않아도 진행 하는데 문제가 많다. 수정하고 넘어가자.
            //ASSERT(false);
            //return false;
        }
        script_infos_[skill_code] = script_info;
        
        COOLTIME_MAP::iterator it = skill_cooltime_table_.find(script_info.m_SkillCode);
        if (it == skill_cooltime_table_.end())
        {
            bool cool_time_add = false;
            if (script_info.m_bySkillUserType != eSKILL_USER_EMOTICON)
            {
                if (script_info.m_bySkillUserType == eSKILL_USER_ACTION)
                {
                    if (script_info.m_dwCoolTime != 0)
                    {
                        cool_time_add = true;
                    }
                }
                else
                {
                    cool_time_add = true;
                }
            }
            if (cool_time_add)
            {
                SKILL_COOLTIME_INFO cool_time_info;
                cool_time_info.m_SkillCode = script_info.m_SkillCode;
                cool_time_info.m_dwCoolTime = script_info.m_dwCoolTime;
                skill_cooltime_table_[script_info.m_SkillCode] = cool_time_info;
            }
        }
        else
        {
            it->second.m_SkillCode = script_info.m_SkillCode;
            it->second.m_dwCoolTime = script_info.m_dwCoolTime;
        }
    }

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    CalculateSkillLineFromSkillInventoryStoreParser();
    job_tree_manager_.LoadTree(this);
#endif

    return true;

}

//==================================================================================================
//==================================================================================================
//  IFF_Filter implements
// NOTE: f110309.1L, the IFF_Filter object support target select filtering in the ability
// this is implemented by _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
//==================================================================================================
//==================================================================================================
#if SUN_CODE_BACKUP
bool IFF_Filter::ConfigureIFF_Check(IFF_Filter* result_iff, const eSKILL_TARGET_TYPE target_type)
{
    BOOST_STATIC_ASSERT(SKILL_TARGET_MAX < 32);
    if (!(target_type != SKILL_TARGET_NONE && target_type < SKILL_TARGET_MAX))
    {
        result_iff->validated = false;
        return false;
    };
    //
    const IFF_Filter::value_type target_type_bit = (1 << target_type);
    IFF_Filter::value_type select_filter = 0;

    if (target_type_bit & eIFF_RelationCheckAttackerOnly) {
        select_filter = eIFF_AttackerOnly | eIFF_One;
    }
    else if (target_type_bit & eIFF_RelationCheckFriend) {
        select_filter = eIFF_Friend;
    }
    else if (target_type_bit & eIFF_RelationCheckEnemy) {
        select_filter = eIFF_Enemy;
    }
    else if (target_type_bit & eIFF_RelationCheckFriendCorpse) {
        select_filter = eIFF_Friend | eIFF_Corpse;
    }
    else {
        // no check about other cases
    };
    //
    result_iff->value = select_filter;
    result_iff->validated = true;
    //
    return true;
}
#endif

bool IFF_Filter::ConfigureIFF_Check(IFF_Filter* result_iff, const BASE_ABILITYINFO& ability_info)
{
    const eABILITY_RANGE_TYPE range_type =
        static_cast<eABILITY_RANGE_TYPE>(ability_info.m_byRangeType);
    BOOST_STATIC_ASSERT(SKILL_ABILITY_MAX < 32);
    if (!FlowControl::FCAssert(ability_info.m_wAbilityID != 0 &&
                               range_type != SKILL_ABILITY_NONE &&
                               range_type < SKILL_ABILITY_MAX))
    {
        result_iff->validated = false;
        return false;
    };
    //
    const IFF_Filter::value_type range_type_bit = (1 << range_type);
    IFF_Filter::value_type select_filter = 0;

    if (range_type_bit & eIFF_AbilityConditionActorOnly) {
        select_filter |= eIFF_AttackerOnly;
    };
    if (range_type_bit & eIFF_AbilityConditionMainTargetOnly) {
        select_filter = eIFF_AttackerOnly | eIFF_MainTargetOnly;
    };
    if (range_type_bit & eIFF_AbilityConditionCorpseAbilities) {
        select_filter |= eIFF_Corpse;
    };
    if (range_type_bit & eIFF_AbilityConditionEnabledResurrection) {
        select_filter |= eIFF_Resurrection;
    };
    if (range_type_bit & eIFF_AbilityConditionPlayer) {
        select_filter |= eIFF_Player;
    };
    if (range_type_bit & eIFF_AbilityConditionSkipAbilities) {
        select_filter |= eIFF_CheckSkip;
    };
    if (range_type_bit & eIFF_AbilityConditionFriendAbilities) {
        select_filter |= eIFF_Friend;
    };
    if (range_type_bit & eIFF_AbilityConditionEnemyAbilities) {
        select_filter |= eIFF_Enemy;
    };

    result_iff->value = select_filter;
    result_iff->validated = true;
    //
    return true;
}

//==================================================================================================
//==================================================================================================
//  BASE_ABILITYINFO implements
//==================================================================================================
//==================================================================================================
// CHANGES: f110214.3L, added debugging facility
struct BASE_ABILITYINFO::DetailLogging
{
    DetailLogging(bool* input_valid_check)
        : valid_check(true)
        , valid_check_reference(input_valid_check)
    {
    }

    ~DetailLogging()
    {
        if (valid_check_reference) {
            *valid_check_reference = valid_check;
        }
    }
    bool valid_check;
    bool* valid_check_reference;
};

eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeDetail(bool* valid_check) const
{
    // CHANGES: f110214.3L, added debugging facility
    DetailLogging logger(valid_check);
    // CHANGES: f1101121.1L, change style
    const eABILITY ability_type = static_cast<eABILITY>(m_wAbilityID);

    switch (ability_type)
    {
    //----------------------------------------------------------------------------------------------
    case eABILITY_MAX_HP_INCREASE:
        return eATTR_MAX_HP;
    case eABILITY_CUR_HP_INCREASE:
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    case eABILITY_VARIATION_PUREVALUE_HP:
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
        return eATTR_CUR_HP;
    case eABILITY_RECOVER_HP_INCREASE:
        return eATTR_RECOVERY_HP;
    //----------------------------------------------------------------------------------------------
    case eABILITY_MAX_MP_INCREASE:
        return eATTR_MAX_MP;
    case eABILITY_CUR_MP_INCREASE:
        return eATTR_CUR_MP;
    case eABILITY_RECOVER_MP_INCREASE:
        return eATTR_RECOVERY_MP;
    //----------------------------------------------------------------------------------------------
    case eABILITY_PHYSICAL_ATTACKPOWER_INCREASE:
    case eABILITY_MAGIC_ATTACKPOWER_INCREASE:
        return GetAttrTypeForAttackpower(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
    case eABILITY_PHYSICAL_DEFENSE_INCREASE:
        return GetAttrTypeForDefence(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    case eABILITY_MAGIC_DEFENSE_INCREASE:
        return GetAttrTypeForDefence(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
    case eABILITY_ATTACKPOWER_BY_ARMORTYPE:
        return GetAttrTypeForBonusDamage((eARMOR_TYPE)m_iOption1, m_iParam[0], &logger);
    case eABILITY_DEFENSE_BY_ATTACKTYPE:
        return GetAttrTypeForBonusDefence(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
#endif
    //----------------------------------------------------------------------------------------------
    case eABILITY_STAT_INCREASE:
        return GetAttrTypeForStat((eSTAT_TYPE)m_iOption1, &logger);
    //----------------------------------------------------------------------------------------------
    case eABILITY_PHYSICAL_ATTACKRATE_INCREASE:
        return eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO;
    case eABILITY_PHYSICAL_AVOIDRATE_INCREASE:
        return eATTR_PHYSICAL_ATTACK_BLOCK_RATIO;

    case eABILITY_MOVE_SPEED_INCREASE:
    case eABILITY_HIDE:
        return eATTR_MOVE_SPEED;
    case eABILITY_PHYSICAL_SPEED_INCREASE:
        return eATTR_ATTACK_SPEED;
    case eABILITY_CASTING_TIME_INCREASE:
        return eATTR_BONUS_CASTING_TIME;

    case eABILITY_DAMAGE_DECREASE:
        return GetAttrTypeForReduceDamage(static_cast<eATTACK_TYPE>(m_iOption1), &logger);

    case eABILITY_SKILLRANGE_INCREASE:
        return GetAttrTypeForAttackRange(static_cast<eRANGE_TYPE>(m_iOption1), &logger);

#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    case eABILITY_CRITICAL_RATIO_CHANGE:
        return GetAttrTypeForCriticalRatio(static_cast<eATTR_APPLY_TYPE>(m_iOption1), &logger);
    case eABILITY_CRITICAL_DAMAGE_CHANGE:
        return GetAttrTypeForCriticalDamage(static_cast<eATTR_APPLY_TYPE>(m_iOption1), &logger);
#else
    case eABILITY_CRITICAL_RATIO_CHANGE:
        return eATTR_ADD_ALL_CRITICAL_RATIO;
    case eABILITY_CRITICAL_DAMAGE_CHANGE:
        return eATTR_CRITICAL_DAMAGE_CHANGE;
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    //----------------------------------------------------------------------------------------------
    case eABILITY_LOWHP_ATTACKPOWER_CHANGE:
        return GetAttrTypeForAttackpower(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
    case eABILITY_LOWHP_DEFENSE_CHANGE:
        return GetAttrTypeForDefence(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
    //----------------------------------------------------------------------------------------------
    case eABILITY_SKILL_COOL_TIME_INCREASE:
        return eATTR_BONUS_SKILL_COOL_TIME;
    case eABILITY_MP_SPEND_INCREASE:
        return eATTR_MP_SPEND_INCREASE;
    case eABILITY_SKILLDAMAGE_INCREASE:
        return eATTR_ADD_SKILL_DAMAGE_RATIO;
    //__NA_S00015_20080828_NEW_HOPE_SHADOW
    case eABILITY_ATTR_ATTACK_POWER:
        return GetAttrTypeForAttackpower(static_cast<eATTACK_TYPE>(m_iOption1), &logger);
    case eABILITY_ATTR_DEFENSIVE_POWER:
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        return GetAttrTypeForElementalResist(static_cast<eATTACK_TYPE>(m_iOption1), &logger); 
#else
        return GetAttrTypeForDefence(static_cast<eATTACK_TYPE>(m_iOption1), &logger); 
#endif
    case eABILITY_CHANGE_ATTR:
        if (FlowControl::FCAssert(m_iParam[0] < _countof(g_ITEM_OPTION))) {
            return g_ITEM_OPTION[m_iParam[0]];
        }
        return eATTR_TYPE_INVALID;
    //end of shadow
    case eABILITY_RECOVERY_CHANCE:
        return eATTR_INCREASE_RESERVE_HP;
    //{_NA_001231_20081216_ADD_OPTION_KIND
    case eABILITY_RESIST_HOLDING:
        return eATTR_RESIST_HOLDING;
    case eABILITY_RESIST_SLEEP:
        return eATTR_RESIST_SLEEP;
    case eABILITY_RESIST_POISON:
        return eATTR_RESIST_POISON;
    case eABILITY_RESIST_KNOCKBACK:
        return eATTR_RESIST_KNOCKBACK;
    case eABILITY_RESIST_DOWN:
        return eATTR_RESIST_DOWN;
    case eABILITY_RESIST_STUN:
        return eATTR_RESIST_STUN;
    case eABILITY_DECREASE_PVPDAMAGE:
        return eATTR_DECREASE_PVPDAMAGE;
    case eABILITY_SKILL_STATE_IGNORE: // arycoat 2013.04.25.
        return eATTR_RESISTANCE_BADSTATUS_RATIO;
    //}
    case eABILITY_MAX_SD_INCREASE:
        return eATTR_MAX_SD;
    case eABILITY_RECOVER_SD_INCREASE:
        return eATTR_RECOVERY_SD;
    case eABILITY_CUR_SD_INCREASE:
        return eATTR_CUR_SD;

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case eABILITY_CRAFT_COST_RATIO:
        return eATTR_CRAFT_COST_RATIO;
    case eABILITY_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO:
        return eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO;
    case eABILITY_ENCHANT_COST_RATIO:
        return eATTR_ENCHANT_COST_RATIO;
    case eABILITY_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO:
        return eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO;
    case eABILITY_RECOVER_POTION_COOLTIME_RATIO:
        return eATTR_RECOVER_POTION_COOLTIME_RATIO;
    case eABILITY_RECOVER_POTION_RECOVERY_RATIO:
        return eATTR_RECOVER_POTION_RECOVERY_RATIO;
    case eABILITY_QUEST_REWARD_EXP_RATIO:
        return eATTR_QUEST_REWARD_EXP_RATIO;
    case eABILITY_MAX_DAMAGE_RATIO:
        return eATTR_MAX_DAMAGE_RATIO;
    case eABILITY_MONEY_RAIO:
        return eATTR_BONUS_MONEY_RATIO;
    case eABILITY_DOMINATION_MAPOBJECT_DAMAGE_RATIO:
        return eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO;
    case eABILITY_SHOP_REPAIR_HEIM_RATIO:
        return eATTR_SHOP_REPAIR_HEIM_RATIO;
    case eABILITY_SHOP_BUY_HEIM_RATIO:
        return eATTR_SHOP_BUY_HEIM_RATIO;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case eABILITY_INCREASE_DAMAGE:
        return eATTR_INCREASE_DAMAGE_RATIO;
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    case eABILITY_MONSTER_KILL_EXP_RATIO:
        return eATTR_BONUS_EXP_RATIO;
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    case eABILITY_AWAKENING_PROBABILITY:
        return eATTR_AWAKENING_PROBABILITY;
#endif
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    case eABILITY_ATTACK_DAMAGE_HP_ABSORPTION:
        return eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO;
#endif
    //----------------------------------------------------------------------------------------------
    default:
        return eATTR_TYPE_INVALID;
    };
}

//_NA001956_110210_PERK_SWITCHABLE_REGION
eATTR_EXTRA_OPTION BASE_ABILITYINFO::GetAttrExtraOption() const
{
#if !defined(_NA001956_110210_PERK_SWITCHABLE_REGION)
    return eATTR_EXTRA_OPTION_INVALID;
#endif
#ifdef _NA001956_110210_PERK_SWITCHABLE_REGION
    const eABILITY ability_type = static_cast<eABILITY>(m_wAbilityID);
    switch (ability_type)
    {
    case eABILITY_TOTEM_NPC_OPTION_EMPOWERMENT:
        return eATTR_EXTRA_TOTEM_NPC_OPTION_EMPOWERMENT;
    }
    return eATTR_EXTRA_OPTION_INVALID;
#endif
}



eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForAttackpower(eATTACK_TYPE attackType, DetailLogging* logging) const
{
    // NOTE: f110125.1L, open logic before next build updating
#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    static const eATTR_TYPE converter_static[eATTACK_TYPE_MAX] =
    {
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_INVALID
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MELEE
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_RANGE
        eATTR_MAGICAL_WATER_ATTACK_POWER,   //eATTACK_TYPE_WATER
        eATTR_MAGICAL_FIRE_ATTACK_POWER,    //eATTACK_TYPE_FIRE
        eATTR_MAGICAL_WIND_ATTACK_POWER,    //eATTACK_TYPE_WIND
        eATTR_MAGICAL_EARTH_ATTACK_POWER,   //eATTACK_TYPE_EARTH
        eATTR_MAGICAL_DARKNESS_ATTACK_POWER,//eATTACK_TYPE_DARKNESS
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_DIVINE
        eATTR_OPTION_PHYSICAL_ATTACK_POWER, //eATTACK_TYPE_PHYSICAL_OPTION
        eATTR_MAGICAL_ALL_ATTACK_POWER,     //eATTACK_TYPE_MAGIC_OPTION
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_ALL_MAGIC
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MAGIC
        //
    };

    const bool accept_flag = (((1 << attackType) & eATTACK_TYPE_ABILITY_OPTION_ALL_BITS) != 0);
    const eATTR_TYPE attr_type
        = (accept_flag && _countof(converter_static) > attackType)
        ?   converter_static[attackType]
        :   eATTR_TYPE_INVALID;
    if (attr_type == eATTR_TYPE_INVALID) {
        logging->valid_check = false;
    };
    return attr_type;
#else
    switch(attackType)
    {
    case eATTACK_TYPE_ALL_OPTION:       return eATTR_OPTION_ALL_ATTACK_POWER;
        //  case eATTACK_TYPE_MELEE:            return eATTR_BASE_MELEE_MIN_ATTACK_POWER;
        //  case eATTACK_TYPE_RANGE:            return eATTR_BASE_RANGE_MIN_ATTACK_POWER;
    case eATTACK_TYPE_WATER:            return eATTR_MAGICAL_WATER_ATTACK_POWER;
    case eATTACK_TYPE_FIRE:             return eATTR_MAGICAL_FIRE_ATTACK_POWER;
    case eATTACK_TYPE_WIND:             return eATTR_MAGICAL_WIND_ATTACK_POWER;
    case eATTACK_TYPE_EARTH:            return eATTR_MAGICAL_EARTH_ATTACK_POWER;
    case eATTACK_TYPE_DARKNESS:         return eATTR_MAGICAL_DARKNESS_ATTACK_POWER;
    case eATTACK_TYPE_DIVINE:           return eATTR_MAGICAL_DIVINE_ATTACK_POWER;
    case eATTACK_TYPE_PHYSICAL_OPTION:  return eATTR_OPTION_PHYSICAL_ATTACK_POWER;
    case eATTACK_TYPE_MAGIC_OPTION:     return eATTR_OPTION_MAGICAL_ATTACK_POWER;
    case eATTACK_TYPE_ALL_MAGIC:        return eATTR_MAGICAL_ALL_ATTACK_POWER;
    default:
        {
            //          SUNLOG(eCRITICAL_LOG, "[GetAttrTypeForAttackpower] Invalid AttackType[%d] \n", attackType);
            return eATTR_OPTION_PHYSICAL_ATTACK_POWER;
        }
    }
#endif
}

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForElementalResist(eATTACK_TYPE attack_type, DetailLogging* logging) const
{
    static const eATTR_TYPE converter_static[eATTACK_TYPE_MAX] =
    {
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_INVALID
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_MELEE
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_RANGE
        eATTR_DEL_WATER_DAMAGE,         //eATTACK_TYPE_WATER
        eATTR_DEL_FIRE_DAMAGE,          //eATTACK_TYPE_FIRE
        eATTR_DEL_WIND_DAMAGE,          //eATTACK_TYPE_WIND
        eATTR_DEL_EARTH_DAMAGE,         //eATTACK_TYPE_EARTH
        eATTR_DEL_DARKNESS_DAMAGE,      //eATTACK_TYPE_DARKNESS
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_DIVINE
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_PHYSICAL_OPTION
        eATTR_DEL_MAGICAL_ALL_DAMAGE,   //eATTACK_TYPE_MAGIC_OPTION
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_ALL_MAGIC
        eATTR_TYPE_INVALID,             //eATTACK_TYPE_MAGIC
        //
    };

    const bool accept_flag = (((1 << attack_type) & eATTACK_TYPE_ABILITY_OPTION_ALL_BITS) != 0);
    const eATTR_TYPE attr_type
        = (accept_flag && _countof(converter_static) > attack_type)
        ?   converter_static[attack_type]
        :   eATTR_TYPE_INVALID;
    if (attr_type == eATTR_TYPE_INVALID) {
        logging->valid_check = false;
    };
    return attr_type;
}
#endif

eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForDefence(eATTACK_TYPE attackType, DetailLogging* logging) const
{
#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
/*
    static const eATTR_TYPE converter_static[eATTACK_TYPE_MAX] =
    {
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_INVALID
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MELEE
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_RANGE
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_WATER
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_FIRE
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_WIND
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_EARTH
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_DARKNESS
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_DIVINE
        eATTR_OPTION_PHYSICAL_DEFENSE_POWER,//eATTACK_TYPE_PHYSICAL_OPTION
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MAGIC_OPTION
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_ALL_MAGIC
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MAGIC
        //
    };

    const bool accept_flag = (((1 << attackType) & eATTACK_TYPE_ABILITY_OPTION_PHYSICAL_BITS) != 0);
    const eATTR_TYPE attr_type
        = (accept_flag && _countof(converter_static) <= attackType)
        ?   converter_static[attackType]
        :   eATTR_TYPE_INVALID;
    if (attr_type == eATTR_TYPE_INVALID) {
        assert(!"unexpected ability Option1 used to eATTACK_TYPE");
    };
    return attr_type;
*/
    if (attackType == eATTACK_TYPE_PHYSICAL_OPTION) {
        return eATTR_OPTION_PHYSICAL_DEFENSE_POWER;
    }
    logging->valid_check = false;
    return eATTR_TYPE_INVALID;
#else
    switch(attackType)
    {
    case eATTACK_TYPE_ALL_OPTION:       return eATTR_OPTION_ALL_DEFENSE_POWER;
        //  case eATTACK_TYPE_MELEE:            return eATTR_BASE_MELEE_DEFENSE_POWER;
        //  case eATTACK_TYPE_RANGE:            return eATTR_BASE_RANGE_DEFENSE_POWER;
    case eATTACK_TYPE_WATER:            return eATTR_MAGICAL_WATER_DEFENSE_POWER;
    case eATTACK_TYPE_FIRE:             return eATTR_MAGICAL_FIRE_DEFENSE_POWER;
    case eATTACK_TYPE_WIND:             return eATTR_MAGICAL_WIND_DEFENSE_POWER;
    case eATTACK_TYPE_EARTH:            return eATTR_MAGICAL_EARTH_DEFENSE_POWER;
    case eATTACK_TYPE_DARKNESS:         return eATTR_MAGICAL_DARKNESS_DEFENSE_POWER;
    case eATTACK_TYPE_DIVINE:           return eATTR_MAGICAL_DIVINE_DEFENSE_POWER;
    case eATTACK_TYPE_PHYSICAL_OPTION:  return eATTR_OPTION_PHYSICAL_DEFENSE_POWER;
    case eATTACK_TYPE_MAGIC_OPTION:     return eATTR_OPTION_MAGICAL_DEFENSE_POWER;
    case eATTACK_TYPE_ALL_MAGIC:        return eATTR_MAGICAL_ALL_DEFENSE_POWER;
    default:
        {
            //          SUNLOG(eCRITICAL_LOG, "[GetAttrTypeForDefence] Invalid AttackType[%d] \n", attackType);
            return eATTR_OPTION_PHYSICAL_DEFENSE_POWER;
        }
    }
#endif
}

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForBonusDefence(eATTACK_TYPE attackType, DetailLogging* logging) const
{
    switch(attackType)
    {
    case eATTACK_TYPE_ALL_OPTION:       return eATTR_ADD_ALL_DEFENSE_POWER;
    case eATTACK_TYPE_MELEE:            return eATTR_ADD_MELEE_DEFENSE_POWER;
    case eATTACK_TYPE_RANGE:            return eATTR_ADD_RANGE_DEFENSE_POWER;
    case eATTACK_TYPE_WATER:            return eATTR_ADD_WATER_DEFENSE_POWER;
    case eATTACK_TYPE_FIRE:             return eATTR_ADD_FIRE_DEFENSE_POWER;
    case eATTACK_TYPE_WIND:             return eATTR_ADD_WIND_DEFENSE_POWER;
    case eATTACK_TYPE_EARTH:            return eATTR_ADD_EARTH_DEFENSE_POWER;
    case eATTACK_TYPE_DARKNESS:         return eATTR_ADD_DARKNESS_DEFENSE_POWER;
    case eATTACK_TYPE_DIVINE:           return eATTR_ADD_DIVINE_DEFENSE_POWER;
    case eATTACK_TYPE_PHYSICAL_OPTION:  return eATTR_ADD_PHYSICAL_DEFENSE_POWER;
    case eATTACK_TYPE_MAGIC_OPTION:     return eATTR_ADD_MAGICAL_DEFENSE_POWER;
    case eATTACK_TYPE_ALL_MAGIC:        return eATTR_ADD_MAGICAL_ALL_DEFENSE_POWER;
    default:                            
        return eATTR_ADD_MELEE_DEFENSE_POWER;
    }
}
#endif

eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForStat(eSTAT_TYPE statType, DetailLogging* logging) const
{
    __TOUCHED(logging);
    switch (statType)
    {
    case eSTAT_TYPE_STR:                return eATTR_STR;
    case eSTAT_TYPE_DEX:                return eATTR_DEX;
    case eSTAT_TYPE_VIT:                return eATTR_VIT;
    case eSTAT_TYPE_SPR:                return eATTR_SPR;
    case eSTAT_TYPE_INT:                return eATTR_INT;
    default:                            
        return eATTR_STR;
    }
}

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForBonusDamage(eARMOR_TYPE armorType, int iValueType, DetailLogging* logging) const
{
    if (iValueType == VALUE_TYPE_VALUE)
    {
        switch(armorType)
        {
        case eARMOR_HARD:                   return eATTR_ADD_ARMOR_HARD_DAMAGE;
        case eARMOR_MEDIUM:                 return eATTR_ADD_ARMOR_MEDIUM_DAMAGE;
        case eARMOR_SOFT:                   return eATTR_ADD_ARMOR_SOFT_DAMAGE;
        case eARMOR_SIEGE:                  return eATTR_ADD_ARMOR_SIEGE_DAMAGE;
        case eARMOR_UNARMOR:                return eATTR_ADD_ARMOR_UNARMOR_DAMAGE;
        default:                            return eATTR_ADD_ARMOR_HARD_DAMAGE;
        }
    }
    else if (iValueType == VALUE_TYPE_RATIO_VALUE)
    {
        switch(armorType)
        {
        case eARMOR_HARD:                   return eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE;
        case eARMOR_MEDIUM:                 return eATTR_ADD_RATIO_ARMOR_MEDIUM_DAMAGE;
        case eARMOR_SOFT:                   return eATTR_ADD_RATIO_ARMOR_SOFT_DAMAGE;
        case eARMOR_SIEGE:                  return eATTR_ADD_RATIO_ARMOR_SIEGE_DAMAGE;
        case eARMOR_UNARMOR:                return eATTR_ADD_RATIO_ARMOR_UNARMOR_DAMAGE;
        default:                            return eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE;
        }
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "BASE_ABILITYINFO : [GetAttrTypeForBonusDamage] Invalid ValueType[%d]! \n", iValueType);
        return eATTR_ADD_ARMOR_HARD_DAMAGE;
    }
}
#endif

eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForReduceDamage(eATTACK_TYPE attackType, DetailLogging* logging) const
{
#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    static const eATTR_TYPE converter_static[eATTACK_TYPE_MAX] =
    {
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_INVALID
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MELEE
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_RANGE
        eATTR_DEL_WATER_DAMAGE,             //eATTACK_TYPE_WATER
        eATTR_DEL_FIRE_DAMAGE,              //eATTACK_TYPE_FIRE
        eATTR_DEL_WIND_DAMAGE,              //eATTACK_TYPE_WIND
        eATTR_DEL_EARTH_DAMAGE,             //eATTACK_TYPE_EARTH
        eATTR_DEL_DARKNESS_DAMAGE,          //eATTACK_TYPE_DARKNESS
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_DIVINE
        eATTR_DEL_PHYSICAL_DAMAGE,          //eATTACK_TYPE_PHYSICAL_OPTION
        eATTR_DEL_MAGICAL_ALL_DAMAGE,       //eATTACK_TYPE_MAGIC_OPTION
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_ALL_MAGIC
        eATTR_TYPE_INVALID,                 //eATTACK_TYPE_MAGIC
        //
    };

    const bool accept_flag = (((1 << attackType) & eATTACK_TYPE_ABILITY_OPTION_ALL_BITS) != 0);
    const eATTR_TYPE attr_type
        = (accept_flag && _countof(converter_static) > attackType)
        ?   converter_static[attackType]
        :   eATTR_TYPE_INVALID;
    if (attr_type == eATTR_TYPE_INVALID) {
        logging->valid_check = false;
    };
    return attr_type;
#else
    switch(attackType)
    {
    case eATTACK_TYPE_ALL_OPTION:       return eATTR_DEL_ALL_DAMAGE;
    case eATTACK_TYPE_MELEE:            return eATTR_DEL_MELEE_DAMAGE;
    case eATTACK_TYPE_RANGE:            return eATTR_DEL_RANGE_DAMAGE;
    case eATTACK_TYPE_WATER:            return eATTR_DEL_WATER_DAMAGE;
    case eATTACK_TYPE_FIRE:             return eATTR_DEL_FIRE_DAMAGE;
    case eATTACK_TYPE_WIND:             return eATTR_DEL_WIND_DAMAGE;
    case eATTACK_TYPE_EARTH:            return eATTR_DEL_EARTH_DAMAGE;
    case eATTACK_TYPE_DARKNESS:         return eATTR_DEL_DARKNESS_DAMAGE;
    case eATTACK_TYPE_DIVINE:           return eATTR_DEL_DIVINE_DAMAGE;
    case eATTACK_TYPE_PHYSICAL_OPTION:  return eATTR_DEL_PHYSICAL_DAMAGE;
    case eATTACK_TYPE_MAGIC_OPTION:     return eATTR_DEL_MAGICAL_DAMAGE;
    case eATTACK_TYPE_ALL_MAGIC:        return eATTR_DEL_MAGICAL_ALL_DAMAGE;
    default:                            
        return eATTR_DEL_MELEE_DAMAGE;
    }
#endif
}

eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForAttackRange(eRANGE_TYPE rangeType, DetailLogging* logging) const
{
    __TOUCHED(logging);
    switch (rangeType)
    {
    case eRANGE_TYPE_ALL:       return eATTR_ALL_ATTACK_RANGE;
    case eRANGE_TYPE_NORMAL:    return eATTR_NORMAL_ATTACK_RANGE;
    case eRANGE_TYPE_SKILL:     return eATTR_SKILL_ATTACK_RANGE;
    default:
        return eATTR_NORMAL_ATTACK_RANGE;
    }
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

bool SkillInfoParser::CheckValidity(SkillScriptInfo* const skill_info, const int row) const
{
    bool is_success = true;
    const SLOTCODE skill_code = skill_info->m_SkillCode;

    // 스킬 레벨을 검사한다.
    if (skill_info->m_wSkillLv > MAX_SKILL_OVER_LEVEL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|스킬 레벨이 범위를 초과 했습니다.|Row = %d, SkillCode = %d, SkillLevel = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_wSkillLv
        );
        skill_info->m_wSkillLv = MAX_SKILL_OVER_LEVEL;
        is_success = false;
        ASSERT(false);
    }

    // 최고 레벨을 검사한다.
    if (skill_info->m_wMaxLv > MAX_SKILL_LEVEL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|스킬 최고 레벨이 범위를 초과 했습니다.|Row = %d, SkillCode = %d, SkillMaxLevel = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_wMaxLv
        );
        skill_info->m_wMaxLv = MAX_SKILL_LEVEL;
        is_success = false;
        ASSERT(false);
    }

    // 오버가능 레벨을 검사한다.
    if (skill_info->m_wOverLv > MAX_SKILL_OVER_LEVEL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|스킬 오버가능 레벨이 범위를 초과 했습니다.|Row = %d, SkillCode = %d, SkillOverLevel = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_wOverLv
        );
        skill_info->m_wOverLv = MAX_SKILL_OVER_LEVEL;
        is_success = false;
        ASSERT(false);
    }

    // 스킬 타입을 검사한다.
    if (skill_info->m_bySkillType && 
        skill_info->m_bySkillType != SKILL_TYPE_PASSIVE && 
        skill_info->m_bySkillType != SKILL_TYPE_ACTIVE)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|스킬 타입이 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillType = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_bySkillType
        );
        skill_info->m_bySkillType = SKILL_TYPE_ACTIVE;
        is_success = false;
        ASSERT(false);
    }

    // 스킬 사용자 타입을 검사한다.
    switch(skill_info->m_bySkillUserType)
    {
    case eSKILL_USER_PLAYER:
        switch(skill_info->m_dwClassDefine)
        {
        case eCHAR_BERSERKER:
            if (skill_info->m_SkillClassCode < BerserkerJob1Boundary::kBegin ||
                skill_info->m_SkillClassCode > BerserkerJob1Boundary::kEnd)
            {
                if (skill_info->m_SkillClassCode < BerserkerJob2Boundary::kBegin || 
                    skill_info->m_SkillClassCode > BerserkerJob2Boundary::kEnd)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|버서커 스킬 클래스 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillClassCode = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        skill_info->m_SkillClassCode
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
            break;
        case eCHAR_DRAGON:
            if (skill_info->m_SkillClassCode < DragonJob1Boundary::kBegin ||
                skill_info->m_SkillClassCode > DragonJob1Boundary::kEnd)
            {
                if (skill_info->m_SkillClassCode < DragonJob2Boundary::kBegin || 
                    skill_info->m_SkillClassCode > DragonJob2Boundary::kEnd)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|드래곤나이트 스킬 클래스 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillClassCode = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        skill_info->m_SkillClassCode
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
            break;
        case eCHAR_SHADOW:
            if (skill_info->m_SkillClassCode < ShadowJob1Boundary::kBegin ||
                skill_info->m_SkillClassCode > ShadowJob1Boundary::kEnd) 
            {
                if (skill_info->m_SkillClassCode < ShadowJob2Boundary::kBegin || 
                    skill_info->m_SkillClassCode > ShadowJob2Boundary::kEnd)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|쉐도우 스킬 클래스 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillClassCode = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        skill_info->m_SkillClassCode
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
            break;
        case eCHAR_VALKYRIE:
            if (skill_info->m_SkillClassCode < ValkyrieJob1Boundary::kBegin ||
                skill_info->m_SkillClassCode > ValkyrieJob1Boundary::kEnd) 
            {
                if (skill_info->m_SkillClassCode < ValkyrieJob2Boundary::kBegin || 
                    skill_info->m_SkillClassCode > ValkyrieJob2Boundary::kEnd)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|발키리 스킬 클래스 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillClassCode = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        skill_info->m_SkillClassCode
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
            break;
        case eCHAR_ELEMENTALIST:
            if (skill_info->m_SkillClassCode < ElementalistJob1Boundary::kBegin ||
                skill_info->m_SkillClassCode > ElementalistJob1Boundary::kEnd) 
            {
                if (skill_info->m_SkillClassCode < ElementalistJob2Boundary::kBegin || 
                    skill_info->m_SkillClassCode > ElementalistJob2Boundary::kEnd)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|엘리멘탈리스트 스킬 클래스 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillClassCode = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        skill_info->m_SkillClassCode
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
            break;
        case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            // 스킬 작업 시작하면 범위 검증 로직을 추가하자.
            break;
        case eCHAREX_DRAGON_1stTRANS:
        case eCHAREX_DRAGON_2ndTRANS:
        case eCHAREX_BERSERKER_100LEVEL:
        case eCHAREX_DRAGON_KNIGHT_100LEVEL:
        case eCHAREX_SHADOW_100LEVEL:
        case eCHAREX_VALKYRIE_100LEVEL:
        case eCHAREX_ELEMENTALIST_100LEVEL:
        case eCHAREX_MYSTIC_100LEVEL:
        case eCHAREX_HELLROID_100LEVEL:
        case eCHAREX_DRAGON_1stTRANS_100LEVEL:
        case eCHAREX_DRAGON_2ndTRANS_100LEVEL:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAREX_WITCHBLADE_100LEVEL:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        default:
            break;
        }
        break;
    case eSKILL_USER_MONSTER:
    case eSKILL_USER_NPC:
        if (skill_info->m_dwClassDefine != 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|해당 스킬 사용자 타입은 클래스 제한값이 0이어야 합니다.|Row = %d, SkillCode = %d, SkillUserType = %d, ClassDefine = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                skill_info->m_bySkillUserType, 
                skill_info->m_dwClassDefine
            );
            skill_info->m_dwClassDefine = 0;
            is_success = false;
            ASSERT(false);
        }
        break;
    case eSKILL_USER_SUMMONED: // 소환체
    case eSKILL_USER_FOLLOWER: // 용병
    case eSKILL_USER_ACTION: // 액션
    case eSKILL_USER_EMOTICON: // 이모티콘
        break;
    default:
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|알 수 없는 스킬 사용자 타입 입니다.|Row = %d, SkillCode = %d, SkillUserType = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_bySkillUserType
        );
        skill_info->m_bySkillUserType = eSKILL_USER_PLAYER;
        is_success = false;
        ASSERT(false);
        break;
    }

    // 요구 숙련도를 검사한다.
    for (int i = 0; i < 2; ++i)
    {
        if (skill_info->m_wRequireSkillStat[i] > 100000)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|요구 숙련도 입력값이 너무 큽니다.|Row = %d, SkillCode = %d, RequireSkillStat = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                skill_info->m_wRequireSkillStat[i]
            );
            is_success = false;
            ASSERT(false);
        }
    }

    // 타겟 지정 대상을 검사한다.
    if (skill_info->m_byTarget >= SKILL_TARGET_MAX)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|타겟 지정 대상 범위를 초과 했습니다.|Row = %d, SkillCode = %d, Target = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_byTarget
        );
        skill_info->m_byTarget = SKILL_TARGET_ENEMY;
        is_success = false;
        ASSERT(false);
    }

    // 스킬 캐스팅 시간을 검사한다.
    if (skill_info->m_wSkillCasting >= 20000)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|스킬 캐스팅 시간 입력값이 너무 큽니다.|Row = %d, SkillCode = %d, SkillCasting = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_wSkillCasting
        );
        skill_info->m_wSkillCasting = 20000;
        is_success = false;
        ASSERT(false);
    }

    // 스킬 사거리를 검사한다.
    if (skill_info->m_wSkillRange != 0)
    {
        if (skill_info->m_SkillClassCode != eSKILL_TELEPORT && 
            (skill_info->m_wSkillRange < 10 || skill_info->m_wSkillRange > 300))
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|스킬 사거리의 입력 범위가 올바르지 않습니다.|Row = %d, SkillCode = %d, SkillRange = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                skill_info->m_wSkillRange
            );
            skill_info->m_wSkillRange = 30;
            is_success = false;
            ASSERT(false);
        }
    }

    // 범위형태를 검사한다.
    if (skill_info->m_byAttRangeform >= SRF_MAX)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|스킬 범위형태가 입력 범위를 초과 했습니다.|Row = %d, SkillCode = %d, AttRangeForm = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code, 
            skill_info->m_byAttRangeform
        );
        skill_info->m_byAttRangeform = SRF_FOWARD_ONE;
        is_success = false;
        ASSERT(false);
    }

    // 스킬 범위를 검사한다.
    if (skill_info->m_byAttRangeform == SRF_FOWARD_120 || 
        skill_info->m_byAttRangeform == SRF_FOWARD_160 || 
        skill_info->m_byAttRangeform == SRF_FOWARD_360 || 
        skill_info->m_byAttRangeform == SRF_PIERCE)
    {
        if (skill_info->m_wSkillArea == 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|해당 스킬 범위 형태는 스킬 범위값을 입력해야 합니다.|Row = %d, SkillCode = %d, AttRangeForm = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                skill_info->m_byAttRangeform
            );
            skill_info->m_wSkillArea = 1;
            is_success = false;
            ASSERT(false);
        }
    }

    // 어빌리티를 검사한다.
    skill_info->SetFirst();
    while (BASE_ABILITYINFO* ability_info = skill_info->GetNext())
    {
        if (ability_info->m_wAbilityID == 0)
        {
            continue;
        }
        
        // 어빌리티ID를 검사한다.
        if (ability_info->m_wAbilityID >= eABILITY_MAX)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|어빌리티ID가 범위를 초과 했습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                ability_info->m_wAbilityID
            );
            ability_info->m_wAbilityID = 1;
            is_success = false;
            ASSERT(false);
        }

        // 적용 범위를 검사한다.
        if (ability_info->m_byRangeType >= SKILL_ABILITY_MAX)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|어빌리티 적용 범위가 입력 범위를 초과 했습니다.|Row = %d, SkillCode = %d, AbilityID = %d, RangeType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                ability_info->m_wAbilityID, 
                ability_info->m_byRangeType
            );
            ability_info->m_byRangeType = SKILL_ABILITY_ENEMY;
            is_success = false;
            ASSERT(false);
        }

        // 성공 확률을 검사한다.
        if (ability_info->m_wSuccessRate == 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|어빌리티 성공 확률은 0보다 커야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                ability_info->m_wAbilityID
            );
            ability_info->m_wSuccessRate = 1000;
            is_success = false;
            ASSERT(false);
        }

        // 유지시간이 존재하면 상태ID가 존재해야한다.
        const int apply_time = ability_info->m_iParam[2];
        if (apply_time != 0)
        {
            if (ability_info->m_wStateID == 0 && 
                ability_info->m_wAbilityID != eABILITY_SUMMON_CHANGE_STATUS &&
                ability_info->m_wAbilityID != eABILITY_BUFF_RANGE_DAMAGE &&
                ability_info->m_wAbilityID != eABILITY_DRAGON_TRANSFORMATION2 &&
                ability_info->m_wAbilityID != eABILITY_DRAGON_TRANSFORMATION3 &&
                ability_info->m_wAbilityID != eABILITY_SUMMON &&
                ability_info->m_wAbilityID != eABILITY_SUMMONMASTERY &&
                ability_info->m_wAbilityID != eABILITY_DAMAGE &&
                ability_info->m_wAbilityID != kAbilityComboSkillEffectAddBadStatus &&
                ability_info->m_wAbilityID != eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY &&
                ability_info->m_wAbilityID != eABILITY_SUMMON_CHANGE_STATUS_EXTEND &&
                ability_info->m_wAbilityID != eABILITY_SUMMON_IMMOVABLE)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|어빌리티 유지시간이 존재하면 유지할 상태ID가 존재해야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d, ApplyTime = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    apply_time
                );
                is_success = false;
                ASSERT(false);
            }
        }

        if (ability_info->m_wAbilityID == eABILITY_DAMAGE)
        {
            if (ability_info->m_wStateID != 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|데미지 어빌리티인데 상태ID가 존재 합니다.|Row = %d, SkillCode = %d, AbilityID = %d, StateID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_wStateID
                );
                ability_info->m_wStateID = 0;
                is_success = false;
                ASSERT(false);
            }

            if (ability_info->m_byRangeType == SKILL_ABILITY_FRIEND || 
                ability_info->m_byRangeType == SKILL_ABILITY_TARGETAREA_FRIEND || 
                ability_info->m_byRangeType == SKILL_ABILITY_MYAREA_FRIEND || 
                ability_info->m_byRangeType == SKILL_ABILITY_ME || 
                ability_info->m_byRangeType == SKILL_ABILITY_FIELD)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|데미지 어빌리티는 아군을 대상으로 사용할 수 없습니다.|Row = %d, SkillCode = %d, AbilityID = %d, RangeType = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_byRangeType
                );
                ability_info->m_byRangeType = SKILL_ABILITY_TARGETAREA_ENEMY;
                is_success = false;
                ASSERT(false);
            }

            if (skill_info->m_wSkillLv < 5)
            {
                const int damage_value = ability_info->m_iParam[0];
                /*if (damage_value > 10000)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|데미지 입력값이 너무 큽니다.|Row = %d, SkillCode = %d, AbilityID = %d, DamageValue = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID, 
                        damage_value
                    );
                    is_success = false;
                    ASSERT(false);
                }*/
                const int damage_ratio = ability_info->m_iParam[1];
                if (damage_ratio > 3000)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|데미지 입력 비율값이 너무 큽니다.|Row = %d, SkillCode = %d, AbilityID = %d, DamageRatio = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID, 
                        damage_ratio
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
#ifdef _NA_000000_20140103_CHANGE_DASH_AND_PIERCE_SKILL_MOVE
            const bool is_moving_attack = (ability_info->m_iParam[2] == 1);
            if (is_moving_attack)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|데미지어빌리티에서 더 이상 이동을 처리하지 않습니다.(1304) |Row = %d, SkillCode = %d, AbilityID = %d, is_moving_attack = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    is_moving_attack
                    );
                is_success = false;
                ASSERT(false);
                //1304 데미지어빌리티 리뉴얼작업으로 이동공격의 이동을 다른 어빌리티로 분리시킴.
            }
#endif //_NA_000000_20140103_CHANGE_DASH_AND_PIERCE_SKILL_MOVE
        }
        else if (ability_info->m_wAbilityID == eABILITY_DAMAGE_PER_TIME)
        {
            if (ability_info->m_wStateID == 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|시간당 데미지를 주는 어빌리티는 상태ID가 필요 합니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID
                );
                ability_info->m_wStateID = eCHAR_STATE_PERIODIC_DAMAGE;
                is_success = false;
                ASSERT(false);
            }

            const int damage_value_type = ability_info->m_iParam[0];
            if (damage_value_type != VALUE_TYPE_VALUE)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|어빌리티 데미지 값 타입은 수치 형식이어야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d, ValueType = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    damage_value_type
                );
                ability_info->m_iParam[0] = VALUE_TYPE_VALUE;
                is_success = false;
                ASSERT(false);
            }
            
            const int damage_value = ability_info->m_iParam[1];
            /*if (damage_value > 1000)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|데미지 입력값이 너무 큽니다.|Row = %d, SkillCode = %d, AbilityID = %d, DamageValue = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    damage_value
                );
                is_success = false;
                ASSERT(false);
            }*/
        }
        if (ability_info->m_iParam[3] && ability_info->m_iParam[2] == 0)
        {
            const int period_time = ability_info->m_iParam[3];
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|어빌리티 주기 시간은 존재하는데 유지 시간은 0일 수 없습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                ability_info->m_wAbilityID
            );
            ability_info->m_iParam[2] = period_time;
            is_success = false;
            ASSERT(false);
        }

        if (ability_info->m_byRangeType == SKILL_ABILITY_TARGETAREA_ENEMY || 
            ability_info->m_byRangeType == SKILL_ABILITY_TARGETAREA_FRIEND ||
            ability_info->m_byRangeType == SKILL_ABILITY_MYAREA_ENEMY ||
            ability_info->m_byRangeType == SKILL_ABILITY_MYAREA_FRIEND)
        {
            /*if (skill_info->m_byAttRangeform != SRF_FOWARD_120 &&
                skill_info->m_byAttRangeform != SRF_FOWARD_160 &&
                skill_info->m_byAttRangeform != SRF_FOWARD_360 &&
                skill_info->m_byAttRangeform != SRF_PIERCE &&
                ability_info->m_wAbilityID != eABILITY_BUFF_RANGE_DAMAGE)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|지역 범위의 어빌리티이면 스킬 범위도 대상이 아니라 지역이어야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d, AbilityRangeType = %d, SkillAttRangeForm = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_byRangeType, 
                    skill_info->m_byAttRangeform
                );
                skill_info->m_byAttRangeform = SRF_FOWARD_360;
                is_success = false;
                // 여기는 어그로 어빌리티 때문에 정책 검토가 필요하다.
                //ASSERT(false);
            }*/
            if(skill_info->m_wSkillArea == 0 || skill_info->m_byMaxTargetNo == 0)
            {
				// 해당 스킬은 무한루프의 원인이 될 수 있다.
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|지역 범위의 어빌리티이면 스킬 범위와 최대 적용 타겟 수가 지정되어 있어야 합니다.." \
                        "|Row = %d, SkillCode = %d, AbilityID = %d, m_wSkillArea = %d, m_byMaxTargetNo = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    skill_info->m_wSkillArea, 
                    skill_info->m_byMaxTargetNo
                    );
                is_success = false;
                ASSERT(false);
            }
        }
        else if (ability_info->m_byRangeType == SKILL_ABILITY_FIELD)
        {
            if (skill_info->m_byTarget != SKILL_TARGET_AREA && 
                skill_info->m_bySkillUserType == eSKILL_USER_PLAYER)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|필드 범위의 어빌리티이면 스킬 타겟도 지역이어야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d, AbilityRangeType = %d, SkillTarget = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_byRangeType, 
                    skill_info->m_byTarget
                );
                skill_info->m_byTarget = SKILL_TARGET_AREA;
                is_success = false;
                ASSERT(false);
            }
        }
        else if (ability_info->m_byRangeType == SKILL_ABILITY_CORPSE_RESURRECTION)
        {
            if (skill_info->m_byTarget != SKILL_TARGET_FRIEND_CORPSE &&
                skill_info->m_byTarget != SKILL_TARGET_AREA &&
                skill_info->m_SkillCode != 55006)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|아군 시체를 범위로 사용하는 어빌리티가 허용할 수 없는 스킬 사용 타겟입니다.|Row = %d, SkillCode = %d, AbilityID = %d, AbilityRangeType = %d, SkillTarget = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_byRangeType, 
                    skill_info->m_byTarget
                );
                skill_info->m_byTarget = SKILL_TARGET_FRIEND_CORPSE;
                is_success = false;
                ASSERT(false);
            }
        }
        else if (ability_info->m_byRangeType == SKILL_ABILITY_SUMMON)
        {
            if (skill_info->m_byTarget != SKILL_TARGET_SUMMON && 
                skill_info->m_byTarget != SKILL_TARGET_AREA)
            {
                if (skill_info->m_byTarget != SKILL_TARGET_AREA)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|소환수를 범위로 사용하는 어빌리티가 허용할 수 없는 스킬 사용 타겟입니다.|Row = %d, SkillCode = %d, AbilityID = %d, AbilityRangeType = %d, SkillTarget = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID, 
                        ability_info->m_byRangeType, 
                        skill_info->m_byTarget
                    );
                    skill_info->m_byTarget = SKILL_TARGET_SUMMON;
                    is_success = false;
                    ASSERT(false);
                }
            }
        }

        if (ability_info->m_wAbilityID == eABILITY_MAGIC_SHIELD)
        {
            const int shield_hp = ability_info->m_iParam[1];
            const int decrese_mp = ability_info->m_iOption2;
            const double absorb_ratio = ability_info->m_iParam[0] / 1000.f;

            if (shield_hp <= 0 || decrese_mp < 0 || absorb_ratio <= 0 || absorb_ratio > 1.00001f)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|매직 실드 어빌리티 입력값이 올바르지 않습니다.|Row = %d, SkillCode = %d, AbilityID = %d, ShieldHP = %d, DecreseMP = %d, AbsorbRatio = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    shield_hp, 
                    decrese_mp, 
                    ability_info->m_iParam[0]
                );
                is_success = false;
                if (shield_hp < 0)
                {
                    ability_info->m_iParam[1] = 1;
                }
                if (decrese_mp < 0)
                {
                    ability_info->m_iOption2 = -decrese_mp;
                }
                if (absorb_ratio <= 0)
                {
                    ability_info->m_iParam[0] = 100;
                }
                if (absorb_ratio > 1.1f)
                {
                    ability_info->m_iParam[0] = 1000;
                }
                ASSERT(false);
            }
        }

        if (ability_info->m_wAbilityID == eABILITY_WIND_SHIELD)
        {
            if (ability_info->m_byRangeType != SKILL_ABILITY_ME)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|윈드 실드 어빌리티는 적용 범위가 자기 자신이어야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d, AbilityRangeType = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_byRangeType
                );
                ability_info->m_byRangeType = SKILL_ABILITY_ME;
                is_success = false;
                ASSERT(false);
            }
        }

        // 투기당 데미지 증가 어빌리티를 검사한다.
        if (ability_info->m_wAbilityID == eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY)
        {
            if (ability_info->m_iOption2 == 0)
            {
                if (ability_info->m_iParam[0] == 0 || 
                    ability_info->m_iParam[1] == 0 || 
                    ability_info->m_iParam[2] == 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|모든 투기 소비 데미지 증가 어빌리티의 데미지 증가값은 0일 수 없습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
            else
            {
                if (ability_info->m_iParam[0] == 0 && ability_info->m_iParam[1] == 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|투기당 데미지 증가값이 모두 0입니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
        }

        // 어빌리티 210(상태별 추가데미지)에서 1번 어빌리티와의 순서를 검증한다
        if (ability_info->m_wAbilityID == eABILITY_BONUS_DAMAGE_PER_STATUS)
        {
            BASE_ABILITYINFO* damage_ability_info = skill_info->GetAbilityInfo(eABILITY_DAMAGE);
            if (damage_ability_info == NULL ||
                damage_ability_info->m_byIndex <= ability_info->m_byIndex)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|해당 어빌리티는 AbilityID=1보다 먼저 사용되어야 합니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID
                    );
                is_success = false;
                ASSERT(false);
            }
        }

        // 인첸트 포이즌 어빌리티를 검사한다.
        if (ability_info->m_wAbilityID == eABILITY_ENCHANT_POISON)
        {
            const int effect_ability_id = ability_info->m_iOption1;
            if (effect_ability_id == 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|인첸트 포이즌 상태에서 타격시 발동시킬 어빌리티가 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID
                );
                is_success = false;
                ASSERT(false);
            }
        }

        // 치유량 증가 어빌리티를 검사한다.
        if (ability_info->m_wAbilityID == kAbilityIncreseHeal)
        {
            const int heal_type = ability_info->m_iOption1;
            if (heal_type != IncreseHealAbilityType::kSkill && 
                heal_type != IncreseHealAbilityType::kItem && 
                heal_type != IncreseHealAbilityType::kSkillAndItem)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|치유량 증가 타입이 올바르지 않습니다.|Row = %d, SkillCode = %d, AbilityID = %d, HealType = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    heal_type
                );
                is_success = false;
                ASSERT(false);
            }
            const int heal_value = ability_info->m_iParam[0];
            if (heal_value == 0)
            {
                const int heal_ratio = ability_info->m_iParam[1];
                if (heal_ratio == 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|증가시킬 치유량 값과 비율 둘다 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
        }

        // 연계 스킬 활성화 어빌리티를 검사한다.
        if (ability_info->m_wAbilityID == kAbilityActiveComboSkill)
        {
            const int skill_class_code = ability_info->m_iOption1;
            if (skill_class_code == 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|강화할 스킬 클래스 코드가 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID
                );
                is_success = false;
                ASSERT(false);
            }
            const int apply_time = ability_info->m_iParam[2];
            if (apply_time == 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|연계 스킬 상태 유지 시간이 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID
                );
                is_success = false;
                ASSERT(false);
            }
            const int state_id = ability_info->m_wStateID;
            if (state_id != kCharStateActiveComboSkill)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|입력된 상태가 연계 스킬 활성화 상태가 아닙니다.|Row = %d, SkillCode = %d, AbilityID = %d, StateID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    state_id
                );
                is_success = false;
                ASSERT(false);
            }
        }

        // 연계 스킬 효과 데미지 추가 어빌리티를 검사한다.
        if (ability_info->m_wAbilityID == kAbilityComboSkillEffectAddDamage)
        {
            const int add_value = ability_info->m_iParam[0];
            if (add_value == 0)
            {
                const int add_ratio = ability_info->m_iParam[1];
                if (add_ratio == 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|강화 시킬 데미지 값과 비율 둘다 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
        }

        // 연계 스킬 효과 상태 이상 강화 어빌리티를 검사한다.
        if (ability_info->m_wAbilityID == kAbilityComboSkillEffectAddBadStatus)
        {
            const int state_id = ability_info->m_iOption1;
            if (state_id == 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|강화할 상태가 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID
                );
                is_success = false;
                ASSERT(false);
            }
            const int success_rate = ability_info->m_iParam[1];
            if (success_rate == 0)
            {
                const int apply_time = ability_info->m_iParam[2];
                if (apply_time == 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|강화 시킬 확률이나 지속시간 둘다 입력되지 않았습니다.|Row = %d, SkillCode = %d, AbilityID = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        skill_code, 
                        ability_info->m_wAbilityID
                    );
                    is_success = false;
                    ASSERT(false);
                }
            }
        }

#ifdef _SERVER
        // CHANGES: f110214.3L, added debugging facility
        bool valid_check_attr_type = false;
        const eATTR_TYPE attr_type = ability_info->GetAttrTypeDetail(&valid_check_attr_type);
        if (valid_check_attr_type == false)
        {
            SUNLOG(
                eCRITICAL_LOG,
                _T("|%s|%d|잘못된 옵션 타입 입니다.|Row = %d, SkillCode = %d, AbilityID = %d, AttrIndex = %d, Option1 = %d, Option2 = %d|"),
                __FUNCTION__, 
                __LINE__, 
                row, 
                skill_code, 
                ability_info->m_wAbilityID,
                ability_info->m_byIndex, 
                ability_info->m_iOption1, 
                ability_info->m_iOption2
            );
            is_success = false;
            ASSERT(false);
        }
#else
        const eATTR_TYPE attr_type = ability_info->GetAttrType();
#endif
        if (attr_type != eATTR_TYPE_INVALID)
        {
            int option_type = ability_info->m_iParam[0];
            int option_value = ability_info->m_iParam[1];
            if (option_type != VALUE_TYPE_VALUE)
            {
                option_value = option_value / 10;
            }
            if (ability_info->m_wAbilityID == eABILITY_CHANGE_ATTR)
            {
                option_type = VALUE_TYPE_VALUE;
            }
            if (CheckAttrValidity(ability_info->GetAttrType(), option_type, option_value) == false)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    "|%s|%d|옵션값 검증에 실패 했습니다.|Row = %d, SkillCode = %d, AbilityID = %d, AttrIndex = %d, OptionType = %d, OptionValue = %d|", 
                    row, 
                    skill_code, 
                    ability_info->m_wAbilityID, 
                    ability_info->GetAttrType(), 
                    option_type, 
                    option_value
                );
                ASSERT(false);
            }
        }
    }

    if (skill_code == SKILLCODE_NORMAL_ATTACK)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|사용할 수 없는 스킬 코드 입니다.|Row = %d, SkillCode = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            skill_code
        );
        ASSERT(false);
        return false;
    }

    return is_success;
}

BOOL SkillInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch(script_code)
    {
    case SCRIPT_SKILLINFO:
        return (_Load(is_reload == TRUE) ? TRUE : FALSE);
    case SCRIPT_STYLEINFO:
        return (_LoadStyle(is_reload == TRUE) ? TRUE : FALSE);
    }
    return false; 
}


#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
void SkillInfoParser::CalculateSkillLineFromSkillInventoryStoreParser()
{
    SkillInventoryStoreParser* skill_inventory_parser = SkillInventoryStoreParser::Instance();
    ASSERT(skill_inventory_parser);
    if (skill_inventory_parser == NULL)
        return;

    int count = skill_inventory_parser->GetSkillInventoryStoreNum();
    for (int idx = 0 ; idx < count ; ++idx)
    {
        const SkillInven_s* const skill_inventory = skill_inventory_parser->GetSkillInventoryStore(idx);
        ASSERT(skill_inventory);
        if (skill_inventory == NULL)
            continue;

        SkillTree* skill_tree = &(skill_tree_[skill_inventory->bClassID]);

        SkillTree::SkillDescript* skill_descript = (SkillTree::SkillDescript*)skill_tree->get_skill_descript(skill_inventory->bSkillTab2);
        if (skill_descript == NULL)
            continue;
        // CHANGES: f110216.1L, check boundary
        BOOST_STATIC_ASSERT(MAX_SKILL_INVENTORY_STORE_NUM == skill_tree->MAX_SKILLTREE_COLUMN);
        for (int num = 0 ; num < MAX_SKILL_INVENTORY_STORE_NUM ; ++num)
        {
            skill_descript[num].character_class_id = skill_inventory->bClassID;
            skill_descript[num].skill_line = skill_inventory->bSkillTab2;
            skill_descript[num].first_skill_code = (skill_inventory->iSkillID[num] > 0) ? skill_inventory->iSkillID[num] : 0;
            skill_descript[num].skill_class = 0;
            skill_descript[num].upper_skill_class = 0;

            if (skill_descript[num].first_skill_code != 0)
            {
                SkillScriptInfo* skill_info = GetSkillInfo(skill_descript[num].first_skill_code);
                if (skill_info)
                {
                    skill_descript[num].skill_class = skill_info->m_SkillClassCode;
                    skill_descript[num].upper_skill_class = skill_info->m_UpperSkillClass;
                    skill_tree->skill_hashmap[skill_info->m_SkillClassCode] = &(skill_descript[num]);
                }
            }
        }
    }
}
#endif

void SkillInfoParser::SkillJobTreeManager::LoadTree(const SkillInfoParser* skill_info_parser)
{
    Clear();

    const SkillInventoryStoreParser* const inven_parser = SkillInventoryStoreParser::Instance();
    if (inven_parser == NULL)
    {
        ASSERT(false);
        return;
    }

    const int index_size = static_cast<int>(inven_parser->GetSkillInventoryStoreNum());
    for (int index = 0; index < index_size; ++index)
    {
        const SkillInven_s* const inven_info = inven_parser->GetSkillInventoryStore(index);
        if (inven_info == NULL)
        {
            ASSERT(false);
            continue;
        }

        const int char_type = inven_info->bClassID;
        if (char_type == 0)
        {
            continue;
        }

        for (int job_type = 0; job_type < kMaxJobSize; ++job_type)
        {
            SkillJobTree* const job_tree = GetTreeByMutable(char_type, job_type);
            if (job_tree == NULL)
            {
                ASSERT(false);
                continue;
            }

            const int line_index = inven_info->bSkillTab2 - 1;
            const int col_size = job_tree->GetColumnSize();
            for (int col = 0; col < col_size; ++col)
            {
                SkillJobTree::Desc* const job_desc = job_tree->GetDescByMutable(line_index, col);
                if (job_desc == NULL)
                {
                    ASSERT(false);
                    continue;
                }

                const int skill_id_index = (job_type * col_size) + col;
                if (skill_id_index >= _countof(inven_info->iSkillID))
                {
                    ASSERT(false);
                    continue;
                }

                const int first_skill_code = inven_info->iSkillID[skill_id_index];
                if (first_skill_code <= 0)
                {
                    continue;
                }

                const SkillScriptInfo* const skill_info = 
                    skill_info_parser->GetSkillInfo(first_skill_code);
                if (skill_info == NULL)
                {
                    ASSERT(false);
                    continue;
                }

                job_desc->job_type = job_type;
                job_desc->skill_class_code = skill_info->m_SkillClassCode;
                job_desc->upper_skill_class_code = skill_info->m_UpperSkillClass;
                job_desc->first_skill_code = first_skill_code;
                job_desc->line_index = line_index;
                
                if (job_tree->InsertDesc(job_desc) == false)
                {
                    ASSERT(false);
                    continue;
                }
            }
        }
    }
}

#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForCriticalRatio( eATTR_APPLY_TYPE state_type, DetailLogging* logging /*= NULL*/ ) const
{
    __TOUCHED(logging);
    switch (state_type)
    {
        case eATTR_APPLY_TYPE_NORMAL    : return eATTR_CRITICAL_RATIO_CHANGE;
        case eATTR_APPLY_TYPE_DAMAGED : return eATTR_ENEMY_CRITICAL_RATIO_CHANGE;
        default:
            break;
    }

    return eATTR_TYPE_INVALID;
}

eATTR_TYPE BASE_ABILITYINFO::GetAttrTypeForCriticalDamage( eATTR_APPLY_TYPE state_type, DetailLogging* logging /*= NULL*/ ) const
{
    __TOUCHED(logging);
    switch (state_type)
    {
        case eATTR_APPLY_TYPE_NORMAL    :   return eATTR_CRITICAL_DAMAGE_CHANGE;
        case eATTR_APPLY_TYPE_DAMAGED :   return eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE;
        default:
            break;
    }

    return eATTR_TYPE_INVALID;
}
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL