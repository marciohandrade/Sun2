//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateChargeAttack.h"
#include "Character.h"
#include "Hero.h"
#include "Clone.h"
#include "CloneManager.h"
#include "GameFunc.h"


//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateChargeAttack::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;
	Character* pTarget = (Character*)pCharacter->GetTargetObject();
	if( pTarget == NULL )
		return STATECHANGE::FAILED;

	// 사용할 변수 초기화
	m_dwAttackTick = 0;
	m_iCloneCreateCount = 0;
	m_bReadyAttack = false;
	m_bBackToTheStartPos = false;
	ZeroMemory(m_sChargeAttackClone, sizeof(m_sChargeAttackClone));

	// 목표위치를 구한다

	WzVector StartPosition = pCharacter->GetVisiblePos();
	WzVector TargetPosition = StartPosition;

	if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
	{
		WzVector Direction = pTarget->GetVisiblePos() - StartPosition;
		VectorNormalize(&Direction, &Direction);

		TargetPosition = StartPosition + Direction * 3.0f;
	}

	// StartSkillMove
	pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );


	m_bReadyAttack = false;
	m_bReadyMove = false;

	return StateSkill::OnStart(pOwner, CurTime );
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateChargeAttack::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 2 )
	{
		m_bReadyAttack = true;
	}
	else if( nEventIndex == 1 )
	{
		m_bReadyMove = true;
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateChargeAttack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	Character* pCharacter = (Character*)pOwner;

	if( m_bReadyAttack == true )
	{
		ProcessMove(pCharacter, 30.0f * SPEED_MULTIPLIER, dwTick);
	}

	m_dwAttackTick += dwTick;

	if (m_bReadyAttack == true && m_dwAttackTick > 30 )
	{

		m_dwAttackTick = 0;

		m_sChargeAttackClone[m_iCloneCreateCount].dwCloneID = g_CloneManager.CreateClone(pCharacter);
		if( Clone* pClone = g_CloneManager.GetClone(m_sChargeAttackClone[m_iCloneCreateCount].dwCloneID) )
		{
			pClone->SetLifeTime(99999);
			pClone->SetColor(WzColor_RGBA(255,110,110,220));
			pClone->SetAlphaBlend(AB_LIGHTING);
			pClone->SetPlayAni(TRUE);
			pClone->SetMoveSpeed(100.0f * SPEED_MULTIPLIER);

			float fNewAngle = pCharacter->GetAngle() + 0.6f - (0.3f*m_iCloneCreateCount) ;

			WzVector vDirection;
			ClampAngle(&fNewAngle);
			vDirection.x = sin(fNewAngle);
			vDirection.y = -cos(fNewAngle);
			vDirection.z = 0.0f;
			
			WzVector wvDestPos;

			if( m_iCloneCreateCount == 0 )
			{
				pClone->SetAnimation(StrToWzID("S006"));
				wvDestPos = pCharacter->GetVisiblePos() + (vDirection * 6);
			}			
			else if( m_iCloneCreateCount==1 )
			{
				pClone->SetAnimation(StrToWzID("S007"));
				wvDestPos = pCharacter->GetVisiblePos() + (vDirection * 7);
			}
			else if( m_iCloneCreateCount==2) 
			{
				pClone->SetAnimation(StrToWzID("S008"));
				wvDestPos = pCharacter->GetVisiblePos() + (vDirection * 8);
			}
			else if( m_iCloneCreateCount==3)
			{
				pClone->SetAnimation(StrToWzID("S009"));
				wvDestPos = pCharacter->GetVisiblePos() + (vDirection * 7);
			}
			else if( m_iCloneCreateCount==4)
			{
				pClone->SetAnimation(StrToWzID("S010"));
				wvDestPos = pCharacter->GetVisiblePos() + (vDirection * 6);
				m_bReadyAttack =false;
			}

			m_sChargeAttackClone[m_iCloneCreateCount].wvDestPos = wvDestPos;
			m_iCloneCreateCount++;
			if (MAX_CHARGE_ATTACK_CLONE <= m_iCloneCreateCount)
			{
				m_iCloneCreateCount = 0;
			}
			pClone->Move(pCharacter->GetVisiblePos(), wvDestPos );
		}
	}

	if ((m_bBackToTheStartPos == false) && (pCharacter->GetCurrentFrame() > 80.0f) && (m_dwAttackTick > 30))
	{
		m_dwAttackTick = 0;
	
		if( Clone* pClone = g_CloneManager.GetClone(m_sChargeAttackClone[m_iCloneCreateCount].dwCloneID) )
		{
			pClone->Move(m_sChargeAttackClone[m_iCloneCreateCount].wvDestPos, pCharacter->GetVisiblePos());
			pClone->SetMoveSpeed(120.0f * SPEED_MULTIPLIER);
		}
		if (m_iCloneCreateCount >= (MAX_CHARGE_ATTACK_CLONE-1))    
		{
			m_bBackToTheStartPos = true;
		}
		else
		{
			m_iCloneCreateCount++;
		}
	}

	return Result;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateChargeAttack::OnEnd(Object* pOwner, DWORD CurTime)
{
	for( int i =0; i< MAX_CHARGE_ATTACK_CLONE; i++)
	{
		if( Clone *pClone = g_CloneManager.GetClone(m_sChargeAttackClone[i].dwCloneID) )
		{
			pClone->SetDisappear(300);
			pClone->ResetCurDelay();
			pClone->SetLifeTime(400);
		}
	}

	return STATECHANGE::SUCCESS;
}
