//------------------------------------------------------------------------------
//  StateSpawn.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statespawn.h"
#include "character.h"
#include "monster.h"
#include <NPCInfoParser.h>
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
/**
*/
StateSpawn::StateSpawn(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateSpawn::~StateSpawn(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateSpawn::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateSpawn::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	//IDLE 에니메이션 설정
	if(pChr->IsKindOfObject(MONSTER_OBJECT))
	{
		Monster *pMon=(Monster *) pChr;
		DWORD monsterCode=pMon->GetMonsterInfo()->m_MonsterCode;;

		pMon->CreateEffect();
		BASE_NPCINFO *pInfo=NPCInfoParser::Instance()->GetNPCInfo( monsterCode );
		
		if(pInfo)
		{
			pInfo->m_dwSpawnTime;
			pChr->SetAnimation(pInfo->m_wzidSpawnAniCode.id, FALSE );
		}
		else
			pChr->SetAnimation( "N999", FALSE );

	}
	else
		pChr->SetAnimation( "N999", FALSE );


	GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A029"),pChr->GetVisiblePos(),pChr->GetObjectKey());



	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateSpawn::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;	
	pChr->ProcessInterpolation( dwTick );
	float fRet = pChr->ProcessAnimation( dwTick );

	if( fRet >= 100.0f )
	{
		WzID idanim = pChr->GetCurrentIdleAnimation();
		assert(idanim);
		pChr->SetAnimation( idanim , TRUE);
	}
	
	
	if( fRet >= 100.0f ||pChr->GetActionQueueCount() > 0)
	{		
		if (pChr->ProcessNextAction())
		{		
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			pChr->SetNextState(STATE::IDLE,g_CurTime);
			return STATEPROCESS::SUSPEND;
		}
	}
	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateSpawn::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	return STATECHANGE::SUCCESS;
}
