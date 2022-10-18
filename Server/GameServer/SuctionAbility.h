#pragma once

#ifndef GAMESERVER_SUCTION_ABILITY_H_
#define GAMESERVER_SUCTION_ABILITY_H_

#include "BaseStatusAbility.h"
#include "SuctionStatus.h"
#include "StatusManager.h"


class SuctionAbility : public BaseStatusAbility
{
public:
    SuctionAbility() 
    {
    }
    virtual ~SuctionAbility() 
    {
    }

    virtual bool IsValidStateID() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_HP_SUCTION:
        case eCHAR_STATE_MP_SUCTION: //CHANGUP_IMMUNITY_DAMAGE_STATUS
            return true;
        default:
            return false;
        }
    }
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size)
    {
        if (Ability::Execute(target, msg, msg_size) == false)
        {
            return false;
        }
        
        if (IsValidStateID() == false)
        {
            return false;
        }

        Character* const attacker = GetAttacker();
        if (attacker == NULL)
        {
            return false;
        }
            
        SuctionStatus* const ability_status = 
            (SuctionStatus*)attacker->GetStatusManager()->AllocAbilityStatus(attacker, this);
        if (ability_status == NULL)
        {
            return false;
        }
        ability_status->SetTargetObjectKey(target->GetObjectKey());

        SKILL_RESULT_CODE* const result_msg = (SKILL_RESULT_CODE*)msg;
        if (result_msg != NULL)
        {
            result_msg->m_wStatusCode = ability_status->GetStateID();
            result_msg->m_iStatusTime = ability_status->GetApplicationTime();
        }

        return true;
    }
};

#endif // GAMESERVER_SUCTION_ABILITY_H_