//------------------------------------------------------------------------------
//  StateDamage.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateKnockBack2.h"
#include "Character.h"
#include "NumericValues.h"
#include "Hero.h"
#include "GlobalFunc.h"



//------------------------------------------------------------------------------
/**
*/
StateKnockBack::StateKnockBack(void)
{
}


//------------------------------------------------------------------------------
/**
*/
StateKnockBack::~StateKnockBack(void)
{
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKnockBack::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

	// 널부러져 죽기
	if (pCharacter->IsKindOfObject(MONSTER_OBJECT))
	{
		if (pCharacter->IsDead())
		{
			pCharacter->SetAnimation( "D002" , FALSE);
		}
		else
		{
			pCharacter->SetAnimation( "H0L2" , FALSE);	
		}
	}
	else
	{
		if(pCharacter->IsKindOfObject(PLAYER_OBJECT))
		{
			Player* pPlayer = (Player*)pCharacter;
			char cWC = pPlayer->GetWeaponAnimationCode();
			char szAniCode[5] = {0,};
			sprintf(szAniCode, "HS%c6", cWC);
			pCharacter->SetAnimation(szAniCode, FALSE);
		}
		else
		{
			pCharacter->SetAnimation( "H0L2" , FALSE);
		}
	}

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateKnockBack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pChr = (Character*)pOwner;

	// 이동
	static float s_MoveSpeed = 15.0f;
	float fMoveSpeed = s_MoveSpeed * (float)dwTick * SPEED_MULTIPLIER;

	g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed, NULL);

	// 애니메이션
	pChr->ProcessInterpolation(dwTick);
	pChr->ProcessAnimation(dwTick);

	// 죽었으면 ㅃㅃ~
	if (pChr->GetHP() <= 0)
	{
		pChr->SetNextState(STATE::DEATH,g_CurTime);
		return STATEPROCESS::SUSPEND;
	}

	// 이동이 끝난 후
	if( pChr->IsMoving() == FALSE )
	{
		if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
		{
			pChr->SetNextState(STATE::IDLE,g_CurTime);
			return STATEPROCESS::SUSPEND;
		}
		else
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
	}

	return STATEPROCESS::HOLD;
}

////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
StateKnockBack2::StateKnockBack2(void)
{
}


//------------------------------------------------------------------------------
/**
*/
StateKnockBack2::~StateKnockBack2(void)
{
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKnockBack2::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	// 애니메이션 결정을 가장먼저해준다.
	if (pChr->IsKindOfObject(PLAYER_OBJECT))
	{
		Player * pPlayer = (Player *) pOwner;
		
		pChr->SetAnimation(pPlayer->GetCurrentKnockBack2Animation());
	}
	else
	{
		return STATECHANGE::FAILED;
	}

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateKnockBack2::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	float fMoveSpeed = GetBaseMoveSpeedAsState(CMS_KNOCKBACK2)  * (float)dwTick;

	SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));

	g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed, NULL);

	SUNPROCESSPROFILE_END(_T("PathFinding"));

	pChr->ProcessInterpolation(dwTick);

	float fRet = pChr->ProcessAnimation(dwTick);

	if( fRet >= 100.0f )
	{
		return STATEPROCESS::END;
	}

	return STATEPROCESS::HOLD;
}


//------------------------------------------------------------------------------
/**
*/
StateKnockBack3::StateKnockBack3(void)
{
}


//------------------------------------------------------------------------------
/**
*/
StateKnockBack3::~StateKnockBack3(void)
{
}


//------------------------------------------------------------------------------
/**
*/
BOOL StateKnockBack3::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	if( pToTransiteState->GetStateKind() == STATE::ATTACK )
	{
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKnockBack3::OnStart(Object* pOwner, DWORD CurTime)
{
	m_IsBound = FALSE;

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateKnockBack3::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	// 이동

	pChr->UpdateAir( dwTick );

	static float s_MoveSpeed = 15.0f;
	float fMoveSpeed = s_MoveSpeed * (float)dwTick * SPEED_MULTIPLIER;

	g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed, NULL);
	pChr->ProcessInterpolation(dwTick);

	float fRet = pChr->ProcessAnimation(dwTick);

	if( pChr->GetHeight() == 0.0f )
	{
		// 바닥에 떨어졌네~
		{
			if( m_IsBound == FALSE )
			{
				m_IsBound = TRUE;

				WzVector vUp;
				vUp.x = 0.0f;
				vUp.y = 0.0f;
				vUp.z = 1.0f;
				pChr->Vibrate(vUp,0.05f);			

				// 먼지튀어
				pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);
			}

			// 죽었으면 ㅃㅃ~
			if (pChr->GetHP() <= 0)
			{
				pChr->SetNextState(STATE::DEATH,g_CurTime);
				return STATEPROCESS::SUSPEND;
			}

			if( pChr->IsMoving() == FALSE )
			{
				if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
				{
					pChr->SetNextState(STATE::IDLE,g_CurTime);
					return STATEPROCESS::SUSPEND;
				}
				else
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
			}
		}
	}

	return STATEPROCESS::HOLD;
}
