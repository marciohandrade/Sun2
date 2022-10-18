#include "stdafx.h"

#include "GameInstanceDungeon.h"

#include "Map.h"
#include "GameMissionRoom.h"

#include "Trigger.h"
#include "TriggerManagerEx.h"

#include "Player.h"
#include "Monster.h"

#include "ChangeMonsterAbility.h"
#include "StatusManager.h"

#include <World/GameWarControlManager.h>

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "InstanceDungeonBattleResult.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "GameBattleResult.h"
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRush.h"
#include "World/ChaosZoneSystem/GoldRushBattleResult.h"
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

__IMPL_CUSTOMPOOL_PTR(GameInstanceDungeon)
GameInstanceDungeon::GameInstanceDungeon(VOID)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
 : chaos_zone_type_(kChaosZoneTypeNone)
 ,chaos_zone_manager_(NULL)
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult = new InstanceDungeonBattleResult;
}

GameInstanceDungeon::~GameInstanceDungeon(VOID)
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    SAFE_DELETE(m_pBattleResult);
}

VOID GameInstanceDungeon::OnCreate()
{
	BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// 추가 타입 존재시 하단 내용 추가할 것
	ChangeHandler( eZONETYPE_INSTANCE, m_KindOfIndun );
	//	SSQControlManager에서 해당 맵코드와 키를 매핑시키기 위함
	switch( m_KindOfIndun )
	{
	case INDUN_KIND_TOP_OF_TRIAL:
    case INDUN_KIND_100LEVEL_QUEST:
	case INDUN_KIND_EXT_03:
	case INDUN_KIND_EXT_04:
	case INDUN_KIND_MAYA_SCROLL:
		break;
	case INDUN_KIND_SS_QUEST:
		GameWarControlManager::OnMsg( GameSSQCtrlCmd::Created(), this );
		break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case INDUN_KIND_CHAOS_ZONE:
        chaos_zone_manager_ = new ChaosZoneManager(this);
        chaos_zone_manager_->Init(chaos_zone_type_);
        break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	}
	GameRoom::OnCreate();

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult->Init(false);
}


VOID GameInstanceDungeon::OnDestroy()
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
    _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "MapCode:%d,TotalHeim:%I64u", GetMapCode(), 
        (m_pBattleResult != NULL) ? m_pBattleResult->GetTotalDropHeim() : 0);
    GAMELOG->LogWzDat(ACT_TOTAL_DROP_HEIM_FROM_MONSTER, tszAddInfo);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
    m_pBattleResult->Release();

	BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// 추가 타입 존재시 하단 내용 추가할 것
	switch( m_KindOfIndun )
	{
	case INDUN_KIND_TOP_OF_TRIAL:
	case INDUN_KIND_100LEVEL_QUEST:
	case INDUN_KIND_EXT_03:
	case INDUN_KIND_EXT_04:
	case INDUN_KIND_MAYA_SCROLL:
		break;
	case INDUN_KIND_SS_QUEST:
		GameWarControlManager::OnMsg( GameSSQCtrlCmd::Destroy(), this );
		break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case INDUN_KIND_CHAOS_ZONE:
        SAFE_RELEASENDELETE(chaos_zone_manager_);
        chaos_zone_type_ = kChaosZoneTypeNone;
        break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	}
	GameRoom::OnDestroy();
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
void GameInstanceDungeon::Update(DWORD dwDeltaTick)
{
    GameRoom::Update(dwDeltaTick);

    switch (m_KindOfIndun)
    {
    case INDUN_KIND_CHAOS_ZONE:
        if (chaos_zone_manager_)
        {
            chaos_zone_manager_->Update();
        }
        break;
    }
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

VOID GameInstanceDungeon::Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_INSTNACE, this, FieldCode, AreaID, pwvStartPos );
}

VOID GameInstanceDungeon::SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_INSTANCE, this, FieldCode, AreaID, pwvStartPos );
}

BOOL GameInstanceDungeon::MoveField( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::MoveField( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
		return FALSE;
	}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (chaos_zone_type_ != kChaosZoneTypeNone)
    {
        if (chaos_zone_manager_ && 
            chaos_zone_manager_->EnterPlayer(pPlayer) == false)
        {
            pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
            return false;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return TRUE;
}

BOOL GameInstanceDungeon::Join( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_INSTNACE ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameInstanceDungeon::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

	if( !GameRoom::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );

		// 미션의 수행정보를 보내준다!!

		return FALSE;
	}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (chaos_zone_type_ != kChaosZoneTypeNone)
    {
        if (chaos_zone_manager_ && 
            chaos_zone_manager_->EnterPlayer(pPlayer) == false)
        {
            GameRoom::Leave(pPlayer);
            pPlayer->SetGameZone(ePRS_NOT_AT_ZONE, NULL, 0, NULL);
            return false;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return TRUE;
}

VOID GameInstanceDungeon::LeaveForWarp( Player* pPlayer )
{
	GameRoom::LeaveForWarp( pPlayer );
    
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (chaos_zone_type_ != kChaosZoneTypeNone)
    {
        if (chaos_zone_manager_)
        {
            chaos_zone_manager_->LeavePlayer(pPlayer);
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
}

VOID GameInstanceDungeon::Leave( Player* pPlayer )
{
	GameRoom::Leave( pPlayer );

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (chaos_zone_type_ != kChaosZoneTypeNone)
    {
        if (chaos_zone_manager_)
        {
            chaos_zone_manager_->LeavePlayer(pPlayer);
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
}

VOID GameInstanceDungeon::OnKillMonster( Monster* pMonster )
{
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	const BOOLEAN bNeedKillEvent
		=   (Policyment.FIELD & Policyment.ENABLE_RANKING)
		&& !(Policyment.FIELD & Policyment.ENABLE_SSQ_CONTROL)
		;

	if( bNeedKillEvent )
		m_pRankingSystemCtl->KillMonster( (eNPC_GRADE)pMonster->GetBaseInfo()->m_byGrade, this );
#endif // _NA_008012_20150130_RANKING_SYSTEM
}

GameBattleResult* GameInstanceDungeon::GetGameBattleResult() const
{
    if (chaos_zone_manager_ && chaos_zone_type_ == kBattleGround)
    {
        BattleGround* const battle_ground = \
            static_cast<BattleGround*>(chaos_zone_manager_->GetCurrentZone());
        if (battle_ground)
            return static_cast<GameBattleResult*>(battle_ground->GetBattleGroundBattleResult());
    }
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    else if (chaos_zone_manager_ && chaos_zone_type_ == eCHAOSZONE_TYPE_GOLDRUSH)
    {
        GoldRush* const goldrush = \
            static_cast<GoldRush*>(chaos_zone_manager_->GetCurrentZone());
        if (goldrush)
            return static_cast<GameBattleResult*>(goldrush->GetGoldRushBattleResult());
        
    }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    else
        return m_pBattleResult;

    return NULL;
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
RC::eSTATUS_RESULT GameInstanceDungeon::ResurrectionAtSafezone( Player* pPlayer )
{
    if (m_KindOfIndun != INDUN_KIND_CHAOS_ZONE || chaos_zone_manager_ == NULL) {
        return RC::RC_STATUS_RESURRECTION_FAIL;
    }

    return chaos_zone_manager_->ResurrectionAtSafezone(pPlayer);
}
#endif //_NA_008334_20150608_SONNENSCHEIN