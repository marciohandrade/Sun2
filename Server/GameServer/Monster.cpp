#include "stdafx.h"
#include "Monster.h"
#include "GameField.h"
#include "IdleState.h"
#include "WanderState.h"
#include "TrackState.h"
#include "AttackState.h"
#include "HelpState.h"
#include "DeadState.h"
#include "ThrustState.h"
#include "JumpState.h"
#include "FallApartState.h"
#include "ReturnState.h"
#include "RetreatState.h"
#include "FlyingState.h"
#include "KnockDownState.h"
#include "RunAwayState.h"
#include "ChaosState.h"
#include "Map.h"
#include "GameZone.h"
#include "GameRoom.h"
#include "GameParty.h"
#include "GamePartyManager.h"
#include "StatusManager.h"
#include "CharacterFormula.h"
#include "AIParamParser.h"
#include "DropManager.h"
#include "NPCStateManager.h"
#include "PatrolState.h"
#include "TargetManager.h"
#include "FatigueManager.h"
#include "ItemExpDistributor.h"
#include "QuestManager_Concrete.h"
#include <AreaConquestCommonHeader.h>
#include "Player.h"
#include "GameServerEx.h"
#include <PublicMath.h>
#include "..\\.\\SolarTrigger\\TriggerMsg.h"
#include "TriggerManagerEx.h"

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/ChaosZoneManager.h"
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "MonsterBookParser.h" //_NA_007086_20140318_MONSTERBOOK

//--------------------------------------------------------------------------------------------------


Monster::Monster()
	: m_bSummoned(FALSE)
	, m_LiveIdle( util::SimpleTimer() )
{
	SetObjectType( MONSTER_OBJECT );
}


//--------------------------------------------------------------------------------------------------


Monster::~Monster()
{

}


//--------------------------------------------------------------------------------------------------


BOOL
Monster::Init()
{	
	WzVector wzVec = { 0, 0, 0 };
	SetRegenPos( wzVec );

	return NPC::Init();
}


//--------------------------------------------------------------------------------------------------


void Monster::InitMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID )
{
	this->Init();
	m_LinkedAction = AppliedNpcDependencyChain::Instance()->GetNpcLinkAction( NPCCode, MONSTER_OBJECT );
	NPC::SetBaseInfo( NPCCode, byMoveType, dwMoveAreaID );
	
}


//--------------------------------------------------------------------------------------------------


VOID Monster::Release()
{
	NPC::Release();
}


//--------------------------------------------------------------------------------------------------


BOOL Monster::Update( DWORD dwDeltaTick )
{
	if( m_bSummoned && m_LiveIdle.IsExpired() )
	{
		OnDead();
	}

	return NPC::Update( dwDeltaTick ); 
}


//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------


// ��Ȳ ����
BOOL
Monster::IsOutOfWanderRadius()
{
	if( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD )
		return FALSE;

	float fDistToRegenPos = GetDist2( m_RegenPos );

	// TODO: �켱 �ڽ��� ������ �����κ��� 50m �������� ���������� ��� ������ �Ѵ�.
	AI_TYPE_INFO* pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(GetBaseInfo()->m_wAICode);
	if( fDistToRegenPos > pAITypeInfo->m_fWANDER_RADIUS_FROM_REGENAREA*pAITypeInfo->m_fWANDER_RADIUS_FROM_REGENAREA )
	{
		return TRUE;
	}

	return FALSE;
}


//--------------------------------------------------------------------------------------------------

//	�̰Ŵ� �ο�ٸ��� ������ ���ư��� �Ǵ� �ִ� ����
BOOL
Monster::IsOutOfRegenLocationLimit( WzVector &vTargetPos )
{
	if( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD )
		return FALSE;

	// 50m�� ����� ������ ���ư���.
	float fDistToRegenPos = GetDist2( m_RegenPos );
	AI_TYPE_INFO* pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(GetBaseInfo()->m_wAICode);
	if( fDistToRegenPos > pAITypeInfo->m_fREGEN_LOCATION_LIMIT*pAITypeInfo->m_fREGEN_LOCATION_LIMIT )
		return TRUE;

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    // ���� ���� üũ
    float fHeightDiff = fabs( GetHeightDiff( m_RegenPos ) );
    if( fHeightDiff > pAITypeInfo->m_fMOVE_HEIGHT_LIMIT )
        return TRUE;
#else
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	if( !stAICommonInfo.m_fMOVE_HEIGHT_LIMIT )	return FALSE;

	// ���� ���� üũ
	float fHeightDiff = fabs( GetHeightDiff( m_RegenPos ) );
	if( fHeightDiff > stAICommonInfo.m_fMOVE_HEIGHT_LIMIT )
		return TRUE;
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

	return FALSE;
}


//--------------------------------------------------------------------------------------------------


VOID
Monster::OnDead()
{
	if( !GetField() )
		return;

	if( m_bSummoned && !m_LiveIdle.GetExpireTime() )
	{
		m_bSummoned = 0;
		NPC::OnDead();
		return;
	}

	// ������ �̺�Ʈ ����
	if( GetGameZonePtr() )
		GetGameZonePtr()->OnKillMonster( this );

	//////////////////////////////////////////////////////////////////////////
	//	(WAVERIX)(THINKING)
	//	-	SSQ������ ���� ���� �� �������� �ٸ� ó���� �䱸�Ѵ�...
	//		���⼭ ����ó���� �ϱ� ���ٴ� Monster����� ���� ���̴�.
	//	-	�ִ��� Monster�� ������ �ڵ带 ����ϱ� ���� ��ü Ÿ�Կ� ����
	//		
	//	<NAME>
	//	-	SSQMonster -> Monster
	//////////////////////////////////////////////////////////////////////////

	// ���� �Ŀ� ������ �� ��� ����ġ�� �������� ���� �ʴ´�.
	const BOOL bTransform = IsSpecialAction( eNPC_SPECIAL_ACTION_TRANSFORMATION );

	// ����ġ, �����ۿ� ���� �������ڸ� ���Ѵ�.(�������� Player�� ������ ����ġ�� ���̳� �뺴�� �������� �𸥴�...)
	Character* pExpOwner = g_ItemExpDistributor.DecideExpOwnership( this );
	Player* pItemOwner = g_ItemExpDistributor.DecideItemOwnership( this );

	if( pExpOwner && !bTransform )
	{
		// ����ġ�� �й��Ѵ�.
		g_ItemExpDistributor.DistributeExp( this, pExpOwner );
	}

	if( pItemOwner && !bTransform )
	{
		// �������� �����Ų��.
		BYTE byDropRatio = pItemOwner->GetFatigueManager()->GetItemDropRatio();
		StatusField& rStatus = pItemOwner->GetStatusManager()->Status;
		if( rStatus.FIELD & rStatus.ENABLE_GM_DROP_LIST_ALL )
			byDropRatio = 100;

		if( random(1, 100) <= byDropRatio )
		{
			// �������� ���� Ȯ������� �����ϸ� �� Ȯ�� ���
			WzVector vDropPos;	GetPos( &vDropPos );

			// ���� �����ۿ� ���� �������� ����!
			//g_DropManager.Drop( GetField(), GetSectorIndex(), &vDropPos, pItemOwner, GetObjectKey(), GetBaseInfo(), (eNPC_GRADE)GetBaseInfo()->m_byGrade, GetDisplayLevel() );
			g_DropManager.DelayedDrop_( GetField(), GetSectorIndex(), &vDropPos, pItemOwner, GetObjectKey(), GetBaseInfo(), (eNPC_GRADE)GetBaseInfo()->m_byGrade, GetDisplayLevel());
		}
	}

	// for Trigger & Quest & ItemDeleteEvent
	m_LinkedAction.OnDead( GetField(), this, ( pItemOwner && !bTransform ) ? pItemOwner : NULL, m_dwKillerObjectKey );

	NPC::OnDead();

	if( GetBaseInfo()->m_byGrade == eNPC_LUCKY_MONSTER || GetBaseInfo()->m_byGrade == eNPC_BOSS)
		GAMELOG->LogKillSpecialMonster(pItemOwner, GetBaseInfo()->m_MonsterCode, GetBaseInfo()->m_byGrade);

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    ChaosZoneManager* chaoszone_manager = GetChaosZoneManager();
    if (chaoszone_manager != NULL)
    {
#ifdef _NA_008334_20150608_SONNENSCHEIN
        ChaosZoneType type = chaoszone_manager->GetChaosZoneType();
        if (type == eCHAOSZONE_TYPE_GOLDRUSH || type == eCHAOSZONE_TYPE_SONNENSCHEIN )
        {
            Character* const killer = GetField()->FindCharacter(m_dwKillerObjectKey);
            chaoszone_manager->DeadCharacter(static_cast<Character*>(this), killer);
        }
#else
        if (chaoszone_manager->GetChaosZoneType() == eCHAOSZONE_TYPE_GOLDRUSH)
        {
            Character* const killer = GetField()->FindCharacter(m_dwKillerObjectKey);
            chaoszone_manager->DeadCharacter(static_cast<Character*>(this), killer);
        }
#endif //_NA_008334_20150608_SONNENSCHEIN
    }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_007086_20140318_MONSTERBOOK
    // moved to AppliedNpcDependencyChain::OnMonsterDeadProcess(...)
#endif //_NA_007086_20140318_MONSTERBOOK
}


//--------------------------------------------------------------------------------------------------


VOID
Monster::StartSummonedTimer(DWORD time)
{
	if( time != 0 )
	{
		m_bSummoned = TRUE;
		m_LiveIdle.SetTimer( time );
	}
	else
		m_bSummoned = FALSE;
}



