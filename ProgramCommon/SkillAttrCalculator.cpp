#include "stdafx.h"
#include "SkillAttrCalculator.h"

#include <Const.h>
#include <SCItemSlot.h>

#include "Attribute.h"
#include "SkillInfoParser.h"
#include "SetItemOptionInfoParser.h"
#include "./RidingInfoParser.h"

//==================================================================================================

// (f100812.2L)
void SkillAttrCalculator::ModifyAttrKindAndCalculatedValue(uint8_t value_type, //eVALUE_TYPE
                                                           uint8_t* attr_kind, //eATTR_KIND
                                                           int * value)
    //eVALUE_TYPE value_type, int* attr_kind, int* value)
{
    *attr_kind = eATTR_KIND_SKILL;
    float calced_value = static_cast<float>(*value);

    if (value_type == VALUE_TYPE_PERCENT_PER_MAX ||
        value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        *attr_kind = eATTR_KIND_SKILL_RATIO;
        calced_value /= 10.0f;
    }
    else if (value_type == VALUE_TYPE_RATIO_VALUE)
    {
        calced_value /= 10.0f;
    }
    //
    *value = static_cast<int>(calced_value);
}

float
SkillAttrCalculator::AddAttr(eATTR_TYPE attr_type, int value_type, int value, bool skip_update)
{
    if (attr_type <= eATTR_TYPE_INVALID || eATTR_MAX <= attr_type) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|attr_type|%d|invalid type value", attr_type);
        return 0;
    }

    eATTR_KIND attr_kind = eATTR_KIND_SKILL;
    float calced_value = static_cast<float>(value);

    if (value_type == VALUE_TYPE_PERCENT_PER_MAX ||
        value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        attr_kind = eATTR_KIND_SKILL_RATIO;
        calced_value /= 10.0f;
    }
    else if (value_type == VALUE_TYPE_RATIO_VALUE)
    {
        calced_value /= 10.0f;
    }

    // (f100812.2L) change an check filter
    // 공격력, 방어력은 Option 수치를 변경할 뿐 Base 수치를 변경하지는 않는다!
    if (IsPassedAddAttr(attr_type) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Can't change attr_type|%d|", attr_type);
        return 0;
    }
    //
    static const Attributes::Operation attr_array_for_all_stats[] = {
        eATTR_STR, eATTR_DEX, eATTR_VIT, eATTR_INT, eATTR_SPR
    };
    const int convert_value = static_cast<int>(calced_value);
    //
    switch (attr_type)
    {
    case eATTR_INCREASE_STAT_POINT:
        {
            // (f100721.1L)
            int value_array[_countof(attr_array_for_all_stats)] = {
                convert_value, convert_value, convert_value, convert_value, convert_value
            };
            //
            attributes_.GroupOperate(attr_array_for_all_stats->eOper_Add, attr_kind,
                                     attr_array_for_all_stats, value_array,
                                     _countof(attr_array_for_all_stats));
        }
        break;
    default:
        attributes_[attr_type][attr_kind] += convert_value;
        break;
    }

    // (CHANGES) (WAVERIX)
    if (skip_update == false) {
        attributes_.UpdateEx();
    }

    return calced_value;
}

float
SkillAttrCalculator::DelAttr(eATTR_TYPE attr_type, int value_type, int value, bool skip_update)
{
    if (attr_type <= eATTR_TYPE_INVALID || eATTR_MAX <= attr_type) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|attr_type|%d|invalid type value", attr_type);
        return 0;
    }

    if (!IsRecoverAttr(attr_type)) {
        return 0;
    }

    eATTR_KIND attr_kind = eATTR_KIND_SKILL;
    float calced_value = static_cast<float>(value);

    if (value_type == VALUE_TYPE_PERCENT_PER_MAX || value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        attr_kind = eATTR_KIND_SKILL_RATIO;
        calced_value /= 10.0f;
    }
    else if (value_type == VALUE_TYPE_RATIO_VALUE) 
    {
        calced_value /= 10.0f;
    }

    // (REMARK) (WAVERIX) 이 위치가 AddAttr과 코드가 다르다. 정확한 이유를 파악할 필요가 있다.
    static const Attributes::Operation attr_array_for_all_stats[] = {
        eATTR_STR, eATTR_DEX, eATTR_VIT, eATTR_INT, eATTR_SPR
    };
    //
    const int convert_value = static_cast<int>(calced_value);
    //
    switch (attr_type)
    {
    case eATTR_INCREASE_STAT_POINT:
        {
            // (f100721.1L)
            int value_array[_countof(attr_array_for_all_stats)] = {
                convert_value, convert_value, convert_value, convert_value, convert_value
            };
            //
            attributes_.GroupOperate(attr_array_for_all_stats->eOper_Sub, attr_kind,
                                     attr_array_for_all_stats, value_array,
                                     _countof(attr_array_for_all_stats));
        }
        break;
    default:
        {
            attributes_[attr_type][attr_kind] -= convert_value;
        }
        break;
    }

    // (CHANGES) (WAVERIX)
    if (skip_update == false) {
        attributes_.UpdateEx();
    }
    return calced_value;
}

void
SkillAttrCalculator::Clear()
{
    // (f100720.3L)
    attributes_.ClearRelatedAttrKind(eATTR_KIND_SKILL);
    /*
    for (eATTR_TYPE attr_type = eATTR_TYPE_INVALID;
         attr_type < eATTR_MAX;
         attr_type = static_cast<eATTR_TYPE>(attr_type + 1))
    {
        if (!attributes_.IsNULL(attr_type))
        {
            attributes_[attr_type][eATTR_KIND_SKILL] = 0;
            attributes_[attr_type][eATTR_KIND_SKILL_RATIO] = 0;
        }
    }
    */
    attributes_.Update();
}

int
SkillAttrCalculator::ApplyDragonFormation(BOOL bApply, SLOTCODE skill_code, DWORD cur_hp)
{
    int change_hp = 0;
    float cur_hp_ratio = static_cast<float>(cur_hp) / attributes_.GetMaxHP();

    const SkillScriptInfo* const base_skill_info = \
        SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (base_skill_info == NULL) {
        return 0;
    }
    // CHANGES; f110124.1L, changes register routine
    const uint8_t oper_type \
        = bApply ? Attributes::Operation::eOper_Add : Attributes::Operation::eOper_Sub;
    //
    // 변신 능력치1 적용
    const BASE_ABILITYINFO* ability_info = \
        base_skill_info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION1);
    if (ability_info)
    {
        Attributes::OperationRecord record_array[2] =
        {
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_STR,
                ability_info->m_iParam[0] },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_DEX,
                ability_info->m_iParam[1] },
        };
        attributes_.GroupBatchOperate(record_array, _countof(record_array));
    };
    // 변신 능력치2 적용
    ability_info = base_skill_info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION2);
    if (ability_info)
    {
        Attributes::OperationRecord record_array[5] =
        {
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_INT,
                ability_info->m_iOption1 },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_SPR,
                ability_info->m_iOption2 },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_VIT,
                ability_info->m_iParam[0] },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_MAX_HP,
                static_cast<int>(ability_info->m_iParam[1] / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_ATTACK_SPEED,
                static_cast<int>(ability_info->m_iParam[2] / 10.f) },
        };
        attributes_.GroupBatchOperate(record_array, _countof(record_array));
    };
    // 변신 능력치2 적용
    ability_info = base_skill_info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION3);
    if (ability_info)
    {
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        Attributes::OperationRecord record_array[6] =
        {
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_OPTION_PHYSICAL_ATTACK_POWER,
                static_cast<int>(ability_info->m_iOption1 / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_MAGICAL_ALL_ATTACK_POWER,
                static_cast<int>(ability_info->m_iOption2 / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_OPTION_PHYSICAL_DEFENSE_POWER,
                static_cast<int>(ability_info->m_iParam[0] / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_DEL_MAGICAL_ALL_DAMAGE,
                static_cast<int>(ability_info->m_iParam[1] / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,
                // CHANGES: f110124.1L, change percentage to permilage
                static_cast<int>(ability_info->m_iParam[2] / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_MOVE_SPEED,
                static_cast<int>(ability_info->m_iParam[3] / 10.f) },
        };
        attributes_.GroupBatchOperate(record_array, _countof(record_array));
    #else
        //
        Attributes::OperationRecord record_array[6] =
        {
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_OPTION_PHYSICAL_ATTACK_POWER,
                static_cast<int>(ability_info->m_iOption1 / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_MAGICAL_ALL_ATTACK_POWER,
                static_cast<int>(ability_info->m_iOption2 / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_OPTION_PHYSICAL_DEFENSE_POWER,
                static_cast<int>(ability_info->m_iParam[0] / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL_RATIO,
                eATTR_OPTION_MAGICAL_DEFENSE_POWER,
                static_cast<int>(ability_info->m_iParam[1] / 10.f) },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,
                static_cast<int>(ability_info->m_iParam[2]) },
            {   oper_type,
                eATTR_KIND_SKILL,
                eATTR_MOVE_SPEED,
                static_cast<int>(ability_info->m_iParam[3] / 10.f) },
        };
        attributes_.GroupBatchOperate(record_array, _countof(record_array));
    #endif
    };
#ifdef _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2
	attributes_.UpdateEx();
#else
	attributes_.Update();
#endif //(_NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2)

    if (cur_hp_ratio) {
        int temp = static_cast<int>(attributes_.GetMaxHP() * cur_hp_ratio);
        change_hp = max(1, temp) - cur_hp;
    }
    else {
        change_hp = 0;
    }

    return change_hp;
    //
#if SUN_CODE_BACKUP
    // 변신 능력치1 적용
    const BASE_ABILITYINFO* ability_info = \
        base_skill_info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION1);
    if (ability_info)
    {
        // (f100721.1L)
        static const Attributes::Operation attr_group[] = { eATTR_STR, eATTR_DEX };
        const int value_array[_countof(attr_group)] = {
            ability_info->m_iParam[0], ability_info->m_iParam[1]
        };
        //
        attributes_.GroupOperate((bApply ? attr_group->eOper_Add : attr_group->eOper_Sub),
                                 eATTR_KIND_SKILL,
                                 attr_group, value_array,
                                 _countof(attr_group));
    }

    // 변신 능력치2 적용
    ability_info = base_skill_info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION2);
    if (ability_info)
    {
        // (f100721.1L)
        static const Attributes::Operation attr_group[] = {
            eATTR_INT, eATTR_SPR, eATTR_VIT, eATTR_ATTACK_SPEED
        };
        const int value_array[_countof(attr_group)] = {
            ability_info->m_iOption1, ability_info->m_iOption2, ability_info->m_iParam[0],
            static_cast<int>(ability_info->m_iParam[2] / 10.f)
        };
        attributes_.GroupOperate((bApply ? attr_group->eOper_Add : attr_group->eOper_Sub),
                                 eATTR_KIND_SKILL,
                                 attr_group, value_array, _countof(attr_group));
        if (attributes_.GetMaxHP())
        {
            if (bApply)
            {
                attributes_[eATTR_MAX_HP][eATTR_KIND_SKILL_RATIO]
                    += static_cast<int>(ability_info->m_iParam[1] / 10.f);
            }
            else
            {
                attributes_[eATTR_MAX_HP][eATTR_KIND_SKILL_RATIO]
                    -= static_cast<int>(ability_info->m_iParam[1] / 10.f);
            }
            attributes_[eATTR_MAX_HP].Update();
        }
    }

    // 변신 능력치3 적용
    ability_info = base_skill_info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION3);
    if (ability_info)
    {
        // (f100721.1L)
        const Attributes::Operation::eOper oper_type \
            = bApply ? Attributes::Operation::eOper_Add : Attributes::Operation::eOper_Sub;
        ;{
            static const Attributes::Operation attr_skill_array[] = {
                eATTR_OPTION_PHYSICAL_ATTACK_POWER,  eATTR_OPTION_MAGICAL_ATTACK_POWER,
                eATTR_OPTION_PHYSICAL_DEFENSE_POWER, eATTR_OPTION_MAGICAL_DEFENSE_POWER
            };
            const int value_array[_countof(attr_skill_array)] = {
                static_cast<int>(ability_info->m_iOption1 / 10.f),
                static_cast<int>(ability_info->m_iOption2 / 10.f),
                static_cast<int>(ability_info->m_iParam[0] / 10.f),
                static_cast<int>(ability_info->m_iParam[1] / 10.f)
            };
            attributes_.GroupOperate(oper_type, eATTR_KIND_SKILL_RATIO,
                                     attr_skill_array, value_array,
                                     _countof(attr_skill_array));
        };
        //
        ;{
            static const Attributes::Operation attr_skill_array[] = {
                eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, eATTR_MOVE_SPEED
            };
            const int value_array[_countof(attr_skill_array)] = {
                ability_info->m_iParam[2],
                static_cast<int>(ability_info->m_iParam[3] / 10.f)
            };
            attributes_.GroupOperate(oper_type, eATTR_KIND_SKILL,
                                     attr_skill_array, value_array,
                                     _countof(attr_skill_array));
        };
        //
    }

    attributes_.Update();

    if (cur_hp_ratio) {
        int temp = static_cast<int>(attributes_.GetMaxHP() * cur_hp_ratio);
        change_hp = max(1, temp) - cur_hp;
    }
    else {
        change_hp = 0;
    }

    return change_hp;
#endif //SUN_CODE_BACKUP
}

//==================================================================================================
// NOTE: f110808.5L, adrenaline configuration
const Attributes::Operation nsSkill::AdrenalineConfiguration::kSkillAttrTypeArray[MaxCounts] =
{
    eATTR_BASE_MELEE_MIN_ATTACK_POWER,
    eATTR_BASE_MELEE_MAX_ATTACK_POWER,
    eATTR_BASE_RANGE_MIN_ATTACK_POWER,
    eATTR_BASE_RANGE_MAX_ATTACK_POWER,
//#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
//    eATTR_BASE_MAGICAL_MIN_ATTACK_POWER, eATTR_BASE_MAGICAL_MAX_ATTACK_POWER,
//    eATTR_BASE_MAGICAL_DEFENSE_POWER,
//#endif
    //
    eATTR_BASE_MELEE_DEFENSE_POWER,
    eATTR_BASE_RANGE_DEFENSE_POWER,
    //
    eATTR_MOVE_SPEED,
    eATTR_ATTACK_SPEED,
    eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,
    eATTR_MAX_HP
};

//==================================================================================================
// NOTE: f110808.5L, adrenaline configuration
bool SkillAttrCalculator::GetCurrentAdrenalineConfiguration(
    nsSkill::AdrenalineConfiguration* const adrenaline_data,
    SLOTCODE skill_code, const Attributes& summoner_attr)
{
    ZeroMemory(adrenaline_data, sizeof(*adrenaline_data));
    //
    const SkillScriptInfo* base_skill_info = \
        SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (base_skill_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find skill(%d)"), skill_code);
        return false;
    };
    const BASE_ABILITYINFO* ability_info = base_skill_info->GetAbilityInfo(eABILITY_ADRENALINE);
    if (ability_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=can't find the adrenaline ability in skill(%d)"),
               skill_code);
        return false;
    };

    const float parameter0 = ability_info->m_iParam[0] / 1000.f;
    const float parameter1 = ability_info->m_iParam[1] / 1000.f;
    const float parameter3 = ability_info->m_iParam[3] / 1000.f;

    int index = 0;
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_BASE_MELEE_MIN_ATTACK_POWER);
        // 물리 데미지 증감
        *value_slot = static_cast<int>(summoner_attr.GetBaseMeleeMinAttPower() * parameter0);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_BASE_MELEE_MAX_ATTACK_POWER);
        // 물리 데미지 증감
        *value_slot = static_cast<int>(summoner_attr.GetBaseMeleeMaxAttPower() * parameter0);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_BASE_RANGE_MIN_ATTACK_POWER);
        // 범위 데미지 증감
        *value_slot = static_cast<int>(summoner_attr.GetBaseRangeMinAttPower() * parameter0);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_BASE_RANGE_MAX_ATTACK_POWER);
        // 범위 데미지 증감
        *value_slot = static_cast<int>(summoner_attr.GetBaseRangeMaxAttPower() * parameter0);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_BASE_MELEE_DEFENSE_POWER);
        // 물리 방어력 증감 비율
        *value_slot = static_cast<int>(summoner_attr.GetBaseMeleeDefPower() * parameter1);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_BASE_RANGE_DEFENSE_POWER);
        // 범위 방어력 증감
        *value_slot = static_cast<int>(summoner_attr.GetBaseRangeDefPower() * parameter1);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type == eATTR_MOVE_SPEED);
        // 이동속도
        *value_slot = static_cast<int>(summoner_attr.GetBaseMoveSpeedRatio() * parameter3);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type == eATTR_ATTACK_SPEED);
        // 공격속도
        *value_slot = static_cast<int>(summoner_attr.GetBaseAttSpeedRatio() * parameter3);
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type ==
               eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO);
        // 공격성공 증감
        *value_slot = static_cast<int>(summoner_attr.GetBasePhysicalAttackRate() *
                                       (ability_info->m_iOption2 / 1000.f));
        ++index;
    };
    if (int* value_slot = &adrenaline_data->skill_attr_values[index])
    {
        assert(adrenaline_data->kSkillAttrTypeArray[index].attr_type == eATTR_MAX_HP);
        // 최대 HP 증감
        *value_slot = static_cast<int>(summoner_attr.GetMaxHP() *
                                       (ability_info->m_iOption1 / 1000.f));
        ++index;
    };
    if (FlowControl::FCAssert(index == _countof(adrenaline_data->skill_attr_values)) == false) {
        __debugbreak();
        return false;
    };
    return true;
};
//==================================================================================================
#if SUN_CODE_RENEWAL // CHANGES: f110808.5L, prevent the overlapped attribute calculation
bool SkillAttrCalculator::ApplyAdrenaline(bool applied, SLOTCODE skill_code,
    const nsSkill::AdrenalineConfiguration& adrenaline_data)
{
    __UNUSED(skill_code);
    const Attributes::Operation::eOper oper_type = applied ?
        attributes_.Operation::eOper_Add : attributes_.Operation::eOper_Sub;

    attributes_.GroupOperate(oper_type, eATTR_KIND_SKILL,
        adrenaline_data.kSkillAttrTypeArray, adrenaline_data.skill_attr_values,
        _countof(adrenaline_data.kSkillAttrTypeArray));
    attributes_.Update();
    //
    return true;
};
//==================================================================================================
#else //SUN_CODE_BACKUP
// NOTE: f110808.5L, custom logic,
// this method has implicit the overlapped attribute calculation problems
//==================================================================================================
bool
SkillAttrCalculator::ApplyAdrenaline(BOOL bApply, SLOTCODE skill_code,
                                     const Attributes& summoner_attr)
{
    const SkillScriptInfo* base_skill_info = \
        SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (base_skill_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               "SkillAttrCalculator::ApplyAdrenaline() Not Exist skill_code [Skill:%d]", 
               skill_code);
        return false;
    }

    const BASE_ABILITYINFO* ability_info = base_skill_info->GetAbilityInfo(eABILITY_ADRENALINE);
    // (f100721.1L)
    if (ability_info)
    {
        static const Attributes::Operation attr_skill_array[] = {
            eATTR_BASE_MELEE_MIN_ATTACK_POWER, eATTR_BASE_MELEE_MAX_ATTACK_POWER,
            eATTR_BASE_RANGE_MIN_ATTACK_POWER, eATTR_BASE_RANGE_MAX_ATTACK_POWER,
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            eATTR_BASE_MAGICAL_MIN_ATTACK_POWER, eATTR_BASE_MAGICAL_MAX_ATTACK_POWER,
            eATTR_BASE_MAGICAL_DEFENSE_POWER,
        #endif
            //
            eATTR_BASE_MELEE_DEFENSE_POWER, eATTR_BASE_RANGE_DEFENSE_POWER,
            //
            eATTR_MOVE_SPEED,
            eATTR_ATTACK_SPEED,
            eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,
            eATTR_MAX_HP
        };
        //
        const float parameter0 = ability_info->m_iParam[0] / 1000.f;
        const float parameter1 = ability_info->m_iParam[1] / 1000.f;
        const float parameter3 = ability_info->m_iParam[3] / 1000.f;

        const int value_array[_countof(attr_skill_array)] = {
            // 물리 데미지 증감
            static_cast<int>(summoner_attr.GetBaseMeleeMinAttPower() * parameter0),
            static_cast<int>(summoner_attr.GetBaseMeleeMaxAttPower() * parameter0),
            // 범위 데미지 증감
            static_cast<int>(summoner_attr.GetBaseRangeMinAttPower() * parameter0),
            static_cast<int>(summoner_attr.GetBaseRangeMaxAttPower() * parameter0),
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            // 마법 데미지 증감
            static_cast<int>(summoner_attr.GetBaseMagicMinAttPower() * parameter0),
            static_cast<int>(summoner_attr.GetBaseMagicMaxAttPower() * parameter0),
            // 마법 방어력 증감
            static_cast<int>(summoner_attr.GetBaseMagicDefPower() * parameter1),
        #endif
            // 물리 방어력 증감 비율
            static_cast<int>(summoner_attr.GetBaseMeleeDefPower() * parameter1),
            // 범위 방어력 증감
            static_cast<int>(summoner_attr.GetBaseRangeDefPower() * parameter1),
            // 이동속도
            static_cast<int>(summoner_attr.GetBaseMoveSpeedRatio() * parameter3),
            // 공격속도
            static_cast<int>(summoner_attr.GetBaseAttSpeedRatio() * parameter3),
            // 공격성공 증감
            static_cast<int>(summoner_attr.GetBasePhysicalAttackRate() *
                             (ability_info->m_iOption2 / 1000.f)),
            // 최대 HP 증감
            static_cast<int>(summoner_attr.GetMaxHP() * (ability_info->m_iOption1 / 1000.f))
        };
        //
        const Attributes::Operation::eOper oper_type \
            = bApply ? attr_skill_array->eOper_Add : attr_skill_array->eOper_Sub;

        attributes_.GroupOperate(oper_type, eATTR_KIND_SKILL,
                                 attr_skill_array, value_array,
                                 _countof(attr_skill_array));

        attributes_.Update();
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, 
               "SkillAttrCalculator::ApplyAdrenaline() Not Exist Ability [Ability:%d]", 
                eABILITY_ADRENALINE);
        return false;
    }
    return true;
}
#endif //SUN_CODE_BACKUP
//==================================================================================================

inline void
SkillAttrCalculator::_CalcAttrValue(bool is_add, AttrValue& attr_value,
                                    int value_type, int value)
{
    eATTR_KIND attr_kind = eATTR_KIND_SKILL;
    float calced_value = static_cast<float>(value);

    if (value_type == VALUE_TYPE_PERCENT_PER_MAX ||
        value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        attr_kind = eATTR_KIND_SKILL_RATIO;
        calced_value = value / 10.0f;
    }
    else if (value_type == VALUE_TYPE_RATIO_VALUE)
    {
        calced_value = value / 10.0f;
    }

    if (is_add) {
        attr_value[attr_kind] += static_cast<int>(calced_value);
    }
    else {
        attr_value[attr_kind] -= static_cast<int>(calced_value);
    }
}

//#define _WAVERIX_RIDER_TEST_
#ifdef _WAVERIX_RIDER_TEST_
static char rider_log_block[2048];
static int rider_log_written = 0;

    #define _WAVERIX_TEST_LOG(msg_proc) \
    { \
        rider_log_written += _snprintf msg_proc ; \
    }
    #define _WAVERIX_TEST_BUFFER_CTRL() \
        &rider_log_block[rider_log_written], _countof(rider_log_block) - rider_log_written

    #define _WAVERIX_TEST_LOG_CLEAR() \
    { \
        printf("%s\n", rider_log_block); \
        rider_log_written = 0; \
    }

#else

    #define _WAVERIX_TEST_LOG(msg_proc)
    #define _WAVERIX_TEST_BUFFER_CTRL()
    #define _WAVERIX_TEST_LOG_CLEAR()

#endif

bool
SkillAttrCalculator::ApplyRidingRider(bool is_apply, BOOL is_pc_room,
                                      const SCItemSlot& rider_item,
                                      const nsSlot::RiderUpdateBits& total_apply_x8bits)
{
    if (rider_item.IsRiderItem() == false) {
        return false;
    }
    BOOST_STATIC_ASSERT(0 < BASE_ITEMINFO::_MAX_OPTION_NUM &&
                            BASE_ITEMINFO::_MAX_OPTION_NUM <= sizeof(DWORD64));
    typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_EQUIP, BASE_ITEMINFO::_MAX_OPTION_NUM> \
        OPTION_EFFECT_EQUIP_MASK;

    AttrValue& speed_attr = attributes_.GetAttrValueRiderSpeedRatio();
    if (is_apply) {
        //speed_attr.Init();
        const BASE_ITEMINFO* item_info = rider_item.GetItemInfo();
        RidingInfoParser* const pRidingInfoParser = RidingInfoParser::Instance();
        const sRiderInfo* rider_info = pRidingInfoParser->GetInfo(item_info->m_wSkillCode);
        int base_rider_speed_ratio = rider_info ? rider_info->set_speed_ : 100;
        speed_attr[eATTR_KIND_BASE] = base_rider_speed_ratio;

        _WAVERIX_TEST_LOG((_WAVERIX_TEST_BUFFER_CTRL(),
                           "|IsApply=%d|SpeedAttrs|Calc=%d|Base=%d|Item=%d|Skill=%d"
                           "|ItemRatio=%d|SkillRatio=%d|TotalRatio=%d|\n",
                           is_apply,
                           (int)speed_attr[eATTR_KIND_CALC], (int)speed_attr[eATTR_KIND_BASE],
                           (int)speed_attr[eATTR_KIND_ITEM], (int)speed_attr[eATTR_KIND_SKILL],
                           (int)speed_attr[eATTR_KIND_ITEM_RATIO],
                           (int)speed_attr[eATTR_KIND_SKILL_RATIO],
                           (int)speed_attr[eATTR_KIND_RATIO]));
    }
    bool need_applied = false;

    // total_apply_x8bits : 0   - add apply rider option
    //                      1   - sub apply rider option
    //                      2~4 - add apply parts option
    //                      5~7 - sub apply parts option

    const BASE_ITEMINFO* rider_item_info = rider_item.GetItemInfo();

    bool rider_option_apply = true;
    if (is_apply) {
        if (total_apply_x8bits.add_rider == 0) {
            rider_option_apply = false;
        }
    }
    else {
        if (total_apply_x8bits.sub_rider == 0) {
            rider_option_apply = false;
        }
    }

    _WAVERIX_TEST_LOG((_WAVERIX_TEST_BUFFER_CTRL(),
                       "|TotalBits=%08X|Apply=%d|AddRider=%d|SubRider=%d|\n",
                       total_apply_x8bits.value, is_apply,
                       total_apply_x8bits.add_rider, total_apply_x8bits.sub_rider));

    if (rider_option_apply)
    {
        int upperbound_selector = is_pc_room ? 2 : 1; //Normal|PC option groups
        for (int selector = 0; selector < upperbound_selector; ++selector) {
            const BASE_ITEMINFO::OptionEffects* __option_effects = selector == 0 ?
                &rider_item_info->m_byOptionExEffect : &rider_item_info->m_byPCOptionExEffect;

            const BASE_ITEMINFO::OptionEffects& option_effects = *__option_effects;

            bool apply = ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_EQUIP_MASK::MASK, option_effects);
            if (apply == false) {
                continue;
            }
            const BASE_ITEMINFO::OptionIndexes* __option_indexes = selector == 0 ?
                &rider_item_info->m_wOptionIndex : &rider_item_info->m_wPCOptionIndex;
            const BASE_ITEMINFO::OptionTypes* __option_types = selector == 0 ?
                &rider_item_info->m_byOptionType : &rider_item_info->m_byPCOptionType;
            const BASE_ITEMINFO::OptionValues* __option_values = selector == 0 ?
                &rider_item_info->m_iOptionValue : &rider_item_info->m_iPCOptionValue;

            const BASE_ITEMINFO::OptionIndexes& option_indexes = *__option_indexes;
            const BASE_ITEMINFO::OptionTypes& option_types = *__option_types;
            const BASE_ITEMINFO::OptionValues& option_values = *__option_values;

            for (int op = 0; op < _countof(option_indexes); ++op)
            {
                bool is_valid_values = option_indexes[op] != 0 && 
                                       option_values[op] != 0 &&
                                       option_indexes[op] < _countof(g_ITEM_OPTION);
                if (is_valid_values == false) {
                    continue;
                }
                const eATTR_TYPE attr_type = g_ITEM_OPTION[option_indexes[op]];
                _WAVERIX_TEST_LOG((_WAVERIX_TEST_BUFFER_CTRL(),
                                   "|  |%sRider|ArrayIndex=%d"
                                   "|OptionIndex=%d|OptionType=%d|OptionValue=%d|PC=%d|\n",
                                   is_apply ? "Add" : "Sub",
                                   op, option_indexes[op], attr_type, option_values[op],
                                   selector));

                if (attr_type == eATTR_MOVE_SPEED) {
                    _CalcAttrValue(is_apply, speed_attr, option_types[op], option_values[op]);
                    continue;
                }

                if (_CanApplyRiderOption(attr_type) == false) {
                    continue;
                }
                if (is_apply) {
                    AddAttr(attr_type, option_types[op], option_values[op], true /*skip update*/);
                }
                else {
                    DelAttr(attr_type, option_types[op], option_values[op], true /*skip update*/);
                }
                need_applied = true;
            } //end 'for'
        } //end 'for' selector
    }

    // total_apply_x8bits : 0   - add apply rider option
    //                      1   - sub apply rider option
    //                      2~4 - add apply parts option
    //                      5~7 - sub apply parts option

    BYTE parts_apply = total_apply_x8bits.value >> 2; //skip 0,1
    // ---- current state : can't used total_apply_x8bits.(add|sub)_xxx
    if (is_apply == false) {
        parts_apply >>= 3;
    }
    eRIDER_PARTS_POS pos = { 0, };
    BOOST_STATIC_ASSERT(pos.Counts == 3);
    SCRiderPartsData parts_data_block[pos.Counts] = { 0, }; // (CHANGES) support set item (full option)
    int set_apply_counts[pos.Counts] = { 0, };

    for (int parts_inc = 0; parts_inc < pos.Counts; ++parts_inc) {
        SCRiderPartsData& parts_data = parts_data_block[parts_inc];
        bool apply_step = !!(parts_apply & 1);
        parts_apply >>= 1;

        pos.value = static_cast<BYTE>(parts_inc) + pos.StartIndex;
        if (rider_item.GetRiderPartsInfo(pos, &parts_data) == 0) {
            continue;
        }
        // empty도 성공으로 반환한다. 또는 하기와 같은 상황에서는 item 정보가 없는 경우도 존재
        if (parts_data.item_info_ == NULL) {
            continue;
        }
        // timeout된 경우 감안해서 no-count처리를 수행한다.
        // add의 경우는 호출자 쪽에서 timeout이 감안된다.
        if (parts_data.item_info_->set_group_code != 0) {
            ++set_apply_counts[parts_inc];
        }
        // apply option
        if (apply_step == false) {
            continue;
        }
        // Client 처리상의 문제:
        //  클라이언트도 SCItemSlot::Update가 수행되며, 클라 로컬 시간을 기준으로 is_expired와 같은
        //  상태 변경이 발생할 수 있다.
        // 입력 인자인 parts_apply를 이용해서 

        // 이 차이점을 이용해서 클라이언트는 이전 상태를 제거할 경우 정확히 제거가 가능할 것이다.
        // 이후 서버로부터 받은 정보로 Add처리를 하면, 갱신된 정보로 적용이 가능하다.

        const BASE_ITEMINFO* parts_item_info = parts_data.item_info_;

        int upperbound_selector = is_pc_room ? 2 : 1; //Normal|PC option groups
        for (int selector = 0; selector < upperbound_selector; ++selector) {

            const BASE_ITEMINFO::OptionEffects* __option_effects = selector == 0 ?
                &parts_item_info->m_byOptionExEffect : &parts_item_info->m_byPCOptionExEffect;

            const BASE_ITEMINFO::OptionEffects& option_effects = *__option_effects;

            bool apply = ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_EQUIP_MASK::MASK, option_effects);
            if (apply == false) {
                continue;
            }
            const BASE_ITEMINFO::OptionIndexes* __option_indexes = selector == 0 ?
                &parts_item_info->m_wOptionIndex : &parts_item_info->m_wPCOptionIndex;
            const BASE_ITEMINFO::OptionTypes* __option_types = selector == 0 ?
                &parts_item_info->m_byOptionType : &parts_item_info->m_byPCOptionType;
            const BASE_ITEMINFO::OptionValues* __option_values = selector == 0 ?
                &parts_item_info->m_iOptionValue : &parts_item_info->m_iPCOptionValue;

            const BASE_ITEMINFO::OptionIndexes& option_indexes = *__option_indexes;
            const BASE_ITEMINFO::OptionTypes& option_types = *__option_types;
            const BASE_ITEMINFO::OptionValues& option_values = *__option_values;

            for (int op = 0; op < _countof(option_indexes); ++op)
            {
                bool is_valid_values = option_indexes[op] != 0 && 
                                       option_values[op] != 0 &&
                                       option_indexes[op] < _countof(g_ITEM_OPTION);
                if (is_valid_values == false) {
                    continue;
                }
                const eATTR_TYPE attr_type = g_ITEM_OPTION[option_indexes[op]];
                _WAVERIX_TEST_LOG((_WAVERIX_TEST_BUFFER_CTRL(),
                                   "|  |%sParts%d|ArrayIndex=%d"
                                   "|OptionIndex=%d|OptionType=%d|OptionValue=%d|PC=%d|\n",
                                   is_apply ? "Add" : "Sub", parts_inc,
                                   op, option_indexes[op], attr_type, option_values[op], selector));

                if (attr_type == eATTR_MOVE_SPEED) {
                    _CalcAttrValue(is_apply, speed_attr, option_types[op], option_values[op]);
                    continue;
                }

                if (_CanApplyRiderOption(attr_type) == false) {
                    continue;
                }
                if (is_apply) {
                    AddAttr(attr_type, option_types[op], option_values[op], true /*skip update*/);
                }
                else {
                    DelAttr(attr_type, option_types[op], option_values[op], true /*skip update*/);
                }
                need_applied = true;
            } //end 'for'
        } //end 'for' selector
    } //end 'for' number of parts

    //--------------------------------------------------------------------------
    // (NOTE) SetItem효과는 FullOption만 지원한다. 기존 ItemAttrCalculator에서
    // 개별 옵션 적용이 되는 이유는 모든 스탯 초기화 후에 재계산을 수행하기 때문
    parts_apply = total_apply_x8bits.value >> 2; //skip 0,1
    // ---- current state : can't used total_apply_x8bits.(add|sub)_xxx
    if (is_apply == 0) {
        parts_apply >>= 3;
    }
    parts_apply &= 0x7;

    if (parts_apply)
    {
        SetItemOptionInfoParser* set_item_parser = SetItemOptionInfoParser::Instance();
        const SETITEM_OPTION_INFO* setitem_option = 0;
        for (SCRiderPartsData* it_1st = parts_data_block;
             it_1st != &parts_data_block[_countof(parts_data_block)];
             ++it_1st)
        {
            if (it_1st->item_info_ == 0) {
                continue;
            }
            WORD set_type_1st = it_1st->item_info_->set_group_code;
            if (set_type_1st == 0) {
                continue;
            }
            if (it_1st->item_info_->m_bySetOptionType != it_1st->item_info_->eSetItem_Active) {
                continue;
            }
            setitem_option = set_item_parser->Find(set_type_1st);
            if (setitem_option == 0) {
                continue;
            }
            // n^2 searching
            int same_set_types = 0;
            for (SCRiderPartsData* it_2nd = parts_data_block;
                 it_2nd != &parts_data_block[_countof(parts_data_block)];
                 ++it_2nd)
            {
                if (it_2nd->item_info_ == 0) {
                    continue;
                }
                //
                WORD set_type_2nd = it_2nd->item_info_->set_group_code;
                if (set_type_2nd == 0) {
                    continue;
                }
                if (it_2nd->item_info_->m_bySetOptionType != it_2nd->item_info_->eSetItem_Active) {
                    continue;
                }
                if (set_type_1st == set_type_2nd) {
                    ++same_set_types;
                }
            }
            // full
            if (setitem_option->m_byActiveFullOptionNum == same_set_types) {
                break;
            }
            setitem_option = 0;
        }

        // (NOTE) add상태는 변경 후의 계산, sub상태는 변경 전의 계산이다.
        // 따라서 sub일 경우라면 full상태일 경우가 사라지는 상황이므로 full검사로 옵션 제거가 가능.

        if (setitem_option)
        {
            for (int full_inc = 0; full_inc < _countof(setitem_option->m_FullOption); ++full_inc)
            {
                const SET_ITEM_OPTION& full_option = setitem_option->m_FullOption[full_inc];
            
                bool is_valid_values = full_option.m_byOptionKind != 0 && 
                                       full_option.m_nOption != 0 &&
                                       full_option.m_byOptionKind < _countof(g_ITEM_OPTION);
                if (is_valid_values == false) {
                    continue;
                }
                const eATTR_TYPE attr_type = g_ITEM_OPTION[full_option.m_byOptionKind];
                _WAVERIX_TEST_LOG((_WAVERIX_TEST_BUFFER_CTRL(),
                                   "|  |FullOption_%s|Index=%d"
                                   "|OptionIndex=%d|OptionType=%d|OptionValue=%d|\n",
                                   is_apply ? "Add" : "Sub", full_inc,
                                   full_option.m_byOptionKind, attr_type, full_option.m_nOption));

                if (attr_type == eATTR_MOVE_SPEED) {
                    _CalcAttrValue(is_apply, speed_attr, full_option.m_byUseType, full_option.m_nOption);
                    continue;
                }

                if (_CanApplyRiderOption(attr_type) == false) {
                    continue;
                }
                if (is_apply) {                                                   /*skip update*/
                    AddAttr(attr_type, full_option.m_byUseType, full_option.m_nOption, true);
                }
                else {                                                            /*skip update*/
                    DelAttr(attr_type, full_option.m_byUseType, full_option.m_nOption, true);
                }
                need_applied = true;
            }
        } //end apply set
    }
    //--------------------------------------------------------------------------

#ifdef _SERVER
    if (need_applied) {
        attributes_.UpdateEx(); // (NOTE) ...좀 더 생각해 보자.
    }
    else {
        speed_attr.Update();
    }
#else
    if (need_applied) {
        attributes_.Update();
    }
    else {
        speed_attr.Update();
    }
#endif

    _WAVERIX_TEST_LOG((_WAVERIX_TEST_BUFFER_CTRL(),
                       "|IsApply=%d|SpeedAttrs|Calc=%d|Base=%d|Item=%d|Skill=%d"
                       "|ItemRatio=%d|SkillRatio=%d|TotalRatio=%d|\n",
                       is_apply,
                       (int)speed_attr[eATTR_KIND_CALC], (int)speed_attr[eATTR_KIND_BASE],
                       (int)speed_attr[eATTR_KIND_ITEM], (int)speed_attr[eATTR_KIND_SKILL],
                       (int)speed_attr[eATTR_KIND_ITEM_RATIO],
                       (int)speed_attr[eATTR_KIND_SKILL_RATIO],
                       (int)speed_attr[eATTR_KIND_RATIO]));
    _WAVERIX_TEST_LOG_CLEAR();

    return true;
}

#ifdef _NA_008220_20150414_BUG_ETHERBULLET_ATTR
float SkillAttrCalculator::ClearAttr( eATTR_TYPE attr_type, int value_type, bool skip_update /*= false*/ )
{
    if (attr_type <= eATTR_TYPE_INVALID || eATTR_MAX <= attr_type) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|attr_type|%d|invalid type value", attr_type);
        return 0;
    }

    if (!IsRecoverAttr(attr_type)) {
        return 0;
    }

    eATTR_KIND attr_kind = eATTR_KIND_SKILL;
    if (value_type == VALUE_TYPE_PERCENT_PER_MAX || value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        attr_kind = eATTR_KIND_SKILL_RATIO;
    }

    // (REMARK) (WAVERIX) 이 위치가 AddAttr과 코드가 다르다. 정확한 이유를 파악할 필요가 있다.
    static const Attributes::Operation attr_array_for_all_stats[] = {
        eATTR_STR, eATTR_DEX, eATTR_VIT, eATTR_INT, eATTR_SPR
    };

    switch (attr_type)
    {
    case eATTR_INCREASE_STAT_POINT:
        {
            // (f100721.1L)
            int value_array[_countof(attr_array_for_all_stats)] = {0, 0, 0, 0, 0};
            //
            attributes_.GroupOperate(attr_array_for_all_stats->eOper_Sub, attr_kind,
                attr_array_for_all_stats, value_array,
                _countof(attr_array_for_all_stats));
        }
        break;
    default:
        {
            attributes_[attr_type][attr_kind] = 0;
        }
        break;
    }

    // (CHANGES) (WAVERIX)
    if (skip_update == false) {
        attributes_.UpdateEx();
    }
    return 0;
}
#endif //_NA_008220_20150414_BUG_ETHERBULLET_ATTR
