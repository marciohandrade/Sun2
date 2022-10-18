#pragma once

#ifndef GAMESERVER_BUFF_STATUS_ABILITY_H_
#define GAMESERVER_BUFF_STATUS_ABILITY_H_

#include "BaseStatusAbility.h"


class BuffStatusAbility : public BaseStatusAbility
{
public:
    BuffStatusAbility() 
    {
    }
    virtual ~BuffStatusAbility() 
    {
    }
public:
    virtual bool IsValidStateID() const
    {
        if (GetAttrID() == eATTR_TYPE_INVALID && GetStateID() == eCHAR_STATE_INVALID)
        {
            return false;
        }
        return true;
    }
};

//==================================================================================================
// Toggle FP ability
//
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
class ToggleFPAbility : public BuffStatusAbility
{
public:
    ToggleFPAbility() {}
    virtual ~ToggleFPAbility() {}

    virtual eABILITY_TYPE GetAbilityType() const;
    virtual bool IsValidStateID() const;

    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);

private:
};
#endif //_NA_007667_20141001_WITCHBLADE_SKILL


#endif // GAMESERVER_BUFF_STATUS_ABILITY_H_