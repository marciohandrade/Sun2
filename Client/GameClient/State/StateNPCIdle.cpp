#include "SunClientPrecompiledHeader.h"
#include "StateNPCIdle.h"
#include "character.h"
#include "npc.h"
#include "Monster.h"

StateNPCIdle::StateNPCIdle(void)
{
}

StateNPCIdle::~StateNPCIdle(void)
{
}


//------------------------------------------------------------------------------
/**
*/
BOOL StateNPCIdle::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateNPCIdle::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	WzID wzAniID = pChr->GetCurrentIdleAnimation();
	pChr->SetAnimation( wzAniID , TRUE);	

	NPC * pNPC = static_cast<NPC *>(pChr);
	if (pNPC)
	{
		pNPC->InitDelaySwingTick();
		pNPC->SetDelaySwingTick(CurTime);
	}
	else
	{
		assert(pNPC);
	}

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateNPCIdle::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	//	에니메이션 업데이트
	pChr->ProcessInterpolation(dwTick);
	float fAnim =pChr->ProcessAnimation( dwTick );
		
	// 스윙하기 얼마나 남았는지 체크해서 스테이트 바꿔준다.
	NPC * pNPC = static_cast<NPC *>(pChr);
	int iRemainTickDelay = pNPC->ProcessDelaySwingTick(dwTick);

	if (iRemainTickDelay <= 0)
	{
		pChr->SetNextState(STATE::NPC_SWING_STATE, CurTime);
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
STATECHANGE::TYPE StateNPCIdle::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;


	return STATECHANGE::SUCCESS;
}
