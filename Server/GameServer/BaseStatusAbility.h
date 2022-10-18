#pragma once

#ifndef GAMESERVER_BASE_STATUS_ABILITY_H_
#define GAMESERVER_BASE_STATUS_ABILITY_H_

#include "Ability.h"


class BaseStatusAbility : public Ability
{
public:
    BaseStatusAbility() 
    {
    }
    virtual ~BaseStatusAbility() 
    {
    }

    virtual bool CanExecute(
        Character* attacker,
        Character* target, 
        DWORD main_target_key, 
        WzVector main_target_pos
        );

    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
};

#endif // GAMESERVER_BASE_STATUS_ABILITY_H_