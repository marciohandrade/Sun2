//------------------------------------------------------------------------------
//  SkillStateWisdom.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateWisdom.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillEffectFunc.h"



BOOL SkillStateWisdom::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{

		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();

		SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D004"), pCharacter->GetWeaponBonePos(0));

        if( pCharacter != NULL && pTarget != NULL )
        {
            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetLifeTime( 1000 );
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
            pProjectile->CreateEffect( StrToWzID("FE53"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		//
        if( pTarget ) 
        {
            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pTarget->GetObjectKey(), TRUE );

            g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("H006"), 1000, Position );
        }
	}

	return TRUE;
}
