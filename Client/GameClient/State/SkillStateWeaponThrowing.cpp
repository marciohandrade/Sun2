//------------------------------------------------------------------------------
//  SkillStateWeaponThrowing.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateWeaponThrowing.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "ResourceManager.h"
#include "ProjectileInfoParser.h"



BOOL SkillStateWeaponThrowing::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
        Character* pCharacter = (Character*)pOwner;
        Character* pTarget = (Character *)pCharacter->GetTargetObject();

        if( cProjectileUnit* pProjectile = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, pTarget, GetSkillInfo() ) )
        {
            pProjectile->SetAlphaMode( AB_LIGHTING );
            pProjectile->SetAddNextAttackSerial( TRUE );
            pProjectile->SetTrailHeight( 1.0f );
        }

        return FALSE;

    }

    return TRUE;
}