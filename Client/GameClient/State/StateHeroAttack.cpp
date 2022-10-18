//------------------------------------------------------------------------------
//  StateHeroAttack.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "stateattack.h"
#include "stateheroattack.h"
#include "Character.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "GameFunc.h"
#include "HeroActionInput.h"
#include "GameFramework.h"
#include "Monster.h"
#include "SceneBase.h"
#include "KBMoveCheckManagerInClient.h"

//------------------------------------------------------------------------------
/**
*/
StateHeroAttack::StateHeroAttack(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateHeroAttack::~StateHeroAttack(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateHeroAttack::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateHeroAttack::OnStart(Object* pOwner, DWORD CurTime)
{
	Hero *pHero = (Hero *)pOwner;
	Character *pTarget = (Character *)pHero->GetTargetObject();
	
	if( NULL == g_pHero || NULL == pTarget )
	{		
        //	타겟이 설정되어 있지 않거나 히어로가 null이다
		return STATECHANGE::FAILED;
	}

	//	방향 설정
	WzVector	vTarget;
	vTarget = pTarget->GetPosition();

    //------------------------------------------------------------------------------ 
    //_NA003109_STYLE_PRESENTATION_IMPROVEMENT
    // 초기화 될때(실제 드래곤 버서커만 사용한다.
    if (pHero->GetComboCount() == 0)
    {
#ifdef _DEBUG
        g_pHero->NormalAttack.ani_type += 1;
        if (g_pHero->NormalAttack.ani_type == 3)
        {
            g_pHero->NormalAttack.ani_type = 0;
        }
#else
        g_pHero->NormalAttack.ani_type = rand() % 3;
#endif
        if (g_pHero->NormalAttack.ani_type == 0)
        {
            g_pHero->NormalAttack.combo_max_count = 2;  //<! 2타
        }
        else
        {
            g_pHero->NormalAttack.combo_max_count = 3;  //<! 3타
        }
    }
    //------------------------------------------------------------------------------ 

	
	switch (pHero->GetComboCount())
	{
	default:
		assert(!"잘못된 ComboCount입니다.");
		break;
	case 0:
		pHero->SetAttackSequence(eATTACK_SEQUENCE_FIRST);
		break;
	case 1:
		pHero->SetAttackSequence(eATTACK_SEQUENCE_SECOND);
		break;
	case 2:
		pHero->SetAttackSequence(eATTACK_SEQUENCE_THIRD);
		break;
	}
	
	pHero->SetDirection( vTarget );
	WzID anim = pHero->GetCurrentAttackAnimation();
	pHero->SetAnimation(anim,FALSE);
    pHero->SendRiderReleasePacket();
    pHero->SendWingsControlByAccessory(false, false);

	if (pHero->IsRangedNormalAttack() )
	{
		pHero->OnShotStart();
	}

	pHero->StopMove();
	
	WzVector vPos = pHero->GetPosition();
	WzVector vDirection = pHero->GetDirection();
	WzVector vDest;
	WzVector vDistance = pHero->GetPosition() - vTarget;

	float fDistance = VectorLength(&vDistance);	
	float fAttackDistance = pHero->GetAttackRange() + pTarget->GetBodyRadius();

		
	if (pHero->IsRangedNormalAttack()) 
    {
        // 원거리!
        vDest = vPos;
	}	
	else
	{
        // 근접공격
        if (fDistance > fAttackDistance)
        {
            vDest = vTarget + (vDirection * -fAttackDistance * 0.9f);
        }
        else 
        {
            vDest = vPos;
        }		
	}
   
	HONGPOS vStart;
	vStart.wvPos = vPos;
	vStart.wTile = g_pHero->GetPathExplorer()->GetTile();

	HONGPOS vEnd;
	if (pHero->Move(vPos,vDest,CMS_RUN,TRUE))
	{		
		if (g_pHero->GetPathExplorer()->IsMoving()) 
        {
			vEnd.wvPos = g_pHero->GetPathExplorer()->GetTargetPos();
			vEnd.wTile = g_pSunTerrain->x_pPathFinder->GetTileToStand( vDest, NULL, -1.0f, 100.0f );

			// 엔진에서 온 좌표가 잘못되어서 3미터보다 더 이동하게 될 경우 방지
			WzVector vFinalDist = vEnd.wvPos - vStart.wvPos;
			float fFinalDist = vFinalDist.x * vFinalDist.x + vFinalDist.y * vFinalDist.y;
			if (fFinalDist >= 2.99f * 2.99f)
			{
				pHero->StopMove();
				vEnd = vStart;
			}
		}
		else
		{
			vEnd = vStart;
		}
	}
	else
	{
		vEnd = vStart;
	}

	pHero->SetAttackMove(TRUE);
	pHero->SetAttackMovePath(vStart,vEnd);
		
	//	패킷 보내기
	//  일반공격
	MSG_CG_BATTLE_PLAYER_ATTACK_SYN	SendAttackPacket;
	SendAttackPacket.dwTargetKey = pTarget->GetObjectKey();		
	SendAttackPacket.byAttackType = pHero->GetAttackSequence();	

	//// 미스틱의 사이드를 사용하는경우 항상 3번째 시퀀스로 공격(범위공격)
	//if( pHero->GetWeaponKind() == eWEAPONTYPE_POLEARM )
	//{
	//	SendAttackPacket.byAttackType = eATTACK_SEQUENCE_THIRD;
	//}

	SendAttackPacket.wvCurPos = vStart.wvPos;			
	SendAttackPacket.wvDestPos = vEnd.wvPos;
	SendAttackPacket.dwClientSerial = pHero->GetNextAttackSerial();
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    SendAttackPacket.style_presentation = pHero->NormalAttack.ani_type;
#endif //_NA003109_STYLE_PRESENTATION_IMPROVEMENT

#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    SendAttackPacket.client_test_result = \
        nsSync::MemoryHackMonitor::Instance()->GetNextClientTestSerial();
#endif

	if(GameFramework::GetCurrentScene()&&
		GameFramework::GetCurrentScene()->IsEnablePkScene())
	{
		SendAttackPacket.byAttackPropensity = g_HeroInput.lsForceAttack();

		if(pTarget->IsKindOfObject(MONSTER_OBJECT))
		{
			Monster *pMon=(Monster *)pTarget;

			Player *pPlayer=(Player *)g_ObjectManager.GetObject(pMon->GetParentId());

			if(pPlayer==NULL)
				SendAttackPacket.byAttackPropensity=ATTACK_PROPENSITY_NORMAL;
		}
	}
	else
    {
		SendAttackPacket.byAttackPropensity=ATTACK_PROPENSITY_NORMAL;
    }


	// 테스트
	g_TempVariable[0]++;

	SendAttackPacket.Encode();

	if( FALSE == pHero->SendPacket( &SendAttackPacket, sizeof(MSG_CG_BATTLE_PLAYER_ATTACK_SYN) ) )
	{
		assert( !"공격패킷 전송 실패( MSG_CG_BATTLE_PLAYER_ATTACK_SYN - StateHeroAttack.cpp )" );
		return STATECHANGE::FAILED;
	}

	pHero->m_iHitEvent = 0;
	pHero->SetHitCount(0);
	pHero->SetCurrentAttackSerial(pHero->GetNextAttackSerial());
	m_bTrail = FALSE;
	
	// 잠시 서버의 다음공격까지 응답을 기다리지않는다 응답 
	pHero->SetCannotMoveFlag(TRUE);

	pHero->StopBlendAnimation();


    DWORD count=g_HeroInput.IncreaseComboCount();

	pHero->SetComboCount(count);

	g_HeroInput.SetCanDoNextAttack(FALSE);

	m_bCriAttkAni = TRUE;

#ifdef _YJ_MOD_BLOW_MOTION_001188
	m_bHit		  = FALSE;
	eWEAPONTYPE eWType = (eWEAPONTYPE)pHero->GetWeaponKind();
	switch(eWType)
	{
	case eWEAPONTYPE_TWOHANDAXE:
		m_dwStopTime = 240;
		break;
	case eWEAPONTYPE_TWOHANDSWORD:
		m_dwStopTime = 190;
		break;
	case eWEAPONTYPE_SPEAR:
		m_dwStopTime = 240;
		break;
	default:
		m_dwStopTime = 0;
		break;
	}
#endif //_YJ_MOD_BLOW_MOTION_001188

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateHeroAttack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE ret;
	
	ret = StateAttack::OnProcess(pOwner,CurTime,dwTick);

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateHeroAttack::OnEnd(Object* pOwner, DWORD CurTime)
{
	Hero *pHero = (Hero *)pOwner;

	if (pHero->IsRangedNormalAttack())
	{
		pHero->OnShotEnd();
	}

	pHero->ProcessAttackResult( pHero->GetCurrentAttackSerial(), TRUE );
	pHero->AddNextAttackSerial();
	pHero->SetAttackStandTime(ATTACK_STAND_TIME);


	pHero->SetForceAttack(FALSE);


	if (pHero->IsKindOfObject( PLAYER_OBJECT ) )
	{		
		pHero->EndTrail(0);
		pHero->EndTrail(1);
	}

	g_HeroInput.SetComboDelay(500);

	pHero->SetCannotMoveFlag(FALSE);

	pHero->SetbtCriAttkAni(0);

	return STATECHANGE::SUCCESS;
}