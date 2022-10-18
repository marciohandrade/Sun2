#pragma once

#include "AbilityStatus.h"

// LowHP일 때 공격력, 방어력 증감
class BonusDamageStatus : public AbilityStatus
{
public:
    BonusDamageStatus() 
    {
    }
    ~BonusDamageStatus() 
    {
    }

    virtual void Init(Character* owner, Character* attacker, Ability* ability);
    virtual void Start() 
    {
    }
    virtual void Execute() 
    {
    }
    void AddDamage(eATTACK_TYPE attack_type, DAMAGETYPE& damage);

private:
    eATTACK_TYPE attack_type_;
};