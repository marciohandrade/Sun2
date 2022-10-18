//------------------------------------------------------------------------------
//  StateGetup.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "StateGetup.h"
#include "Character.h"

//------------------------------------------------------------------------------
/**
*/
StateGetup::StateGetup(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateGetup::~StateGetup(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateGetup::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateGetup::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	assert(pChr->GetCurrentAction()->ActionID == ACTION_GETUP);

	pChr->SetAnimation( "F003" , FALSE);	
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateGetup::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;	
	pChr->ProcessInterpolation( dwTick );
	float fRet = pChr->ProcessAnimation( dwTick );

	if( fRet >= 100.0f )
	{
		WzID idanim = pChr->GetCurrentIdleAnimation();
		assert(idanim);
		pChr->SetAnimation( idanim , TRUE);
	}
	// 시간이 다돼거나 새 명령이 왔음.
	if (pChr->GetCurrentAction()->GETUP.dwEndTime <= CurTime || pChr->GetActionQueueCount() > 0)
	{		
		if (pChr->ProcessNextAction())
		{		
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			pChr->SetNextState(STATE::IDLE,g_CurTime);
			return STATEPROCESS::SUSPEND;
		}
	}
	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateGetup::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;	
	return STATECHANGE::SUCCESS;
}
