#include "StdAfx.h"
#include "CharacterFormula.h"

//==================================================================================================
//  Program Common
#include <misc.h>
#include <NumericValues.h>
#include <Battle.h>
#include <PublicMath.h>
#include <FormulaRatioParser.h>
#include <EtherBulletInfoParser.h>
#include <Attribute.h>
//--------------------------------------------------------------------------------------------------
// SolarSlot
#include "ItemOptionHeader.h"
#include "SCItemSlot.h"
#include "SCItemSlotContainer.h"    //..
//--------------------------------------------------------------------------------------------------
// Common
#include "ArmorDamageParser.h"
//--------------------------------------------------------------------------------------------------
#include "Player.h"
#include "ItemManager.h"
#include "./Status/StatusManager.h"

// Ability
#include "AbilityStatus.h"
#include "Skill.h"

//#ifdef _NA_006040_20120830_RESIS_BAD_STATUS_RATIO
#include "StateInfoParser.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Ability
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ns_formula {
namespace ability {

namespace impl {

const BASE_ABILITYINFO* GetAbilityInfo(
    const Character& skill_user, const eCHAR_STATE_TYPE state_type)
{
    const StatusManager* const status_manager = skill_user.GetStatusManager();
    if (status_manager == NULL)
    {
        return NULL;
    }
    const BaseStatus* const status = status_manager->FindStatus(state_type);
    if (status == NULL)
    {
        return NULL;
    }
    if (status->IsAbilityStatus() == false)
    {
        return NULL;
    }
    const AbilityStatus* const ability_status = static_cast<const AbilityStatus* const>(status);
    if (ability_status == NULL)
    {
        return NULL;
    }
    return ability_status->GetAbilityInfo();
}

const BASE_ABILITYINFO* GetComboEffectInfo(
    const Character& skill_user, 
    const Skill& skill, 
    const eABILITY effect_ability_code, 
    const int state_id)
{
    // 콤보 상태가 활성화 되어 있는지 검사한다.
    const BASE_ABILITYINFO* const active_combo_info = 
        GetAbilityInfo(skill_user, kCharStateActiveComboSkill);
    if (active_combo_info == NULL)
    {
        return NULL;
    }

    // 사용 가능한 연계 스킬인지 검사한다.
    const eSKILL skill_class_code = skill.GetSkillClassCode();
    int temp_skill_class_code = active_combo_info->m_iOption1;
    if (temp_skill_class_code != skill_class_code)
    {
        temp_skill_class_code = active_combo_info->m_iOption2;
        if (temp_skill_class_code != skill_class_code)
        {
            temp_skill_class_code = active_combo_info->m_iParam[0];
            if (temp_skill_class_code != skill_class_code)
            {
                temp_skill_class_code = active_combo_info->m_iParam[1];
                if (temp_skill_class_code != skill_class_code)
                {
                    return NULL;
                }
            }
        }
    }
    
    // 발동 효과 어빌리티 정보를 얻는다.
    const SkillScriptInfo* const skill_info = skill.GetSkillBaseInfo();
    if (skill_info == NULL)
    {
        return NULL;
    }
    const BASE_ABILITYINFO* const combo_effect_info = 
        skill_info->GetAbilityInfo(effect_ability_code);
    if (combo_effect_info == NULL)
    {
        return NULL;
    }

    // 상태이상 강화이면 강화할 상태인지 검사한다.
    if (effect_ability_code == kAbilityComboSkillEffectAddBadStatus && state_id != 0)
    {
        int temp_state_id = combo_effect_info->m_iOption1;
        if (temp_state_id != state_id)
        {
            temp_state_id = combo_effect_info->m_iOption2;
            if (temp_state_id != state_id)
            {
                temp_state_id = combo_effect_info->m_iParam[0];
                if (temp_state_id != state_id)
                {
                    return NULL;
                }
            }
        }
    }

    return combo_effect_info;
}

} // impl

#ifdef _DEBUG
#define DEBUG_ABILITY_CALC_
#endif // _DEBUG

int CalcIncreseHeal(
    const IncreseHealAbilityType::Value heal_type, 
    const int heal_value, 
    const Character& skill_user)
{
    const BASE_ABILITYINFO* const ability_info = 
        impl::GetAbilityInfo(skill_user, kCharStateIncreseHeal);
    if (ability_info == NULL)
    {
        return 0;
    }
    const int temp_heal_type = ability_info->m_iOption1;
    if (temp_heal_type != IncreseHealAbilityType::kSkillAndItem)
    {
        if (temp_heal_type != heal_type)
        {
            return 0;
        }
    }
    int add_value = 0;
    {
        // 추가 회복 값
        const int add_heal_value = ability_info->m_iParam[0];
        if (add_heal_value != 0)
        {
            add_value += add_heal_value;
        }

        // 추가 회복 비율
        const int add_heal_ratio = ability_info->m_iParam[1];
        if (add_heal_ratio != 0)
        {
            add_value += 
                static_cast<int>(static_cast<float>(heal_value) * (static_cast<float>(add_heal_ratio) / 1000.0f));
        }

#ifdef DEBUG_ABILITY_CALC_
        if (add_value != 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|HealValue = %d, HealRatio = %d, AddValue = %d|"), 
                __FUNCTION__, 
                add_heal_value, 
                add_heal_ratio, 
                add_value
            );
        }
#endif // DEBUG_ABILITY_CALC_
    }
    return add_value;
}

int CalcIncreseCurse(
    const eSKILL skill_class_code, 
    const int skill_value, 
    const Character& skill_user)
{
    const BASE_ABILITYINFO* const ability_info = 
        impl::GetAbilityInfo(skill_user, eCHAR_STATE_CURSE_INCREASE);
    if (ability_info == NULL)
    {
        return 0;
    }
    
    int add_ratio = 0;
    switch (skill_class_code)
    {
    case eSKILL_PAIN:
        add_ratio = ability_info->m_iOption1;
        break;
    case eSKILL_DARK_FIRE:
        add_ratio = ability_info->m_iOption2;
        break;
    case eSKILL_ENCHANT_POISON:
        add_ratio = ability_info->m_iParam[0];
        break;
    default:
        return 0;
    }
    if (skill_value == 0 || add_ratio == 0)
    {
        return 0;
    }

    const int add_value = static_cast<int>(static_cast<float>(skill_value) * (static_cast<float>(add_ratio) / 1000.0f));
#ifdef DEBUG_ABILITY_CALC_
    if (add_value != 0)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|SkillClassCode = %d, AddRatio = %d, AddValue = %d|"), 
            __FUNCTION__, 
            skill_class_code, 
            add_ratio, 
            add_value
        );
    }
#endif // DEBUG_ABILITY_CALC_
    return add_value;
}

int CalcComboDamage(
    const Character& skill_user, 
    const Skill& skill, 
    const int skill_value)
{
    const BASE_ABILITYINFO* const combo_effect_info = 
        impl::GetComboEffectInfo(skill_user, skill, kAbilityComboSkillEffectAddDamage, 0);
    if (combo_effect_info == NULL)
    {
        return 0;
    }
    int calc_damage = 0;
    {
        const int add_value = combo_effect_info->m_iParam[0];
        if (add_value != 0)
        {
            calc_damage += add_value;
        }
        const int add_ratio = combo_effect_info->m_iParam[1];
        if (add_ratio != 0)
        {
            calc_damage += 
                static_cast<int>(static_cast<float>(skill_value) * 
                (static_cast<float>(add_ratio) / 1000.0f));
        }
#ifdef DEBUG_ABILITY_CALC_
        if (calc_damage != 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|AddValue = %d, AddRatio = %d, CalcDamage = %d|"), 
                __FUNCTION__, 
                add_value, 
                add_ratio, 
                calc_damage
            );
        }
#endif // DEBUG_ABILITY_CALC_
    }
    return calc_damage;
}

int CalcComboApplyTime(const Character& skill_user, const Skill& skill, const int state_id)
{
    const BASE_ABILITYINFO* const combo_effect_info = impl::GetComboEffectInfo(
        skill_user, skill, kAbilityComboSkillEffectAddBadStatus, state_id);
    if (combo_effect_info == NULL)
    {
        return 0;
    }
    const int add_time = combo_effect_info->m_iParam[2];
#ifdef DEBUG_ABILITY_CALC_
    if (add_time != 0)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|AddTime = %d|"), 
            __FUNCTION__, 
            add_time
        );
    }
#endif // DEBUG_ABILITY_CALC_
    return add_time;
}

// 콤보 사용으로 인한 추가 상태 성공률을 계산한다.
int CalcComboSuccessRatio(const Character& skill_user, const Skill& skill, const int state_id)
{
    const BASE_ABILITYINFO* const combo_effect_info = impl::GetComboEffectInfo(
        skill_user, skill, kAbilityComboSkillEffectAddBadStatus, state_id);
    if (combo_effect_info == NULL)
    {
        return 0;
    }
    const int add_ratio = combo_effect_info->m_iParam[1];
#ifdef DEBUG_ABILITY_CALC_
    if (add_ratio != 0)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|AddRatio = %d|"), 
            __FUNCTION__, 
            add_ratio
        );
    }
#endif // DEBUG_ABILITY_CALC_
    return add_ratio;
}

int CalcIncreseSkillDamage(const Character& skill_user, const Skill& skill, const int skill_value)
{
    // 스킬 데미지 증가 상태가 걸려 있는지 검사한다.
    const StatusManager* const status_manager = skill_user.GetStatusManager();
    if (status_manager == NULL) {
        return 0;
    }

#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    const BaseStatus* const status_inc_skill_dmg = status_manager->FindStatus(kCharStateIncreseSkillDamage);
    const BaseStatus* const status_transformation6 = status_manager->FindStatus(eCHAR_STATE_TRANSFORMATION6);

    //데미지 증가 상태가 한가지라도 있고 어빌리티라면 진행한다
    if ((status_inc_skill_dmg == NULL || status_inc_skill_dmg->IsAbilityStatus() == false)
        && (status_transformation6 == NULL || status_transformation6->IsAbilityStatus() == false))
    {
        return 0;
    }
#else  //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    const BaseStatus* const active_status = status_manager->FindStatus(kCharStateIncreseSkillDamage);
    if (active_status == NULL || active_status->IsAbilityStatus() == false) {
        return 0;
    }
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL

    // 스킬 데미지 증가 정보를 얻는다.
    const SkillScriptInfo* effect_skill_script_info = skill.GetSkillBaseInfo();
    if (effect_skill_script_info == NULL) {
        return 0;
    }

    const BASE_ABILITYINFO* const increse_damage_info = 
        effect_skill_script_info->GetAbilityInfo(kAbilityIncreseSkillDamage);
    if (increse_damage_info == NULL) {
        return 0;
    }

    // 증가값을 계산한다.
    int add_damage = 0;
    {
        const int add_value = increse_damage_info->m_iParam[0];
        if (add_value != 0)
        {
            add_damage += add_value;
        }
        const int add_ratio = increse_damage_info->m_iParam[1];
        if (add_ratio != 0)
        {
            add_damage += 
                static_cast<int>(static_cast<float>(skill_value) * 
                (static_cast<float>(add_ratio) / 1000.0f));
        }
#ifdef DEBUG_ABILITY_CALC_
        if (add_damage != 0)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|AddValue = %d, AddRatio = %d, AddDamage = %d, SkillLevel = %d|"), 
                __FUNCTION__, 
                add_value, 
                add_ratio, 
                add_damage, 
                effect_skill_script_info->m_wSkillLv
            );
        }
#endif // DEBUG_ABILITY_CALC_
    }
    return add_damage;
}

int CalcFightingEnergy(Character& skill_user, const Skill& skill, const int skill_value)
{
    const SkillScriptInfo* const skill_info = skill.GetSkillBaseInfo();
    if (skill_info == NULL)
    {
        return 0;
    }
    
    const int used_size = skill_user.GetUsedFightingEnergySize();
    if (used_size < 1)
    {
        return 0;
    }
    skill_user.SetUsedFightingEnergySize(0);

    const BASE_ABILITYINFO* const ability_info = 
        skill_info->GetAbilityInfo(eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY);
    if (ability_info == NULL)
    {
        return 0;
    }

    int add_damage = 0;
    {
        const short decrese_count = static_cast<short>(ability_info->m_iOption2);
        const bool is_use_all = (decrese_count == 0);
        if (is_use_all)
        {
            if (used_size >= 1 && used_size <= MAX_FIGHTING_ENERGY_NUM)
            {
                // 개수별 파라미터에 입력한 값만큼 증가 시킨다. 
                const int add_value = ability_info->m_iParam[used_size-1];
                if (add_value != 0)
                {
                    add_damage += add_value;
                }
#ifdef DEBUG_ABILITY_CALC_
                if (add_damage != 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|AddDamage = %d, UsedSize = %d|"), 
                        __FUNCTION__, 
                        add_damage, 
                        used_size
                    );
                }
#endif // DEBUG_ABILITY_CALC_
            }
        }
        else
        {
            // 값 증가
            const int add_value = ability_info->m_iParam[0];
            if (add_value != 0)
            {
                add_damage += add_value;
            }
            // 비율 증가
            const int add_ratio = ability_info->m_iParam[1];
            if (add_ratio != 0)
            {
                const float temp_add_ratio = add_ratio / 1000.0f;
                add_damage += static_cast<int>(skill_value * temp_add_ratio);
            }
#ifdef DEBUG_ABILITY_CALC_
            if (add_damage != 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|AddValue = %d, AddRatio = %d, AddDamage = %d, UsedSize = %d|"), 
                    __FUNCTION__, 
                    add_value, 
                    add_ratio, 
                    add_damage, 
                    used_size
                );
            }
#endif // DEBUG_ABILITY_CALC_
        }
    }
    return add_damage;
}

} // ability

} // ns_formula


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : EP1
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ns_formula { namespace ep1 {
;

//--------------------------------------------------------------------------------------------------
// internal interfaces
//
static bool IsPhysicalHit(Character* attacker, Character* target);
static int _CalcAttackPower(BOOL bBase, eATTACK_TYPE eAttackType, BOOL bMin, Character* attacker);
static int CalcFirstDamage(eATTACK_TYPE eAttackType, int iBaseAttackPower, int iSkillAttackPower,
                           short sSkillStat, float fSkillPercentDamage);
static int CalcSecondDamage(eATTACK_TYPE eAttackType, int iFirstMinDamage, int iFirstMaxDamage,
                            int iBonusDamage, LEVELTYPE attackerLevel, LEVELTYPE targetLevel,
                            int iTargetDefense,
                            float fArmorDamageRatio, float fDamageDecreaseRatio);
static int CalcBaseDamage(Character* attacker, Character* target,
                          eATTACK_TYPE eAttackType,
                          int iSkillAttackPower, float fSkillPercentDamage,
                          BYTE bySkillStatType, float fDefenseIgnore);
static int CalcMagicalDamage(Character* attacker, Character* target,
                             eATTACK_TYPE eAttackType, int iSkillAttackPower,
                             float fSkillPercentDamage, BYTE bySkillStatType);
static BOOL IsCriticalHit(eOBJECT_TYPE eObjectType, int criticalRatio, LEVELTYPE level);
static int _CalcCriticalDamage(eOBJECT_TYPE eObjectType,
                               LEVELTYPE targetLevel,
                               int iSecondDamage,
                               int iCriticalRatio,
                               int iCriticalDamageBonus,
                               float fCriticalDamagePercentBonus,
                               BOOL needMaxDamage = false);
static DAMAGETYPE _CalcDamage(BOOL bNormalAttack,
                              Character* attacker, Character* target,
                              eATTACK_TYPE eBaseAttackType,
                              eATTACK_TYPE eMagicalAttackType,
                              int iSkillAttackPower,
                              float fSkillPercentDamage,
                              WORD criticalRatioBonus,
                              BYTE* pEffect,
                              BYTE bySkillStatType,
                              float fDefenseIgnore,
                              BOOL bCritical = true);

// 방어력
static int CalcDefensePower(BOOL bBase, eATTACK_TYPE eAttackType,
                            Character* attacker, Character* target,
                            float fDefenseIgnore);

inline bool IsBypassDeffence(Character* attacker);
// Exp 배율수
inline float CalcExpRatioNum(DWORD partymemberNum);

}}; //end of namespace 'ns_formula::ep1'


namespace ns_formula { namespace ep1 {

#if SUN_CODE_BACKUP // NOTE: f101221.4L, remove unused section
DWORD ns_formula::FastRecoveryTime(DWORD delayTime, short dexterity,
                                   DWORD itemFastRecovery, DWORD skillFastRecovery)
{
    return DWORD(delayTime * (100 / (100 + dexterity / 30 + itemFastRecovery + skillFastRecovery)));
}
#endif //

inline bool ns_formula::ep1::IsBypassDeffence(Character* attacker)
{
    int byPassRatio  = attacker->GetAttr().GetBypassDeffenceRatio();

    return (random(0, 100) < byPassRatio) ? true : false;
}

int ns_formula::ep1::CalcDoubleDamage(Character* attacker, int baseDamage)
{
    int iDoubleDamageRatio = attacker->GetAttr().GetDoubleDamageRatio();

    return (random(0, 100) < iDoubleDamageRatio) ? baseDamage : 0;
}

// 플레이어 HP, MP 회복량
// vitality 은 체력이다.
// 이 함수는 회복되는 단위 수치를 구하는 함수이다. 
// 즉 회복이 일어날 때마다 일어나는 게 아니라, 특정 이벤트에 의해서 Recover값이 변경될 때
// 이 함수를 호출해야 한다.
int ns_formula::ep1::CalcHPRecover(eCHAR_TYPE char_type, DWORD vitality,
                                   BYTE byCharState, LEVELTYPE level)
{
    float recoverValue = 0;
    int recoverHP = 0;
    float fSitDownValue = 0.0f;
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    switch (char_type)
    {
    case eCHAR_DRAGON       :
        recoverValue = 8;
        fSitDownValue = 3;
        break;
    case eCHAR_BERSERKER    :
        recoverValue = 10;
        fSitDownValue = 2;
        break;
    case eCHAR_SHADOW       :
        recoverValue = 8;
        fSitDownValue = 1;      // 섀도우는 임시로...
        break;
    case eCHAR_VALKYRIE     :
        recoverValue = 7;
        fSitDownValue = 1.5;
        break;
    case eCHAR_ELEMENTALIST :
        recoverValue = 6;
        fSitDownValue = 3;
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        recoverValue = 8;
        fSitDownValue = 1;
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        recoverValue = 11;
        fSitDownValue = 2;
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        recoverValue = 8;
        fSitDownValue = 3;
        break;
    default:
        {
            SASSERT(false, "Wrong Char_Type in CalcHPRecover()");
            return 0;
        }
    }

#else
    switch (char_type)
    {
    case eCHAR_DRAGON       :
        recoverValue = 4;
        fSitDownValue = 3;
        break;
    case eCHAR_BERSERKER    :
        recoverValue = 5;
        fSitDownValue = 2;
        break;
    case eCHAR_SHADOW       :
        recoverValue = 4;
        fSitDownValue = 1;      // 섀도우는 임시로...
        break;
    case eCHAR_VALKYRIE     :
        recoverValue = 3.5f;
        fSitDownValue = 1.5;
        break;
    case eCHAR_ELEMENTALIST :
        recoverValue = 3;
        fSitDownValue = 3;
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        recoverValue = 4;
        fSitDownValue = 1;
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        recoverValue = 4;
        fSitDownValue = 1;
        break;

    default:
        {
            SASSERT(false, "Wrong Char_Type in CalcHPRecover()");
            return 0;
        }
    }
#endif //
    if (byCharState == eCHAR_CONDITION_STANDUP)
    {   //1. 서있을 때
        recoverHP = (int)(((vitality / recoverValue + level / 5.0f) - 2) * 2.0f);
    }
    else if (byCharState == eCHAR_CONDITION_SITDOWN)
    {   //2. 앉아 있을 때
        recoverHP = (int)((((vitality / recoverValue) * 2 + (level / 5.0f) * 2) - 2) * fSitDownValue);
    }

    if (recoverHP < 0) {
        recoverHP = 1;
    }
    return recoverHP;
}

//SD 자동 회복 공식
int ns_formula::ep1::CalcSDRecover(eCHAR_TYPE char_type,
                                   eCHAR_CONDITION byCharState, BYTE byCharAction,
                                   LEVELTYPE level)
{
    if (CHAR_ACTION_CONDITION_FIGHTING & byCharAction) {
        return 0;
    }

    DWORD dwMaxSD = CalcSD(char_type, level);

    if (CHAR_ACTION_CONDITION_MOVING & byCharAction) {
        return (dwMaxSD/30/2) + (level/5);
    }
    if (eCHAR_CONDITION_SITDOWN == byCharState) {
        return (dwMaxSD/30*2) + (level/5*2);
    }
    return (dwMaxSD/30) + (level/5);
}

int ns_formula::ep1::CalcMPRecover(eCHAR_TYPE char_type, DWORD spirit, BYTE byCharState)
{
    int recoverValue = 0;

    float recoverRatio = 1.0f;

    // 앉기 상태일 경우 회복량이 틀리다.
    if (byCharState == eCHAR_CONDITION_SITDOWN) {
        recoverRatio = 3.0f;
    }
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    switch (char_type)
    {
        case eCHAR_DRAGON       : recoverValue = 2; break;

        // 버서커는 타격계 회복이라.. 공격/맞을때 MP가 회복된다.
        case eCHAR_BERSERKER    : return -1;    // 18
        case eCHAR_SHADOW       : recoverValue = 2; break;

        case eCHAR_VALKYRIE     : recoverValue = 2; break;
        case eCHAR_ELEMENTALIST : recoverValue = 2; break;
        case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            recoverValue = 2; 
            break;
        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            recoverValue = 2;
            break;
        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            recoverValue = 2;
            break;

        default:
        {
            SASSERT(false, "Wrong Char_Type in CalcMPRecover()");
            return 0;
        }
    }
#else
    switch (char_type)
    {
        case eCHAR_DRAGON       : recoverValue = 1; break;

        // 버서커는 타격계 회복이라.. 공격/맞을때 MP가 회복된다.
        case eCHAR_BERSERKER    : return -1;    // 18
        case eCHAR_SHADOW       : recoverValue = 1; break;

        case eCHAR_VALKYRIE     : recoverValue = 1; break;
        case eCHAR_ELEMENTALIST : recoverValue = 1; break;
        case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            recoverValue = 1; 
            break;
        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            recoverValue = 1;
            break;

        default:
        {
            SASSERT(false, "Wrong Char_Type in CalcMPRecover()");
            return 0;
        }
    }
#endif
    int recoverMP = (int)(spirit * recoverRatio / recoverValue / 5) + 1;

    return recoverMP;
}

DWORD ns_formula::ep1::CalcMPRecoverByAttack(eCHAR_TYPE char_type, DWORD spirit,
                                             DWORD skillRecoverPercent, DWORD skillRequireMP)
{
    DWORD recoverValue = 0;

    switch (char_type)
    {
        case eCHAR_DRAGON       : return 0;

        // 버서커/섀도우는 타격계 회복이라.. 공격/맞을때 MP가 회복된다.
        case eCHAR_BERSERKER    : recoverValue = 18; break;
        case eCHAR_VALKYRIE     : return 0;
        case eCHAR_ELEMENTALIST : return 0;
        case eCHAR_SHADOW       : return 0;
        case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            return 0;
        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            return 0;
        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            return 0;

        default:
        {
            SASSERT(false, "Wrong Char_Type in CalcMPRecover()");
            return 0;
        }
    }

    // MP/SP 회복양 = (징신력/ 캐릭터별 상수) * 퍼센트 회복 스킬 + 스킬 요구 SP,MP/5 + 3
    DWORD regenMP = spirit / recoverValue * skillRecoverPercent / 100 + skillRequireMP / 5 + 1;

    return regenMP;
}


DWORD ns_formula::ep1::CalcMPRecoverByAttacked(eCHAR_TYPE char_type, DWORD spirit, DAMAGETYPE damage)
{
    DWORD recoverValue = 0;

    switch (char_type)
    {
    case eCHAR_DRAGON       : return 0;

        // 버서커/섀도우는 타격계 회복이라.. 공격/맞을때 MP가 회복된다.
    case eCHAR_BERSERKER    : recoverValue = 18; break;
    case eCHAR_VALKYRIE     : return 0;
    case eCHAR_ELEMENTALIST : return 0;
    case eCHAR_SHADOW       : return 0;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return 0;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return 0;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return 0;
    default:
        {
            SASSERT(false, "Wrong Char_Type in CalcMPRecover()");
            return 0;
        }
    }

    // (정신력/캐릭터별 상수) + 실제 받은 데미지 / 30 + 3
    DWORD regenMP = spirit / recoverValue + damage / 30 + 1;

    return regenMP;
}

// HP, MP 회복량(몬스터)
int ns_formula::ep1::CalcNPCHPRecover(DWORD maxHP, DWORD recoverRatio)
{
    return (int)(maxHP * 0.005f * recoverRatio);
}

int ns_formula::ep1::CalcNPCMPRecover(DWORD maxMP, DWORD recoverRatio)
{
    return (int)(maxMP * 0.01f * recoverRatio);
}

// NPC SD 회복 공식
int ns_formula::ep1::CalcNPCSDRecover(DWORD maxSD)
{
    return (int)(maxSD * 0.15f);
}

// Exp 배율수
inline float ns_formula::ep1::CalcExpRatioNum(DWORD partymemberNum)
{
    return (float)partymemberNum;
}

EXPTYPE ns_formula::ep1::GetKillExpOfParty(
    BYTE /*room_bonus_type*/,
    DWORD party_member_num,
    LEVELTYPE party_total_lv,
    LEVELTYPE attacker_lv, 
    LEVELTYPE target_lv,
    float npc_grade_exp_ratio,
    DWORD attack_damage,
    DWORD target_max_hp,
    eZONETYPE zone_type,
    Player* player)
{
    if (party_member_num == 0 || target_max_hp == 0 || party_total_lv == 0) 
    {
        return 0;
    }
    
#if defined(_KOREA) || defined(_TAIWAN) || defined(_JAPAN) || \
    defined(_USA) || defined(_GLOBAL) || defined(_RUSSIA)
    const float level_diff_ratio = GetExpLevelRatioKorea(attacker_lv - target_lv);
#elif defined(_CHINA)
    const float level_diff_ratio = GetExpLevelRatioChina(attacker_lv - target_lv);
#else
    const float level_diff_ratio = GetExpLevelRatio(attacker_lv - target_lv);
#endif
    
    float script_ratio = 1.0f;
    {
        const BASE_FORMULA_RATIOINFO* formula_ratio = NULL;
        FormulaRatioParser* const formula_ratio_parser = FormulaRatioParser::Instance();
        switch (zone_type)
        {
        case eZONETYPE_FIELD:
        case eZONETYPE_VILLAGE:
        case eZONETYPE_DOMINATION_FIELD:
        case eZONETYPE_SPA_FIELD:           //_NA_008069_20150210_ADD_SPA_SYSTEM
            formula_ratio = formula_ratio_parser->GetFormulaRatio(RATIO_TYPE_EXP_FIELD);
            break;
        case eZONETYPE_CHUNTING:
        case eZONETYPE_HUNTING:
            formula_ratio = formula_ratio_parser->GetFormulaRatio(RATIO_TYPE_EXP_HUNTING);
            break;
        case eZONETYPE_MISSION:
            formula_ratio = formula_ratio_parser->GetFormulaRatio(RATIO_TYPE_EXP_MISSION);
            break;
        default:
            formula_ratio = formula_ratio_parser->GetFormulaRatio(RATIO_TYPE_EXP_FIELD);
            break;
        }
        if (formula_ratio != NULL) 
        {
            script_ratio = formula_ratio->m_fRatio;
        }
    }

    EXPTYPE decision_exp = 0;
    {
        float seed_exp = (target_lv + 45) * 0.03f;
        {
            const float seed_exp_x3 = seed_exp * seed_exp * seed_exp;
            const float min_exp = EXPTYPE(seed_exp_x3 * seed_exp + 3 * seed_exp_x3 - 5) *
                npc_grade_exp_ratio * script_ratio;
            
            const StatusField& status = player->GetStatusManager()->Status;
            if (status.FIELD & status.ENABLE_GM_MAX_EXP) 
            {
                decision_exp = EXPTYPE(min_exp);
            }
            else 
            {
                decision_exp = (EXPTYPE)dRandom(min_exp / 2, min_exp);
            }
        }
    }
    
    const EXPTYPE damage_exp = (attack_damage * decision_exp) / target_max_hp;

    float add_exp_ratio = 1.0f;
    {
        if (eZONETYPE_FIELD == zone_type || eZONETYPE_DOMINATION_FIELD == zone_type)
        {
            add_exp_ratio = GetFieldExpPartyBonusFactor(party_member_num);
        }
        else
        {
            add_exp_ratio = GetRoomExpPartyBonusFactor(party_member_num);
        }
    }
    
#if defined(_CHINA)
    const float party_ratio = CalcExpRatioNum(party_member_num);
    const float level_ratio = party_ratio * attacker_lv / party_total_lv;
#else
    const LEVELTYPE party_lv = (LEVELTYPE)(party_total_lv / party_member_num);
    const float level_ratio = GetPartyLevelExpRatio(attacker_lv - party_lv);
#endif

    EXPTYPE result_exp = (EXPTYPE)(damage_exp * level_diff_ratio * add_exp_ratio * level_ratio);
    if (result_exp <= 0) 
    {
        result_exp = 1;
    }
    return result_exp;
}

//----------------------------------------------------------------------------------------------------
//  데미지 계산 관련(기초 공식들)
//----------------------------------------------------------------------------------------------------

// 공격 성공 체크
static bool ns_formula::ep1::IsPhysicalHit(Character* attacker, Character* target)
{
    const Attributes& attacker_attrs = attacker->GetAttr();
    int phy_attack_rate = attacker_attrs.GetPhysicalAttackRate();
    if (phy_attack_rate <= 0) 
    {
        phy_attack_rate = 1;
    }
    // 레벨에 따른 블록 가중
    float fLevelWeight;
    {
        int diff_level = int(attacker->GetDisplayLevel()) - target->GetDisplayLevel();
        fLevelWeight = 1.0f - diff_level * 0.05f;
    };

    // 블록 확률
    int iAvoidValue;
    {   // (WAVERIX) (090608) Minor change 'Convert to float on pre-calculation'
        int calced = int(10000 * target->GetPhysicalAvoidValue() / float(phy_attack_rate));
        iAvoidValue = min(10000, calced);
    }
    //int iAvoidValue = min(10000, (int)(10000* target->GetPhysicalAvoidValue() / nPhysicalAttackRate));

    iAvoidValue = int(iAvoidValue * fLevelWeight);
    if (iAvoidValue < 500) {
        iAvoidValue = 500;
    }
    else if (9000 < iAvoidValue) {
        iAvoidValue = 9000;
    }

    int iSuccessValue = random(0, 10000);
    if (iSuccessValue <= iAvoidValue) {
        return false;
    }
    return true;
}

// (WAVERIX) (090602) (CHANGES) internal function, change name '' -> '_CalcAttackPower'
static int ns_formula::ep1::_CalcAttackPower(BOOL bBase, eATTACK_TYPE eAttackType, BOOL bMin,
                                             Character* attacker)
{
    int iBaseAttackPower = 0;

    const Attributes& rAttackerAttr = attacker->GetAttr();
    // 기본 공격력(Base + Option)
    if (bBase)
    {
        if (eAttackType == eATTACK_TYPE_MELEE)
        {
            iBaseAttackPower = rAttackerAttr.GetOptionPhysicalAttPower();
            if (bMin)   iBaseAttackPower += rAttackerAttr.GetBaseMeleeMinAttPower();
            else        iBaseAttackPower += rAttackerAttr.GetBaseMeleeMaxAttPower();
        }
        else if (eAttackType == eATTACK_TYPE_RANGE)
        {
            iBaseAttackPower = rAttackerAttr.GetOptionPhysicalAttPower();
            if (bMin)   iBaseAttackPower += rAttackerAttr.GetBaseRangeMinAttPower();
            else        iBaseAttackPower += rAttackerAttr.GetBaseRangeMaxAttPower();
        }
        else
        {
            iBaseAttackPower = rAttackerAttr.GetOptionMagicAttPower();
            if (bMin)   iBaseAttackPower += rAttackerAttr.GetBaseMagicMinAttPower();
            else        iBaseAttackPower += rAttackerAttr.GetBaseMagicMaxAttPower();
        }
    }
    // 속성 공격력(Magical)
    else
    {
        iBaseAttackPower = rAttackerAttr.GetMagicalAttackPower(eAttackType);
    }

    iBaseAttackPower += (int)(iBaseAttackPower * rAttackerAttr.GetAttackIncRate() / 100.0f);

    return iBaseAttackPower;
}

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
// 타겟 방어력
static int ns_formula::ep1::CalcDefensePower(BOOL bBase, eATTACK_TYPE eAttackType,
                                             Character* attacker, Character* target,
                                             float fDefenseIgnore)
{
    int iTargetDefense = 0;

    const Attributes& rAttackerAttr = attacker->GetAttr();
    const Attributes& rTargetAttr = target->GetAttr();
    // 기본 방어력(Base + Option)
    if (bBase)
    {
        if (eAttackType == eATTACK_TYPE_MELEE)
        {
            iTargetDefense =  rTargetAttr.GetBaseMeleeDefPower();
            iTargetDefense += rTargetAttr.GetOptionPhysicalDefPower();
        }
        else if (eAttackType == eATTACK_TYPE_RANGE)
        {
            iTargetDefense =  rTargetAttr.GetBaseRangeDefPower();
            iTargetDefense += rTargetAttr.GetOptionPhysicalDefPower();
        }
        else
        {
            iTargetDefense =  rTargetAttr.GetBaseMagicDefPower();
            iTargetDefense += rTargetAttr.GetOptionMagicDefPower();
        }
    }
    // 속성 방어력(Magical)
    else
    {
        iTargetDefense = rTargetAttr.GetMagicalDefense(eAttackType);
    }

    // 추가 방어력
    iTargetDefense += rTargetAttr.GetBonusDefense(eAttackType);

    // 방어력 증가율, 감소율
    int iIncreaseDefence = int(iTargetDefense * rTargetAttr.GetDefenseIncRate() / 100.0f);
    int iDecreaseDefense = 0;
    if (attacker) {
        iDecreaseDefense = int(iTargetDefense * rAttackerAttr.GetReduceDefenseRate(eAttackType) / 100.0f);
    }

    iTargetDefense = iTargetDefense + iIncreaseDefence - iDecreaseDefense;

    // 방어력 무시
    if (0 < fDefenseIgnore &&
            fDefenseIgnore <= 1.0f)
    {
       iTargetDefense -= (int)(iTargetDefense * fDefenseIgnore);
    }

    return iTargetDefense;
}

// 1차 데미지 계산
static int ns_formula::ep1::CalcFirstDamage(eATTACK_TYPE eAttackType,
                                            int iBaseAttackPower, int iSkillAttackPower,
                                            short sSkillStat, float fSkillPercentDamage)
{
    float fRate = 1.0f;
#if WAVERIX_CODE_BACKUP
    if (eAttackType == eATTACK_TYPE_MELEE || eAttackType == eATTACK_TYPE_RANGE) {
        fRate = 1.0;
    }
    else {
        fRate = 1.0;
    }
#endif

    int iFirstDamage = (int)((iBaseAttackPower + iSkillAttackPower + sSkillStat) *
                             (fRate + fSkillPercentDamage));

    iFirstDamage = max(0, iFirstDamage);

    return iFirstDamage;
}

// 2차 데미지 계산
static int ns_formula::ep1::CalcSecondDamage(eATTACK_TYPE eAttackType,
                                             int iFirstMinDamage, int iFirstMaxDamage,
                                             int iBonusDamage,
                                             LEVELTYPE attackerLevel, LEVELTYPE targetLevel,
                                             int iTargetDefense,
                                             float fArmorDamageRatio, float fDamageDecreaseRatio)
{
    int iFirstDamage = (int)dRandom(iFirstMinDamage, iFirstMaxDamage);
    int iSecondDamage = 0;
    float fRatio = 0;

    iTargetDefense = max(0, iTargetDefense);

    // (WAVERIX) (090602) (NOTE) 뭐 하자는 건가? AttackType이 구분되어 있지만, 정리해 보니까
    // 둘 모두 동일한 계산 로직을 가지고 있다. 뭐... 장기적으로 공식이 구분될 가능성을 고려한
    // 다면 필요한 형태일 지도 모르겠지만... 정리하고 나서야 동일하다는 것을 알게 되니, 어이만
    // 없어라...
    // (추가) 하나만 삐긋해도 문제가 크게 발생하는 정말 위태위태한...

    // 물리 공격
    if (eAttackType == eATTACK_TYPE_MELEE || eAttackType == eATTACK_TYPE_RANGE)
    {
        fRatio =  (float)(80 + targetLevel * 6 - iTargetDefense) /
                  (float)(750 + targetLevel * 2) +
                  fDamageDecreaseRatio +
                  (int(attackerLevel) - targetLevel) * 0.03f;

        if (fRatio < 0.1f) {
            fRatio = 0.1f;
        }

        iSecondDamage = int(iFirstDamage * min(1.0f, fRatio));
        iSecondDamage = int(iSecondDamage * fArmorDamageRatio);
    }
    // 마법 공격
    else
    {
        fRatio =  (float)(80 + targetLevel * 6 - iTargetDefense) /
                  (float)(750 + targetLevel * 2) +
                  fDamageDecreaseRatio +
                  (int(attackerLevel) - targetLevel) * 0.03f;

        if (fRatio < 0.1f) {
            fRatio = 0.1f;
        }
        iSecondDamage = (int)(iFirstDamage * min(1.0f, fRatio));
        iSecondDamage = (int)(iSecondDamage * fArmorDamageRatio);
    }

    iSecondDamage += iBonusDamage;
    if (iSecondDamage < 0) {
        iSecondDamage = 0;
    }
    return iSecondDamage;
}

// 기본 데미지
static int ns_formula::ep1::CalcBaseDamage(Character* attacker, Character* target,
                                           eATTACK_TYPE eAttackType,
                                           int iSkillAttackPower, float fSkillPercentDamage,
                                           BYTE bySkillStatType, float fDefenseIgnore)
{
    // 기본공격력
    int iBaseMinAttackPower = _CalcAttackPower(true, eAttackType, true, attacker);
    int iBaseMaxAttackPower = _CalcAttackPower(true, eAttackType, false, attacker);

    const Attributes& rAttackerAttr = attacker->GetAttr();
    const Attributes& rTargetAttr = target->GetAttr();

    int incBaseMinDamageRatio = 0; /* BaseMinAttackPower -= BaseMinAttackPower * MinDamage 감소률 */
    {
        if ((incBaseMinDamageRatio = rAttackerAttr.GetRatioIncreaseMinDamage()) > 0) {
            iBaseMinAttackPower -= int(iBaseMinAttackPower * (incBaseMinDamageRatio / 100.0f));
        }
        iBaseMinAttackPower += rAttackerAttr.GetItemIncreaseMinDamage();
    }

    int incBaseMaxDamageRatio = 0; /* BaseMaxAttackPower -= BaseMaxAttackPower * MaxDamage감소률 */
    {
        if ((incBaseMaxDamageRatio = rAttackerAttr.GetRatioIncreaseMaxDamage()) > 0) {
            iBaseMaxAttackPower -= int(iBaseMaxAttackPower * (incBaseMaxDamageRatio / 100.0f));
        }
        iBaseMaxAttackPower += rAttackerAttr.GetItemIncreaseMaxDamage();
    }

    int etherDamageRatio = rAttackerAttr.GetEhterDamageRatio();
    if (etherDamageRatio > 0)
    {
        const float tmpEtherDamageRatio = float(etherDamageRatio) / 100.0f;
        iBaseMinAttackPower = int(iBaseMaxAttackPower * (0.95f + tmpEtherDamageRatio));
        iBaseMaxAttackPower = int(iBaseMaxAttackPower * (1.05f + tmpEtherDamageRatio));
    }

    // 스킬공격력
    iSkillAttackPower += rAttackerAttr.GetSkillAttackPower();

    // 스킬숙련도
    short sSkillStat = 0;
    if (bySkillStatType == 1)          sSkillStat = rAttackerAttr.GetExperty1();
    else if (bySkillStatType == 2)     sSkillStat = rAttackerAttr.GetExperty2();

    // 타겟방어력
    if (IsBypassDeffence(attacker))
        fDefenseIgnore = 1.0f;

    int iTargetDefense = CalcDefensePower(true, eAttackType, attacker, target, fDefenseIgnore);

    // 스킬퍼센트추가데미지
    fSkillPercentDamage += (float)(rAttackerAttr.GetSkillPercentDamage() / 100.0f);

    // 1차 데미지 계산
    int iFirstMinDamage = CalcFirstDamage(eAttackType, iBaseMinAttackPower, iSkillAttackPower,
                                          sSkillStat, fSkillPercentDamage);
    int iFirstMaxDamage = CalcFirstDamage(eAttackType, iBaseMaxAttackPower, iSkillAttackPower,
                                          sSkillStat, fSkillPercentDamage);

    eARMOR_TYPE targetArmorType = target->GetArmorType();
    eMELEE_TYPE meleeType = attacker->GetMeleeType();

    // 보너스 데미지, 아머 타입별 데미지 적용률
    int iBonusDamage = rAttackerAttr.GetBonusDamage(targetArmorType) -
                       rTargetAttr.GetReduceDamage(eAttackType);
    float fBonusPercentDamage = rAttackerAttr.GetBonusPercentDamage(targetArmorType) / 100.0f;

    float fArmorDamageRatio =
        ArmorDamageParser::Instance()->FindDamageRatio(targetArmorType, meleeType);
    fArmorDamageRatio = fArmorDamageRatio / 100.f + fBonusPercentDamage;
    
    // 2차 데미지 계산
    int iSecondDamage = 0;
    StatusField& rStatus = attacker->GetStatusManager()->Status;
    {
        int tempDamage = iFirstMinDamage;
        if (rStatus.FIELD & rStatus.ENABLE_GM_MAX_DAMAGE)
            tempDamage = iFirstMaxDamage;

        float fDamageDecreaseRatio = 0.70f;
        
        if (false != target->IsEqualObjectKind(PLAYER_OBJECT) &&
            false != attacker->IsEqualObjectKind(PLAYER_OBJECT))   //PVP 시 데미지 차감 공식
        {
            fDamageDecreaseRatio = 0.30f;
        }

        iSecondDamage = CalcSecondDamage(eAttackType,
                                         tempDamage, iFirstMaxDamage, iBonusDamage,
                                         attacker->GetDisplayLevel(), target->GetDisplayLevel(),
                                         iTargetDefense, fArmorDamageRatio, fDamageDecreaseRatio);
    }
    // 최소보장을 체크한다.
    if (iSecondDamage <= (int)(iFirstMinDamage / 20))
    {
        int damageBound = iFirstMinDamage / 40 + 1;
        iSecondDamage = damageBound + (int)dRandom(0, damageBound);
    }

    return iSecondDamage;
}

// 속성 데미지
static int ns_formula::ep1::CalcMagicalDamage(Character* attacker, Character* target,
                                              eATTACK_TYPE eAttackType, int iSkillAttackPower,
                                              float fSkillPercentDamage, BYTE bySkillStatType)
{
    // 속성공격력
    int iBaseAttackPower = _CalcAttackPower(false, eAttackType, true, attacker);

    const Attributes& rAttackerAttr = attacker->GetAttr();
    const Attributes& rTargetAttr = target->GetAttr();

    // (WAVERIX) (090602) (NOTE) FileVersion 182에서 Line 676과 일부 코드 중복 발생 중...
    int iBaseMinAttackPower = 0, iBaseMaxAttackPower = 0;
    int etherDamageRatio = rAttackerAttr.GetEhterDamageRatio();
    if (etherDamageRatio > 0)
    {
        const float tmpEtherDamageRatio = etherDamageRatio / 100.0f;
        iBaseMinAttackPower = int(iBaseAttackPower * (0.95f + tmpEtherDamageRatio));
        iBaseMaxAttackPower = int(iBaseAttackPower * (1.05f + tmpEtherDamageRatio));
    }
    else
    {
        iBaseMinAttackPower = iBaseMaxAttackPower = iBaseAttackPower;
    }

    // 스킬퍼센트추가데미지
    fSkillPercentDamage += (float)(rAttackerAttr.GetSkillPercentDamage() / 100.0f);

    // 1차 데미지 계산
    int iFirstMinDamage = CalcFirstDamage(eAttackType, iBaseMinAttackPower, 0, 0, fSkillPercentDamage);
    int iFirstMaxDamage = CalcFirstDamage(eAttackType, iBaseMaxAttackPower, 0, 0, fSkillPercentDamage);

    // 보너스 데미지, 아머 타입별 데미지 적용률
    int iBonusDamage = -rTargetAttr.GetReduceDamage(eAttackType);
    int iTargetDefense = CalcDefensePower(false, eAttackType, attacker, target, 0);

    // 2차 데미지 계산
    int iSecondDamage = 0;
    StatusField& rStatus = attacker->GetStatusManager()->Status;
    int tempDamage = iFirstMinDamage;
    if (rStatus.FIELD & rStatus.ENABLE_GM_MAX_DAMAGE)
        tempDamage = iFirstMaxDamage;

        float fDamageDecreaseRatio = 0.70f;

        if (false != target->IsEqualObjectKind(PLAYER_OBJECT) &&
            false != attacker->IsEqualObjectKind(PLAYER_OBJECT))
        {
            fDamageDecreaseRatio = 0.30f;
        }
        iSecondDamage = CalcSecondDamage(eAttackType, tempDamage, iFirstMaxDamage, iBonusDamage,
                                         attacker->GetDisplayLevel(), target->GetDisplayLevel(),
                                         iTargetDefense, 1.0f, fDamageDecreaseRatio);

    return iSecondDamage;
}

static BOOL ns_formula::ep1::IsCriticalHit(eOBJECT_TYPE eObjectType, int criticalRatio, LEVELTYPE level)
{
    if ((eObjectType & PLAYER_OBJECT) == PLAYER_OBJECT)
    {
        int block_factor = 30 + level * 4;
        int block_ratio = (int)dRandom(1, block_factor);

        if (block_ratio <= criticalRatio)
        {
            return true;
        }
    }
    else 
    {
        //몬스터의 경우는 NPCInfo.txt에서 확률값(criticalRatio)을 얻어서 판정
        if ((int)random(1, 100) <= criticalRatio) {
            return true;
        }
    }

    return false;
}

// 크리티컬 데미지 계산
// (WAVERIX) (090602) (NOTE) internal function
static int ns_formula::ep1::_CalcCriticalDamage(eOBJECT_TYPE eObjectType,
                                                LEVELTYPE targetLevel,
                                                int iSecondDamage,
                                                int iCriticalRatio,
                                                int iCriticalDamageBonus,
                                                float fCriticalDamagePercentBonus,
                                                BOOL needMaxDamage)
{
    if (!IsCriticalHit(eObjectType, iCriticalRatio, targetLevel))
        return 0;   

    int iCriticalDamage = 0;
    if (needMaxDamage != false) {
        iCriticalDamage = (int)(iSecondDamage * 1.2f + iSecondDamage * 0.5f);
    }
    else {
        iCriticalDamage = (int)(iSecondDamage * 1.2f + (int)dRandom(0, iSecondDamage) * 0.5f);
    }
    // 크리티컬 데미지(퍼센트) 보너스
    iCriticalDamage = (int)(iCriticalDamage * (1.0f + fCriticalDamagePercentBonus));
    iCriticalDamage += iCriticalDamageBonus;

    return iCriticalDamage;
}

//----------------------------------------------------------------------------------------------------
//  데미지 계산(일반+스킬)
//----------------------------------------------------------------------------------------------------
// (WAVERIX) (090602) (CHANGES) internal function
static DAMAGETYPE ns_formula::ep1::_CalcDamage(BOOL bNormalAttack,
                                               Character* attacker, Character* target,
                                               eATTACK_TYPE eBaseAttackType,
                                               eATTACK_TYPE eMagicalAttackType,
                                               int iSkillAttackPower,
                                               float fSkillPercentDamage,
                                               WORD criticalRatioBonus,
                                               BYTE* pEffect,
                                               BYTE bySkillStatType,
                                               float fDefenseIgnore,
                                               BOOL bCritical)
{
    // 일반 공격일 경우는 블록율을 계산하고 스킬 공격일 경우는 계산하지 않음!!
    if (bNormalAttack && !IsPhysicalHit(attacker, target)) {
        return 0;
    }
    int iResultDamage = 0;

    // 기본 데미지
    int iBaseDamage = CalcBaseDamage(attacker, target,
                                     eBaseAttackType, iSkillAttackPower,
                                     fSkillPercentDamage, bySkillStatType, fDefenseIgnore);
    iResultDamage += iBaseDamage;

    // 속성 데미지
    int iMagicalDamage = CalcMagicalDamage(attacker, target,
                                           eMagicalAttackType, iSkillAttackPower,
                                           fSkillPercentDamage, bySkillStatType);
    iResultDamage += iMagicalDamage;

    const Attributes& rAttackerAttr = attacker->GetAttr();

    if (bCritical != false)
    {   // 크리티컬 확률, 크리티컬 데미지 보너스, 크리티컬 데미지 퍼센트 보너스
        int iCriticalRatio = criticalRatioBonus;
        if (eBaseAttackType == eATTACK_TYPE_MELEE || eBaseAttackType == eATTACK_TYPE_RANGE) {
            iCriticalRatio += rAttackerAttr.GetCriticalRatioChange();
        }
        else {
            iCriticalRatio += rAttackerAttr.GetMagicCriticalRatio();
        }
        int iCriticalDamageBonus = rAttackerAttr.GetCriticalDamageBonus();
        float fCriticalDamagePercentBonus = rAttackerAttr.GetRatioCriticalDamageBonus() / 100.0f;

        // 크리티컬 데미지 계산
        int iCriticalDamage = 0;
        StatusField& rStatus = attacker->GetStatusManager()->Status;
        {
            BOOLEAN needMaxDamage = false;
            if (rStatus.FIELD & rStatus.ENABLE_GM_MAX_DAMAGE) {
                needMaxDamage = true;
            }
            iCriticalDamage = _CalcCriticalDamage(attacker->GetObjectType(),
                target->GetDisplayLevel(), iResultDamage, iCriticalRatio, iCriticalDamageBonus,
                fCriticalDamagePercentBonus, needMaxDamage);
        }

        if (iCriticalDamage && pEffect != 0)
        {   
            *pEffect |= SKILL_EFFECT_CRITICAL;
        }
        else if (pEffect != 0)
        {   
            *pEffect &= ~SKILL_EFFECT_CRITICAL;
        }
        else
        {
        //  DISPMSG("CalcSkillDamage!\n");
        }

        // 최종 데미지
        if (iCriticalDamage)
        {
            iResultDamage = iCriticalDamage;
        }
    }

    // 더블 데미지: 확율로 데미지가 두배로 증가한다.
    int iDoubleDamage = CalcDoubleDamage(attacker, iResultDamage);
    if (iDoubleDamage && pEffect != 0)
    {
        iResultDamage += iDoubleDamage;
        *pEffect |= SKILL_EFFECT_DOUBLEDAMAGE;
    }
    else if (pEffect != 0)
    {
        *pEffect &= ~SKILL_EFFECT_DOUBLEDAMAGE;
        //DISPMSG("CalcSkillDamage!\n");
    }

    // 추가 데미지가 있다면..
    iResultDamage += rAttackerAttr.GetAddDamage();

    return iResultDamage;
}

//==================================================================================================
//  외부에서 쓰는 함수(일반, 스킬)
//==================================================================================================
DAMAGETYPE ns_formula::ep1::CalcNormalDamage(Character* attacker, Character* target,
                                             eATTACK_TYPE eBaseAttackType,
                                             eATTACK_TYPE eMagicalAttackType,
                                             WORD criticalRatioBonus,
                                             BYTE* pCritical,
                                             float fDefenseIgnore)
{
    DAMAGETYPE wDamage = _CalcDamage(true, attacker, target,
                                     eBaseAttackType, eMagicalAttackType,
                                     0, 0, criticalRatioBonus, pCritical, 0, fDefenseIgnore);
    return wDamage;
}

DAMAGETYPE ns_formula::ep1::CalcSkillDamage(Character* attacker, Character* target,
                                            eATTACK_TYPE eAttackType, int iSkillAttackPower,
                                            float fSkillPercentDamage, WORD criticalRatioBonus,
                                            BYTE* pCritical,
                                            BYTE bySkillStatType,
                                            BOOL bCritical)
{
    eATTACK_TYPE eBaseAttackType = eATTACK_TYPE_MAGIC;
    if (eAttackType == eATTACK_TYPE_MELEE || eAttackType == eATTACK_TYPE_RANGE)
        eBaseAttackType = eAttackType;

    DAMAGETYPE wDamage = _CalcDamage(false, attacker, target,
                                     eBaseAttackType, eAttackType,
                                     iSkillAttackPower, fSkillPercentDamage,
                                     criticalRatioBonus, pCritical, bySkillStatType, 0, bCritical);
    return wDamage;
}

#endif //_NA001605_EP2_ATTRIBUTES_RENEWAL_

//_NA_001231_20081216_ADD_OPTION_KIND
bool ns_formula::ep1::IsStatusHit(
    Character* const attacker, 
    Character* const target, 
    const int success_ratio, 
    const int state_id, 
    const Skill* const skill)
{
    if (attacker == NULL || target == NULL)
    {
        return false;
    }

#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    // 대상과의 관계에 상관없이 스크립트 입력 확률을 기준으로 검사한다.
#else
    if (attacker->IsFriend(target) == USER_RELATION_FRIEND)
    {
        return true;
    }
#endif

    float calc_ratio = (float)success_ratio;
    
    // 연계 스킬 사용에 의한 성공 확률 증가를 검사한다.
    if (skill != NULL)
    {
        const int combo_ratio = 
            ns_formula::ability::CalcComboSuccessRatio(*attacker, *skill, state_id);
        if (combo_ratio != 0)
        {
            calc_ratio += (float)combo_ratio;
        }
    }

    if (attacker->IsEqualObjectKind(PLAYER_OBJECT) && target->IsEqualObjectKind(PLAYER_OBJECT))
    {
        calc_ratio = (float)success_ratio / 10.0f;
    }
    else
    {
        float grade_ratio = 1.0f;
        if (target->IsEqualObjectKind(NPC_OBJECT))
        {
            const NPC* const npc = static_cast<const NPC*>(target);
            grade_ratio = GetStatusRatioAsNPCGrade(npc->GetGrade());

#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
            switch(state_id)
            {
            case eCHAR_STATE_POISON:
            case eCHAR_STATE_POISON2:
            case kCharStatePoison3:
            case eCHAR_STATE_WOUND:
            case eCHAR_STATE_FIRE_WOUND:
            case eCHAR_STATE_FIRE_WOUND2:
            case eCHAR_STATE_PAIN:
            case eCHAR_STATE_PAIN2:
            case eCHAR_STATE_PHOENIX_BURN:
            case eCHAR_STATE_ELECTRICSHOCK:
            case eCHAR_STATE_HP_SUCTION:
            case eCHAR_STATE_MP_SUCTION:
                {
                    grade_ratio = 1.0f;
                } break;
            }
#else
    #ifdef _NA_000000_20130529_STATUS_EXCEPTIONS_AS_NPC_GRADE
            switch(state_id)
            {
            case eCHAR_STATE_POISON:
            case eCHAR_STATE_WOUND:
            case eCHAR_STATE_FIRE_WOUND:
            case eCHAR_STATE_FIRE_WOUND2:
            case eCHAR_STATE_PAIN:
            case eCHAR_STATE_PAIN2:
            case eCHAR_STATE_POISON2:
            case eCHAR_STATE_PHOENIX_BURN:
                {
                    const BYTE npc_grade = npc->GetGrade();
                    if (npc_grade == eNPC_ELITE || npc_grade == eNPC_MIDDLEBOSS || npc_grade == eNPC_BOSS) 
                    {
                        grade_ratio = 1.0f;
                    }
                } break;
            }
    #endif //_NA_000000_20130529_STATUS_EXCEPTIONS_AS_NPC_GRADE
#endif //_NA_007123_20140324_SHADOW_SKILL_RENEWAL
        }
        calc_ratio = ((float)success_ratio * grade_ratio) / 10.0f;
    }
//#ifdef _NA_006040_20120830_RESIS_BAD_STATUS_RATIO
    BASE_STATEINFO* const state_info = \
        StateInfoParser::Instance()->GetStateInfo(state_id);
    if (state_info->m_byType == 3 || state_info->m_byType == 4)
    {
        calc_ratio = 100.0f;
    }
    else
    {
        calc_ratio -= float(target->GetResistBadStatusRatio(state_id));
    }

    int calc_success_ratio = round(calc_ratio);
    calc_success_ratio = min(calc_success_ratio, 100);
    calc_success_ratio = max(calc_success_ratio, 0);

    if (DrawLots((BYTE)calc_success_ratio)) 
    {
        return true;
    }

    return false;
}

float ns_formula::ep1::GetDuraDecRatio(eZONE_STATE zone_state,
                                       eCOMPETITION_MODE_TYPE eCompetitionMode)
{
    float fDecRatio = 0.0f;

    switch (zone_state)
    {
    case ePRS_AT_MISSION:
        fDecRatio = 0.1f;
        break;
    case ePRS_AT_FIELD:
        fDecRatio = 0.1f;
        break;
    case ePRS_AT_DOMINATION_FIELD:
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        fDecRatio = 0.025f;
#else
        fDecRatio = 0.05f;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        break;
    case ePRS_AT_PVP:
        fDecRatio = 0.0f;
        break;
    case ePRS_AT_CHUNTING:
        switch (eCompetitionMode)
        {
        case eCOMPETITION_MODE_SURVIVAL:    fDecRatio = 0.05f; break;
        case eCOMPETITION_MODE_TIME_ATTACK: fDecRatio = 0.10f; break;
        case eCOMPETITION_MODE_DEATH_MATCH: fDecRatio = 0.05f; break;
        default:                            fDecRatio = 0.20f; break;
        }
        break;
    default:
        fDecRatio = 0.2f;
        break;
    }

    return fDecRatio;
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
float ns_formula::ep1::GetDuraDecRatio(eZONE_STATE zone_state, ChaosZoneType chaos_zone_type)
{
    float fDecRatio = 0.0f;
    switch (zone_state)
    {
    case ePRS_AT_INSTANCE:
        switch(chaos_zone_type)
        {
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        case eCHAOSZONE_TYPE_GOLDRUSH:
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
        case kBattleGround:   fDecRatio = 0.05f; break; // 전장 장비 내구도 감소 비율
        case kFreePVP:        fDecRatio = 0.00f; break; // 자유전투실 장비 내구도 감소 비율 
#ifdef _NA_008334_20150608_SONNENSCHEIN
        case eCHAOSZONE_TYPE_SONNENSCHEIN: fDecRatio = 0.025f; break;
#endif //_NA_008334_20150608_SONNENSCHEIN
        default:              fDecRatio = 0.10f; break;   // 그외 모든 인스턴스 던전
        }
        break;
    default:
        fDecRatio = 0.2f;
        break;
    }
    return fDecRatio;
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

}}; //end of namespace 'ns_formula::ep1'


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : EP2
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_

namespace ns_formula { namespace ep2 {
;

// remove attributes list
//  = { OptionMagicAttPower, BaseMagicMinAttPower, BaseMagicMaxAttPower,
//      GetOptionMagicDefPower, GetBaseMagicDefPower,
//      GetBaseMeleeDefPower, GetBaseRangeDefPower,
//      GetMagicCriticalRatio
//
//      GetMagicalAttackPower(eATTACK_TYPE attack_type, eATTR_KIND attr_kind = eATTR_KIND_CALC) const;
//      GetMagicalDefense(eATTACK_TYPE attack_type) const;
//      GetBonusDefense(eATTACK_TYPE attack_type) const;
//      GetReduceDefenseRate(eATTACK_TYPE attack_type) const;
//      GetBonusDamage(eARMOR_TYPE armor_type) const;
//      GetBonusPercentDamage(eARMOR_TYPE armor_type) const;
//      GetReduceDamage(eATTACK_TYPE attack_type) const;
//      BypassDeffenceRatio

// renaming attributes list
//  = { 
//      GetBaseMeleeDefPower -> GetBaseDefensePower,
/*
*/
//==================================================================================================

class DamageCalculator
{
public:
#ifdef _DEBUG
    #define DamageMinMaxRandom(output_type, min_val, max_val) \
        static_cast<output_type>(\
            (this->kSeedValue / static_cast<double>(0xFFFFFFFFUL) * \
                (max_val - min_val)) + \
            min_val)
#else
    #define DamageMinMaxRandom(output_type, min_val, max_val) \
        static_cast<output_type>(\
            (RandomNumberGenerator::GetRandomNumber() / static_cast<double>(0xFFFFFFFFUL) * \
                (max_val - min_val)) + \
            min_val)
#endif
    DamageCalculator(Character* attacker, Character* defender,
        eATTACK_TYPE base_attack_type, eATTACK_TYPE skill_attack_type,
        bool handle_to_fix_damage, bool handle_to_fix_skill_damage);
    //
    int CalculateTotalDamage(
        const bool handle_critical_calculation,
        const uint16_t rate_of_critical_bonus,
        uint8_t* const result_of_skill_effect_flag,
        //
        const int skill_attack_power,  // + value
        const float skill_added_ratio_damage) const; // * (100% + ratio value)

private:
    // calculate the probability of it if this process is normal attack
    bool CanPassNextStepOnNormalDamage() const;
    // unused Episode II version
    bool IsBypassDeffence() const;
    //
    bool IsApplyCriticalDamage(int rate_of_critical_occurrance) const;
    // T = (Base + Option)
    // [BaseAttackPower] = T + (T * increase_ratio)
    void GetDefaultAttackPower(
        int* const result_min, int* const result_max) const;
    // T = (Base + Option)
    // [BaseDefensePower] = T + (T * increase_ratio)
    void GetDefaultDefensePower(int* const result_value) const;
    //
    void CalculateSharedDamageOption(
        int* const base_attack_power_min, int* const base_attack_power_max) const;
#ifdef _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
    // 스킬 및 원소공격력에 에테르 탄환 추가 데미지 적용
    void CalculateSharedDamageOption(int* const base_skill_attack_power) const;
#endif //_NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
    //
    void CalculateDefaultDamageOption(
        int* const base_attack_power_min, int* const base_attack_power_max) const;
    // skill_attack_power : input/output
    void CalculateSkillDamage(
        int* const skill_attack_power, const float skill_added_ratio_damage) const;
    //
    void CalculateElementDamage(
        int* const attributes_attack_power,
        eATTACK_TYPE attack_type,
        const int skill_attack_power,
        const float skill_added_ratio_damage) const;
    //void CalcAttributeAttackPower(
    //    Character* attacker, eATTACK_TYPE attack_type, Character* defender,
    //    int* const result_max, int* const result_min);
    //
    int CalculateLastAdjustmentDamage(
        int confirmed_damage, int confirmed_defense,
        double damage_decrease_ratio) const;
    //
    int CalculateCriticalDamage(
        bool handle_to_max_damage,
        int adjustment_damage, int rate_of_critical_ratio) const;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    int CalculateAdditionFPDamage(
        Character* skill_user, 
        int damage
        ) const;
    int CalculateIncreaseDamageRatio(
        Character* skill_user, 
        int damage
        ) const;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    //
    // data fields
    //
    const DWORD kSeedValue;
    const bool handle_to_fix_damage_; // CHANGES: f1110215.3L, added to support periodic damage
    const bool handle_to_fix_skill_damage_;
    const bool is_pvp_; // CHANGES: f110601.4L, added pvp tags to support PvP, PvE brached formula
    Character* const attacker_;
    const Attributes& attacker_attr_;
    const int attacker_level_;
    Character* const defender_;
    const Attributes& defender_attr_;
    const int defender_level_;
    //
    const eATTACK_TYPE base_attack_type_;
    const eATTACK_TYPE skill_attack_type_;
    //
    static DamageBalanceConstant kConstants;
    //
    friend bool ep2::ModifyDamageBalanceConstant(const DamageBalanceConstant&);
    friend bool ep2::GetCurrentDamageBalanceConstant(DamageBalanceConstant* const);
    __DISABLE_COPY(DamageCalculator);
};

//==================================================================================================
// CHANGES: f110706.4L, waverix, the #2997 post changes that are modified by 'youngmoon' change
// the damage_deduction_pvp_weight constant value.
ns_formula::ep2::DamageBalanceConstant
ns_formula::ep2::DamageCalculator::kConstants =
{
    3.0f,  // attaker_level_weight
    4.0f,  // diff_level_weight
    0.1f,  // damage_deduction_pvp_weight (CHANGES: f110706.4L)
    0.6f   // damage_deduction_pve_weight (CHANGES: f110518.8L, '0.7'->'0.6', planner's request)
};
//==================================================================================================

ns_formula::ep2::DamageCalculator::DamageCalculator(Character* attacker, Character* defender,
    eATTACK_TYPE base_attack_type, eATTACK_TYPE skill_attack_type,
    bool handle_to_fix_damage, bool handle_to_fix_skill_damage)
    : attacker_(attacker)
    , handle_to_fix_damage_(handle_to_fix_damage)
    , handle_to_fix_skill_damage_(handle_to_fix_skill_damage)
    , attacker_attr_(attacker->GetAttr())
    , attacker_level_(attacker->GetDisplayLevel())
    , defender_(defender)
    , defender_attr_(defender->GetAttr())
    , defender_level_(defender->GetDisplayLevel())
    , is_pvp_(attacker->IsEqualObjectKind(PLAYER_OBJECT) &&
              defender->IsEqualObjectKind(PLAYER_OBJECT))
    , base_attack_type_(base_attack_type)
    , skill_attack_type_(skill_attack_type)
    , kSeedValue(RandomNumberGenerator::GetRandomNumber())
{
}

// calculate the probability of it if this process is normal attack
bool ns_formula::ep2::DamageCalculator::CanPassNextStepOnNormalDamage() const
{
    int attacker_probablity = attacker_attr_.GetPhysicalAttackRate(); // 100 -> 100%
    int defender_probablity = defender_attr_.GetPhysicalAvoidRate(); // 100 -> 100%
    if (attacker_probablity >= 100) {
        attacker_probablity = 99;
    };
#ifdef _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY
    if (defender_probablity >= 81) 
    {
        defender_probablity = 80;
    }
#else
    if (defender_probablity >= 100) {
        defender_probablity = 99;
    }
#endif  // _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY

    int enable_attack_draw_number = DamageMinMaxRandom(int, 0, 100);
    bool enable_attack = (enable_attack_draw_number <= attacker_probablity);

    if (enable_attack)
    {
        int enable_block_draw_number = DamageMinMaxRandom(int, 0, 100);
        bool enable_block = (enable_block_draw_number <= defender_probablity);
        if (enable_block) {
            enable_attack = false;
        }
    };

    return enable_attack;
}

bool ns_formula::ep2::DamageCalculator::IsBypassDeffence() const
{
    int bypass_ratio = attacker_attr_.GetBypassDeffenceRatio();
    if (bypass_ratio < 0) {
        return false;
    }
    int spreading_value = ((kSeedValue >> 3) % 100);
    return (spreading_value < bypass_ratio) ? true : false;
}

bool ns_formula::ep2::DamageCalculator::IsApplyCriticalDamage(
    int rate_of_critical_occurrance) const
{
    if (attacker_->IsEqualObjectKind(PLAYER_OBJECT))
    {
        // CHANGES: f110103.2L, change critical occurrence probability
    #if SUN_CODE_BACKUP
        int block_factor = 30 + (defender_level_ * 4);
        // block_ratio = [0, block_factor)
        int block_ratio = (kSeedValue >> 3) % block_factor;
        if (block_ratio < rate_of_critical_occurrance) {
            return true;
        }
    #endif
        int block_ratio = (kSeedValue >> 3) % 100;
        if (block_ratio < rate_of_critical_occurrance) {
            return true;
        }
    }
    else
    {
        // BASE_NPCINFO::m_byCriticalRatio
        // block_ratio = [0, 100)
        int block_ratio = (kSeedValue >> 9) % 100;
        if (block_ratio < rate_of_critical_occurrance) {
            return true;
        }
    }
    return false;
}

//==================================================================================================

void ns_formula::ep2::DamageCalculator::GetDefaultAttackPower(
    int* const result_min, int* const result_max) const
{
    // T = (Base + Option)
    // [BaseAttackPower] = T + (T * increase_ratio)

    int base_min = 0, base_max = 0;
    int option_value = 0;
    //
    ;    if (base_attack_type_ == eATTACK_TYPE_MELEE)
    {
        option_value = attacker_attr_.GetOptionPhysicalAttPower();
        base_min += attacker_attr_.GetBaseMeleeMinAttPower();
        base_max += attacker_attr_.GetBaseMeleeMaxAttPower();
    }
    else if (base_attack_type_ == eATTACK_TYPE_RANGE)
    {
        option_value = attacker_attr_.GetOptionPhysicalAttPower();
        base_min += attacker_attr_.GetBaseRangeMinAttPower();
        base_max += attacker_attr_.GetBaseRangeMaxAttPower();
    }
    //
    if (option_value)
    {
        base_min += option_value;
        base_max += option_value;
    };
    if (int attack_increase_ratio = attacker_attr_.GetAttackIncRate())
    {
        if (base_min) {
            base_min = base_min + static_cast<int>(base_min * attack_increase_ratio / 100.0f);
        }
        if (base_max) {
            base_max = base_max + static_cast<int>(base_max * attack_increase_ratio / 100.0f);
        }
    };

    if (result_max) {
        *result_max = base_max;
    }
    if (result_min) {
        *result_min = base_min;
    }
}

//==================================================================================================
// T = (Base + Option)
// [BaseDefensePower] = T + (T * increase_ratio)
void ns_formula::ep2::DamageCalculator::GetDefaultDefensePower(
    int* const result_value) const
{
    int defense_power = 0;
    //
    ;    if (base_attack_type_ == eATTACK_TYPE_MELEE) {
        defense_power = defender_attr_.GetBaseMeleeDefPower();
    }
    else if (base_attack_type_ == eATTACK_TYPE_RANGE) {
        defense_power = defender_attr_.GetBaseRangeDefPower();
    }
    
    if (int option_value = defender_attr_.GetOptionPhysicalDefPower()) {
        defense_power += option_value;
    }

    // removed added defense power based on attack type

    if (int defense_increase_ratio = defender_attr_.GetDefenseIncRate())
    {
        defense_power = defense_power +
                        static_cast<int>(defense_power * defense_increase_ratio / 100.0f);
    };
    // NOTE: add a missing attribute about the reduce target defense rate against physical attack
    if (int target_defense_decrease_ratio = attacker_attr_.GetReducePhysicalTargetDefenseRatio()) {
        defense_power = defense_power -
                        static_cast<int>(defense_power * target_defense_decrease_ratio / 100.0f);
    }

    // f101229.1L, forced rule
    if (defense_power < 0) {
        defense_power = 0;
    };
    if (result_value) {
        *result_value = defense_power;
    }
}

//==================================================================================================
// return input damage if a damage has doubled by roulette.
int ns_formula::ep2::CalcDoubleDamage(Character* attacker, int damage)
{
    const Attributes& attacker_attr = attacker->GetAttr();
    if (int rate_of_double_damage = attacker_attr.GetDoubleDamageRatio()) {
        // makes to positive value and makes to simple numerator
        int fail_bound = ((RandomNumberGenerator::GetRandomNumber() >> 1) & 0xFFFFLL);
        ;   fail_bound = fail_bound % 100;
        if (fail_bound < rate_of_double_damage) {
            return damage;
        }
    }
    return 0;
}

// 상태적용 성공여부, _NA_001231_20081216_ADD_OPTION_KIND
bool ns_formula::ep2::IsStatusHit(
    Character* const attacker, 
    Character* const defender,
    const int rate_of_success,
    const int state_id, 
    const Skill* const skill)
{
    // TODO: do reorganize
    return ep1::IsStatusHit(attacker, defender, rate_of_success, state_id, skill);
}


//==================================================================================================
// base_attack_power_min : input/output 
// base_attack_power_max : input/output
void ns_formula::ep2::DamageCalculator::CalculateSharedDamageOption(
    int* const base_attack_power_min, int* const base_attack_power_max) const
{
    int attack_power_min = *base_attack_power_min;
    int attack_power_max = *base_attack_power_max;
    //----------------------------------------------------------------------------------------------
    // #2-2: Ether weapon option
    //  accept (+) ranges
    // CHANGES: f110601.4L, the ether weapons using ether bullets have various bullets options.
    // only the bullets option has the ether weapon damage increase ratio and
    // these only apply when player equipped and used bullets.
    // the below attribute selection is designed on the script rule
    // that is one bullet has only one of the two.
    int ether_weapon_damage_ratio = attacker_attr_.GetEhterDamageRatio(); // custom option (PvP+PvE)
    if (ether_weapon_damage_ratio == 0 && is_pvp_ == false) {
        ether_weapon_damage_ratio = attacker_attr_.GetEhterPvEDamageRatio(); // new option (PvE)
    };
    if (ether_weapon_damage_ratio > 0)
    {
        // Min = min_damage * ( 95% + ether_ratio);
        // Max = max_damage * (105% + ether_ratio);
        attack_power_min = static_cast<int>(attack_power_min *
            ( 95 + ether_weapon_damage_ratio) / 100.0f);
        attack_power_max = static_cast<int>(attack_power_max *
            (105 + ether_weapon_damage_ratio) / 100.0f);
    }
    //
    *base_attack_power_min = attack_power_min;
    *base_attack_power_max = attack_power_max;
}
#ifdef _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
//==================================================================================================
// skill_attack_power : input/output
void ns_formula::ep2::DamageCalculator::CalculateSharedDamageOption(int* const base_skill_attack_power) const
{
    int skill_attack_power = *base_skill_attack_power;
    //----------------------------------------------------------------------------------------------
    // #2-2: Ether weapon option
    //  accept (+) ranges
    // CHANGES: f110601.4L, the ether weapons using ether bullets have various bullets options.
    // only the bullets option has the ether weapon damage increase ratio and
    // these only apply when player equipped and used bullets.
    // the below attribute selection is designed on the script rule
    // that is one bullet has only one of the two.
    int ether_weapon_damage_ratio = attacker_attr_.GetEhterDamageRatio(); // custom option (PvP+PvE)
    if (ether_weapon_damage_ratio == 0 && is_pvp_ == false) {
        ether_weapon_damage_ratio = attacker_attr_.GetEhterPvEDamageRatio(); // new option (PvE)
    };
    if (ether_weapon_damage_ratio > 0)
    {
        // Min = min_damage * ( 95% + ether_ratio);
        // Max = max_damage * (105% + ether_ratio);
        skill_attack_power = static_cast<int>(skill_attack_power *
            ( 100 + ether_weapon_damage_ratio) / 100.0f);
    }
    *base_skill_attack_power = skill_attack_power;
}
#endif

//==================================================================================================
// base_attack_power_min : input/output 
// base_attack_power_max : input/output
void ns_formula::ep2::DamageCalculator::CalculateDefaultDamageOption(
    int* const base_attack_power_min, int* const base_attack_power_max) const
{
    int attack_power_min = *base_attack_power_min;
    int attack_power_max = *base_attack_power_max;
    //----------------------------------------------------------------------------------------------
    // #2: apply options = { GetRatioIncrease(Min/Max)Damage, Ether weapon }
    //  fixed the usage about 'GetRatioIncrease(Min/Max)Damage'
    //  -> base_attack_power_XXX = base_attack_power_XXX + (base_attack_power_XXX * ratio)
    //  accept (+/-) ranges
    if (int increase_min_damage_ratio = attacker_attr_.GetRatioIncreaseMinDamage()) {
        attack_power_min = attack_power_min +
            static_cast<int>(attack_power_min * increase_min_damage_ratio / 100.0f);
    }
    if (int increase_min_damage_item = attacker_attr_.GetItemIncreaseMinDamage()) {
        attack_power_min += increase_min_damage_item;
    };
    //
    if (int increase_max_damage_ratio = attacker_attr_.GetRatioIncreaseMaxDamage()) {
        attack_power_max = attack_power_max +
            static_cast<int>(attack_power_max * increase_max_damage_ratio / 100.0f);
    }
    if (int increase_max_damage_item = attacker_attr_.GetItemIncreaseMaxDamage()) {
        attack_power_max += increase_max_damage_item;
    };
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //----------------------------------------------------------------------------------------------
    // 공성 오브젝트 데미지 증가 옵션
    if (defender_->IsEqualObjectKind(MAPNPC_OBJECT))
    {
        BYTE npc_grade = (static_cast<NPC*>(defender_))->GetBaseInfo()->m_byGrade;
        if(npc_grade == eNPC_DOMINATION_MAPOBJECT_NPC || npc_grade == eNPC_CRYSTAL_WARP)
        {
            attack_power_min = attacker_->GetAttr().GetValueAppliedAttr(attack_power_min,\
                eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO);
            attack_power_max = attacker_->GetAttr().GetValueAppliedAttr(attack_power_max,\
                eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO);
        }
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //
    *base_attack_power_min = attack_power_min;
    *base_attack_power_max = attack_power_max;
    //
    CalculateSharedDamageOption(base_attack_power_min, base_attack_power_max);
}

// skill_attack_power : input/output
void ns_formula::ep2::DamageCalculator::CalculateSkillDamage(
    int* const skill_attack_power, const float skill_added_ratio_damage) const
{
    int skill_power = *skill_attack_power;

    // formula
    // skill_attack_power = skill attribute
    // skill_added_ratio_damage = skill attribute + character.GetSkillPercentDamage
    //                         ratio_of_intelligence_to_skill_attack_power
    // BaseSkillAttackPower = _____________________________________________ * intelligence
    //                                            100
    // SkillAttackPower = BaseSkillAttackPower + other options of a character
    // [SkillDamage] = (skill_attack_power + SkillAttackPower) *
    //                 (1 + skill_added_ratio_damage) <- '%'

    skill_power += attacker_attr_.GetSkillAttackPower();
    skill_power = util::LL_Round(skill_power * (1.0f + skill_added_ratio_damage));

    *skill_attack_power = skill_power;

#ifdef _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
    CalculateSharedDamageOption(skill_attack_power);
#endif // _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
}

//==================================================================================================
//
void ns_formula::ep2::DamageCalculator::CalculateElementDamage(
    int* const attributes_attack_power,
    eATTACK_TYPE attack_type,
    const int skill_attack_power,
    const float skill_added_ratio_damage) const
{
    Attributes::Elements attacker_power;
    Attributes::Elements rate_of_defense_power_reduction_of_target;
    Attributes::Elements damage_reduction_of_defender;
    //
    attacker_attr_.GetDefaultMagicalAttributes(
        &attacker_power,
        &rate_of_defense_power_reduction_of_target,
        NULL);
    //
    defender_attr_.GetDefaultMagicalAttributes(
        NULL,
        NULL,
        &damage_reduction_of_defender);
    // renewal magic skill, magic skill = elemental skill
    const bool is_elemental_skill = attack_type >= eATTACK_TYPE_WATER &&
                                    attack_type <= eATTACK_TYPE_DARKNESS;
    if (is_elemental_skill)
    {
        // magic skill = elemental skill
        Attributes::Elements::eElement element = \
            static_cast<Attributes::Elements::eElement>(attack_type - eATTACK_TYPE_WATER);
        int power = attacker_power.values[element];
        ;   power += skill_attack_power;
        ;   power = static_cast<int>(power * (1.0f + skill_added_ratio_damage));
        attacker_power.values[element] = power;
    }

    if (int attack_increase_ratio = attacker_attr_.GetAttackIncRate())
    {
        for (int* it = attacker_power.values;
             it != &attacker_power.values[_countof(attacker_power.values)];
             ++it)
        {
            if (*it) {
                (*it) = (*it) + static_cast<int>((*it) * attack_increase_ratio / 100.0f);
            }
        };
    };
    const bool is_player = this->defender_->IsEqualObjectKind(PLAYER_OBJECT);
    // CHANGES: f110825.2L, fixed a missing calcuation modification with boundary
    ;{
        int defense_increase_ratio = defender_attr_.GetDefenseIncRate();
        for (int* it = damage_reduction_of_defender.values;
             it != &damage_reduction_of_defender.values[_countof(damage_reduction_of_defender.values)];
             ++it)
        {
            // NOTE: f110118.4L, modified value under (MAX:80%) by game rule.
            int value = (*it) + defense_increase_ratio;
            if (is_player && value > 80) {
                value = 80;
            };

            (*it) = value;
        };
    }
    if (const int* reduction_it = rate_of_defense_power_reduction_of_target.values)
    {
        for (int* it = damage_reduction_of_defender.values;
             it != &damage_reduction_of_defender.values[_countof(damage_reduction_of_defender.values)];
             ++it, ++reduction_it)
        {
            // CHANGES: f110825.2L, fixed a missing calcuation modification with boundary
            //if ((*reduction_it) == 0) {
            //    continue;
            //};
            int reduction_ratio = (*reduction_it);
            int decrease_damage_ratio = *it - reduction_ratio;
            // ranges=(-100%, +XXX%)
            if (decrease_damage_ratio < -100) {
                decrease_damage_ratio = -100;
            };
            (*it) = decrease_damage_ratio;
        };
    };
    //
    int sum_of_element_damage = 0;
    // Atk:100 - (Atk * reduction_ratio:-100%) = 100 - (-100) = 200
    if (is_elemental_skill)
    {
        Attributes::Elements::eElement element = \
            static_cast<Attributes::Elements::eElement>(attack_type - eATTACK_TYPE_WATER);
        double element_damage = static_cast<double>(attacker_power.values[element]);
        double defense_power =
            (element_damage * damage_reduction_of_defender.values[element] / 100.0f);

        element_damage -= defense_power;
        if (element_damage > 0.0f) {
            sum_of_element_damage += util::LL_Round(element_damage);
        }
    }
    else
    {
        const int* reduction_it = damage_reduction_of_defender.values;
        for (int* it = attacker_power.values;
             it != &attacker_power.values[_countof(attacker_power.values)];
             ++it, ++reduction_it)
        {
            double element_damage = static_cast<double>(*it);
            double defense_power = (element_damage * (*reduction_it) / 100.0f);
            element_damage -= defense_power;
            if (element_damage > 0.0f) {
                sum_of_element_damage += util::LL_Round(element_damage);
            }
        }
    };

    if (sum_of_element_damage < 0) {
        assert(!"expected to impossible");
        sum_of_element_damage = 0;
    }
    // result
    *attributes_attack_power = sum_of_element_damage;
#ifdef _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
    CalculateSharedDamageOption(attributes_attack_power);
#endif // _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY
}

//==================================================================================================
//
int ns_formula::ep2::DamageCalculator::CalculateLastAdjustmentDamage(
    int confirmed_damage, int confirmed_defense,
    double damage_decrease_ratio) const
{
#if SUN_CODE_BACKUP
    // The Episode I damage deduction formula
    float adjustment_ratio = static_cast<float>(80 + defender_level_ * 6 - confirmed_defense) /
                             static_cast<float>(750 + defender_level_ * 2) +
                             damage_decrease_ratio +
                             (attacker_level_ - defender_level_) * 0.03f;

    // The Episode II damage deduction formula
    // Backup:
    //  (AtkLv * C1 + (AtkLv - DefLv) * C2 + INT(TotalDamage * C3))
    //  ___________________________________________________________
    //                     (TotalDefense + 1)
    //  C1 : kConstants.attaker_level_weight (default = 3)
    //  C2 : kConstants.diff_level_weight (default = 4)
    //double numerator =
    //    ((attacker_level_ * kConstants.attacker_level_weight) +
    //     ((attacker_level_ - defender_level_) * kConstants.diff_level_weight) +
    //     util::LL_Round(confirmed_damage * damage_decrease_ratio));
#endif
    // The Episode II damage deduction formula
    // 2nd renewal:
    //   TotalDamage * C3
    //  __________________
    //  (TotalDefense + 1)
    //
    //  C3 : kConstants.damage_deduction_pvp_weight (default = 0.1),
    //       kConstants.damage_deduction_pve_weight (default = 0.6)
    double numerator = confirmed_damage * damage_decrease_ratio;
    double adjustment_ratio = numerator / static_cast<double>(confirmed_defense + 1);
    //
    const float min_value_damage = 0.01f; // #4240 데미지 차감율 관련 공식 변경 - arycoat 2012.01.31
    if (adjustment_ratio < min_value_damage) {
        adjustment_ratio = min_value_damage;
    }
    if (adjustment_ratio > 1.0f) {
        adjustment_ratio = 1.0f;
    };
    //
    int adjustment_damage = util::LL_Round(confirmed_damage * adjustment_ratio);
    return adjustment_damage;
}

//==================================================================================================
// returns
//  critical_damage = adjustment_damage + additional critical damage
int ns_formula::ep2::DamageCalculator::CalculateCriticalDamage(
    bool handle_to_max_damage,
    const int adjustment_damage, int rate_of_critical_bonus) const
{
    if (adjustment_damage <= 0) {
        return 0;
    };
    // rate_of_critical_bonus, critical_bonus_damage, critical_bonus_ratio_damage
    rate_of_critical_bonus += attacker_attr_.GetCriticalRatioChange();

    rate_of_critical_bonus += defender_attr_.GetEnemyCriticalRatioChange(); // 피격 시 상대의 크리티컬 확률 감소
    if (rate_of_critical_bonus < 0) {
        rate_of_critical_bonus = 0;
    }

    if (IsApplyCriticalDamage(rate_of_critical_bonus) == false) {
        return 0;
    }

    // formula:
    // EP1 = [1.2, 1.7]
    // EP2 = [1.5, 2.0]
    //  critical_damage     = adjustment_damage * 1.5f + ([0, adjustment_damage] * 0.5f)
    //  max_critical_damage = adjustment_damage * 2.0f
    //                                       (1.5f + 0.5f)

    assert(adjustment_damage >= 0);
    int calc_slot = adjustment_damage;
    if (handle_to_max_damage)
    {
        calc_slot <<= 1;
    }
    else
    {
        int default_damage = (adjustment_damage >> 1);
        int random_damage = RandomNumberGenerator::GetRandomNumberRange(\
            0, default_damage);
        calc_slot = calc_slot + default_damage + random_damage;
    };

#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    //크리티컬 데미지 증감
    AttrValue attacker_critical_damage = attacker_attr_[eATTR_CRITICAL_DAMAGE_CHANGE];
    attacker_critical_damage[eATTR_KIND_BASE] = calc_slot;

#ifdef _NA_008441_20150819_CRITICAL_DAMAGE_CHANGE_FROM_LEVEL_DIFFERENCCE
    const int max_decrease = 10;
    int critical_damage_change_decrease = defender_level_ - attacker_level_;
    if (critical_damage_change_decrease > 0)
    {
        if (critical_damage_change_decrease > max_decrease)
        {
            critical_damage_change_decrease = max_decrease;
        }
        attacker_critical_damage[eATTR_KIND_ITEM_RATIO] -= critical_damage_change_decrease;
    }
#endif // _NA_008441_20150819_CRITICAL_DAMAGE_CHANGE_FROM_LEVEL_DIFFERENCCE

    attacker_critical_damage.Update();

    //피격시 상대 크리티컬 데미지 증감
    AttrValue defender_critical_damage = defender_attr_[eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE];
    defender_critical_damage[eATTR_KIND_BASE] = attacker_critical_damage[eATTR_KIND_CALC];
    defender_critical_damage.Update();

    calc_slot = defender_critical_damage[eATTR_KIND_CALC];
#else
    float critical_bonus_ratio_damage = attacker_attr_.GetRatioCriticalDamageBonus() / 100.0f;
    calc_slot = static_cast<int>(calc_slot * (1.0f + critical_bonus_ratio_damage));

    int critical_bonus_damage = 0;
    critical_bonus_damage = attacker_attr_.GetCriticalDamageBonus();

    if (critical_bonus_damage > 0) {
        calc_slot += critical_bonus_damage;
    }
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL

    return calc_slot;
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
int ns_formula::ep2::DamageCalculator::CalculateAdditionFPDamage( Character* skill_user, int damage ) const
{
    if (skill_user->IsEqualObjectKind(PLAYER_OBJECT) == false ) {
        return damage;
    }
    if (damage <= 0) {
        return damage;
    }

    Player* player = reinterpret_cast<Player*>(skill_user);
    StatusManager* status_manager = player->GetStatusManager();
    //피드온레이지 상태 찾는다
    BaseStatus* status = status_manager->FindStatus(eCHAR_STATE_FP_POWER_INCREASE);
    
    float overlap_count = 0;
    float additional_damage_ratio = 0;
    if (status != NULL)
    {
        overlap_count = status->GetStatusOverlapCount();
        
        AbilityStatus* fp_buff_status = reinterpret_cast<AbilityStatus*>(status);
        additional_damage_ratio = fp_buff_status->GetAbilityInfo()->m_iParam[1];
    }

    // 버림
    float floor_fp =floor(player->GetFP());
    if (floor_fp == 0) {
        return damage;
    }
    float token = (1.0f+(floor_fp/25.0f)) + ((additional_damage_ratio / 100.0f)*overlap_count);
    float cur_damage = (float)damage;

    return (int)( cur_damage * token);

}

int ep2::DamageCalculator::CalculateIncreaseDamageRatio( Character* skill_user, int damage ) const
{
    int attr_value = skill_user->GetAttr().GetIncreaseDamageRatio();

    float increase_ratio = 1.0f + ((float)attr_value / 100.0f);
    
    int result_damage = (int)((float)damage * increase_ratio);

    return result_damage;
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//==================================================================================================

int ns_formula::ep2::DamageCalculator::CalculateTotalDamage(
    const bool handle_critical_calculation,
    const uint16_t rate_of_critical_bonus,
    uint8_t* const result_of_skill_effect_flag,
    //
    const int input_skill_attack_power,  // + value
    const float input_skill_added_ratio_damage) const // * (100% + ratio value)
{
    const bool is_apply_skill_damage = \
        (handle_to_fix_skill_damage_ || input_skill_attack_power != 0);
    // renewal magic skill, magic skill = elemental skill
    const bool is_elemental_skill = skill_attack_type_ >= eATTACK_TYPE_WATER &&
                                    skill_attack_type_ <= eATTACK_TYPE_DARKNESS;
    //----------------------------------------------------------------------------------------------
    // calculate the probability of it if this process is normal attack
    if (is_apply_skill_damage == false && CanPassNextStepOnNormalDamage() == false) {
        return 0; // blocking
    }
    //----------------------------------------------------------------------------------------------
    // preparation
    int base_attack_power_max = 0;
    int base_attack_power_min = 0;
    int base_defense_power = 0;
    int skill_attack_power = input_skill_attack_power;
    //----------------------------------------------------------------------------------------------
    // #1: acquire default attack/defense power
    this->GetDefaultAttackPower(&base_attack_power_min, &base_attack_power_max);
    // NOTE: this formula is unused currently. (style skill only)
    // probability of the decrease defender_ defense power
    //--- float bypass_defense_ratio = IsBypassDeffence(attacker_attr_) ? 1.0f : 0.0f;
    //
    this->GetDefaultDefensePower(&base_defense_power);
    //----------------------------------------------------------------------------------------------
    // #2: apply options = { GetRatioIncrease(Min/Max)Damage, Ether weapon }
    //  fixed the usage about 'GetRatioIncrease(Min/Max)Damage'
    //  -> base_attack_power_XXX = base_attack_power_XXX + (base_attack_power_XXX * ratio)
    this->CalculateDefaultDamageOption(&base_attack_power_min, &base_attack_power_max);
    //----------------------------------------------------------------------------------------------
    float sum_of_skill_added_ratio_damage = 0.0f;
    // #3: calculate skills
    if (is_apply_skill_damage) {
        sum_of_skill_added_ratio_damage = input_skill_added_ratio_damage +
            static_cast<float>(attacker_attr_.GetSkillPercentDamage() / 100.0f);
        this->CalculateSkillDamage(&skill_attack_power, sum_of_skill_added_ratio_damage);
    }
    //----------------------------------------------------------------------------------------------
    // #4: calculate attributes
    // TODO: list of attributes = { Water, Fire, Wind, Earth, Darkness }
    int element_attack_power = 0;
    this->CalculateElementDamage(&element_attack_power,
                                 (is_elemental_skill ? skill_attack_type_ : base_attack_type_),
                                 skill_attack_power, sum_of_skill_added_ratio_damage);
    //
    //----------------------------------------------------------------------------------------------
    // #5 make to confirmed values
    // #5.1: recalculate damage using defender's damage reduction attributes
    // (base_attack_power + skill_attack_power) * damage_reduction_ratio
    int confirmed_damage_max = base_attack_power_max + skill_attack_power;
    int confirmed_damage_min = base_attack_power_min + skill_attack_power;
    if (int physical_damage_reduction = defender_attr_.GetReducePhysicalDamageRatio()) {
        if (physical_damage_reduction > 99) {
            physical_damage_reduction = 99;
        }
        if (confirmed_damage_max) {
            confirmed_damage_max = confirmed_damage_max -
                static_cast<int>(confirmed_damage_max * physical_damage_reduction / 100.0f);
        }
        if (confirmed_damage_min) {
            confirmed_damage_min = confirmed_damage_min -
                static_cast<int>(confirmed_damage_min * physical_damage_reduction / 100.0f);
        }
    };
    // #5.2: make to confirmed values
    //;   confirmed_damage_max = confirmed_damage_max + element_attack_power;
    //;   confirmed_damage_min = confirmed_damage_min + element_attack_power;
    int confirmed_defense_power = base_defense_power;
    // fixing
    if (confirmed_damage_max < 0) {
        confirmed_damage_max = 0;
    }
    if (confirmed_damage_min < 0) {
        confirmed_damage_min = 0;
    };
    if (confirmed_damage_max < confirmed_damage_min) {
        confirmed_damage_max = confirmed_damage_min;
    }
    // ... thinking 'value < 0'
    //
    //----------------------------------------------------------------------------------------------
    // #6: calculate final damage that is considered PvP option and flag options
    //kConstants.damage_deduction_pvp_weight = 0.10f;
    //kConstants.damage_deduction_pve_weight = 0.70f;

    const StatusField& status_field = attacker_->GetStatusManager()->Status;
    const bool handle_to_max_damage = \
        ((status_field.FIELD & status_field.ENABLE_GM_MAX_DAMAGE) != 0);
    ///
    int confirmed_damage = 0;
    if (false) {
    }
    else if (handle_to_max_damage) {
        confirmed_damage = confirmed_damage_max;
    }
    else if (handle_to_fix_damage_ || handle_to_fix_skill_damage_) { // CHANGES: f110215.3L
        confirmed_damage = ((confirmed_damage_max + confirmed_damage_min) >> 1);
    }
    else {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 최대 데미지 발생확률 어트리뷰트 적용
        int max_damage_ratio = attacker_->GetAttr().GetMaxDamageRatio();
        (max_damage_ratio > 0 && random_percent(max_damage_ratio)) \
            ? confirmed_damage = confirmed_damage_max \
            : confirmed_damage = DamageMinMaxRandom(int, confirmed_damage_min, confirmed_damage_max);
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        confirmed_damage = DamageMinMaxRandom(int, confirmed_damage_min, confirmed_damage_max);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    };

    int adjustment_damage = this->CalculateLastAdjustmentDamage(
        confirmed_damage, confirmed_defense_power,
        is_pvp_ ? kConstants.damage_deduction_pvp_weight : kConstants.damage_deduction_pve_weight);
    // #6.2: make to confirmed values
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    adjustment_damage = this->CalculateAdditionFPDamage(attacker_, adjustment_damage);
    adjustment_damage = this->CalculateIncreaseDamageRatio(attacker_, adjustment_damage);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    ;   adjustment_damage = adjustment_damage + element_attack_power;
    // #6.3: guarantee minimum damage
    if (adjustment_damage <= (confirmed_damage_min / 20))
    {
        int lower_bound_value = confirmed_damage_min / 40 + 1;
        adjustment_damage = lower_bound_value + DamageMinMaxRandom(int, 0, lower_bound_value);
    };

    //----------------------------------------------------------------------------------------------
    // #7: calculate additional damage after final calculated damage
    // #7-1: critical damage
    if (handle_critical_calculation)
    {
        int critical_damage = this->CalculateCriticalDamage(
            handle_to_max_damage, adjustment_damage, rate_of_critical_bonus);

        if (result_of_skill_effect_flag)
        {
            if (critical_damage != 0) {
                (*result_of_skill_effect_flag) |= SKILL_EFFECT_CRITICAL;
            }
            else {
                (*result_of_skill_effect_flag) &= ~SKILL_EFFECT_CRITICAL;
            }
        }

        if (critical_damage != 0) {
            adjustment_damage = critical_damage;
        }
    };
    // #7-2: add damage
    if (int add_damage = attacker_attr_.GetAddDamage()) {
        adjustment_damage += add_damage;
    };

    // #8: limit maximum damage by upper boundary
    if (adjustment_damage > DAMAGETYPE_MAX) {
        adjustment_damage = DAMAGETYPE_MAX;
    };
    return adjustment_damage;
}

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_

// damage formula specification (version:EP2.01)
// tree diagram
//--------------------------------------------------------
//  [Normal]    [Skill]
//     |           |----------------|
//  [Normal]  [Normal Skill]  [Magic Skill]
//--------------------------------------------------------
//  [Normal] - [Player] with default style attack
//           - [Monster] simple attack
//  [Normal Skill] - no elemental option
//  [Magic Skill]  - with elemental option
//

//==================================================================================================
// NOTE: calculate (simulate) physical normal damage
//  rate_of_critical_bonus : increase damage if the result is succeed
int ep2::CalcNormalDamage(
    Character* attacker, Character* defender,
    eATTACK_TYPE attack_type,
    const bool handle_critical_calculation,
    const uint16_t rate_of_critical_bonus,
    uint8_t* const result_of_skill_effect_flag)
{
    ep2::DamageCalculator calculator(
        attacker, defender, attack_type, eATTACK_TYPE_INVALID, false, false);
    int total_damage = calculator.CalculateTotalDamage(
        handle_critical_calculation, rate_of_critical_bonus, result_of_skill_effect_flag,
        0, 0);
    return total_damage;
}

// NOTE: calculate (simulate) physical | elemental skill damage
//  rate_of_critical_bonus : increase damage if the result is succeed
int ep2::CalcSkillDamage(
    Character* attacker, Character* defender,
    eATTACK_TYPE attack_type,
    const bool handle_critical_calculation,
    const uint16_t rate_of_critical_bonus,
    uint8_t* const result_of_skill_effect_flag,
    //
    eATTACK_TYPE skill_attack_type,
    const int skill_attack_power,  // + value
    const float skill_added_ratio_damage) // * (100% + ratio value)
{
    ep2::DamageCalculator calculator(
        attacker, defender, attack_type, skill_attack_type, false, false);
    int total_damage = calculator.CalculateTotalDamage(
        handle_critical_calculation, rate_of_critical_bonus, result_of_skill_effect_flag,
        skill_attack_power, skill_added_ratio_damage);
    return total_damage;
}

// NOTE: f110215.3L, calculate (simulate) fixed physical | elemental skill damage
//  rate_of_critical_bonus : increase damage if the result is succeed
int ep2::CalcFixedSkillDamage(
    Character* attacker, Character* target,
    eATTACK_TYPE attack_type,
    const bool handle_critical_calculation, const uint16_t rate_of_critical_bonus,
    uint8_t* const result_of_skill_effect_flag,
    //
    eATTACK_TYPE skill_attack_type,
    const int skill_attack_power,  // + value
    const float skill_added_ratio_damage) // * (100% + ratio value)
{
    ep2::DamageCalculator calculator(
        attacker, target, attack_type, skill_attack_type, false, true);
    int total_damage = calculator.CalculateTotalDamage(
        handle_critical_calculation, rate_of_critical_bonus, result_of_skill_effect_flag,
        skill_attack_power, skill_added_ratio_damage);
    return total_damage;
}

#endif //_NA001605_EP2_ATTRIBUTES_RENEWAL_

//==================================================================================================
// 내구도 감소비율
float ep2::GetDuraDecRatio(eZONE_STATE zone_state, eCOMPETITION_MODE_TYPE eCompetitionMode)
{
    return ep1::GetDuraDecRatio(zone_state, eCompetitionMode);
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
float ep2::GetDuraDecRatio(eZONE_STATE zone_state, ChaosZoneType chaos_zone_type)
{
    return ep1::GetDuraDecRatio(zone_state, chaos_zone_type);
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//
EXPTYPE ep2::GetKillExpOfParty(
    BYTE byRoomBonusType, DWORD partymemberNum,
    LEVELTYPE partyTotalLV, LEVELTYPE attackerLV, LEVELTYPE targetLV,
    float fNPCGradeExpRatio,
    DWORD attackDamage,
    DWORD targetMaxHP,
    eZONETYPE ZoneType,
    Player* player)
{
    return ep1::GetKillExpOfParty(byRoomBonusType, partymemberNum,
        partyTotalLV, attackerLV, targetLV,
        fNPCGradeExpRatio,
        attackDamage,
        targetMaxHP,
        ZoneType,
        player);
}

// HP,MP 회복량(플레이어)
int ep2::CalcHPRecover(eCHAR_TYPE char_type, DWORD maxHP, BYTE byCharState, LEVELTYPE Level)
{
    return ep1::CalcHPRecover(char_type, maxHP, byCharState, Level);
}

int ep2::CalcSDRecover(
    eCHAR_TYPE char_type, eCHAR_CONDITION byCharState,
    BYTE byCharAction, LEVELTYPE level)
{
    return ep1::CalcSDRecover(char_type, byCharState, byCharAction, level);
}


int ep2::CalcMPRecover(eCHAR_TYPE char_type, DWORD spirit, BYTE byCharState)
{
    return ep1::CalcMPRecover(char_type, spirit, byCharState);
}

DWORD ep2::CalcMPRecoverByAttack(
    eCHAR_TYPE char_type, DWORD spirit,
    DWORD skillRecoverPercent, DWORD skillRequireMP) // 공격할때 회복량
{
    return ep1::CalcMPRecoverByAttack(char_type, spirit, skillRecoverPercent, skillRequireMP);
}

DWORD ep2::CalcMPRecoverByAttacked(
    eCHAR_TYPE char_type, DWORD spirit, DAMAGETYPE damage) // 타격 받았을떄 회복량
{
    return ep1::CalcMPRecoverByAttacked(char_type, spirit, damage);
}

// HP, MP 회복량(몬스터)
int ep2::CalcNPCHPRecover(DWORD maxHP, DWORD recoverRatio)
{
    return ep1::CalcNPCHPRecover(maxHP, recoverRatio);
}

int ep2::CalcNPCMPRecover(DWORD maxMP, DWORD recoverRatio)
{
    return ep1::CalcNPCMPRecover(maxMP, recoverRatio);
}

int ep2::CalcNPCSDRecover(DWORD maxSD)
{
    return ep1::CalcNPCSDRecover(maxSD);
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
PLAYERFP CalcFpRecover( eCHAR_TYPE char_type )
{
    PLAYERFP recover_value = 0;
    switch (char_type)
    {
    case eCHAR_WITCHBLADE:
        recover_value = -0.5f;
        break;
    default:
        break;
    }

    return recover_value;
}

PLAYERFP CalcFPRecoverByAttack( eCHAR_TYPE char_type )
{
    PLAYERFP recover_value = 0;
    switch (char_type)
    {
    case eCHAR_WITCHBLADE:
        recover_value = 1.0f;
        break;
    default:
        break;
    }

    return recover_value;
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

// GM utility
//double ns_formula::ep2::DamageCalculator::kConstants.attaker_level_weight = 3.0f;
//double ns_formula::ep2::DamageCalculator::kConstants.diff_level_weight = 4.0f;
//double ns_formula::ep2::DamageCalculator::kConstants.damage_deduction_pvp_weight = 0.1f;
//double ns_formula::ep2::DamageCalculator::kConstants.damage_deduction_pve_weight = 0.6f;
bool ep2::ModifyDamageBalanceConstant(const DamageBalanceConstant& modifying_constants)
{
    DamageBalanceConstant modified = modifying_constants;
    // > 0.0f
    if (modified.attacker_level_weight < 0.0f) {
        modified.attacker_level_weight = 3.0f;
    }
    if (modified.diff_level_weight < 0.0f) {
        modified.diff_level_weight = 4.0f;
    }
    if (modified.damage_deduction_pvp_weight < 0.0f) {
        modified.damage_deduction_pvp_weight = 0.1f;
    }
    if (modified.damage_deduction_pve_weight < 0.0f) {
        modified.damage_deduction_pve_weight = 0.6f;
    }

    ns_formula::ep2::DamageCalculator::kConstants = modified;
    return true;
}

bool ep2::GetCurrentDamageBalanceConstant(DamageBalanceConstant* const current_constants)
{
    *current_constants = ns_formula::ep2::DamageCalculator::kConstants;
    return true;
}


//==================================================================================================
// CHANGES: f110215.3L, f110217.5L, changes periodic damage formula
// (Base + Element damage) * [apply ratio (iOption2)] + [skill attack power]
int DamagePerCycleCalculator::Simulate(
    Character* attacker, Character* target,
    eATTACK_TYPE /*attack_type*/,
    eATTACK_TYPE skill_attack_type,
    const int apply_added_base_damage_ratio, // INPUT : permilage (ability_info->m_iOption2)
    const int skill_attack_power,  // + value
    const float skill_added_ratio_damage) // * (100% + ratio value)
{
    // CHANGES: f110215.3L, f110217.5L, changes periodic damage formula
    // (Base + Element damage) * [apply ratio (iOption2)] + [skill attack power]
    uint8_t dummy_effects = 0;

    const eATTACK_TYPE base_attack_type = attacker->GetWeaponBaseAttType();
    int base_skill_damage = ep2::CalcFixedSkillDamage(
        attacker, target, base_attack_type, false, 0, &dummy_effects, skill_attack_type, 0, 0);
    int result_skill_damage = ep2::CalcFixedSkillDamage(
        attacker, 
        target, 
        base_attack_type, 
        false, 
        0, 
        &dummy_effects, 
        skill_attack_type, 
        skill_attack_power, 
        skill_added_ratio_damage
    );
    
    const int max_temp = max(result_skill_damage, base_skill_damage);
    base_skill_damage = min(result_skill_damage, base_skill_damage);
    result_skill_damage = max_temp;

    result_skill_damage -= base_skill_damage;
    const float apply_added_base_damage = (apply_added_base_damage_ratio / 1000.0f);
    base_skill_damage = util::LL_Round(base_skill_damage * apply_added_base_damage);
    result_skill_damage += base_skill_damage;
    
    return result_skill_damage;
}

}}; //end of namespace 'ns_formula::ep2'

#endif //_NA001605_EP2_ATTRIBUTES_RENEWAL_
