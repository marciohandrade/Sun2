#include "StdAfx.h"
#include "ThrustAbility.h"
#include "StatusManager.h"
#include "MoveStateControl.h"
#include "AbilityStatus.h"
#include "NumericValues.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

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
            _DamageEffect(attacker, game_field_for_integrity_);	// 譆蟾 撲薑.
        }
        break;
    default:
        control_field_ = eControl_None;
        ASSERT(!"Unknown Thrust Type");
        break;
    };
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


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


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


bool ThrustAbility::_DamageEffect(Character* pCharacter, GameField* const game_field)
{
    if (pCharacter && !(control_field_ & eControl_DamageStarted))
    {
        control_field_ |= eControl_DamageStarted;
        SelfDestructionDamageEffect* const pEffect = static_cast<SelfDestructionDamageEffect*>(\
            game_field->GetEffectManager()->AllocEffect(EFFECT_TYPE_SELF_DESTRUCTION));

        const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

        //  Specification
        //  -   m_iOption1  : 譆模 傚寥腎朝 剪葬
        //  -   m_iOption2  : 譆渠 傚寥腎朝 剪葬
        //  -   m_iParam[0] : Damage Application Type (SelfDestructionDamageEffect::eDamageOpt 霤褻)
        //  -   m_iParam[1] : Damage ([0]==1檣 唳辦) 錳楚 Thrust縑憮朝 罹晦陛 塵葬朝 剪葬雖虜, m_iOption1,2縑憮 譆渠/譆模陛 撲薑脾.
        //  -   m_iParam[2] : Damage Application Range (錳楚 瞳辨 衛除檜 撲薑腎朝 奢除歜)
        SelfDestructionDamage_info_t damage_char_info;
        ZeroMemory(&damage_char_info, sizeof(damage_char_info));
        damage_char_info.m_DamageOpt = ability_info->m_iParam[0];
        damage_char_info.m_Damage    = ability_info->m_iParam[1]; //m_DamageOpt陛 1檣 唳辦 餌辨
        const float apply_range      = ability_info->m_iParam[2]/10.0f;

        //  譆蟾 撲薑 鼻鷓
        //  譆蟾 撲薑橾 唳辦, pCharacter陛 owner陛 脹棻.
        pEffect->Init(GetSkillCode(), BASE_EXPIRE_TIME_INFINITY, 0, pCharacter, *pCharacter->GetPosPtr(), apply_range);
        pEffect->SetInformation(pCharacter, game_field, &damage_char_info);
        //pEffect->Start();
        m_pEffect = pEffect;
        return true;
    }

    return true;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


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
    //  - 謠擎 寞徹擎 瞰渠 嬴棲釭 摹滔縑 僖橫陛晦煎 и棻.
    //  - 欽, 掘褻羹 濠羹蒂 寥機ж朝 寞衝戲煎 瞪?納挬?.
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
        //  - iOption1, iOption2, Param[0~5], m_wStateID 醞縑憮
        //  (length) Param[1], (apply time)Param[2]虜 餌辨ж堅 釭該雖朝 蟾晦?? ж朝 寞衝戲煎 晦襄 掘褻縑 蜃轔棻.
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

    //  奢問濠諦 顫啃曖 嬪纂高
    WzVector vAttackerPos= *attacker->GetPosPtr();
    const WzVector& vTargetPos  = *target->GetPosPtr();
    
    //!~ 檜斜棲暮 夥款顫啃た戲煎 滲唳 - arycoat 2013.02.22.
    if (ability_info->m_wAbilityID == eABILITY_PULLING) {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // PULLING 橫網葬じ蒂 褫暮1 高縑 評塭 莊溥螃朝 嬪纂 滲唳陛棟ж紫煙 熱薑
        switch(ability_info->m_iOption1)
        {
        case 0: // 0 : 詭檣 顫啃 嬪纂煎 PULL
            vAttackerPos = m_wvMainTargetPos;
            break;
        case 1: // 1 : 衛瞪濠 嬪纂煎 PULL
            break;
        default: // 晦獄高 : 詭檣 顫啃 嬪纂煎 PULL
            vAttackerPos = m_wvMainTargetPos;
            break;
        }
#else  // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        vAttackerPos = m_wvMainTargetPos;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
    //~!

    if (need_skip_thrust_position)  // thrust 籀葬朝 熱чж腎 謝ル 滲唳擎 ж雖 彊紫煙 籀葬и棻.
        pPositionMsg->m_wvDestPos = vTargetPos;
    else
    {
        // 塵葬晦 籀葬蒂 п遽棻.
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
        //  (WAVERIX)(080922)(BUG-FIX) Thrust::Execute 褒ぬ衛 等檜攪 犒掘 寰ж朝 僥薯 熱薑
        //  滲熱菟擊 錳鼻鷓煎 蟾晦?倆挬?.
        if (!result)
        {
            return false;
        }
    }



    //////////////////////////////////////////////////////////////////////////
    //  鼻鷓 籀葬
    // 塵葬晦 鼻鷓蒂 勘橫遽棻.
    base_ability_info->m_wStateID = eCHAR_STATE_THRUST;

    // 塵葬朝 鼻鷓 嶸雖衛除 啗骯
    fKnockBackLength = VectorLength(&(vTargetPos - pPositionMsg->m_wvDestPos));

    const float fMoveSpeed = GetBaseMoveSpeedAsState(moveState);
    {
        base_ability_info->m_iParam[2] = (int)(fKnockBackLength / fMoveSpeed);
        int tval = base_ability_info->m_iParam[2] - 100;
        base_ability_info->m_iParam[2] = max(100, tval);
    }

    const bool result = BaseStatusAbility::Execute(target, pMsg, wMsgSize);

    {   //  (WAVERIX)(080922)(BUG-FIX) Thrust::Execute 褒ぬ衛 等檜攪 犒掘 寰ж朝 僥薯 熱薑
        //  滲熱菟擊 錳鼻鷓煎 蟾晦?倆挬?.
        //*base_ability_info_ = backupInfo;
    }

    if (!result)
        return false;

    // ぬ韃掘褻羹蒂 瓣遴棻.
    pPositionMsg->m_wvCurPos = vTargetPos;
    wMsgSize = pPositionMsg->GetSize();

    return true;
}




