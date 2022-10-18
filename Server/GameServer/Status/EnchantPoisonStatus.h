#pragma once

#include "AbilityStatus.h"

class EnchantPoisonStatus : public AbilityStatus
{
public:
    EnchantPoisonStatus();
    ~EnchantPoisonStatus();
public:
    void ExecuteSkill(Character* target);
};