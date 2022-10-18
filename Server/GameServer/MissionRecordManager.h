#ifdef _NA_008012_20150130_RANKING_SYSTEM
#pragma once
//=======================================================================================================================
/// MissionRecordManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 5. 4 ~
	@purpose
			개인의 미션 기록을 관리하는 클래스. (미션 클리어시 최고기록을 경신)
	@description
			- 미션 방에 하나의 MissionRecordManager가 존재한다.
			- 
	@todo
			- 시간단위가 int이기 때문에 최대시간을 정한다면 타입을 줄일 수 있다.
			- 혹시 클리어 했는데 모든 유저가 죽은 상태이면? 우선 update는 하지 않음
	@history
			- 2006.8.29 : 실시간 미션 랭킹 기획 변경으로 새로 제작
							
*/
//=======================================================================================================================

#include <PacketStruct_DG.h>
#include <PacketStruct_GP.h>
#include <RankingDefine.h>

//struct MISSION_RECORD_INFO;
class GameZone;

class MissionRecordManager
{
public:
	MissionRecordManager(void);
	~MissionRecordManager(void);

	void Clear();
//	void Save( const MISSION_RECORD_INFO * pRecordInfo, BYTE IN Num );
//	BYTE Load( MISSION_RECORD_INFO * pRecordInfo, BYTE IN MaxArrayNum );

	DWORD DecreasingTime( DWORD desc_time );

	// 미션 시작시 호출
	void StartMission( RANKINGNO wMissionNo );

	// 들어 온 플레이어에 경과된 시간을 보낸다.
	// 추후, 죽은 플레이어(타임이정지된)가 다시 부활했을 때 경과된 시간을 보내줄 때 사용
	// 여기서 클라이언트는 타이머를 돌림
	void SendElapesedTime( Player * pPlayer );

	// 미션 완료시 호출
	void ClearMission( GameZone * pZone );

	// DB기록
	// ClearTime broadcast : 죽은 사람 제외
	// 개인별 최고 기록 DB Update : 죽은 사람 제외
	void UpdateClearTime( Player * pPlayer );

	// 파티 기록 DB Update : 죽은 사람 제외
	void UpdateClearTimeParty( GameZone * pZone );
	void ClearMissionTest( DWORD time, INT64 RegisterTime );
	void KillMonster( eNPC_GRADE grade, GameZone * pZone );

	void OnCreate();
	void OnDestroy();

private:
	__BOOL_PROPERTY( SatisfyCondition );					//< 미션 랭킹 조건 만족 여부 체크 , (입장시)
	__PROPERTY( DWORD, ConditionErrorCode );
	__PROPERTY( RANKINGNO, CurMissionNo );
	__PROPERTY( DWORD, CurMissionStartTime );
	__PROPERTY( DWORD, MissionClearTime );
	
	enum {
		COUNTING_MONSTER_TYPE_GENERAL = 0,
		COUNTING_MONSTER_TYPE_LEADER,
		COUNTING_MONSTER_TYPE_NUM,
	};
	DWORD m_MonsterKillNum[COUNTING_MONSTER_TYPE_NUM];
	__PROPERTY( DWORD, TotalMonsterKillNum );
	__PROPERTY( DWORD, MinLimitedMonsterKillNum );
	__PROPERTY( BOOL , bStartMissionTime );
	//MSG_DG_MISSION_CLEAR_UPDATE_SYN m_PartyUpdateMsg;

	MSG_GP_RANKING_MISSION_CLEAR_SYN m_MissionRankingMsg;
};
#endif // _NA_008012_20150130_RANKING_SYSTEM