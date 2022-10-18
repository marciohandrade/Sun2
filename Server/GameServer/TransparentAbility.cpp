#include "StdAfx.h"
#include ".\transparentability.h"
#include "StatusManager.h"
#include "BaseStatus.h"

TransparentAbility::TransparentAbility(void)
{
}

TransparentAbility::~TransparentAbility(void)
{
}


bool TransparentAbility::CanExecute(Character* attacker, Character* target,
                                    DWORD main_target_key, WzVector main_target_pos)
{
    __UNUSED((main_target_key, main_target_pos));
    // (NOTE) no ratio, no check... invincible skill. hummm...
    // (f100309.2L) changes custom logic 'transparent skill' prevent replacing, architectural issue.
    if (attacker != target) {
        return false; // add rule that no skill is in current.
    }
    // add pre-check
    if (this->IsValidStateID() == false) {
        return false;
    }
    // filters
    StatusManager* const status_manager = attacker->GetStatusManager();
    StatusField& status_field = status_manager->Status;
    BaseStatus* status = NULL;
    if (status_field.IsPlayer_TransOn())
    {
        // current state has stop requested the 'Hide' skill status.
        // need cancel stop process, how to do it?
        status = status_manager->FindStatus(eCHAR_STATE_TRANSPARENT);
        if (FlowControl::FCAssert(status != NULL)) 
        {
            status->CancelRequestStop();
#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
            // 투명상태에서 투명상태를 다시 적용시키려 할 때, 투명상태를 새로 걸어주도록 한다.
            return true;
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL
        }
        else {
            // expect unreachable point, but prevent terrible status,
            // because this skill is the invincible level.
            status_field.On_CharTransOn(false);
        }
        return false; //
    }
#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
#else
    status = status_manager->FindStatus(eCHAR_STATE_BATTLE);
    if (status != NULL) {
        return false;
    }
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL
    return true;
}

bool TransparentAbility::IsValidStateID() const
{
    const eCHAR_STATE_TYPE state_id_by_script = GetStateID();
    if (state_id_by_script != eCHAR_STATE_TRANSPARENT) {
        return false;
    }
    return true;
}

