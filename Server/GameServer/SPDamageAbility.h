#pragma once

#ifndef GAMESERVER_SP_DAMAGE_ABILITY_H_
#define GAMESERVER_SP_DAMAGE_ABILITY_H_

#include "Ability.h"


class SPDamageAbility : public Ability
{
public:
    SPDamageAbility() 
    {
    }
    virtual ~SPDamageAbility() 
    {
    }

public:
    virtual void Release(Character* target);
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
};

#endif // GAMESERVER_SP_DAMAGE_ABILITY_H_