//------------------------------------------------------------------------------
//  SkillStatePoisonRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStatePoisonRain.h"
#include "Character.h"
#include "Hero.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectFunc.h"

// 락스톰

void CreateLockProjectile( Character* pCharacter, const WzVector& TargetPosition, DWORD ActiveDelay )
{
    if( pCharacter == NULL )
    {
        return;
    }

    BOOL IsMyHero = FALSE;
    if( g_pHero != NULL )
    {
        if( pCharacter->GetObjectKey() == g_pHero->GetObjectKey() )
        {
            IsMyHero = TRUE;
        }
    }

    WzVector Rotate = c_ZeroVector;
    Rotate.z = float( rand()%628 ) / 100.f;

    WzVector StartPosition = TargetPosition;
    StartPosition.z += 10.0f;

    cProjectileUnit* pProjectile = new cProjectileUnit();
    pProjectile->SetArrowStartPosition( StartPosition );
    pProjectile->SetArrowTargetPosition( TargetPosition );
    pProjectile->SetLifeTime( 333 );
    pProjectile->SetActiveDelayTime( ActiveDelay );
    pProjectile->SetRemainTime( 1333 );
    pProjectile->SetUnitCode( eProjectile_SRC_STONE );
    pProjectile->SetUnitAnimation( StrToWzID("N001"), FALSE );
    pProjectile->SetNextUnitAnimation( 333, StrToWzID("N002"), FALSE );
    pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, IsMyHero, 333, FALSE );
    pProjectile->SetUnitRotate( Rotate );
    g_ProjectileManagerEx.InsertProjectile( pProjectile );

}

BOOL SkillStatePoisonRain::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{

	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;

		WzVector pos = pCharacter->GetArrowBonePos(0);

		if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		{
			pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		}

		SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D004"), pos);

        WzVector TargetPosition;

        TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
        CreateLockProjectile( pCharacter, TargetPosition, 0 );

        TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
        TargetPosition.x += 1.0f;
        CreateLockProjectile( pCharacter, TargetPosition, 200 );

        TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
        TargetPosition.y += 1.0f;
        CreateLockProjectile( pCharacter, TargetPosition, 400 );

        TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
        TargetPosition.y -= 1.0f;
        CreateLockProjectile( pCharacter, TargetPosition, 600 );

		//if(GENERALPARAM->IsNet()==FALSE)
		//{
		//	ZeroMemory(&param,sizeof(param));
		//	param.fHeight = 2.0f;

		//	param.vStartPos =pCharacter->GetCurrentAction()->SKILL.vTargePos;		

		//	param.vTargetPos =pCharacter->GetCurrentAction()->SKILL.vTargePos;
		//	param.dwLifeTime =10000;
		//	param.dwMoveTime =10000;

		//	param.fSpeed = 3.0f;
		//	param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//	param.dwAttackSerial = 0;
		//	param.dwOwner = 0;
		//	param.effectID =StrToWzID("I020");
		//	param.bAddNextSerial=FALSE;
		//	param.bAllDamage=TRUE;
		//	param.bFadeOut=FALSE;
		//	param.dwActiveDelayTime=200;
		//	g_ProjectileManager.CreateProjectile(param);
		//}
	}

	return FALSE;
}