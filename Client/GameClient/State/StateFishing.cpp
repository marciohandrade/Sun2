#include "SunClientPrecompiledHeader.h"

#ifdef _YJ_FISHING_SYSTEM_000933

#include "statebase.h"
#include "StateFishing.h"
#include "Character.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//StateFishing_Throw Class

StateFishing_Throw::StateFishing_Throw(void)
{
}

StateFishing_Throw::~StateFishing_Throw(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateFishing_Throw::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateFishing_Throw::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetAnimation( "N401", FALSE);

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateFishing_Throw::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateFishing_Throw::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetNextState(STATE::FISHING_STAND, g_CurTime);

	return STATECHANGE::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//StateFishing_Stand Class

StateFishing_Stand::StateFishing_Stand(void)
{
}

StateFishing_Stand::~StateFishing_Stand(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateFishing_Stand::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateFishing_Stand::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;

	pChr->SetAnimation( "N402", TRUE); //Loop

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateFishing_Stand::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateFishing_Stand::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetCurFishingAniID(0);
	return STATECHANGE::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//StateFishing_Bite Class

StateFishing_Bite::StateFishing_Bite(void)
{
}

StateFishing_Bite::~StateFishing_Bite(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateFishing_Bite::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateFishing_Bite::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetAnimation( "N403", FALSE);

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateFishing_Bite::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateFishing_Bite::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetCurFishingAniID(0);

	return STATECHANGE::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//StateFishing_Success Class

StateFishing_Success::StateFishing_Success(void)
{
}

StateFishing_Success::~StateFishing_Success(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateFishing_Success::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateFishing_Success::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;

	pChr->SetAnimation( "N404", FALSE);

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateFishing_Success::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateFishing_Success::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetCurFishingAniID(0);

	return STATECHANGE::SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//StateFishing_Failed Class

StateFishing_Failed::StateFishing_Failed(void)
{
}

StateFishing_Failed::~StateFishing_Failed(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateFishing_Failed::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateFishing_Failed::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;

	pChr->SetAnimation( "N405", FALSE);

	return STATECHANGE::SUCCESS;	
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateFishing_Failed::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE StateFishing_Failed::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pChr = (Player*)pOwner;
	pChr->SetCurFishingAniID(0);

	return STATECHANGE::SUCCESS;
}

#endif 