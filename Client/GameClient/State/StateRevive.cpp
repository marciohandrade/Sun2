

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
			//pPlayer->GetBasePart()->SetFrame( 1000.0f );	// ������ ���������� ����

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

	
	//	���ϸ��̼� ������Ʈ
	pChr->ProcessInterpolation(dwTick,0.9999f);
	

	// �ִϴ������� ��
	if( 100.0f == pChr->ProcessAnimation( dwTick ) )
	{
		return STATEPROCESS::END;
	}

	// ��κ��� State���� ���������� ó���ϴ� ���º�ȭ�� ó���Ѵ�.
	if (pChr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

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
STATECHANGE::TYPE StateRevive::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;


	return STATECHANGE::SUCCESS;
}

