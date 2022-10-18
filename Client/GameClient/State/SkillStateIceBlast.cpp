//------------------------------------------------------------------------------
//  SkillStateIceBlast.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateIceBlast.h"
#include "Character.h"
#include "Hero.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "Map.h"


BOOL SkillStateIceBlast::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{

	if( nEventIndex == 1 )
	{

		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();

		//crash 2012/8/16 _ 120302_0809 타겟이 없을 경우의 방어코드
		if(pTarget == NULL)
		{
			return false;
		}

		WzVector pos = pCharacter->GetVisiblePos();

		WzVector dest;
		if(pTarget)
		{
			dest = pTarget->GetVisiblePos();
		}
		else
		{
			dest = pos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		}


		BASE_SKILLINFO *pInfo = GetSkillInfo();
		assert(pInfo);

		float range=((float)pInfo->m_wSkillRange / 10.0f);
		WzVector wzVec1 =dest - pos;
		float fLength = VectorLength(&wzVec1);
		fLength+=1.5f;

		float h=0.0f;
		int count=0;
		
		DWORD dwDelay=0;

		for(float i=0;i<=fLength;i+=1.1f)
		{

            float fRate;
            fRate= (float)i / (float) fLength;


            WzVector vResult;
            WzVector vNewPos = pos + pCharacter->GetDirection()*0.5f+pCharacter->GetDirection()*i;;
            g_pMap->GetTerrainHeight(&vNewPos,&vResult);
            vNewPos=vResult;


            dwDelay=50*(DWORD)i;

            WzVector Scale1, Scale2;

            float fRand=float( rand()%50 );
            fRand/=100.0f;
            Scale1.x=1.0f+fRand;

            fRand=float( rand()%50 );
            fRand/=100.0f;
            Scale1.y=1.0f+fRand;

            fRand=float( rand()%50 );
            fRand/=100.0f;

            Scale1.z=1.0f+fRand+h;
            h+=0.25f;

            float fScale=h/2.0f;

            fScale=min(fScale,1.3f);

            Scale2.x=1.0f+fScale;
            Scale2.y=1.0f+fScale;
            Scale2.z=1.0f+fScale;

            WzVector Rotate;
            Rotate.y = 0.0f;
            Rotate.z = float( rand()%628 );
            Rotate.z /= 100.0f;
            Rotate.x = 0.0f;


            {
                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                pProjectile->SetTargetPosition( vNewPos );
                pProjectile->SetLifeTime( 3000 );
                pProjectile->SetActiveDelayTime( dwDelay );
                pProjectile->CreateEffect( StrToWzID("k400"), pCharacter->GetObjectKey(), pTarget ? pTarget->GetObjectKey() : 0);


                if(i+1.3f>=fLength)
                {
                    pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 10, TRUE );
                }

                pProjectile->SetEffectScale( Scale1 );
                pProjectile->SetEffectRotate( Rotate );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }

            {
                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                pProjectile->SetTargetPosition( vNewPos );
                pProjectile->SetLifeTime( 3000 );
                pProjectile->SetActiveDelayTime( dwDelay );
                pProjectile->CreateEffect( StrToWzID("k401"), pCharacter->GetObjectKey(), pTarget ? pTarget->GetObjectKey() : 0 );


                pProjectile->SetEffectScale( Scale2 );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }

            ++count;
		}

		return FALSE;
	}

	return TRUE;
}