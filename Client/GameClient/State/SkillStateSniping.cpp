//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateSniping.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "Hero.h"


BOOL SkillStateSniping::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	Character* pCharacter = (Character*)pOwner;
	Character* pTarget = (Character *)pCharacter->GetTargetObject();

	if( nEventIndex == 1 )
	{
        if( pCharacter->GetObjectKey() == g_pHero->GetObjectKey() )
        {
            g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
            g_Camera.StartVibration(30);
        }


        if( pCharacter != NULL && pTarget != NULL )
        {
            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( 166 );

            pProjectile->CreateEffect( StrToWzID("E078"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );


            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		//if (pCharacter == (Character *)g_pHero)
		//{	
		//	g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
		//	g_Camera.StartVibration(30);
		//}


		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.03f;

		//
		//param.vStartPos += pCharacter->GetArrowBonePos(0);

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}


		//param.dwLifeTime = 0;
		//param.dwMoveTime = 500;
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwImageCode=0;
		//param.dwOwner = pCharacter->GetObjectKey();		
		//param.effectID = StrToWzID("E078");

		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.bAddNextSerial=TRUE;//화살이 소멸될때 증가 한다.
		//param.bAllDamage = TRUE;

		//g_ProjectileManager.CreateProjectile(param);

		return FALSE;
	}

	return TRUE;
}
