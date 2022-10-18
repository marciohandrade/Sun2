#pragma once
#ifndef GAMESERVER_ABSORB_ABILITY_H
#define GAMESERVER_ABSORB_ABILITY_H

#include "BaseStatusAbility.h"

//==================================================================================================
// 피격 데미지 HP/SP/MP 흡수.
class AbsorbAbility : public BaseStatusAbility
{
public:
    AbsorbAbility() {}
    virtual ~AbsorbAbility() {}

    //virtual void Init(Skill* skill, const BASE_ABILITYINFO* base_ability_info);
    //virtual void InitDetailed(Character* attacker,
    //                          SLOTCODE skill_code, SkillInfo* skill_info_data,
    //                          BYTE skill_stat_type, const BASE_ABILITYINFO* base_ability_info);

    //virtual void Release(Character* target);
    ////
    //virtual eABILITY_TYPE GetAbilityType() const;
    virtual bool IsValidStateID() const;
    ////
    //virtual bool CanExecute(Character* attacker,
    //                        Character* target, DWORD main_target_key, WzVector main_target_pos);
    //virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
    //virtual bool ExecuteEffect(BYTE* pMsg, WORD& wMsgSize);
    //virtual void SetBonusEffect(BONUS_ABILITY_EFFECT* bonus_ability_effect);
};

//==================================================================================================
// implements

//virtual
inline bool AbsorbAbility::IsValidStateID() const
{
    switch (GetStateID())
    {
    case eCHAR_STATE_ABSORB: case eCHAR_STATE_IMMUNITY_DAMAGE: //CHANGUP_IMMUNITY_DAMAGE_STATUS
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    case eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE:
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
        return true;
    default:
        return false;
    }
}

#endif //GAMESERVER_ABSORB_ABILITY_H