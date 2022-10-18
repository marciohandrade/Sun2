//------------------------------------------------------------------------------
//  StateEventJump.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "StateEventJump.h"
#include "character.h"
#include "monster.h"

// �߷°��ӵ�
const int GRAVITY = 20;
// �����ϴ� ����
const float UP_HEIGHT = 2.5f;
const int X_ACCELERATION = 20;

//------------------------------------------------------------------------------
/**
*/
StateEventJump::StateEventJump(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateEventJump::~StateEventJump(void)
{
}

//------------------------------------------------------------------------------
/**
	������ MOVE �� �ݵ�� ���ư����Ѵ�.
	������ �ʾƿ�~
*/
BOOL StateEventJump::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{	
	return TRUE;	
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateEventJump::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
    
    pChr->SetFreeMove(TRUE);

    ATTACK_JUMP_INFO * pAttackJump = &pChr->m_AttackJumpInfo;

    // ���� �ִϸ��̼� ����
    WzID animationID = pChr->GetCurrentAttackJumpAnimation();
    assert(animationID);
    pChr->SetAnimation( "J001" );

    pAttackJump->m_dwTotalTime = 
        pChr->GetCurrentAction()->EVENT_JUMP.dwEndTime - 
        pChr->GetCurrentAction()->EVENT_JUMP.dwStartTime;

    // �Ʒ��� �������� �ϴ� ����
    pAttackJump->m_fDestHeight = 
        pChr->GetCurrentAction()->EVENT_JUMP.vDestPos.wvPos.z - 
        pChr->GetCurrentAction()->EVENT_JUMP.vCurPos.wvPos.z;

    // ������ ��ǥ�� ���� ��ǥ���� ����
    pAttackJump->m_vDist = 
        pChr->GetCurrentAction()->EVENT_JUMP.vDestPos.wvPos - 
        pChr->GetCurrentAction()->EVENT_JUMP.vCurPos.wvPos;


    // ĳ���� ���� ����
    pChr->SetAngle( Math_GetAngleFromVector(&pAttackJump->m_vDist), TRUE );

    if (UP_HEIGHT == 0 && pAttackJump->m_fDestHeight == 0)
    {
        return STATECHANGE::FAILED;
    }

    // ������ ������ ���� �ʱ� �ӵ�
    float fDestLength = VectorLength( &pAttackJump->m_vDist );    
    float fUpHeight = max( UP_HEIGHT, pAttackJump->m_fDestHeight ); 
    pAttackJump->m_fVelocity_Y = sqrt( 2 * GRAVITY * fUpHeight );

    if( pAttackJump->m_fDestHeight >= UP_HEIGHT )
    {
        pAttackJump->m_fVelocity_X = fDestLength * GRAVITY / pAttackJump->m_fVelocity_Y;
    }
    else
    {
        pAttackJump->m_fVelocity_X = fDestLength * GRAVITY /
            ( 
            pAttackJump->m_fVelocity_Y + 
            sqrt( pAttackJump->m_fVelocity_Y * pAttackJump->m_fVelocity_Y - 2*GRAVITY*pAttackJump->m_fDestHeight ) 
            );
    }

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateEventJump::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE ret = STATEPROCESS::HOLD;
	Character*	pChr = (Character*)pOwner;

    ATTACK_JUMP_INFO * pAttackJump = &pChr->m_AttackJumpInfo;

    float fRet = 0.f; 

    DWORD dwRunTime = g_CurTime - pChr->GetCurrentAction()->EVENT_JUMP.dwStartTime ;
    float fMoveRate = (float)dwRunTime / (float)pChr->m_AttackJumpInfo.m_dwTotalTime;
    if (fMoveRate >= 1.0f)
    {
        fMoveRate = 1.0f;
    }
    // ���(x,y)������ �̵��� �Ÿ�
    float fMoveLength = VectorLength( &( pAttackJump->m_vDist * fMoveRate ) );
    float fVecLength = VectorLength( &pAttackJump->m_vDist );

    // fMoveLength�� �������� ���� ���� ���� ����Ѵ�.
    float fResult_Delta_Z = 
        ( 2*pAttackJump->m_fVelocity_X*pAttackJump->m_fVelocity_Y-GRAVITY*fMoveLength ) / 
        ( 2*pAttackJump->m_fVelocity_X*pAttackJump->m_fVelocity_X ) * fMoveLength;

	// �������� ���� ��ǥ�� �����Ѵ�.
	WzVector vResultPos;
    vResultPos = 
        pChr->GetCurrentAction()->EVENT_JUMP.vCurPos.wvPos + pAttackJump->m_vDist * fMoveRate;
    vResultPos.z = fResult_Delta_Z + pChr->GetCurrentAction()->EVENT_JUMP.vCurPos.wvPos.z;

	float fTotalFrame = pChr->GetTotalFrame();
	if (fMoveRate <= 0.2f) 
	{
		float fNewFrame = (fTotalFrame * 0.5f)  * (fMoveRate / 0.2f);
		pChr->SetCurrentFrame(fNewFrame);
		pChr->ProcessAnimation(0);
	}	
	else if (fMoveRate >= 0.8f)
	{
		if (pChr->IsKindOfObject(MONSTER_OBJECT) )
		{
			pChr->SetAnimation("J002",FALSE);
		}

		float fNewFrame = (fTotalFrame * 0.5f) * (1.0f + (fMoveRate - 0.8f) / 0.2f);	
		pChr->SetCurrentFrame(fNewFrame);
		pChr->ProcessAnimation(0);
	}
	else
	{
		if (pChr->IsKindOfObject(MONSTER_OBJECT) )
		{
			pChr->SetAnimation("N007",TRUE);
		}
		
		pChr->ProcessAnimation(dwTick);
	}

	pChr->ProcessInterpolation(dwTick);
	
	// process interpolation ������renderpart��ġ�� set�ϹǷ� ��ȿ�ϴ�.
	pChr->GetBasePart()->SetPosition(vResultPos);

	if (fMoveRate >= 1.0f)
	{
		pChr->SetPosition(pChr->GetCurrentAction()->EVENT_JUMP.vDestPos.wvPos,pChr->GetCurrentAction()->EVENT_JUMP.vDestPos.wTile);
		pChr->SetInterpolatePosition();
		pChr->SetVisiblePos(pChr->GetCurrentAction()->EVENT_JUMP.vDestPos.wvPos);

		if (pChr->ProcessNextAction())
		{
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			return STATEPROCESS::END;
		}
	}
	
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateEventJump::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetJumpFlag(FALSE);
	pChr->m_fHeight = 0.0f;
	return STATECHANGE::SUCCESS;
}


