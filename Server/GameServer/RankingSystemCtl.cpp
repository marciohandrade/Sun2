#include "Stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "RankingSystemCtl.h"
#include "GameRoom.h"
#include "Player.h"
#include "GameRankingManager.h"

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#include "BattleGroundRankingManager.h"
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

RankingSystemCtl::RankingSystemCtl()
{
	m_pGameRoom = NULL;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    battleground_ranking_manager = NULL;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}

VOID RankingSystemCtl::OnCreate( GameRoom* pGameRoom )
{
	ASSERT(!m_pGameRoom);
	m_pGameRoom = pGameRoom;
	
    m_RankingType = g_GameRankingMgr.GetRankingType( m_pGameRoom->GetMapCode() );

	m_MissionRecordMgr.OnCreate();
	m_MissionPointMgr.OnCreate();

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    if (m_RankingType >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
        battleground_ranking_manager = new BattleGroundRankingManager();
    }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}
VOID RankingSystemCtl::OnDestroy()
{
	m_MissionRecordMgr.OnDestroy();
	m_MissionPointMgr.OnDestroy();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    if(m_RankingType >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
        SAFE_DELETE(battleground_ranking_manager);
    }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	m_pGameRoom = NULL;
	m_RankingType = eRANKING_TYPE_NONE;
}

VOID RankingSystemCtl::JoinRoom( Player * pPlayer, MAPCODE MapCode )
{
	switch( m_RankingType )
	{
	case eRANKING_TYPE_TIME_ATTACK:
		m_MissionRecordMgr.StartMission( MapCode );
		m_MissionRecordMgr.SendElapesedTime( pPlayer );
		break;
	case eRANKING_TYPE_POINT:
		m_MissionPointMgr.JoinPlayer( pPlayer, MapCode );
		break;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    case eRANKING_TYPE_BATTLEGROUND_WIN:
    case eRANKING_TYPE_BATTLEGROUND_KILL:
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        battleground_ranking_manager->EnterPlayer(pPlayer, MapCode);
        break;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	}

	pPlayer->SetRankingSystemCtl( this );
}

VOID RankingSystemCtl::LeaveRoom( Player * pPlayer)
{
    // 전장랭킹의 경우엔 SetRankingSystemCtl을 BattleGroundLeaveRoom에서 해주도록한다.
    // 다른 경우엔 LeaveRoom에서 한다
    switch( m_RankingType )
    {
    case eRANKING_TYPE_TIME_ATTACK:
        pPlayer->SetRankingSystemCtl( NULL );
        break;
    case eRANKING_TYPE_POINT:
        m_MissionPointMgr.LeavePlayer( pPlayer, m_pGameRoom->GetMapCode() );
        pPlayer->SetRankingSystemCtl( NULL );
        break;
    }
}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
VOID RankingSystemCtl::BattleGroundLeaveRoom( Player * pPlayer, const BattleRecorder::Record* record , 
                                             bool battleground_is_win)
{
    switch( m_RankingType )
    {
    case eRANKING_TYPE_BATTLEGROUND_WIN:
    case eRANKING_TYPE_BATTLEGROUND_KILL:
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        battleground_ranking_manager->BattleGround_LeavePlayer(pPlayer, m_pGameRoom->GetMapCode(), 
            record, battleground_is_win);
        break;
    }

    pPlayer->SetRankingSystemCtl( NULL );
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

//KEYWORD SHOGEN WILL MODIFY
VOID RankingSystemCtl::KillMonster( eNPC_GRADE grade, GameZone * pZone )
{
	//	(NOTE)	애초 죽일 때마다 이런 식으로 계산해야 하는가? 의미 전~혀 없다.
	//	-	일단은 한 번이라도 줄이자.
	// From. 창현 - 랭킹 or 포인트 매니저도 생성시 둘중 하나만 가져도 된다. 몬스터 테이블도 생성 시 한번만 가지게 하면 된다.
	//				왜 이렇게 했는가? 신입에다가 남겨진 문서는 없고 시퀀스 다이어그램 조차 없다. 분석할 시간조차 없다.
	//				대부분의 구조가 파악 된다면 2개의 매니저중 하나를 메모리 풀에서 할당하도록 수정하겠다.
	const MAPCODE mapCode = m_pGameRoom->GetMapCode();
	switch( m_RankingType )
	{
	case eRANKING_TYPE_TIME_ATTACK:
		m_MissionRecordMgr.KillMonster( grade, pZone );
		break;
	case eRANKING_TYPE_POINT:
		m_MissionPointMgr.KillMonster( grade, pZone, g_GameRankingMgr.GetPointRankingIndex( mapCode ) );
		break;
	}
}


VOID RankingSystemCtl::ClearMission( GameZone * pZone )
{
	switch( m_RankingType )
	{
	case eRANKING_TYPE_TIME_ATTACK:
		m_MissionRecordMgr.ClearMission( pZone );
		break;
	}
}

/*
VOID RankingSystemCtl::SendElapesedTime( Player * pPlayer )
{
	//타임어택 적용?
	if( m_pGameRoom->GetMAPINFO()->bTimeAttack )
	{
		m_MissionRecordMgr.SendElapesedTime( pPlayer );
	}	
}
*/


VOID RankingSystemCtl::UpdateClearTime( Player * pPlayer )
{
	switch( m_RankingType )
	{
	case eRANKING_TYPE_TIME_ATTACK:
		m_MissionRecordMgr.UpdateClearTime( pPlayer );
		break;
	}
}


VOID RankingSystemCtl::UpdateClearTimeParty( GameZone * pZone )
{
	switch( m_RankingType )
	{
	case eRANKING_TYPE_TIME_ATTACK:
		m_MissionRecordMgr.UpdateClearTimeParty( pZone );
		break;
	}
}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
VOID RankingSystemCtl::SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey, RANKINGTYPE ranking_type /*= 0*/ )
{
    switch( ranking_type )
    {
    case eRANKING_TYPE_POINT:
        m_MissionPointMgr.SetPlayerPoint( UserGUID, Point, DBKey );
        break;
    case eRANKING_TYPE_BATTLEGROUND_WIN:
    case eRANKING_TYPE_BATTLEGROUND_KILL:
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        battleground_ranking_manager->SetPlayerPoint(UserGUID, Point, DBKey, ranking_type);
        break;
    }
}
#else
VOID RankingSystemCtl::SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey )
{
    switch( m_RankingType )
    {
    case eRANKING_TYPE_POINT:
        m_MissionPointMgr.SetPlayerPoint( UserGUID, Point, DBKey );
        break;
    }
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

//	(SpecialCmd) 타락한 사원용 최종 정산 처리 목적
//	- 타락한 사원용 처리 방법 (KillMonster는 호출되지 않는다. 최종 계산된 값을 통해 일괄 처리된다.)
VOID
RankingSystemCtl::SetLastSettleAccount(
								Player* const pPlayer
								, const WORD totalPoint
								)
{
	sPOLICY_ZONE& rPolicyment = m_pGameRoom->Policyment;
	if( !(rPolicyment.FIELD & rPolicyment.ENABLE_SSQ_CONTROL) )
		return;

	const MAPCODE mapCode = m_pGameRoom->GetMapCode();
	switch( m_RankingType )
	{
	case eRANKING_TYPE_POINT:
		m_MissionPointMgr.SetLastSettleAccount(
					pPlayer
					, mapCode
					, g_GameRankingMgr.GetPointRankingIndex( mapCode ), totalPoint
					);
		break;
	}
}

#ifdef _NA_001990_ACSYSTEM_ADD
WORD RankingSystemCtl::GetMissionClearTime()
{
    return (WORD)(m_MissionRecordMgr.GetMissionClearTime() / 1000);
}
#endif //_NA_001990_ACSYSTEM_ADD
#endif // _NA_008012_20150130_RANKING_SYSTEM