#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "character.h"
#include "StateNPCSpecialAction_Loop.h"

StateNPCSpecialAction_Loop::StateNPCSpecialAction_Loop(void)
{
}

StateNPCSpecialAction_Loop::~StateNPCSpecialAction_Loop(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateNPCSpecialAction_Loop::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateNPCSpecialAction_Loop::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetAnimation( "SA01" , TRUE);	

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateNPCSpecialAction_Loop::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateNPCSpecialAction_Loop::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetNextState(STATE::NPC_IDLE_STATE, g_CurTime);

	return STATECHANGE::SUCCESS;
}
