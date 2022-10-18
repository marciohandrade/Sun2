//------------------------------------------------------------------------------
//  StateMove.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statemove.h"
#include "character.h"
#include "NPC.h"
#include "Monster.h"
#include "Map.h"
#include "Pet.h"
#include "NumericValues.h"
#include "GlobalFunc.h"
#include "Player.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroAttributes.h"


float StateMouseMove::GetMoveLength( Character* pCharacter, DWORD dwTick )
{
    //	이동
#ifdef _NA_000000_20120410_SPEEDHACK
    float fMoveLength = 0.0f;
    if (pCharacter == g_pHero)
    {
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = g_pHero->GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = 0.0f;
        if (g_pHero->IsRiding() || g_pHero->m_bCustomMoveSpeed)
        {
            move_speed_modifier =  g_pHero->GetMoveSpeedModifier();
        }
        else
        {
            HeroAttributes* const hero_attr = (HeroAttributes*)(g_pHero->GetPlayerAttribute());
            move_speed_modifier = hero_attr->move_speed_modifier_;
        }
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        fMoveLength = g_pHero->move_speed_ * move_speed_modifier * (float)dwTick;
    }
    else
    {
        fMoveLength = pCharacter->GetMoveSpeed() * pCharacter->GetMoveSpeedModifier() * (float)dwTick;
    }
#else
    float fMoveLength = pCharacter->GetMoveSpeed() * pCharacter->GetMoveSpeedModifier() * (float)dwTick;
#endif //_NA_000000_20120410_SPEEDHACK


    return fMoveLength;
}



//------------------------------------------------------------------------------
/**
*/
StateMove::StateMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateMove::~StateMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateMove::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateMove::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->StartNextMove();
	WzID animation;

	animation = pChr->GetCurrentMoveAnimation();
	assert(animation);

	if ( pChr->IsKindOfObject(PLAYER_OBJECT) )
	{
		Player *pPlayer = (Player *)pChr;
		pPlayer->SetPlayerAnimation( animation,eANI_RUN, TRUE );
	}
	else
	{
		pChr->SetAnimation( animation, TRUE );	
	}

	
	if ( pChr->IsKindOfObject(MONSTER_OBJECT) )
	{		
		Monster *pMonster = (Monster *)pChr;

		if (pChr->GetMoveState() == CMS_WALK ) {		
			pChr->SetMoveSpeed( pMonster->GetMonsterInfo()->m_fWalkSpeed * SPEED_MULTIPLIER);
		}
		else
		{		
			pChr->SetMoveSpeed( pMonster->GetMonsterInfo()->m_fRunSpeed * SPEED_MULTIPLIER);
			pChr->OnStartRun();
		}
		
	}
	else if(pChr->IsKindOfObject(PET_OBJECT) )
	{
		Pet *pPet = (Pet *)pChr;

		if (pChr->GetMoveState() == CMS_WALK ) {		
			pChr->SetMoveSpeed( pPet->GetMonsterInfo()->m_fWalkSpeed * SPEED_MULTIPLIER);
		}
		else
		{		
			pChr->SetMoveSpeed( pPet->GetMonsterInfo()->m_fRunSpeed * SPEED_MULTIPLIER);
			pChr->OnStartRun();
		}
	}
	else if (pChr->IsKindOfObject(NPC_OBJECT) )
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
STATEPROCESS::TYPE StateMove::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetMoveAnimation();




    CPathExplorer* pexp = pChr->GetPathExplorer();
    WzVector altPos = pexp->GetPos();

 	WzVector vOldPos = pChr->GetPosition();
	SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));


    WzPathFindEvent wzEvent;
    ZeroMemory(&wzEvent,sizeof(wzEvent));

	if (pChr->GetPathExplorer()->IsMoving()) 
    {
        float fMoveLength = GetMoveLength(pChr, dwTick );

		if (g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveLength ,&wzEvent))
		{
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

    //	이동
#ifdef _NA_000000_20120410_SPEEDHACK
    if (pChr == g_pHero)
    {
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = g_pHero->GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = 0.0f;
        if (g_pHero->IsRiding() || g_pHero->m_bCustomMoveSpeed)
        {
            move_speed_modifier =  g_pHero->GetMoveSpeedModifier();
        }
        else
        {
            HeroAttributes* const hero_attr = (HeroAttributes*)(g_pHero->GetPlayerAttribute());
            move_speed_modifier = hero_attr->move_speed_modifier_;
        }
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        pChr->ProcessAnimation((DWORD)(dwTick * move_speed_modifier));	
    }
    else
    {
        pChr->ProcessAnimation((DWORD)( dwTick * pChr->GetMoveSpeedModifier() ) );	
    }
#else
    pChr->ProcessAnimation((DWORD)( dwTick * pChr->GetMoveSpeedModifier() ) );	
#endif //_NA_000000_20120410_SPEEDHACK

	

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
STATECHANGE::TYPE StateMove::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetJumpFlag(FALSE);

	return STATECHANGE::SUCCESS;
}