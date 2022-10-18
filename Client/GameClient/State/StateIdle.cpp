//------------------------------------------------------------------------------
//  StateIdle.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "stateidle.h"
#include "character.h"
#include "HeroActionInput.h"
#include "InterfaceManager.h"
#include "npc.h"
#include "Monster.h"

//------------------------------------------------------------------------------
/**
*/
StateIdle::StateIdle(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateIdle::~StateIdle(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateIdle::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateIdle::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetStandAnimationDelay();


	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateIdle::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	// Idle 애니메이션은 상황에따라 여러가지이므로 각 클래스 별로 처리를 분리하였음.
	pChr->SetIdleAnimation();
	
	//	에니메이션 업데이트
	pChr->ProcessInterpolation(dwTick,0.9999f);
	float fAnim = pChr->ProcessAnimation( dwTick );

	// 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
	if (pChr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	// 가끔 Idle Ani를 플레이 하기위한 처리
	SetIdleAnimDelay(pChr,fAnim);

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
STATECHANGE::TYPE StateIdle::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;


	return STATECHANGE::SUCCESS;
}

void StateIdle::SetIdleAnimDelay(Object *pObj,float fAnim)
{
	// 두리번 거리는게 끝났다면 Anim진척률이 100.0f이 될것이다.
	if (fAnim >= 100.0f)
	{
		Character *pChr = (Character *)pObj;
		pChr->SetStandAnimationDelay();
	}
}