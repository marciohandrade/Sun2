#pragma once

#ifndef GAMESERVER_ABNORMAL_ABILITY_H_
#define GAMESERVER_ABNORMAL_ABILITY_H_

#include "BaseStatusAbility.h"


class AbnormalAbility : public BaseStatusAbility
{
public:
    AbnormalAbility() 
    {
    }
    virtual ~AbnormalAbility() 
    {
    }
public:
    virtual eABILITY_TYPE GetAbilityType() const
    {
        if (GetStateID() == eCHAR_STATE_POISON2) 
        {
            return eABILITY_TYPE_MANUAL;
        }
        return eABILITY_TYPE_ACTIVE;
    }
    virtual bool IsValidStateID() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_CHAOS:     
        case eCHAR_STATE_BLIND:         
        case eCHAR_STATE_SEALING:
        case eCHAR_STATE_WEAKENING: 
        case eCHAR_STATE_FRUSTRATION:   
        case eCHAR_STATE_FETTER:
        case eCHAR_STATE_SLOWDOWN:  
        case eCHAR_STATE_FEAR:          
        case eCHAR_STATE_SEQUELA:
        case eCHAR_STATE_ATTACK_IMPOSSIBLE:
        case eCHAR_STATE_PROTECTION:
        case eCHAR_STATE_MP_FEAR2:
        case eCHAR_STATE_POISON2:
        case eCHAR_STATE_STAMP:
        case eCHAR_STATE_DETECTING_HIDE:
        case eCHAR_STATE_WHITE_BLIND:
        case eCHAR_STATE_DARK_OF_LIGHT_ZONE:
        case eCHAR_STATE_POLYMORPH:
        case eCHAR_STATE_PROTECTION1:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_STATE_CHARMED:
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            return true;
        }
        return false;
    }
};

#endif // GAMESERVER_ABNORMAL_ABILITY_H_