#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFactory.h"
#include "SkillInfoParser.h"
#include "Hero.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"
#include "GlobalFunc.h"
#include "ProjectileManagerEx.h"
#include "SkillStateHpSuction.h"

SkillStateHpSuction::SkillStateHpSuction()
: m_LastTime(0)
{
#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
    target_projectile_key_list.clear();
#else 
    m_TargetProjectileKey = 0;
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
}

SkillStateHpSuction::~SkillStateHpSuction(void)
{
}
#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
void SkillStateHpSuction::UpdateEffectRotate(Character* pCharacter)
{
    Projectile_Info_List::iterator itr = target_projectile_key_list.begin();
    Projectile_Info_List::iterator end_itr = target_projectile_key_list.end();
    for ( ; itr != end_itr; ++itr)
    {
        UpdateEffectRotate(pCharacter, (*itr).target, (*itr).projectile_key);
    }
}
void SkillStateHpSuction::UpdateEffectRotate(Character* pCharacter, Character* pTarget, DWORD projectile_key)
{
    if (pCharacter == NULL || pTarget == NULL)
        return;

    if (cProjectileChaseTarget* pProjectile = (cProjectileChaseTarget*)g_ProjectileManagerEx.GetProjectile(projectile_key))
    {
        HANDLE EffectHandle = pProjectile->GetEffectHandle();

        if( CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(EffectHandle) )
        {
            pBody->SetAllElementTargetPos(pCharacter->GetBonePos(14));
        }

        static DWORD temp_tick_time = 0;
        static DWORD temp_time = 0;
        temp_time += m_dwTime;
        if (temp_time > temp_tick_time)
        {
            temp_time = 0;
        }
        else
        {
            return;
        }

        WzVector vLen = pCharacter->GetBonePos(14) - pTarget->GetArrowTargetPos();	// 목표와의 상대적 벡터
        float fLen = VectorLength(&vLen);
        WzVector vDir; VectorNormalize(&vDir, &vLen);


        float fTemp = pCharacter->GetBonePos(14).z - pTarget->GetArrowTargetPos().z;
        WzVector vRot;
        vRot.x = atan2(fLen, fTemp);
        vRot.z = Math_GetAngleFromVector(&vDir);
        vRot.y = 0.f;//0.0f;

        pProjectile->SetEffectRotate( vRot );
    }
}
#else
void SkillStateHpSuction::UpdateEffectRotate( Character* pCharacter, Character* pTarget )
{
    if( pCharacter == NULL || pTarget == NULL )
        return;

    if( cProjectileChaseTarget* pProjectile = (cProjectileChaseTarget*)g_ProjectileManagerEx.GetProjectile(m_TargetProjectileKey) )
    {
        HANDLE EffectHandle = pProjectile->GetEffectHandle();

        if( CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(EffectHandle) )
        {
            pBody->SetAllElementTargetPos(pCharacter->GetBonePos(14));
        }

        WzVector vLen = pCharacter->GetBonePos(14) - pTarget->GetArrowTargetPos();	// 목표와의 상대적 벡터
        float fLen = VectorLength(&vLen);
        WzVector vDir; VectorNormalize(&vDir, &vLen);


        float fTemp = pCharacter->GetBonePos(14).z - pTarget->GetArrowTargetPos().z;
        WzVector vRot;
        vRot.x = atan2(fLen, fTemp);
        vRot.z = Math_GetAngleFromVector(&vDir);
        vRot.y = 0.f;//0.0f;

        pProjectile->SetEffectRotate( vRot );
    }
}
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
STATECHANGE::TYPE SkillStateHpSuction::OnStart(Object* pOwner, DWORD CurTime)
{
    m_btAniState = 0;
    m_dwTime = 0;

#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
#else
	Character*	pChr = (Character*)pOwner;

	Character* pTarget   = (Character *)pChr->GetTargetObject();
	if(NULL == pChr || NULL == pTarget)
	{
		assert(!"AddSkillResult::SkillStateHpSuction");
        return StateSkill::OnStart(pOwner,CurTime);
	}

    DWORD LifeTime = 4000;

    SkillScriptInfo* pInfo = GetSkillInfo();
    if(pInfo)
    {
        BASE_ABILITYINFO * pAbilityInfo = pInfo->GetAbilityInfoByIndex(0);
        if(pAbilityInfo)
            LifeTime = pAbilityInfo->m_iParam[2];
    }


    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
    pProjectile->SetTargetObjectKey( pTarget->GetObjectKey(), TRUE );
    pProjectile->CreateEffect( StrToWzID("K800"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
    pProjectile->SetLifeTime( LifeTime );
    g_ProjectileManagerEx.InsertProjectile(pProjectile);

    m_TargetProjectileKey = pProjectile->GetKey();

    UpdateEffectRotate( pChr, pTarget );

#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL

	return StateSkill::OnStart(pOwner,CurTime);	
}

STATEPROCESS::TYPE SkillStateHpSuction::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    if (pChr && pChr->GetObjectType() == PLAYER_OBJECT)
    {
        if (pChr->IsDead())
        {
            return STATEPROCESS::END;
        }
    }

	EnterProcess(pOwner,CurTime,dwTick);

	if( 90.f < GetAnimationRate()  && 0 == m_btAniState)
	{
        if (pChr->GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            pChr->SetAnimation(StrToWzID("S617"), TRUE);
        }
        else    //eWEAPONTYPE_WHIP
        {
            pChr->SetAnimation(StrToWzID("S417"), TRUE);
        }

		SetAnimationRate(0.f);
		m_btAniState = 1;
	}

    if( m_btAniState == 1 )
    {
#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
        UpdateEffectRotate(pChr);

        // 2.5초가 지나도 맞았다는 판정이 안오면 스킬 멈춤
        if ((m_dwTime > 2500) &&
            (target_projectile_key_list.size() == 0))
        {
            g_pHero->SendStopMovePacket();
            return STATEPROCESS::END;
        }
#else
        Character* pTarget = (Character *)pChr->GetTargetObject();
        UpdateEffectRotate( pChr, pTarget );
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL

        //Character* pTarget = (Character *)pChr->GetTargetObject();

        //static DWORD Delay = 100;
        //if( GetCurrentTime() >= m_LastTime + Delay )
        //{
        //    cProjectileUnit* pProjectile = new cProjectileUnit();
        //    pProjectile->SetArrowStartPosition( pTarget->GetArrowTargetPos() );
        //    pProjectile->SetArrowTargetPosition( pChr->GetBonePos(14) );
        //    pProjectile->SetLifeTime(150);
        //    pProjectile->CreateEffect( StrToWzID("K797"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
        //    g_ProjectileManagerEx.InsertProjectile( pProjectile );

        //    m_LastTime = GetCurrentTime();
        //}
    }


	SkillScriptInfo *pInfo = GetSkillInfo();
	if(pInfo)
	{
		BASE_ABILITYINFO * pAbilityInfo = 
			pInfo->GetAbilityInfoByIndex(0);

		if(pAbilityInfo)
		{
			if(m_dwTime > (DWORD)pAbilityInfo->m_iParam[2] && 1 == m_btAniState)
			{
                if (pChr->GetWeaponKind() == eWEAPONTYPE_DAGGER)
                {
                    pChr->SetAnimation(StrToWzID("S618"));
                }
                else    //eWEAPONTYPE_WHIP
                {
                    pChr->SetAnimation(StrToWzID("S418"));
                }

				m_btAniState = 2;
			}
		}
	}

	m_dwTime += dwTick;

	// 이벤트체크
	CheckEvent();

	// 친다
	ProcessHit(pChr,1);

	if(g_pHero->GetObjectKey() == pChr->GetObjectKey())
	{
#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
#else
		Character* pTarget = (Character *)pChr->GetTargetObject();

		if(NULL == pTarget)
		{
			g_pHero->SendStopMovePacket();
			return STATEPROCESS::END;
		}

		if(pTarget->IsDead())
		{
			g_pHero->SendStopMovePacket();
			return STATEPROCESS::END;
		}

		WzVector vLen = pChr->GetBonePos(14) - pTarget->GetArrowTargetPos(); //14 == Hand Bone Idx
		float fLen = VectorLength(&vLen);
		if(30.f < fLen)
		{
			g_pHero->SendStopMovePacket();
			return STATEPROCESS::END;
		}
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
        
        if(TRUE == g_pHero->CannotAction())
        {
            g_pHero->SendStopMovePacket();
            return STATEPROCESS::END;
        }
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}

STATECHANGE::TYPE SkillStateHpSuction::OnEnd(Object* pOwner, DWORD CurTime)
{
#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
    Projectile_Info_List::iterator itr = target_projectile_key_list.begin();
    Projectile_Info_List::iterator end_itr = target_projectile_key_list.end();
    for ( ; itr != end_itr; ++itr)
    {
        g_ProjectileManagerEx.DeleteProjectile((*itr).projectile_key);
    }
#else
    g_ProjectileManagerEx.DeleteProjectile( m_TargetProjectileKey );
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL

    return StateSkill::OnEnd(pOwner, CurTime );
}

#ifdef _NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL
void SkillStateHpSuction::AddHpSuctionProjectile(Character* owner_, Character* target)
{
    if (owner_ == NULL || target == NULL)
    {
        return;
    }

    DWORD LifeTime = 4000;

    SkillScriptInfo* pInfo = GetSkillInfo();
    if(pInfo)
    {
        BASE_ABILITYINFO * pAbilityInfo = pInfo->GetAbilityInfoByIndex(0);
        if(pAbilityInfo)
            LifeTime = pAbilityInfo->m_iParam[2];
    }

    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
    pProjectile->SetTargetObjectKey( target->GetObjectKey(), TRUE );
    pProjectile->CreateEffect( StrToWzID("K800"), owner_->GetObjectKey(), target->GetObjectKey() );
    pProjectile->SetLifeTime( LifeTime );
    g_ProjectileManagerEx.InsertProjectile(pProjectile);

    DWORD target_projectile_key = pProjectile->GetKey();


    Projectile_Info projectile_;
    projectile_.target = target;
    projectile_.projectile_key = target_projectile_key;

    target_projectile_key_list.push_back(projectile_);

    UpdateEffectRotate(owner_, target, target_projectile_key);
}
#endif //_NA_007113_20140324_SHADOW_HPSUCTION_RENEWAL