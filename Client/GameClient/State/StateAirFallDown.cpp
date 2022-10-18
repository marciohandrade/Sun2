#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "MapObject.h"
#include "TerrainObject.h"
#include "MouseCursor.h"
#include "stateairfalldown.h"
#include "camera.h"
#include "Hero.h"


StateAirFallDown::StateAirFallDown(void)
{
}

StateAirFallDown::~StateAirFallDown(void)
{
}

BOOL StateAirFallDown::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

STATECHANGE::TYPE StateAirFallDown::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	switch( pChr->GetWeaponAnimationCode() )
	{
#ifdef _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
    case '0':	pChr->SetAnimation("HS01",FALSE); break;
    case '1':	pChr->SetAnimation("HS11",FALSE); break;
    case '2':	pChr->SetAnimation("HS21",FALSE); break;
    case '3':	pChr->SetAnimation("HS31",FALSE); break;
#else // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
    case '0':	pChr->SetAnimation("HS03",FALSE); break;
    case '1':	pChr->SetAnimation("HS13",FALSE); break;
    case '2':	pChr->SetAnimation("HS23",FALSE); break;
    case '3':	pChr->SetAnimation("HS33",FALSE); break;
#endif // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
	}
	
	if(pChr==g_pHero)
	{
		g_Camera.StartVibration(100,1.0f);
	}

	pChr->SetFallDown( -7.0f, 38.0f );

	return STATECHANGE::SUCCESS;
}

STATEPROCESS::TYPE StateAirFallDown::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    pChr->ProcessInterpolation(dwTick);

	float fRet = pChr->ProcessAnimation(dwTick);

	pChr->UpdateFallDown( dwTick );

	if( pChr->GetHeight() == -100.0f )
	{
		return STATEPROCESS::SUSPEND;
	}

	if( (pChr->GetCurrentAnimation()==StrToWzID("HS01") ||
		pChr->GetCurrentAnimation()==StrToWzID("HS11") ||
		pChr->GetCurrentAnimation()==StrToWzID("HS21") ||
		pChr->GetCurrentAnimation()==StrToWzID("HS31") )
		&& pChr->GetHeight() <= 0.0f )
	{
		switch( pChr->GetWeaponAnimationCode() )
		{
		case '0':	pChr->SetAnimation("HS04",FALSE); break;
		case '1':	pChr->SetAnimation("HS14",FALSE); break;
		case '2':	pChr->SetAnimation("HS24",FALSE); break;
		case '3':	pChr->SetAnimation("HS34",FALSE); break;
		}
	}
	else if(pChr->GetHeight() == -100.0f )
	{
		return STATEPROCESS::SUSPEND;
	}

    return STATEPROCESS::HOLD;
}

STATECHANGE::TYPE StateAirFallDown::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*pChr = (Character*)pOwner;

	pChr->SetHeight( 0.0f );

	return STATECHANGE::SUCCESS;
}

