//------------------------------------------------------------------------------
//  StateDamage.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statedamage.h"
#include "Character.h"
#include "NumericValues.h"
#include "Hero.h"
#include "GlobalFunc.h"
#define INT_DUST_TIME 100


//------------------------------------------------------------------------------
/**
*/
StateDamage::StateDamage(void)
{
}


//------------------------------------------------------------------------------
/**
*/
StateDamage::~StateDamage(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateDamage::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateDamage::OnStart(Object* pOwner, DWORD CurTime)
{
	//Character*	pChr = (Character*)pOwner;

	//assert(pChr->GetCurrentAction()->ActionID == ACTION_KNOCKBACK);

	//// 애니메이션 결정을 가장먼저해준다.

	//// 널부러져 죽기
	//if (pChr->IsKindOfObject(MONSTER_OBJECT))
	//{
	//	if (pChr->IsDead())
	//	{
	//		pChr->SetAnimation( "D002" , FALSE);
	//	}
	//	else
	//	{
	//		pChr->SetAnimation( "H0L2" , FALSE);	
	//	}
	//}
	//else
	//{
	//	if(pChr->IsKindOfObject(PLAYER_OBJECT))
	//	{
	//		Player* pPlayer = (Player*)pChr;
	//		char cWC = pPlayer->GetWeaponAnimationCode();
	//		char szAniCode[5] = {0,};
	//		sprintf(szAniCode, "HS%c6", cWC);
	//		pChr->SetAnimation(szAniCode, FALSE);
	//	}
	//	else
	//	{
	//		pChr->SetAnimation( "H0L2" , FALSE);
	//	}
	//}

	////pChr->Move(
 ////       pChr->GetCurrentAction()->KNOCKBACK.vCurPos.wvPos,
 ////       pChr->GetCurrentAction()->KNOCKBACK.vDestPos.wvPos,
 ////       CMS_WALK,
 ////       TRUE, 
 ////       pChr->GetCurrentAction()->KNOCKBACK.vCurPos.wTile,
 ////       pChr->GetCurrentAction()->KNOCKBACK.vDestPos.wTile );

	//m_dwDustTime=INT_DUST_TIME;

	//WzVector vDir=pChr->GetCurrentAction()->KNOCKBACK.vCurPos.wvPos;

	//pChr->SetDirection(vDir);

	//pChr->m_AirInfo.fGravity = -0.00098f;
	//pChr->m_AirInfo.fUpperForce = 0.0f;

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateDamage::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	float fMoveSpeed;

	if (pChr->IsDead())//죽을땐 좀더 빠르게
	{
		fMoveSpeed = GetBaseMoveSpeedAsState(CMS_KNOCKBACK)*2.0f * (float)dwTick;
	}
	else
	{
		fMoveSpeed = GetBaseMoveSpeedAsState(CMS_KNOCKBACK) * (float)dwTick;
	}
	
	SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));

	g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed, NULL);

	SUNPROCESSPROFILE_END(_T("PathFinding"));

	pChr->ProcessInterpolation(dwTick);
	float fRet = pChr->ProcessAnimation(dwTick);

	if (fRet >= 100.0f)
	{
		pChr->SetIdleAnimation();
	}

	if (!pChr->GetPathExplorer()->IsMoving()  || pChr->GetActionQueueCount() > 0)
	{
		if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
		{
			pChr->SetNextState(STATE::IDLE,g_CurTime);
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			if (pChr->IsKindOfObject(MONSTER_OBJECT))
			{
				if (pChr->IsDead())
				{
					pChr->SetNextState(STATE::DEATH,g_CurTime);
					return STATEPROCESS::SUSPEND;		
				}
			}

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
	}
		
	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateDamage::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	return STATECHANGE::SUCCESS;
}
