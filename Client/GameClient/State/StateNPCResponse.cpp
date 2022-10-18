#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "character.h"
#include "statenpcresponse.h"
#include "NPC.h"

StateNPCResponse::StateNPCResponse(void)
{
}

StateNPCResponse::~StateNPCResponse(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateNPCResponse::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateNPCResponse::OnStart(Object* pOwner, DWORD CurTime)
{
	NPC* pNPC = static_cast<NPC*>(pOwner);

	pNPC->SetAnimation(pNPC->GetCurrentResponseAnimation(), FALSE);

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateNPCResponse::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateNPCResponse::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetNextState(STATE::NPC_IDLE_STATE, g_CurTime);

	return STATECHANGE::SUCCESS;
}
