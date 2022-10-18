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
	m_RegenHP = (DWORD)(GetHP() * 0.99f);   //�������� HP�� 99%�� �����Ѵ�.
										//�Ҹ�ð� üũ�� ����, %�� ���� ������ �κ����� ����
#endif 
}

BOOL LuckyMonster::Update( DWORD dwDeltaTick )
{
	// �ð��� �ٵǸ� �Ҹ�ȴ�.
	if( m_ExpireTimer.IsExpired() && IsAlive() )
	{
		AI_TYPE_INFO *pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(GetBaseInfo()->m_wAICode);
		m_dwRunAwayRegenTime = pAITypeInfo->m_dwLUCKY_MONSTER_REGEN_TIME;

		// ��������� Ŭ�� �˷��ش�.
		MSG_CG_SYNC_LUCKY_MONSTER_RUNAWY_BRD brdMsg;
		brdMsg.m_dwObjectKey = GetObjectKey();
		brdMsg.m_iRotateAngle = GetRunAwayAngle();
		SendPacketAround( &brdMsg, sizeof(brdMsg) );


		OnDead();
	}

	// �ѹ��� �����Ÿ� ��ŭ ����ģ��.
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
    if( !m_ExpireTimer.GetIntervalTime() )              // ���ʷ� ������ ������ �ð������� �ߵ��ȴ�.
#endif
    {
        UnitRegenInfo* pUnitRegenInfo = GetUnitRegenInfo();

        // ��� ������ pUnitRegenInfo�� ���� �Ѵ�. 
        // ���� gm��ɾ�� ������ ���� ���� �ʴ´�.
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

    if( GetRunAwayRegenTime() )                         // ���������� �������� ������� ���̶�� ������ Miss ó���Ѵ�.
        return;

    Character*  pAttacker = damage_arg->actor_;
    const Attributes& attributes = pAttacker->GetAttr();

    damage_arg->damage_ = (damage_arg->option_.is_critical_ == false) ? 1 : 2;
    damage_arg->damage_ += attributes.GetLuckMonIncDamage();
    // ���� ������: Ȯ���� �������� �ι�� �����Ѵ�.
    damage_arg->damage_ += CalcDoubleDamage(pAttacker, damage_arg->damage_);

    return NPC::_Damaged(damage_arg);
}

VOID LuckyMonster::OnDead()
{
	// ���Ͱ� �����ļ� ������°Ŷ�, �¾Ƽ� �״°Ŷ� ó���� �ٸ���.
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

	// ���� �����ϰ� �ִ� Ÿ���� ������ Ÿ���� �ݴ�������� ����ģ��.
	if( GetTargetChar() )
	{
		WzVector vCurPos, vTargetPos;
		GetPos( &vCurPos );		GetTargetChar()->GetPos( &vTargetPos );

		iRotateAngle = Math_GetAngleFromVector2( &( vCurPos - vTargetPos ) );
	}
	// Ÿ���� ������ �����ϰ� ����ħ
	else
	{
		iRotateAngle = rand() % 360;
	}

	return iRotateAngle;
}

#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
//HP 99%���Ҹ� Ÿ�̸Ӹ� �۵� ��Ű�� ���� üũ ��
BOOL LuckyMonster::CheckExpireTimeStart()
{
	DWORD currentHP = GetHP();

	//Ÿ�̸Ӱ� �۵����� �ʰ� ������ HP�� 99% ������ ���
	if( m_RegenHP > currentHP )
		return TRUE;

	return FALSE;
}
#endif





































