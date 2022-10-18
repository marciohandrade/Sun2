//------------------------------------------------------------------------------
//  SkillStateMonsterSkill.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateMonsterSkill.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"


#include "SkillInfoParser.h"
#include "ProjectileInfoParser.h"
#include "Clone.h"
#include "CloneManager.h"
#include "NumericValues.h"
#include "Map.h"
#include "Camera.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#define INT_DUST_TIME 50
#define INT_CLONE_TIME 700


//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterSkill::SkillStateMonsterSkill()
{
}

//------------------------------------------------------------------------------
/**
*/
SkillStateMonsterSkill::~SkillStateMonsterSkill()
{
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMonsterSkill::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;
	//m_dwSkillCode = pCharacter->GetCurrentAction()->SKILL.dwSkillID;

	STATECHANGE::TYPE rt=StateSkill::OnStart(pOwner,CurTime);
	m_iHealCount = 0;

	BASE_SKILLINFO *pInfo = GetSkillInfo();
	assert(pInfo && "적용할v수없는 스킬의 결과입니다");

	assert(pCharacter->IsKindOfObject(MONSTER_OBJECT));
	
	SetDirection(pOwner);

	m_hEffectHandle=INVALID_HANDLE_VALUE;

	InitSkillForType(pOwner);
	m_dwDustTime=INT_DUST_TIME;
	m_dwCopyTime=INT_DUST_TIME;
	m_dwCurTime=0;

	return rt;
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::SetDirection(Object* pOwner)
{
	BASE_SKILLINFO *pInfo = GetSkillInfo();
	assert(pInfo && "적용할수없는 스킬의 결과입니다");
	Character*	pCharacter = (Character*)pOwner;

	assert(pCharacter->IsKindOfObject(MONSTER_OBJECT));

	if(pInfo&&pInfo->m_byTarget==SKILL_TARGET_ENEMY)
	{
		Character *pTarget = (Character *)pCharacter->GetTargetObject();

		if (pTarget)
		{
			pCharacter->SetDirection(pTarget->GetPosition());
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateMonsterSkill::InitSkillForType(Object* pOwner)
{
	Character*	pCharacter = (Character*)pOwner;

	BASE_SKILLINFO *pInfo = GetSkillInfo();
	assert(pInfo && "적용할수없는 스킬의 결과입니다");

	if(pInfo==NULL)
		return FALSE;

	switch (pInfo->m_SkillClassCode)
	{
	case eSKILL_BEAST_MASTER_DEADLY_STRIKE:
		{
			if(pCharacter)
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L001"),pCharacter->GetVisiblePos(),pCharacter->GetObjectKey());	
			}
		}
		break;

	case eSKILL_PATIENCE:
		{
            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );

            g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("J540"), 10000, Position );
		}
		break;

	case eSKILL_BATTLE_BARRIOR:
		{
            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );

            g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("J550"), 10000, Position );
		}
		break;

	case eSKILL_RESSURECTION:
		{
            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );

            g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("J560"), 10000, Position );
		}
		break;

	case eSKILL_FLAME_RULER_EXPLORZEN_KNOCKLE:
		{
			if(pCharacter)
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("I010"),pCharacter->GetVisiblePos(),pCharacter->GetObjectKey());	
			}
		}
		break;

	case eSKILL_MASTER_OF_CASTLE_ICE_MISSILE:
		{
			if(pCharacter)
			{

                WzVector vSrc = pCharacter->GetArrowTargetPos();
                WzVector vRot = c_ZeroVector;
                vRot.z = pCharacter->GetAngle();
                vRot.x = pCharacter->GetAngleX();


                cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
                pProjectile->SetTargetPosition( vSrc );
                pProjectile->SetLifeTime( 1666 );
                pProjectile->SetEffectRotate( vRot );
                pProjectile->CreateEffect( StrToWzID("L016"), pCharacter->GetObjectKey(), 0 );
                g_ProjectileManagerEx.InsertProjectile( pProjectile );
           

				//PROJECTILE_PARAM param;
				//ZeroMemory(&param,sizeof(param));

				//param.fHeight = 0.0f;
				//param.vStartPos =vSrc;		
				//param.vTargetPos=vSrc;
				//param.dwLifeTime = 5000;
				//param.dwMoveTime = 5000;		
				//param.dwDamageTime=0;
				//param.vScale.x=1.0f;
				//param.vScale.y=1.0f;
				//param.vScale.z=1.0f;

				//param.vDir.z=pCharacter->GetAngle();
				//param.vDir.x=pCharacter->GetAngleX();

				//param.fSpeed = 3.0f;
				//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
				//param.dwAttackSerial = 0;
				//param.dwOwner = pCharacter->GetObjectKey();
				//param.dwImageCode = 0;
				//param.effectID = StrToWzID("L016");
				//param.bAddNextSerial=FALSE;
				//param.bAllDamage=FALSE;
				//g_ProjectileManager.CreateProjectile(param);

			}
		}
		break;

	case eSKILL_FLAME_BEAST_TACKLE:	
	case eSKILL_FELLEN_WARRIOR_DASH_ATTACK:
		{
			PLAYER_ACTION *pAction = (PLAYER_ACTION *)pCharacter->GetCurrentAction();

			if(GENERALPARAM->IsNet()==FALSE)
			{

				pAction->SKILL.vCurPos.wvPos=pCharacter->GetVisiblePos();
				pAction->SKILL.vDestPos.wvPos=g_pHero->GetVisiblePos();
			}


			Character *pTarget = (Character *)pCharacter->GetTargetObject();
			if (pTarget)
			{
				pCharacter->SetDirection(pAction->SKILL.vDestPos.wvPos, TRUE);
			}

		}
		break;

	case eSKILL_CURSED_PRIEST_ICE_STORM:
		{
			if(pCharacter)
			{
				WzVector vPos=pCharacter->GetVisiblePos();

				vPos.z+=4.0f;
				m_hEffectHandle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L008"),vPos,pCharacter->GetObjectKey());	
			}
		}
		break;

	case eSKILL_MASTER_OF_CASTLE_FROST_RING:
		{
			Character *pTarget = (Character *)pCharacter->GetTargetObject();

			if(pTarget)
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L014"),pTarget->GetVisiblePos(),pTarget->GetObjectKey());

			}
			else
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L014"),pCharacter->GetVisiblePos(),pCharacter->GetObjectKey());

			}

		}
		break;


	case eSKILL_CURSED_PRIEST_ICE_SPRAY:
		{
			if(pCharacter)
			{
				WzVector vPos=pCharacter->GetBonePos(8);

				vPos.z-=1.0f;


				m_hEffectHandle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L007"),vPos,pCharacter->GetObjectKey());	

				if(m_hEffectHandle!=INVALID_HANDLE_VALUE)
				{
					CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffectHandle);
					if(pBody)
					{
						WzVector vRot;
						vRot.x = pCharacter->GetAngleX();
						vRot.y = 0.0f;
						vRot.z =pCharacter->GetAngle();

						WzVector vScale;
						vScale.x = 1.0f;
						vScale.y = 1.0f;
						vScale.z = 1.0f;

						WzVector vPos=pCharacter->GetBonePos(8);

						vPos.z-=1.0f;

						pBody->SetResultMatrix(pCharacter->GetBonePos(8),vRot,vScale);
					}
				}
			}
		}
		break;

	case eSKILL_FIRE_CERBERUS_FIRE_BREATH:
		{
			if( pCharacter )
			{
				m_hEffectHandle = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L487") , pCharacter->GetVisiblePos() );
			}
		}
		break;
	}



	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateMonsterSkill::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	BASE_SKILLINFO *pInfo = GetSkillInfo();
		
	if(pInfo)
	{
		switch (pInfo->m_SkillClassCode)
		{
		case eSKILL_FLAME_BEAST_TACKLE:
			{
				return OnTackle(pOwner, CurTime,dwTick);
			}
			break;
		}
	}

	EnterProcess(pOwner,CurTime,dwTick);
	m_dwCurTime+=dwTick;
	CheckEvent();

	assert(pInfo && "적용할수없는 스킬의 결과입니다");

	switch (pInfo->m_SkillClassCode)
	{
	
	case eSKILL_BEAST_MASTER_DEADLY_STRIKE:
		{

			OnBeastMasterDeadlyStrikeProcess(pOwner,CurTime,dwTick);
		}
		break;

	case eSKILL_SLAVE_ARCHER_RECOVERY_SHOT:
		{
			
		}
		break;

	case eSKILL_VOLCANO_BEAST_UPPER_BLOW:
	case eSKILL_SNOW_DRAGON_ZOMBIE_UPPER_BLOW:
	case eSKILL_DRAGON_ZOMBIE_UPPER_BLOW:
		{
			SetDirection(pOwner);

		    OnDragonZombieUpperBlow(pOwner, CurTime,dwTick);

		}
		break;

	case eSKILL_EVIL_SORCERER_FIRE_BALL:     
		{
			SetDirection(pOwner);

			OnEvilSorcererFireBall(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_EVIL_SORCERER_PRESSING_WALL: 
		{
			SetDirection(pOwner);

			OnEvilSorcererPressingWall(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_DRAGON_ZOMBIE_SOUL_OF_DRAGON:
		{
			SetDirection(pOwner);

			OnDragonZombieSoulOfDragon(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_REDAMOR_WARRIOR_SONIC_BLADE:  
		{
			SetDirection(pOwner);

			OnRedamorWarriorSonicBlade(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_HEAVY_HUNTER_SNIPING:
		{
			SetDirection(pOwner);
			OnHeavyHunterSnipiing(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_HEAVY_NOIZE_SHOT:		
		{
			SetDirection(pOwner);

			OnHeavyNoiseShot(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_HEAVY_WHISTLE_SHOT:	
		{
			SetDirection(pOwner);
			OnHeavyWhistleShot(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_SLAVE_ARCHER_HEAVY_SHOT:     
		{
			SetDirection(pOwner);
			OnSlaveArcherHeavyShot(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_DARK_F_INFANTRY_BASH:
	case eSKILL_REDAMOR_WARRIOR_SPIN_CRUSH:   
		{
			SetDirection(pOwner);

			if(ProcessHit(pCharacter,1)==FALSE)
			{
				
			}
			if(ProcessHit(pCharacter,2)==FALSE)
			{
				
			}
			if(ProcessHit(pCharacter,3)==FALSE)
			{

			}
		}
		break;
	case eSKILL_DARK_F_PATROL_TWIN_STRECH:
	case eSKILL_BEAST_KNIGHT_DEATH_DIVIDE:	
		{
			SetDirection(pOwner);

			ProcessHit(pCharacter,1);
			ProcessHit(pCharacter,2);
		}
		break;

	case eSKILL_SACRIFICE:
		{
			SetDirection(pOwner);

			if (CheckNotProcessedHitEvent(1))
			{

				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("J467"),pCharacter->GetVisiblePos(),pCharacter->GetObjectKey());	

	
                pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial());
                ++m_iHitProcessed;
			}
		}
		break;

	case eSKILL_FILL_OFF:
		{
			SetDirection(pOwner);

			OnPillOff(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_CURSED_PRIEST_ICE_SPRAY:
		{

			SetDirection(pOwner);

			OnCursedPriestIceSpray(pOwner, CurTime,dwTick);

		}
		break;

	case eSKILL_CURSED_PRIEST_ICE_STORM:
		{
			OnCursedPriestIceStorm(pOwner, CurTime,dwTick);

		}
		break;

	case eSKILL_FLAME_RULER_EXPLORZEN_KNOCKLE:
		{
			OnFlameRulerExplorzenKnockle(pOwner, CurTime,dwTick);
		}
		break;


	case eSKILL_MASTER_OF_CASTLE_BLIZZARD2:
	case eSKILL_MASTER_OF_CASTLE_BLIZZARD:
		{
			OnMasterOfCastleBlizzard(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_FLAME_RULER_METEO_SHOWER:
		{
			OnFrameRulerMeteoShower(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_MASTER_OF_CASTLE_FROST_RING:
		{
			OnMasterOfCastleFrostRing(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_MASTER_OF_CASTLE_ICE_MISSILE:
		{
			OnMasterOfCastleIceMissile(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_FLAME_RULER_FIRE_WALL:
		{
			OnFlameRulerFireWall(pOwner, CurTime,dwTick);
		}
		break;

	case eSKILL_FELLEN_WARRIOR_DASH_ATTACK:
		{
			return OnFellenWarrionDashAttace(pOwner, CurTime , dwTick);
		}
		break;

	case eSkILL_CURSED_ARCHER_POISON_ARROW:
		{
			OnCursedArcherPoisonArrow(pOwner , CurTime ,dwTick);
		}
		break;

	case eSKILL_DARK_WARRIOR_BEAT:
		{
			OnDarkWarriorBeat(pOwner, CurTime, dwTick);
		}
		break;
	
	case eSKILL_ARCHBISHOP_OF_DARKNESS_DOOM:
		{
			OnArchbishopOfDarknessDoom(pOwner, CurTime, dwTick);
		}
		break;
	
	case eSKILL_BINDED_ARCHER_STUN_SHOT:
		{
			OnBindedArcherStunShot(pOwner, CurTime, dwTick);
		}
		break;

	case eSKILL_FIRE_CERBERUS_FIRE_BREATH:
		{
			OnFireCerberusFireBreath(pOwner, CurTime, dwTick);
		}
		break;

	default: //타격계 6타까지 잡아 낸다.
		{
			SetDirection(pOwner);

			//발사체 코드가 있으면 
			if(pInfo->m_dwFlyingObjectCode)
			{
				OnShot(pOwner, CurTime,dwTick);
			}
			else
			{
				OnHit(pOwner, CurTime,dwTick);
			}
		}
		break;
	}



	return LeaveProcess(pOwner,CurTime,dwTick);
}
//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnHit(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	for(int i=1;i<=MAX_HIT;i++)
	{
		if (ProcessHit((Character *)pOwner,i))  
		{

			int a=0;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnShot(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character *pCharacter=(Character*)pOwner;
	Character *pTarget=(Character *)pCharacter->GetTargetObject();

    for(int i=1;i<=MAX_HIT;i++)
	{
		if (CheckNotProcessedHitEvent(i))  
		{
			++m_iHitProcessed;

			BASE_SKILLINFO *pInfo = GetSkillInfo();
			assert(pInfo);

			BASE_ProjectileInfo *pInfoProjectIile = ProjectileInfoParser::Instance()->GetProjectileInfo(pInfo->m_dwFlyingObjectCode);
			assert(pInfoProjectIile);

			if(!pInfoProjectIile)
			{
				pInfo->m_dwFlyingObjectCode=0;
				ProcessHit((Character *)pOwner,i);//스크립트가 잘못됐을땐 발사체 코드 날리고 기본 처리로 변경한다.
			}
			else
			{

				WzVector StartPosition = pCharacter->GetArrowBonePos(0);
				WzVector TargetPosition = pTarget ? pTarget->GetArrowTargetPos() : StartPosition + pCharacter->GetDirection() * 4.0f;

				WzVector Distance = TargetPosition - StartPosition;
				float fDistance = VectorLength(&Distance);

				DWORD MoveTime = 100;
				if( fDistance <= 0.0f)
				{
					MoveTime = 100;
				}
				else
				{
					MoveTime = max(100, min(500, (DWORD)(fDistance * 50.f) ) );
				}

				BOOL AddAttackSerial = FALSE;
				if(SkillHitCount(pOwner)==i) //마지막 발은 어텍시리얼을 증가 시킨당.
					AddAttackSerial = TRUE;


				cProjectileUnit* pProjectile = new cProjectileUnit();
				pProjectile->SetArrowStartPosition( StartPosition );

				if( pTarget != NULL )
                    pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
				else
					pProjectile->SetArrowTargetPosition( TargetPosition );

				pProjectile->SetLifeTime( MoveTime );
				pProjectile->CreateEffect( pInfoProjectIile->m_wzidFlyingEffectID.id, pCharacter->GetObjectKey(), pTarget ? pTarget->GetObjectKey() : 0);
				pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), pInfoProjectIile->m_wzidHitEffectID.id, FALSE, 0, AddAttackSerial );
				if( pInfoProjectIile->m_bMakeTrail == TRUE )
				{
					pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, 150, 200, 255 );
				}
				g_ProjectileManagerEx.InsertProjectile( pProjectile );


				//WzVector pos;
				//pos = pCharacter->GetArrowBonePos(0);

				//if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
				//{
				//	pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
				//}


				//PROJECTILE_PARAM param;
				//ZeroMemory(&param,sizeof(param));
				//param.fHeight = 1.0f;
				//param.vStartPos = pos;		

				//if(pTarget)
				//{
				//	param.vTargetPos = pTarget->GetVisiblePos();
				//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
				//	param.dwTargetID=pTarget->GetObjectKey();
				//}
				//else
				//{
				//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
				//}
	
				//float range=((float)pInfo->m_wSkillRange / 10.0f);
				//WzVector wzVec1 =param.vTargetPos - param.vStartPos;
				//float fLength = VectorLength(&wzVec1);
				//float rate=fLength/range;
				//float fMoveTime=(float)pInfo->m_wFlyingLifeTime;
				//fMoveTime*=rate;
				//param.dwMoveTime=(DWORD)fMoveTime;
				//param.dwMoveTime=max(100,param.dwMoveTime);

				//param.dwLifeTime = 0;
				//param.fSpeed = (float) pInfoProjectIile->m_fSpeed;

				//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
				//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
				//param.dwOwner = pCharacter->GetObjectKey();
				//param.effectID = pInfoProjectIile->m_wzidFlyingEffectID.id;
				//param.dwImageCode=pInfoProjectIile->m_dwImageID;
				//param.EndeffectID=pInfoProjectIile->m_wzidHitEffectID.id;
				//param.bAllDamage=TRUE;

				//if(SkillHitCount(pOwner)==i) //마지막 발은 어텍시리얼을 증가 시킨당.
				//{
				//	param.bAddNextSerial=TRUE;
				//}
				//else
				//{
				//	param.bAddNextSerial=FALSE;
				//}
				//g_ProjectileManager.CreateProjectile(param);

			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
 STATEPROCESS::TYPE SkillStateMonsterSkill::OnTackle(Object* pOwner, DWORD CurTime,DWORD dwTick)
{

	Character*	pCharacter = (Character*)pOwner;

	//Todo:스킬 처리 
	float fMoveSpeed = GetBaseMoveSpeedAsState(CMS_SWIPE) * (float)dwTick;

	g_pSunTerrain->PathExp_Progress(pCharacter->GetPathExplorer(), fMoveSpeed, NULL);

	const PLAYER_ACTION* pAction = pCharacter->GetCurrentAction();
	WzVector vDest=pAction->SKILL.vDestPos.wvPos;

	vDest =  vDest - pCharacter->GetPosition();
	float fLength;

	fLength = VectorLength(&vDest);

	if( fLength < 0.3f)
	{

		if(GENERALPARAM->IsNet()==FALSE)
		{
			++m_iHitProcessed;
		}
		else
		{
			if (pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial()))
			{
				++m_iHitProcessed;
			}
		}
	}



	if ( GetAnimationRate() >= 100.0f && pCharacter->IsMove() == FALSE )//에니메이션 진척도가 100%일때
	{
		return STATEPROCESS::END;
	}

	if( (DWORD)m_dwDustTime < dwTick ) 
	{
		WzVector vPos = pCharacter->GetVisiblePos();

		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A030"),vPos,pCharacter->GetObjectKey());	

		m_dwDustTime=INT_DUST_TIME;
	}
	else
	{
		m_dwDustTime-=dwTick;
	}

	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnPillOff(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character *pCharacter=(Character*)pOwner;
	Character *pTarget = (Character *)pCharacter->GetTargetObject();

#ifdef _NA_006679_20130423_1302_SKILL_RENEWAL
    if (CheckNotProcessedHitEvent(1))  
    {
        ++m_iHitProcessed;

        if( BASE_SKILLINFO* pInfo = GetSkillInfo() )
        {
            WzVector StartPosition = pCharacter->GetHandBonePos(1);//손에서 발사	
            WzVector TargetPosition;
            if( pTarget )
            {
                TargetPosition = pTarget->GetArrowTargetPos();
            }
            else
            {
                TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
            }

            DWORD MoveTime = CalcProjectileMoveTimeByRange( StartPosition, TargetPosition, ((float)pInfo->m_wSkillRange / 10.0f), 500 );

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( MoveTime );
            pProjectile->CreateEffect( StrToWzID("J500"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
    }

#else // _NA_006679_20130423_1302_SKILL_RENEWAL
    if (CheckNotProcessedHitEvent(1))  
    {
        ++m_iHitProcessed;

        if( BASE_SKILLINFO* pInfo = GetSkillInfo() )
        {
            WzVector StartPosition = pCharacter->GetHandBonePos(1);//손에서 발사	
            WzVector TargetPosition;
            if( pTarget )
            {
                TargetPosition = pTarget->GetArrowTargetPos();
            }
            else
            {
                TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
            }

            DWORD MoveTime = CalcProjectileMoveTimeByRange( StartPosition, TargetPosition, ((float)pInfo->m_wSkillRange / 10.0f), 500 );

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( MoveTime );
            pProjectile->CreateEffect( StrToWzID("J500"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
    }

    if (CheckNotProcessedHitEvent(2))  
    {
        ++m_iHitProcessed;

        if( BASE_SKILLINFO* pInfo = GetSkillInfo() )
        {
            WzVector StartPosition = pCharacter->GetHandBonePos(0);//손에서 발사	
            WzVector TargetPosition;
            if( pTarget )
            {
                TargetPosition = pTarget->GetArrowTargetPos();
            }
            else
            {
                TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
            }

            DWORD MoveTime = CalcProjectileMoveTimeByRange( StartPosition, TargetPosition, ((float)pInfo->m_wSkillRange / 10.0f), 500 );

            cProjectileUnit* pProjectile = new cProjectileUnit();
            pProjectile->SetArrowStartPosition( StartPosition );
            pProjectile->SetArrowTargetPosition( TargetPosition );
            pProjectile->SetLifeTime( MoveTime );
            pProjectile->CreateEffect( StrToWzID("J500"), pCharacter->GetObjectKey(), 0 );
            pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
    }

#endif // _NA_006679_20130423_1302_SKILL_RENEWAL

}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnHeavyHunterSnipiing(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		Character* pTarget = (Character *)pCharacter->GetTargetObject();

		WzVector StartPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);

		WzVector TargetPosition;
		if( pTarget )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}
		else
		{
			TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E021"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );


		//WzVector TargetPosition = pCharacter->GetVisiblePos() + pCharacter->GetDirection() * 2.0f;
		//TargetPosition.z = StartPosition.z;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));

		//param.fHeight = 1.0f;

		//param.vStartPos = StartPosition;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E021");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}
}


//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnHeavyNoiseShot(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		Character *pTarget = (Character *)pCharacter->GetTargetObject();


		WzVector StartPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);

		WzVector TargetPosition;
		if( pTarget )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}
		else
		{
			TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E030"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//dest.z=pos.z;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));

		//param.fHeight = 1.0f;

		//param.vStartPos = pos;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E030");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}


	if (CheckNotProcessedHitEvent(2))  
	{
		++m_iHitProcessed;


		WzVector StartPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);

		WzVector TargetPosition;
		if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}
		else
		{
			TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E030"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		//Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//dest.z=pos.z;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));

		//param.fHeight = 1.0f;

		//param.vStartPos = pos;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E030");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}

	if (CheckNotProcessedHitEvent(3))  
	{
		++m_iHitProcessed;

		WzVector StartPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);

		WzVector TargetPosition;
		if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}
		else
		{
			TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E030"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );


		//Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//dest.z=pos.z;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));

		//param.fHeight = 1.0f;

		//param.vStartPos = pos;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E030");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnHeavyWhistleShot(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;


		WzVector StartPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);

		WzVector TargetPosition;
		if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}
		else
		{
			TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E030"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		//Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//dest.z=pos.z;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 1.0f;
		//param.vStartPos = pos;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E030");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}


	if (CheckNotProcessedHitEvent(2))  
	{
		++m_iHitProcessed;


		WzVector StartPosition;
		StartPosition = pCharacter->GetArrowBonePos(0);

		WzVector TargetPosition;
		if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}
		else
		{
			TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E030"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		//Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//dest.z=pos.z;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 1.0f;
		//param.vStartPos = pos;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();

		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E030");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}

}


//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnCursedPriestIceSpray(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (ProcessHit(pCharacter,1))  
	{
	}

	if(m_hEffectHandle!=INVALID_HANDLE_VALUE)
	{
		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffectHandle);
		if(pBody)
		{
			WzVector vRot;
			vRot.x = pCharacter->GetAngleX();
			vRot.y = 0.0f;
			vRot.z =pCharacter->GetAngle();

			WzVector vScale;
			vScale.x = 1.0f;
			vScale.y = 1.0f;
			vScale.z = 1.0f;

			WzVector vPos=pCharacter->GetBonePos(8);

			vPos.z-=1.0f;

			pBody->SetResultMatrix(vPos,vRot,vScale);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnMasterOfCastleFrostRing(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnMasterOfCastleIceMissile(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		for(float i=0.0f;i<WZ_PI*2.0f;i+=WZ_PI*2.0f/20.0f)
		{
			DWORD dwLife=200+rand()%200;

			WzVector vSrc = pCharacter->GetArrowTargetPos();
			
			WzVector vDirection;

			vDirection.x = 0.0f;
			vDirection.y = (float)-cos(i);
			vDirection.z=(float)sin(i);
		
			float fAngle=pOwner->GetAngle();
			WzVector wvPos, vDest;

			WzMatrix rot;
			MatrixIdentity( &rot );
			MatrixRotationZ( &rot, fAngle + WZ_PI / 2.0f);
			VectorTransformCoord(&wvPos, &vDirection, &rot);

			WzVector tempPos;
			vDest=vSrc;
			tempPos=vDest+(wvPos)*5.0f;
			tempPos+=pCharacter->GetDirection()*1.0f;	
	
			WzVector StartPosition = vSrc;
			WzVector TargetPosition = tempPos;


			int r=rand()%10;
			float fScale=(float)r;
			fScale/=10.0f;
			{
				cProjectileUnit* pProjectile = new cProjectileUnit();
				pProjectile->SetArrowStartPosition( StartPosition );
				pProjectile->SetArrowTargetPosition( TargetPosition );
				pProjectile->SetLifeTime( 200 );
				pProjectile->SetEffectScale( fScale + 1.0f );
				pProjectile->CreateEffect( StrToWzID("L013"), pCharacter->GetObjectKey(), 0 );
				g_ProjectileManagerEx.InsertProjectile( pProjectile );
			}


			//PROJECTILE_PARAM param;

			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.0f;
			//param.vStartPos = vSrc;		
			//param.vTargetPos = tempPos;



			//param.dwLifeTime = dwLife;
			//param.dwMoveTime = 200;							
			//param.fSpeed = 1.0f;
			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//param.dwAttackSerial = 0;
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwImageCode = 0;
			//param.effectID = StrToWzID("L013");
			//param.bAddNextSerial=FALSE;
			//param.bAllDamage=FALSE;
			//param.dwActiveDelayTime=0;
		
			//int r=rand()%10;
			//float fScale=(float)r;
			//fScale/=10.0f;

			//param.vScale.x=1.0f;
			//param.vScale.y=1.0f;
			//param.vScale.z=1.0f;

			//param.vScale.x+=fScale;
			//param.vScale.y+=fScale;
			//param.vScale.z+=fScale;

			//g_ProjectileManager.CreateProjectile(param);
	
			////
			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.0f;
			tempPos = vDest+(wvPos)*5.0f;
			tempPos += pCharacter->GetDirection()*1.0f;
			//param.vStartPos=tempPos;

			StartPosition = tempPos;

			WzVector BezierPosition0, BezierPosition1;

			tempPos=vDest+(wvPos)*3.5f;
			tempPos+=pCharacter->GetDirection()*6.0f;
			tempPos.z-=1.0f;

			//param.BEZIER.vPos0=tempPos;
			BezierPosition0 = tempPos;

			tempPos=vDest+(wvPos)*2.5f;
			tempPos+=pCharacter->GetDirection()*12.0f;
			tempPos.z-=2.0f;

			//param.BEZIER.vPos1 = tempPos;
			BezierPosition1 = tempPos;

			tempPos=vDest+(wvPos)*1.0f;
			tempPos+=pCharacter->GetDirection()*20.0f;
			//tempPos.z=pCharacter->GetVisiblePos().z;
			tempPos.z-=3.0f;

			//param.vTargetPos = tempPos;
			TargetPosition = tempPos;

			{
				cProjectileBezierUnit* pProjectile = new cProjectileBezierUnit();
				pProjectile->SetBezierCourse( StartPosition, TargetPosition, BezierPosition0, BezierPosition1 );
				pProjectile->SetLifeTime( 500+rand()%500 );
				pProjectile->CreateEffect( StrToWzID("E011"), pCharacter->GetObjectKey(), 0 );
				pProjectile->SetEffectScale( fScale + 1.0f );
				pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, FALSE );

				pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, 150, 200, 255 );

				if(i>=WZ_PI*2.0f-WZ_PI*2.0f/20.0f)
				{
					pProjectile->SetAddNextAttackSerial( TRUE );
					//param.bAllDamage=TRUE;
					//param.bAddNextSerial=TRUE;
				}

				g_ProjectileManagerEx.InsertProjectile( pProjectile );
			}

			//param.dwLifeTime = 0;
			//param.dwMoveTime = 500+rand()%500;							
			//param.fSpeed = 3.0f;
			//param.dwType=PROJECTILE::PROJECTILEID_BEZIER;
			//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwDamageTime=100;
			//param.dwImageCode = 0;
			//param.effectID = StrToWzID("E011");

			//param.bAddNextSerial=FALSE;
			//param.bAllDamage=FALSE;

			//if(i>=WZ_PI*2.0f-WZ_PI*2.0f/20.0f)
			//{
			//	param.bAllDamage=TRUE;
			//	param.bAddNextSerial=TRUE;
			//}

			//param.vScale.x=1.0f;
			//param.vScale.y=1.0f;
			//param.vScale.z=1.0f;

			//param.vScale.x+=fScale;
			//param.vScale.y+=fScale;
			//param.vScale.z+=fScale;
			//param.dwActiveDelayTime=dwLife;

			//param.bR = 150;
			//param.bG = 200;
			//param.bB = 255;
			//param.bMakeTrail = TRUE;
			//param.fTrailLength = 0.8f;
			//param.fHeight = 0.04f;

			//g_ProjectileManager.CreateProjectile(param);

		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnCursedPriestIceStorm(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		bool bShake=false;

		//
		if(g_pHero)
		{
			WzVector vDest=g_pHero->GetPosition();
			vDest =  vDest - pCharacter->GetPosition();
			float fLength;
			fLength = VectorLength(&vDest);

			if(fLength<=10.0f)
				bShake=true;
		}

		WzVector StartPosition = pOwner->GetVisiblePos();
		StartPosition.z += 10.0f;
		WzVector TargetPosition = pOwner->GetVisiblePos() + pOwner->GetDirection() * 4.0f;
		WzVector vResult;
		g_pMap->GetTerrainHeight( &TargetPosition, &vResult );
		TargetPosition = vResult;

		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 333 );
		pProjectile->CreateEffect( StrToWzID("L009"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), StrToWzID("L010"), bShake, 0, TRUE );
		pProjectile->SetActiveDelayTime( 500 );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );




		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.0f;

		//param.vStartPos =pOwner->GetVisiblePos();		
		//param.vStartPos.z+=10.0f;

		//param.vTargetPos =pOwner->GetVisiblePos()+pOwner->GetDirection()*4.0f;
		//WzVector vResult;
		//g_pMap->GetTerrainHeight(&param.vTargetPos,&vResult);
		//param.dwActiveDelayTime=500;
		//param.vTargetPos=vResult;
		//param.dwLifeTime =0;
		//param.dwMoveTime = 1000;
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//

		//param.effectID =  StrToWzID("L009");
		//param.dwImageCode=0;
		//param.EndeffectID= StrToWzID("L010");	
		//param.bAllDamage=TRUE;
		//param.bAddNextSerial=TRUE;
		//param.bCameraShake=bShake;

		//g_ProjectileManager.CreateProjectile(param);
	}

}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnFlameRulerExplorzenKnockle(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		Character* pTarget = (Character *)pCharacter->GetTargetObject();

		WzVector TargetPosition;

		{
			WzVector temp = pCharacter->GetVisiblePos() + pCharacter->GetDirection()*5.0f;
			g_pMap->GetTerrainHeight( &temp, &TargetPosition );

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->SetLifeTime( 1333 );
			pProjectile->CreateEffect( StrToWzID("L301"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, TRUE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
		{
			WzVector temp = pCharacter->GetVisiblePos() + pCharacter->GetDirection()*9.0f;
			g_pMap->GetTerrainHeight( &temp, &TargetPosition );

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->SetLifeTime( 1333 );
			pProjectile->SetActiveDelayTime(500);
			pProjectile->CreateEffect( StrToWzID("L301"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, TRUE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
		{
			WzVector temp = pCharacter->GetVisiblePos() + pCharacter->GetDirection()*13.0f;
			g_pMap->GetTerrainHeight( &temp, &TargetPosition );

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->SetLifeTime( 1333 );
			pProjectile->SetActiveDelayTime(1000);
			pProjectile->CreateEffect( StrToWzID("L301"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, TRUE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
		{
			WzVector temp = pCharacter->GetVisiblePos() + pCharacter->GetDirection()*17.0f;
			g_pMap->GetTerrainHeight( &temp, &TargetPosition );

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->SetLifeTime( 1333 );
			pProjectile->SetActiveDelayTime(1500);
			pProjectile->CreateEffect( StrToWzID("L301"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, TRUE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
	}


	//	WzVector pos;
	//	pos = pCharacter->GetArrowBonePos(0);
	//
	//	PROJECTILE_PARAM param;
	//	ZeroMemory(&param,sizeof(param));
	//
	//	pos=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*5.0f;

	//	WzVector vResult;
	//	g_pMap->GetTerrainHeight(&pos,&vResult);

	//	param.fHeight = 0.0f;
	//	param.vStartPos =vResult;		
	//	param.vTargetPos=vResult;
	//	param.dwLifeTime = 4000;
	//	param.dwMoveTime = 4000;		
	//	param.dwDamageTime=100;

	//	param.fSpeed = 3.0f;
	//	param.dwType=PROJECTILE::PROJECTILEID_COMMON;
	//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
	//	param.dwOwner = pCharacter->GetObjectKey();
	//	param.dwImageCode = 0;
	//	param.effectID = StrToWzID("L301");
	//	param.bAddNextSerial=FALSE;
	//	param.bAllDamage=FALSE;
	//	g_ProjectileManager.CreateProjectile(param);//1발

	//	ZeroMemory(&param,sizeof(param));
	//	pos=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*9.0f;
	//	g_pMap->GetTerrainHeight(&pos,&vResult);

	//	param.fHeight = 0.0f;
	//	param.vStartPos =vResult;		
	//	param.vTargetPos=vResult;
	//	param.dwActiveDelayTime=500;
	//	param.dwLifeTime = 4000;
	//	param.dwMoveTime = 4000;		
	//	param.dwDamageTime=100;

	//	param.fSpeed = 3.0f;
	//	param.dwType=PROJECTILE::PROJECTILEID_COMMON;
	//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
	//	param.dwOwner = pCharacter->GetObjectKey();
	//	param.dwImageCode = 0;
	//	param.effectID = StrToWzID("L301");
	//	param.bAddNextSerial=FALSE;
	//	param.bAllDamage=FALSE;
	//	g_ProjectileManager.CreateProjectile(param);//2발

	//	ZeroMemory(&param,sizeof(param));
	//	pos=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*13.0f;
	//	g_pMap->GetTerrainHeight(&pos,&vResult);

	//	param.fHeight = 0.0f;
	//	param.vStartPos =vResult;		
	//	param.vTargetPos=vResult;
	//	param.dwActiveDelayTime=1000;
	//	param.dwLifeTime = 4000;
	//	param.dwMoveTime = 4000;		
	//	param.dwDamageTime=100;

	//	param.fSpeed = 3.0f;
	//	param.dwType=PROJECTILE::PROJECTILEID_COMMON;
	//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
	//	param.dwOwner = pCharacter->GetObjectKey();
	//	param.dwImageCode = 0;
	//	param.effectID = StrToWzID("L301");
	//	param.bAddNextSerial=TRUE;
	//	param.bAllDamage=TRUE;
	//	g_ProjectileManager.CreateProjectile(param);//3발

	//	ZeroMemory(&param,sizeof(param));
	//	pos=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*17.0f;
	//	g_pMap->GetTerrainHeight(&pos,&vResult);

	//	param.fHeight = 0.0f;
	//	param.vStartPos =vResult;		
	//	param.vTargetPos=vResult;
	//	param.dwActiveDelayTime=1500;
	//	param.dwLifeTime = 4000;
	//	param.dwMoveTime = 4000;		
	//	param.dwDamageTime=100;

	//	param.fSpeed = 3.0f;
	//	param.dwType=PROJECTILE::PROJECTILEID_COMMON;
	//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
	//	param.dwOwner = pCharacter->GetObjectKey();
	//	param.dwImageCode = 0;
	//	param.effectID = StrToWzID("L301");
	//	param.bAddNextSerial=TRUE;
	//	param.bAllDamage=TRUE;
	//	g_ProjectileManager.CreateProjectile(param);//4발
	//}

}


//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnFlameRulerFireWall(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		WzVector TargetPosition = pCharacter->GetVisiblePos() + pCharacter->GetDirection() * 5.0f;
		WzVector EffectRotate;
		EffectRotate.z = pCharacter->GetAngle();
		EffectRotate.x = pCharacter->GetAngleX();


		cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
		pProjectile->SetTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 666 );
		pProjectile->SetEffectRotate( EffectRotate );
		pProjectile->CreateEffect( StrToWzID("L302"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, FALSE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		//Character* pTarget = (Character *)pCharacter->GetTargetObject();

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param,sizeof(param));

		//pos=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*5.0f;

		//WzVector vResult;
		//g_pMap->GetTerrainHeight(&pos,&vResult);

		//param.fHeight = 0.0f;
		//param.vStartPos =vResult;		
		//param.vTargetPos=vResult;
		//param.dwLifeTime = 2000;
		//param.dwMoveTime = 2000;		
		//param.dwDamageTime=100;
		//
		//param.vDir.z=pCharacter->GetAngle();
		//param.vDir.x=pCharacter->GetAngleX();

		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("L302");
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=FALSE;
		//g_ProjectileManager.CreateProjectile(param);//1발
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnSlaveArcherHeavyShot(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;


		WzVector StartPosition = pCharacter->GetArrowBonePos(0);
		WzVector TargetPosition = StartPosition + pCharacter->GetDirection() * 4.0f;
		if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}

		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 200 );
		pProjectile->CreateEffect( StrToWzID("E023"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );


		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//dest.z=pos.z;

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 1.0f;

		//param.vStartPos = pos;		

		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();
		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.dwLifeTime = 0;
		//param.dwMoveTime = 600;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("E023");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnRedamorWarriorSonicBlade(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))
	{
		++m_iHitProcessed;

		WzVector StartPosition = pCharacter->GetVisiblePos();
		WzVector TargetPosition = pCharacter->GetVisiblePos() + pCharacter->GetDirection() * 10.0f;
		
		WzVector Offset1 = c_ZeroVector;
		WzVector Offset2 = c_ZeroVector;
		Offset1.z += 1.0f;
		Offset2.z -= 1.5f;

		cProjectileSwordTrailUnit* pProjectile = new cProjectileSwordTrailUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetSwordTrail( pCharacter->GetDirection(), Offset1, Offset2 );
		pProjectile->SetSwordTrailLandEffectDelay( 30 );
		pProjectile->CreateEffect( StrToWzID("E001"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 300, TRUE );
		pProjectile->SetLifeTime( 400 );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );
		




		//Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.0f;
		//param.fSpeed=6.0f;
		//param.vStartPos=pCharacter->GetVisiblePos();

		//if(pTarget)
		//{
		//	param.vTargetPos=pTarget->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
		//}

		//param.SWORD_TRAIL.vEndPos =pCharacter->GetVisiblePos();
		//param.SWORD_TRAIL.vStartPos =param.SWORD_TRAIL.vEndPos;
		//param.SWORD_TRAIL.vAdditionalStartPos.z=1.0f;
		//param.SWORD_TRAIL.vAdditionalEndPos.z=1.5f;

		//WzVector dir=pCharacter->GetDirection();
		////Todo:컨버트 함수 만들자 귀찮다.
		//vector3 dir2; 
		//dir2.x=dir.x;
		//dir2.y=dir.z;
		//dir2.z=dir.y;
		//dir2.norm(); //z축

		//vector3 up;
		//up.set(0.0f,1.0f,0.0f);//y축

		//vector3 temp=up*dir2;//x축
		//WzVector inc;
		//inc.x=temp.x;
		//inc.y=temp.z;
		//inc.z=temp.y;
		//param.SWORD_TRAIL.vEndPos +=inc;
		//param.SWORD_TRAIL.vStartPos-=inc;
		//param.SWORD_TRAIL.vEndPos +=pCharacter->GetDirection();
		//param.SWORD_TRAIL.vStartPos+=pCharacter->GetDirection();
		//param.SWORD_TRAIL.vDir=pCharacter->GetDirection();
		//param.fSpeed = 1.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_SWORD_TRAIL;
		//param.SWORD_TRAIL.dwTrailEffectDelay=30;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode =0;
		//param.effectID = StrToWzID("E001");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//param.dwMoveTime=300;
		//g_ProjectileManager.CreateProjectile(param);

		++m_iHitProcessed;
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnFrameRulerMeteoShower(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;


		////////
		for(int i=0;i<=15;i++)
		{
			WzVector StartPosition = pCharacter->GetVisiblePos();
			StartPosition.z += 28.0f;
			WzVector TargetPosition = pCharacter->GetVisiblePos();

			float randHeight = float(rand()%10);
			randHeight /= 10.0f;
			StartPosition.z+=randHeight;

			float randX = float(rand()%35);
			randX /= 10.0f;

			float randY = float(rand()%35);
			randY/=10.0f;

			int dir=rand()%3;
			if(dir==0)
			{
				randX*=-1.0f;
			}
			else if(dir==1)
			{
				randY*=-1.0f;

			}
			else
			{
				randX*=-1.0f;
				randY*=-1.0f;
			}

			StartPosition.x += randX;
			StartPosition.y += randY;

			TargetPosition.x = StartPosition.x;
			TargetPosition.y = StartPosition.y;

			TargetPosition += pCharacter->GetDirection()*8.0f;
			StartPosition -= pCharacter->GetDirection()*2.0f;

			float fT=0.0f;
			WzVector vVisiblePos = TargetPosition;
			vVisiblePos.z+=3.0f;
			int iMesh = -1;
			if( g_pMap )
				iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 100.0f,TMGO_NO_COLLISION);

			//타일로 바꾸자.
			if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
			{
				vVisiblePos.z -= fT;
				TargetPosition = vVisiblePos;
			}


			DWORD LifeTime = 1666+rand()%166;
			DWORD ActiveDelayTime = i*60+rand()%100;


			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition );
			pProjectile->SetActiveDelayTime( ActiveDelayTime );
			pProjectile->SetLifeTime( LifeTime );
			pProjectile->CreateEffect( StrToWzID("L101"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), i==1 ? pCharacter->GetCurrentAttackSerial() : 0, StrToWzID("L102"), FALSE, 0, i==15 ? TRUE : FALSE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );



			//PROJECTILE_PARAM param;

			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.0f;

			//param.vStartPos =pCharacter->GetVisiblePos();		
			//param.vStartPos.z+=28.0f;
			//param.vTargetPos =pCharacter->GetVisiblePos();
			//

			//float randHeight = float(rand()%10);
			//randHeight /= 10.0f;
			//param.vStartPos.z+=randHeight;

			//float randX = float(rand()%35);
			//randX /= 10.0f;

			//float randY = float(rand()%35);
			//randY/=10.0f;

			//int dir=rand()%3;
			//if(dir==0)
			//{
			//	randX*=-1.0f;
			//}
			//else if(dir==1)
			//{
			//	randY*=-1.0f;

			//}
			//else
			//{
			//	randX*=-1.0f;
			//	randY*=-1.0f;
			//}

			//param.vStartPos.x+=randX;
			//param.vStartPos.y+=randY;

			//param.vTargetPos.x=param.vStartPos.x;
			//param.vTargetPos.y=param.vStartPos.y;

			//param.vTargetPos+=pCharacter->GetDirection()*8.0f;
			//param.vStartPos-=pCharacter->GetDirection()*2.0f;

			//param.dwLifeTime =0;
			//param.dwMoveTime = 5000+rand()%500;
			//param.fSpeed = 3.0f;
			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;

			//if(i==1)
			//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
			//
			//param.dwOwner = pCharacter->GetObjectKey();

			//param.dwActiveDelayTime=i*60+rand()%100;

			//param.effectID =  StrToWzID("L101");
			//param.dwImageCode=0;
			//param.EndeffectID= StrToWzID("L102");

			//if(i==15)
			//{
			//	param.bAllDamage=TRUE;
			//	param.bAddNextSerial=TRUE;
			//}
			//else
			//	param.bAddNextSerial=FALSE;


			//float fT=0.0f;
			//WzVector vVisiblePos=param.vTargetPos;
			//vVisiblePos.z+=3.0f;
			//int iMesh = -1;
			//if(g_pMap)
			//	iMesh=g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 100.0f,TMGO_NO_COLLISION);

			////타일로 바꾸자.
			//if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
			//{
			//	vVisiblePos.z -= fT;
			//	param.vTargetPos=vVisiblePos;
			//}

			//g_ProjectileManager.CreateProjectile(param);

		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnMasterOfCastleBlizzard(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;
	////////
	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;
		
		for(int i=0;i<=15;i++)
		{
			WzVector StartPosition = pCharacter->GetVisiblePos();
			StartPosition.z += 28.0f;
			WzVector TargetPosition = pCharacter->GetVisiblePos();

			float randHeight = float(rand()%10);
			randHeight /= 10.0f;
			StartPosition.z+=randHeight;

			float randX = float(rand()%35);
			randX /= 10.0f;

			float randY = float(rand()%35);
			randY/=10.0f;

			int dir=rand()%3;
			if(dir==0)
			{
				randX*=-1.0f;
			}
			else if(dir==1)
			{
				randY*=-1.0f;

			}
			else
			{
				randX*=-1.0f;
				randY*=-1.0f;
			}

			StartPosition.x += randX;
			StartPosition.y += randY;

			TargetPosition.x = StartPosition.x;
			TargetPosition.y = StartPosition.y;

			TargetPosition += pCharacter->GetDirection()*8.0f;
			StartPosition -= pCharacter->GetDirection()*2.0f;

			float fT=0.0f;
			WzVector vVisiblePos = TargetPosition;
			vVisiblePos.z+=3.0f;
			int iMesh = -1;
			if( g_pMap )
				iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 100.0f,TMGO_NO_COLLISION);

			//타일로 바꾸자.
			if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
			{
				vVisiblePos.z -= fT;
				TargetPosition = vVisiblePos;
			}


			DWORD LifeTime = 1666+rand()%166;
			DWORD ActiveDelayTime = i*60+rand()%100;


			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition );
			pProjectile->SetActiveDelayTime( ActiveDelayTime );
			pProjectile->SetLifeTime( LifeTime );
			pProjectile->CreateEffect( StrToWzID("L031"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), i==1 ? pCharacter->GetCurrentAttackSerial() : 0, StrToWzID("L032"), FALSE, 0, i==15 ? TRUE : FALSE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );


			//PROJECTILE_PARAM param;

			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.0f;

			//param.vStartPos =pCharacter->GetVisiblePos();		
			//param.vStartPos.z+=28.0f;
			//param.vTargetPos =pCharacter->GetVisiblePos();


			//float randHeight = float(rand()%10);
			//randHeight /= 10.0f;
			//param.vStartPos.z+=randHeight;

			//float randX = float(rand()%35);
			//randX /= 10.0f;

			//float randY = float(rand()%35);
			//randY/=10.0f;

			//int dir=rand()%3;
			//if(dir==0)
			//{
			//	randX*=-1.0f;
			//}
			//else if(dir==1)
			//{
			//	randY*=-1.0f;

			//}
			//else
			//{
			//	randX*=-1.0f;
			//	randY*=-1.0f;
			//}

			//param.vStartPos.x+=randX;
			//param.vStartPos.y+=randY;

			//param.vTargetPos.x=param.vStartPos.x;
			//param.vTargetPos.y=param.vStartPos.y;

			//param.vTargetPos+=pCharacter->GetDirection()*8.0f;
			//param.vStartPos-=pCharacter->GetDirection()*2.0f;

			//param.dwLifeTime =0;
			//param.dwMoveTime = 5000+rand()%500;
			//param.fSpeed = 3.0f;
			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//if(i==1)
			//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		
			//param.dwOwner = pCharacter->GetObjectKey();

			//param.dwActiveDelayTime=i*60+rand()%100;

			//param.effectID =  StrToWzID("L031");
			//param.dwImageCode=0;
			//param.EndeffectID= StrToWzID("L032");

			//if(i==15)
			//{
			//	param.bAllDamage=TRUE;
			//	param.bAddNextSerial=TRUE;
			//}
			//else
			//	param.bAddNextSerial=FALSE;


			//float fT=0.0f;
			//WzVector vVisiblePos=param.vTargetPos;
			//vVisiblePos.z+=3.0f;
			//int iMesh = -1;
			//if(g_pMap)
			//	iMesh=g_pMap->GetWorldBase()->PathExp_GetMeshToStand( vVisiblePos, &fT, 100.0f,TMGO_NO_COLLISION);

			////타일로 바꾸자.
			//if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
			//{
			//	vVisiblePos.z -= fT;
			//	param.vTargetPos=vVisiblePos;
			//}

			//g_ProjectileManager.CreateProjectile(param);

		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnEvilSorcererFireBall(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	BOOL bShake=FALSE;

	//
	if(g_pHero)
	{
		WzVector vDest=g_pHero->GetPosition();
		vDest =  vDest - pCharacter->GetPosition();
		float fLength;
		fLength = VectorLength(&vDest);
		
		if(fLength<=5.0f)
			bShake=TRUE;
	}

	//

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;
	
		////////
		for(int i=0;i<=15;i++)
		{
			WzVector StartPosition = pCharacter->GetVisiblePos();
			StartPosition.z += 10.0f;
			WzVector TargetPosition = pCharacter->GetVisiblePos();


			float randHeight=float(rand()%10);
			randHeight/=10.0f;
			StartPosition.z+=randHeight;

			float randX=float(rand()%70);
			randX/=10.0f;

			float randY=float(rand()%70);
			randY/=10.0f;

			int dir=rand()%3;
			if(dir==0)
			{
				randX*=-1.0f;
			}
			else if(dir==1)
			{
				randY*=-1.0f;
			}
			else
			{
				randX*=-1.0f;
				randY*=-1.0f;
			}

			StartPosition.x += randX;
			StartPosition.y += randY;

			TargetPosition.x = StartPosition.x;
			TargetPosition.y = StartPosition.y;

			TargetPosition += pCharacter->GetDirection()*3.0f;
			StartPosition -= pCharacter->GetDirection()*4.0f;


			DWORD LifeTime = 400+rand()%100;
			DWORD ActiveDelayTime = i*50+rand()%2000;


			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition );
			pProjectile->SetActiveDelayTime( ActiveDelayTime );
			pProjectile->SetLifeTime( LifeTime );
			pProjectile->SetEffectScale( 0.3f );
			pProjectile->CreateEffect( StrToWzID("E057"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial() , StrToWzID("F021"), FALSE, bShake&&(i%3==0||i==0) ? TRUE : FALSE, i==15 ? TRUE : FALSE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );

			//PROJECTILE_PARAM param;

			//ZeroMemory(&param,sizeof(param));
			//param.fHeight = 0.0f;

			//param.vStartPos =pOwner->GetVisiblePos();		
			//param.vStartPos.z+=10.0f;
			//param.vTargetPos =pOwner->GetVisiblePos();

			//float randHeight=float(rand()%10);
			//randHeight/=10.0f;
			//param.vStartPos.z+=randHeight;

			//float randX=float(rand()%70);
			//randX/=10.0f;

			//float randY=float(rand()%70);
			//randY/=10.0f;

			//int dir=rand()%3;
			//if(dir==0)
			//{
			//	randX*=-1.0f;
			//}
			//else if(dir==1)
			//{
			//	randY*=-1.0f;
			//}
			//else
			//{
			//	randX*=-1.0f;
			//	randY*=-1.0f;
			//}

			//param.vStartPos.x+=randX;
			//param.vStartPos.y+=randY;

			//param.vTargetPos.x=param.vStartPos.x;
			//param.vTargetPos.y=param.vStartPos.y;

			//param.vTargetPos+=pCharacter->GetDirection()*3.0f;
			//param.vStartPos-=pCharacter->GetDirection()*4.0f;

			//param.dwMoveTime = 1200+rand()%300;
			//param.dwLifeTime =param.dwMoveTime+10;
			//param.fSpeed = 3.0f;
			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwActiveDelayTime=i*50+rand()%2000;

			//param.effectID =  StrToWzID("E057");
			//param.EndeffectID =StrToWzID("F021");

			//param.dwImageCode=0;
			//
			//if(i==15)
			//{
			//	param.bAllDamage=TRUE;
			//	param.bAddNextSerial=TRUE;
			//}
			//else
			//	param.bAddNextSerial=FALSE;

			//param.vScale.x=0.3f;
			//param.vScale.y=0.3f;
			//param.vScale.z=0.3f;

			//if(bShake&&(i%3==0||i==0))
			//	param.bCameraShake=TRUE;

			//g_ProjectileManager.CreateProjectile(param);

			//WzVector vPos=param.vTargetPos;

			//DWORD dwDelay=param.dwMoveTime;

		}
	}
}

//------------------------------------------------------------------------------
/**
4타처리 
*/
void SkillStateMonsterSkill::OnDragonZombieSoulOfDragon(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		pCharacter->ProcessAttackResult(pCharacter->GetCurrentAttackSerial(),TRUE);

		g_Camera.SetVibeType(Camera::VIBE_TYPE_ALL);
		g_Camera.StartVibration(100);

		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("L028"),pCharacter->GetVisiblePos(),pCharacter->GetObjectKey());

	}
}


//------------------------------------------------------------------------------
/**
	4타처리 
*/
void SkillStateMonsterSkill::OnEvilSorcererPressingWall(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		{
			WzVector TargetPosition = pCharacter->GetPosition();
			TargetPosition.x += 1.2f;
			TargetPosition.y -= 1.2f;

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetLifeTime( 500 );
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE , 100, FALSE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
		{
			WzVector TargetPosition = pCharacter->GetPosition();
			TargetPosition.x += 1.2f;
			TargetPosition.y += 1.2f;

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetLifeTime( 500 );
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
		{
			WzVector TargetPosition = pCharacter->GetPosition();
			TargetPosition.x -= 1.2f;
			TargetPosition.y -= 1.2f;

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetLifeTime( 500 );
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}
		{
			WzVector TargetPosition = pCharacter->GetPosition();
			TargetPosition.x -= 1.2f;
			TargetPosition.y += 1.2f;

			cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
			pProjectile->SetLifeTime( 500 );
			pProjectile->SetTargetPosition( TargetPosition );
			pProjectile->CreateEffect( StrToWzID("A013"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}

		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);

		//WzVector dest=pos;
		//dest.z=pos.z+1.0f;

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 1.0f;
		//param.vStartPos = pCharacter->GetPosition();
		//param.vStartPos.x+=1.2f;
		//param.vStartPos.y-=1.2f;
		//param.vTargetPos =  param.vStartPos;
		//param.dwLifeTime = 1500;
		//param.dwMoveTime = 1500;
		//param.fSpeed = 3.0f;
		//param.dwDamageTime=300;

		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("A013");
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//param.bGroupDamage=TRUE;
		//param.dwDamageTime=100;
		//g_ProjectileManager.CreateProjectile(param);

		//param.vStartPos = pCharacter->GetPosition();
		//param.vStartPos.x-=1.2f;
		//param.vStartPos.y+=1.2f;
		//param.vTargetPos =  param.vStartPos;
		//param.dwActiveDelayTime=0;
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//param.bGroupDamage=TRUE;
		//param.dwDamageTime=100;
		//param.vTargetPos = param.vStartPos;
		//g_ProjectileManager.CreateProjectile(param);

		//param.vStartPos = pCharacter->GetPosition();
		//param.vStartPos.x-=1.2f;
		//param.vStartPos.y-=1.2f;
		//param.dwActiveDelayTime=0;
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//param.bGroupDamage=TRUE;
		//param.dwDamageTime=100;
		//param.vTargetPos = param.vStartPos;
		//g_ProjectileManager.CreateProjectile(param);

		//param.vStartPos = pCharacter->GetPosition();
		//param.vStartPos.x+=1.2f;
		//param.vStartPos.y+=1.2f;
		//param.dwActiveDelayTime=0;
		//param.vTargetPos = param.vStartPos;
		//param.dwDamageTime=100;
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//param.bGroupDamage=FALSE;
		//g_ProjectileManager.CreateProjectile(param);

	}
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnDragonZombieUpperBlow(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if(ProcessHit(pCharacter,1))
	{
		//PlaySound( 4, pCharacter->GetTargetObject() );
	}
}

//------------------------------------------------------------------------------
/**
	힐을 위한 비주얼 발사체 이다.
*/
void SkillStateMonsterSkill::OnSlaveArcherRecoveryShot(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{

		++m_iHitProcessed;

		WzVector StartPosition = pCharacter->GetArrowBonePos(0);
		WzVector TargetPosition = StartPosition + pCharacter->GetDirection()*4.0f;
		if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}

		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 166 );
		pProjectile->CreateEffect( StrToWzID("L002"), pCharacter->GetObjectKey(), 0 );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );


		//WzVector pos=pCharacter->GetArrowBonePos(0);

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 1.0f;
		//param.vStartPos = pos;	
		//if(pTarget)
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//	param.dwTargetID=pTarget->GetObjectKey();
		//}
		//else
		//{
		//	param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;
		//}
		//param.dwLifeTime = 0;
		//param.dwMoveTime = 500;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial =0;
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("L002");
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=FALSE;

		//g_ProjectileManager.CreateProjectile(param);
	}

}

//------------------------------------------------------------------------------
/**
*/
void SkillStateMonsterSkill::OnBeastMasterDeadlyStrikeProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	if (CheckNotProcessedHitEvent(1))  
	{
		++m_iHitProcessed;

		WzVector StartPosition = pCharacter->GetVisiblePos() + pCharacter->GetDirection();
		StartPosition.z += 4.0f;

		WzVector TargetPosition = pCharacter->GetVisiblePos() + pCharacter->GetDirection() * 30.0f;
		TargetPosition.z += 1.0f;

		DWORD LifeTime = 1666;

		{
			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition );
			pProjectile->SetLifeTime( 1666 );
			pProjectile->CreateEffect( StrToWzID("L002"), pCharacter->GetObjectKey(), 0 );
			pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 100, FALSE );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}

		WzVector vDirection = TargetPosition - StartPosition;
		vDirection.z = 0.0f;;

		float length = VectorLength(&vDirection);


		{

			WzVector TargetPosition2 = StartPosition;

			vDirection.x = (float)sin(WZ_PI/4.5 + pCharacter->GetAngle());
			vDirection.y = (float)-cos(WZ_PI/4.5 + pCharacter->GetAngle());
			vDirection.z = 0.0f;

			TargetPosition2 += vDirection * length;

			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition2 );
			pProjectile->SetLifeTime( 1666 );
			pProjectile->CreateEffect( StrToWzID("L002"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}


		{

			WzVector TargetPosition2 = StartPosition;

			vDirection.x = (float)sin(WZ_PI/2.25 + pCharacter->GetAngle());
			vDirection.y = (float)-cos(WZ_PI/2.25 + pCharacter->GetAngle());
			vDirection.z = 0.0f;

			TargetPosition2 += vDirection * length;

			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition2 );
			pProjectile->SetLifeTime( 1666 );
			pProjectile->CreateEffect( StrToWzID("L002"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}


		{

			WzVector TargetPosition2 = StartPosition;

			vDirection.x = (float)sin(WZ_PI*2-WZ_PI/4.5+pCharacter->GetAngle());
			vDirection.y = (float)-cos(WZ_PI*2-WZ_PI/4.5+pCharacter->GetAngle());
			vDirection.z = 0.0f;

			TargetPosition2 += vDirection * length;

			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition2 );
			pProjectile->SetLifeTime( 1666 );
			pProjectile->CreateEffect( StrToWzID("L002"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}


		{

			WzVector TargetPosition2 = StartPosition;

			vDirection.x = (float)sin(WZ_PI*2-WZ_PI / 2.25+pCharacter->GetAngle());
			vDirection.y = (float)-cos(WZ_PI*2-WZ_PI / 2.25+pCharacter->GetAngle());
			vDirection.z = 0.0f;

			TargetPosition2 += vDirection * length;

			cProjectileUnit* pProjectile = new cProjectileUnit();
			pProjectile->SetArrowStartPosition( StartPosition );
			pProjectile->SetArrowTargetPosition( TargetPosition2 );
			pProjectile->SetLifeTime( 1666 );
			pProjectile->CreateEffect( StrToWzID("L002"), pCharacter->GetObjectKey(), 0 );
			g_ProjectileManagerEx.InsertProjectile( pProjectile );
		}

		////무기본이 없다 대충 밀자 
		//WzVector pos=pCharacter->GetVisiblePos();
		//pos+=pCharacter->GetDirection();
		//pos.z+=4.0f;

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param,sizeof(param));
		//param.dwDamageTime=0;
		//param.fHeight = 1.0f;
		//param.vStartPos = pos;		
		//param.vTargetPos = pCharacter->GetPosition()+pCharacter->GetDirection()*30;

		//param.vTargetPos.z+=1.0f;
		//param.dwDamageTime=100;
		//param.dwLifeTime = 0;
		//param.dwMoveTime = 5000;							
		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("L002");
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);

		//WzVector vDirection;
		//float len;

		////
		//param.vStartPos = pos;		
		//param.vTargetPos = pCharacter->GetPosition()+pCharacter->GetDirection()*30;

		//param.vTargetPos.z+=1.0f;

		//vDirection = param.vTargetPos - param.vStartPos ;

		//vDirection.z=0.0f;
		//param.vTargetPos=param.vStartPos;

		//len=VectorLength( &vDirection );

		//vDirection.x = (float)sin(WZ_PI/4.5+pCharacter->GetAngle());
		//vDirection.y = (float)-cos(WZ_PI/4.5+pCharacter->GetAngle());
		//vDirection.z=0.0f;

		//param.vTargetPos+= vDirection*len;

		//g_ProjectileManager.CreateProjectile(param);
		////
		//param.vStartPos = pos;		
		//param.vTargetPos = pCharacter->GetPosition()+pCharacter->GetDirection()*30;
		//param.vTargetPos.z+=1.0f;

		//vDirection = param.vTargetPos - param.vStartPos ;

		//vDirection.z=0.0f;
		//param.vTargetPos=param.vStartPos;

		//len=VectorLength( &vDirection );

		//vDirection.x = (float)sin(WZ_PI/2.25+pCharacter->GetAngle());
		//vDirection.y = (float)-cos(WZ_PI/2.25+pCharacter->GetAngle());
		//vDirection.z=0.0f;

		//param.vTargetPos+= vDirection*len;

		//param.vTargetPos.z=param.vStartPos.z;

		//g_ProjectileManager.CreateProjectile(param);

		////////////////////////////////////
		////
		//param.vStartPos = pos;		
		//param.vTargetPos = pCharacter->GetPosition()+pCharacter->GetDirection()*30;
		//param.vTargetPos.z+=1.0f;

		//vDirection = param.vTargetPos - param.vStartPos ;
		//vDirection.z=0.0f;
		//param.vTargetPos=param.vStartPos;

		//len=VectorLength( &vDirection );

		//vDirection.x = (float)sin(WZ_PI*2-WZ_PI/4.5+pCharacter->GetAngle());
		//vDirection.y = (float)-cos(WZ_PI*2-WZ_PI/4.5+pCharacter->GetAngle());
		//vDirection.z=0.0f;

		//param.vTargetPos+= vDirection*len;
		//param.vTargetPos.z=param.vStartPos.z;

		//g_ProjectileManager.CreateProjectile(param);
		////
		//param.vStartPos = pos;		
		//param.vTargetPos = pCharacter->GetPosition()+pCharacter->GetDirection()*30;
		//param.vTargetPos.z+=1.0f;

		//vDirection = param.vTargetPos - param.vStartPos ;

		//vDirection.z=0.0f;
		//param.vTargetPos=param.vStartPos;

		//len=VectorLength( &vDirection );

		//vDirection.x = (float)sin(WZ_PI*2-WZ_PI/2.25+pCharacter->GetAngle());
		//vDirection.y = (float)-cos(WZ_PI*2-WZ_PI/2.25+pCharacter->GetAngle());
		//vDirection.z=0.0f;

		//param.vTargetPos+= vDirection*len;

		//param.vTargetPos.z=param.vStartPos.z;
		//param.bAddNextSerial=TRUE;

		//g_ProjectileManager.CreateProjectile(param);

		////PlaySound( 4, pCharacter );
	}
}

STATEPROCESS::TYPE  SkillStateMonsterSkill::OnFellenWarrionDashAttace( Object * pOwner , DWORD CurTime , DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	// 태클 시작
	if( CheckNotProcessedHitEvent(1) )
	{		
		++m_iHitProcessed;

		const PLAYER_ACTION* pAction = pCharacter->GetCurrentAction();
		pCharacter->Move( pAction->SKILL.vCurPos.wvPos, pAction->SKILL.vDestPos.wvPos, CMS_RUN, TRUE, pAction->SKILL.vCurPos.wTile, pAction->SKILL.vDestPos.wTile);
	}

	if(m_iHitProcessed==1)
	{
		pCharacter->SetCurrentFrame(pCharacter->GetCurrentFrame());
		
	}

	return OnTackle(pOwner , CurTime , dwTick );
}
void SkillStateMonsterSkill::OnCursedArcherPoisonArrow(Object* pOwner , DWORD CurTime, DWORD dwTick)
{
	Character * pCharacter = (Character *) pOwner;
	if( CheckNotProcessedHitEvent(1) )
	{
		++m_iHitProcessed;

		WzVector StartPosition = pCharacter->GetArrowBonePos(0);
		WzVector TargetPosition = pCharacter->GetArrowBonePos(0) + pCharacter->GetDirection()*4.0f;
		if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 333 );
		pProjectile->CreateEffect( StrToWzID("E012"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );



		//Character * pTarget = ( Character *) pCharacter->GetTargetObject();
		//
		//WzVector pos;
		//pos = pCharacter->GetArrowBonePos(0);
		//
		//if( pCharacter->GetVisiblePos() == pos )
		//{
		//	pos = pCharacter->GetWeaponBonePos(0);
		//}

		//BASE_SKILLINFO * pInfo = GetSkillInfo();
		//assert( pInfo);
		//
		//if(!pInfo)
		//	return;

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param , sizeof(param) );

		//param.fHeight = 1.0f;
		//param.vStartPos = pos;

		//if( pTarget )
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += ( pTarget->GetArrowTargetHeight() );
		//	param.dwTargetID = pTarget->GetObjectKey();
		//}
		//else
		//{
		//	param.vTargetPos = param.vStartPos + pCharacter->GetDirection()*4.0f; // 타겟이 없을땐 앞으로
		//}

		//param.dwLifeTime = 0;
		//float range = (( float ) pInfo->m_wSkillRange / 10.0f);

		//WzVector wzVec1 = param.vTargetPos - param.vStartPos;
		//float fLength = VectorLength(&wzVec1);
		//param.dwMoveTime = 1000;
		//float rate = fLength/range;
		//float fMoveTime = (float)param.dwMoveTime;
		//fMoveTime*=rate;
		//param.dwMoveTime=(DWORD)fMoveTime;
		//param.dwMoveTime=max(100,param.dwMoveTime);

		//param.fSpeed = 3.0f;
		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;

		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = pInfo->m_dwFlyingObjectCode;
		//param.effectID = StrToWzID("E012");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;

		//g_ProjectileManager.CreateProjectile(param);
	}

}


void SkillStateMonsterSkill::OnDarkWarriorBeat(Object * pOwner, DWORD CurTime, DWORD dwTick)
{
	Character * pCharacter = (Character *) pOwner;
	if( CheckNotProcessedHitEvent(1) )
	{
		++m_iHitProcessed;


		//WzVector StartPosition = pCharacter->GetArrowBonePos(0);
		WzVector TargetPosition = pCharacter->GetArrowBonePos(0) + pCharacter->GetDirection()*4.0f;
		if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}


		cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
		pProjectile->SetTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 840 );
		pProjectile->CreateEffect( StrToWzID("J586"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 500, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );



		//Character * pTarget = ( Character *) pCharacter->GetTargetObject();
		//BASE_SKILLINFO * pInfo = GetSkillInfo();
		//assert( pInfo);

		//PROJECTILE_PARAM param;
		//ZeroMemory(&param , sizeof(param) );

		//param.fHeight = 1.0f;
		//if( pTarget )
		//{
		//	param.vTargetPos = pTarget->GetVisiblePos();
		//	param.vTargetPos.z += ( pTarget->GetArrowTargetHeight() );
		//	param.dwTargetID = pTarget->GetObjectKey(); 
		//}
		//else
		//{
		//	param.vTargetPos = pCharacter->GetVisiblePos()+ pCharacter->GetDirection()*4.0f; // 타겟이 없을땐 앞으로

		//}

		//param.vStartPos=param.vTargetPos;


		//param.dwMoveTime=4000;
		//param.dwLifeTime=4200;
		//param.fSpeed = 5.0f;

		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = 0;
		//param.effectID = StrToWzID("J586");
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//param.dwDamageTime = 500;

		//g_ProjectileManager.CreateProjectile(param);
	}
}

void SkillStateMonsterSkill::OnArchbishopOfDarknessDoom(Object * pOwner, DWORD CurTime, DWORD dwTick)
{
	Character * pCharacter = (Character *) pOwner;
	if( CheckNotProcessedHitEvent(1) )
	{
		++m_iHitProcessed;
		BASE_SKILLINFO * pInfo = GetSkillInfo();
		assert( pInfo);

		// 연출용 구름 이펙트
		WzVector pos = pCharacter->GetVisiblePos();
		pos.z = pos.z + 7.5f;
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("I577") , pos, pCharacter->GetObjectKey());

		// 연출용 번개 이펙트
		for( int i =0; i < 10; i++ )
		{
			float fProbablility = ((float)(rand() % 100) * 0.01f) * WZ_PI *2.0f;
			WzVector vDirection;
			vDirection.x = (float)sin(fProbablility +pCharacter->GetAngle());
			vDirection.y = (float)-cos(fProbablility+pCharacter->GetAngle());
			float fLength = (float) ((float)(rand() % 100) * 0.01f) * 3.0f + 0.5f;
			
			WzVector TargetPosition = pCharacter->GetVisiblePos() + vDirection * fLength;
			DWORD ActiveDelayTime = rand() % 1000 + 1000 * i;

			{
				WzVector TargetPosition = pCharacter->GetVisiblePos() + vDirection * fLength;

				cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
				pProjectile->SetTargetPosition( TargetPosition );
				pProjectile->SetActiveDelayTime( ActiveDelayTime );
				pProjectile->SetLifeTime( 2000 );
				pProjectile->CreateEffect( StrToWzID("I578"), pCharacter->GetObjectKey(), 0 );
				pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 500, TRUE );
				g_ProjectileManagerEx.InsertProjectile( pProjectile );
			}

			fProbablility += 2.0f;
			vDirection.x = (float)sin(fProbablility +pCharacter->GetAngle());
			vDirection.y = (float)-cos(fProbablility+pCharacter->GetAngle());
			fLength = (float) ((float)(rand() % 100) * 0.01f) * 3.0f + 0.5f;

			{
				WzVector TargetPosition = pCharacter->GetVisiblePos() + vDirection * fLength;

				cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
				pProjectile->SetTargetPosition( TargetPosition );
				pProjectile->SetActiveDelayTime( ActiveDelayTime - 750 );
				pProjectile->SetLifeTime( 2000 );
				pProjectile->CreateEffect( StrToWzID("I578"), pCharacter->GetObjectKey(), 0 );
				g_ProjectileManagerEx.InsertProjectile( pProjectile );
			}

			fProbablility -= 4.0f;
			vDirection.x = (float)sin(fProbablility +pCharacter->GetAngle());
			vDirection.y = (float)-cos(fProbablility+pCharacter->GetAngle());
			fLength = (float) ((float)(rand() % 100) * 0.01f) * 3.0f + 0.5f;

			{
				WzVector TargetPosition = pCharacter->GetVisiblePos() + vDirection * fLength;

				cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
				pProjectile->SetTargetPosition( TargetPosition );
				pProjectile->SetActiveDelayTime( ActiveDelayTime );
				pProjectile->SetLifeTime( 2000 );
				pProjectile->CreateEffect( StrToWzID("I578"), pCharacter->GetObjectKey(), 0 );
				g_ProjectileManagerEx.InsertProjectile( pProjectile );
			}

			//
			//PROJECTILE_PARAM param;
			//ZeroMemory(&param , sizeof(param) );

			//param.fHeight = 1.0f;
			//param.dwMoveTime=10000;
			//param.dwLifeTime=10000;
			//param.fSpeed = 5.0f;

			//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
			//param.dwOwner = pCharacter->GetObjectKey();
			//param.dwImageCode = 0;
			//param.effectID = StrToWzID("I578");

			//if( i == 0 )
			//{
			//	param.bAddNextSerial=TRUE;
			//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
			//}
			//else
			//{
			//	param.bAddNextSerial=FALSE;
			//	param.dwAttackSerial = 0;
			//}

			//param.bAllDamage=FALSE;
			//param.dwDamageTime = 500;

			//
			//float fProbablility = ((float)(rand() % 100) * 0.01f) * WZ_PI *2.0f;
			//WzVector vDirection;
			//vDirection.x = (float)sin(fProbablility +pCharacter->GetAngle());
			//vDirection.y = (float)-cos(fProbablility+pCharacter->GetAngle());
			//vDirection.z = 0.0f;
			//float fLength = (float) ((float)(rand() % 100) * 0.01f) * 3.0f + 0.5f;
			//param.vTargetPos = pCharacter->GetVisiblePos() + vDirection * fLength;
			//param.vStartPos=param.vTargetPos;
			//int delayTime = rand() % 1000 + 1000 * i;
			//param.dwActiveDelayTime = delayTime;
			//g_ProjectileManager.CreateProjectile(param);


			//fProbablility += 2.0f;
			//vDirection.x = (float)sin(fProbablility +pCharacter->GetAngle());
			//vDirection.y = (float)-cos(fProbablility+pCharacter->GetAngle());
			//vDirection.z = 0.0f;
			//fLength = (float) ((float)(rand() % 100) * 0.01f) * 3.0f + 0.5f;
			//param.vTargetPos = pCharacter->GetVisiblePos() + vDirection * fLength;
			//param.vStartPos=param.vTargetPos;
			//param.dwActiveDelayTime = delayTime - 750;
			//g_ProjectileManager.CreateProjectile(param);


			//fProbablility -= 4.0f;
			//vDirection.x = (float)sin(fProbablility +pCharacter->GetAngle());
			//vDirection.y = (float)-cos(fProbablility+pCharacter->GetAngle());
			//vDirection.z = 0.0f;
			//fLength = (float) ((float)(rand() % 100) * 0.01f) * 3.0f +0.5f;
			//param.vTargetPos = pCharacter->GetVisiblePos() + vDirection * fLength;
			//param.vStartPos=param.vTargetPos;
			//param.dwActiveDelayTime = delayTime;
			//g_ProjectileManager.CreateProjectile(param);


		}
	}
}

void SkillStateMonsterSkill::OnBindedArcherStunShot(Object* pOwner, DWORD CurTime, DWORD dwTick)
{
	Character * pCharacter = ( Character *) pOwner;
	if( CheckNotProcessedHitEvent(1) )
	{
		++m_iHitProcessed;


		WzVector StartPosition = pCharacter->GetArrowBonePos(0);
		WzVector TargetPosition = pCharacter->GetArrowBonePos(0) + pCharacter->GetDirection()*4.0f;
		if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
		{
			TargetPosition = pTarget->GetArrowTargetPos();
		}


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetPosition( TargetPosition );
		pProjectile->SetLifeTime( 250 );
		pProjectile->CreateEffect( StrToWzID("E120"), pCharacter->GetObjectKey(), 0 );
		pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), 0, FALSE, 500, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

	//	PROJECTILE_PARAM param;
	//	ZeroMemory(&param , sizeof(param) );


	//	WzVector pos = pCharacter->GetArrowBonePos(0);

	//	if( pCharacter->GetVisiblePos() == pos )
	//	{
	//		pos = pCharacter->GetWeaponBonePos(0);
	//	}
	//	param.vStartPos = pos;

	//	Character * pTarget = ( Character *) pCharacter->GetTargetObject();
	//	if( pTarget )
	//	{
	//		param.vTargetPos = pTarget->GetVisiblePos();
	//		param.vTargetPos.z += ( pTarget->GetArrowTargetHeight() );
	//		param.dwTargetID = pTarget->GetObjectKey(); 
	//	}
	//	else
	//	{
	//		param.vTargetPos = pCharacter->GetVisiblePos()+ pCharacter->GetDirection()*4.0f; // 타겟이 없을땐 앞으로

	//	} 
	//	param.dwMoveTime=500;
	//	param.dwLifeTime=0;
	//	param.fSpeed = 2.0f;
	//	param.fHeight = 1.0f;
	//	param.dwType=PROJECTILE::PROJECTILEID_COMMON;
	//	param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
	//	param.dwOwner = pCharacter->GetObjectKey();
	//	param.dwImageCode = 0;
	//	param.effectID = StrToWzID("E120");
	//	param.bAddNextSerial=TRUE;
	//	param.bAllDamage=TRUE;
	//	param.dwDamageTime = 500;
	//	g_ProjectileManager.CreateProjectile(param);
	}

}

void SkillStateMonsterSkill::OnFireCerberusFireBreath( Object* pOwner, DWORD CurTime, DWORD dwTick)
{
	Character * pCharacter = (Character *) pOwner;
	if(m_hEffectHandle !=INVALID_HANDLE_VALUE)
	{
		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffectHandle);
		if(pBody )
		{
			WzVector vPos = pCharacter->GetBonePos(10);	
			WzVector vRot;
			vRot.x = pCharacter->GetAngleX();
			vRot.y = 0.0f;
			vRot.z = pCharacter->GetAngle();
			WzVector vScale;
			vScale.x = 1.0f;
			vScale.y = 1.0f;
			vScale.z = 1.0f;
			pBody->SetResultMatrix(vPos , vRot, vScale);
		}
	}
}

//------------------------------------------------------------------------------
/**
	발사체에서 어택 씨리얼을 증가 시키는 것들은 false를 리턴시킨다.
*/
BOOL SkillStateMonsterSkill::EndSkillForType(Object* pOwner)
{
	Character*	pCharacter = (Character*)pOwner;

	BASE_SKILLINFO *pInfo = GetSkillInfo();
	assert(pInfo && "적용할수없는 스킬의 결과입니다");

	//기존의 하드 코딩 정리 할것 
	switch (pInfo->m_SkillClassCode)
	{
	case eSKILL_BEAST_MASTER_DEADLY_STRIKE:
	case eSKILL_EVIL_SORCERER_FIRE_BALL:     
	case eSKILL_EVIL_SORCERER_PRESSING_WALL: 
	case eSKILL_REDAMOR_WARRIOR_SONIC_BLADE:  
	case eSKILL_HEAVY_HUNTER_SNIPING:			
	case eSKILL_HEAVY_NOIZE_SHOT:			
	case eSKILL_HEAVY_WHISTLE_SHOT:			
	case eSKILL_SLAVE_ARCHER_HEAVY_SHOT:     
	case eSKILL_MASTER_OF_CASTLE_BLIZZARD:
	case eSKILL_MASTER_OF_CASTLE_BLIZZARD2:
	case eSKILL_FLAME_RULER_METEO_SHOWER:
	case eSkILL_CURSED_ARCHER_POISON_ARROW:
	case eSKILL_DARK_WARRIOR_BEAT:
	case eSKILL_BINDED_ARCHER_STUN_SHOT:
		{
		}
		return FALSE;
	}

	//발사체 코드가 있으면 
	if(pInfo->m_dwFlyingObjectCode)
	{
		return FALSE;
	}

	return TRUE;
}

BYTE SkillStateMonsterSkill::SkillHitCount(Object* pOwner)
{
	Character*	pCharacter = (Character*)pOwner;

	BASE_SKILLINFO *pInfo = GetSkillInfo();
	assert(pInfo && "적용할수없는 스킬의 결과입니다");

	if (pInfo) 
    {
		switch (pInfo->m_SkillClassCode)
		{
		case eSKILL_DARKNESS_OBSERVER_WHEEL_ATTACK:
			return 4;

		case eSKILL_DARK_F_INFANTRY_BASH:
		case eSKILL_REDAMOR_WARRIOR_SPIN_CRUSH: 
		case eSKILL_DARK_KNIGHT_DARK_SWORD:
			return 3;

		case eSKILL_DARK_F_PATROL_TWIN_STRECH:
		case eSKILL_BEAST_KNIGHT_DEATH_DIVIDE:
		case eSKILL_FILL_OFF:
		case eSKILL_FIRE_DESTROYER_DOUBLE_HEAD:
			return 2;
		default:
			return 1;		
		}
	}

	return 1;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateMonsterSkill::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;

	//타격점 처리가 이루어 지지 않는 것들 (처리한 hit수가 원래 hit수에비해 모자란것들)
	if(EndSkillForType(pOwner)||m_iHitProcessed < SkillHitCount(pOwner)) 
	{
		pCharacter->ProcessAttackResult(pCharacter->GetCurrentAttackSerial(),TRUE);
		pCharacter->AddNextAttackSerial();
	}

	return STATECHANGE::SUCCESS;
}