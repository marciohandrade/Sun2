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
상태가 시작 할때 공통의 처리를 한다.-지완 추가 
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
				assert(!"스킬사용 패킷 전송실패");
				return STATECHANGE::FAILED;
			}

			// 오프라인 처리
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

	// 몬스터의 스킬 보이스를 사용한다
	pCharacter->OnUseSkill();
}

void StateSkill::ProcessStartAnimation(Character* pCharacter)
{
	if( SkillScriptInfo* pSkillInfo = GetSkillInfo() )
	{
		//캐스팅 스킬인 경우 
		if( pSkillInfo->IsCastingSkill() == true )
		{
			// 애니메이션 시작
			pCharacter->SetAnimation( pSkillInfo->GetCastingAnimationID(), TRUE );

			m_isCastAni = TRUE;

			// 캐스팅 시간을 구한다.
			m_dwTotalCastTime = pSkillInfo->GetCastingTime();

            // 현재는 캐스팅 개념 자체가 없어졌다
			//// 플레이어인 경우에는 보너스 캐스팅 속도롤 적용
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

			// 자신이 사용한 경우 캐스팅 UI등을 표시
			if( pCharacter == g_pHero )
			{
				g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING, TRUE);

				//캐스팅 취소 이벤트 삽입 
				g_KeyQueueManager.PushKeyMsg( eDoType_cast_cancel, GlobalFunc::DoCastCancel );

				g_pHero->SetCast( m_isCastAni );
			}



			//OnEnd가 호출 안될때가 있을까....
			if (m_hCastHandle[0] != INVALID_HANDLE_VALUE )
			{
				g_pEffectManager->RemoveEffectPtr( m_hCastHandle[0] );
			}
			m_hCastHandle[0] = SkillEffectFunc::CreateCastingSkillEffect( m_dwSkillClassCode, pCharacter );
		}
		else
		{
			// 캐스팅이 없는 즉시 사용 스킬
			StartSkillAnimation( pCharacter );
		}
	}
}

void StateSkill::ProcessStartDirection(Character* pCharacter)
{
	// 스킬에 따라 캐릭터 방향을 틀어준다.
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
		// 이펙트 
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
	// 타격점 체크
	if ( m_event.m_iNumCustom != 0 )
	{
		for (int i = 0; i < m_event.m_iNumCustom; ++i)
		{
			if( m_event.m_Custom[i].m_iaParam3[0] == 1 )
			{
				++m_iHitCount;

				// Custom Event시에 이펙트를 출력
				SkillEffectFunc::ProcessSkillHitEffect( pCharacter, m_dwSkillClassCode, m_iHitCount );
				SkillEffectFunc::ProcessSkillEffect( pCharacter, m_dwSkillClassCode, m_iHitCount );

				// OnProcessCustomEvent에 하드코딩 된 작업을 수행 할 수 있도록.
				// 날아가는 발사체가 타격을 처리하는 경우에는 FALSE를 리턴해야하고, 이 경우 타격 처리는 하지않는다. (발사체가 할 것이다.)
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
	순수 가상 함수 였으나 특별한 연출이 없는한 StateSkill를 인스턴스화 할것임-지완 
*/
STATEPROCESS::TYPE StateSkill::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	ProcessAnimationEvent((Character*)pOwner, CurTime, dwTick );

	return LeaveProcess( pOwner, CurTime, dwTick );
}

//------------------------------------------------------------------------------
/**
상태가 끝날때 공통의 처리를 한다.-지완 추가 
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
	초기화 처리 꼭 여기서 할것 
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
				fprintf(fp,"Hero SkillCode 이상 SkillCode : %d", m_dwSkillCode);
				fclose(fp);
			}
		}
		else
		{
			if( FILE* fp = fopen("SkillCrashLog.txt","wt") )
			{
				fprintf(fp,"Character SkillCode 이상 SkillCode : %d", m_dwSkillCode);
				fclose(fp);
			}
		}

		assert(false);
	}
}
//------------------------------------------------------------------------------
/**
	강제 종료시 필요한 일들을 한다.
*/
void  StateSkill::OnForceChange(Object* pOwner)
{
	StateBase::OnForceChange(pOwner);

	//시작 이펙트를 제거 한다. 
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
	프로세스가 끝날때의 공통의 처리 OnEnd의 경우 자식 클래스에서 호출이 안되는 경우가 있으나 Finalize의 경우 항상 호출 한다.
	해제가 필요한것들 처리
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

//		//이펙트 스크립트가 있을때 
//		if( SkillEffectInfo* pEffectInfo = SkillEffectInfoParser::Instance()->GetSkillEffectInfo(pInfo->m_SkillClassCode) )
//		{
//
//			// 지정된 무기가 아니라면 이펙트를 표시하지 않음
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
프로세스가 시작할때의 공통 처리를 한다.
*/
VOID StateSkill::EnterProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    Character*	pCharacter = (Character*)pOwner;
    pCharacter->ProcessInterpolation(dwTick);
	// 내캐릭터 일때 게이지 그리기 

	if(m_isCastAni)//캐스트 에니가 있다면 
	{
		m_dwCurCastTime+=dwTick;

		if(m_dwCurCastTime>=m_dwTotalCastTime)//캐스트 시간이 다됐다면
		{
			m_dwCurCastTime=m_dwTotalCastTime;

			StartSkillAnimation( pCharacter );
			m_isCastAni=FALSE;//캐스트를 끄고 다음 에니로 넘어간다.
			
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

    // #8392 / 송찬종 / 스킬State은 라이더 Animation Rate를 안쓰고 플레이어의 Animation Rate를 쓰도록 수정
    m_fRet = pCharacter->current_ani_progress_rate();
}

//------------------------------------------------------------------------------
/**
프로세스가 끝날때의 공통 처리를 한다.-지완 추가 
*/
STATEPROCESS::TYPE StateSkill::LeaveProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)  
{
    Character*	pCharacter = (Character*)pOwner;

    // 종료~
    if (m_fRet >= 100.0f)//에니메이션 진척도가 100%일때
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

// 목표점까지 이동.
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

	// 무기에 상관없으면
	if ( pSkillInfo->m_iWeaponDefine[0] == -1 || pCharacter->IsKindOfObject(MONSTER_OBJECT))
	{
		animationID = pSkillInfo->m_wzidAnimationCode[0].id;
	}
	else
	{
		// 장착된 무기종류와 비교해 알맞은 애니메이션을 찾는다.

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

	// 스킬애니메이션이 정상적으로 나오도록 Blend Animation을 꺼준다!
	pCharacter->StopBlendAnimation();
	pCharacter->SetAnimation(animationID, bLoop);



#ifdef _DEBUG

	TCHAR szText[INTERFACE_STRING_LENGTH];
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

    if (pCharacter->GetBasePart()->GetWzUnit() != NULL)
    {
	    // 스킬 애니메이션이 들어있나 체크.
	    if( WzAnimationInfo* pAniInfo = pCharacter->GetAnimationInfo(animationID) )
	    {
		    if (pAniInfo->m_wiAction != animationID)
		    {
			    if (pCharacter->IsKindOfObject(PLAYER_OBJECT))
			    {
				    char *pAni = (char *)&animationID;
				    //	스킬 애니메이션이 없습니다.
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
			    //	스킬 애니메이션이 없습니다.
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
	// 타격점 체크
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
		// 이미 사거리 이내면 제자리
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



