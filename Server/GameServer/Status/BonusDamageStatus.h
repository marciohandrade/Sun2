#pragma once

#include "AbilityStatus.h"

// LowHP�� �� ���ݷ�, ���� ����
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