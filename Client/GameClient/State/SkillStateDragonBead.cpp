//------------------------------------------------------------------------------
//  SkillStatePoisonDagger.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateDragonBead.h"
#include "ProjectileInfoParser.h"
#include "SkillInfoParser.h"
#include "ProjectileManagerEx.h"
#include "Character.h"
#include "Hero.h"



BOOL SkillStateDragonBead::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();

        if( cProjectileUnit* pProjectile = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, pTarget, GetSkillInfo() ) )
        {
            pProjectile->SetAddNextAttackSerial( TRUE );

            if( pOwner->GetObjectKey() == g_pHero->GetObjectKey() )
            {
                pProjectile->SetCameraShakeHit( TRUE );
            }
        }

        g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, pTarget, GetSkillInfo() );

        return FALSE;

	}
	return TRUE;
}

