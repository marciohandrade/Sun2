#pragma once

#include "AbilityStatus.h"

// ���ɷ� : ���� �Ұ� ����
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