#pragma once
#ifndef GAMESERVER_TRUST_ABILITY_H
#define GAMESERVER_TRUST_ABILITY_H

#include "BaseStatusAbility.h"

class SelfDestructionDamageEffect;

//==================================================================================================

class ThrustAbility : public BaseStatusAbility
{
public:
    ThrustAbility() {}
    virtual ~ThrustAbility() {}

    //virtual void Init(Skill* skill, const BASE_ABILITYINFO* base_ability_info);
    virtual void InitDetailed(Character* attacker,
                              SLOTCODE skill_code, SkillInfo* skill_info_data,
                              BYTE skill_stat_type, const BASE_ABILITYINFO* base_ability_info);

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

private:
    enum eControl {
        eControl_None               = 0,
        eControl_Thrust             = 1<< 0,
        eControl_SelfDestruction    = 1<< 1,    // 자폭은 넉백과 데미지를 동반한다.
        eControl_DamageStarted      = 1<<20,
    };
    //
    bool _Thrust(Character* const target, BYTE* pMsg, WORD& wMsgSize);
    bool _DamageEffect(Character* target, GameField* const game_field);

    DWORD control_field_;
    GameField* game_field_for_integrity_;
    SelfDestructionDamageEffect* m_pEffect;
    WzVector m_wvMainTargetPos;
};

//==================================================================================================

//virtual
inline bool ThrustAbility::IsValidStateID() const {
    return true;
}

#endif