#include "StdAfx.h"
#include ".\gamemissionroom.h"
#include "Player.h"
#include "Trigger.h"
#include "Map.h"
#include "ChangeMonsterAbility.h"
#include "Monster.h"
#include "TriggerManagerEx.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "MissionBattleResult.h"


__IMPL_CUSTOMPOOL_PTR(GameMissionRoom)
GameMissionRoom::GameMissionRoom(void)
{
    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult = new MissionBattleResult;
}

GameMissionRoom::~GameMissionRoom(void)
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    SAFE_DELETE(m_pBattleResult);
}

void GameMissionRoom::OnCreate()
{
	ChangeHandler( eZONETYPE_MISSION );
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    // 하드모드에서는 파티 인원수에 따른 몬스터의 능력치(공격력/HP) 변화 없음 - arycoat 2021.01.20
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    if(GetPartyZoneType() == PartyOfHardcore ||
       GetPartyZoneType() == PartyOfSpecialMission) {
        Policyment.SetField(Policyment.FIELD & ~Policyment.ENABLE_CHANGE_MOB_ABILITY);
    }
#else
    if(GetPartyZoneType() == PartyOfHardcore) {
        Policyment.SetField(Policyment.FIELD & ~Policyment.ENABLE_CHANGE_MOB_ABILITY);
    }
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

    PartyZoneType zonetype = GetPartyZoneType();
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
    if (zonetype == PartyOfTopOfTrialLight_cash ||
        zonetype == PartyOfTopOfTrialLight_normal ) {
        Policyment.SetField(Policyment.FIELD & ~Policyment.ENABLE_CHANGE_MOB_ABILITY);
    }
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT

	GameRoom::OnCreate();

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult->Init(false);
}

void GameMissionRoom::OnDestroy()
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
    _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "MapCode:%d,TotalHeim:%I64u", GetMapCode(), 
        (m_pBattleResult != NULL) ? m_pBattleResult->GetTotalDropHeim() : 0);
    GAMELOG->LogWzDat(ACT_TOTAL_DROP_HEIM_FROM_MONSTER, tszAddInfo);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
    
    m_pBattleResult->Release();

	GameRoom::OnDestroy();
}

VOID GameMissionRoom::Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_MISSION, this, FieldCode, AreaID, pwvStartPos );
}

void GameMissionRoom::SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_MISSION, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameMissionRoom::MoveField( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::MoveField( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
		return FALSE;
	}

	return TRUE;
}

BOOL GameMissionRoom::Join( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_MISSION ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameMissionRoom::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );

		// 미션의 수행정보를 보내준다!!

		return FALSE;
	}

    return TRUE;
}

VOID GameMissionRoom::LeaveForWarp( Player* pPlayer )
{
	GameRoom::LeaveForWarp( pPlayer );
}

VOID GameMissionRoom::Leave( Player* pPlayer )
{
    GameRoom::Leave( pPlayer );
}

void GameMissionRoom::OnKillMonster( Monster* pMonster )
{
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	if( Policyment.FIELD & Policyment.ENABLE_RANKING )	// SSQ control은 필요없는 곳이다.
		m_pRankingSystemCtl->KillMonster( (eNPC_GRADE)pMonster->GetBaseInfo()->m_byGrade, this );

	return;
#endif // _NA_008012_20150130_RANKING_SYSTEM
	/*
	DWORD desc_time = 0; 
	switch( pMonster->GetBaseInfo()->m_byGrade )
	{
	case eNPC_GENERAL:
		desc_time = 1000; break;
	case eNPC_LEADER:
		desc_time = 2000; break;
	default:
		return;
	}

	MSG_CG_MISSION_TIMER_CHANGED_BRD msg;
	msg.m_MissionElapsedTimePerSecond = (DWORD)(GetMissionRecordManager().DecreasingTime(desc_time)/1000);
	SendPacketToAll( &msg, sizeof(msg) );
	*/
}







