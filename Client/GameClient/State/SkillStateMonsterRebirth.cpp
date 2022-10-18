//------------------------------------------------------------------------------
//  SkillStateMonsterRebirth.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "stateattack.h"
#include ".\stateheroattack.h"
#include "SkillStateFactory.h"
#include ".\SkillStateMonsterRebirth.h"
#include "../Character.h"

//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterRebirth::SkillStateMonsterRebirth(int iLevel)
{
	m_iLevel = iLevel;
}

//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterRebirth::~SkillStateMonsterRebirth()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateMonsterRebirth::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMonsterRebirth::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetAnimation("S001",FALSE);

	m_iHealCount = 0;

	Character *pTarget = (Character *)pChr->GetTargetObject();

	if (pTarget)
	{
		pChr->SetDirection(&pTarget->GetPosition());
	}

	assert(pChr->IsKindOfObject(MONSTER_OBJECT));
	
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateMonsterRebirth::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

	pChr->ProcessInterpolation(dwTick);
	float fRet = pChr->ProcessAnimation(dwTick);

	if (!m_iHealCount)
	{
		if (fRet > 50.0f)
		{
			pChr->ProcessAttackResult(pChr->GetCurrentAttackSerial());
			m_iHealCount = 1;
		}
	}
	if (fRet >= 100.0f) {
		if (pChr->ProcessNextAction())
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
STATECHANGE::TYPE SkillStateMonsterRebirth::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	if (!m_iHealCount)
	{
		pChr->ProcessAttackResult(pChr->GetCurrentAttackSerial());		
	}
	return STATECHANGE::SUCCESS;
}