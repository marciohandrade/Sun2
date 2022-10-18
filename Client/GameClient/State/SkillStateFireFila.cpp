//------------------------------------------------------------------------------
//  SkillStateFireFila.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFireFila.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectFunc.h"




BOOL SkillStateFireFila::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		ReleaseCastingEffect();


		Character* pCharacter = (Character*)pOwner;

		WzVector pos = pCharacter->GetArrowBonePos(0);
		if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		{
			pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		}

		SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D001"), pos);

        {
            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetTargetPosition( pCharacter->GetCurrentAction()->SKILL.vTargePos );
            pProjectile->SetLifeTime( 1500 );
            pProjectile->CreateEffect( StrToWzID("I012"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, TRUE, 100, TRUE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		return FALSE;
	}

	return TRUE;
}
