//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateMachSlash.h"
#include "Character.h"
#include "CloneManager.h"
#include "Hero.h"
#include "Clone.h"
#include "SkillInfoParser.h"

//------------------------------------------------------------------------------
/**
	부모 Onstart 호출안한다.
	전진하며 적을 뚫고 지나간다.
*/
STATECHANGE::TYPE SkillStateMachSlash::OnStart(Object* pOwner, DWORD CurTime)
{
	m_iIllusionCount = 0;

	Character* pCharacter = (Character*)pOwner;

	WzVector StartPosition = pCharacter->GetVisiblePos();
	WzVector TargetPosition;

	if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
	{
		WzVector Direction = pTarget->GetVisiblePos() - StartPosition;
		VectorNormalize(&Direction, &Direction);
		
		TargetPosition = StartPosition + Direction * (GetSkillInfo()->m_wSkillRange * 0.1f);
	}
	else
	{
		TargetPosition = StartPosition;
	}

	pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );


	return StateSkill::OnStart( pOwner, CurTime );

}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateMachSlash::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pCharacter = (Character*)pOwner;

    EnterProcess(pOwner,CurTime,dwTick);

	if (m_iIllusionCount < 1 && GetAnimationRate() >= 0.0f)
	{
		CreateIllusion( pCharacter, 900, 800 );
		m_iIllusionCount++;

	}
	if (m_iIllusionCount < 2 && GetAnimationRate() >= 5.0f)
	{
		CreateIllusion( pCharacter, 1000, 900 );
		m_iIllusionCount++;

	}
	if (m_iIllusionCount < 3 && GetAnimationRate() >= 10.0f)
	{
		CreateIllusion( pCharacter, 1100, 1000 );
		m_iIllusionCount++;

	}
	if (m_iIllusionCount < 4 && GetAnimationRate() >= 15.0f)
	{
		CreateIllusion( pCharacter, 1200, 1100 );
		m_iIllusionCount++;

	}
	if (m_iIllusionCount < 5 && GetAnimationRate() >= 20.0f)
	{
		CreateIllusion( pCharacter, 1300, 1200 );
		m_iIllusionCount++;

	}
	

	// 이벤트체크
	CheckEvent();

	// 무지하게 빨리 지나가면서
	const float SPEED_PER_SECOND = 30.0f * SPEED_MULTIPLIER;	// 대충 초속 30m/s
	ProcessMove(pCharacter,SPEED_PER_SECOND,dwTick);

	// 친다
	ProcessHit(pCharacter,1);

    return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMachSlash::CreateIllusion( Character *pCharacter, DWORD LiveTime, DWORD DisappearTime )
{
	DWORD dwCloneKey = g_CloneManager.CreateClone(pCharacter);

	if ( Clone *pClone = g_CloneManager.GetClone(dwCloneKey) )
	{
		pClone->SetColor(WzColor_RGBA(255,255,255,255));		
		pClone->SetLifeTime( LiveTime );	
		pClone->SetDisappear( DisappearTime );
	}
}