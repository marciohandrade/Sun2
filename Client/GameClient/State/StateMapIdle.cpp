//------------------------------------------------------------------------------
//  StateSpawn.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statemapidle.h"
#include "MapObject.h"

//------------------------------------------------------------------------------
/**
*/
StateMapIdle::StateMapIdle(void)
{
}


//------------------------------------------------------------------------------
/**
*/
StateMapIdle::~StateMapIdle(void)
{
}


//------------------------------------------------------------------------------
/**
*/
BOOL StateMapIdle::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateMapIdle::OnStart(Object* pOwner, DWORD CurTime)
{
	MapObject* pMapObject = static_cast<MapObject *>(pOwner);

	// 애니메이션이 없는 녀석들이 있다는 의미
	if( pMapObject->GetAnimationCount() == 0 )
    {
        return STATECHANGE::SUCCESS;
    }
	
	WzID animationID = pMapObject->GetCurrentIdleAnimation();

    WzAnimationInfo* pwzAniInfo = pMapObject->GetAnimationInfo(animationID);
	if (pwzAniInfo && animationID == pwzAniInfo->m_wiAction)
	{
		pMapObject->SetAnimation( animationID, TRUE );
	}
	else
	{
		pMapObject->SetAnimation((WzID)0,FALSE);
	}


	return STATECHANGE::SUCCESS;
}


//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateMapIdle::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	MapObject * pMapObject = static_cast<MapObject *>(pOwner);

	pMapObject->ProcessAnimation(dwTick);

	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateMapIdle::OnEnd(Object* pOwner, DWORD CurTime)
{
	MapObject * pMapObject = static_cast<MapObject *>(pOwner);

	return STATECHANGE::SUCCESS;
}