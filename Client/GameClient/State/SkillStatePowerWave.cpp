//------------------------------------------------------------------------------
//  SkillStatePowerWave.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStatePowerWave.h"
#include "Character.h"
#include "ProjectileManagerEx.h"


BOOL SkillStatePowerWave::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;

        WzVector TargetPosition = pCharacter->GetVisiblePos();
        TargetPosition.z += 0.5f;

        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetLifeTime( 1000 );
        pProjectile->SetTargetPosition( TargetPosition );
        pProjectile->CreateEffect( StrToWzID("E044"), pCharacter->GetObjectKey(), 0 );
        pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 200, TRUE );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
	}
	return FALSE;
}