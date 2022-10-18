//------------------------------------------------------------------------------
//  StateHeroMove.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statemove.h"
#include "stateheromove.h"
#include "Hero.h"
#include "Map.h"
#include "ObjectManager.h"
#include "NumericValues.h"
#include "HeroActionInput.h"

//------------------------------------------------------------------------------
/**
*/
StateHeroMove::StateHeroMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateHeroMove::~StateHeroMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateHeroMove::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case STATE::DAMAGE:
	case STATE::KNOCKBACK2:
	case STATE::IDLE:
	case STATE::MOVE:
	case STATE::ATTACK:
	case STATE::DEATH:
	case STATE::SKILL:
	case STATE::JUMP:
	case STATE::KEYBOARDMOVE:
	case STATE::TRANSFORM:
	case STATE::FALL_DOWN:
	case STATE::EMOTICON:
#ifdef _NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE
    case STATE::DOWN:
#endif //_NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateHeroMove::OnStart(Object* pOwner, DWORD CurTime)
{
    if (pOwner->IsKindOfObject(PLAYER_OBJECT) == false)
    {
        return STATECHANGE::FAILED;
    }
    if (static_cast<Player*>(pOwner)->IsHero() == false)
    {
        return STATECHANGE::FAILED;
    }
    

    // prepare move
	Hero* pHero = static_cast<Hero*>(pOwner);
	WzID animation = pHero->GetCurrentMoveAnimation();
	assert(animation);

	pHero->SetPlayerAnimation(animation,eANI_RUN,TRUE);


#ifdef _NA_000000_20120410_SPEEDHACK
    pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_RUN);
#else
    // 현재 플레이어는 달리기만 가능
    if (pHero->GetMoveState() == CMS_WALK ) 
    {		
        pHero->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN) );
    }
    else
    {
        pHero->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN) );
    }
#endif //_NA_000000_20120410_SPEEDHACK




	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateHeroMove::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE ret;
	// 우선 이동~
	ret = StateMove::OnProcess(pOwner,CurTime,dwTick);

#ifdef PERF_AUTO_MOVE_TEST
	if(g_HeroInput.GetAutoMove()&&ret==STATEPROCESS::END)
	{
		g_HeroInput.SwitchMove();
	}
#endif 

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateHeroMove::OnEnd(Object* pOwner, DWORD CurTime)
{
	WzVector vDummy;
	vDummy.x = 0.0f;
	vDummy.y = 0.0f;
	vDummy.z = 0.0f;
	Hero* pHero = (Hero*)pOwner;
	pHero->LockOnPosition(FALSE,vDummy);
	pHero->SetVisibleAngleModifer(0.0f);



	return StateMove::OnEnd(pOwner,CurTime);
}

