#pragma once

#include "AbilityStatus.h"

// LowHP�� �� ���ݷ�, ���� ����
class LowHPStatus : public AbilityStatus
{
public:
    LowHPStatus() 
    {
    }
    ~LowHPStatus() 
    {
    }

    virtual void Init(Character* owner, Character* attacker, Ability* ability);

    virtual void Start();
    virtual void Execute() 
    {
    }
    virtual void End();
    void UpdateLowHP();

private:
    float hp_ratio_;
};