#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFactory.h"
#include "Hero.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"
#include "GlobalFunc.h"
#include "skillstatedarkstun.h"
#include "SkillEffectFunc.h"

SkillStateDarkStun::SkillStateDarkStun()
:m_iIllusionCount( 0 )
,m_bStartMove( FALSE )
{
}

SkillStateDarkStun::~SkillStateDarkStun(void)
{
}

STATECHANGE::TYPE SkillStateDarkStun::OnStart(Object* pOwner, DWORD CurTime)
{
	m_iIllusionCount = 0;

	Character* pChr = (Character*)pOwner;

	WzVector StartPosition = pChr->GetVisiblePos();
    WzVector TargetPosition = GetTargetNearPosition( StartPosition, (Character*)pChr->GetTargetObject(), 1.5f );

	pChr->Move( StartPosition, TargetPosition, CMS_RUN, FALSE, -1, -1 );


	return StateSkill::OnStart(pOwner, CurTime );
}

STATEPROCESS::TYPE SkillStateDarkStun::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pCharacter = (Character*)pOwner;

	EnterProcess(pOwner,CurTime,dwTick);

	float fAnimationRate = GetAnimationRate();


	if( fAnimationRate >= 20.0f && FALSE == m_bStartMove)
	{
		// 이동시작
		m_bStartMove = TRUE;
	}
	
	WzVector wvPos, wvDir, wvLen, wvVal;
	wvPos = pCharacter->GetPosition();
	wvLen = pCharacter->GetTarGetPosition() - wvPos;
	wvDir = pCharacter->GetDirection() * 0.5f;
	wvVal = wvPos + wvDir;

	if (0 == m_iIllusionCount && fAnimationRate >= 20.0f)
	{
		
		CreateIllusion( pCharacter, 900, 800 );
		wvVal = wvLen * 0.3f;
		CreateIllusion( pCharacter, 1000, 900 , &wvVal);
		m_iIllusionCount++;
	}
	else if (1 == m_iIllusionCount && fAnimationRate >= 20.0f)
	{
		CreateIllusion( pCharacter, 1100, 1000 );
		wvVal = wvLen * 0.6f;
		CreateIllusion( pCharacter, 1200, 1100 , &wvVal);
		m_iIllusionCount++;
	}
	else if (2 == m_iIllusionCount && fAnimationRate >= 20.0f)
	{
		CreateIllusion( pCharacter, 1300, 1200 );
		m_iIllusionCount++;
	}

	// 이벤트체크
	CheckEvent();

	if( m_bStartMove == TRUE )
	{
		// 무지하게 빨리 지나가면서
		const float SPEED_PER_SECOND = 100.0f * SPEED_MULTIPLIER;
		ProcessMove(pCharacter,SPEED_PER_SECOND,dwTick);
	}

	// 친다
	ProcessHit(pCharacter,1);

	return LeaveProcess(pOwner,CurTime,dwTick);
}

void SkillStateDarkStun::CreateIllusion( Character *pChr, DWORD LiveTime, DWORD DisappearTime, WzVector* pwvPos/* = NULL*/)
{
	DWORD dwCloneKey = g_CloneManager.CreateClone(pChr);

	if (Clone* pClone = g_CloneManager.GetClone(dwCloneKey) )
	{
		if(NULL != pwvPos)
		{
			pClone->SetCurPos(*pwvPos);
			pClone->SetPosition(*pwvPos);
		}

		pClone->SetAlphaBlend(AB_ALPHA);
		pClone->SetAnimation(StrToWzID("S507"));
		pClone->SetColor(WzColor_RGBA(0,0,0,255));		
		pClone->SetLifeTime( LiveTime );	
		pClone->SetDisappear( DisappearTime );
	}
}
