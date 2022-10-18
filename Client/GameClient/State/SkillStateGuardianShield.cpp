#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateGuardianShield.h"

#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"


BOOL SkillStateGuardianShield::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
    if( nEventIndex == 1 )
    {
        Character* pCharacter = (Character*)pOwner;
        Character* pTarget = (Character *)pCharacter->GetTargetObject();


        if( pCharacter != NULL && pTarget != NULL )
        {
            BASE_SKILLINFO* pInfo = GetSkillInfo();

            float range = ((float)pInfo->m_wSkillRange / 10.0f);
            WzVector wzVec1 = pTarget->GetVisiblePos() - pCharacter->GetVisiblePos();
            float fLength = VectorLength(&wzVec1);
            DWORD MoveTime = max(100, (DWORD)(500.0f * (float)(fLength/range)) );



            {   // 발사체
                cProjectileUnit* pProjectile = new cProjectileUnit();
                pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
                pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

                pProjectile->SetLifeTime( MoveTime );

                pProjectile->CreateEffect( StrToWzID("KD76"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );

                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }

            {   // 피격시 이펙트
                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                pProjectile->SetTargetObjectKey( pTarget->GetObjectKey(), TRUE );

                pProjectile->SetLifeTime( MoveTime + 2000 );
                pProjectile->SetActiveDelayTime( MoveTime );

                pProjectile->CreateEffect( StrToWzID("KD77"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );

                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }

        }
    }

    return TRUE;
}
