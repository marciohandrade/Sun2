#include "stdafx.h"
#include "./BonusDamagePerStatusAbility.h"

#include "StateInfoParser.h"
//
#include "StatusManager.h"
#include "Skill.h"
//==================================================================================================

//virtual
bool BonusDamagePerStatusAbility::CanExecute(
    Character* attacker, Character* target,
    DWORD main_target_key, WzVector main_target_pos)
{
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    __UNUSED((attacker, target, main_target_key, main_target_pos));
    return false;
#endif

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    if (Ability::CanExecute(attacker, target, main_target_key, main_target_pos) == false) {
        return false;
    };
    assert(GetSKill() != NULL);

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

    //
    //if (base_ability_info_->m_byRangeType != SKILL_ABILITY_ME)
    //{
    //    SUNLOG(eCRITICAL_LOG,
    //           _T("|["__FUNCTION__"]|Msg=failed, this skill(%d)'s ability(%d) can only apply to me"),
    //           skill_code_, base_ability_info_->m_wAbilityID);
    //    return false;
    //}
    // [Option1] = StateID of Attacker, [Option2] = StateID of Target
    // [Option1] == 0 && [Option2] == 0 |=> fails
    // [Option1] == 0 && [Option2] != 0 |=> check current defender state
    // [Option1] != 0 && [Option2] == 0 |=> check current attacker state
    // [Option1] != 0 && [Option2] != 0 |=> AND condition, check current attacker and defender state
    const eCHAR_STATE_TYPE attacker_state_id =
        static_cast<eCHAR_STATE_TYPE>(ability_info->m_iOption1);
    const eCHAR_STATE_TYPE defender_state_id =
        static_cast<eCHAR_STATE_TYPE>(ability_info->m_iOption2);

    bool fails = (attacker_state_id == eCHAR_STATE_INVALID &&
                  defender_state_id == eCHAR_STATE_INVALID);
    if (fails == false)
    {
        StateInfoParser* state_parser = StateInfoParser::Instance();
        const BASE_STATEINFO* attacker_state_info
            = attacker_state_id != eCHAR_STATE_INVALID
            ?   state_parser->GetStateInfo(attacker_state_id)
            :   NULL;
        const BASE_STATEINFO* defender_state_info
            = defender_state_id != eCHAR_STATE_INVALID
            ?   state_parser->GetStateInfo(defender_state_id)
            :   NULL;
        //
        fails = (attacker_state_info == NULL && defender_state_info == NULL);
        if (fails == false) {
            fails = (attacker_state_info == NULL && attacker_state_id != eCHAR_STATE_INVALID);
        }
        if (fails == false) {
            fails = (defender_state_info == NULL && defender_state_id != eCHAR_STATE_INVALID);
        };

    };
    if (fails)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=failed, this skill(%d)'s ability(%d) can accept ")
               _T("(Option1 != 0 AND Option2 == 0) OR")
               _T("(Option1 == 0 AND Option2 != 0) OR")
               _T("(Option1 != 0 AND Option2 != 0). ")
               _T("but, input arguments are Option1(%d) and Option2(%d)|"),
               GetSkillCode(), ability_info->m_wAbilityID,
               attacker_state_id, defender_state_id);
        return false;
    };
    //
    //assert(fails == false);
    if (attacker_state_id != eCHAR_STATE_INVALID)
    {
        StatusManager* status_manager = attacker->GetStatusManager();
        BaseStatus* status = status_manager->FindStatus(attacker_state_id);
        if (status == NULL) {
            fails = true;
        };
    };
    if (defender_state_id != eCHAR_STATE_INVALID)
    {
        StatusManager* status_manager = target->GetStatusManager();
        BaseStatus* status = status_manager->FindStatus(defender_state_id);
        if (status == NULL) {
            fails = true;
        };
    };
    // 
    // base_ability_info_->m_iParam[0] == value_type;
    // base_ability_info_->m_iParam[1] == value;
    // NOTE: add additional attack power,
    if (fails == false) {
        int added_attack_power = GetSKill()->GetAddedAttackPowerPerStatus();
        ;   added_attack_power += ability_info->m_iParam[1];
        GetSKill()->SetAddedAttackPowerPerStatus(added_attack_power);
    };
    //
    return true;
#endif
}

//virtual
bool BonusDamagePerStatusAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    __TOUCHED((target, pMsg));
    wMsgSize = 0;
    return true;
#else
    __UNUSED((target, pMsg, wMsgSize));
    wMsgSize = 0;
    return false;
#endif
}
