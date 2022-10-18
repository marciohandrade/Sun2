#include "StdAfx.h"
#include "PeriodicEffectAbility.h"
#include "BaseEffect.h"
#include "EffectManager.h"
#include "Skill.h"

#include "PeriodicDamageEffect.h"
#include "StatusManager.h"

void PeriodicEffectAbility::InitDetailed(Character* attacker,
                                         SLOTCODE skill_code, SkillInfo* skill_info,
                                         BYTE skill_stat_type,
                                         const BASE_ABILITYINFO* ability_info)
{
    Ability::InitDetailed(attacker, skill_code, skill_info, skill_stat_type, ability_info);

    if (skill_info)
        m_wvMainTargetPos = skill_info->main_target_pos_;
    else
        memset(&m_wvMainTargetPos, 0, sizeof(m_wvMainTargetPos));
}

bool PeriodicEffectAbility::ExecuteEffect(BYTE* pMsg, WORD& wMsgSize)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    // 파라미터 셋팅
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    eATTACK_TYPE eAttackType = (eATTACK_TYPE)ability_info->m_iOption1;
    float fRadius = ability_info->m_iParam[0] / 10.f;
    int iDamage = ability_info->m_iParam[1];
    int iApplicationTime = ability_info->m_iParam[2];
    int iPeriodicTime = ability_info->m_iParam[3];

    GameField *pField = attacker->GetField();
    if (!pField) return false;

    // 영역에 주기 데미지 이펙트 추가
    BaseEffect *pEffect = pField->GetEffectManager()->AllocEffect(EFFECT_TYPE_PERIODIC_DAMAGE);
    if (!pEffect)   return false;

    pEffect->Init(GetSkillCode(), iApplicationTime, iPeriodicTime, attacker, m_wvMainTargetPos, fRadius);
    pEffect->SetDamage(eAttackType, iDamage);
    // _NA001605_EP2_ATTRIBUTES_RENEWAL_
    if (PeriodicDamageEffect* periodic = static_cast<PeriodicDamageEffect*>(pEffect)) {
        periodic->SetAbilityInfo(ability_info);
    }
    pEffect->Start();

    // 패킷구조체를 채운다.
    SKILL_RESULT_EFFECT *pEffectMsg = (SKILL_RESULT_EFFECT*)pMsg;
    if (pEffectMsg)
    {
        pEffectMsg->m_wAbilityIndex = GetIndex();
        pEffectMsg->m_byCount = 1;
        pEffectMsg->m_EffectInfo[0].m_Time = 0;
        pEffectMsg->m_EffectInfo[0].m_wvEffectPos = m_wvMainTargetPos;
        wMsgSize = pEffectMsg->GetSize();
    }

    return true;
}

bool AuroraAbility::IsValidStateID() const
{
    switch(Ability::GetStateID())
    {
        case eCHAR_STATE_SLOW_AURORA:
        case eCHAR_STATE_WEAKNESS_AURORA:
        case eCHAR_STATE_MISCHANCE_AURORA:
        case eCHAR_STATE_DECLINE_AURORA:
        case eCHAR_STATE_RECOVERY_AURORA:
        case eCHAR_STATE_BOOST_AURORA:
        case eCHAR_STATE_IGNORE_AURORA:
        case eCHAR_STATE_CONCENTRATION_AURORA:
        case eCHAR_STATE_IGNORE_RESERVEHP6:
            return true;
    }

    return false;
}

eABILITY_TYPE AuroraAbility::GetAbilityType() const
{
    return eABILITY_TYPE_ACTIVE_AND_EFFECT;
}

bool AuroraAbility::ExecuteEffect( BYTE* pMsg, WORD& wMsgSize )
{
    ;{
        PeriodicEffectAbility periodic_effect_ability;
        periodic_effect_ability.Init( GetSKill(), GetBaseAbilityInfo());
        if (periodic_effect_ability.ExecuteEffect(pMsg, wMsgSize) == false) {
            return false;
        }
    };

    return true;
}

bool AuroraAbility::Execute( Character* target, BYTE* pMsg, WORD& wMsgSize )
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL) {
        return false;
    }

    StatusManager* const status_manager = attacker->GetStatusManager();
    if (status_manager == NULL) {
        return false;
    }

    BaseStatus* const aurora_status = status_manager->FindAuroraStatus();
    if (aurora_status != NULL && aurora_status->IsAbilityStatus()) {
        status_manager->Remove((eCHAR_STATE_TYPE)aurora_status->GetStateID());
    }

    return BaseStatusAbility::Execute(target, pMsg, wMsgSize);
}

