//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFrozenArrow.h"
#include "Character.h"
#include "ProjectileManagerEx.h"



BOOL SkillStateFrozenArrow::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	Character* pCharacter = (Character*)pOwner;
	Character* pTarget = (Character *)pCharacter->GetTargetObject();

	if( nEventIndex == 1 )
	{
        if( pCharacter != NULL && pTarget != NULL )
        {
            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( 166 );

            pProjectile->CreateEffect( StrToWzID("E079"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetEffectScale( 0.7f );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        if( pCharacter != NULL && pTarget != NULL )
        {
            //param.bAllDamage =TRUE;


            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(1) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( 166 );

            pProjectile->CreateEffect( StrToWzID("E079"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetEffectScale( 0.7f );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

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
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();		
		//param.effectID = StrToWzID("E079");
		//param.vScale.x=0.7f;
		//param.vScale.y=0.7f;
		//param.vScale.z=0.7f;

		//param.bAddNextSerial=TRUE;//화살이 소멸될때 증가 한다.

		//g_ProjectileManager.CreateProjectile(param);



		//
		

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.03f;

		//
		//param.vStartPos += pCharacter->GetArrowBonePos(1);

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
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();		
		//param.effectID = StrToWzID("E079");


		//param.bAddNextSerial=TRUE;//화살이 소멸될때 증가 한다.
		//param.bAllDamage =TRUE;

		//g_ProjectileManager.CreateProjectile(param);

		return FALSE;
	}

	return TRUE;
}