//------------------------------------------------------------------------------
//  SkillStateFireArrow.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFireArrow.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"


BOOL SkillStateFireArrow::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		ReleaseCastingEffect();

		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();


        if( pCharacter != NULL && pTarget != NULL )
        {
            BASE_SKILLINFO* pInfo = GetSkillInfo();

            float range = ((float)pInfo->m_wSkillRange / 10.0f);
            WzVector wzVec1 = pTarget->GetVisiblePos() - pCharacter->GetVisiblePos();
            float fLength = VectorLength(&wzVec1);
            DWORD MoveTime = max(100, (DWORD)(700.0f * (float)(fLength/range)) );

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( MoveTime );

            pProjectile->CreateEffect( StrToWzID("E010"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		return FALSE;
	}

	return TRUE;
}
