//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statesit.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "InterfaceManager.h"
#include "npc.h"
#include "Monster.h"
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#include "Map.h"
#include "SpaZoneInfoParser.h"
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
//------------------------------------------------------------------------------
/**
*/
StateSit::StateSit(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateSit::~StateSit(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateSit::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateSit::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    if (pPlayer->IsInSpaTile_Change_Sit())
    {
        pPlayer->SetSitState(TRUE);
        pPlayer->SetStandUp(FALSE);

        if(g_pHero->GetObjectKey()==pPlayer->GetObjectKey())
        {
            g_HeroInput.SetAutoRun(FALSE);
            g_HeroInput.SetAutoAttack(FALSE);
            g_HeroInput.SetAreaSkill(FALSE);
            g_SkillQueue.ClearAll();
        }

        return STATECHANGE::SUCCESS;
    }
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

	WzID anim;

	pPlayer->SetSitState( FALSE );
	pPlayer->SetStandUp(FALSE);


	anim = pPlayer->GetCurrentSitAnimation();
	pPlayer->SetPlayerAnimation(anim,eANI_SIT,FALSE);

	if(g_pHero->GetObjectKey()==pPlayer->GetObjectKey())
	{
		g_HeroInput.SetAutoRun(FALSE);
		g_HeroInput.SetAutoAttack(FALSE);
		g_HeroInput.SetAreaSkill(FALSE);
        g_SkillQueue.ClearAll();
	}

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateSit::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
	
	
	//	에니메이션 업데이트
	WzID anim;
	pPlayer->ProcessInterpolation(dwTick);
	float fRet = pPlayer->ProcessAnimation(dwTick);

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    if (pPlayer->IsInSpaTile_Change_Sit())
    {
        return ProcessSpaSitAnimation(pPlayer);
    }
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

	if(pPlayer->IsStandUp())
	{
		anim = pPlayer->GetCurrentStandUpAnimation();
		pPlayer->SetPlayerAnimation(anim,eANI_GETUP,FALSE);	
		pPlayer->SetStandUp(FALSE);
	}
	else if(pPlayer->GetCurrentAnimation()!=pPlayer->GetCurrentStandUpAnimation())
	{
		if (fRet >= 100.0f)
		{
			pPlayer->SetSitState( TRUE );
		}

		if( pPlayer->GetSitState() )
		{
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
  #ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
            if (pPlayer->IsInSpaTile_Change_SitIdle())
  #else
            if (pPlayer->IsInSpaTile())
  #endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
            {
                // 온천 앉기 애니메이션
                pPlayer->SetSpaSittingAnimation(dwTick);
            }
            else
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
            {
                anim = pPlayer->GetCurrentSittingAnimation();
                pPlayer->SetPlayerAnimation(anim,eANI_SIT_IDLE,TRUE);
            }
		}
	}
	else
	{
		if (fRet >= 100.0f)
			return STATEPROCESS::END;
	}

	// 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
	if (pPlayer->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}
	
	// 다음 동작이 있으면 바로 수행해주세요~
	if (pPlayer->GetActionQueueCount())
	{
		pPlayer->ProcessNextAction();
		return STATEPROCESS::SUSPEND;
	}


	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateSit::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	pPlayer->SetSitState( FALSE );
	pPlayer->SetStandUp(FALSE);


	return STATECHANGE::SUCCESS;
}

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
STATEPROCESS::TYPE StateSit::ProcessSpaSitAnimation(Player* pPlayer)
{
    if (pPlayer->IsInSpaTile_Change_Sit())
    {
        SpaZoneInfo* spa_info = SpaZoneInfoParser::Instance()->GetSpaZoneInfo(g_pMap->GetMapID());
        if (spa_info && (spa_info->special_animation[0].id != 0))
        {
            pPlayer->SetPlayerAnimation(spa_info->special_animation[0].id,eANI_SIT,TRUE);
        }
    }

    if(pPlayer->IsStandUp())
    {
        pPlayer->SetStandUp(FALSE);
        return STATEPROCESS::END;
    }

    // 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
    if (pPlayer->CommonStateProcess())
    {
        return STATEPROCESS::SUSPEND;
    }

    // 다음 동작이 있으면 바로 수행해주세요~
    if (pPlayer->GetActionQueueCount())
    {
        pPlayer->ProcessNextAction();
        return STATEPROCESS::SUSPEND;
    }


    return STATEPROCESS::SUSPEND;
}
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION