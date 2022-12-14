//------------------------------------------------------------------------------
//  SkillStateMonsterHealAreaArea.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "stateattack.h"
#include ".\stateheroattack.h"
#include "SkillStateFactory.h"
#include ".\SkillStateMonsterHealArea.h"
#include "../Character.h"
#include "..\soundeffect.h"
//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterHealArea::SkillStateMonsterHealArea(int iLevel)
{
	m_iLevel = iLevel;
}

//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterHealArea::~SkillStateMonsterHealArea()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateMonsterHealArea::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMonsterHealArea::OnStart(Object* pOwner, DWORD CurTime)
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
STATEPROCESS::TYPE SkillStateMonsterHealArea::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
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
STATECHANGE::TYPE SkillStateMonsterHealArea::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	
	return StateSkill::OnEnd(pOwner,CurTime);
}