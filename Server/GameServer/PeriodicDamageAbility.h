#pragma once

#ifndef GAMESERVER_PERIODIC_DAMAGE_ABILITY_H_
#define GAMESERVER_PERIODIC_DAMAGE_ABILITY_H_

#include "BaseStatusAbility.h"


class PeriodicDamageAbility : public BaseStatusAbility
{
public:
    PeriodicDamageAbility() 
    {
    }
    virtual ~PeriodicDamageAbility() 
    {
    }

    virtual bool IsValidStateID() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_POISON:    
        case eCHAR_STATE_WOUND: 
        case eCHAR_STATE_FIRE_WOUND:
        case eCHAR_STATE_PAIN:      
        case eCHAR_STATE_FIRE_WOUND2:   
        case eCHAR_STATE_PAIN2:
        case eCHAR_STATE_POISON2:
        case kCharStatePoison3:
        case eCHAR_STATE_PERIODIC_DAMAGE: // EP2: default value
        case eCHAR_STATE_PHOENIX_BURN:
        case eCHAR_STATE_ELECTRICSHOCK:
            return true;
        }
        return false;
    }
};

#endif // GAMESERVER_PERIODIC_DAMAGE_ABILITY_H_