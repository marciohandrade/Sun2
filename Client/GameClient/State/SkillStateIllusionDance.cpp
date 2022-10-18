//------------------------------------------------------------------------------
//  SkillStateDoubleAttack.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateIllusionDance.h"
#include "Clone.h"
#include "CloneManager.h"
#include "Character.h"
#include "Hero.h"

const float l_fAnimMuliplier = 1.0f;

//-------------------------------------------------------------------------------
/**
*/
WzVector GetAngleVector(float fAngle)
{
	WzVector vDirection;
	vDirection.x = sin(fAngle);
	vDirection.y = -cos(fAngle);
	vDirection.z = 0.0f;
	return vDirection;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateIllusionDance::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;

	pCharacter->SetAnimation("S404");

	m_iAttackCount = 0;
	m_iDamageProcessCount = 0;
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
#else
	m_dwLastingDelay = 0;
#endif // _NA_007294_20140613_DRAGONKNIGHT_RENEWAL

	
	float fIllusionDistance = 2.0f;

	for (int a = 0; a < MAX_ILLUSION_ATTACK; a++)
	{
		m_arIllusion[a].dwDelay = 0;
		m_arIllusion[a].bStart = FALSE;
		m_arIllusion[a].dwCloneKey = g_CloneManager.CreateClone(pCharacter);

		if( Clone* pClone = g_CloneManager.GetClone(m_arIllusion[a].dwCloneKey) )
		{
			pClone->SetColor(WzColor_RGBA(255,255,255,0));
			pClone->SetLifeTime(999999999);		// 라이프 타임을 이렇게 설정하면, 상태가 끝날때 직접 삭제되도록 해주어야한다.
		}
	}

	Character *pTarget = (Character *)pCharacter->GetTargetObject();
	assert(pTarget);

	WzVector vCenter =pCharacter->GetPosition();
	if(pTarget)	
		vCenter=pTarget->GetPosition();

	WzVector vPos = pCharacter->GetPosition();	
	WzVector vDiff = vPos - vCenter;
	WzVector vPoint[MAX_ILLUSION_ATTACK];

	float fAngle = Math_GetAngleFromVector(&vDiff);

	fAngle += WZ_PI / 5.0f;

	for (int a = 0; a < MAX_ILLUSION_ATTACK; a++)
	{
		vPoint[a] = vCenter + (GetAngleVector(fAngle) * fIllusionDistance);
		fAngle += WZ_PI * 2.0f / 5.0f;
	}

	m_arIllusion[0].vIllusionStart = vPoint[0];
	m_arIllusion[0].vIllusionEnd = vPoint[2];

	m_arIllusion[1].vIllusionStart = vPoint[2];
	m_arIllusion[1].vIllusionEnd = vPoint[4];

	m_arIllusion[2].vIllusionStart = vPoint[4];
	m_arIllusion[2].vIllusionEnd = vPoint[1];

	m_arIllusion[3].vIllusionStart = vPoint[1];
	m_arIllusion[3].vIllusionEnd = vPoint[3];

	m_arIllusion[4].vIllusionStart = vPoint[3];
	m_arIllusion[4].vIllusionEnd = vPoint[0];

    return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
Todo:정리 필요 
*/
STATEPROCESS::TYPE SkillStateIllusionDance::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	pCharacter->SetShow(FALSE);

    EnterProcess(pOwner,CurTime,dwTick);

	if (m_iAttackCount == 0)
	{
		pCharacter->SetAnimation("S404");

		MakeIllusion(m_iAttackCount);		
		m_iAttackCount++;
	}
	else 
	{

		if ( GetAnimationRate() >= 100.0f) 
		{
			switch(m_iAttackCount % 5)
			{
			case 1:
				pCharacter->SetAnimation("S404");
				break;
			case 2:
				pCharacter->SetAnimation("S405");
				break;
			case 3:
				pCharacter->SetAnimation("S406");
				break;
			case 4:
				pCharacter->SetAnimation("S407");
				break;

			}

			if (m_iAttackCount < 5) 
			{
				MakeIllusion(m_iAttackCount);
			}
			m_iAttackCount++;
		}
	}

	ProcessIllusion( pOwner, dwTick );

    return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
    프로세스가 시작할때의 처리를 한다.
    Todo:정리 필요 
*/
VOID SkillStateIllusionDance::EnterProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    Character*	pCharacter = (Character*)pOwner;
    pCharacter->ProcessInterpolation(dwTick);
    
	// 이느낌이 마음에 든다고 한다 --;
	float fRate = pCharacter->ProcessAnimation( 
		 (DWORD)pCharacter->ProcessAnimation( (DWORD)((float)dwTick * l_fAnimMuliplier)) );

	SetAnimationRate( fRate );
}

//------------------------------------------------------------------------------
/**
    프로세스가 끝날때의  처리를 한다.-지완 추가 
    Todo:정리 필요 
*/
STATEPROCESS::TYPE SkillStateIllusionDance::LeaveProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)  
{
    Character*	pCharacter = (Character*)pOwner;

    // 데미지 처리
    if (m_iDamageProcessCount < m_iAttackCount)
    {
        if (pCharacter->ProcessAttackResult(pCharacter->GetCurrentAttackSerial(), FALSE) == TRUE)
        {
            m_iDamageProcessCount++;
        }
    }

    if (m_iAttackCount >= 5)
    {
        if (m_iDamageProcessCount < 5)
        {
            // 미출력된 DamageResult 정리
            pCharacter->ProcessAttackResult(pCharacter->GetCurrentAttackSerial(), TRUE);
        }
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
        return STATEPROCESS::END;
#else
        m_dwLastingDelay += dwTick;
        if (m_dwLastingDelay >= 800)
        {
            return STATEPROCESS::END;
        }
#endif // _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    }
    
    return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateIllusionDance::OnEnd(Object* pOwner, DWORD CurTime)
{	
	Character*	pCharacter = (Character*)pOwner;
  
    //Todo:특화된 처리를 한다.-지완
	pCharacter->SetShow(TRUE);

	for (int a = 0; a < MAX_ILLUSION_ATTACK; a++)
	{
		if ( Clone *pClone = g_CloneManager.GetClone(m_arIllusion[a].dwCloneKey) )
		{
			pClone->SetDisappear(300);
			pClone->ResetCurDelay();
			pClone->SetLifeTime(500);
		}		
	}

    return StateSkill::OnEnd(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateIllusionDance::MakeIllusion(int iIllusionIndex)
{
	if ( Clone *pClone = g_CloneManager.GetClone(m_arIllusion[iIllusionIndex].dwCloneKey) )
	{
		WzVector vDist;
		pClone->Move(m_arIllusion[iIllusionIndex].vIllusionStart,m_arIllusion[iIllusionIndex].vIllusionEnd);
		pClone->SetMoveSpeed(50.0f * SPEED_MULTIPLIER);
		vDist = m_arIllusion[iIllusionIndex].vIllusionEnd - m_arIllusion[iIllusionIndex].vIllusionStart;			
		pClone->SetRotation(Math_GetAngleFromVector( &vDist ));
		m_arIllusion[iIllusionIndex].bStart = TRUE;
		m_arIllusion[iIllusionIndex].dwDelay = 0;

		WzColor color;
		color = WzColor_RGBA(255,255,255,220);
		pClone->SetAlphaBlend(AB_LIGHTING);
		pClone->SetColor(color);
		switch(iIllusionIndex)
		{
		case 0:
			pClone->SetAnimation("S404");
			break;
		case 1:
			pClone->SetAnimation("S405");			
			break;		
		case 2:
			pClone->SetAnimation("S406");			
			break;
		case 3:
			pClone->SetAnimation("S407");			
			break;
		case 4:
			pClone->SetAnimation("S408");			
			break;
		}		
		pClone->ResetCurDelay();
		pClone->SetLifeTime(4000);
		pClone->SetAppear(1000,180);
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateIllusionDance::ProcessIllusion( Object* pOwner, DWORD dwTick )
{
	Character*	pCharacter = (Character*)pOwner;

	for (int a = 0; a < MAX_ILLUSION_ATTACK; a++)
	{
		if (m_arIllusion[a].bStart)
		{
			Clone *pClone = g_CloneManager.GetClone(m_arIllusion[a].dwCloneKey);
			
			if (pClone)
			{				
				pClone->ProcessAnimation( (DWORD)((float)dwTick * l_fAnimMuliplier) );
				pClone->Process(0);
			}
		}
	}
}


