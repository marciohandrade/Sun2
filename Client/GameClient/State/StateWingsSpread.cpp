//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateWingsSpread.h"
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
StateWingsSpread::StateWingsSpread(void)
{
}
//------------------------------------------------------------------------------ 
StateWingsSpread::~StateWingsSpread(void)
{
}
//------------------------------------------------------------------------------ 
BOOL StateWingsSpread::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}
//------------------------------------------------------------------------------ 
bool StateWingsSpread::CanTransite(StateBase* from_state)
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
STATECHANGE::TYPE StateWingsSpread::OnStart(Object* pOwner, DWORD CurTime)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATECHANGE::FAILED;
    }

    if (pOwner == g_pHero)
    {
        if (g_pHero->SendWingsControlByAccessory(true, true) == false)
        {
            return STATECHANGE::FAILED;
        }
    }
	
    //! �ִϸ��� �Ƕ����� ���� ������ �����Ѵ�.
    //! ��Ŷ(MSG_CG_SYNC_WINGS_CONTROL_CMD) �޴ºκ�(����/���� ���)���� �ѹ��� ������ �Ѵ�.
    static_cast<Player*>(pOwner)->set_spread_wings(true);
    static_cast<Player*>(pOwner)->SetPlayerAnimation(StrToWzID("N520"), eANI_WINGS_SPREAD, FALSE);

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------ 
STATEPROCESS::TYPE StateWingsSpread::OnProcess(Object* pOwner, DWORD CurTime, DWORD dwTick)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        assert(pOwner->IsKindOfObject(PLAYER_OBJECT));
        return STATEPROCESS::END;
    }

    Player*	player_ptr = static_cast<Player*>(pOwner);

    if (player_ptr->ProcessAnimation(dwTick) >= 100.0f)
    {
        return STATEPROCESS::END;
    }

	// ��κ��� State���� ���������� ó���ϴ� ���º�ȭ�� ó���Ѵ�.
	if (player_ptr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

    return STATEPROCESS::HOLD;
}
//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateWingsSpread::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}
