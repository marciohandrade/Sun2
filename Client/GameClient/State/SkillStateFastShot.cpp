//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFastShot.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "Hero.h"


BOOL SkillStateFastShot::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	Character* pCharacter = (Character*)pOwner;
	Character* pTarget = (Character *)pCharacter->GetTargetObject();


    if( pCharacter->GetObjectKey() == g_pHero->GetObjectKey() )
    {
        g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
        g_Camera.StartVibration(5,0.01f);
    }


    if( pCharacter != NULL && pTarget != NULL )
    {
        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
        pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

        pProjectile->SetLifeTime( 166 );

        pProjectile->CreateEffect( StrToWzID("E077"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
        pProjectile->SetEffectScale( 0.3f );
        pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );

        g_ProjectileManagerEx.InsertProjectile( pProjectile );

        //param.vScale.x=0.3f;
        //param.vScale.y=0.3f;
        //param.vScale.z=0.3f;

    }

	//PROJECTILE_PARAM param;

	//ZeroMemory(&param,sizeof(param));
	//param.fHeight = 0.03f;

	//WzVector dir=pCharacter->GetDirection();

	//WzVector pos = pCharacter->GetArrowBonePos(0);
	//if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
	//{
	//	pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
	//}

	//param.vStartPos = pos;

	//if(pTarget)
	//{
	//	param.vTargetPos = pTarget->GetVisiblePos();
	//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
	//	param.dwTargetID=pTarget->GetObjectKey();

	//}
	//else
	//	param.vTargetPos =param.vStartPos+dir*4.0f;//타겟이 없어도 나가게한다 

	//param.dwLifeTime = 0;
	//param.dwMoveTime = 500;
	//param.fSpeed = 3.0f;
	//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
	//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
	//param.dwOwner = pCharacter->GetObjectKey();		
	//param.effectID = StrToWzID("E077");
	//param.dwImageCode=0;
	//param.vScale.x=0.3f;
	//param.vScale.y=0.3f;
	//param.vScale.z=0.3f;
	//
	//g_ProjectileManager.CreateProjectile(param);

	//if(pCharacter->GetObjectKey()==g_pHero->GetObjectKey())
	//{
	//	g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
	//	g_Camera.StartVibration(5,0.01f);
	//}

	return TRUE;
}
