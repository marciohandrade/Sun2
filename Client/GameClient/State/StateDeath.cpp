//------------------------------------------------------------------------------
//  StateDeath.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statedeath.h"
#include "Character.h"
#include "Monster.h"
#include "Player.h"
#include "HeroActionInput.h"


#include "MouseCursor.h"
#include "Hero.h"

//------------------------------------------------------------------------------
/**
*/
StateDeath::StateDeath(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateDeath::~StateDeath(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateDeath::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	// 부활할때는 무조건 IDLE상태로
	switch(pToTransiteState->GetStateKind())
	{
	case STATE::IDLE:
		return TRUE;	

	case STATE::TRANSFORM:
		return TRUE;	

	case STATE::REVIVE:
		return TRUE;

	}
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateDeath::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	WzID animationID = pChr->GetCurrentDeathAnimation();
	assert(animationID);

	pChr->OnDead();
	pChr->ProcessAllRemainActionResult();
	pChr->StopMove();

	// 이미 널부러져 죽는 중이 아니었을때만 새 애니메이션을 세팅한다.
	if (pChr->IsKindOfObject(MONSTER_OBJECT))
	{
		Monster* pMon = (Monster*)pChr;
		if(MONSTER_TYPE_SETTING_MAPOBJECT != pMon->GetMonsterType())
		{
			WzID animID = pChr->GetCurrentAnimation();
			if (animID != StrToWzID("D002"))
			{
				pChr->SetAnimation( animationID );	
                pChr->StartDeathMove();
			}
		}
	}
	else
	{
		pChr->SetAnimation( animationID );	
	}

	if (pChr->IsMyHero())
	{
        g_pHero->SendRiderReleasePacket();                  // 탈 것을 해제한다.
        g_pHero->SendWingsControlByAccessory(false, false); // 날고 있다면 날고 있는 것도 해제한다.

        pChr->SetReserveHP(0);
		g_HeroInput.SetCurrentTarget(0);
		g_HeroInput.SetMousePointHide(FALSE);
		MouseCursor::Instance()->ShowCursor( TRUE );
	}

	if (pChr->IsKindOfObject(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)pChr;
		pPlayer->SetPetEmotion(E_OWNER_DEATH);
	}

	pChr->EmptyActionQueue();

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateDeath::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    if( pChr->ProcessDeathMove( dwTick ) == FALSE )
    {
	    if (pChr->GetPathExplorer()->IsMoving()) 
        {
		    float fMoveSpeed = 15.0f * SPEED_MULTIPLIER * (float)dwTick;
		    SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));

		    g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed, NULL);

		    SUNPROCESSPROFILE_END(_T("PathFinding"));
	    }
    }

	if (pChr == (Character *)g_pHero)
	{
		g_HeroInput.SetCurrentTarget(0);
	}

	pChr->ProcessInterpolation(dwTick,0.9999f);
	pChr->ProcessAnimation( dwTick );


	// 죽은놈이 피가 있으면 살아난다~
	// Todo:문제 될수 있다.
	if (pChr->GetHP() > 0)
	{
		if (pChr->IsKindOfObject(PLAYER_OBJECT))
		{
			pChr->SetNextState(STATE::REVIVE,g_CurTime);
		}
	
		return STATEPROCESS::END;
	}
	
	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateDeath::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	return STATECHANGE::SUCCESS;
}

