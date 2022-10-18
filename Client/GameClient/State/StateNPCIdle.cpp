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

	//	���ϸ��̼� ������Ʈ
	pChr->ProcessInterpolation(dwTick);
	float fAnim =pChr->ProcessAnimation( dwTick );
		
	// �����ϱ� �󸶳� ���Ҵ��� üũ�ؼ� ������Ʈ �ٲ��ش�.
	NPC * pNPC = static_cast<NPC *>(pChr);
	int iRemainTickDelay = pNPC->ProcessDelaySwingTick(dwTick);

	if (iRemainTickDelay <= 0)
	{
		pChr->SetNextState(STATE::NPC_SWING_STATE, CurTime);
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
STATECHANGE::TYPE StateNPCIdle::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;


	return STATECHANGE::SUCCESS;
}
