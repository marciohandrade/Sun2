//------------------------------------------------------------------------------
//  SkillStateSpinningWave.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateSpinningWave.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Clone.h"
#include "CloneManager.h"
#include "Hero.h"
#include "SkillEffectFunc.h"


#define SPINNING_TIME 1000
#define ROT_TIME 500

#define INT_DUST_TIME 20

static int s_shot_count = 0;

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateSpinningWave::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;
	m_dwCopyTime=INT_DUST_TIME;
	m_dwCurTime=0;
	m_dwCurRotTime=0;
	m_fStartAngle=pCharacter->GetAngle();
	m_dwShotDelay=0;

    s_shot_count = 0;
	return  StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateSpinningWave::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	Character* pCharacter = (Character*)pOwner;

	m_dwShotDelay += dwTick;

    static DWORD s_shot_delay = 30;

    // 0.01초에 한번씩
	if( m_dwShotDelay > s_shot_delay )
	{
		m_dwShotDelay = 0;

        WzVector direction = pCharacter->GetDirection();
		
        // 양손무기일때는 양손으로 전후로 발사
		if (pCharacter->GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) 
		{
            {
                WzVector StartPosition = pCharacter->GetWeaponBonePos(0) + direction * -2.0f;
                WzVector TargetPosition = StartPosition + direction * -20.f;

                cProjectileUnit* pProjectile = new cProjectileUnit();
                pProjectile->SetArrowStartPosition( StartPosition );
                pProjectile->SetArrowTargetPosition( TargetPosition );

                pProjectile->SetLifeTime( 333 );
                pProjectile->CreateEffect( StrToWzID("E077"), pCharacter->GetObjectKey(), 0 );
                pProjectile->SetEffectScale( 0.3f );
                pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, rand()%155 + 100, 0, 0 );

                g_ProjectileManagerEx.InsertProjectile( pProjectile );
            }
            {
                WzVector StartPosition = pCharacter->GetWeaponBonePos(0) + direction * -2.0f;
                WzVector TargetPosition = StartPosition + direction * +20.f;

                cProjectileUnit* pProjectile = new cProjectileUnit() ;
                pProjectile->SetArrowStartPosition( StartPosition );
                pProjectile->SetArrowTargetPosition( TargetPosition );

                pProjectile->SetLifeTime( 333 );
                pProjectile->CreateEffect( StrToWzID("E077"), pCharacter->GetObjectKey(), 0 );
                pProjectile->SetEffectScale( 0.3f );
                pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, rand()%155 + 100, 0, 0 );

                g_ProjectileManagerEx.InsertProjectile( pProjectile );

            }


   //         PROJECTILE_PARAM param;
			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.02f;
			//param.dwMoveTime = 1000;
			//param.dwLifeTime = 0;
			//param.fSpeed = 3.0f;
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwAttackSerial = 0;
			//
			//param.dwType = PROJECTILE::PROJECTILEID_COMMON;
			//param.dwImageCode = eProjectile_SRC_ARROW;
			//param.effectID = StrToWzID("E077");
			//param.vScale.x = 0.3f;
			//param.vScale.y = 0.3f;
			//param.vScale.z = 0.3f;

			//param.bR = rand()%155 + 100;
			//param.bG = 0; 
			//param.bB = 0;

			//param.bMakeTrail = TRUE;
			//param.fTrailLength = 0.8f;
   //         param.vStartPos = pCharacter->GetWeaponBonePos(0) + direction * -2.0f;	
   //         param.vTargetPos = param.vStartPos + direction * -20.0f;
			//g_ProjectileManager.CreateProjectile(param);

   //         //pos = param.vTargetPos;

			//// 반대방향
			//param.vStartPos = pCharacter->GetWeaponBonePos(1) + direction * 2.0f;	
			//param.vTargetPos = param.vStartPos + direction * 20.0f;
   //         param.bR = rand()%155+100;
			////pos=param.vTargetPos;

			//g_ProjectileManager.CreateProjectile(param);
		}
		else
		{
            WzVector StartPosition = pCharacter->GetWeaponBonePos(0) + direction * -2.0f;
            WzVector TargetPosition = StartPosition + direction * -20.f;

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );

            pProjectile->SetLifeTime( 333 );
            pProjectile->CreateEffect( StrToWzID("E077"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetEffectScale( 0.5f );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.02f, 0.8f, rand()%155 + 100, 0, 0 );

            g_ProjectileManagerEx.InsertProjectile( pProjectile );

			////
   //         PROJECTILE_PARAM param;
			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.02f;
			//param.dwMoveTime = 1000;
			//param.dwLifeTime = 0;
			//param.fSpeed = 3.0f;
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwAttackSerial = 0;

			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//param.effectID = StrToWzID("E077");
			//param.vScale.x=0.5f;
			//param.vScale.y=0.5f;
			//param.vScale.z=0.5f;

			//param.bR = rand()%155+100;
			//param.bG = 0;
			//param.bB = 0;
			//param.bMakeTrail = TRUE;
			//param.fTrailLength = 0.8f;
   //         param.vStartPos = pCharacter->GetWeaponBonePos(0) + direction * 3.0f;	
   //         param.vTargetPos = param.vStartPos + direction * 20.0f;

			//g_ProjectileManager.CreateProjectile(param);
		}

        // 먼지표시
		SkillEffectFunc::CreateFootEffect(pCharacter->GetObjectKey(), pCharacter->GetBonePos(92), pCharacter->m_dwTerrainAttribute);

        ++s_shot_count;

        pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial());
	}

    // 잔상효과를 위해 클론 사용
	if( m_dwCopyTime < dwTick ) 
	{
		m_dwCopyTime = INT_DUST_TIME;
		DWORD key = g_CloneManager.CreateClone(pCharacter);	
		if ( Clone* pClone = g_CloneManager.GetClone(key) )
		{
			pClone->SetAlphaBlend(AB_ALPHA);
			pClone->SetColor(WzColor_RGBA(128,128,128,255));
			pClone->SetPlayAni(TRUE);
			pClone->SetDisappear(180);//알파 안된다
			pClone->SetLifeTime(280);

		}
	}
	else
	{
		m_dwCopyTime -= dwTick;
	}

	float fRate=(float)m_dwCurRotTime/(float)(ROT_TIME);
	fRate=min(fRate,1.0f);
	if(fRate==1.0f)
		m_dwCurRotTime=0;

	float temp = WZ_PI * 2.0f * (fRate);

	temp += m_fStartAngle;

	pCharacter->SetAngle(temp,TRUE);		

	m_dwCurTime += dwTick;
	m_dwCurRotTime += dwTick;


	// 종료~
	// StateSkill::OnProcess 의 Result는 일단, 무시한다
	if (m_dwCurTime >= SPINNING_TIME)//에니메이션 진척도가 100%일때
	{
		if (pCharacter->ProcessNextAction())
		{
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			return STATEPROCESS::END;
		}
	}

	return STATEPROCESS::HOLD;
}
//------------------------------------------------------------------------------ 
STATECHANGE::TYPE SkillStateSpinningWave::OnEnd(Object* pOwner, DWORD CurTime)
{
    if (pOwner->IsKindOfObject(CHARACTER_OBJECT) == TRUE)
    {
        Character* character = static_cast<Character*>(pOwner);
        character->ProcessAttackResult(character->GetCurrentAttackSerial(), TRUE);
        character->AddNextAttackSerial();
        character->SetAttackStandTime(ATTACK_STAND_TIME);
    }

    return STATECHANGE::SUCCESS;
}