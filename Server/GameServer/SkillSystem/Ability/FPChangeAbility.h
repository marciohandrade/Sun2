#pragma once
#include "Ability.h"

class FPChangeAbility : public Ability
{
public:
    FPChangeAbility() {}
    ~FPChangeAbility() {}
    virtual bool Execute(Character* target, BYTE* pMsg, WORD& wMsgSize);
private:

};