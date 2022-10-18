#ifndef GAMESERVER_TELEPORT_ABILITY_H_
#define GAMESERVER_TELEPORT_ABILITY_H_

#pragma once

#include "Ability.h"


class TeleportAbility : public Ability
{
public:
    TeleportAbility() 
    {
    }
    virtual ~TeleportAbility() 
    {
    }

    virtual void InitDetailed(
        Character* attacker,
        SLOTCODE skill_code, 
        SkillInfo* skill_info_data,
        BYTE skill_stat_type, 
        const BASE_ABILITYINFO* base_ability_info
    );
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);

private:
    bool executable_condition_;
    nsGrid::ViewBoxCubic available_range_box_;
};

#endif // GAMESERVER_TELEPORT_ABILITY_H_