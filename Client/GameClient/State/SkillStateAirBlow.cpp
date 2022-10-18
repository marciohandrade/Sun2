//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateAirBlow.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "BattleScene.h"
#include "MissionScene.h"
#include "Hero.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
STATECHANGE::TYPE SkillStateAirBlow::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;
	
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    // no operation
#else
	if (g_pEffectManager)
	{

		m_hTrailEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E004"),pCharacter->GetBonePos(87),pCharacter->GetObjectKey());


		// 굴절은 엔진팀의 일단 사용하지 말라고함
		// !! 굴절적용시 인터페이스를 그리지 못함. !!
		CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hTrailEffect);
		if( pBody)
		{

			pBody->SetRenderType(AR_REFRACTION_GLASS);
			pBody->SetRefractionRate(0.5f);		// 굴절률.(0에 가까워 질수록 투명)
			pBody->SetRefractionAlpha(1.0f);	// 1.0 에 가까워질수록 본 이미지에 가까워짐

			WzVector pos=pCharacter->GetBonePos(90);
			WzVector dir;
			WzMatrix mat=pCharacter->GetBoneMatrix(90);
			dir.x=mat._11*1.2f;
			dir.y=mat._12*1.2f;
			dir.z=mat._13*1.2f;//밑으로 내린다.

			pos+=dir;

			CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
            assert( pFChain );

            if( pFChain )
            {
			    pFChain->SetLifeInfo(0, 30000);
			    pFChain->SetTailNum(40);							
			    pFChain->SetSplineLength(0.2f);

			    pFChain->SetPosition(pCharacter->GetBonePos(90),pos);				
            }

		}
		else
		{
			//assert(!"검기 이펙트를 생성하지 못했습니다");
		}
	}
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

    return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateAirBlow::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    EnterProcess(pOwner,CurTime,dwTick);

	// 타격점 체크
	// 타격점 체크
	CheckEvent();

#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    
    if (CheckNotProcessedHitEvent(1))
    {
        ++m_iHitProcessed;
        
        if (Character* pTarget = static_cast<Character*>(pChr->GetTargetObject()))
        {
            DWORD TargetObjectKey;
            WzVector StartPosition;
            WzVector TargetPosition;

            WzVector dir=pChr->GetDirection();
            StartPosition = pChr->GetWeaponBonePos(0);

            TargetPosition = pTarget->GetVisiblePos();        
            TargetPosition.z -= (pTarget->GetArrowTargetHeight());
            TargetPosition.z += (pTarget->GetArrowTargetHeight()) / 100.0f * 80.0f; // 80%정도 위로 올려서 보정해 준다.        
            TargetObjectKey = pTarget->GetObjectKey();

            cProjectileAirBlow* pProjectile = new cProjectileAirBlow();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetBlowTargetObjectKey( TargetObjectKey );
            pProjectile->SetBlowInfo( 20.0f, 0.5f );

            pProjectile->SetLifeTime( 160 );
            pProjectile->CreateEffect( StrToWzID("KF60"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
            pProjectile->SetEffectScale( 1.0f );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, rand()&155+100, 0, 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

        if (pChr == (Character *)g_pHero)
        {	
            g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
            g_Camera.StartVibration(30);
        }
    }    
#else
    if (CheckNotProcessedHitEvent(1))
    {
        ++m_iHitProcessed;

        if( Character* pTarget = (Character *)pChr->GetTargetObject() )
        {
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
            if (GlobalFunc::IsDominationScene() == FALSE)
            {
                pTarget->SetAir( 5000 );
            }
            else if (GlobalFunc::IsDominationScene() == TRUE 
                && pTarget->GetCurState() != STATE::COLLECT)
            {
                pTarget->SetAir( 5000 );
            }
#else
			pTarget->SetAir( 5000 );
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
        }
    }

	// 4발발사! 다다다다 Todo: 살짝 살짝 대상 올릴것 
	if (CheckNotProcessedHitEvent(2))
	{
		++m_iHitProcessed;

		if( Character* pTarget = (Character *)pChr->GetTargetObject() )
		{
            DWORD TargetObjectKey;
            WzVector StartPosition;
            WzVector TargetPosition;

            WzVector dir=pChr->GetDirection();
            StartPosition = pChr->GetWeaponBonePos(0);

            TargetPosition = pTarget->GetVisiblePos();
            TargetPosition.z += (pTarget->GetArrowTargetHeight());
            TargetPosition.z += 3.0f;
            TargetObjectKey = pTarget->GetObjectKey();

            cProjectileAirBlow* pProjectile = new cProjectileAirBlow();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetBlowTargetObjectKey( TargetObjectKey );
            pProjectile->SetBlowInfo( 20.0f, 0.5f );

            pProjectile->SetLifeTime( 160 );
            pProjectile->CreateEffect( StrToWzID("E077"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
            pProjectile->SetEffectScale( 0.4f );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, rand()&155+100, 0, 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
		{
			g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
			g_Camera.StartVibration(5,0.01f);
		}
	}
	
	if (CheckNotProcessedHitEvent(3))
	{
		++m_iHitProcessed;

		if( Character *pTarget = (Character *)pChr->GetTargetObject() )
		{
            DWORD TargetObjectKey;
            WzVector StartPosition;
            WzVector TargetPosition;

            WzVector dir=pChr->GetDirection();
            StartPosition = pChr->GetWeaponBonePos(1);

            TargetPosition = pTarget->GetVisiblePos();
            TargetPosition.z += (pTarget->GetArrowTargetHeight());
            TargetPosition.z += 3.0f;

            TargetObjectKey = pTarget->GetObjectKey();


            cProjectileAirBlow* pProjectile = new cProjectileAirBlow();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetBlowTargetObjectKey( TargetObjectKey );
			pProjectile->SetBlowInfo( 20.0f, 0.5f );

            pProjectile->SetLifeTime( 160 );
            pProjectile->CreateEffect( StrToWzID("E077"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
            pProjectile->SetEffectScale( 0.4f );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, rand()&155+100, 0, 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }


		if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
		{
			g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
			g_Camera.StartVibration(5,0.01f);
		}
	}


	if (CheckNotProcessedHitEvent(4))
	{
		
		++m_iHitProcessed;

		if( Character *pTarget = (Character *)pChr->GetTargetObject() )
		{
            DWORD TargetObjectKey;
            WzVector StartPosition;
            WzVector TargetPosition;

            WzVector dir=pChr->GetDirection();
            StartPosition = pChr->GetWeaponBonePos(0);

            TargetPosition = pTarget->GetVisiblePos();
            TargetPosition.z += (pTarget->GetArrowTargetHeight());
            TargetPosition.z += 3.0f;

            TargetObjectKey = pTarget->GetObjectKey();


            cProjectileAirBlow* pProjectile = new cProjectileAirBlow();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetBlowTargetObjectKey( TargetObjectKey );
			pProjectile->SetBlowInfo( 20.0f, 0.5f );

            pProjectile->SetLifeTime( 160 );
            pProjectile->CreateEffect( StrToWzID("E077"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
            pProjectile->SetEffectScale( 0.4f );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, rand()&155+100, 0, 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}

		if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
		{
			g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
			g_Camera.StartVibration(5,0.01f);
		}
	}

	if (CheckNotProcessedHitEvent(5))
	{
		++m_iHitProcessed;

        if( Character *pTarget = (Character *)pChr->GetTargetObject() )
        {
            DWORD TargetObjectKey;
            WzVector StartPosition;
            WzVector TargetPosition;

            WzVector dir=pChr->GetDirection();
            StartPosition = pChr->GetWeaponBonePos(1);

            TargetPosition = pTarget->GetVisiblePos();
            TargetPosition.z += (pTarget->GetArrowTargetHeight());
            TargetPosition.z += 3.0f;

            TargetObjectKey = pTarget->GetObjectKey();


            cProjectileAirBlow* pProjectile = new cProjectileAirBlow();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetBlowTargetObjectKey( TargetObjectKey );
			pProjectile->SetBlowInfo( 20.0f, 0.5f );

            pProjectile->SetLifeTime( 160 );
            pProjectile->CreateEffect( StrToWzID("E077"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pChr->GetObjectKey(), pChr->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
            pProjectile->SetEffectScale( 0.4f );
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, rand()&155+100, 0, 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }

		if(pChr->GetObjectKey()==g_pHero->GetObjectKey())
		{
			g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
			g_Camera.StartVibration(5,0.01f);
		}
	}
	

	if( GetAnimationRate() >= 15 && GetAnimationRate() <= 30)
	{
		if(m_hTrailEffect!=INVALID_HANDLE_VALUE)
		{
			CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hTrailEffect);
			if(pBody)
			{
				WzVector pos=pChr->GetBonePos(90);
				WzMatrix mat=pChr->GetBoneMatrix(90);
				WzVector dir;
				dir.x=mat._11*1.2f;
				dir.y=mat._12*1.2f;
				dir.z=mat._13*1.2f;

				pos+=dir;

			    CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
                if( pFChain )
                {
			        pFChain->SetPosition(pChr->GetBonePos(90),pos);
                }
			}
		}
	}

	if(GetAnimationRate()>30)
	{
		if (g_pEffectManager&&m_hTrailEffect!=INVALID_HANDLE_VALUE)
		{
			g_pEffectManager->RemoveEffectPtr(m_hTrailEffect);
			m_hTrailEffect=INVALID_HANDLE_VALUE;
		}

	}
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

  return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateAirBlow::OnEnd(Object* pOwner, DWORD CurTime)
{
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
#else
    //Todo:특화된 처리를 한다.
	if (g_pEffectManager&&m_hTrailEffect!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hTrailEffect);
	}
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

      return STATECHANGE::SUCCESS;

}