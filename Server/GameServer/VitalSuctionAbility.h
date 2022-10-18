#pragma once

#ifndef GAMESERVER_VITAL_SUCTION_ABILITY_H_
#define GAMESERVER_VITAL_SUCTION_ABILITY_H_

#include "BaseStatusAbility.h"


class VitalSuctionAbility : public BaseStatusAbility
{
public:
    VitalSuctionAbility();
    virtual ~VitalSuctionAbility();

    virtual void Init(Skill* skill, const BASE_ABILITYINFO* ability_info);
    virtual bool IsValidStateID() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_VITAL_SUCTION:
            return true;
        default:
            return false;
        }
    }
    virtual bool CanExecute(
        Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos);

    DWORD GetSuctionHP() {
        return suction_hp_;
    }

private:
    int suction_count_;
    DWORD total_suction_hp_;
    DWORD suction_hp_;
};

#endif // GAMESERVER_VITAL_SUCTION_ABILITY_H_