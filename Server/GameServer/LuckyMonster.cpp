#include "stdafx.h"
#include "LuckyMonster.h"
#include "CharacterFormula.h"
#include "NPCStateManager.h"
#include "AIParamParser.h"
#include "AIMessageQueue.h"
#include <SinTable.h>
#include "PublicMath.h"


LuckyMonster::LuckyMonster()
{
	SetObjectType( LUCKYMON_OBJECT );
}

BOOL LuckyMonster::Init()
{	
	m_dwRunAwayRegenTime = 0;
	m_ExpireTimer.Clear();
	m_RetreatTimer.SetTimer( 5*1000 );

	return NPC::Init();
}

VOID LuckyMonster::InitMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID )
{
	this->Init();
	m_LinkedAction = AppliedNpcDependencyChain::Instance()->GetNpcLinkAction( NPCCode, MONSTER_OBJECT );
	NPC::SetBaseInfo( NPCCode, byMoveType, dwMoveAreaID );

#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
	m_RegenHP = (DWORD)(GetHP() * 0.99f);   //생성시의 HP의 99%를 저장한다.
										//소멸시간 체크를 위해, %는 변경 가능할 부분으로 예상
#endif 
}

BOOL LuckyMonster::Update( DWORD dwDeltaTick )
{
	// 시간이 다되면 소멸된다.
	if( m_ExpireTimer.IsExpired() && IsAlive() )
	{
		AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(GetBaseInfo()->m_wAICode);
		m_dwRunAwayRegenTime = pAITypeInfo->m_dwLUCKY_MONSTER_REGEN_TIME;

		// 도망사실을 클라에 알려준다.
		MSG_CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD brdMsg;
		brdMsg.m_dwObjectKey = GetObjectKey();
		brdMsg.m_iRotateAngle = GetRunAwayAngle();
		SendPacketAround( &brdMsg, sizeof(brdMsg) );


		OnDead();
	}

	// 한번씩 일정거리 만큼 도망친다.
	if( m_RetreatTimer.IsExpired() && (rand()%2) ) 
	{
		AI_MSG_CHANGETORETREATSTATE AIMsg;
		AIMsg.fMaxDistance = 25.f;
		AIMsg.bIdleAfterRetreat = TRUE;
		SendAIMessage( &AIMsg, sizeof(AIMsg) );
	}

	return NPC::Update( dwDeltaTick );
}

VOID LuckyMonster::SetExpireTime( DWORD dwInterval )
{
	m_ExpireTimer.SetTimer( dwInterval );

	if( !dwInterval )
		m_ExpireTimer.DisableCheckTime();
}



void
LuckyMonster::_Damaged(DamageArgs* damage_arg)
{
#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
    if( !m_ExpireTimer.GetIntervalTime() && CheckExpireTimeStart()   )
#else
    if( !m_ExpireTimer.GetIntervalTime() )              // 최초로 공격을 받으면 시간제한이 발동된다.
#endif
    {
        UnitRegenInfo* pUnitRegenInfo = GetUnitRegenInfo();

        // 모든 유닛은 pUnitRegenInfo가 존재 한다. 
        // 예외 gm명령어로 생성시 존재 하지 않는다.
        if (!pUnitRegenInfo || pUnitRegenInfo->IsRegenTypeTrigger() == false)
        {
            const WORD wAICode = GetBaseInfo()->m_wAICode;
            const AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(wAICode);

            const DWORD minTime = pAITypeInfo->m_dwLUCKY_MONSTER_RUNAWAY_TIME_MIN;
            const DWORD maxTime = pAITypeInfo->m_dwLUCKY_MONSTER_RUNAWAY_TIME_MAX;
            const DWORD dwInterval = (DWORD)dRandom( minTime, maxTime );

            m_ExpireTimer.SetTimer( dwInterval );
        }
    }

    if( GetRunAwayRegenTime() )                         // 마지막으로 도망가서 사라지는 중이라면 데미지 Miss 처리한다.
        return;

    Character*  pAttacker = damage_arg->actor_;
    const Attributes& attributes = pAttacker->GetAttr();

    damage_arg->damage_ = (damage_arg->option_.is_critical_ == false) ? 1 : 2;
    damage_arg->damage_ += attributes.GetLuckMonIncDamage();
    // 더블 데미지: 확율로 데미지가 두배로 증가한다.
    damage_arg->damage_ += CalcDoubleDamage(pAttacker, damage_arg->damage_);

    return NPC::_Damaged(damage_arg);
}

VOID LuckyMonster::OnDead()
{
	// 몬스터가 도망쳐서 사라지는거랑, 맞아서 죽는거랑 처리가 다르다.
	if( GetRunAwayRegenTime() )
	{
        NPC::OnDead();
	}
	else
		Monster::OnDead();
}

int LuckyMonster::GetRunAwayAngle()
{
	int iRotateAngle = 0;

	// 현재 공격하고 있는 타겟이 있으면 타겟의 반대방향으로 도망친다.
	if( GetTargetChar() )
	{
		WzVector vCurPos, vTargetPos;
		GetPos( &vCurPos );		GetTargetChar()->GetPos( &vTargetPos );

		iRotateAngle = Math_GetAngleFromVector2( &( vCurPos - vTargetPos ) );
	}
	// 타겟이 없으면 랜덤하게 도망침
	else
	{
		iRotateAngle = rand() % 360;
	}

	return iRotateAngle;
}

#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
//HP 99%에소멸 타이머를 작동 시키기 위한 체크 함
BOOL LuckyMonster::CheckExpireTimeStart()
{
	DWORD currentHP = GetHP();

	//타이머가 작동하지 않고 몬스터의 HP가 99% 이하일 경우
	if( m_RegenHP > currentHP )
		return TRUE;

	return FALSE;
}
#endif





































