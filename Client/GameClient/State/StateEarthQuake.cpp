//------------------------------------------------------------------------------
//  StateEarthQuake.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "MapObject.h"
#include "TerrainObject.h"

#include "StateEarthQuake.h"
#include "MouseCursor.h"
#include "camera.h"
#include "Hero.h"
#define SHAKE_START_MIN_POWER 0.02f
#define SHAKE_START_MAX_POWER SHAKE_START_MIN_POWER*3.0f

#define SHAKE_END_MIN_POWER 0.2f
#define SHAKE_END_MAX_POWER SHAKE_END_MIN_POWER*3.0f

//------------------------------------------------------------------------------
/**
*/
StateEarthQuake::StateEarthQuake(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateEarthQuake::~StateEarthQuake(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateEarthQuake::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateEarthQuake::OnStart(Object* pOwner, DWORD CurTime)
{
	if(pOwner->IsKindOfObject(MAP_OBJECT))
	{
		MapObject *pMapObj=(MapObject *)pOwner;
		if(pMapObj->IsTerrain())
		{
			TerrainObject *pTerrainObj=(TerrainObject *)pMapObj;

			pTerrainObj->SetAnimation("A003",TRUE);
			pTerrainObj->SetFallDown(FALSE);
		}
	}

	g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_MOTION_BLUR);

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateEarthQuake::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	float fRet = pOwner->ProcessAnimation(dwTick);

	if(pOwner->IsKindOfObject(MAP_OBJECT))
	{
		MapObject *pMapObj=(MapObject *)pOwner;
		if(pMapObj->IsTerrain())
		{
			TerrainObject* pTerrainObj=(TerrainObject *)pMapObj;

			if(pTerrainObj->IsFallDown())
			{
				if(pTerrainObj->GetCurrentAnimation()==StrToWzID("A003"))
				{
					pTerrainObj->SetAnimation("A002",FALSE);
					g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);

					float fPower=SHAKE_END_MAX_POWER;

					if(g_pHero)
					{
						WzVector vOffset = pOwner->GetVisiblePos() - g_pHero->GetVisiblePos();
						float fDistance = VectorLength( &vOffset );

						float fAddPower=0.008f*fDistance;
						
						fPower-=fAddPower;
					}

					fPower=max(fPower,SHAKE_START_MIN_POWER);				
					g_Camera.StartVibration(5,fPower);
					g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_MOTION_BLUR);
					fRet=0.0f;
				}
			}
			else if(g_Camera.GetVibeType()!=Camera::VIBE_TYPE_VERTICAL) //수직으로 흔드는 중에는 무시
			{
				g_Camera.SetVibeType(Camera::VIBE_TYPE_ALL);
				float fPower=SHAKE_START_MAX_POWER;

				if(g_pHero)
				{
					WzVector vOffset = pOwner->GetVisiblePos() - g_pHero->GetVisiblePos();
					float fDistance = VectorLength( &vOffset );

					float fAddPower=0.0005f*fDistance;
					fPower-=fAddPower;
				}

				fPower=min(fPower,SHAKE_START_MAX_POWER);
				g_Camera.StartVibration(100,fPower);

			}
		}
	}


	if(pOwner->GetCurrentAnimation()==StrToWzID("A002")&&fRet==100.0f)
	{
		pOwner->SetShow(FALSE);
	}
	
    return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateEarthQuake::OnEnd(Object* pOwner, DWORD CurTime)
{
	g_Camera.EndVibration();
	g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_MOTION_BLUR);


	return STATECHANGE::SUCCESS;
}
