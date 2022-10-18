#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateRiderSpread.h"
#include "player.h"
#include "hero.h"
#include "Rider.h"

//------------------------------------------------------------------------------ 
StateRiderSpread::StateRiderSpread(void)
{
}
//------------------------------------------------------------------------------ 
StateRiderSpread::~StateRiderSpread(void)
{
}
//------------------------------------------------------------------------------ 
BOOL StateRiderSpread::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}
//------------------------------------------------------------------------------ 
bool StateRiderSpread::CanTransite(StateBase* from_state)
{
    if (from_state == NULL)
    {
        return false;
    }

    switch (from_state->GetStateKind())
    {
    case STATE::MOVE:
    case STATE::KEYBOARDMOVE:
    case STATE::IDLE:
    case STATE::SIT:
        return true;
    }
    
    return false;
}
//------------------------------------------------------------------------------ 
STATECHANGE::TYPE StateRiderSpread::OnStart(Object* pOwner, DWORD CurTime)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATECHANGE::FAILED;
    }

    if (pOwner == g_pHero)
    {
        if (g_pHero->SendWingsControlByRider(true, true) == false)
        {
            return STATECHANGE::FAILED;
        }
    }
	
	Player* player = static_cast<Player*>(pOwner);
	if (player != NULL)
	{
		if (!player->rider())
		{
			return STATECHANGE::FAILED;
		}

		player->rider()->set_spread(true);
		player->SetPlayerAnimation(player->GetCurrentRiderSpreadAnimation(), eANI_RIDER_SPREAD, FALSE);

		return STATECHANGE::SUCCESS;
	}

	return STATECHANGE::FAILED;
}

//------------------------------------------------------------------------------ 
STATEPROCESS::TYPE StateRiderSpread::OnProcess(Object* pOwner, DWORD CurTime, DWORD dwTick)
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
/**
*/
STATECHANGE::TYPE StateRiderSpread::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}
