//------------------------------------------------------------------------------
//  SkillStateLightingWave.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateLightingWave.h"
#include "Character.h"
#include "Hero.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectFunc.h"


void CreateLightingEffect( Character* pCharacter, const WzVector& TargetPosition, DWORD ActiveDelay )
{
    {
        //WzVector Position = TargetPosition;

        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetTargetPosition( TargetPosition );
        pProjectile->SetLifeTime(500);
        pProjectile->SetActiveDelayTime( 50 + ActiveDelay );
        pProjectile->CreateEffect( StrToWzID("I030"), 0, 0 );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }

    {
        WzVector Position = TargetPosition;
        Position.z += 2.0f;

        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetTargetPosition( Position );
        pProjectile->SetLifeTime(500);
        pProjectile->SetActiveDelayTime( ActiveDelay );
        pProjectile->CreateEffect( StrToWzID("I031"), pCharacter->GetObjectKey(), 0 );
        //pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }
    {
        WzVector Position = TargetPosition;
        Position.z += 0.5f;

        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetTargetPosition( Position );
        pProjectile->SetLifeTime(500);
        pProjectile->SetActiveDelayTime( ActiveDelay );
        pProjectile->CreateEffect( StrToWzID("F040"), pCharacter->GetObjectKey(), 0 );
        pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }

}


BOOL SkillStateLightingWave::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
        Character* pCharacter = (Character*)pOwner;

        SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D003"), pCharacter->GetWeaponBonePos(0));

        WzVector TargetPosition;


        {
            // 1
            TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
            TargetPosition.x += 0.0f;
            TargetPosition.y += 0.0f;
            CreateLightingEffect( pCharacter, TargetPosition, 0 );

            // 2
            TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
            TargetPosition.x += 1.0f;
            TargetPosition.y += 1.0f;
            CreateLightingEffect( pCharacter, TargetPosition, 300 );

            // 3
            TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
            TargetPosition.x -= 1.0f;
            TargetPosition.y -= 1.0f;
            CreateLightingEffect( pCharacter, TargetPosition, 500 );

            // 4
            TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
            TargetPosition.x += 1.0f;
            TargetPosition.y -= 1.0f;
            CreateLightingEffect( pCharacter, TargetPosition, 700 );

            // 5
            TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
            TargetPosition.x -= 1.0f;
            TargetPosition.y += 1.0f;
            CreateLightingEffect( pCharacter, TargetPosition, 900 );

			// 6
			TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
			TargetPosition.x -= 1.2f;
			TargetPosition.y += 1.2f;
			CreateLightingEffect( pCharacter, TargetPosition, 1100 );

			// 7
			TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
			TargetPosition.x -= 0.0f;
			TargetPosition.y += 0.0f;
			CreateLightingEffect( pCharacter, TargetPosition, 1300 );
        }
	}


	return FALSE;
}

