//------------------------------------------------------------------------------
//  SkillStatePoisonThorn.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStatePoisonThorn.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Map.h"


BOOL SkillStatePoisonThorn::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();

		WzVector StartPosition = pCharacter->GetVisiblePos();
		WzVector TargetPosition;
		if( pTarget )
		{
			TargetPosition = pTarget->GetVisiblePos();
		}
		else
		{
			TargetPosition=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		}

        WzVector Rotate = c_ZeroVector;
        Rotate.z = pCharacter->GetAngle();

		WzVector wzVec1 = TargetPosition - StartPosition;
		float fLength = VectorLength(&wzVec1);

		DWORD ActiveDelay = 0;
		for(float i=0;i<=fLength+10.0f;i+=2.2f)
		{
			float fRate;
			fRate= (float)i / (float) fLength;

			if(i>=fLength)
			{
				i=fLength;
			}

			WzVector vNewPos = StartPosition + pCharacter->GetDirection()*i;
			WzVector vResult;

            vNewPos.z += 1.0f;
			g_pMap->GetTerrainHeight(&vNewPos,&vResult);
			vNewPos = vResult;
			float fDelay=80.0f*i;

			ActiveDelay = (DWORD)fDelay;

            if( i < fLength )
            {

                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                pProjectile->SetTargetPosition( vNewPos );
                pProjectile->SetUnitCode( eProjectile_SRC_THORN );
                pProjectile->SetUnitRotate( Rotate );
                pProjectile->SetUnitAnimation( StrToWzID("N001"), FALSE );
                pProjectile->SetLifeTime( 3000 );
                pProjectile->SetActiveDelayTime( ActiveDelay );
                pProjectile->SetUnitFadeOut( TRUE );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }
            else
            {
                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                pProjectile->SetTargetPosition( vNewPos );
                pProjectile->CreateEffect( StrToWzID("k460"), pCharacter->GetObjectKey(), pTarget ? pTarget->GetObjectKey() : 0);
                pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, TRUE );
                pProjectile->SetLifeTime( 1666 );
                pProjectile->SetActiveDelayTime( ActiveDelay + 300 );
                pProjectile->SetEffectFadeOut( TRUE );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );

                break;
           }
		}
	}

	return FALSE;
}