#pragma once

#ifndef GAMESERVER_DRAGON_TRANSFORMATION_ABILITY_H_
#define GAMESERVER_DRAGON_TRANSFORMATION_ABILITY_H_

#include "BaseStatusAbility.h"


class DragonTransAbility : public BaseStatusAbility
{
public:
    DragonTransAbility() 
    {
    }
    virtual ~DragonTransAbility() 
    {
    }

    virtual bool IsValidStateID() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_TRANSFORMATION:
            return true;
        default:
            return false;
        }
    }
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
};

#endif // GAMESERVER_DRAGON_TRANSFORMATION_ABILITY_H_