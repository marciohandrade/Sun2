#include "StdAfx.h"
#include ".\suctionstatus.h"
#include "Ability.h"
#include <PublicMath.h>
#include "StateInfoParser.h"
#ifdef _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION
#include "StatusManager.h"
#endif // _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION

SuctionStatus::SuctionStatus(void)
{
}

SuctionStatus::~SuctionStatus(void)
{
}

VOID		SuctionStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
	AbilityStatus::Init( pOwner, pAttacker, pAbility );

	const BASE_ABILITYINFO *pBaseAbilityInfo = pAbility->GetBaseAbilityInfo();
	m_wPeriodicDamage = pBaseAbilityInfo->m_iParam[0];
	m_bySuctionType = pBaseAbilityInfo->m_iOption1;
}

VOID		SuctionStatus::Execute()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        StopStatus();
        return;
    }

	Character* pTarget = owner->GetField()->FindCharacter( m_dwTargetObjectKey );
	if( !pTarget )
	{
		StopStatus();
		return;
	}
#ifdef _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION
    StatusManager* const status_manager = owner->GetStatusManager();
    if (status_manager && status_manager->CanAttack() == false)
    {
        StopStatus();
        return;
    }
#endif // _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION

	// 거리체크 ( 30m 이상이면 정지한다. )
	WzVector vecOwner, vecTarget;
	owner->GetPos( &vecOwner );
	pTarget->GetPos( &vecTarget );
	float fCurPosDiff = Math_Vector2DLength( vecOwner, vecTarget );
	if( fCurPosDiff > 30 )
	{
		StopStatus();
		return;
	}

    // f100908.1L
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(owner, eATTACK_TYPE_MELEE, GetSkillCode());
#else
    DamageArgs damage_args(owner, eATTACK_TYPE_MELEE);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY

    DAMAGETYPE result_periodic_damage = m_wPeriodicDamage;
#ifdef _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE
    const Attributes& skill_user_attr = owner->GetAttr();
    const int add_skill_ratio = skill_user_attr.GetSkillPercentDamage();

    if (add_skill_ratio != 0)
    {
        const int calc_skill_ratio = 
            static_cast<DAMAGETYPE>(static_cast<float>(m_wPeriodicDamage) * (static_cast<float>(add_skill_ratio) / 100.0f));
        if (calc_skill_ratio != 0)
        {
            result_periodic_damage += calc_skill_ratio;
        }
    }
#endif //_NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE

	//HP흡수
	if( m_bySuctionType == 1 )
	{
        ;{
            //damage_args.option_.is_critical_;
            //damage_args.option_.is_mirror_;
            //damage_args.limit_hp_;
            damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
            BASE_STATEINFO* pStateInfo = StateInfoParser::Instance()->GetStateInfo(GetStateID());
            if (pStateInfo) {
                damage_args.sd_apply_ = (eSD_APPLY)pStateInfo->m_bySDApply;
            }

            damage_args.damage_ = result_periodic_damage;
            //damage_args.damage_sd_;
        };
        // (WAVERIX) (090907) (WOPS:4774) 섀도우 HP석션 사용시 적 HP가 감소하지 않는 현상(m_pOwner->m_pTarget)
        pTarget->Damaged(&damage_args);
        owner->IncreaseHP(damage_args.damage_);
	}
	//MP/SP 흡수
	else if(  m_bySuctionType == 2 || m_bySuctionType == 3 )
	{
        damage_args.damage_ = pTarget->DecreaseMP( result_periodic_damage );
		owner->IncreaseMP(damage_args.damage_);
	}

    ;{
        MSGSUB_SKILL_SUCTION_BRD msg_brd;
        msg_brd.Clear();
        if (GetAttacker() != NULL) 
        {
            msg_brd.m_dwAttackerKey = owner->GetObjectKey();
        }
        msg_brd.m_SkillCode = GetSkillCode();
        msg_brd.m_wTargetDamage = damage_args.damage_;
        msg_brd.m_dwTargetHP = pTarget->GetHP();
        msg_brd.m_dwTargetMP = pTarget->GetMP();
        msg_brd.m_dwAttackerHP = owner->GetHP();
        msg_brd.m_dwAttackerMP = owner->GetMP();

        owner->SendPacketAroundForSync(&msg_brd);
    };

    BaseStatus::Execute();
}

