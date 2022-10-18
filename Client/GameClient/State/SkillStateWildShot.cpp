//------------------------------------------------------------------------------
//  SkillStateWildShot.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateWildShot.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "Hero.h"


BOOL SkillStateWildShot::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	Character* pCharacter = (Character*)pOwner;

	if(pCharacter->GetObjectKey()==g_pHero->GetObjectKey())
	{
		g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
		g_Camera.StartVibration(5,0.01f);
	}
	
    {
        WzVector StartPosition = pCharacter->GetWeaponBonePos(0);
        WzVector TargetPosition = StartPosition + pCharacter->GetDirection()*2.0f;
        TargetPosition.z += 4.0f;

        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetPosition( TargetPosition );

        pProjectile->SetLifeTime( 200 );
        pProjectile->CreateEffect( StrToWzID("E077"), pCharacter->GetObjectKey(), 0);
        pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, rand()%155 + 100, 0, 0 );
        pProjectile->SetEffectScale(0.3f);

        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }

	// 석궁인 경우 양손으로 한발 더 발사한다.
	if( pCharacter->GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW )
	{
        WzVector StartPosition = pCharacter->GetWeaponBonePos(1);
        WzVector TargetPosition = StartPosition + pCharacter->GetDirection()*2.0f;
        TargetPosition.z += 4.0f;

        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetPosition( TargetPosition );

        pProjectile->SetLifeTime( 200 );
        pProjectile->CreateEffect( StrToWzID("E077"), pCharacter->GetObjectKey(), 0 );
        pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, rand()%155 + 100, 0, 0 );
        pProjectile->SetEffectScale(0.3f);

        g_ProjectileManagerEx.InsertProjectile( pProjectile );
	}

	if( nEventIndex == 2 )
	{
		// 지면에 떨어지는 이펙트
		WzVector vStart = pCharacter->GetWeaponBonePos(0) + pCharacter->GetDirection() * 2.0f;
        vStart.z += 4.0f;

		WzVector vDest;

        WzVector TargetAreaPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;



		for(int i=10;i>=1;i--)
		{

            //WzVector vDirection;
            //vDirection.x = (float)sin((WZ_PI/11.25f)*(float)i+pCharacter->GetAngle()-WZ_PI/2.25);
            //vDirection.y = (float)-cos((WZ_PI/11.25f)*(float)i+pCharacter->GetAngle()-WZ_PI/2.25);
            //vDirection.z = 0.0f;

            //vDest = pCharacter->GetVisiblePos()+pCharacter->GetDirection()*5.0f;
            //vDest += vDirection*5.0f;
            //vDest -= pCharacter->GetDirection() * 2.0f;

            //param.vTargetPos = vDest;
            //param.vStartPos = vStart + vDirection*2.0f;
            //g_ProjectileManager.CreateProjectile(param);

            WzVector vDirection;
            vDirection.x = (float)sin((WZ_PI/11.25f)*(float)i+pCharacter->GetAngle()-WZ_PI/2.25);
            vDirection.y = (float)-cos((WZ_PI/11.25f)*(float)i+pCharacter->GetAngle()-WZ_PI/2.25);
            vDirection.z = 0.0f;

            static float s_moveback = 7.0f;

            vDest = TargetAreaPosition + pCharacter->GetDirection() * 5.0f;
            vDest += vDirection * 5.0f;
            vDest -= pCharacter->GetDirection() * s_moveback;

            WzVector StartPosition = vStart;
            //WzVector StartPosition = vStart + vDirection*1.0f;
            WzVector TargetPosition = vDest;

            DWORD ActiveDelayTime = 50*(i-1)+500;
            DWORD MoveTime = 250;

            {
                cProjectileUnit* pProjectile = new cProjectileUnit();
                pProjectile->SetArrowStartPosition( StartPosition );
                pProjectile->SetArrowTargetPosition( TargetPosition );

                pProjectile->SetActiveDelayTime(ActiveDelayTime);
                pProjectile->SetLifeTime( MoveTime );
                pProjectile->CreateEffect( StrToWzID("E085"), pCharacter->GetObjectKey(), 0 );
                pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetNextAttackSerial(), 0, FALSE, ActiveDelayTime, FALSE );

                pProjectile->SetRemainTime( 700 );
                pProjectile->SetEffectFadeOut(TRUE);

                if( i == 1 )
                {
                    pProjectile->SetAddNextAttackSerial( TRUE );
                    pProjectile->SetAllDamage( FALSE );
                }
                else
                {
                    pProjectile->SetAddNextAttackSerial( TRUE );
                    pProjectile->SetAllDamage( FALSE );
                }
                
                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }

            {
                g_ProjectileManagerEx.CreateFieldEffect( TargetPosition, StrToWzID("F011"), 1000, 0, 0, 50*(i-1)+500+250 );
            }
		}
	}

	return FALSE;
}