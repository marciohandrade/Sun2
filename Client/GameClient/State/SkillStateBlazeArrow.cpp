//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateBlazeArrow.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "Hero.h"


BOOL SkillStateBlazeArrow::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();

        if( pCharacter != NULL && pTarget != NULL )
        {
            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( 166 ); // 일단 대충
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );

            pProjectile->CreateEffect( StrToWzID("E084"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetUnitCode( eProjectile_SRC_ARROW );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		// 카메라 흔들기
		if( pCharacter->GetObjectKey()==g_pHero->GetObjectKey())
		{
			g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
			g_Camera.StartVibration(10,0.05f);
		}

		return FALSE;
	}

	return TRUE;
}
