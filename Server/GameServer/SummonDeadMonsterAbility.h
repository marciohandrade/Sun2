#pragma once

#ifndef SUMMON_DEAD_MONSTER_ABILITY_H_
#define SUMMON_DEAD_MONSTER_ABILITY_H_

#include "Ability.h"


class SummonDeadMonsterAbility : public Ability
{
public:
    SummonDeadMonsterAbility()
    {
    }
    virtual ~SummonDeadMonsterAbility()
    {
    }

    virtual bool CanExecute(
        Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos);
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
};

#endif // SUMMON_DEAD_MONSTER_ABILITY_H_