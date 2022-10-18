//------------------------------------------------------------------------------
//  StateSpawn.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "MapObject.h"
#include "statetrigger.h"
#include "MouseCursor.h"

//------------------------------------------------------------------------------
/**
*/
StateTrigger::StateTrigger(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateTrigger::~StateTrigger(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateTrigger::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateTrigger::OnStart(Object* pOwner, DWORD CurTime)
{
	MapObject * pMapObject = static_cast<MapObject *>(pOwner);

	if(pMapObject)
	{
		pMapObject->PlayTriggerAnimation();
		pMapObject->SetTriggerOperating(TRUE);
	}

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateTrigger::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	MapObject * pMapObject = static_cast<MapObject *>(pOwner);

	pMapObject->ProcessAnimation(dwTick);

    return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateTrigger::OnEnd(Object* pOwner, DWORD CurTime)
{
	MapObject * pMapObject = static_cast<MapObject *>(pOwner);

	pMapObject->SetTriggerOperating(FALSE);

	return STATECHANGE::SUCCESS;
}
