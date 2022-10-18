//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateManaDrain.h"
#include "Character.h"
#include "ProjectileManagerEx.h"


BOOL SkillStateManaDrain::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
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

            pProjectile->SetLifeTime( 333 );

            pProjectile->CreateEffect( StrToWzID("E082"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetEffectScale( 0.7f );
            pProjectile->SetHitData( 0, 0, 0, FALSE, 0, FALSE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        if( pCharacter != NULL && pTarget != NULL )
        {
            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(1) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( 333 );

            pProjectile->CreateEffect( StrToWzID("E082"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetEffectScale( 0.7f );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }


		//PROJECTILE_PARAM param;
		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.03f;

		//WzVector dir=pCharacter->GetDirection();

		//param.vStartPos += pCharacter->GetArrowBonePos(0);

		//Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//	param.vTargetPos =param.vStartPos+dir*4.0f;//타겟이 없어도 나가게한다 

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 1000;
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwImageCode=eProjectile_SRC_ARROW;
		//param.dwAttackSerial = 0;
		//param.dwOwner = pCharacter->GetObjectKey();		
		//param.effectID = StrToWzID("E082");
		//param.bAddNextSerial=FALSE;
		//
		//param.vScale.x=0.7f;
		//param.vScale.y=0.7f;
		//param.vScale.z=0.7f;

		//param.bAllDamage=FALSE; //마지막발은 전부 뺀다.

		//g_ProjectileManager.CreateProjectile(param);

		////
		//ZeroMemory(&param,sizeof(param));

		//param.fHeight = 0.03f;

	
		//param.vStartPos += pCharacter->GetArrowBonePos(1);

	
		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();
		//}
		//else
		//	param.vTargetPos =param.vStartPos+dir*4.0f;//타겟이 없어도 나가게한다 

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 1000;
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwImageCode=eProjectile_SRC_ARROW;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();		
		//param.effectID = StrToWzID("E082");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE; //마지막발은 전부 뺀다.

		//param.vScale.x=0.7f;
		//param.vScale.y=0.7f;
		//param.vScale.z=0.7f;

		//g_ProjectileManager.CreateProjectile(param);

		return FALSE;
	}

	return TRUE;

}
