//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateWingsFurl.h"
#include "player.h"
#include "hero.h"
#include "CastDialog.h"
#include "RiderContainer.h"
#include "HeroActionInput.h"
#include "RidingInfoParser.h"
#include "CharInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include <SCItemSlot.h>
//------------------------------------------------------------------------------
/**
*/
StateWingsFurl::StateWingsFurl(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateWingsFurl::~StateWingsFurl(void)
{
}
//------------------------------------------------------------------------------
/**
*/
BOOL StateWingsFurl::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateWingsFurl::OnStart(Object* pOwner, DWORD CurTime)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATECHANGE::FAILED;
    }
	
    if (pOwner == g_pHero)
    {
        if (g_pHero->SendWingsControlByAccessory(false, true) == false)
        {
            return STATECHANGE::FAILED;
        }
    }

    static_cast<Player*>(pOwner)->SetPlayerAnimation(StrToWzID("N521"), eANI_WINGS_FURL, FALSE);
    
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateWingsFurl::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATEPROCESS::END;
    }

    Player*	player_ptr = static_cast<Player*>(pOwner);

    WzEvent event;
	float ani_ratio = player_ptr->ProcessAnimation(dwTick, &event);
    if (ani_ratio >= 100.0f)
    {
        return STATEPROCESS::END;
    }

	// 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
	if (player_ptr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

    return STATEPROCESS::HOLD;
}
//------------------------------------------------------------------------------ 
STATECHANGE::TYPE StateWingsFurl::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}
