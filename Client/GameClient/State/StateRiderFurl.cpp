#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateRiderFurl.h"
#include "player.h"
#include "hero.h"
#include "Rider.h"

//------------------------------------------------------------------------------
StateRiderFurl::StateRiderFurl(void)
{
}

//------------------------------------------------------------------------------
StateRiderFurl::~StateRiderFurl(void)
{
}
//------------------------------------------------------------------------------
BOOL StateRiderFurl::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}
//------------------------------------------------------------------------------
STATECHANGE::TYPE StateRiderFurl::OnStart(Object* pOwner, DWORD CurTime)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATECHANGE::FAILED;
    }
	
    if (pOwner == g_pHero)
    {
        if (g_pHero->SendWingsControlByRider(false, true) == false)
        {
            return STATECHANGE::FAILED;
        }
    }

    Player* player = static_cast<Player*>(pOwner);
    if (player != NULL)
    {
        player->SetPlayerAnimation(player->GetCurrentRiderFureAnimation(), eANI_RIDER_FURL, FALSE);
    }
    
	return STATECHANGE::SUCCESS;
}
//------------------------------------------------------------------------------
STATEPROCESS::TYPE StateRiderFurl::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATEPROCESS::END;
    }

    Player*	player_ptr = static_cast<Player*>(pOwner);

    float player_progress_rate = player_ptr->ProcessAnimation(dwTick);
    if (((player_ptr->rider() == NULL) && (player_progress_rate >= 100.0f)) || 
        ((player_ptr->rider() != NULL) && (player_ptr->rider()->current_ani_progress_rate() >= 100.0f)))
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
STATECHANGE::TYPE StateRiderFurl::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}
