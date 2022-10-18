#include "stdafx.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Ability.h"
#include "ToggleFPStatus.h"
#include "SkillInfoParser.h"
#include "SkillManager.h"
#include "StatusManager.h"

void ToggleFPStatus::Init( Character* owner, Character* attacker, Ability* ability )
{
    AbilityStatus::Init(owner, attacker, ability);

    const BASE_ABILITYINFO* const base_ability_info = ability->GetBaseAbilityInfo();
    if (base_ability_info != NULL) 
    {
        recover_fp_ = (PLAYERFP)base_ability_info->m_iParam[1];
        
        SLOTCODE skill_code = base_ability_info->m_iOption2;
        connected_skill_info_ = SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    }

    //Owner_Player
    if (owner == NULL) {
        return;
    }
    if (owner->IsAlive() && owner->IsEqualObjectKind(PLAYER_OBJECT))
    {
        Player* player = reinterpret_cast<Player*>(owner);
        if (player != NULL) {
            owner_player_ = player;
        }
    }

    if (GetStateID() == eCHAR_STATE_RISING_FORCE)
    {
        StatusManager* status_manager = owner->GetStatusManager();
        if (status_manager != NULL)
        {
            BaseStatus* fp_status = status_manager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_FP);
            if (fp_status == NULL) {
                return;
            }

            fp_status->Pause();
        }
    }
    
}

void ToggleFPStatus::Execute()
{
    //FP감소
    _ReduceFP();

    if (_UseSkill() == FALSE)
    {
        StopStatus();
        return;
    }

    AbilityStatus::Execute();
}

void ToggleFPStatus::_ReduceFP()
{
    if (owner_player_->IsAlive()) {
        owner_player_->OnRecoverFP(recover_fp_);
    }
}

BOOL ToggleFPStatus::_UseSkill()
{
    if (connected_skill_info_ == NULL) {
        return false;
    }
    if (owner_player_->ChkNeedFP(recover_fp_) == FALSE) {
        return FALSE;
    }

    // 자기자신의 좌표 저장
    const WzVector* target_pos = NULL;
    target_pos = owner_player_->GetPosPtr();

    SkillInfo skill_info_data(GetOwner(), connected_skill_info_->m_SkillCode, 0);
    skill_info_data.SetArgsObjectInfo(owner_player_->GetObjectKey(), target_pos, target_pos, target_pos);
    skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
                                    skill_info_data.kDefaultSkillEffect,
                                    skill_info_data.kDefaultSkillDelay,
                                    SKILL_FACTOR_NOMAL,
                                    skill_info_data.kDefaultPropensity,
                                    skill_info_data.kDefaultEvent);

    SkillManager* skill_manager = owner_player_->GetSkillManager();
    return skill_manager->RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);
}

void ToggleFPStatus::End()
{
    AbilityStatus::End();

    if (GetStateID() == eCHAR_STATE_RISING_FORCE)
    {
        StatusManager* status_manager = GetOwner()->GetStatusManager();
        if (status_manager != NULL)
        {
            BaseStatus* fp_status = status_manager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_FP);
            fp_status->Resume();
        }
    }
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
