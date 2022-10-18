//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateBloodRain.h"
#include "Character.h"
#include "Hero.h"
#include "GameFunc.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"




//------------------------------------------------------------------------------
/**
*/
static WzVector GetAngleVector(float fAngle)
{
	WzVector vDirection;
	vDirection.x = sin(fAngle);
	vDirection.y = -cos(fAngle);
	vDirection.z = 0.0f;

	return vDirection;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateBloodRain::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;

	m_dwShotDelay = 100;
	m_iShotCount = 0;
	m_iDamageCount = 0;
	// ¼®±Ã º¸ÀÌ°Ô ÇØÁØ´Ù
	pCharacter->HideAttachment(FALSE);
	
    return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateBloodRain::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

    EnterProcess(pOwner,CurTime,dwTick);


	float fRate = GetAnimationRate();

	// ÅÁÅÁÅÁÅÁÅÁ
	if (fRate >= 22.0f && fRate <= 48.0f)
	{
		BASE_SKILLINFO *pInfo = GetSkillInfo();

		if (fRate >= 22.0f && m_iDamageCount < 1)
		{
			pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial());
			m_iDamageCount++;
		}

		if (fRate >= 30.6f && m_iDamageCount < 2)
		{
			pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial());
			m_iDamageCount++;
		}

		if (fRate >= 39.2f && m_iDamageCount < 3)
		{
			pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial());
			m_iDamageCount++;
		}

		m_dwShotDelay += dwTick;

		while(m_dwShotDelay >= 20)
		{
            float fSkillArea = (float)pInfo->m_wSkillArea / 10.0f;

            DWORD area = (DWORD)(fSkillArea * 1000.0f);
            DWORD dwAngle = rand() % 628;
            DWORD dwArea = rand() % area;
            float fAngle = (float)dwAngle / 100.0f;
            float fDistance = (float)dwArea / 1000.0f;
            WzVector vDirection = GetAngleVector(fAngle);
            WzVector TargetPosition = pCharacter->GetPosition() + (vDirection * fDistance);


            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos( m_iShotCount % 2) );
            pProjectile->SetArrowTargetPosition( TargetPosition );

            pProjectile->SetLifeTime( 200 );

            pProjectile->SetUnitCode( eProjectile_SRC_ARROW );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, 150, 0, 0 );

            pProjectile->SetHitData( 0, 0, StrToWzID("F011"), FALSE, 300, FALSE );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );


			//PROJECTILE_PARAM param;


			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.02f;
			//param.vStartPos = pCharacter->GetWeaponBonePos(m_iShotCount % 2);	
			//param.dwMoveTime = 600;
			//param.dwLifeTime = 600;

			//param.fSpeed = 3.0f;
	
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwAttackSerial = 0;
			//param.dwImageCode = 0;
			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//param.dwImageCode=eProjectile_SRC_ARROW;
			//param.bR = 150;
			//param.bG = 0;
			//param.bB = 0;
	
			//param.bMakeTrail = TRUE;
			//param.fTrailLength = 0.8f;

			//float fSkillArea;
			//fSkillArea = (float)pInfo->m_wSkillArea / 10.0f;

			//DWORD area = (DWORD)(fSkillArea * 1000.0f);
			//DWORD dwAngle = rand() % 628;
			//DWORD dwArea = rand() % area;
			//float fAngle = (float)dwAngle / 100.0f;
			//float fDistance = (float)dwArea / 1000.0f;

			//WzVector vDirection = GetAngleVector(fAngle);

			//param.vTargetPos = pCharacter->GetPosition() + (vDirection * fDistance);
			//pos=param.vTargetPos;

			//g_ProjectileManager.CreateProjectile(param);

			///////////////////////////////////////////////////////////////////////////
			//
			//ZeroMemory(&param,sizeof(param));
			//param.dwActiveDelayTime=300;
			//param.fHeight = 0.0f;
			//param.dwMoveTime =1000;
			//param.dwLifeTime =0;
			//param.fSpeed = 0.0f;
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwAttackSerial = 0;
			//param.dwImageCode = 0;
			//param.bAddNextSerial=FALSE;
			//param.bAllDamage=FALSE;
			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//param.dwImageCode=0;
			//param.effectID=StrToWzID("F011");		
			//param.vTargetPos = pos;
			//param.vStartPos = pos;

			//g_ProjectileManager.CreateProjectile(param);

			m_dwShotDelay -= 20;
			m_iShotCount++;
		}
	}
    
    return LeaveProcess(pOwner,CurTime,dwTick);
}

