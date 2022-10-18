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
    // ���巩ŷ�� ��쿣 SetRankingSystemCtl�� BattleGroundLeaveRoom���� ���ֵ����Ѵ�.
    // �ٸ� ��쿣 LeaveRoom���� �Ѵ�
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
	//	(NOTE)	���� ���� ������ �̷� ������ ����ؾ� �ϴ°�? �ǹ� ��~�� ����.
	//	-	�ϴ��� �� ���̶� ������.
	// From. â�� - ��ŷ or ����Ʈ �Ŵ����� ������ ���� �ϳ��� ������ �ȴ�. ���� ���̺� ���� �� �ѹ��� ������ �ϸ� �ȴ�.
	//				�� �̷��� �ߴ°�? ���Կ��ٰ� ������ ������ ���� ������ ���̾�׷� ���� ����. �м��� �ð����� ����.
	//				��κ��� ������ �ľ� �ȴٸ� 2���� �Ŵ����� �ϳ��� �޸� Ǯ���� �Ҵ��ϵ��� �����ϰڴ�.
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
	//Ÿ�Ӿ��� ����?
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

//	(SpecialCmd) Ÿ���� ����� ���� ���� ó�� ����
//	- Ÿ���� ����� ó�� ��� (KillMonster�� ȣ����� �ʴ´�. ���� ���� ���� ���� �ϰ� ó���ȴ�.)
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