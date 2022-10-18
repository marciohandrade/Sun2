#include "StdAfx.h"
#include ".\blockskillability.h"

BlockSkillAbility::BlockSkillAbility(void)
{
}

BlockSkillAbility::~BlockSkillAbility(void)
{
}


bool BlockSkillAbility::IsValidStateID() const
{
    switch (GetStateID())
    {
    case eCHAR_STATE_SEALING:
    case eCHAR_STATE_CONFUSE:
    case eCHAR_STATE_SLIP:
        return true;
    default:
        return false;
    }
}

