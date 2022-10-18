//------------------------------------------------------------------------------
//  SkillStateThunderClap.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "stateattack.h"
#include ".\stateheroattack.h"
#include "SkillStateFactory.h"
#include ".\SkillStateThunderClap.h"
#include "../Character.h"
#include "../SoundEffect.h"

//------------------------------------------------------------------------------
/**
*/
SkillStateThunderClap::SkillStateThunderClap(int iLevel)
{
	m_iLevel = iLevel;
}

//------------------------------------------------------------------------------
/**
*/
SkillStateThunderClap::~SkillStateThunderClap()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateThunderClap::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateThunderClap::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	assert(pChr->IsKindOfObject(PLAYER_OBJECT));
	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateThunderClap::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    Character*	pChr = (Character*)pOwner;

    //Todo:스킬 처리 

    return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateThunderClap::OnEnd(Object* pOwner, DWORD CurTime)
{
    //Todo:특화된 처리를 한다.

	
	return StateSkill::OnEnd(pOwner,CurTime);
}