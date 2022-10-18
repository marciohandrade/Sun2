#pragma once
#ifndef GAMESERVER_SUMMON_CRYSTAL_WARP_ABILITY_H
#define GAMESERVER_SUMMON_CRYSTAL_WARP_ABILITY_H

#include "Ability.h"

//==================================================================================================

class SummonCrystalWarpAbility : public Ability
{
public:
    SummonCrystalWarpAbility() {}
    virtual ~SummonCrystalWarpAbility() {}

    //virtual void Init(Skill* skill, const BASE_ABILITYINFO* base_ability_info);
    //virtual void InitDetailed(Character* attacker,
    //                          SLOTCODE skill_code, SkillInfo* skill_info_data,
    //                          BYTE skill_stat_type, const BASE_ABILITYINFO* base_ability_info);

    //virtual void Release(Character* target);
    ////
    virtual eABILITY_TYPE GetAbilityType() const;
    //virtual bool IsValidStateID() const;
    ////
    //virtual bool CanExecute(Character* attacker,
    //                        Character* target, DWORD main_target_key, WzVector main_target_pos);
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
    virtual bool ExecuteEffect(BYTE* pMsg, WORD& wMsgSize);
    //virtual void SetBonusEffect(BONUS_ABILITY_EFFECT* bonus_ability_effect);
};

//==================================================================================================

//virtual
inline eABILITY_TYPE SummonCrystalWarpAbility::GetAbilityType() const {
    return eABILITY_TYPE_EFFECT;
}

//virtual
inline bool SummonCrystalWarpAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    __UNUSED((target, pMsg, wMsgSize));
    return false;
}

#endif //GAMESERVER_SUMMON_CRYSTAL_WARP_ABILITY_H