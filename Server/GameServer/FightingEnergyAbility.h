#pragma once

#ifndef GAMESERVER_FIGHTING_ENERGY_ABILITY_H_
#define GAMESERVER_FIGHTING_ENERGY_ABILITY_H_

#include "Ability.h"


class FightingEnergyAbility : public Ability
{
public:
    FightingEnergyAbility() 
    {
    }
    virtual ~FightingEnergyAbility() 
    {
    }

    virtual bool IsValidStateID() const;
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
};

#endif // GAMESERVER_FIGHTING_ENERGY_ABILITY_H_