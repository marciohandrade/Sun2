//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateMultiTarget.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "ObjectManager.h"



//------------------------------------------------------------------------------
/**
*/
SkillStateMultiTarget::SkillStateMultiTarget()
{

}

//------------------------------------------------------------------------------
/**
*/
SkillStateMultiTarget::~SkillStateMultiTarget()
{
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMultiTarget::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	// 총을 꺼내면서 (attachment hide false) 애니메이션을 실행한다.	
	pChr->HideAttachment(FALSE);
	
	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateMultiTarget::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    EnterProcess(pOwner,CurTime,dwTick);

	/// 타격점 체크
	CheckEvent();

	// 1발발사!
	if (CheckNotProcessedHitEvent(1))
	{		
		Character *pTarget = (Character *)pChr->GetTargetObject();
	
		//int iNum = 0;
  //      DWORD TargetList[MAX_MULTITARGET] = {0, };
		//pChr->GetAttackTargetList(pChr->GetCurrentAttackSerial(),&iNum, TargetList, _countof(TargetList));

		//if( iNum > 0 )
		//{

		//	// 이펙트출력
		//	++m_iHitProcessed;

		//	WzVector StartPosition = pChr->GetArrowBonePos(0);
		//	WzVector TargetPosition;

		//	for( int i=0;i<iNum;++i)
		//	{
		//		Object* pTargetObject = g_ObjectManager.GetObject( TargetList[i] );
		//		if( pTargetObject->IsKindOfObject( CHARACTER_OBJECT ) == TRUE )
		//		{
		//			Character* pTarget = (Character*)pTargetObject;
		//			TargetPosition = pTarget->GetArrowTargetPos();
		//		}
		//		else
		//		{
		//			// 임의의 좌표
		//			TargetPosition = pTargetObject->GetVisiblePos();
		//			TargetPosition.z += 1.5f;
		//		}

		//		cProjectileUnit* pProjectile = new cProjectileUnit();
		//		pProjectile->SetArrowStartPosition( StartPosition );
		//		pProjectile->SetArrowTargetPosition( TargetPosition );
		//		pProjectile->SetActiveDelayTime( i*200 );
		//		pProjectile->SetLifeTime( 200 );
		//		pProjectile->CreateEffect( StrToWzID("E083"), pChr->GetObjectKey(), TargetList[i] );
		//		pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.8f, 3.0f, 150, 200, 255 );
		//		pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, 0, 0, TRUE );
		//		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		//		StartPosition = TargetPosition;
		//	}
		//}
		//else
		//{
			// 그냥 타겟팅중인넘을 쏘도록
			WzVector StartPosition = pChr->GetArrowBonePos(0);
			WzVector TargetPosition = StartPosition + pChr->GetDirection() * 4.0f;

			if( Character* pTarget = (Character*)pChr->GetTargetObject() )
			{
				TargetPosition = pTarget->GetArrowTargetPos();
			}

			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition );
			pProjectile->SetLifeTime( 200 );
			pProjectile->CreateEffect( StrToWzID("E083"), pChr->GetObjectKey(), 0 );
			//pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 1.0f, 0.8f, 150, 200, 255 );
			pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, 0, 0, TRUE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		//}

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 1.0f;

		//param.vStartPos = pChr->GetArrowBonePos(0);		    

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//	param.vTargetPos =param.vStartPos+pChr->GetDirection()*4.0f;//타겟이 없어도 나가게한다 
	
		//param.dwLifeTime = 0;	
		//param.dwMoveTime = 500;//패킷오기전에 끝나 버려서 살짝만 늦게 쏘자
		//param.bR = 150;
		//param.bG = 200;
		//param.bB = 255;									
		//param.bMakeTrail = false;
		//param.fSpeed = 3.0f;
		//param.fTrailLength = 0.8f;
		//param.dwTrailEffectDelay = 90;
		//param.dwType=PROJECTILEID_ARROW;
		//param.dwImageCode=0;
		//param.effectID = StrToWzID("E083");
		//param.dwAttackSerial = pChr->GetCurrentAttackSerial();
		//param.dwOwner = pChr->GetObjectKey();
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;

		//DWORD dwKey = g_ProjectileManager.CreateProjectile(param);

		//Projectile *pProj =g_ProjectileManager.GetProjectile(dwKey);
		//
		//if(iNum)
		//	pProj->SetMultiTarget(iNum,TargetList);

		//param.vStartPos = pChr->GetArrowBonePos(1);	
		//param.dwAttackSerial = 0;//두번쨰 발은 모양만
		//param.bAddNextSerial=FALSE;

	 //   dwKey = g_ProjectileManager.CreateProjectile(param);

		//pProj =g_ProjectileManager.GetProjectile(dwKey);

		//if(iNum)
		//	pProj->SetMultiTarget(iNum,TargetList);

		// 이펙트출력
		++m_iHitProcessed;
		
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}

