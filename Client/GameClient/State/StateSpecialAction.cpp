#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "character.h"
#include "StateSpecialAction.h"

StateSpecialAction::StateSpecialAction(void)
{
}

StateSpecialAction::~StateSpecialAction(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateSpecialAction::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateSpecialAction::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetAnimation( "SA00" , FALSE);	

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateSpecialAction::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	//	에니메이션 업데이트
	pChr->ProcessInterpolation(dwTick);
	float fRet = pChr->ProcessAnimation( dwTick );

	if (fRet >= 100.0f) 
	{
		return STATEPROCESS::END;
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
STATECHANGE::TYPE StateSpecialAction::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetNextState(STATE::NPC_IDLE_STATE, g_CurTime);

	return STATECHANGE::SUCCESS;
}
