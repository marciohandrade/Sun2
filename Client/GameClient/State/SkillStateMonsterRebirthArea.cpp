//------------------------------------------------------------------------------
//  SkillStateMonsterRebirthArea.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "stateattack.h"
#include ".\stateheroattack.h"
#include "SkillStateFactory.h"
#include ".\SkillStateMonsterRebirthArea.h"
#include "../Character.h"
#include "..\soundeffect.h"

//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterRebirthArea::SkillStateMonsterRebirthArea(int iLevel)
{
	m_iLevel = iLevel;
}

//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterRebirthArea::~SkillStateMonsterRebirthArea()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateMonsterRebirthArea::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMonsterRebirthArea::OnStart(Object* pOwner, DWORD CurTime)
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
	
	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateMonsterRebirthArea::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	Character*	pChr = (Character*)pOwner;

	// 타격점 체크
	CheckEvent();

	if(ProcessHit(pChr,1))
	{
		Character *pTarget = (Character *)pChr->GetTargetObject();
		if (pTarget)
		{
			g_SoundEffect.Play(4,&pTarget->GetPosition());
		}
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMonsterRebirthArea::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	return StateSkill::OnEnd(pOwner,CurTime);
}