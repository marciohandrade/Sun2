//------------------------------------------------------------------------------
//  SkillStateBeastRebirthArea.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include ".\SkillStateBeastRebirthArea.h"
#include "../Character.h"
#include "..\soundeffect.h"
#include "../Hero.h"
//------------------------------------------------------------------------------
/**
*/
SkillStateBeastRebirthArea::SkillStateBeastRebirthArea(int iLevel)
{
	m_iLevel = iLevel;
}

//------------------------------------------------------------------------------
/**
*/
SkillStateBeastRebirthArea::~SkillStateBeastRebirthArea()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateBeastRebirthArea::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateBeastRebirthArea::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetAnimation("S001",FALSE);

	m_iHealCount = 0;

	WzVector vTarget=pChr->GetCurrentAction()->SKILL.vTargePos;

	pChr->SetDirection(&vTarget);

	g_pEffectManager->CreateEffect(StrToWzID("L003"),pChr->GetVisiblePos());	

	assert(pChr->IsKindOfObject(MONSTER_OBJECT));
	
	g_pEffectManager->CreateEffect(StrToWzID("A023"),g_pHero->GetVisiblePos());

	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateBeastRebirthArea::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	Character*	pChr = (Character*)pOwner;

	// 타격점 체크
	CheckEvent();

	if (CheckNotProcessedHitEvent(1))  
	{
		m_iHitProcessed++;

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
STATECHANGE::TYPE SkillStateBeastRebirthArea::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	return StateSkill::OnEnd(pOwner,CurTime);
}