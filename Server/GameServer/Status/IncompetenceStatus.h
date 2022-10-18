#pragma once

#include "AbilityStatus.h"

// 무능력 : 공격 불가 상태
class IncompetenceStatus : public AbilityStatus
{
public:
    IncompetenceStatus() 
    {
    }
    ~IncompetenceStatus() 
    {
    }

    virtual void Start();
    virtual void Execute() 
    {
    }
};