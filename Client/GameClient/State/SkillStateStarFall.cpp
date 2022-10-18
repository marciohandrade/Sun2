//------------------------------------------------------------------------------
//  SkillStateStarFall.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateStarFall.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "map.h"


BOOL SkillStateStarFall::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;

		for(int i=0;i<=15;i++)
		{
            if( pCharacter != NULL  )
            {

                static DWORD s_move_time = 1200;
                static DWORD s_move_alpha = 500;
                static float s_move_height = 13.0f;
                static DWORD s_range = 52;

                WzVector StartPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
                WzVector TargetPosition = pCharacter->GetCurrentAction()->SKILL.vTargePos;
                StartPosition.z += s_move_height;

                float randHeight = float(rand()%10);
                randHeight *= 0.1f;
                StartPosition.z += randHeight;

                float randX = float(rand()%s_range);
                randX *= 0.1f;

                float randY = float(rand()%s_range);
                randY *= 0.1f;

                int dir=rand()%3;
                if(dir==0)
                {
                    randX*=-1.0f;
                }
                else if(dir==1)
                {
                    randY*=-1.0f;

                }
                else
                {
                    randX*=-1.0f;
                    randY*=-1.0f;
                }

                StartPosition.x += randX;
                StartPosition.y += randY;

                TargetPosition.x = StartPosition.x;
                TargetPosition.y = StartPosition.y;

                StartPosition -= pCharacter->GetDirection()*8.0f;

                if(g_pMap)
                {
                    WzVector vVisiblePos = TargetPosition;
                    vVisiblePos.z += 3.0f;

                    float fT = 0.0f;
                    int iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 100.0f,TMGO_NO_COLLISION);

                    //타일로 바꾸자.
                    if ( iMesh >= 0 && fT) //딛을수 있는 곳인가
                    {
                        vVisiblePos.z -= fT;
                        TargetPosition = vVisiblePos;
                    }
                }



                DWORD MoveTime = (s_move_time+rand()%s_move_alpha) / 3;
                DWORD ActiveDelayTime = i*60+rand()%100;



                cProjectileUnit* pProjectile = new cProjectileUnit();
                pProjectile->SetArrowStartPosition( StartPosition );
                pProjectile->SetArrowTargetPosition( TargetPosition );

                pProjectile->SetLifeTime( MoveTime );
                pProjectile->SetActiveDelayTime( ActiveDelayTime );

                pProjectile->CreateEffect( StrToWzID("K340"), pCharacter->GetObjectKey(), 0 );

                pProjectile->SetHitData( pCharacter->GetObjectKey(), i == 0 ? pCharacter->GetCurrentAttackSerial() : 0, StrToWzID("K341"), FALSE, 0, i==15 );

                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }


		}

		return FALSE;
	}

	return TRUE;
}
