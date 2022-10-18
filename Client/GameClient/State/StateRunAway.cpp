//------------------------------------------------------------------------------
//  StateRunAway.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateRunAway.h"
#include "character.h"
#include "NPC.h"
#include "Monster.h"
#include "Map.h"
#include "NumericValues.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
/**
*/
StateRunAway::StateRunAway(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateRunAway::~StateRunAway(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateRunAway::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateRunAway::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->StartNextMove();
	WzID animation;

	animation = pChr->GetCurrentRunAwayAnimation();
	assert(animation);

	pChr->SetAnimation( animation, TRUE );
	
	if ( pChr->IsKindOfObject(MONSTER_OBJECT) )
	{		
		Monster *pMonster = (Monster *)pChr;
		pChr->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_LUCKY_MONSTER_RUNAWAY) );		
	}
	else if (pChr->IsKindOfObject(NPC_OBJECT))
	{
		NPC *pNPC = (NPC *)pChr;

		if (pChr->GetMoveState() == CMS_WALK ) {		
			pChr->SetMoveSpeed( pNPC->GetMonsterInfo()->m_fWalkSpeed * SPEED_MULTIPLIER);
		}
		else
		{		
			pChr->SetMoveSpeed( pNPC->GetMonsterInfo()->m_fRunSpeed * SPEED_MULTIPLIER);
			pChr->OnStartRun();
		}
	}
	else 
	{
		if (pChr->GetMoveState() == CMS_WALK )
		{		
			pChr->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_WALK) );
		}
		else
		{

			pChr->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN) );
			pChr->OnStartRun();
		}
	}

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateRunAway::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;
	//	이동
	float fMoveSpeed = pChr->GetMoveSpeed() * pChr->GetMoveSpeedModifier() * (float)dwTick;
	WzPathFindEvent wzEvent;
	ZeroMemory(&wzEvent,sizeof(wzEvent));

    pChr->SetRunAwayAnimation();

    CPathExplorer* pexp = pChr->GetPathExplorer();
    WzVector altPos = pexp->GetPos();

 	WzVector vOldPos = pChr->GetPosition();
	SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));
	if (pChr->GetPathExplorer()->IsMoving()) 
    {
		if (g_pSunTerrain->PathExp_Progress( pChr->GetPathExplorer(), fMoveSpeed ,&wzEvent))
		{
			//	방향 설정
			pChr->SetDirection( pChr->GetPosition(), vOldPos);
		}




	}

	SUNPROCESSPROFILE_END(_T("PathFinding")); 
	
	// 최종적인 점프 정보만 중요하다.
	if (wzEvent.m_iCountJump > 0)
	{
		int lastindex = wzEvent.m_iCountJump - 1;
		if (wzEvent.m_Jump[lastindex].m_eJumpStep == EJS_JUMPSTART)
		{
			// 점프로
			pChr->SetJumpInfo(&wzEvent.m_Jump[lastindex].m_wvJumpFrom,&wzEvent.m_Jump[lastindex].m_wvJumpTo);
			pChr->SetAnimation("J001");
			pChr->SetJumpFlag(TRUE);			
			pChr->ProcessInterpolation(dwTick);
			pChr->ProcessAnimation(dwTick);
			pChr->SetNextState(STATE::JUMP,g_CurTime);
			return STATEPROCESS::SUSPEND;
		}
	}
	
	pChr->ProcessInterpolation(dwTick);	
	pChr->ProcessAnimation((DWORD)( dwTick * pChr->GetMoveSpeedModifier() ) );	

	if (pChr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}
	
	// 이동이 다끝났으면
	if( FALSE == pChr->GetPathExplorer()->IsMoving() )
	{		
		// 다음 행동을!
		if (pChr != (Character *)g_pHero) 
        {
			if (pChr->ProcessNextAction())
			{		
				return STATEPROCESS::SUSPEND;		
			}
			else
			{
				return STATEPROCESS::END;		
			}
		}

		return STATEPROCESS::END;				
	}

	if (pChr != (Character *)g_pHero) 
    {
		if (pChr->GetActionQueueCount()) 
        {
			if (pChr->ProcessNextAction())
			{		
				return STATEPROCESS::SUSPEND;					
			}			
		}
	}

	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateRunAway::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetJumpFlag(FALSE);


	return STATECHANGE::SUCCESS;
}

