#include "SunClientPrecompiledHeader.h"
#include "SkillStateCustom.h"

#include "SkillState.h"
#include "Character.h"
#include "Hero.h"
#include "SkillEffectFunc.h"
#include "GlobalFunc.h"
#include "SkillInfoParser.h"
#include "CloneManager.h"
#include "clone.h"
#include "ProjectileManagerEx.h"
#include "AppearanceManager.h"
#include "HeroActionInput.h"



SkillStateCustom::SkillStateCustom()
: m_bCreateProjectile( FALSE )
{

}

STATEPROCESS::TYPE	SkillStateCustom::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	CheckEvent();

    STATEPROCESS::TYPE ProcessType = STATEPROCESS::HOLD;
	switch( m_dwSkillClassCode )
	{
	case eSKILL_DRAGON_ARM_TRANSFORM:
		{
			ProcessType = _OnProcessDragonTransForm(pOwner,CurTime,dwTick);
		}
		break;

	case eSKILL_DRAGON_CLER:
		{
			ProcessType = _OnProcessDragonCler(pOwner,CurTime,dwTick);
		}
		break;

	//case eSKILL_SACRED_FIRE:
	//	{
	//		ProcessType = _OnProcessSacredFire(pOwner,CurTime,dwTick);
	//	}
	//	break;

	case eSKILL_INQUIRY:
		{
			ProcessType = _OnProcessInQuiry(pOwner,CurTime,dwTick);
		}
		break;

    case eSKILL_MYSTIC_919: // 에너지볼트
	case eSKILL_MYSTIC_920:	// 플레임 버스트
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eSKILL_HELLROID_1017:  // 에테르차징
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eSKILL_WITCHBLADE_1100: // 호라이즌잭
    case eSKILL_WITCHBLADE_1119: // 다이브헬팡
    case eSKILL_WITCHBLADE_1125: // 프로스트블레스
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            if (CheckNotProcessedHitEvent(1) == TRUE )
            {
                Character* pCharacter = static_cast<Character*>(pOwner);
                SkillEffectFunc::ProcessSkillProjectile( pCharacter, m_dwSkillClassCode, GetSkillInfo(), 1 );
                ++m_iHitProcessed;
            }
        }
        break;

	case eSKILL_MYSTIC_926:	// 와일드팽

		if (CheckNotProcessedHitEvent(1) == TRUE )
		{
			if( Character* pCharacter = static_cast<Character*>(pOwner) )
			{
				if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
				{
					WzVector TargetPosition = pTarget->GetBodyPos();

					WzVector Temp;
					VectorCrossProduct( &Temp, &pCharacter->GetDirection(), &c_UpVector );

					{
						WzVector StartPosition = pCharacter->GetVisiblePos() - pCharacter->GetDirection() * 2.0f + Temp;


						WzVector Length = TargetPosition - StartPosition;
						float l = VectorLength(&Length);

						static float s_move_per_length = 70.0f;
						DWORD MoveTime = (DWORD)(l * s_move_per_length);

						static float s_OffsetHeight = 0.0f;
						TargetPosition.z += s_OffsetHeight;
						StartPosition.z += s_OffsetHeight;


						cProjectileUnit* pProjectile = new cProjectileUnit();
						pProjectile->SetArrowTargetPosition( TargetPosition );
						pProjectile->SetArrowStartPosition( StartPosition );
						pProjectile->SetLifeTime( MoveTime );
						pProjectile->SetUnitCode( eProjectile_SRC_WOLF );
						pProjectile->SetUnitAnimation( StrToWzID("A001"), TRUE );
						pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("KB73"), FALSE, 0, FALSE );
						g_ProjectileManagerEx.InsertProjectile( pProjectile );
					}

					{
						WzVector StartPosition = pCharacter->GetVisiblePos() - pCharacter->GetDirection() * 2.0f - Temp;


						WzVector Length = TargetPosition - StartPosition;
						float l = VectorLength(&Length);

						static DWORD s_move_per_length = 70;
						DWORD MoveTime = (DWORD)(l * s_move_per_length);

						static float s_OffsetHeight = 0.0f;
						TargetPosition.z += s_OffsetHeight;
						StartPosition.z += s_OffsetHeight;


						cProjectileUnit* pProjectile = new cProjectileUnit();
						pProjectile->SetArrowTargetPosition( TargetPosition );
						pProjectile->SetArrowStartPosition( StartPosition );
						pProjectile->SetActiveDelayTime(70);
						pProjectile->SetLifeTime( MoveTime );
						pProjectile->SetUnitCode( eProjectile_SRC_WOLF );
						pProjectile->SetUnitAnimation( StrToWzID("A001"), TRUE );
						pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("KB73"), FALSE, 0, FALSE );
						g_ProjectileManagerEx.InsertProjectile( pProjectile );

					}
				}

			}

			++m_iHitProcessed;
		}
		break;
	case eSKILL_MYSTIC_928:	// 와일드크래쉬

		if (CheckNotProcessedHitEvent(1) == TRUE )
		{
			if( Character* pCharacter = static_cast<Character*>(pOwner) )
			{
				if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
				{
					WzVector TargetPosition = pTarget->GetBodyPos();

					WzVector Temp;
					VectorCrossProduct( &Temp, &pCharacter->GetDirection(), &c_UpVector );

					{
						WzVector StartPosition = pCharacter->GetVisiblePos() - pCharacter->GetDirection() * 2.5f + Temp * 0.5f;


						WzVector Length = TargetPosition - StartPosition;
						float l = VectorLength(&Length);

						static float s_move_per_length = 70.0f;
						DWORD MoveTime = (DWORD)(l * s_move_per_length);

						static float s_OffsetHeight = 0.0f;
						TargetPosition.z += s_OffsetHeight;
						StartPosition.z += s_OffsetHeight;


						cProjectileUnit* pProjectile = new cProjectileUnit();
						pProjectile->SetArrowTargetPosition( TargetPosition );
						pProjectile->SetArrowStartPosition( StartPosition );
						pProjectile->SetLifeTime( MoveTime );
						pProjectile->SetUnitCode( eProjectile_SRC_BEAR );
						pProjectile->SetUnitAnimation( StrToWzID("A001"), TRUE );
						pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("KC23"), FALSE, 0, FALSE );
						g_ProjectileManagerEx.InsertProjectile( pProjectile );
					}
				}

			}

			++m_iHitProcessed;
		}
		break;

    case eSKILL_MYSTIC_936: // 피닉스버스트
        {
            if (CheckNotProcessedHitEvent(1) == TRUE )
            {
                if( Character* pCharacter = static_cast<Character*>(pOwner) )
                {
					static DWORD FlayingTime = 400;
					static DWORD Temp = 500;

					{
						WzVector StartPosition = pCharacter->GetVisiblePos();
						WzVector TargetPosition = GetTargetAreaPosition( pCharacter );
						WzVector Direction = TargetPosition - StartPosition;
						VectorNormalize(&Direction, &Direction);
						StartPosition = TargetPosition - Direction*10.0f;
						StartPosition.z += 5.0f;

						cProjectileUnit* pProjectile = new cProjectileUnit();
						pProjectile->SetArrowTargetPosition( TargetPosition );
						pProjectile->SetArrowStartPosition( StartPosition );
						pProjectile->SetLifeTime( FlayingTime );
						pProjectile->SetUnitCode( eProjectile_SRC_PHOENIX );
						pProjectile->SetUnitAnimation( StrToWzID("A001"), TRUE );
						g_ProjectileManagerEx.InsertProjectile( pProjectile );
					}

					{
						WzVector TargetPosition = GetTargetAreaPosition( pCharacter );

						cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
						pProjectile->SetTargetPosition( TargetPosition );
						pProjectile->SetActiveDelayTime( FlayingTime );
						pProjectile->SetLifeTime( 4000 );
						pProjectile->CreateEffect( StrToWzID("KC14"), pCharacter->GetObjectKey(), 0 );
						pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
						g_ProjectileManagerEx.InsertProjectile( pProjectile );
					}
                }

                ++m_iHitProcessed;
            }
        }

	default://연출이 필요 없는 기본 형 
		{
			Character* pCharacter = static_cast<Character*>(pOwner);

			//6연타까지 잡아낸다.
			for(int i=1;i<=MAX_HIT;i++)
			{
				ProcessHit(pCharacter,i);
			}
		}
		break;
	}

    if (ProcessType == STATEPROCESS::HOLD)
    {
        ProcessType = LeaveProcess( pOwner, CurTime, dwTick );
    }
	return ProcessType;
}


//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateCustom::_OnProcessDragonTransForm(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if( GENERALPARAM->IsNet()==FALSE)
	{
		if(pCharacter==g_pHero&&!g_pHero->IsTransFormation())//싱글은 직접 속성을 넣어 준다.
		{

			RESOURCECODE code=0;
			eTRANSFORM_TYPE type=eTRANSFORM_TYPE_NONE;

			if( SkillScriptInfo* pSkillInfo = GetSkillInfo() )
			{
				BASE_ABILITYINFO * pAbility = pSkillInfo->GetAbilityInfo( eABILITY_DRAGON_TRANSFORMATION1 );
				//assert(pAbility);

				if(pAbility)
				{
					code= pAbility->m_iOption1;
					type= (eTRANSFORM_TYPE)pAbility->m_iOption2;
				}
				else
				{
					ASSERT("변신 어빌리티 인포가 없다");

				}
			}
			else 
			{
				ASSERT("변신 스킬 인포가 없다");

			}


			if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( eCHAR_STATE_TRANSFORMATION, 0 ) )
            {
                pCharacter->AddSkillEffect(pEffect);
            }

			PLAYER_ACTION action;
			ZeroMemory(&action,sizeof(action));
			action.ActionID = ACTION_TRANSFORM;	
			action.TRANSFORM.appearKind=APPEARANCEINFO::PLAYERTRANSFORM;
			action.TRANSFORM.transFormProcess=TRANSFORM_PROCESS_TRANSFORM;
			//Todo:스크립트에서 얻어 온다.
			action.TRANSFORM.transFormType=type;
			action.TRANSFORM.rcCode=code;

			g_pHero->SetTransSkillCode(m_dwSkillCode);
			g_pHero->UpdateSkillAttr();
			g_HeroInput.SetTransFormAction(&action);
		}
	}

	if(pCharacter->IsTransFormation())//에니메이션이 반정도 나오고 변신 상태 일땐 
	{
		if (CheckNotProcessedHitEvent(1))
		{
			++m_iHitProcessed;

			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("H623"),pCharacter->GetVisiblePos(),pCharacter->GetScale(),pCharacter->GetObjectKey());   
	
			DWORD dwCloneKey = g_CloneManager.CreateClone(pCharacter);
			if (Clone* pClone = g_CloneManager.GetClone(dwCloneKey) )
			{
				int iR = Red_WzColor(pCharacter->GetColor());
				int iG = Green_WzColor(pCharacter->GetColor());
				int iB = Blue_WzColor(pCharacter->GetColor());

				WzColor color=WzColor_RGBA(iR,iG,iB,255);
				pClone->SetAlphaBlend(AB_LIGHTING);
				pClone->SetColor(color);

				pClone->SetLifeTime(200);
				pClone->SetDisappear(100);
				pClone->SetPlayAni(TRUE);

				pClone->SetRestraintAniTick(TRUE);
			
			}
            return STATEPROCESS::END;
		}
	}
    return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateCustom::_OnProcessInQuiry(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	if (CheckNotProcessedHitEvent(1) == FALSE )
		return STATEPROCESS::HOLD;

	++m_iHitProcessed;

	Character*	pCharacter = (Character*)pOwner;

	if( Character *pTarget = (Character *)pCharacter->GetTargetObject() )
	{
		WzVector StartPosition;
		WzVector TargetPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);
		TargetPosition = pTarget->GetArrowTargetPos();

		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 33 );
		pProjectile->SetRemainTime( 500 - 33 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0 , FALSE, 200, TRUE );
		pProjectile->CreateEffect( StrToWzID("k610"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		m_bCreateProjectile = TRUE;
	}

    return STATEPROCESS::HOLD;
}

////------------------------------------------------------------------------------
///**
//*/
//STATEPROCESS::TYPE SkillStateCustom::_OnProcessSacredFire(Object* pOwner, DWORD CurTime,DWORD dwTick)
//{
//	if (CheckNotProcessedHitEvent(1) == FALSE )
//		return STATEPROCESS::HOLD;
//
//	++m_iHitProcessed;
//
//	Character*	pCharacter = (Character*)pOwner;
//
//	if( Character *pTarget = (Character *)pCharacter->GetTargetObject() )
//	{
//		//for(int i=0;i<8;i++)
//		//{
//
//		//	WzVector vDirection;
//
//		//	vDirection.x = (float)sin(WZ_PI/4.0f*(float)i);
//		//	vDirection.y = (float)-cos(WZ_PI/4.0f*(float)i);
//		//	vDirection.z=0.0f;
//
//		//	DWORD dwDelay=rand()%1000;
//
//		//	WzVector StartPosition;
//		//	WzVector TargetPosition;
//
//		//	StartPosition = pTarget->GetVisiblePos();
//		//	StartPosition.z += 10.0f;
//		//	StartPosition += vDirection * 1.5f;
//
//		//	TargetPosition = pTarget->GetVisiblePos();
//		//	TargetPosition += vDirection * 1.5f;
//
//		//	{
//		//		cProjectileUnit* pProjectile = new cProjectileUnit();
//		//		pProjectile->SetArrowStartPosition( StartPosition );
//		//		pProjectile->SetArrowTargetPosition( TargetPosition );
//		//		pProjectile->SetLifeTime( 500 );
//		//		pProjectile->SetRemainTime( 100 );
//		//		pProjectile->CreateEffect( StrToWzID("k601"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
//		//		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
//		//		g_ProjectileManagerEx.InsertProjectile( pProjectile );
//		//	}
//
//		//	{
//		//		TargetPosition = pTarget->GetVisiblePos();
//		//		TargetPosition += vDirection * 1.5f;
//		//		TargetPosition.z += 0.5f;
//
//		//		cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
//		//		pProjectile->SetTargetPosition( TargetPosition );
//		//		pProjectile->SetLifeTime(2000);
//		//		pProjectile->SetActiveDelayTime( 600*dwDelay );
//		//		pProjectile->CreateEffect( StrToWzID("k602"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
//		//		g_ProjectileManagerEx.InsertProjectile( pProjectile );
//		//	}
//
//		//	m_bCreateProjectile = TRUE;
//		//}
//        m_bCreateProjectile = TRUE;
//	}
//
//    return STATEPROCESS::HOLD;
//}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateCustom::_OnProcessDragonCler(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
        pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial());
        ++m_iHitProcessed;

		Character* pTarget = (Character*)pCharacter->GetTargetObject();
		WzVector vPos = pCharacter->GetVisiblePos();
		DWORD dwTempId = pCharacter->GetObjectKey();

		if(pTarget)
		{
			dwTempId = pTarget->GetObjectKey();
			vPos = pTarget->GetVisiblePos();
			vPos -= pCharacter->GetDirection()*1.5f;

		}
		HANDLE hHandle= GlobalFunc::CreateEffectBeUpToOption(StrToWzID("H640"),vPos,pCharacter->GetScale(),pCharacter->GetObjectKey(),dwTempId); 

		if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hHandle) )
		{
			WzVector vDest,vTemp;
			vDest = vPos + pCharacter->GetDirection()*10.0f;
			vDest = vDest - vPos;
			vTemp = vDest;
			vTemp.z = 0.0f;

			float fLength;
			if (vTemp.x == 0.0f && vTemp.y == 0.0f)
			{
				fLength = 0.0f;
			}
			else
			{
				fLength = VectorLength(&vTemp);
			}


			WzVector vRot;
			vRot.x = -atan2(vDest.z, fLength);
			vRot.z = Math_GetAngleFromVector(&vDest);
			vRot.y = 0.0f;

			WzVector vScale;
			vScale.x = 1.0f;
			vScale.y = 1.0f;
			vScale.z = 1.0f;

			pBody->SetResultMatrix( vPos, vRot, vScale );
		}
	}

	if ( CheckNotProcessedHitEvent(2) )
	{
		if ( pCharacter->ProcessAttackResult_Group( pCharacter->GetCurrentAttackSerial()) )
		{
			++m_iHitProcessed;
		}
	}
    return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
상태가 끝날때 공통의 처리를 한다.-지완 추가 
*/
STATECHANGE::TYPE SkillStateCustom::OnEnd(Object* pOwner, DWORD CurTime)
{
    Character* pCharacter = (Character*)pOwner;

	if( m_bCreateProjectile == FALSE )
	{
		pCharacter->ProcessAttackResult( pCharacter->GetCurrentAttackSerial(), TRUE );
		pCharacter->AddNextAttackSerial();
	}

	if (pCharacter != NULL)
	{
		pCharacter->SetAttackStandTime(ATTACK_STAND_TIME);
	}

    return STATECHANGE::SUCCESS;
}


const WzVector& SkillStateCustom::GetTargetAreaPosition( Character* pCharacter )
{
	return pCharacter->GetCurrentAction()->SKILL.vTargePos;
}
