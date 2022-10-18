#include "stdafx.h"
#include "Ability.h"
#include "DragonTransStatus.h"
#include "SkillAttrCalculator.h"
#include "PacketStruct_AG.h"
#include "Player.h"

#include "SkillSlotManager.h"

void DragonTransStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    // CHANGES: f110428.6L, prevent a skipping previous style buff release process
    // because at this point, player is already transform.
    starting_state_ = true;
    //
    // 변신 수치를 적용시킨다.
    SkillAttrCalculator skill_calculator(owner->GetAttr());
    int changed_hp = skill_calculator.ApplyDragonFormation(true, GetSkillCode(), owner->GetHP());
    owner->OnRecover(changed_hp, 0);

    ApplyAttrEx(true);
    starting_state_ = false;
}

VOID DragonTransStatus::End()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	// 클라에 알린다.
	MSG_CG_STATUS_END_DRAGON_FORMATION_BRD brdMsg;
	brdMsg.m_dwObjKey = owner->GetObjectKey();
	brdMsg.m_SkillCode = GetSkillCode();
	brdMsg.m_wStatusCode = GetStateID();
	owner->SendPacketAround( &brdMsg, sizeof(brdMsg) ); 

	// 변신 전에 적용시켰던 수치를 뺀다.
	SkillAttrCalculator SkillCalc( owner->GetAttr() );
	int iChangeHP = SkillCalc.ApplyDragonFormation( FALSE, GetSkillCode(), owner->GetHP() );
	if( iChangeHP )	owner->OnRecover( iChangeHP, 0 );
	ApplyAttrEx( FALSE );
}

void DragonTransStatus::ApplyAttrEx(bool application)
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    // old comments
    // (WAVERIX) (NOTE) 공통 알고리즘 구현상 하기 내용 나쁘지는 않다... 하지만, Send계열 종류가
    // 복수 개이며, 그것마다 상태가 미묘하게 틀리다는 점에서 일관성 보장하기 힘들다. 따라서
    // 명시적으로 Player인 경우에 대해 처리하는 것으로 변경한다.

	// 지력, 체력 변화에 따른 HP, MP 회복율 변화
	owner->UpdateCalcRecover( TRUE, TRUE, FALSE );

	Player* pPlayer = owner->GetPlayerOwner();
	if(pPlayer == 0)
        return;

    pPlayer->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
    pPlayer->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);

    // 스타일을 교체한다.
    if (application)
    {
		if( GetCharType() == eTRANSFORM_TYPE_DRAGON1 )
			pPlayer->GetSkillSlotManager()->SetSelectStyle( eSTYLE_DRAGON_TRANSFORM1 );
		else if( GetCharType() == eTRANSFORM_TYPE_DRAGON2 )
			pPlayer->GetSkillSlotManager()->SetSelectStyle( eSTYLE_DRAGON_TRANSFORM2 );
	}
	else
	{
		pPlayer->GetSkillSlotManager()->SetSelectStyle( pPlayer->GetBaseSelectedStyle() );
	}
}















