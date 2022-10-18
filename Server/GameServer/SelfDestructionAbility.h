#pragma once

#ifndef GAMESERVER_SELF_DESTRUCTION_ABILITY_H_
#define GAMESERVER_SELF_DESTRUCTION_ABILITY_H_

#include "Ability.h"


class SelfDestructionAbility : public Ability
{
public:
    SelfDestructionAbility()
    {
    }
    virtual ~SelfDestructionAbility()
    {
    }

public:
    virtual bool CanExecute(
        Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos);
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
};

#endif // GAMESERVER_SELF_DESTRUCTION_ABILITY_H_