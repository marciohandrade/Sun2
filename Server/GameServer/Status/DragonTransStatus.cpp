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
    // ���� ��ġ�� �����Ų��.
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

	// Ŭ�� �˸���.
	MSG_CG_STATUS_END_DRAGON_FORMATION_BRD brdMsg;
	brdMsg.m_dwObjKey = owner->GetObjectKey();
	brdMsg.m_SkillCode = GetSkillCode();
	brdMsg.m_wStatusCode = GetStateID();
	owner->SendPacketAround( &brdMsg, sizeof(brdMsg) ); 

	// ���� ���� ������״� ��ġ�� ����.
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
    // (WAVERIX) (NOTE) ���� �˰��� ������ �ϱ� ���� �������� �ʴ�... ������, Send�迭 ������
    // ���� ���̸�, �װ͸��� ���°� �̹��ϰ� Ʋ���ٴ� ������ �ϰ��� �����ϱ� �����. ����
    // ��������� Player�� ��쿡 ���� ó���ϴ� ������ �����Ѵ�.

	// ����, ü�� ��ȭ�� ���� HP, MP ȸ���� ��ȭ
	owner->UpdateCalcRecover( TRUE, TRUE, FALSE );

	Player* pPlayer = owner->GetPlayerOwner();
	if(pPlayer == 0)
        return;

    pPlayer->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
    pPlayer->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);

    // ��Ÿ���� ��ü�Ѵ�.
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















