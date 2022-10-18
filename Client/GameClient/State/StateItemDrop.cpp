//------------------------------------------------------------------------------
//  StateSpawn.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "stateitemdrop.h"
#include "Item.h"


//------------------------------------------------------------------------------
/**
*/
StateItemDrop::StateItemDrop(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateItemDrop::~StateItemDrop(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateItemDrop::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateItemDrop::OnStart(Object* pOwner, DWORD CurTime)
{
	Item * pItem = static_cast<Item *>(pOwner);

	// 공격 애니메이션 세팅
	WzID animationID = pItem->GetCurrentDropAnimation();
	assert(animationID);
	pItem->SetAnimation(animationID,FALSE);

	
	

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateItemDrop::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateItemDrop::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}