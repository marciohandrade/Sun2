#include "stdafx.h"
#include "RecoverStatus.h"
#include "StatusManager.h"
#include "CharacterFormula.h"


void RecoverStatus::Init(Character* owner, WORD state_id, int application_time, int period)
{
    regen_hp_ = 0;
    regen_mp_ = 0;
    use_pet_ = false;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    mode_decrease_fp_ = false;

    status_period_ = period;
    status_execute_counter_ = 0;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    EtcStatus::Init(owner, state_id, application_time, period);
}

void RecoverStatus::Start()
{
    EtcStatus::Start();
    Execute();
}

void RecoverStatus::Execute()
{
    EtcStatus::Execute();
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    if (IsPaused() == true) {
        return;
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    const WORD state_id = GetStateID();

    int regen_hp = 0;
    int regen_mp = 0;
    {
        if (state_id == eCHAR_STATE_ETC_ITEM_RECOVER_HP || 
            state_id == eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY || 
            state_id == eCHAR_STATE_ETC_ITEM_RECOVER_MP) 
        {
            regen_hp = regen_hp_;
            regen_mp = regen_mp_;
        }
        else
        {
            regen_hp = owner->GetRegenHP();
            regen_mp = owner->GetRegenMP();
        }
    }

    if (owner->IsAlive())
    {
        if (IsHPRecover())
        {
            if (IsUseItem())
            {
                const int add_value = ns_formula::ability::CalcIncreseHeal(
                    IncreseHealAbilityType::kItem, regen_hp, *owner);
                if (add_value != 0)
                {
                    regen_hp += add_value;
                }
            }

            if (state_id == eCHAR_STATE_ETC_AUTO_RECOVER_HP || 
                state_id == eCHAR_STATE_ETC_AUTO_RECOVER_HPMP)
            {
                if (IsIgnoreReserveHP())
                {
                    owner->OnRecover(regen_hp, 0, 0, eRECOVER_TYPE_IGNORE_RESERVE_HP);
                }
                else
                {
                    owner->OnRecover(regen_hp, 0, 0, eRECOVER_TYPE_AUTO_HP);
                }
            }
            else
            {
                owner->OnRecover(regen_hp, 0);
            }
        }
        if (IsMPRecover())
        {
            owner->OnRecover(0, regen_mp);
        }
        if (IsSDRecover())
        {
            owner->UpdateCalcRecover(FALSE, FALSE, TRUE);
            owner->OnRecover(0, 0, owner->GetRegenSD());
        }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (IsFPRecover() == true && owner->IsEqualObjectKind(PLAYER_OBJECT) == true)
        {
            Player* player = reinterpret_cast<Player*>(owner);
            if (player == NULL) {
                return;
            }
            if (player->GetFP() <= 0) {
                return;
            }

            // execute()함수가 돌때마다 카운트
            status_execute_counter_++;
            int start_count = util::LL_Ceil((float)fp_maintain_time_ / (float)status_period_);
            if (mode_decrease_fp_ == false)
            {
                if (status_execute_counter_ >= start_count) {
                    mode_decrease_fp_ = true;
                }
            }

            if (mode_decrease_fp_ == true)
            {
                PLAYERFP regen_fp= CalcFpRecover(player->GetCharType());
                player->OnRecoverFP(regen_fp);
            }
        }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
}

bool RecoverStatus::IsIgnoreReserveHP() const
{
    const Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return false;
    }

    if (owner->IsEqualObjectKind(PLAYER_OBJECT))
    {
        if (owner->GetStatusManager()->FindStatus(eCHAR_STATE_IGNORE_RESERVEHP) ||
            owner->GetStatusManager()->FindStatus(eCHAR_STATE_IGNORE_RESERVEHP2) ||
            owner->GetStatusManager()->FindStatus(eCHAR_STATE_IGNORE_RESERVEHP3) ||
            owner->GetStatusManager()->FindStatus(eCHAR_STATE_IGNORE_RESERVEHP4) ||
            owner->GetStatusManager()->FindStatus(eCHAR_STATE_IGNORE_RESERVEHP5) || 
            owner->GetStatusManager()->FindStatus(eCHAR_STATE_IGNORE_RESERVEHP6) ||
            IsUsePet())
        {
            return true;
        }
    }

    return false;
}