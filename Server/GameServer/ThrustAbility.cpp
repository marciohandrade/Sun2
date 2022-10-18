#include "StdAfx.h"
#include "ThrustAbility.h"
#include "StatusManager.h"
#include "MoveStateControl.h"
#include "AbilityStatus.h"
#include "NumericValues.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "BombEffect.h"
#include "EffectManager.h"

void ThrustAbility::InitDetailed(Character* attacker,
                                 SLOTCODE skill_code, SkillInfo* skill_info_data,
                                 BYTE skill_stat_type, const BASE_ABILITYINFO* base_ability_info)
{
    BaseStatusAbility::InitDetailed(attacker, skill_code,
                                    skill_info_data, skill_stat_type, base_ability_info);

    if (skill_info_data)
        m_wvMainTargetPos = skill_info_data->main_target_pos_;
    else
        memset(&m_wvMainTargetPos, 0, sizeof(m_wvMainTargetPos));

    game_field_for_integrity_ = attacker ? attacker->GetField() : 0;
    const eABILITY ability_id = static_cast<eABILITY>(base_ability_info->m_wAbilityID);
    switch (ability_id)
    {
    case eABILITY_KNOCKBACK:
    case eABILITY_KNOCKBACK2:
    case eABILITY_PULLING:
        control_field_ = eControl_Thrust;
        break;
    case eABILITY_SELF_DESTRUCTION:
        {
            control_field_ = eControl_Thrust|eControl_SelfDestruction;
            _DamageEffect(attacker, game_field_for_integrity_);	// 최초 설정.
        }
        break;
    default:
        control_field_ = eControl_None;
        ASSERT(!"Unknown Thrust Type");
        break;
    };
}


//─────────────────────────────────────────


bool ThrustAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    GameField* const target_field = target->GetField();
    const bool check_integrity = target_field && (target_field == game_field_for_integrity_);

    if (!check_integrity)
        return false;

    const bool result_thrust = (control_field_ & eControl_Thrust)
                             ?   _Thrust(target, pMsg, wMsgSize)
                             :   false;

    //if (control_field_ & eControl_SelfDestruction)
    //  _DamageEffect();

    return result_thrust;
}


//─────────────────────────────────────────


bool ThrustAbility::_DamageEffect(Character* pCharacter, GameField* const game_field)
{
    if (pCharacter && !(control_field_ & eControl_DamageStarted))
    {
        control_field_ |= eControl_DamageStarted;
        SelfDestructionDamageEffect* const pEffect = static_cast<SelfDestructionDamageEffect*>(\
            game_field->GetEffectManager()->AllocEffect(EFFECT_TYPE_SELF_DESTRUCTION));

        const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

        //  Specification
        //  -   m_iOption1  : 최소 넉백되는 거리
        //  -   m_iOption2  : 최대 넉백되는 거리
        //  -   m_iParam[0] : Damage Application Type (SelfDestructionDamageEffect::eDamageOpt 참조)
        //  -   m_iParam[1] : Damage ([0]==1인 경우) 원래 Thrust에서는 여기가 밀리는 거리지만, m_iOption1,2에서 최대/최소가 설정됨.
        //  -   m_iParam[2] : Damage Application Range (원래 적용 시간이 설정되는 공간임)
        SelfDestructionDamage_info_t damage_char_info;
        ZeroMemory(&damage_char_info, sizeof(damage_char_info));
        damage_char_info.m_DamageOpt = ability_info->m_iParam[0];
        damage_char_info.m_Damage    = ability_info->m_iParam[1]; //m_DamageOpt가 1인 경우 사용
        const float apply_range      = ability_info->m_iParam[2]/10.0f;

        //  최초 설정 상태
        //  최초 설정일 경우, pCharacter가 owner가 된다.
        pEffect->Init(GetSkillCode(), BASE_EXPIRE_TIME_INFINITY, 0, pCharacter, *pCharacter->GetPosPtr(), apply_range);
        pEffect->SetInformation(pCharacter, game_field, &damage_char_info);
        //pEffect->Start();
        m_pEffect = pEffect;
        return true;
    }

    return true;
}


//─────────────────────────────────────────


bool ThrustAbility::_Thrust(Character* const target, BYTE* pMsg, WORD& wMsgSize)
{
    const Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    SKILL_RESULT_POSITION* const pPositionMsg = (SKILL_RESULT_POSITION*)pMsg;

    StatusManager* const pStatusManager = target->GetStatusManager();
    const bool bDownAfterThrust = !!pStatusManager->FindStatus(eCHAR_STATE_DOWN);

    eCHAR_MOVE_STATE moveState = bDownAfterThrust ? CMS_KNOCKBACK_DOWN : CMS_KNOCKBACK;
    DWORD animationDelay = 1000;

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    switch (ability_info->m_wAbilityID)
    {
    case eABILITY_KNOCKBACK2:
        animationDelay = 4000;
        moveState = CMS_KNOCKBACK2;
        break;
    };

    const bool need_skip_thrust_position = \
        !pStatusManager->AnimationDelay.SetAnimationDelay(this, animationDelay);

    //  (WAVERIX)(080922)(NOTE)
    //  - 좋은 방법은 절대 아니나 선례에 묻어가기로 한다.
    //  - 단, 구조체 자체를 백업하는 방식으로 전환한다.
    struct RollbackData {
        RollbackData(const BASE_ABILITYINFO* ability_info)
            : ability_info_(const_cast<BASE_ABILITYINFO*>(ability_info))
        {
            backuped_data_ = *ability_info;
        }
        ~RollbackData() {
            *ability_info_ = backuped_data_;
        }
        BASE_ABILITYINFO* const ability_info_;
        BASE_ABILITYINFO backuped_data_;
    } rollback_data(ability_info);

    BASE_ABILITYINFO* const base_ability_info = rollback_data.ability_info_;
    BASE_ABILITYINFO& backup_info = rollback_data.backuped_data_;

    float fKnockBackLength = .0f;
    if (control_field_ & eControl_SelfDestruction)
    {
        //  - iOption1, iOption2, Param[0~5], m_wStateID 중에서
        //  (length) Param[1], (apply time)Param[2]만 사용하고 나머지는 초기화 하는 방식으로 기존 구조에 맞춘다.
        const DWORD rndlength =
            RandomNumberGenerator::GetRandomNumberRange(backup_info.m_iOption1, backup_info.m_iOption2);
        fKnockBackLength = need_skip_thrust_position ? 0.0f : rndlength / 10.f;
        base_ability_info->m_iOption1 = 0;
        base_ability_info->m_iOption2 = 0;
        base_ability_info->m_wStateID = eCHAR_STATE_INVALID;
        ZeroMemory(base_ability_info->m_iParam, sizeof(base_ability_info->m_iParam));
        base_ability_info->m_iParam[1] = need_skip_thrust_position ? 0 : rndlength;
        //printf("thrust\n");
        //  Damage Processing ActionCmd
        if (m_pEffect->IsPassIntegrity())
            m_pEffect->CmdExecute();
    }
    else
    {
        fKnockBackLength = need_skip_thrust_position
                         ?  0.0f
                         :  (float)backup_info.m_iParam[1] / 10.f;
    }

    //  공격자와 타겟의 위치값
    WzVector vAttackerPos= *attacker->GetPosPtr();
    const WzVector& vTargetPos  = *target->GetPosPtr();
    
    //!~ 이그니션 바닥타겟팅으로 변경 - arycoat 2013.02.22.
    if (ability_info->m_wAbilityID == eABILITY_PULLING) {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // PULLING 어빌리티를 옵션1 값에 따라 끌려오는 위치 변경가능하도록 수정
        switch(ability_info->m_iOption1)
        {
        case 0: // 0 : 메인 타겟 위치로 PULL
            vAttackerPos = m_wvMainTargetPos;
            break;
        case 1: // 1 : 시전자 위치로 PULL
            break;
        default: // 기본값 : 메인 타겟 위치로 PULL
            vAttackerPos = m_wvMainTargetPos;
            break;
        }
#else  // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        vAttackerPos = m_wvMainTargetPos;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
    //~!

    if (need_skip_thrust_position)  // thrust 처리는 수행하되 좌표 변경은 하지 않도록 처리한다.
        pPositionMsg->m_wvDestPos = vTargetPos;
    else
    {
        // 밀리기 처리를 해준다.
        WzVector& vNormalVectorP2M = (vTargetPos - vAttackerPos);
        ;{
            //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
            if (fKnockBackLength < 0) /*PULLING*/
            {
                float distance = VectorLength( &vNormalVectorP2M );
                if (distance < (-fKnockBackLength))
                    fKnockBackLength = -distance;
            }
        };
        VectorNormalize(&vNormalVectorP2M, &vNormalVectorP2M);
        vNormalVectorP2M *= fKnockBackLength;
        const BOOL result = target->ExecuteThrust(true, &vNormalVectorP2M, pPositionMsg->m_wvDestPos, fKnockBackLength, bDownAfterThrust);
        //  (WAVERIX)(080922)(BUG-FIX) Thrust::Execute 실패시 데이터 복구 안하는 문제 수정
        //  변수들을 원상태로 초기화한다.
        if (!result)
        {
            return false;
        }
    }



    //////////////////////////////////////////////////////////////////////////
    //  상태 처리
    // 밀리기 상태를 걸어준다.
    base_ability_info->m_wStateID = eCHAR_STATE_THRUST;

    // 밀리는 상태 유지시간 계산
    fKnockBackLength = VectorLength(&(vTargetPos - pPositionMsg->m_wvDestPos));

    const float fMoveSpeed = GetBaseMoveSpeedAsState(moveState);
    {
        base_ability_info->m_iParam[2] = (int)(fKnockBackLength / fMoveSpeed);
        int tval = base_ability_info->m_iParam[2] - 100;
        base_ability_info->m_iParam[2] = max(100, tval);
    }

    const bool result = BaseStatusAbility::Execute(target, pMsg, wMsgSize);

    {   //  (WAVERIX)(080922)(BUG-FIX) Thrust::Execute 실패시 데이터 복구 안하는 문제 수정
        //  변수들을 원상태로 초기화한다.
        //*base_ability_info_ = backupInfo;
    }

    if (!result)
        return false;

    // 패킷구조체를 채운다.
    pPositionMsg->m_wvCurPos = vTargetPos;
    wMsgSize = pPositionMsg->GetSize();

    return true;
}




