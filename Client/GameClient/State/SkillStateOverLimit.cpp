//------------------------------------------------------------------------------
//  SkillStateOverLimit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateOverLimit.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "SkillInfoParser.h"
#include "NumericValues.h"
#include "Hero.h"
#include "Character.h"
#include "GlobalFunc.h"
#include "SkillEffectFunc.h"



SkillStateOverLimit::SkillStateOverLimit()
: m_bMove(FALSE)
, m_dwCurTime(0)
{
}

BOOL SkillStateOverLimit::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	Character* pCharacter = (Character*)pOwner;
	Character* pTarget = (Character *)pCharacter->GetTargetObject();

	if( nEventIndex == 1 )
	{

        if(pCharacter->GetObjectKey()==g_pHero->GetObjectKey())
        {
            g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
            g_Camera.StartVibration(10,0.05f);	
        }

        if( pCharacter != NULL && pTarget != NULL )
        {
            BASE_SKILLINFO* pInfo = GetSkillInfo();
            float range=((float)pInfo->m_wSkillRange / 10.0f);
            WzVector wzVec1 = pTarget->GetVisiblePos() - pCharacter->GetVisiblePos();
            float fLength = VectorLength(&wzVec1);
            DWORD MoveTime = max(100, (DWORD)(800.0f * (float)(fLength/range)) );

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
            pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

            pProjectile->SetLifeTime( MoveTime );

            pProjectile->CreateEffect( StrToWzID("E081"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }


		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.03f;


		//param.vStartPos += pCharacter->GetArrowBonePos(0);

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}


		//param.dwLifeTime = 0;

  //      DWORD MoveTime;


		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwImageCode=eProjectile_SRC_ARROW;
		//param.dwOwner = pCharacter->GetObjectKey();		
		//param.effectID = StrToWzID("E081");

		//param.bAddNextSerial = TRUE;	//화살이 소멸될때 증가 한다.
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();

		//g_ProjectileManager.CreateProjectile(param);


		WzVector Dest = pCharacter->GetPosition() + pCharacter->GetDirection()*-4.0f;
		pCharacter->Move( pCharacter->GetPosition(), Dest, CMS_RUN, TRUE );
		m_bMove = TRUE;

		return FALSE;
	}

	return TRUE;
}


STATEPROCESS::TYPE SkillStateOverLimit::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	m_dwCurTime += dwTick;

	if (m_bMove == TRUE )
	{
		Character* pCharacter = (Character*)pOwner;

		// 이동
		float fMoveSpeed = GetBaseMoveSpeedAsState(CMS_SWIPE) * (float)dwTick;
		g_pSunTerrain->PathExp_Progress( pCharacter->GetPathExplorer(), fMoveSpeed * 2.0f, NULL);
		
		// 일정 간격으로 발자국
		if( m_dwCurTime > 20 && GetAnimationRate() < 90 )
		{
			m_dwCurTime = 0;
			SkillEffectFunc::CreateFootEffect( pCharacter->GetObjectKey(), pCharacter->GetVisiblePos(), pCharacter->m_dwTerrainAttribute);
		}
	}

	return Result;
}
