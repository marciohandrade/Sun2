//------------------------------------------------------------------------------
//  SkillStateIcePressingWall.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateIcePressingWall.h"
#include "Character.h"
#include "ProjectileManagerEx.h"


BOOL SkillStateIcePressingWall::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;

        {
            WzVector TargetPosition = pCharacter->GetVisiblePos();
            TargetPosition.x += 1.2f;
            TargetPosition.y -= 1.2f;

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( 1500 );
            pProjectile->SetActiveDelayTime( rand() % 50 );
            pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 10, FALSE );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        {
            WzVector TargetPosition = pCharacter->GetVisiblePos();
            TargetPosition.x -= 1.2f;
            TargetPosition.y += 1.2f;

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( 1500 );
            pProjectile->SetActiveDelayTime( rand() % 50 );
            pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        {
            WzVector TargetPosition = pCharacter->GetVisiblePos();
            TargetPosition.x -= 1.2f;
            TargetPosition.y -= 1.2f;

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( 1500 );
            pProjectile->SetActiveDelayTime( rand() % 50 );
            pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        {
            WzVector TargetPosition = pCharacter->GetVisiblePos();
            TargetPosition.x += 1.2f;
            TargetPosition.y += 1.2f;

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( 1500 );
            pProjectile->SetActiveDelayTime( rand() % 50 );
            pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		return FALSE;
	}

	return TRUE;
}