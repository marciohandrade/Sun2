//------------------------------------------------------------------------------
//  SkillStatePoisonDagger.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStatePoisonDagger.h"
#include "ProjectileManagerEx.h"
#include "Character.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "Camera.h"
#include "SkillInfoParser.h"



class Projectile;
SkillStatePoisonDagger::SkillStatePoisonDagger()
{
	m_bAniStop = FALSE;
}

SkillStatePoisonDagger::~SkillStatePoisonDagger(void)
{
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStatePoisonDagger::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	if(m_bAniStop)
	{
		////�Ȼ�ü�� ���ƿ����������� �ִϸ��̼� �����Ѵ�.
		//Projectile*  pProject = g_ProjectileManager.GetProjectile(m_dwDaggerKey);
		//if(!pProject)
		{
			m_bAniStop = FALSE;
			EnterProcess(pOwner,CurTime,dwTick);
		}
	}
	else
	{
		EnterProcess(pOwner,CurTime,dwTick);
	}
	
	Character*	pChr = (Character*)pOwner;
	CheckEvent();

	//ù��°�̺�Ʈ�� �߻�ü�� ������.
	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;
		

        BOOL IsMyHero = pChr->GetObjectKey() == g_pHero->GetObjectKey();

        //������ü�� ���ΰ��϶��� �����ش�.
        if ( IsMyHero == TRUE )
        {
            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
            g_Camera.StartVibration(100,0.07f);
        }


		if( Character* pTarget = (Character *)pChr->GetTargetObject() )
		{

            SkillScriptInfo* pInfo = GetSkillInfo();

            WzVector StartPosition = pChr->GetWeaponBonePos(0);
            WzVector TargetPosition = pTarget->GetArrowTargetPos();


            WzVector vDistance = TargetPosition - StartPosition;
            float fRange = VectorLength(&vDistance);


            float fSkillRange = ((float)pInfo->m_wSkillRange / 10.0f);
            DWORD LifeTime1 = (DWORD)(160.0f * fRange/fSkillRange);
            DWORD LifeTime2 = max(900,(DWORD)(150.0f * (14.f+fRange)/fSkillRange));



            WzVector vCharDir = pChr->GetDirection();

            //ù��° ������
            WzVector vCtrlDir1;
            float fAngle1 = n_deg2rad(33);
            vCtrlDir1.x = vCharDir.x * cos(fAngle1) + vCharDir.y * sin(fAngle1);
            vCtrlDir1.y = vCharDir.x * -sin(fAngle1) + vCharDir.y * cos(fAngle1);
            vCtrlDir1.z = 0.0f;
            WzVector ControlPoint1 = (StartPosition + vCtrlDir1 * (fRange/2) );

            //�ι�° ������
            WzVector vCtrlDir2;
            float fAngle2 = n_deg2rad(-33);
            vCtrlDir2.x = vCharDir.x * cos(fAngle2) + vCharDir.y * sin(fAngle2);
            vCtrlDir2.y = vCharDir.x * -sin(fAngle2) + vCharDir.y * cos(fAngle2);
            vCtrlDir2.z = 0.0f;
            WzVector ControlPoint2 = ( StartPosition + vCtrlDir2 * 14.f );


            {
                cProjectileBezierUnit* pProjectile = new cProjectileBezierUnit();
                pProjectile->SetBezierCourseSingleControl( StartPosition, TargetPosition, ControlPoint1 );
                pProjectile->CreateEffect( StrToWzID("K845"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
                pProjectile->SetEffectScale( 0.5f );
                pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), StrToWzID("K849"), FALSE, 0, FALSE );
                if( IsMyHero == TRUE )
                {
                    pProjectile->SetCameraShakeHit( TRUE );
                    pProjectile->SetCameraShakeType( Camera::VIBE_TYPE_VERTICAL, 100.0f, 0.07f );
                }
                pProjectile->SetLifeTime( LifeTime1 );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }
            {
                cProjectileBezierUnit* pProjectile = new cProjectileBezierUnit();
                pProjectile->SetBezierCourseSingleControl( TargetPosition, StartPosition, ControlPoint2 );
                pProjectile->CreateEffect( StrToWzID("K845"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
                pProjectile->SetEffectScale( 0.5f );
                pProjectile->SetHitData( pChr->GetObjectKey(), 0, StrToWzID("K848"), FALSE, 0, FALSE );
                //pProjectile->SetCameraShakeHit( TRUE );
                //pProjectile->SetCameraShakeType( Camera::VIBE_TYPE_VERTICAL, 100.0f, 0.07f );
                pProjectile->SetLifeTime( LifeTime2 );
                pProjectile->SetActiveDelayTime( LifeTime1 );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );

            }
		}
	}

	//�ι�° Ÿ������ �ִϸ� �����.
	if(CheckNotProcessedHitEvent(2))
	{
		++m_iHitProcessed;
		m_bAniStop = TRUE;
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}





















