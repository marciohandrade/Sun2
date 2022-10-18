#pragma once
#ifndef GAMESERVER_CANCEL_STATUS_ABILITY_H
#define GAMESERVER_CANCEL_STATUS_ABILITY_H

#include "Ability.h"

//==================================================================================================

class CancelStatusAbility : public Ability
{
public:
    CancelStatusAbility(void);
    virtual ~CancelStatusAbility(void);

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
inline bool CancelStatusAbility::IsValidStateID() const
{
    switch (GetStateID())
    {
    case eCHAR_STATE_CANCELATION:
        // (WAVERIX) (090313) CancelStatusAbility에 적법한 상태 추가
    case eCHAR_STATE_BUFF_CANCEL:
        return true;
    default:
        return false;
    }
}

#endif //GAMESERVER_CANCEL_STATUS_ABILITY_H