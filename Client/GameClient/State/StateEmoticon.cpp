

//------------------------------------------------------------------------------
//  StateEmoticon.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "StateEmoticon.h"
#include "HeroActionInput.h"
#include "InterfaceManager.h"
#include "npc.h"
#include "Monster.h"
#include "Hero.h"

//------------------------------------------------------------------------------
/**
*/
StateEmoticon::StateEmoticon(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateEmoticon::~StateEmoticon(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateEmoticon::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateEmoticon::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	WzID anim;

	anim = pPlayer->GetCurrentEmotion();
	if( anim )
	{
		pPlayer->SetAnimation( anim, FALSE );
		pPlayer->GetBasePart()->SetInterpolationFrame(6);
	}

	if(g_pHero && g_pHero->GetObjectKey()==pPlayer->GetObjectKey())
	{
		g_HeroInput.SetAutoRun(FALSE);
		g_HeroInput.SetAutoAttack(FALSE);
		g_HeroInput.SetAreaSkill(FALSE);
        g_SkillQueue.ClearAll();
	}

	if( pPlayer->GetCondition() == eCHAR_CONDITION_SITDOWN )
	{
		pPlayer->SetCondition( eCHAR_CONDITION_STANDUP );
	}

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    if( g_pHero && g_pHero->GetObjectKey()==pPlayer->GetObjectKey() && GAMEOPTION->GetShowCostume())
    {
        MSG_CG_STATUS_RENDER_OPTION_SYN msg;
        msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_COSTUME_OPTION;
        msg.m_byOptionLevel = 0;

        g_pHero->SendPacket(&msg,sizeof(msg));
        g_pHero->SetHideCostumetoAction(true);
    }
#endif


	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateEmoticon::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));


	//	에니메이션 업데이트
	pPlayer->ProcessInterpolation(dwTick);
	float fRet = pPlayer->ProcessAnimation(dwTick);

	BOOL bEnd = FALSE;

	if (fRet >= 100.0f)
	{
		pPlayer->SetEmotion( 0 );
		bEnd = TRUE;
	}

	// 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
	if (pPlayer->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	// 다음 동작이 있으면 바로 수행해주세요~
	if (pPlayer->GetActionQueueCount() || bEnd )
	{

		if (pPlayer->ProcessNextAction())
		{
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			return STATEPROCESS::END;

		}
	}


	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateEmoticon::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    if(g_pHero && g_pHero->GetObjectKey()==pPlayer->GetObjectKey() && GAMEOPTION->GetShowCostume())
    {
        MSG_CG_STATUS_RENDER_OPTION_SYN msg;
        msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_COSTUME_OPTION;
        msg.m_byOptionLevel = 1;

        g_pHero->SendPacket(&msg,sizeof(msg));
        g_pHero->SetHideCostumetoAction(false);
    }
#endif

	return STATECHANGE::SUCCESS;
}
