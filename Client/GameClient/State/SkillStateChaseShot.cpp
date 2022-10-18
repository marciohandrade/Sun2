//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateChaseShot.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "Hero.h"



BOOL SkillStateChaseShot::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 || nEventIndex == 2 || nEventIndex == 3 || nEventIndex == 4)
	{
		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();


		if(pCharacter->GetObjectKey()==g_pHero->GetObjectKey())
		{
			g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
			g_Camera.StartVibration(10,0.05f);
		}
	
        WzVector StartPosition;
        WzVector TargetPosition;

        if( pTarget )
        {
            TargetPosition = pTarget->GetVisiblePos();
		}
		else
		{
			// 타겟이 없으면 앞쪽 땅에다 발사한다.
			WzVector wvDir = pCharacter->GetDirection();
			WzVector wvTargetPos = pCharacter->GetVisiblePos();
			wvDir.z = 0.0f;
			TargetPosition = wvTargetPos + wvDir * 4.0f;
		}

        StartPosition = TargetPosition;
        StartPosition.z += 8.0f;

        int posx = rand() % 4000;
        int posy = rand() % 4000;

        StartPosition.x -= 2.0f;
        StartPosition.y -= 2.0f;
        StartPosition.x += posx * 0.001f;
        StartPosition.y += posy * 0.001f;



        //param.bGroupDamage = TRUE;

        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetPosition( TargetPosition );

        pProjectile->SetLifeTime( 250 );
        pProjectile->SetActiveDelayTime( 700 );

        pProjectile->CreateEffect( StrToWzID("K730"), pCharacter->GetObjectKey(), pTarget ? pTarget->GetObjectKey() : 0 );
        pProjectile->SetUnitCode( eProjectile_SRC_ARROW );
        pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, 150, 0, 0 );

        pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("K731"), TRUE, 0, FALSE );
        pProjectile->SetCameraShakeType( Camera::VIBE_TYPE_HORISONTAL, 10.0f, 0.05f );

        if( nEventIndex == 4 )
        {
            pProjectile->SetAddNextAttackSerial( TRUE );//화살이 소멸될때 증가 한다.
        }

        g_ProjectileManagerEx.InsertProjectile( pProjectile );


		return FALSE;
	}

	return TRUE;
}
