#ifndef	__SPBONUSSTATUS_H__
#define __SPBONUSSTATUS_H__

#include "AbilityStatus.h"

// SP당 추가 공격력 상태
class SPBonusStatus : public AbilityStatus
{
public:
    SPBonusStatus() 
    {
    }
    ~SPBonusStatus() 
    {
    }

    virtual void Start();
    virtual void Execute() 
    {
    }
    virtual void End();
};

#endif