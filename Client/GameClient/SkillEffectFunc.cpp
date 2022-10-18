#include "SunClientPrecompiledHeader.h"
#include "SkillEffectFunc.h"
#include "GlobalFunc.h"

#include "SkillInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "Character.h"
#include "Player.h"

#include "ProjectileManagerEx.h"
#include "InterfaceManager.h"
#include "Camera.h"

#include <PublicMath.h>

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
#include "State/StateBase.h"
#include "State/SkillStateHpSuction.h"
#include "State/SkillState.h"
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

namespace SkillEffectFunc
{
	HANDLE AddSkillEffectEfftct( Character* pTarget, WzID EffectID )
	{
		HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption( EffectID, pTarget->GetVisiblePos(), pTarget->GetScale(), pTarget->GetObjectKey());//!loop 이펙트 쓰지 말것    

		if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hHandle) )
		{
			// 발사체와는 다르게 캐릭터의 각도와 스케일등을 직접 설정할 수 있다
			WzVector vRot;
			vRot.y = 0.0f;
			vRot.z = pTarget->GetAngle(); 
			vRot.x = pTarget->GetAngleX();

			WzVector vScale;
			vScale.x = 1.0f;
			vScale.y = 1.0f;
			vScale.z = 1.0f;

			pBody->SetResultMatrix( pTarget->GetVisiblePos(), vRot, vScale);
		}

		return hHandle;
	}

	DWORD AddSkillEffectProjectile( Character* pTarget, WzID EffectID )
	{
        cTargetPosition Position;
        Position.SetTargetVisiblePosition( pTarget->GetObjectKey(), TRUE );
        cProjectileBase* pProjectile = g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), EffectID, 10000, Position );
        return pProjectile->GetKey();
	}

	void AddCameraVibration( Character* pCharacter, float fTime, Camera::VIBE_TYPE VibeType /*= VIBE_TYPE_ALL*/, float fPower /*= 0.05f */)
	{
		if ( pCharacter == (Character *)g_pHero)
		{	
			g_Camera.SetVibeType( VibeType );
			g_Camera.StartVibration( fTime, fPower );  
		}
	}

	//------------------------------------------------------------------------------
	/**
	스킬 사용 결과 이펙트를 출력 한다.주로 버프 계열 
	*/
	DWORD AddSkillEffectProjectile( Character* pTarget, EffectInfo* pEffectInfo, BOOL bSyncAngle )
	{
		if( pEffectInfo->m_wzidEffectCode.id == StrToWzID("NULL") )
			return 0;

        cTargetPosition Position;
        Position.SetTargetBonePosition( pTarget->GetObjectKey(), pEffectInfo->m_dwPositionType, pEffectInfo->m_wBoneIndex, pEffectInfo->m_bChase );
        Position.SetSyncTargetAngle( bSyncAngle );
        cProjectileBase* pProjectile = g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), pEffectInfo->m_wzidEffectCode.id, pEffectInfo->m_dwLifeTime, Position );

        return pProjectile->GetKey();
	}


	DWORD CreateDirtyStartSkillEffect( DWORD dwSkillClassCode, Character* pCharacter )
	{
		// 스킬 시작시 카메라 흔드는 것들
		switch( dwSkillClassCode )
		{
		case eSKILL_WILD_SPIRITS:	
		case eSKILL_VITAL_AMPLYFY:
		case eSKILL_SPIRIT_RECHARGE:
		case kSkillSPRecovery:
		case eSKILL_TAUNT:
		case eSKILL_THORNS_SKIN:
			AddCameraVibration( pCharacter, 100 ); 			
			break;

		case eSKILL_DARK_FORCE:
			AddCameraVibration( pCharacter, 30 ); 			
			break;
		}


		// 이펙트
		switch (dwSkillClassCode)
		{
		case eSKILL_DEADLY_STRIKE:			AddSkillEffectEfftct( pCharacter, StrToWzID("H005") ); break;
		case eSKILL_DEATH_DIVIDE:			AddSkillEffectEfftct( pCharacter, StrToWzID("H510") ); break;
		case eSKILL_SWORDDANCING:			AddSkillEffectEfftct( pCharacter, StrToWzID("H500") ); break;
		case eSKILL_ARCANE_BLADE:			AddSkillEffectEfftct( pCharacter, StrToWzID("H340") ); break;

		case eSKILL_THORNS_SKIN:
			AddSkillEffectEfftct( pCharacter, StrToWzID("A012") ); 
			AddSkillEffectEfftct( pCharacter, StrToWzID("H006") ); 
			break;
		}


		// 이펙트 (발사체)
		switch (dwSkillClassCode)
		{
		case eSKILL_SACRED_FIRE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k600") );
		case eSKILL_ACCRUCY_SPIRIT:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k560") );
		case eSKILL_MAGIC_FORCE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k570") );
		case eSKILL_NATURAL_ATTACK:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k550") );
		case eSKILL_MIGHT_POWER:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k490") );
		case eSKILL_MANA_RECOVERY:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k580") );
		case eSKILL_FOCUS:					return AddSkillEffectProjectile( pCharacter, StrToWzID("k530") );
		case eSKILL_ESSENSE:				return AddSkillEffectProjectile( pCharacter, StrToWzID("k510") );
		case eSKILL_THORNS_SKIN:			return AddSkillEffectProjectile( pCharacter, StrToWzID("I440") );
		case kSkillSPRecovery:			    return AddSkillEffectProjectile( pCharacter, StrToWzID("I490") );
		case eSKILL_RAPID_ATTACK:			return AddSkillEffectProjectile( pCharacter, StrToWzID("H560") );
		case eSKILL_ICE_FORCE:				return AddSkillEffectProjectile( pCharacter, StrToWzID("k540") );
		case eSKILL_BLAZE_FORCE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k520") );
		case eSKILL_MANA_REACHARGE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k500") );
		case eSKILL_MIGHTY_SPIRIT:			return AddSkillEffectProjectile( pCharacter, StrToWzID("k480") );
		case eSKILL_MARK_OF_SPIRIT_POWER:	return AddSkillEffectProjectile( pCharacter, StrToWzID("H430") );
		case eSKILL_MARK_OF_FACTION:		return AddSkillEffectProjectile( pCharacter, StrToWzID("J520") );
		case eSKILL_MARK_OF_SYMPHONY:		return AddSkillEffectProjectile( pCharacter, StrToWzID("J530") );
		case eSKILL_UNLIMIT_FORCE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("H580") );
		case eSKILL_MARK_OF_RECOVERY:		return AddSkillEffectProjectile( pCharacter, StrToWzID("J510") );
		case eSKILL_MARK_OF_CHANCE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("H550") );
		case eSKILL_DEVINE_FORCE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("H540") );
		case eSKILL_BERSERK_MODE:			return AddSkillEffectProjectile( pCharacter, StrToWzID("I510") );
		case eSKILL_DAMAGE_RECOVERY:		return AddSkillEffectProjectile( pCharacter, StrToWzID("I470") );
//		case eSKILL_WILD_SPIRITS:			return AddSkillEffectProjectile( pCharacter, StrToWzID("I390") );
		case eSKILL_VITAL_AMPLYFY:			return AddSkillEffectProjectile( pCharacter, StrToWzID("I480") );
		case eSKILL_SPECTUAL_GUARD:			return AddSkillEffectProjectile( pCharacter, StrToWzID("I350") );
		}

		return 0;
	}


	DWORD CreateDirtyReceiverSkillEffect(DWORD dwSkillClassCode, Character *pTarget )
	{
		//패킷을 받아 발사체를 생성하는 것들 

		switch(dwSkillClassCode) 
		{
		case eSKILL_TAUNT:					return AddSkillEffectProjectile( pTarget, StrToWzID("I461") );
		case eSKILL_WILD_SPIRITS:			return AddSkillEffectProjectile( pTarget, StrToWzID("I391") );
		case eSKILL_THORNS_SKIN:			return AddSkillEffectProjectile( pTarget, StrToWzID("I441") );

		case eSKILL_BERSERK_MODE:			return AddSkillEffectProjectile( pTarget, StrToWzID("I511") );
		case eSKILL_DEVINE_FORCE:			return AddSkillEffectProjectile( pTarget, StrToWzID("H541") );
		case eSKILL_MARK_OF_CHANCE:			return AddSkillEffectProjectile( pTarget, StrToWzID("H551") );
		case eSKILL_FEAR:					return AddSkillEffectProjectile( pTarget, StrToWzID("H571") );
		case eSKILL_MARK_OF_RECOVERY	:	return AddSkillEffectProjectile( pTarget, StrToWzID("H581") );

		case eSKILL_MARK_OF_SPIRIT_POWER:	return AddSkillEffectProjectile( pTarget, StrToWzID("H431") );
		case eSKILL_UNLIMIT_FORCE:			return AddSkillEffectProjectile( pTarget, StrToWzID("J511") );
		case eSKILL_MARK_OF_FACTION:		return AddSkillEffectProjectile( pTarget, StrToWzID("J521") );
		case eSKILL_MARK_OF_SYMPHONY:		return AddSkillEffectProjectile( pTarget, StrToWzID("J531") );
		case eSKILL_PATIENCE:				return AddSkillEffectProjectile( pTarget, StrToWzID("J541") );

		//case eSKILL_BATTLE_BARRIOR:			return AddSkillEffectProjectile( pTarget, StrToWzID("J551") );
		case eSKILL_RESSURECTION:			return AddSkillEffectProjectile( pTarget, StrToWzID("J561") );
		case eSKILL_MIGHTY_SPIRIT:			return AddSkillEffectProjectile( pTarget, StrToWzID("k481") );
		case eSKILL_MIGHT_POWER:			return AddSkillEffectProjectile( pTarget, StrToWzID("k491") );
		case eSKILL_BLAZE_FORCE:			return AddSkillEffectProjectile( pTarget, StrToWzID("k521") );

		case eSKILL_FOCUS:					return AddSkillEffectProjectile( pTarget, StrToWzID("k531") );
		case eSKILL_ICE_FORCE:				return AddSkillEffectProjectile( pTarget, StrToWzID("k541") );
		case eSKILL_NATURAL_ATTACK:			return AddSkillEffectProjectile( pTarget, StrToWzID("k551") );
		case eSKILL_ACCRUCY_SPIRIT:			return AddSkillEffectProjectile( pTarget, StrToWzID("k561") );
		case eSKILL_MAGIC_FORCE:			return AddSkillEffectProjectile( pTarget, StrToWzID("k571") );
		case eSKILL_MANA_RECOVERY:			return AddSkillEffectProjectile( pTarget, StrToWzID("k581") );
		case eSKILL_INCRESE_SPEED:			return AddSkillEffectProjectile( pTarget, StrToWzID("k591") );
		}
		return 0;
	}

	//------------------------------------------------------------------------------
	/**
	스킬 사용 결과 이펙트를 출력 한다.주로 버프 계열 
	*/
	void CreateReceiverSkillEffect(DWORD dwSkillCode, int TargetIndex, Character* pTarget, Character* pCharacter)
	{
		if( pTarget && pCharacter )
		{
			if ( SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillCode) )
			{
				// 배틀배리어인경우에는 파티만
				if( pInfo->m_SkillClassCode == eSKILL_BATTLE_BARRIOR )
				{
					if( pTarget->IsPartyMember() == TRUE )
					{
						SkillEffectFunc::CreateSkillEventEffect( pInfo->m_SkillClassCode, pTarget, cSkillEventEffectInfoData::EVENT_TYPE_BUFF );
					}
				}
				else
				{
					SkillEffectFunc::CreateSkillEventEffect( pInfo->m_SkillClassCode, pTarget, cSkillEventEffectInfoData::EVENT_TYPE_BUFF );
				}

                if( pInfo->m_SkillClassCode == eSKILL_MYSTIC_916 ) // 미스틱댄스
                {
                    DWORD ActiveDealy = 150 * TargetIndex;
                    if( TargetIndex >= 5 )
                    {
                        ActiveDealy = 150 * (rand()%5);
                    }

                    WzVector TargetPosition = pTarget->GetVisiblePos();
                    WzVector Direction = TargetPosition - pCharacter->GetVisiblePos();
                    VectorNormalize(&Direction, &Direction );

                    static DWORD s_life_time = 1300;
                    static DWORD s_delay_time = 200;
                    static float s_length = 1.2f;
                    static size_t s_max_clone = 1;
                    static DWORD s_color = 0xEEEEEEEE;  // argb
                    static ENUM_ALPHABLEND s_blend = AB_LIGHTING;
                    std::vector<WzID> animation_array_array;
                    animation_array_array.push_back( StrToWzID("S419") );
                    animation_array_array.push_back( StrToWzID("S420") );
                    animation_array_array.push_back( StrToWzID("S421") );
                    animation_array_array.push_back( StrToWzID("S422") );
                    animation_array_array.push_back( StrToWzID("S423") );


                    std::random_shuffle( animation_array_array.begin(), animation_array_array.end() );

                    for( size_t i=0;i!=animation_array_array.size();++i)
                    {
                        WzVector NewDirection = Direction;
                        Math_ZRotateVector( &Direction, NewDirection, rand() % 360 );
                        WzVector NewPosition = TargetPosition - NewDirection * s_length;

                        {
                            cProjectileClone* pProjectile = new cProjectileClone();
                            //pProjectile->SetTargetObjectKey( pCharacter->GetObjectKey(), TRUE );
                            pProjectile->SetCloneColor( s_color );
                            pProjectile->SetCloneBlendType( s_blend );
                            pProjectile->SetClonePosition( NewPosition );
                            pProjectile->SetCloneDirection( NewDirection );
                            pProjectile->SetActiveDelayTime( i*s_delay_time + ActiveDealy );
                            pProjectile->CreateClone( pCharacter->GetObjectKey(), animation_array_array.at(i) );
                            pProjectile->SetLifeTime( s_life_time );
                            //pProjectile->SetCloneFadeOut(TRUE);
                            g_ProjectileManagerEx.InsertProjectile( pProjectile );
                        }

                        {
                            static DWORD s_ActiveDelay = 100;
                            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                            pProjectile->SetLifeTime( 1000 );
                            pProjectile->SetTargetPosition( pTarget->GetArrowTargetPos() );
                            pProjectile->CreateEffect( StrToWzID("KB21"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                            pProjectile->SetActiveDelayTime( s_ActiveDelay + ActiveDealy );
                            g_ProjectileManagerEx.InsertProjectile( pProjectile );
                        }

                        {
                            static DWORD s_ActiveDelay = 1200;
                            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                            pProjectile->SetLifeTime( 1000 );
                            pProjectile->SetTargetPosition( NewPosition );
                            pProjectile->CreateEffect( StrToWzID("KB20"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                            pProjectile->SetActiveDelayTime( s_ActiveDelay + ActiveDealy );
                            g_ProjectileManagerEx.InsertProjectile( pProjectile );
                        }
                        if( i +1 >= s_max_clone )
                            break;
                    }
                }


				//// 스크립트를 통한 이펙트
				//if ( SkillEffectInfo *pEffectInfo = SkillEffectInfoParser::Instance()->GetSkillEffectInfo(pInfo->m_SkillClassCode) )
				//{
    //                AddSkillEffectProjectile( pTarget, &pEffectInfo->m_EffectInfo[eEffect_Type_Buff], TRUE );
				//}

				// 이펙트
				CreateDirtyReceiverSkillEffect( pInfo->m_SkillClassCode, pTarget );

				// 그외
				switch(pInfo->m_SkillClassCode)//스크립트로 안되는 연출 적인 것들
				{
                case eSKILL_SACRED_FIRE:

                    for(int i=0;i<8;i++)
                    {
                        WzVector vDirection;

                        vDirection.x = (float)sin(WZ_PI/4.0f*(float)i);
                        vDirection.y = (float)-cos(WZ_PI/4.0f*(float)i);
                        vDirection.z=0.0f;

                        WzVector StartPosition;
                        WzVector TargetPosition;

                        StartPosition = pTarget->GetVisiblePos();
                        StartPosition.z += 10.0f;
                        StartPosition += vDirection * 1.5f;

                        TargetPosition = pTarget->GetVisiblePos();
                        TargetPosition += vDirection * 1.5f;

                        {
                            cProjectileUnit* pProjectile = new cProjectileUnit();
                            pProjectile->SetArrowStartPosition( StartPosition );
                            pProjectile->SetArrowTargetPosition( TargetPosition );
                            pProjectile->SetLifeTime( 500 );
                            pProjectile->SetRemainTime( 100 );
                            pProjectile->CreateEffect( StrToWzID("k601"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
                            g_ProjectileManagerEx.InsertProjectile( pProjectile );
                        }

                        {
                            TargetPosition = pTarget->GetVisiblePos();
                            TargetPosition += vDirection * 1.5f;
                            TargetPosition.z += 0.5f;

                            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                            pProjectile->SetTargetPosition( TargetPosition );
                            pProjectile->SetLifeTime(2000);
                            pProjectile->SetActiveDelayTime( 600 );
                            pProjectile->CreateEffect( StrToWzID("k602"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                            g_ProjectileManagerEx.InsertProjectile( pProjectile );
                        }
                    }
                    break;

                case eSKILL_BATTLE_BARRIOR:
                    {
                        if (pTarget)
                        {
                            EffectInfo effectInfo;
                            if (pTarget->GetObjectType() == PLAYER_OBJECT)
                            {
                                effectInfo.m_wzidEffectCode.id = StrToWzID("J551");
                                effectInfo.m_bChase = 1;
                                effectInfo.m_dwLifeTime = 1000;
                                effectInfo.m_dwPositionType = eSKILLEFFECT_POS_HEAD;
                                effectInfo.m_wBoneIndex = 0;
                                effectInfo.m_bRot = 0;
                            }
                            else if (pTarget == pCharacter)
                            { //술자자신
                                effectInfo.m_wzidEffectCode.id = StrToWzID("J550");
                                effectInfo.m_bChase = 1;
                                effectInfo.m_dwLifeTime = 1000;
                                effectInfo.m_dwPositionType = eSKILLEFFECT_POS_FOOT;
                                effectInfo.m_wBoneIndex = 0;
                                effectInfo.m_bRot = 0;
                            }
                            AddSkillEffectProjectile(pTarget, &effectInfo, TRUE);
                        }
                    }
                    break;

				case eSKILL_SONY_VINE:
					{

						BASE_ABILITYINFO * pAbility =pInfo->GetAbilityInfo( eABILITY_STUN_STATUS );
						assert(pAbility);

						if(pAbility)
						{
                            WzVector Rotate = c_ZeroVector;
                            Rotate.z = (float)(rand() % 628) / 100.f;


                            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                            pProjectile->SetTargetPosition( pTarget->GetVisiblePos() );
                            pProjectile->SetUnitCode( eProjectile_SRC_THORN2 );
                            pProjectile->SetUnitRotate( Rotate );
                            pProjectile->SetUnitAnimation( StrToWzID("N001"), FALSE );
                            pProjectile->SetUnitFadeOut( TRUE );
                            pProjectile->SetLifeTime( pAbility->m_iParam[2] );
                            g_ProjectileManagerEx.InsertProjectile( pProjectile );
                            

							float inc=rand()%50+50.0f;

							if(rand()%2==1)
							{
								inc*=-1.0f;
							}
							inc/=100.0f;

							if(rand()%5>=2)//보여지는 것 srand해야 할까?
							{
                                WzVector Rotate = c_ZeroVector;
                                Rotate.z = (float)(rand() % 628) / 100.f;


                                WzVector TargetPosition = pTarget->GetVisiblePos();
                                TargetPosition.x += inc;
                                TargetPosition.y += inc;

                                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                                pProjectile->SetTargetPosition( TargetPosition );
                                pProjectile->SetUnitCode( eProjectile_SRC_THORN );
                                pProjectile->SetUnitRotate( Rotate );
                                pProjectile->SetUnitAnimation( StrToWzID("N001"), FALSE );
                                pProjectile->SetUnitFadeOut( TRUE );
                                pProjectile->SetLifeTime( pAbility->m_iParam[2] );
                                g_ProjectileManagerEx.InsertProjectile( pProjectile );
							}
						}
					}
					break;

				case eSKILL_BEAST_MASTER_DEADLY_STRIKE:
					{
                        //플레이어에게 볼을 쏜다. 보여지는것 이외에 효과 없음 

                        WzVector StartPosition =pCharacter->GetVisiblePos();
                        StartPosition += pCharacter->GetDirection();
                        StartPosition.z += 4.0f;

                        cProjectileUnit* pProjectile = new cProjectileUnit();
                        pProjectile->SetArrowStartPosition( StartPosition );
                        pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
                        pProjectile->SetLifeTime( 1000 );
                        pProjectile->SetActiveDelayTime( 3200 );
                        pProjectile->CreateEffect( StrToWzID("E020"), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                        g_ProjectileManagerEx.InsertProjectile( pProjectile );
					}
					break;
#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
                case eSKILL_HP_SUCTION:
                    {
                        // 맞은 대상에서 오너 플레이어에게로 빨아들이는 이펙트
                        if ((pCharacter->GetCurState() == STATE::SKILL) &&
                            (pCharacter->GetCurSkillState()->GetSkillClassCode() == eSKILL_HP_SUCTION))
                        {
                                SkillStateHpSuction* skill_state = (SkillStateHpSuction*)pCharacter->GetCurSkillState();
                                skill_state->AddHpSuctionProjectile(pCharacter, pTarget);
                        }
                    }
                    break;
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
				}
			}
		}
	}


	BOOL IsEnableWeapon( Character* pCharacter, SkillEffectInfo* pSkillEffectInfo )
	{
		// 이펙트 스크립트에 지정된 무기와 다른 경우에 패스
		if( pCharacter->IsKindOfObject(PLAYER_OBJECT) )
		{
			Player* pPlayer = (Player*)pCharacter;

			if( pSkillEffectInfo->m_byWeaponType != 0 && pPlayer->GetWeaponKind() != pSkillEffectInfo->m_byWeaponType )
			{
				return FALSE;
			}
		}

		return TRUE;
	}



	DWORD CreateStartSkillEffect(DWORD dwSkillClassCode, Character* pCharacter )
	{
        SkillEffectFunc::CreateSkillEventEffect( dwSkillClassCode, pCharacter, cSkillEventEffectInfoData::EVENT_TYPE_START );

        return CreateDirtyStartSkillEffect( dwSkillClassCode, pCharacter );


		//// 이펙트가 스크립트에 있는 경우
		//if( SkillEffectInfo* pSkillEffectInfo = SkillEffectInfoParser::Instance()->GetSkillEffectInfo( (SLOTCODE)dwSkillClassCode ) )
		//{
		//	if( IsEnableWeapon( pCharacter, pSkillEffectInfo ) == FALSE )
		//		return 0;

		//	return AddSkillEffectProjectile( pCharacter, &pSkillEffectInfo->m_EffectInfo[eEffect_Type_Start], TRUE );
		//}
		//else //이펙트 스크립트가 없을땐 하드 코딩 된 이펙트 실행 
		//{
		//	return CreateDirtyStartSkillEffect( dwSkillClassCode, pCharacter );
		//}

		//return 0;
	}

	HANDLE CreateCastingSkillEffect(DWORD dwSkillClassCode, Character* pCharacter )
	{
		WzID EffectID = INVALID_WZID;

		switch(dwSkillClassCode)
		{
		case eSKILL_FIRE_FILA: 			EffectID=StrToWzID("C011");		break;
		case eSKILL_FIRE_SPEAR:			EffectID=StrToWzID("C021");		break;
		case eSKILL_PRESSING_WALL:		EffectID=StrToWzID("C010");		break;
		case eSKILL_LIGHTING_WAVE:		EffectID=StrToWzID("C013");		break;
		case eSKILL_SONY_VINE:			EffectID=StrToWzID("C024");		break;
		case eSKILL_POISON_RAIN:		EffectID=StrToWzID("C014");		break;
		case eSKILL_MAGIC_SHIELD:		EffectID=StrToWzID("C024");		break;
		case eSKILL_REVIVAL:			EffectID=StrToWzID("C024");		break;
		case eSKILL_REVERSE:			EffectID=StrToWzID("C024");		break;
		}

		if( EffectID == INVALID_WZID )
			return INVALID_HANDLE_VALUE;

		WzVector pos = pCharacter->GetVisiblePos();
		pos.z += 0.25f;

		return GlobalFunc::CreateEffectBeUpToOption( EffectID, pos, pCharacter->GetScale(), pCharacter->GetObjectKey() );
	}

	HANDLE CreateCastingSkillBottomEffect( DWORD dwSkillClassCode, Character* pCharacter )
	{
		WzID EffectID = INVALID_WZID;

		switch(dwSkillClassCode)
		{
		case eSKILL_FIRE_ARROW:			EffectID=StrToWzID("D008");		break;
		case eSKILL_FIRE_BALL:			EffectID=StrToWzID("D008");		break;
		case eSKILL_FIRE_FILA:			EffectID=StrToWzID("D008");		break;
		case eSKILL_FIRE_SPEAR:			EffectID=StrToWzID("D008");		break;
		case eSKILL_ICE_DAGGER:			EffectID=StrToWzID("D007");		break;
		case eSKILL_ICE_DAGGER2:		EffectID=StrToWzID("D007");		break;
		case eSKILL_ICE_BLAST:			EffectID=StrToWzID("D007");		break;
		case eSKILL_ICE_BOLT:			EffectID=StrToWzID("D007");		break;
		case eSKILL_PRESSING_WALL:		EffectID=StrToWzID("D007");		break;
		case eSKILL_FROZON_SHIELD:		EffectID=StrToWzID("D007");		break;
		case eSKILL_WIND_BOLT:			EffectID=StrToWzID("D010");		break;
		case eSKILL_LIGHTING:			EffectID=StrToWzID("D010");		break;
		case eSKILL_ELECTRIC_FIELD:		EffectID=StrToWzID("D010");		break;
		case eSKILL_WIND_SHIELD:		EffectID=StrToWzID("D010");		break;
		case eSKILL_LIGHTING_WAVE:		EffectID=StrToWzID("D010");		break;
		case eSKILL_POISON_THORN:		EffectID=StrToWzID("D011");		break;
		case eSKILL_SONY_VINE:			EffectID=StrToWzID("D011");		break;
		case eSKILL_POISON_RAIN:		EffectID=StrToWzID("D011");		break;
		case eSKILL_POWER_WAVE:			EffectID=StrToWzID("D011");		break;
		case eSKILL_MAGIC_SHIELD:		EffectID=StrToWzID("D011");		break;
		}

		if( EffectID == INVALID_WZID )
			return INVALID_HANDLE_VALUE;

		return GlobalFunc::CreateEffectBeUpToOption( EffectID, pCharacter->GetPosition(), pCharacter->GetScale(), pCharacter->GetObjectKey() );
	}



	void CreateHandEffect( Object* pOwner, DWORD dwId, WzID effectID, const WzVector& vPos )
	{
		WzVector vTarget = pOwner->GetVisiblePos() + pOwner->GetDirection()*2.0f;
		vTarget.z = vPos.z;


		//손앞에 터지기
		HANDLE handle;
		Character*	pCharacter = (Character*)pOwner;

		handle = GlobalFunc::CreateEffectBeUpToOption(effectID,vPos,pCharacter->GetScale(),dwId);

		WzVector vDest,vTemp;
		float fArrowAngleZ,fArrowAngleX;

		vDest = vTarget - vPos;
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

		fArrowAngleX = -atan2(vDest.z,fLength);
		fArrowAngleZ = Math_GetAngleFromVector(&vDest);

		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(handle);
		if(pBody)
		{
			WzVector vRot;
			vRot.x = fArrowAngleX;
			vRot.y = 0.0f;
			vRot.z = fArrowAngleZ;

			WzVector vScale;
			vScale.x = 1.0f;
			vScale.y = 1.0f;
			vScale.z = 1.0f;

			pBody->SetResultMatrix(vPos,vRot,vScale);
		}
	}


    void _CalcDirectionRotate( const WzVector& StartPosition, const WzVector& TargetPosition, OUT WzVector& Rotate )
    {
        WzVector vDest = TargetPosition - StartPosition;

        float fLength;

        if ( vDest.x == 0.0f && vDest.y == 0.0f )
        {
            fLength = 0.0f;
        }
        else
        {
            WzVector vTemp = vDest;
            vTemp.z = 0.0f;

            fLength = VectorLength(&vTemp);
        }

        float fArrowAngleX = -atan2(vDest.z,fLength);
        float fArrowAngleZ = Math_GetAngleFromVector(&vDest);

        Rotate.x = fArrowAngleX;
        Rotate.y = 0.0f;
        Rotate.z = fArrowAngleZ;
    }

    void CreateCameraViberation( const cSkillEventEffectInfoData* data, const WzVector& Position )
    {
        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetTargetPosition( Position );
        pProjectile->SetLifeTime( data->GetLifeTime() );
        pProjectile->SetActiveDelayTime( data->GetDelayTime() );
        pProjectile->SetHitData( 0, 0, 0, TRUE, 1, FALSE );
        pProjectile->SetCameraShakeType( data->GetCameraViberationType(), data->GetCameraViberationTime(), data->GetCameraViberationPower() );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }

    void CreateFieldEffect( const cSkillEventEffectInfoData* data, const WzVector& Position )
    {
        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetTargetPosition( Position );
        //pProjectile->SetEffectScale( EffectScale );
        pProjectile->SetLifeTime( data->GetLifeTime() );
        pProjectile->SetActiveDelayTime( data->GetDelayTime() );
        pProjectile->CreateEffect( data->GetEventCode(), 0, 0 );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }

    void CreateTargetEffect( const cSkillEventEffectInfoData* data, Character* pCharacter )
    {
        Character* pTarget = (Character*)pCharacter->GetTargetObject();

        WzVector EffectScale = c_UnitVector;
        if( data->GetPositionType() == cSkillEventEffectInfoData::SCALE_TYPE_USER )
            EffectScale = pCharacter->GetScale();
        else if( data->GetPositionType() == cSkillEventEffectInfoData::SCALE_TYPE_TARGET )
            EffectScale = pTarget != NULL ? pTarget->GetScale() : c_UnitVector;

        if( data->GetPositionType() == cSkillEventEffectInfoData::POSITION_TYPE_TARGET_ARROW )
        {
            if( pTarget != NULL )
            {
                if( data->IsChaseTarget() == TRUE )
                {
                    cTargetPosition Position;
                    Position.SetTargetArrowPosition( pTarget->GetObjectKey(), TRUE );
                    Position.SetSyncTargetAngle( data->IsStaticRotate() == FALSE );

                    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                    pProjectile->SetPositionData( Position );
                    pProjectile->SetEffectScale( EffectScale );
                    pProjectile->SetLifeTime( data->GetLifeTime() );
                    pProjectile->SetActiveDelayTime( data->GetDelayTime() );
                    pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                    g_ProjectileManagerEx.InsertProjectile( pProjectile );
                }
                else
                {
                    WzVector HitPosition = pTarget->GetArrowTargetPos();
                    WzVector HitRotate;
                    _CalcDirectionRotate( pCharacter->GetArrowTargetPos(), HitPosition, HitRotate );

                    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                    pProjectile->SetTargetPosition( HitPosition );
                    pProjectile->SetEffectRotate( HitRotate );
                    pProjectile->SetEffectScale( EffectScale );
                    pProjectile->SetLifeTime( data->GetLifeTime() );
                    pProjectile->SetActiveDelayTime( data->GetDelayTime() );
                    pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                    g_ProjectileManagerEx.InsertProjectile( pProjectile );
                }
            }
        }
        else if( data->GetPositionType() == cSkillEventEffectInfoData::POSITION_TYPE_TARGET_POSITION )
        {
            if( pTarget != NULL )
            {
                if( data->IsChaseTarget() == TRUE )
                {
                    cTargetPosition Position;
                    Position.SetTargetVisiblePosition( pTarget->GetObjectKey(), TRUE );
                    Position.SetSyncTargetAngle( data->IsStaticRotate() == FALSE );

                    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                    pProjectile->SetPositionData( Position );
                    pProjectile->SetEffectScale( EffectScale );
                    pProjectile->SetLifeTime( data->GetLifeTime() );
                    pProjectile->SetActiveDelayTime( data->GetDelayTime() );
                    pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                    g_ProjectileManagerEx.InsertProjectile( pProjectile );
                }
                else
                {
                    WzVector HitPosition = pTarget->GetVisiblePos();
                    WzVector HitRotate;
                    _CalcDirectionRotate( pCharacter->GetVisiblePos(), HitPosition, HitRotate );

                    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                    pProjectile->SetTargetPosition( HitPosition );
                    pProjectile->SetEffectRotate( HitRotate );
                    pProjectile->SetEffectScale( EffectScale );
                    pProjectile->SetLifeTime( data->GetLifeTime() );
                    pProjectile->SetActiveDelayTime( data->GetDelayTime() );
                    pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
                    g_ProjectileManagerEx.InsertProjectile( pProjectile );
                }
            }
        }
        else if( data->GetPositionType() == cSkillEventEffectInfoData::POSITION_TYPE_USER_POSITION )
        {
            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), data->IsChaseTarget() );
            Position.SetSyncTargetAngle( data->IsStaticRotate() == FALSE );

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetPositionData( Position );
            pProjectile->SetEffectScale( EffectScale );
            pProjectile->SetLifeTime( data->GetLifeTime() );
            pProjectile->SetActiveDelayTime( data->GetDelayTime() );
            pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), pTarget != NULL ? pTarget->GetObjectKey() : 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
        else if( data->GetPositionType() == cSkillEventEffectInfoData::POSITION_TYPE_USER_ARROW_POSITION )
        {
            cTargetPosition Position;
            Position.SetTargetArrowPosition( pCharacter->GetObjectKey(), data->IsChaseTarget() );
            Position.SetSyncTargetAngle( data->IsStaticRotate() == FALSE );

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetPositionData( Position );
            pProjectile->SetEffectScale( EffectScale );
            pProjectile->SetLifeTime( data->GetLifeTime() );
            pProjectile->SetActiveDelayTime( data->GetDelayTime() );
            pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), pTarget != NULL ? pTarget->GetObjectKey() : 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
        else if( data->GetPositionType() == cSkillEventEffectInfoData::POSITION_TYPE_CUSTOM )
        {
            cTargetPosition Position;
            Position.SetTargetBonePosition( pCharacter->GetObjectKey(), data->GetPositionData().GetPositionType(), data->GetPositionData().GetBoneIndex(), data->IsChaseTarget() );
            Position.SetSyncTargetAngle( data->IsStaticRotate() == FALSE );

            cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
            pProjectile->SetPositionData( Position );
            pProjectile->SetEffectScale( EffectScale );
            pProjectile->SetLifeTime( data->GetLifeTime() );
            pProjectile->SetActiveDelayTime( data->GetDelayTime() );
            pProjectile->CreateEffect( data->GetEventCode(), pCharacter->GetObjectKey(), 0 );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
    }

    void CreateSkillEventEffectByFieldSkill( DWORD dwSkillClassCode, const WzVector& Position )
    {
        if( dwSkillClassCode == INVALID_SKILL_CLASS_CODE )
            return;

        if( cSkillEventEffectInfo* pInfo = cSkillEventEffectInfoParser::Instance()->GetInfo( dwSkillClassCode ) )
        {
            for( size_t i=0;i!=pInfo->GetEventDataCount();++i)
            {
                const cSkillEventEffectInfoData* data = pInfo->GetEventData(i);
                if( data->GetEventType() == cSkillEventEffectInfoData::EVENT_TYPE_FIELD  )
                {
                    if( data->HasCameraViberation() )
                    {
                        CreateCameraViberation( data, Position );
                    }
                    else
                    {
                        CreateFieldEffect( data, Position );
                    }
                }
            }
        }
    }
    void ProcessSkillProjectile( Character* pCharacter, DWORD dwSkillClassCode, SkillScriptInfo* pSkillInfo, int HitCount )
    {
        if( pCharacter == NULL )
            return;

        if( dwSkillClassCode == INVALID_SKILL_CLASS_CODE )
            return;


#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        if( dwSkillClassCode == eSKILL_HELLROID_1017 )  // 에테르차징
        {
            if( HitCount == 1 )
            {
                if( cProjectileUnit* pProjectile = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, (Character*)pCharacter->GetTargetObject(), pSkillInfo ) )
                {
                    pProjectile->SetAddNextAttackSerial(TRUE);
                }
            }
        }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

		if( dwSkillClassCode == eSKILL_MYSTIC_919 || dwSkillClassCode == eSKILL_MYSTIC_920 )
		{
			if( HitCount == 1 )
			{
				if( cProjectileUnit* pProjectile = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, (Character*)pCharacter->GetTargetObject(), pSkillInfo ) )
				{
					pProjectile->SetAddNextAttackSerial(TRUE);
				}
			}
        }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if( dwSkillClassCode == eSKILL_WITCHBLADE_1100 || dwSkillClassCode == eSKILL_WITCHBLADE_1119 || dwSkillClassCode == eSKILL_WITCHBLADE_1125 )  // 호라이즌잭, 다이브헬팡, 프로스트브레스
        {
            if( HitCount == 1 )
            {
                if( cProjectileUnit* pProjectile = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, (Character*)pCharacter->GetTargetObject(), pSkillInfo ) )
                {
                    pProjectile->SetAddNextAttackSerial(TRUE);
                }
            }
        }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
    
    void CreateSkillEventEffectByAnimationEvent( DWORD dwSkillClassCode, Character* pCharacter, int HitCount )
    {
        if( pCharacter == NULL )
            return;

        if( dwSkillClassCode == INVALID_SKILL_CLASS_CODE )
            return;

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
        if (pCharacter && pCharacter->GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE)
        {
            // 하이드 상태면 안띄움
            return;
        }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

        if( cSkillEventEffectInfo* pInfo = cSkillEventEffectInfoParser::Instance()->GetInfo( dwSkillClassCode ) )
        {
            for( size_t i=0;i!=pInfo->GetEventDataCount();++i)
            {
                const cSkillEventEffectInfoData* data = pInfo->GetEventData(i);
                if( data->GetEventType() != cSkillEventEffectInfoData::EVENT_TYPE_ANIMATION_EVENT )
                    continue;

                if( HitCount == 0 )
                    continue;

                if( data->GetHitEventNumber() != HitCount )
                    continue;


                if( data->HasCameraViberation() == TRUE )
                {
                    CreateCameraViberation( data, pCharacter->GetVisiblePos() );
                }
                else
                {
                    CreateTargetEffect( data, pCharacter );
                }

            }
        }
    }


    void CreateSkillEventEffect( DWORD dwSkillClassCode, Character* pCharacter, cSkillEventEffectInfoData::eEventType EventType )
    {
        if( pCharacter == NULL )
            return;

        if( dwSkillClassCode == INVALID_SKILL_CLASS_CODE )
            return;

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
        if (pCharacter && pCharacter->GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE)
        {
            // 하이드 상태면 안띄움
            return;
        }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

        if( cSkillEventEffectInfo* pInfo = cSkillEventEffectInfoParser::Instance()->GetInfo( dwSkillClassCode ) )
        {
            for( size_t i=0;i!=pInfo->GetEventDataCount();++i)
            {
                const cSkillEventEffectInfoData* data = pInfo->GetEventData(i);
                if( data->GetEventType() == EventType )
                {
                    if( data->HasCameraViberation() == TRUE )
                    {
                        CreateCameraViberation( data, pCharacter->GetVisiblePos() );
                    }
                    else
                    {
                        CreateTargetEffect( data, pCharacter );
                    }

                }
            }
        }
    }

    void ProcessSkillHitEffect( Character* pCharacter, DWORD dwSkillClassCode, int HitCount )
    {
        CreateSkillEventEffectByAnimationEvent( dwSkillClassCode, pCharacter, HitCount);
    }




	BOOL ProcessSkillEffect( Character* pCharacter, DWORD dwSkillClassCode, int HitCount )
	{
		switch ( dwSkillClassCode )
		{
		case eSKILL_UPPER_BLOW:
			if(HitCount == 1 )
			{
				WzVector wvEffectPos = pCharacter->GetVisiblePos() + pCharacter->GetDirection()*2;
				GlobalFunc::CreateEffectBeUpToOption( StrToWzID("A012"), wvEffectPos, pCharacter->GetScale(), pCharacter->GetObjectKey() );

				return TRUE;
			}
			break;

		case eSKILL_SONIC_EXPLOSION:
			AddCameraVibration( pCharacter, 50.f );
			return TRUE;


		case eSKILL_HEAL_MAGIC_ARRAY:
			if(HitCount == 1 )
			{
				GlobalFunc::CreateEffectBeUpToOption( StrToWzID("k421"), pCharacter->GetVisiblePos(), pCharacter->GetScale(), pCharacter->GetObjectKey());//!loop 이펙트 쓰지 말것    
				return TRUE;
			}
			break;

		case eSKILL_FIST_OF_FIRE:
			if( HitCount == 1 )
			{
				if( Object* pTargetObject = pCharacter->GetTargetObject() )
				{
					GlobalFunc::CreateEffectBeUpToOption( StrToWzID("H646"), pTargetObject->GetVisiblePos(), pCharacter->GetScale(), pCharacter->GetObjectKey());//!loop 이펙트 쓰지 말것    
					return TRUE;
				}
			}
			break;

		case eSKILL_GROUND_SHOCK:
			AddCameraVibration( pCharacter, 60.f, Camera::VIBE_TYPE_VERTICAL, 0.1f );
			return TRUE;

		case eSKILL_KNIGHT_OF_HUNGRY_SLAYER_THUNDER_CLAP:
			if( HitCount == 1 )
			{
				AddCameraVibration( pCharacter, 60.f, Camera::VIBE_TYPE_ALL, 0.1f );
				return TRUE;
			}
			break;

		case eSKILL_EARTHQUAKE:
			AddCameraVibration( pCharacter, 500.f );
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K350"), pCharacter->GetCurrentAction()->SKILL.vTargePos, pCharacter->GetObjectKey());
			return TRUE;
		}

		return FALSE;
	}


	void CreateFootEffect( DWORD dwObjectKey, const WzVector& vPosition, DWORD dwTerrainAttribute)
	{
		// 지형속성에 따라 이펙트를 뿌려준다
		switch(dwTerrainAttribute)
		{
		case eTERRAIN_ATTRIBUTE_WOOD:
		case eTERRAIN_ATTRIBUTE_MUD:
		case eTERRAIN_ATTRIBUTE_METAL:
		case eTERRAIN_ATTRIBUTE_STONE:
		case eTERRAIN_ATTRIBUTE_DIRT:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPosition,dwObjectKey);
			}
			break;
		case eTERRAIN_ATTRIBUTE_WATER:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A002"),vPosition,dwObjectKey);
			}
			break;

		case eTERRAIN_ATTRIBUTE_SNOW:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPosition,dwObjectKey);
			}
			break;
		case eTERRAIN_ATTRIBUTE_DESERT:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPosition,dwObjectKey);
			}
			break;

		default:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPosition,dwObjectKey);
			}
			break;
		}
	}

}