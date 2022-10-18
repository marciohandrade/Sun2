#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateMysticDance.h"

#include "Character.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "SkillInfoParser.h"
#include "PublicMath.h"
#include "ProjectileManagerEx.h"


//------------------------------------------------------------------------------
/**
*/
SkillStateMysticDance::SkillStateMysticDance()
{
	m_StartTime = 0;
}

//------------------------------------------------------------------------------
/**
	부모 Onstart 호출안한다.
*/
STATECHANGE::TYPE SkillStateMysticDance::OnStart(Object* pOwner, DWORD CurTime)
{
	return StateSkill::OnStart( pOwner, CurTime );
}

//------------------------------------------------------------------------------
/**
*/
// 목표점까지 이동해서 한대 때리는 기술이다.
STATEPROCESS::TYPE SkillStateMysticDance::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );
	if( Result != STATEPROCESS::HOLD )
	{
		Character* pCharacter = (Character*)pOwner;

		if( m_StartTime == 0 )
		{
			m_StartTime = GetCurrentTime();
			pCharacter->SetHideSkill(TRUE);

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetTargetPosition( pCharacter->GetBodyPos() );
			pProjectile->CreateEffect( StrToWzID("KB22"), pOwner->GetObjectKey(), 0 );
			pProjectile->SetLifeTime( 2000 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}

		if( GetCurrentTime() < m_StartTime + 2000 )
		{
			return STATEPROCESS::HOLD;
		}

		//pCharacter->SetHideSkill(FALSE);

		//cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
		//pProjectile->SetTargetPosition( pCharacter->GetBodyPos() );
		//pProjectile->CreateEffect( StrToWzID("KB22"), pOwner->GetObjectKey(), 0 );
		//pProjectile->SetLifeTime( 2000 );
		//g_ProjectileManagerEx.InsertProjectile( pProjectile );

	}

	return Result;
}

void SkillStateMysticDance::Finalize(Object* pOwner)
{
	StateSkill::Finalize(pOwner);

	if( Character* pCharacter = (Character*)pOwner )
	{
		pCharacter->SetHideSkill(FALSE);

		cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
		pProjectile->SetTargetPosition( pCharacter->GetBodyPos() );
		pProjectile->CreateEffect( StrToWzID("KB22"), pOwner->GetObjectKey(), 0 );
		pProjectile->SetLifeTime( 2000 );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );
	}

}
