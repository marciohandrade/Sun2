#include "StdAfx.h"
#include ".\vitalsuctionstatus.h"
#include "Ability.h"
#include "VitalSuctionAbility.h"
#include "NPCStateManager.h"
#include "DeadState.h"
#include "Skill.h"
#include "SkillInfoParser.h"

VitalSuctionStatus::VitalSuctionStatus(void)
{
	m_pGameField = NULL;
	m_fSpeed = 0.0f;
	m_fAccelate = 0.0f;
}

VitalSuctionStatus::~VitalSuctionStatus(void)
{

}

VOID		VitalSuctionStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
    // TODO: f101031.1L, targets of a 'eCHAR_STATE_VITAL_SUCTION' can include player by a script
	AbilityStatus::Init( pOwner, pAttacker, pAbility );
    VitalSuctionAbility* vital_suction_ablity = (VitalSuctionAbility*)pAbility;
    
    suction_hp_ = vital_suction_ablity->GetSuctionHP();

	m_pGameField = pAttacker->GetField();
	m_dwAttackerObjectKey = pAttacker->GetObjectKey();
	pOwner->GetPos( &m_vGhost );
	m_fAccelate = 0.001f;
	m_fSpeed = 0.001f;
	m_dwFirstTick = GetTickCount();
	m_dwPreTick = m_dwFirstTick;
	SetVector( &m_vGhostUnitDir, 0.f, 0.f, 1.f );

	// Ÿ���� �׾ ������� �ð��� ���� �����Ѵ�.
	// �����ڴ� ���Ϳ��� �Ѵ�.
	NPC* pMonster = (NPC*)pOwner;
	State* pState = pMonster->GetStateManager()->GetCurState();
	if( !pState )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] ���Ͱ� �׾��µ� ���� ���°� ����." );
		return;
	}

	if( pState->GetStateID() != STATE_ID_DEAD )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] ���Ͱ� �׾��µ� ���� ���°� �ƴϴ�. CurState = %d", pState->GetStateID() );
        // (CHANGES) (100223) (WAVERIX) fix up missing return that post action assume dead state
        return;
	}

	// ������ DEAD���¸� ���� ��Ų��.
    static_cast<DeadState*>(pState)->SetNextTimeoutInterval(20 * 1000);
}

VOID		VitalSuctionStatus::Execute()
{
	Character* pAttacker = m_pGameField->FindCharacter( m_dwAttackerObjectKey );
    // (WAVERIX) (090608) (WOPS:4077) VitalSuction ���� ���� ���� �߰�
	if(pAttacker == 0 || pAttacker->IsDead())
	{
		// �׸��� ���� ���� �����ڴ� �Ҹ��Ų��.
		StopStatus();
		return;
	}
    
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        StopStatus();
        return;
    }

	NPC* pMonster = (NPC*)owner;
	State* pState = pMonster->GetStateManager()->GetCurState();
	if( !pState )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] ���Ͱ� �׾��µ� ���� ���°� ����." );
		StopStatus();
		return;
	}

	if( pState->GetStateID() != STATE_ID_DEAD )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] VitalSuctionStatus�ε� ���� ���°� �ƴϴ�. CurState = %d", pState->GetStateID() );
		StopStatus();
		return;
	}

	DWORD	dwCurTick = GetTickCount();
	DWORD dwTick = dwCurTick - m_dwPreTick;
	WzVector vTemp, vAxis, vLen1, vLen2, vAttackerPos;

	//1�� ������ ���� ����Ѵ�.
	if( ( m_dwFirstTick + 1000 ) >= dwCurTick )
	{
		WzVector vUp;
		SetVector( &vUp, 0.f, 0.f, 1.f );
		vTemp = vUp*(dwTick*m_fSpeed);			//��������..
		m_vGhost = m_vGhost + vTemp;
	}
	else
	{
		pAttacker->GetPos( &vAttackerPos );
		WzVector vLen = vAttackerPos - m_vGhost;
		float fLen = VectorLength( &vLen );
		//���� ���ϴ� �Լ�.
		VectorCrossProduct( &vAxis, &m_vGhostUnitDir, &vLen );

		//ȸ����
		float theta = 0.001f * dwTick;

		//ȸ����ȯ�� ��� ����.
		WzMatrix matRotAxis;
		MatrixRotation( &matRotAxis, &vAxis, theta );

		if( 0.5f < fLen )
		{
			vTemp = m_vGhostUnitDir;
			VectorTransformCoord( &m_vGhostUnitDir, &vTemp, &matRotAxis );
		}
		else
		{
			VectorNormalize( &m_vGhostUnitDir, &vLen );
		}

		// �ѹ� Execute�Ҷ� ����( 1�� ) �̵��ϴ� �Ÿ�..
		m_fSpeed += m_fAccelate;
		vTemp = m_vGhostUnitDir*( dwTick*m_fSpeed );
		WzVector vLenCheck = m_vGhost + vTemp;
		vLen1 = vLenCheck - m_vGhost;
		vLen2 = vAttackerPos - m_vGhost;
		float fLen1 = VectorLength( &vLen1 );  //�߻�ü�� �̹� Tick ���� �̵��� �Ÿ�
		float fLen2 = VectorLength( &vLen2 );  //�߻�ü�� �����ڿ��� �Ÿ�.
		if( fLen1 > fLen2 )
		{
			m_vGhost = vAttackerPos;

			// �Ѿ�Ƿ�... Ŭ������ ��Ŷ�� ������.
			DWORD dwAddHP = suction_hp_;
			pAttacker->IncreaseHP( dwAddHP );

            // HP����....
            ;{
                MSGSUB_SKILL_VITAL_SUCTION_BRD msg_brd;
                msg_brd.Clear();
                msg_brd.m_dwAttackerKey = pAttacker->GetObjectKey();
                msg_brd.m_dwTargetKey = owner->GetObjectKey();
                msg_brd.m_dwHP = pAttacker->GetHP();
                owner->SendPacketAroundForSync(&msg_brd);
            };

			SUNLOG( eDEV_LOG, "[VitalSuctionStatus::Execute()] AddHP = %d, MonsterObjectKey = %d", dwAddHP, owner->GetObjectKey() );

			// 1. ���͸� �������¸� �����Ų��.
            static_cast<DeadState*>(pState)->SetNextTimeoutInterval(0);

			// ����Ǿ����Ƿ�, �ٽô� Execute�� ������� �ʰ��Ѵ�.
			// ������ End�� ����ǰ� �Ϸ� ������, DeadState�� ����ð��� 0���� �߱� ������, Update�ÿ�
			// FASLE�� ������, �ش� ������ ��ü�� Release �� ���̴�. �׶� �� ���µ� ������ ���̴�.
			StopStatus();

			return;
		}
		else
			m_vGhost = m_vGhost + vTemp;

	}

	AbilityStatus::Execute();
}
 

// ĳ���Ͱ� ������ �ش� Status�� �����ϱ� ���ؼ� Update�� �ش� Status�� �����Ѵ�.
// ���� ���� ���ƾ� �� ���� CanRemove�� �Ѵ�.
BOOL VitalSuctionStatus::CanRemove()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return TRUE;
    }

	//���� ���� ���� ���°� ������ �������� ����, ���� ���°� �ƴϸ� �����϶�.
	NPC* pMonster = (NPC*)owner;
	State* pState = pMonster->GetStateManager()->GetCurState();
	if( !pState )
	{
		SUNLOG( eCRITICAL_LOG, "[VitalSuctionStatus::Init] ���Ͱ� �׾��µ� ���� ���°� ����." );
		return TRUE;
	}

	//���� ���� ���¸�..
	if( pState->GetStateID() == STATE_ID_DEAD )
		return FALSE;

	return TRUE;
}

