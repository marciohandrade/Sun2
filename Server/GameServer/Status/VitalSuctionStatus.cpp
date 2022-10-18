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

	// 타겟의 죽어서 사라지는 시간을 새로 설정한다.
	// 소유자는 몬스터여만 한다.
	NPC* pMonster = (NPC*)pOwner;
	State* pState = pMonster->GetStateManager()->GetCurState();
	if( !pState )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] 몬스터가 죽었는데 죽은 상태가 없다." );
		return;
	}

	if( pState->GetStateID() != STATE_ID_DEAD )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] 몬스터가 죽었는데 죽은 상태가 아니다. CurState = %d", pState->GetStateID() );
        // (CHANGES) (100223) (WAVERIX) fix up missing return that post action assume dead state
        return;
	}

	// 몬스터의 DEAD상태를 증가 시킨다.
    static_cast<DeadState*>(pState)->SetNextTimeoutInterval(20 * 1000);
}

VOID		VitalSuctionStatus::Execute()
{
	Character* pAttacker = m_pGameField->FindCharacter( m_dwAttackerObjectKey );
    // (WAVERIX) (090608) (WOPS:4077) VitalSuction 실행 제약 조건 추가
	if(pAttacker == 0 || pAttacker->IsDead())
	{
		// 그리고 현재 상태 소유자는 소멸시킨다.
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
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] 몬스터가 죽었는데 죽은 상태가 없다." );
		StopStatus();
		return;
	}

	if( pState->GetStateID() != STATE_ID_DEAD )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] VitalSuctionStatus인데 죽은 상태가 아니다. CurState = %d", pState->GetStateID() );
		StopStatus();
		return;
	}

	DWORD	dwCurTick = GetTickCount();
	DWORD dwTick = dwCurTick - m_dwPreTick;
	WzVector vTemp, vAxis, vLen1, vLen2, vAttackerPos;

	//1초 동안은 위로 상승한다.
	if( ( m_dwFirstTick + 1000 ) >= dwCurTick )
	{
		WzVector vUp;
		SetVector( &vUp, 0.f, 0.f, 1.f );
		vTemp = vUp*(dwTick*m_fSpeed);			//단위벡터..
		m_vGhost = m_vGhost + vTemp;
	}
	else
	{
		pAttacker->GetPos( &vAttackerPos );
		WzVector vLen = vAttackerPos - m_vGhost;
		float fLen = VectorLength( &vLen );
		//외적 구하는 함수.
		VectorCrossProduct( &vAxis, &m_vGhostUnitDir, &vLen );

		//회전각
		float theta = 0.001f * dwTick;

		//회전변환용 행렬 생성.
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

		// 한번 Execute할때 마다( 1초 ) 이동하는 거리..
		m_fSpeed += m_fAccelate;
		vTemp = m_vGhostUnitDir*( dwTick*m_fSpeed );
		WzVector vLenCheck = m_vGhost + vTemp;
		vLen1 = vLenCheck - m_vGhost;
		vLen2 = vAttackerPos - m_vGhost;
		float fLen1 = VectorLength( &vLen1 );  //발사체가 이번 Tick 동안 이동한 거리
		float fLen2 = VectorLength( &vLen2 );  //발사체와 공격자와의 거리.
		if( fLen1 > fLen2 )
		{
			m_vGhost = vAttackerPos;

			// 넘어가므로... 클라한테 패킷을 보낸다.
			DWORD dwAddHP = suction_hp_;
			pAttacker->IncreaseHP( dwAddHP );

            // HP증가....
            ;{
                MSGSUB_SKILL_VITAL_SUCTION_BRD msg_brd;
                msg_brd.Clear();
                msg_brd.m_dwAttackerKey = pAttacker->GetObjectKey();
                msg_brd.m_dwTargetKey = owner->GetObjectKey();
                msg_brd.m_dwHP = pAttacker->GetHP();
                owner->SendPacketAroundForSync(&msg_brd);
            };

			SUNLOG( eDEV_LOG, "[VitalSuctionStatus::Execute()] AddHP = %d, MonsterObjectKey = %d", dwAddHP, owner->GetObjectKey() );

			// 1. 몬스터를 죽은상태를 만료시킨다.
            static_cast<DeadState*>(pState)->SetNextTimeoutInterval(0);

			// 실행되었으므로, 다시는 Execute가 실행되지 않게한다.
			// 원래는 End가 실행되게 하려 했으나, DeadState의 만료시간을 0으로 했기 때문에, Update시에
			// FASLE를 리턴해, 해당 몬스터의 객체를 Release 할 것이다. 그때 이 상태도 없어질 것이다.
			StopStatus();

			return;
		}
		else
			m_vGhost = m_vGhost + vTemp;

	}

	AbilityStatus::Execute();
}
 

// 캐릭터가 죽으면 해당 Status를 제거하기 위해서 Update시 해당 Status를 제거한다.
// 제거 하지 말아야 할 것은 CanRemove로 한다.
BOOL VitalSuctionStatus::CanRemove()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return TRUE;
    }

	//만일 현재 죽은 상태가 있으면 제거하지 말고, 죽은 상태가 아니면 제거하라.
	NPC* pMonster = (NPC*)owner;
	State* pState = pMonster->GetStateManager()->GetCurState();
	if( !pState )
	{
		SUNLOG( eCRITICAL_LOG, "[VitalSuctionStatus::Init] 몬스터가 죽었는데 죽은 상태가 없다." );
		return TRUE;
	}

	//현재 죽은 상태면..
	if( pState->GetStateID() == STATE_ID_DEAD )
		return FALSE;

	return TRUE;
}

