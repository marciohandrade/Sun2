#include "stdafx.h"
#include "BuffStatusAbility.h"

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "BaseStatus.h"
#include "StatusManager.h"

bool ToggleFPAbility::Execute( Character* target, BYTE* pMsg, WORD& wMsgSize )
{
    Character* const attacker = GetAttacker();

    StatusManager* const status_manager = attacker->GetStatusManager();
    if (status_manager == NULL) {
        return false;
    }

    BaseStatus* const status = status_manager->FindToggleFPStatus();
    if (status != NULL && status->IsAbilityStatus() && status->GetStateID() == this->GetStateID()) {
        status_manager->Remove((eCHAR_STATE_TYPE)status->GetStateID());
    }

    if (BuffStatusAbility::Execute(target,pMsg,wMsgSize) == false) {
        return false;
    }

    return true;
}

eABILITY_TYPE ToggleFPAbility::GetAbilityType() const
{
    return eABILITY_TYPE_ACTIVE;
}

bool ToggleFPAbility::IsValidStateID() const
{
    //switch(Ability::GetStateID())
    //{
    //    //
    //}
    //return false;

    return true;
}

#endif //_NA_007667_20141001_WITCHBLADE_SKILL