#pragma once

#ifndef GAMESERVER_WEAPON_MASTERY_ABILITY_H_
#define GAMESERVER_WEAPON_MASTERY_ABILITY_H_

#include "Ability.h"


class WeaponMasteryAbility : public Ability
{
public:
    WeaponMasteryAbility() 
    {
    }
    virtual ~WeaponMasteryAbility() 
    {
    }

public:
    virtual void Release(Character* target);
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
};

#endif // GAMESERVER_WEAPON_MASTERY_ABILITY_H_