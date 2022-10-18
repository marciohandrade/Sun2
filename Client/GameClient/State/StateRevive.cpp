

//------------------------------------------------------------------------------
//  StateRevive.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"



#include "StateRevive.h"
#include "character.h"
#include "HeroActionInput.h"
#include "InterfaceManager.h"
#include "npc.h"
#include "Monster.h"

#include "AppearanceManager.h"



//------------------------------------------------------------------------------
/**
*/
StateRevive::StateRevive(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateRevive::~StateRevive(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateRevive::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateRevive::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	if( pChr->GetTransformData().GetAppearanceKind() == APPEARANCEINFO::PLAYERTRANSFORM )
	{
		if (pChr->IsKindOfObject(PLAYER_OBJECT))
		{
			Player* pPlayer = (Player*)pChr;
			pPlayer->ForceUnTransForm();
			//WzID animationID = pPlayer->GetCurrentDeathAnimation();
			//pPlayer->SetAnimation( animationID, FALSE );
			//pPlayer->GetBasePart()->SetFrame( 1000.0f );	// 마지막 프레임으로 설정

            if (pChr == (Character *)g_pHero)
            {
                g_HeroInput.ClearTransFromQueue();
            }
		}
	}

	WzID AnimID = pChr->GetCurrentReviveAnimation();

	pChr->SetAnimation(AnimID,FALSE);
	

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateRevive::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	
	//	에니메이션 업데이트
	pChr->ProcessInterpolation(dwTick,0.9999f);
	

	// 애니다했으면 끝
	if( 100.0f == pChr->ProcessAnimation( dwTick ) )
	{
		return STATEPROCESS::END;
	}

	// 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
	if (pChr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	// 다음 동작이 있으면 바로 수행해주세요~
	if (pChr->GetActionQueueCount())
	{
		pChr->ProcessNextAction();
		return STATEPROCESS::SUSPEND;
	}
	
	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateRevive::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;


	return STATECHANGE::SUCCESS;
}

