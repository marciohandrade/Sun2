#ifndef	__WOUNDSTATUS_H__
#define __WOUNDSTATUS_H__

#include "AbilityStatus.h"

// 상처 상태
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

    eATTACK_TYPE attack_type_; // 공격타입
    DAMAGETYPE period_damage_; // 주기 데미지 수치
};

#endif