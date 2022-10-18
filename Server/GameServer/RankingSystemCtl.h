#ifdef _NA_008012_20150130_RANKING_SYSTEM
#pragma once

#include "MissionRecordManager.h"
#include "MissionPointManager.h"
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#include "BattleRecorder.h"

class BattleGroundRankingManager;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
class GameRoom;

class RankingSystemCtl
{
public:
	RankingSystemCtl();
    VOID					OnCreate( GameRoom* pGameRoom );
	VOID					OnDestroy();
	VOID					JoinRoom( Player * pPlayer, MAPCODE MapCode );
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    VOID					BattleGroundLeaveRoom( Player * pPlayer, const BattleRecorder::Record* record, 
                                                         bool battleground_is_win);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	VOID					LeaveRoom( Player * pPlayer );
	VOID					KillMonster( eNPC_GRADE grade, GameZone * pZone );
	VOID					ClearMission( GameZone * pZone );
	///VOID					SendElapesedTime( Player * pPlayer );
	VOID					UpdateClearTime( Player * pPlayer );
	VOID					UpdateClearTimeParty( GameZone * pZone );
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	VOID					SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey, RANKINGTYPE ranking_type = 0 );
#else
	VOID					SetPlayerPoint( USERGUID UserGUID, DWORD Point, RANKINGKEY DBKey );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	//	(SpecialCmd) 타락한 사원용 최종 정산 처리 목적
	//	- 타락한 사원용 처리 방법 (KillMonster는 호출되지 않는다. 최종 계산된 값을 통해 일괄 처리된다.)
	VOID					SetLastSettleAccount(
										Player* const pPlayer
										, const WORD totalPoint
										);

#ifdef _NA_001990_ACSYSTEM_ADD
    WORD                    GetMissionClearTime();
#endif //_NA_001990_ACSYSTEM_ADD

private:
	GameRoom*				m_pGameRoom;
	eRANKING_SYSTEM_TYPE	m_RankingType;
	MissionRecordManager	m_MissionRecordMgr;
	MissionPointManager		m_MissionPointMgr;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    BattleGroundRankingManager* battleground_ranking_manager;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
};

#endif // _NA_008012_20150130_RANKING_SYSTEM