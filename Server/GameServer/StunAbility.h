#pragma once
#ifndef GAMESERVER_STUN_ABILITY_H
#define GAMESERVER_STUN_ABILITY_H

#include "BaseStatusAbility.h"

//==================================================================================================

class StunAbility : public BaseStatusAbility
{
public:
    StunAbility() {}
    virtual ~StunAbility() {}

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
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
    //virtual bool ExecuteEffect(BYTE* pMsg, WORD& wMsgSize);
    //virtual void SetBonusEffect(BONUS_ABILITY_EFFECT* bonus_ability_effect);
};

//==================================================================================================

//virtual
inline bool StunAbility::IsValidStateID() const
{
    switch (GetStateID())
    {
    case eCHAR_STATE_DOWN:      case eCHAR_STATE_DELAY:     case eCHAR_STATE_STUN:
    case eCHAR_STATE_FROZEN:    case eCHAR_STATE_HOLDING:   case eCHAR_STATE_STONE:
    case eCHAR_STATE_SLEEP: 
    case eCHAR_STATE_STUN2:     //다크스턴..
    case eCHAR_STATE_FATAL_POINT:
    case eCHAR_STATE_UPPERDOWN: // _NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN
        return true;
    default:
        return false;
    }
}

#endif