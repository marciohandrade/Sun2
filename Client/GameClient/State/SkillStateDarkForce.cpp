//------------------------------------------------------------------------------
//  SkillStateDarkForce.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateDarkForce.h"
#include "ProjectileManagerEx.h"
#include "Character.h"
#include "Hero.h"
#include "Camera.h"
#include "SkillInfoParser.h" 


BOOL SkillStateDarkForce::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;
        SkillScriptInfo* pInfo = GetSkillInfo();

        if( pCharacter != NULL && pInfo != NULL )
        {

            BOOL IsHeroCharacter = g_pHero->GetObjectKey() == pCharacter->GetObjectKey();

            WzVector StartPosition = pCharacter->GetWeaponBonePos(0) + pCharacter->GetDirection() * 1.3f;
            WzVector TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;

            float fSkillRange=((float)pInfo->m_wSkillRange / 10.0f);
            float fRate = VectorLength(&(TargetPosition-StartPosition))/fSkillRange;
            DWORD MoveTime = min(220,max(100,(DWORD)(200.f*fRate)) );

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( MoveTime ); 
            pProjectile->CreateEffect( StrToWzID("K837"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("K838"), IsHeroCharacter, 0, TRUE );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
		
		return FALSE;
	}

	return TRUE;
}



















