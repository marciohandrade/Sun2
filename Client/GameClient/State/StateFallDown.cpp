//------------------------------------------------------------------------------
//  StateFallDown.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "MapObject.h"
#include "TerrainObject.h"

#include "StateFallDown.h"
#include "MouseCursor.h"
#include "camera.h"
#include "Hero.h"


//------------------------------------------------------------------------------
/**
*/
StateFallDown::StateFallDown(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateFallDown::~StateFallDown(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateFallDown::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateFallDown::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	if(pChr==g_pHero)
	{
		g_Camera.StartVibration(100,1.0f);
	}

	pChr->SetFallDown( -7.0f, 0.0f );

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateFallDown::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pChr = (Character*)pOwner;

    pChr->ProcessInterpolation(dwTick);

	float fRet = pChr->ProcessAnimation(dwTick);
	
	pChr->UpdateFallDown( dwTick );

	if( pChr->GetHeight() == -100.0f )
	{
		return STATEPROCESS::SUSPEND;
	}

    return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateFallDown::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	pChr->SetHeight(0.0f);

	return STATECHANGE::SUCCESS;
}
