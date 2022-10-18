//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "StateBase.h"
#include "SkillState.h"
#include "SkillStatePurge.h"
#include "Character.h"
#include "SkillInfoParser.h"
#include "GlobalFunc.h"


STATECHANGE::TYPE SkillStatePurge::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;
	SetVector(&m_wvTargetPos, 1.0f, 1.0f, 1.0f);
	return StateSkill::OnStart(pOwner,CurTime);
}

STATEPROCESS::TYPE SkillStatePurge::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	// Ÿ�������� �̹� Target������Ʈ�� �����Ƿ� ���������� ������ ������ ������ �־�� �Ѵ�.

	Character* pCharacter = (Character*)pOwner;

	if( pCharacter->GetTargetObject() )
	{
		WzVector wvDistance = pCharacter->GetTargetObject()->GetVisiblePos() - pCharacter->GetVisiblePos();
		float fLength = VectorLength(&wvDistance);
		if( fLength < 10.0f)
		{
			m_wvTargetPos = pCharacter->GetTargetObject()->GetVisiblePos() ;
		}
	}

	return Result;
}


BOOL SkillStatePurge::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex != 1 )
		return TRUE;

	ReleaseCastingEffect();

	GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K762"),m_wvTargetPos);

	return TRUE;
}
