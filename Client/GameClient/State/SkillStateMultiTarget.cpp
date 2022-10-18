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

	// ���� �����鼭 (attachment hide false) �ִϸ��̼��� �����Ѵ�.	
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

	/// Ÿ���� üũ
	CheckEvent();

	// 1�߹߻�!
	if (CheckNotProcessedHitEvent(1))
	{		
		Character *pTarget = (Character *)pChr->GetTargetObject();
	
		//int iNum = 0;
  //      DWORD TargetList[MAX_MULTITARGET] = {0, };
		//pChr->GetAttackTargetList(pChr->GetCurrentAttackSerial(),&iNum, TargetList, _countof(TargetList));

		//if( iNum > 0 )
		//{

		//	// ����Ʈ���
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
		//			// ������ ��ǥ
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
			// �׳� Ÿ�������γ��� ���
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
		//	param.vTargetPos =param.vStartPos+pChr->GetDirection()*4.0f;//Ÿ���� ��� �������Ѵ� 
	
		//param.dwLifeTime = 0;	
		//param.dwMoveTime = 500;//��Ŷ�������� ���� ������ ��¦�� �ʰ� ����
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
		//param.dwAttackSerial = 0;//�ι��� ���� ��縸
		//param.bAddNextSerial=FALSE;

	 //   dwKey = g_ProjectileManager.CreateProjectile(param);

		//pProj =g_ProjectileManager.GetProjectile(dwKey);

		//if(iNum)
		//	pProj->SetMultiTarget(iNum,TargetList);

		// ����Ʈ���
		++m_iHitProcessed;
		
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}

