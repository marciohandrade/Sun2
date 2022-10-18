#ifndef _SERVER_GAMESERVER_SKILLSYSTEM_ABILITY_CURRENTHPINCREASEABILITY_H
#define _SERVER_GAMESERVER_SKILLSYSTEM_ABILITY_CURRENTHPINCREASEABILITY_H

#include "BaseStatusAbility.h"

class CurrnetHPIncreaseAbility : public BaseStatusAbility
{
public:
    CurrnetHPIncreaseAbility() {}
    virtual ~CurrnetHPIncreaseAbility() {}

    //virtual void Init(Skill* skill, const BASE_ABILITYINFO* base_ability_info);
    //virtual void InitDetailed(Character* attacker,
    //                          SLOTCODE skill_code, SkillInfo* skill_info_data,
    //                          BYTE skill_stat_type, const BASE_ABILITYINFO* base_ability_info);

    //virtual void Release(Character* target);
    ////
    //virtual eABILITY_TYPE GetAbilityType() const;
    virtual bool IsValidStateID() const;
    ////
    virtual bool CanExecute(Character* attacker,
                            Character* target, DWORD main_target_key, WzVector main_target_pos);
    //virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
    //virtual bool ExecuteEffect(BYTE* pMsg, WORD& wMsgSize);
    //virtual void SetBonusEffect(BONUS_ABILITY_EFFECT* bonus_ability_effect);
};


bool CurrnetHPIncreaseAbility::CanExecute(Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos)
{
    if (Ability::CanExecute(attacker, target, main_target_key, main_target_pos) == false) {
        return false;
    };

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    // 소환 몬스터는 힐 불가. arycoat.2012.02.02
    if (attacker->IsEqualObjectKind(PLAYER_OBJECT) &&
        target->IsEqualObjectKind(SUMMON_OBJECT)) {
        return false;
    }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

    return true;
};

bool CurrnetHPIncreaseAbility::IsValidStateID() const
{
    if (GetAttrID() == eATTR_TYPE_INVALID && GetStateID() == eCHAR_STATE_INVALID)  
        return false;

    return true;
}

#endif //_SERVER_GAMESERVER_SKILLSYSTEM_ABILITY_CURRENTHPINCREASEABILITY_H