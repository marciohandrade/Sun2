#include "StdAfx.h"
#include "ThrustAbility.h"
#include "StatusManager.h"
#include "MoveStateControl.h"
#include "AbilityStatus.h"
#include "NumericValues.h"

//����������������������������������������������������������������������������������������������������������������������

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
            _DamageEffect(attacker, game_field_for_integrity_);	// ���� ����.
        }
        break;
    default:
        control_field_ = eControl_None;
        ASSERT(!"Unknown Thrust Type");
        break;
    };
}


//����������������������������������������������������������������������������������


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


//����������������������������������������������������������������������������������


bool ThrustAbility::_DamageEffect(Character* pCharacter, GameField* const game_field)
{
    if (pCharacter && !(control_field_ & eControl_DamageStarted))
    {
        control_field_ |= eControl_DamageStarted;
        SelfDestructionDamageEffect* const pEffect = static_cast<SelfDestructionDamageEffect*>(\
            game_field->GetEffectManager()->AllocEffect(EFFECT_TYPE_SELF_DESTRUCTION));

        const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

        //  Specification
        //  -   m_iOption1  : �ּ� �˹�Ǵ� �Ÿ�
        //  -   m_iOption2  : �ִ� �˹�Ǵ� �Ÿ�
        //  -   m_iParam[0] : Damage Application Type (SelfDestructionDamageEffect::eDamageOpt ����)
        //  -   m_iParam[1] : Damage ([0]==1�� ���) ���� Thrust������ ���Ⱑ �и��� �Ÿ�����, m_iOption1,2���� �ִ�/�ּҰ� ������.
        //  -   m_iParam[2] : Damage Application Range (���� ���� �ð��� �����Ǵ� ������)
        SelfDestructionDamage_info_t damage_char_info;
        ZeroMemory(&damage_char_info, sizeof(damage_char_info));
        damage_char_info.m_DamageOpt = ability_info->m_iParam[0];
        damage_char_info.m_Damage    = ability_info->m_iParam[1]; //m_DamageOpt�� 1�� ��� ���
        const float apply_range      = ability_info->m_iParam[2]/10.0f;

        //  ���� ���� ����
        //  ���� ������ ���, pCharacter�� owner�� �ȴ�.
        pEffect->Init(GetSkillCode(), BASE_EXPIRE_TIME_INFINITY, 0, pCharacter, *pCharacter->GetPosPtr(), apply_range);
        pEffect->SetInformation(pCharacter, game_field, &damage_char_info);
        //pEffect->Start();
        m_pEffect = pEffect;
        return true;
    }

    return true;
}


//����������������������������������������������������������������������������������


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
    //  - ���� ����� ���� �ƴϳ� ���ʿ� ������ �Ѵ�.
    //  - ��, ����ü ��ü�� ����ϴ� ������� ��ȯ�Ѵ�.
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
        //  - iOption1, iOption2, Param[0~5], m_wStateID �߿���
        //  (length) Param[1], (apply time)Param[2]�� ����ϰ� �������� �ʱ�ȭ �ϴ� ������� ���� ������ �����.
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

    //  �����ڿ� Ÿ���� ��ġ��
    WzVector vAttackerPos= *attacker->GetPosPtr();
    const WzVector& vTargetPos  = *target->GetPosPtr();
    
    //!~ �̱״ϼ� �ٴ�Ÿ�������� ���� - arycoat 2013.02.22.
    if (ability_info->m_wAbilityID == eABILITY_PULLING) {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // PULLING �����Ƽ�� �ɼ�1 ���� ���� �������� ��ġ ���氡���ϵ��� ����
        switch(ability_info->m_iOption1)
        {
        case 0: // 0 : ���� Ÿ�� ��ġ�� PULL
            vAttackerPos = m_wvMainTargetPos;
            break;
        case 1: // 1 : ������ ��ġ�� PULL
            break;
        default: // �⺻�� : ���� Ÿ�� ��ġ�� PULL
            vAttackerPos = m_wvMainTargetPos;
            break;
        }
#else  // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        vAttackerPos = m_wvMainTargetPos;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
    //~!

    if (need_skip_thrust_position)  // thrust ó���� �����ϵ� ��ǥ ������ ���� �ʵ��� ó���Ѵ�.
        pPositionMsg->m_wvDestPos = vTargetPos;
    else
    {
        // �и��� ó���� ���ش�.
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
        //  (WAVERIX)(080922)(BUG-FIX) Thrust::Execute ���н� ������ ���� ���ϴ� ���� ����
        //  �������� �����·� �ʱ�ȭ�Ѵ�.
        if (!result)
        {
            return false;
        }
    }



    //////////////////////////////////////////////////////////////////////////
    //  ���� ó��
    // �и��� ���¸� �ɾ��ش�.
    base_ability_info->m_wStateID = eCHAR_STATE_THRUST;

    // �и��� ���� �����ð� ���
    fKnockBackLength = VectorLength(&(vTargetPos - pPositionMsg->m_wvDestPos));

    const float fMoveSpeed = GetBaseMoveSpeedAsState(moveState);
    {
        base_ability_info->m_iParam[2] = (int)(fKnockBackLength / fMoveSpeed);
        int tval = base_ability_info->m_iParam[2] - 100;
        base_ability_info->m_iParam[2] = max(100, tval);
    }

    const bool result = BaseStatusAbility::Execute(target, pMsg, wMsgSize);

    {   //  (WAVERIX)(080922)(BUG-FIX) Thrust::Execute ���н� ������ ���� ���ϴ� ���� ����
        //  �������� �����·� �ʱ�ȭ�Ѵ�.
        //*base_ability_info_ = backupInfo;
    }

    if (!result)
        return false;

    // ��Ŷ����ü�� ä���.
    pPositionMsg->m_wvCurPos = vTargetPos;
    wMsgSize = pPositionMsg->GetSize();

    return true;
}




