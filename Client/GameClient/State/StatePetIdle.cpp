#include "SunClientPrecompiledHeader.h"


#include "statebase.h"
#include "character.h"
#include "StatePetIdle.h"

StatePetIdle::StatePetIdle(void)
{
}

StatePetIdle::~StatePetIdle(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StatePetIdle::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StatePetIdle::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetAnimation( "N002" , TRUE);	

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StatePetIdle::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	//	에니메이션 업데이트
	pChr->ProcessInterpolation(dwTick);
	float fRet = pChr->ProcessAnimation( dwTick );

	// 다음 동작이 있으면 바로 수행해주세요~
	if (pChr->GetActionQueueCount())
	{
		pChr->ProcessNextAction();
		return STATEPROCESS::SUSPEND;
	}

	if (fRet > 95.f) 
	{
		return STATEPROCESS::END;
	}

	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StatePetIdle::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetNextState(STATE::NPC_IDLE_STATE, g_CurTime);

	return STATECHANGE::SUCCESS;
}

