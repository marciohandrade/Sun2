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

	//	Ÿ���� �����Ǿ� ���� ������
	if( pTarget == NULL )
	{		
		return STATECHANGE::FAILED;
	}

	//	���� ����
	WzVector	wvTarget = pTarget->GetPosition();
	if (pChr->IsKindOfObject(MONSTER_OBJECT))
	{
		Monster * pMonster = (Monster *)pChr;
		if ( MONSTER_TYPE_SETTING_MAPOBJECT != pMonster->GetMonsterType() )
		{
			// ��ġ�� ���Ͱ� �ƴ� ���͸� ���⼳���� �ض�.
			pChr->SetDirection( wvTarget );
		}	
	}
	else
	{
		pChr->SetDirection( wvTarget );
	}
	
	// ���� �ִϸ��̼� ����
	WzID animationID = pChr->GetCurrentAttackAnimation();
	assert(animationID);
	pChr->SetAnimation(animationID,FALSE);

	if (pChr->IsRangedNormalAttack() )
	{
		pChr->OnShotStart();
	}

	// �̵��ϸ鼭 �ϴ� �����ϰ��
	if (pChr->m_bAttackMove)
	{
		pChr->Move(pChr->m_vAttackStartPos.wvPos,pChr->m_vAttackEndPos.wvPos,CMS_RUN,TRUE,pChr->m_vAttackStartPos.wTile,pChr->m_vAttackEndPos.wTile);
	}

	// ��Ʈ�� ó���� ���� �ӽú���
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
	
	// Ÿ���� Ÿ�̹�
	if (event.m_iNumCustom)
	{
		for (int a = 0; a < event.m_iNumCustom; a++)
		{
			// Ÿ������ ������ġ������ ����ġ������ �����Ѵ�.
			switch(event.m_Custom[a].m_iaParam3[0])
			{
			// 1�� event�� ���� Ÿ�̹�
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

	// ����� �⺻���� ���� ���� ���� üũ
	if (pChr->CommonStateProcess())
	{
		// hero������ �ϵ��ڵ��� �ʹ����� �ҽ��ڵ带 ������ �����ϸ鼭��
		// hero�� Ŀ���͸������� ����� ã��.
		if (pChr == g_pHero)
		{
			g_HeroInput.SetCanDoNextAttack(TRUE);
		}
		return STATEPROCESS::SUSPEND;
	}
	
	// �̵��ӵ�
	const float fMoveSpeed = GetBaseMoveSpeedAsState(CMS_SWIPE) ;	

	
	// �̵�ó��
	if (pChr->m_bAttackMove) 
    {
		if (pChr->GetPathExplorer()->IsMoving())
		{		
			SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));
			g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed * dwTick, NULL);
			SUNPROCESSPROFILE_END(_T("PathFinding"));
		}		
	}

	// Ÿ���� ó��
	
	SUNPROCESSPROFILE_BEGIN(_T("ProcessAttack"));
	if( pTarget )
	{
		if( pChr->m_iHitEvent > pChr->m_iHitCount )
		{
			// ���Ÿ� ȭ��߻�
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
					// ��Ű�� ���� Ư��ó��~~ ����� �Ѿ��� ���³��� �������� �;��Ѵ�.
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
			// ���� ���� �Է��� �ְ� Ready�� �޾�����
			if (GENERALPARAM->IsNet()) 
            {
				if (g_pHero && !g_pHero->GetCannotMoveFlag())
				{
					bEndCondition = TRUE;
				}
			}
			// �̱۸�忡���� �׳� ������������ �Ѿ���ִ�.
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
	
	// ���� ������ �� �������� ���� �׼��� �����Ѵ�!
	if( bEndCondition)
	{
		// hero������ �ϵ��ڵ��� �ʹ����� �ҽ��ڵ带 ������ �����ϸ鼭��
		// hero�� Ŀ���͸������� ����� ã��.
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

		//	������Ʈ %d�� ���� Ÿ������ ���ų� ������ ��ҵǾ����ϴ�.
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