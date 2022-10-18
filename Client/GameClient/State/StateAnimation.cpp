//------------------------------------------------------------------------------
//  StateAnimation.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "stateanimation.h"
#include "Character.h"

//------------------------------------------------------------------------------
/**
*/
StateAnimation::StateAnimation(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateAnimation::~StateAnimation(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateAnimation::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateAnimation::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character*)pOwner;

	WzID anim = pChr->GetCurrentIdleAnimation();
	pChr->SetAnimation( anim );
	
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateAnimation::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pChr = (Character*)pOwner;

	pChr->ProcessInterpolation(dwTick);

	if( 100.0f == pChr->ProcessAnimation( dwTick ) )
	{
		return STATEPROCESS::END;
	}

	if (pChr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}
	
	// 다음 행동을 바로바로 처리해준다!
	if (pChr != (Character *)g_pHero) {
		pChr->ProcessNextAction();
		return STATEPROCESS::SUSPEND;		
	}

	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAnimation::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}

