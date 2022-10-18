#pragma once

#ifndef GAMESERVER_CHANGE_ATTR_ABILITY_H_
#define GAMESERVER_CHANGE_ATTR_ABILITY_H_

#include "BaseStatusAbility.h"


class ChangeAttrAbility : public BaseStatusAbility
{
public:
    ChangeAttrAbility()
    {
    }
    virtual ~ChangeAttrAbility()
    {
    }

    virtual bool IsValidStateID() const;
    virtual bool CanExecute(
        Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos);
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
};

#endif // GAMESERVER_CHANGE_ATTR_ABILITY_H_