//------------------------------------------------------------------------------
//  SkillStateFireSpear.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFireSpear.h"
#include "ProjectileManagerEx.h"
#include "Character.h"


BOOL SkillStateFireSpear::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		ReleaseCastingEffect();

		Character* pCharacter = (Character*)pOwner;

        WzVector StartPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos + pCharacter->GetDirection() * -5.2f;
        StartPosition.z +=10.0f;

        WzVector TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
        TargetPosition.z -= 0.5f;

        {
            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );

            pProjectile->SetLifeTime( 333 );
            pProjectile->SetRemainTime( 1500 );
            pProjectile->SetEffectFadeOut( TRUE );

            pProjectile->CreateEffect( StrToWzID("E057"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, TRUE, 333, TRUE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        {
            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( pCharacter->GetCurrentAction()->SKILL.vTargePos );
            pProjectile->SetLifeTime( 3000 );
            pProjectile->SetActiveDelayTime( 350 );
            pProjectile->CreateEffect( StrToWzID("A012"), pCharacter->GetObjectKey(), 0 );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        {
            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( pCharacter->GetCurrentAction()->SKILL.vTargePos );
            pProjectile->SetLifeTime( 200 );
            pProjectile->SetActiveDelayTime( 333 );
            pProjectile->CreateEffect( StrToWzID("F021"), pCharacter->GetObjectKey(), 0 );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		return FALSE;
	}

	return TRUE;
}
