#pragma once

#ifndef GAMESERVER_AGGRO_ABILITY_H_
#define GAMESERVER_AGGRO_ABILITY_H_

#include "Ability.h"


class AggroAbility : public Ability
{
public:
    AggroAbility() : 
        touched_counting_aggressive_dispersion_(false), 
        number_of_targets_for_aggressive_dispersion_(0)
    {
    }
    virtual ~AggroAbility() 
    {
    }
    
    virtual void Init(Skill* skill, const BASE_ABILITYINFO* base_ability_info);
    virtual bool IsValidStateID() const
    {
        if (GetStateID() == kCharStateIncreaseAggroPoint)
        {
            return true;
        }
        return false;
    }
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);

private:
    enum 
    {
        AGGRO_TYPE_NORMAL = 0, // ��׷� ����Ʈ�� ��ũ��Ʈ ��ġ ��ŭ �޴´�.
        AGGRO_TYPE_DISPERSION  // ��׷� ����Ʈ�� ��ũ��Ʈ ��ġ / Target ��ŭ �޴´�.
    };
    bool touched_counting_aggressive_dispersion_;
    uint16_t number_of_targets_for_aggressive_dispersion_;
};

#endif // GAMESERVER_AGGRO_ABILITY_H_