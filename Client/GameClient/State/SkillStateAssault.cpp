//------------------------------------------------------------------------------
//  SkillStateAssault.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------


#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateAssault.h"
#include "Character.h"
#include "Hero.h"
#include "GlobalFunc.h"


//------------------------------------------------------------------------------
/**
*/
SkillStateAssault::SkillStateAssault()
: m_pBody(NULL)
{
}

//------------------------------------------------------------------------------
/**
	부모 Onstart 호출안한다.
*/
STATECHANGE::TYPE SkillStateAssault::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

	// 이펙트 생성
	HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("H530"), pCharacter->GetVisiblePos(), pCharacter->GetScale(), pCharacter->GetObjectKey());//!loop 이펙트 쓰지 말것    
	m_pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);

	StartSkillAnimation( pCharacter );

	WzVector StartPosition = pCharacter->GetVisiblePos();
	WzVector TargetPosition;

	if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
	{
		TargetPosition = pTarget->GetVisiblePos();

		WzVector vDistance = TargetPosition - StartPosition;
		float fDistance = VectorLength(&vDistance);

		if( fDistance < 1.3f )
		{
			TargetPosition = StartPosition;
		}
		else
		{
			WzVector Direction;
			VectorNormalize(&Direction,&vDistance);

			TargetPosition = TargetPosition + (Direction * -1.2f);
		}
	}

	pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );

	return StateSkill::OnStart( pOwner, CurTime );
}

//------------------------------------------------------------------------------
/**
*/
// 목표점까지 이동해서 한대 때리는 기술이다.
STATEPROCESS::TYPE SkillStateAssault::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	Character*	pCharacter = (Character*)pOwner;

	// 목표점까지 이동
	ProcessMove(pCharacter,8.0f * SPEED_MULTIPLIER,dwTick);

	//// 타격점 체크
	//CheckEvent();
	//
	//// 한대 친다.
	//ProcessHit(pCharacter,1);
	
	if( m_pBody != NULL )
	{
		WzVector vRot;

		vRot.y = 0.0f;
		vRot.z=pCharacter->GetAngle(); 
		vRot.x=pCharacter->GetAngleX();

		WzVector vScale;

		vScale.x = 1.0f;
		vScale.y = 1.0f;
		vScale.z = 1.0f;

		m_pBody->SetResultMatrix(pCharacter->GetVisiblePos(),vRot,vScale);
	}

	return Result;
}
