#pragma once

#ifndef GAMESERVER_DAMAGE_ABILITY_H_
#define GAMESERVER_DAMAGE_ABILITY_H_

#include "Ability.h"

struct BONUS_ABILITY_EFFECT;


class DamageAbility : public Ability
{
public:
    DamageAbility() : 
        skill_stat_type_(0), skill_effect_notify_link_(NULL)
    {
    }
    virtual ~DamageAbility() 
    {
    }

    virtual void InitDetailed(
        Character* attacker,
        SLOTCODE skill_code, 
        SkillInfo* skill_info_data,
        BYTE skill_stat_type, 
        const BASE_ABILITYINFO* base_ability_info
    );
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);

private:
    int CalcAddDamage(const int skill_attack_power, Character* const attacker) const;
    
    BYTE skill_stat_type_;
    BYTE* skill_effect_notify_link_;
};

#endif // GAMESERVER_DAMAGE_ABILITY_H_