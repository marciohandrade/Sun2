//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateCircleSprits.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"

// 에테르 스피릿

BOOL SkillStateCircleSprits::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		ReleaseCastingEffect();

        Character* pCharacter = (Character*)pOwner;

        WzVector StartPosition = pCharacter->GetWeaponBonePos(0);
        WzVector TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;

        if( pCharacter != NULL )
        {
            BASE_SKILLINFO* pInfo = GetSkillInfo();
            float range = ((float)pInfo->m_wSkillRange / 10.0f);
            WzVector wzVec1 = TargetPosition - StartPosition;
            float fLength = VectorLength(&wzVec1);
            DWORD MoveTime = max(100, (DWORD)(700.0f * (float)(fLength/range)) );


            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );

            pProjectile->SetLifeTime( MoveTime );
            pProjectile->CreateEffect( StrToWzID("K747"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("K748"), FALSE, 0, TRUE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		return FALSE;
	}

	return TRUE;
}
