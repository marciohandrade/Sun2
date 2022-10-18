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

	// Idle �ִϸ��̼��� ��Ȳ������ ���������̹Ƿ� �� Ŭ���� ���� ó���� �и��Ͽ���.
	pChr->SetIdleAnimation();
	
	//	���ϸ��̼� ������Ʈ
	pChr->ProcessInterpolation(dwTick,0.9999f);
	float fAnim = pChr->ProcessAnimation( dwTick );

	// ��κ��� State���� ���������� ó���ϴ� ���º�ȭ�� ó���Ѵ�.
	if (pChr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	// ���� Idle Ani�� �÷��� �ϱ����� ó��
	SetIdleAnimDelay(pChr,fAnim);

	// ���� ������ ������ �ٷ� �������ּ���~
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
	// �θ��� �Ÿ��°� �����ٸ� Anim��ô���� 100.0f�� �ɰ��̴�.
	if (fAnim >= 100.0f)
	{
		Character *pChr = (Character *)pObj;
		pChr->SetStandAnimationDelay();
	}
}