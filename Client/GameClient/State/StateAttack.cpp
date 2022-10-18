//------------------------------------------------------------------------------
//  StateAttack.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "stateattack.h"
#include "character.h"
#include "player.h"
#include "soundeffect.h"
#include "InterfaceManager.h"
#include "monster.h"
#include "objectmanager.h"
#include "ProjectileManagerEx.h"
#include "hero.h"
#include "heroactioninput.h"
#include "ChatDialog.h"
#include "NumericValues.h"
#include "globalfunc.h"
#include "PlayerMonster.h"

//------------------------------------------------------------------------------
/**
*/
StateAttack::StateAttack(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateAttack::~StateAttack(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateAttack::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAttack::OnStart(Object* pOwner, DWORD CurTime)
{
	Character *pChr = (Character *)pOwner;
	Character *pTarget = (Character *)pChr->GetTargetObject();

	DWORD dwAttackSerial = pChr->GetNextActionAttackSerial();

	pChr->SetCurrentAttackSerial(dwAttackSerial);
	pChr->SetNextAttackSerial(dwAttackSerial);

	//	타겟이 설정되어 있지 않으면
	if( pTarget == NULL )
	{		
		return STATECHANGE::FAILED;
	}

	//	방향 설정
	WzVector	wvTarget = pTarget->GetPosition();
	if (pChr->IsKindOfObject(MONSTER_OBJECT))
	{
		Monster * pMonster = (Monster *)pChr;
		if ( MONSTER_TYPE_SETTING_MAPOBJECT != pMonster->GetMonsterType() )
		{
			// 설치형 몬스터가 아닌 몬스터만 방향설정을 해라.
			pChr->SetDirection( wvTarget );
		}	
	}
	else
	{
		pChr->SetDirection( wvTarget );
	}
	
	// 공격 애니메이션 세팅
	WzID animationID = pChr->GetCurrentAttackAnimation();
	assert(animationID);
	pChr->SetAnimation(animationID,FALSE);

	if (pChr->IsRangedNormalAttack() )
	{
		pChr->OnShotStart();
	}

	// 이동하면서 하는 공격일경우
	if (pChr->m_bAttackMove)
	{
		pChr->Move(pChr->m_vAttackStartPos.wvPos,pChr->m_vAttackEndPos.wvPos,CMS_RUN,TRUE,pChr->m_vAttackStartPos.wTile,pChr->m_vAttackEndPos.wTile);
	}

	// 히트수 처리를 위한 임시변수
	pChr->m_iHitEvent = 0;
	pChr->m_iHitCount = 0;
	pChr->m_bRightHit = FALSE;


	if (pChr->IsKindOfObject(PLAYER_OBJECT) || pChr->IsKindOfObject(SSQMONSTER_OBJECT))
	{
		pChr->StopBlendAnimation();
	}

	m_bCriAttkAni = TRUE;
#ifdef _YJ_MOD_BLOW_MOTION_001188
	m_bHit		  = FALSE;
	if(pChr->IsKindOfObject(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)pChr;
		eWEAPONTYPE eWType = (eWEAPONTYPE)pPlayer->GetWeaponKind();
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
	}
#endif //_YJ_MOD_BLOW_MOTION_001188

	return STATECHANGE::SUCCESS;
}

STATEPROCESS::TYPE StateAttack::OnProcess(Object* pOwner, DWORD CurTime , DWORD dwTick)
{
	Character* pChr = (Character*)pOwner;
	assert(pChr);
	Character *pTarget = (Character *)pChr->GetTargetObject();

	WzEvent event;	
	pChr->ProcessInterpolation(dwTick);	

	float fGguoMultiplier = 1.0f;
	DWORD dwAttkSpd = DWORD( (float)dwTick * pChr->GetAttackSpeedModifier() * fGguoMultiplier );

#ifdef _YJ_MOD_BLOW_MOTION_001188
	//////////////////////////////////////////////////////////////////////////
	if(pChr->IsKindOfObject(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)pChr;
		WzID wiAni = pPlayer->GetCurrentAttackAnimation();
		if((1 == pPlayer->GetbtCriAttkAni()) && m_bCriAttkAni && eCHAR_SHADOW == pPlayer->GetClass())
		{
			WzID wiAni = pPlayer->GetCurrentAttackAnimation();
			CHAR szAniID[4] = {0,};
			WzIDToString(szAniID, wiAni);
			szAniID[2] = '1';

			WzAnimationInfo *pInfo = pChr->GetAnimationInfo(StrToWzID(szAniID));
			if(pInfo &&
				INVALID_HANDLE_VALUE != pInfo->m_hWzdAni && 
				StrToWzID("N001") != pInfo->m_wiAction)
			{
				pPlayer->SetAnimation(StrToWzID(szAniID));
			}

			m_bCriAttkAni = FALSE;
		}

		if((2 != pPlayer->GetbtCriAttkAni()) && TRUE == m_bHit)
		{
			if(0 != m_dwStopTime)
				dwAttkSpd = (DWORD)((float)dwAttkSpd / ((float)m_dwStopTime/(float)dwTick));

			if(dwTick < m_dwStopTime)
				m_dwStopTime -= dwTick;
			else
				m_dwStopTime = dwTick;

			if(100 > m_dwStopTime)
				m_bHit = FALSE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
#endif _YJ_MOD_BLOW_MOTION_001188

    float fRet = pChr->ProcessAnimation( dwAttkSpd, &event);

	WzAnimationInfo *pInfo = pChr->GetAnimationInfo(StrToWzID("A001"));
	
	// 타격점 타이밍
	if (event.m_iNumCustom)
	{
		for (int a = 0; a < event.m_iNumCustom; a++)
		{
			// 타격점에 오른쪽치기인지 왼쪽치기인지 결정한다.
			switch(event.m_Custom[a].m_iaParam3[0])
			{
			// 1번 event는 공격 타이밍
			case 1:
				{
					pChr->m_iHitEvent++;
					if (event.m_Custom[a].m_iaParam3[1] == 1) {
						pChr->m_bRightHit = FALSE;
					}
					else
					{
						pChr->m_bRightHit = TRUE;
					}		
				}
				break;
			}
		}
	}

	// 사망등 기본적인 상태 전이 조건 체크
	if (pChr->CommonStateProcess())
	{
		// hero에대한 하드코딩이 너무많음 소스코드를 적당히 공유하면서도
		// hero를 커스터마이즈할 방법을 찾자.
		if (pChr == g_pHero)
		{
			g_HeroInput.SetCanDoNextAttack(TRUE);
		}
		return STATEPROCESS::SUSPEND;
	}
	
	// 이동속도
	const float fMoveSpeed = GetBaseMoveSpeedAsState(CMS_SWIPE) ;	

	
	// 이동처리
	if (pChr->m_bAttackMove) 
    {
		if (pChr->GetPathExplorer()->IsMoving())
		{		
			SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));
			g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed * dwTick, NULL);
			SUNPROCESSPROFILE_END(_T("PathFinding"));
		}		
	}

	// 타격점 처리
	
	SUNPROCESSPROFILE_BEGIN(_T("ProcessAttack"));
	if( pTarget )
	{
		if( pChr->m_iHitEvent > pChr->m_iHitCount )
		{
			// 원거리 화살발사
			if (pChr->IsRangedNormalAttack() )
			{
				if (pChr->m_iHitCount < pChr->GetShotCount() ) 
                {
					if (pChr->GetShotCount() == 1) 
                    {
						int iRt=0;
						if (pTarget) 
						{	
							iRt=pChr->CreateProjectile(pTarget,pChr->m_iHitCount);

						}

						pChr->OnShotFire(pChr->m_iHitCount);

						if(iRt)
							pChr->m_iHitCount++;		
					}
					// 발키리 공격 특별처리~~ 사실은 총알이 없는놈이 이쪽으로 와야한다.
					else
					{
						pChr->OnShotFire(pChr->m_iHitCount);
						if( pChr->ProcessAttackResult(pChr->GetCurrentAttackSerial(), FALSE, pChr->m_bRightHit ) )
						{
							pChr->OnShotFire(pChr->m_iHitCount);
							pChr->m_iHitCount++;									
						}
					}					
				}
			}

			if( pChr->m_iHitCount < 1 )
			{
				if( pChr->ProcessAttackResult_Group( pChr->GetCurrentAttackSerial(), pChr->m_bRightHit ) )
				{									
					pChr->m_iHitCount++;	
#ifdef _YJ_MOD_BLOW_MOTION_001188
					m_bHit = TRUE;
#endif //_YJ_MOD_BLOW_MOTION_001188
				}				
			}
		}				
	}

	BOOL bEndCondition = FALSE;

	if (pChr == g_pHero)
	{
		BOOL bAttackEnd = FALSE;

		if (GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_STEP3)
		{
			if (fRet >= 100.0f)
			{
				bAttackEnd = TRUE;
			}

			if (fRet >= 80.0f && g_HeroInput.GetKeyboardQueueAttack())
			{
				bAttackEnd = TRUE;
			}
		}
		else
		{
			if (fRet >= 100.0f)
			{
				bAttackEnd = TRUE;
			}
		}

		if (bAttackEnd) 
		{
			// 다음 공격 입력이 있고 Ready를 받았으면
			if (GENERALPARAM->IsNet()) 
            {
				if (g_pHero && !g_pHero->GetCannotMoveFlag())
				{
					bEndCondition = TRUE;
				}
			}
			// 싱글모드에서는 그냥 다음공격으로 넘어갈수있다.
			else
			{
				bEndCondition = TRUE;
			}
		}
	}
	else
	{
		if (fRet >= 100.0f) 
		{
			bEndCondition = TRUE;
		}
	}

	SUNPROCESSPROFILE_END(_T("ProcessAttack"));
	
	// 공격 동작이 다 끝났으면 다음 액션을 수행한답!
	if( bEndCondition)
	{
		// hero에대한 하드코딩이 너무많음 소스코드를 적당히 공유하면서도
		// hero를 커스터마이즈할 방법을 찾자.
		if (pChr == g_pHero)
		{
			g_HeroInput.SetCanDoNextAttack(TRUE);
		}

		if (pChr->ProcessNextAction()) 
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
/**
*/
STATECHANGE::TYPE StateAttack::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character *)pOwner;

	if (pChr->m_iHitCount < pChr->GetShotCount())
	{
#if defined( _DEBUG )
		TCHAR	szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];

		//	오브젝트 %d의 공격 타격점이 없거나 공격이 취소되었습니다.
		g_InterfaceManager.GetInterfaceString(eST_STATE_ATTACK_FAIL, szText, INTERFACE_STRING_LENGTH);
		_stprintf(szMessage, szText, pChr->GetObjectKey());
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
#endif

		pChr->ProcessAttackResult( pChr->GetCurrentAttackSerial(), TRUE);
	}

	if (pChr->IsRangedNormalAttack())
	{
		pChr->OnShotEnd();
	}

	pChr->m_iHitCount = 0;
	pChr->AddNextAttackSerial();
	pChr->SetAttackStandTime(ATTACK_STAND_TIME);

	if (pChr->IsKindOfObject(SSQMONSTER_OBJECT))
	{
		PlayerMonster* pMon = (PlayerMonster*)pChr;
		pMon->SetComboDelay(10000);
	}


	if (pChr->IsKindOfObject(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)pChr;
		pPlayer->SetbtCriAttkAni(0);
	}

	return STATECHANGE::SUCCESS;
}