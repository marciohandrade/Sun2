//------------------------------------------------------------------------------
//  SkillState.cpp
//  (C) 2005 Webzen Sunstudio
//-------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Character.h"
#include "Statebase.h"
#include "SkillState.h"
#include "SkillInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "GameFunc.h"
#include "SkillEffectFunc.h"
#include "CastDialog.h"
#include "Player.h"
#include "Hero.h"
#include "CooltimeManager.h"
#include "ProjectileManagerEx.h"
#include "InterfaceManager.h"
#include "HeroActionInput.h"
#include "GameFunc.h"
#include "KeyQueueManager.h"
#include "GlobalFunc.h"
#include "GameFramework.h"
#include "Monster.h"
#include "ObjectManager.h"
#include <PlayerAttributes.h>
#include "CloneManager.h"
#include "clone.h"
#include "AppearanceManager.h"
#include "Camera.h"
#include "SceneBase.h"
#include "SkillEffectFunc.h"

//------------------------------------------------------------------------------
/**
*/
StateSkill::StateSkill()
: m_dwSkillCode(0)
, m_dwSkillClassCode(0)
, m_fRet(0.0f)
, m_iHitCount(0)
, m_iHitProcessed(0)
, m_isCastAni(FALSE)
{
	m_hCastHandle[0] = INVALID_HANDLE_VALUE;
	m_hCastHandle[1] = INVALID_HANDLE_VALUE;
}

//------------------------------------------------------------------------------
/**
*/
StateSkill::~StateSkill()
{
	ReleaseCastingEffect();
}

//------------------------------------------------------------------------------
/**
���°� ���� �Ҷ� ������ ó���� �Ѵ�.-���� �߰� 
*/
STATECHANGE::TYPE StateSkill::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character *)pOwner;
	if( pCharacter != NULL )
	{
		if( pCharacter->IsMyHero() == TRUE )
		{
			if( SendSkillPacket() == FALSE )
			{
				assert(!"��ų��� ��Ŷ ���۽���");
				return STATECHANGE::FAILED;
			}

			// �������� ó��
			if( GENERALPARAM->IsNet() == FALSE )
			{
				if( util::Timer* pTimer = xGetCooltimeManager()->GetSkillCooltime(m_dwSkillCode) )
				{
					pTimer->Reset();
				}
			}
		}

		ProcessStartAttackSerial(pCharacter);
		ProcessStartAnimation(pCharacter);
		ProcessStartDirection(pCharacter);
		ProcessStartSkillEffect(pCharacter);

		return STATECHANGE::SUCCESS;
	}

	return STATECHANGE::FAILED;
}

void StateSkill::ProcessStartAttackSerial(Character* pCharacter)
{
	if( pCharacter->IsMyHero() == TRUE )
	{
		pCharacter->SetCurrentAttackSerial(pCharacter->GetNextAttackSerial());
	}
	else
	{
		DWORD dwAttackSerial = pCharacter->GetNextActionAttackSerial();

		pCharacter->SetCurrentAttackSerial(dwAttackSerial);
		pCharacter->SetNextAttackSerial(dwAttackSerial);
	}

	// ������ ��ų ���̽��� ����Ѵ�
	pCharacter->OnUseSkill();
}

void StateSkill::ProcessStartAnimation(Character* pCharacter)
{
	if( SkillScriptInfo* pSkillInfo = GetSkillInfo() )
	{
		//ĳ���� ��ų�� ��� 
		if( pSkillInfo->IsCastingSkill() == true )
		{
			// �ִϸ��̼� ����
			pCharacter->SetAnimation( pSkillInfo->GetCastingAnimationID(), TRUE );

			m_isCastAni = TRUE;

			// ĳ���� �ð��� ���Ѵ�.
			m_dwTotalCastTime = pSkillInfo->GetCastingTime();

            // ����� ĳ���� ���� ��ü�� ��������
			//// �÷��̾��� ��쿡�� ���ʽ� ĳ���� �ӵ��� ����
			//if( pCharacter->IsKindOfObject(PLAYER_OBJECT))
			//{
			//	Player* pPlayer= (Player*)pCharacter;

			//	if( PlayerAttributes* pAtt = pPlayer->GetPlayerAttribute() )
			//	{
			//		int iBonusCastingTime = pAtt->GetBonusCastingTime();

			//		float fBonusCastingTimeRatio = pAtt->GetRatioBonusCastingTime() / 100.f;
			//		fBonusCastingTimeRatio = max(-1,fBonusCastingTimeRatio);

			//		int iInterval = (int)m_dwTotalCastTime + iBonusCastingTime;
			//		m_dwTotalCastTime = max( 0, iInterval );
			//		m_dwTotalCastTime = (WORD)( m_dwTotalCastTime * ( 1 + fBonusCastingTimeRatio ) );
			//	}
			//}

			// �ڽ��� ����� ��� ĳ���� UI���� ǥ��
			if( pCharacter == g_pHero )
			{
				g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING, TRUE);

				//ĳ���� ��� �̺�Ʈ ���� 
				g_KeyQueueManager.PushKeyMsg( eDoType_cast_cancel, GlobalFunc::DoCastCancel );

				g_pHero->SetCast( m_isCastAni );
			}



			//OnEnd�� ȣ�� �ȵɶ��� ������....
			if (m_hCastHandle[0] != INVALID_HANDLE_VALUE )
			{
				g_pEffectManager->RemoveEffectPtr( m_hCastHandle[0] );
			}
			m_hCastHandle[0] = SkillEffectFunc::CreateCastingSkillEffect( m_dwSkillClassCode, pCharacter );
		}
		else
		{
			// ĳ������ ���� ��� ��� ��ų
			StartSkillAnimation( pCharacter );
		}
	}
}

void StateSkill::ProcessStartDirection(Character* pCharacter)
{
	// ��ų�� ���� ĳ���� ������ Ʋ���ش�.
	if( SkillScriptInfo* pSkillInfo = GetSkillInfo() )
	{
		if (
			pSkillInfo->m_byTarget == SKILL_TARGET_ENEMY ||
			pSkillInfo->m_byTarget == SKILL_TARGET_FRIEND ||
			pSkillInfo->m_byTarget == SKILL_TARGET_FRIEND_CORPSE ||
			pSkillInfo->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY ||
			pSkillInfo->m_byTarget == SKILL_TARGET_REACHABLE_FRIEND ||
			pSkillInfo->m_byTarget == SKILL_TARGET_ENEMY_PLAYER  || 
			pSkillInfo->m_byTarget == SKILL_TARGET_ENEMY_AND_ME 
			)
		{
			if( Character* pTarget = (Character*) pCharacter->GetTargetObject() )
			{
				pCharacter->SetDirection( pTarget->GetPosition() );
			}
		}
		else if( pSkillInfo->m_byTarget == SKILL_TARGET_AREA )
		{
			WzVector vTarget = pCharacter->GetCurrentAction()->SKILL.vTargePos;
			pCharacter->SetDirection( vTarget );
		}
	}
}

void StateSkill::ProcessStartSkillEffect(Character* pCharacter)
{
	if( SkillScriptInfo* pSkillInfo = GetSkillInfo() )
	{
		// ����Ʈ 
		// Casting BottomEffect
		if( m_hCastHandle[1] != INVALID_HANDLE_VALUE )
		{
			g_pEffectManager->RemoveEffectPtr( m_hCastHandle[1] );
		}
		m_hCastHandle[1] = SkillEffectFunc::CreateCastingSkillBottomEffect( m_dwSkillClassCode, pCharacter );

		// Start Effect
		m_dwStartProjectileKey = SkillEffectFunc::CreateStartSkillEffect( m_dwSkillClassCode, pCharacter );

		// Script Effect
		if( pSkillInfo->m_szSkillMasterEffectName )
		{
			if( StrnCmp( pSkillInfo->m_szSkillMasterEffectName , _T("NULL") , MAX_SKILLNAME_LENGTH  ) != 0 )
			{
				GlobalFunc::CreateEffectBeUpToOption( StrToWzID(pSkillInfo->m_szSkillMasterEffectName), pCharacter->GetBonePos(1), pCharacter->GetScale(), pCharacter->GetObjectKey());
			}
		}
	}
}



void StateSkill::ProcessAnimationEvent(Character* pCharacter, DWORD CurTime, DWORD dwTick )
{
	// Ÿ���� üũ
	if ( m_event.m_iNumCustom != 0 )
	{
		for (int i = 0; i < m_event.m_iNumCustom; ++i)
		{
			if( m_event.m_Custom[i].m_iaParam3[0] == 1 )
			{
				++m_iHitCount;

				// Custom Event�ÿ� ����Ʈ�� ���
				SkillEffectFunc::ProcessSkillHitEffect( pCharacter, m_dwSkillClassCode, m_iHitCount );
				SkillEffectFunc::ProcessSkillEffect( pCharacter, m_dwSkillClassCode, m_iHitCount );

				// OnProcessCustomEvent�� �ϵ��ڵ� �� �۾��� ���� �� �� �ֵ���.
				// ���ư��� �߻�ü�� Ÿ���� ó���ϴ� ��쿡�� FALSE�� �����ؾ��ϰ�, �� ��� Ÿ�� ó���� �����ʴ´�. (�߻�ü�� �� ���̴�.)
				if( OnProcessCustemEvent( m_iHitCount, pCharacter, CurTime, dwTick ) == TRUE )
				{
					pCharacter->ProcessAttackResult_Group( pCharacter->GetCurrentAttackSerial() );
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
/**
	���� ���� �Լ� ������ Ư���� ������ ������ StateSkill�� �ν��Ͻ�ȭ �Ұ���-���� 
*/
STATEPROCESS::TYPE StateSkill::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	ProcessAnimationEvent((Character*)pOwner, CurTime, dwTick );

	return LeaveProcess( pOwner, CurTime, dwTick );
}

//------------------------------------------------------------------------------
/**
���°� ������ ������ ó���� �Ѵ�.-���� �߰� 
*/
STATECHANGE::TYPE StateSkill::OnEnd(Object* pOwner, DWORD CurTime)
{
    Character* character = static_cast<Character*>(pOwner);
    if (character != NULL)
    {
        character->SetAttackStandTime(ATTACK_STAND_TIME);
    }

    return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
	�ʱ�ȭ ó�� �� ���⼭ �Ұ� 
*/
void StateSkill::Initialize(Object* pOwner)
{
	Character*	pCharacter = (Character*)pOwner;
	m_iHitProcessed = 0;
	m_iHitCount = 0;
	m_fRet = 0;
	m_isCastAni = FALSE;
	m_dwTotalCastTime = 0;
	m_dwCurCastTime = 0;
    m_pSkillScript = NULL;

	m_dwSkillCode = pCharacter->GetCurrentAction()->SKILL.dwSkillID;

	ZeroMemory(&m_event,sizeof(WzEvent));
	m_hCastHandle[0] = INVALID_HANDLE_VALUE;
	m_hCastHandle[1] = INVALID_HANDLE_VALUE;



	if( SkillScriptInfo* pSkillScriptInfo = SkillInfoParser::Instance()->GetSkillInfo( m_dwSkillCode ) )
	{
		m_pSkillScript = pSkillScriptInfo;
		m_dwSkillClassCode = pSkillScriptInfo->m_SkillClassCode;
	}
	else
	{
		if( g_pHero == pOwner )
		{
			if( FILE* fp = fopen("SkillCrashLog.txt","wt") )
			{
				fprintf(fp,"Hero SkillCode �̻� SkillCode : %d", m_dwSkillCode);
				fclose(fp);
			}
		}
		else
		{
			if( FILE* fp = fopen("SkillCrashLog.txt","wt") )
			{
				fprintf(fp,"Character SkillCode �̻� SkillCode : %d", m_dwSkillCode);
				fclose(fp);
			}
		}

		assert(false);
	}
}
//------------------------------------------------------------------------------
/**
	���� ����� �ʿ��� �ϵ��� �Ѵ�.
*/
void  StateSkill::OnForceChange(Object* pOwner)
{
	StateBase::OnForceChange(pOwner);

	//���� ����Ʈ�� ���� �Ѵ�. 
	if(m_dwStartProjectileKey)
	{
		g_ProjectileManagerEx.DeleteProjectile(m_dwStartProjectileKey);
		m_dwStartProjectileKey = 0;
	}

    Character* pCharacter = static_cast<Character*>(pOwner);

    if (pCharacter)
    {
        pCharacter->ProcessAllRemainActionResult();
    }
}

//------------------------------------------------------------------------------
/**
	���μ����� �������� ������ ó�� OnEnd�� ��� �ڽ� Ŭ�������� ȣ���� �ȵǴ� ��찡 ������ Finalize�� ��� �׻� ȣ�� �Ѵ�.
	������ �ʿ��Ѱ͵� ó��
*/
void StateSkill::Finalize(Object* pOwner)
{
	Character* pCharacter = (Character*)pOwner;

	if(m_hCastHandle[0]!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hCastHandle[0]);
		m_hCastHandle[0]=INVALID_HANDLE_VALUE;
	}

	if(m_hCastHandle[1]!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hCastHandle[1]);
		m_hCastHandle[1]=INVALID_HANDLE_VALUE;
	}

	pCharacter->EndTrail(0);
	pCharacter->EndTrail(1);
	

	if( pCharacter == g_pHero )
	{

		g_pHero->SetCast(m_isCastAni);
		g_KeyQueueManager.DeleteMsg(eDoType_cast_cancel);
		CastDialog * pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
		if(pCastDlg)
		{
			pCastDlg->SetText("");
			pCastDlg->ShowWindow(FALSE);
		}

	}


	if( SkillScriptInfo* pInfo = GetSkillInfo() )
	{
        SkillEffectFunc::CreateSkillEventEffect( pInfo->m_SkillClassCode, pCharacter, cSkillEventEffectInfoData::EVENT_TYPE_END );

//		//����Ʈ ��ũ��Ʈ�� ������ 
//		if( SkillEffectInfo* pEffectInfo = SkillEffectInfoParser::Instance()->GetSkillEffectInfo(pInfo->m_SkillClassCode) )
//		{
//
//			// ������ ���Ⱑ �ƴ϶�� ����Ʈ�� ǥ������ ����
//			Player* pPlayer = NULL;
//			if(pCharacter->IsKindOfObject(PLAYER_OBJECT))
//			{
//				pPlayer = (Player*)pCharacter;
//
//				if( (0 != pEffectInfo->m_byWeaponType) &&
//					(pPlayer->GetWeaponKind() != pEffectInfo->m_byWeaponType) )
//				{
//					return;
//				}
//			}
//
//			if(pEffectInfo->m_EffectInfo[eEffect_Type_End].m_wzidEffectCode.id != StrToWzID("NULL"))
//			{
//				SkillEffectFunc::AddSkillEffectProjectile( pCharacter, &pEffectInfo->m_EffectInfo[eEffect_Type_End], TRUE );
//			}
		//}
	}

	//if( GENERALPARAM->IsNet()==FALSE)
	//	_TestRecvDamageSkillEffect(pCharacter);
}

//------------------------------------------------------------------------------
/**
���μ����� �����Ҷ��� ���� ó���� �Ѵ�.
*/
VOID StateSkill::EnterProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    Character*	pCharacter = (Character*)pOwner;
    pCharacter->ProcessInterpolation(dwTick);
	// ��ĳ���� �϶� ������ �׸��� 

	if(m_isCastAni)//ĳ��Ʈ ���ϰ� �ִٸ� 
	{
		m_dwCurCastTime+=dwTick;

		if(m_dwCurCastTime>=m_dwTotalCastTime)//ĳ��Ʈ �ð��� �ٵƴٸ�
		{
			m_dwCurCastTime=m_dwTotalCastTime;

			StartSkillAnimation( pCharacter );
			m_isCastAni=FALSE;//ĳ��Ʈ�� ���� ���� ���Ϸ� �Ѿ��.
			
			ReleaseCastingEffect();
		}


		WzVector wzVisualPos = pCharacter->GetVisiblePos();
		WzVector wzGaugePos = wzVisualPos;
		wzGaugePos.z += 2.5f;

		float fRate =  (float)m_dwCurCastTime / (float)m_dwTotalCastTime;

		if (fRate < 0.0f)
		{
			fRate = 0.0f;
		}
		else if (fRate > 1.0f)
		{
			fRate = 1.0f;
		}

		if(pCharacter==g_pHero)
		{
			CastDialog* pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
			pCastDlg->SetGage(fRate*100.0f);
		}

	}
	else if(pCharacter==g_pHero)
    {
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING,FALSE);
    }
		

    pCharacter->ProcessAnimation(dwTick,&m_event);

    // #8392 / ������ / ��ųState�� ���̴� Animation Rate�� �Ⱦ��� �÷��̾��� Animation Rate�� ������ ����
    m_fRet = pCharacter->current_ani_progress_rate();
}

//------------------------------------------------------------------------------
/**
���μ����� �������� ���� ó���� �Ѵ�.-���� �߰� 
*/
STATEPROCESS::TYPE StateSkill::LeaveProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)  
{
    Character*	pCharacter = (Character*)pOwner;

    // ����~
    if (m_fRet >= 100.0f)//���ϸ��̼� ��ô���� 100%�϶�
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

// ��ǥ������ �̵�.
BOOL StateSkill::ProcessMove( Character *pCharacter, float fMoveSpeed, DWORD dwTick)
{
	SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));
	if (pCharacter->GetPathExplorer()->IsMoving())
	{
		return g_pSunTerrain->PathExp_Progress(pCharacter->GetPathExplorer(), fMoveSpeed  * dwTick, NULL);
	}
	SUNPROCESSPROFILE_END(_T("PathFinding"));

	return FALSE;
}

BOOL StateSkill::StartSkillAnimation( Character* pCharacter, BOOL bLoop )
{
	SkillScriptInfo* pSkillInfo = GetSkillInfo();

	if( pSkillInfo == NULL )
		return FALSE;

	assert(pCharacter->GetCurrentAction()->ActionID == ACTION_SKILL);

	const PLAYER_ACTION* pAction = pCharacter->GetCurrentAction();


	WzID animationID = 0;

	// ���⿡ ���������
	if ( pSkillInfo->m_iWeaponDefine[0] == -1 || pCharacter->IsKindOfObject(MONSTER_OBJECT))
	{
		animationID = pSkillInfo->m_wzidAnimationCode[0].id;
	}
	else
	{
		// ������ ���������� ���� �˸��� �ִϸ��̼��� ã�´�.

		DWORD dwWeaponKind = pCharacter->GetWeaponKind();
		BOOL bFind = FALSE;

		animationID = pSkillInfo->m_wzidAnimationCode[0].id;

		for (int a = 0; a < 4; a++)
		{
			if ( pSkillInfo->m_iWeaponDefine[a] == dwWeaponKind )
			{
				bFind = TRUE;
				animationID = pSkillInfo->m_wzidAnimationCode[a].id;
				break;
			}
		}
	}

	// ��ų�ִϸ��̼��� ���������� �������� Blend Animation�� ���ش�!
	pCharacter->StopBlendAnimation();
	pCharacter->SetAnimation(animationID, bLoop);



#ifdef _DEBUG

	TCHAR szText[INTERFACE_STRING_LENGTH];
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

    if (pCharacter->GetBasePart()->GetWzUnit() != NULL)
    {
	    // ��ų �ִϸ��̼��� ����ֳ� üũ.
	    if( WzAnimationInfo* pAniInfo = pCharacter->GetAnimationInfo(animationID) )
	    {
		    if (pAniInfo->m_wiAction != animationID)
		    {
			    if (pCharacter->IsKindOfObject(PLAYER_OBJECT))
			    {
				    char *pAni = (char *)&animationID;
				    //	��ų �ִϸ��̼��� �����ϴ�.
				    g_InterfaceManager.GetInterfaceString( eST_NOT_FOUND_SKILL_ANIMATION, szText, INTERFACE_STRING_LENGTH );
				    _stprintf( szMessage, _T("%c%c%c%c %s"), pAni[0], pAni[1], pAni[2], pAni[3], szText );
				    Player *pPlayer = (Player *)pCharacter;
				    pPlayer->AddChatMessage(szMessage);
			    }

		    }
	    }
	    else
	    {
		    if (pCharacter->IsKindOfObject(PLAYER_OBJECT))
		    {
			    char *pAni = (char *)&animationID;
			    //	��ų �ִϸ��̼��� �����ϴ�.
			    g_InterfaceManager.GetInterfaceString( eST_NOT_FOUND_SKILL_ANIMATION, szText, INTERFACE_STRING_LENGTH );
			    _stprintf( szMessage, _T("%c%c%c%c %s"), pAni[0], pAni[1], pAni[2], pAni[3], szText );
			    Player *pPlayer = (Player *)pCharacter;
			    pPlayer->AddChatMessage(szMessage);
		    }

	    }
    }
#endif

	return TRUE;
}

BOOL StateSkill::SendSkillPacket()
{
    return g_pHero->SendUseSkillPacket( m_dwSkillCode );
}

BOOL StateSkill::CheckEvent()
{
	// Ÿ���� üũ
	if ( m_event.m_iNumCustom == 0 )
		return FALSE;

	for (int i = 0; i < m_event.m_iNumCustom; ++i)
	{
		if( m_event.m_Custom[i].m_iaParam3[0] == 1 )
		{
			++m_iHitCount;

			//OnAnimationEvent( m_iHitCount );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL StateSkill::ProcessHit( Character* pCharacter, int HitCount)
{
	assert(HitCount > 0);

	if (CheckNotProcessedHitEvent(HitCount))
	{
        //SkillEffectFunc::ProcessSkillProjectile( pCharacter, m_dwSkillClassCode, GetSkillInfo(), HitCount );
        SkillEffectFunc::ProcessSkillHitEffect( pCharacter, m_dwSkillClassCode, HitCount );
        SkillEffectFunc::ProcessSkillEffect( pCharacter, m_dwSkillClassCode, HitCount );

        ++m_iHitProcessed;
        if (pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial()))
        {
            return TRUE;
        }
	}

	return FALSE;
}

BOOL StateSkill::CheckNotProcessedHitEvent(int HitCount)
{
	if (m_iHitProcessed < HitCount)
	{
		if (m_iHitCount > (HitCount - 1))		
		{
			return TRUE;			
		}
	}

	return FALSE;
}

void StateSkill::ReleaseCastingEffect()
{
	if(m_hCastHandle[0]!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hCastHandle[0]);
		m_hCastHandle[0]=INVALID_HANDLE_VALUE;
	}
	if(m_hCastHandle[1]!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hCastHandle[1]);
		m_hCastHandle[1]=INVALID_HANDLE_VALUE;
	}
}

WzVector StateSkill::GetTargetNearPosition( const WzVector& StartPosition, Character* pTarget, float Range )
{
	WzVector TargetPosition;

	if( pTarget != NULL )
	{
		TargetPosition = pTarget->GetPosition();
	}
	else
	{
		TargetPosition = StartPosition;
	}

	WzVector vDistance = TargetPosition - StartPosition;
	float fDistance = VectorLength(&vDistance);

	if( fDistance < Range )
	{
		// �̹� ��Ÿ� �̳��� ���ڸ�
		TargetPosition = StartPosition;
	}
	else
	{
		WzVector Direction;
		VectorNormalize(&Direction,&vDistance);

		TargetPosition = TargetPosition - (Direction * Range);
	}

	return TargetPosition;
}



