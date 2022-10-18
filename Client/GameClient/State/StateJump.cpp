//------------------------------------------------------------------------------
//  StateJump.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statejump.h"
#include "character.h"
#include "monster.h"
#include "NumericValues.h"

//------------------------------------------------------------------------------
/**
*/
StateJump::StateJump(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateJump::~StateJump(void)
{
}


//------------------------------------------------------------------------------
/**
	점프는 MOVE 로 반드시 돌아가야한다.
	죽지도 않아요~
*/
BOOL StateJump::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case STATE::MOVE:
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateJump::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetAnimation( "J001" );
	if(pChr->IsKindOfObject(MONSTER_OBJECT) )
	{		
		Monster *pMonster = (Monster *)pChr;

		if (pChr->GetMoveState() == CMS_WALK ) {		
			pChr->SetMoveSpeed( pMonster->GetMonsterInfo()->m_fWalkSpeed * SPEED_MULTIPLIER);
		}
		else
		{		
			pChr->SetMoveSpeed( pMonster->GetMonsterInfo()->m_fRunSpeed * SPEED_MULTIPLIER * 2.0f);
		}
	}
	else 
	{
		if (pChr->GetMoveState() == CMS_WALK ) {		
			pChr->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN) );
		}
		else
		{
			pChr->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN) );
		}
	}
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateJump::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE ret = STATEPROCESS::HOLD;
	Character*	pChr = (Character*)pOwner;

	//	이동
	float fMoveSpeed = pChr->GetMoveSpeed() * (float)dwTick;
	WzPathFindEvent wzEvent;
	ZeroMemory(&wzEvent,sizeof(wzEvent));

	WzVector vOldPos = pChr->GetPosition();

	SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));

	if (g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed ,&wzEvent))
	{
		//	방향 설정
		pChr->SetDirection(pChr->GetPosition(), vOldPos);		
	}

	

	SUNPROCESSPROFILE_END(_T("PathFinding"));
	
	// 최종적인 점프 정보만 중요하다.
	if (wzEvent.m_iCountJump > 0)
	{
		int lastindex = wzEvent.m_iCountJump - 1;
		if (wzEvent.m_Jump[lastindex].m_eJumpStep == EJS_JUMPEND)
		{
			// 점프로
			pChr->SetJumpFlag(FALSE);
			pChr->SetNextState(STATE::MOVE,g_CurTime);			
			ret = STATEPROCESS::SUSPEND;
		}
	}

	float fFrame = pChr->GetCurrentFrame();
	pChr->ProcessInterpolation(dwTick);
	WzVector vJumpFrom;
	WzVector vJumpTo;
	float fDistance;
	float fCurDistance;
	float fMoveRate;

	pChr->GetJumpInfo(&vJumpFrom,&vJumpTo,&fDistance);

	float fHeightConst = -fDistance * 1.0f;

	WzVector vCurrent = pChr->GetPathExplorer()->GetPos();
	WzVector vCurDistance = vJumpTo - vCurrent;

	fCurDistance = VectorLength(&vCurDistance);

	if (fCurDistance >= fDistance)
	{
		fCurDistance = fDistance;
	}

	if (fDistance <= 0.0f) 
	{
		fMoveRate = 1.0f;
	}
	else
	{
		fMoveRate = fCurDistance / fDistance;
	}

	float fTotalFrame = pChr->GetTotalFrame();
	// 1번째방법

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

	pChr->m_fHeight = fHeightConst * fMoveRate * (fMoveRate - 1.0f);

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateJump::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetJumpFlag(FALSE);
	pChr->m_fHeight = 0.0f;
	return STATECHANGE::SUCCESS;
}


