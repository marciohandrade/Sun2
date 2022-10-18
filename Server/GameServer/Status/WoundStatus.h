#ifndef	__WOUNDSTATUS_H__
#define __WOUNDSTATUS_H__

#include "AbilityStatus.h"

// ��ó ����
class WoundStatus : public AbilityStatus
{
public:
    WoundStatus() 
    {
    }
    ~WoundStatus() 
    {
    }

    virtual void Init(Character* owner, Character* attacker, Ability* ability);
    virtual void Start() 
    {
    }
    virtual void Execute();

private:
    int CalcAddDamage(const Ability& ability, const Character& attacker) const;

    eATTACK_TYPE attack_type_; // ����Ÿ��
    DAMAGETYPE period_damage_; // �ֱ� ������ ��ġ
};

#endif