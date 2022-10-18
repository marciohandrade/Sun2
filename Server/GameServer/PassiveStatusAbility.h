#pragma once

#ifndef GAMESERVER_PASSIVE_STATUS_ABILITY_H_
#define GAMESERVER_PASSIVE_STATUS_ABILITY_H_

#include "Ability.h"


class PassiveStatusAbility : public Ability
{
public:
    PassiveStatusAbility() {}
    virtual ~PassiveStatusAbility() {}

public:
    virtual void Release(Character* target);
    virtual eABILITY_TYPE GetAbilityType() const
    {
        return eABILITY_TYPE_PASSIVE;
    }
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
};

#endif // GAMESERVER_PASSIVE_STATUS_ABILITY_H_