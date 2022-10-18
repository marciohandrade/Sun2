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
			������ �̼� ����� �����ϴ� Ŭ����. (�̼� Ŭ����� �ְ����� ���)
	@description
			- �̼� �濡 �ϳ��� MissionRecordManager�� �����Ѵ�.
			- 
	@todo
			- �ð������� int�̱� ������ �ִ�ð��� ���Ѵٸ� Ÿ���� ���� �� �ִ�.
			- Ȥ�� Ŭ���� �ߴµ� ��� ������ ���� �����̸�? �켱 update�� ���� ����
	@history
			- 2006.8.29 : �ǽð� �̼� ��ŷ ��ȹ �������� ���� ����
							
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

	// �̼� ���۽� ȣ��
	void StartMission( RANKINGNO wMissionNo );

	// ��� �� �÷��̾ ����� �ð��� ������.
	// ����, ���� �÷��̾�(Ÿ����������)�� �ٽ� ��Ȱ���� �� ����� �ð��� ������ �� ���
	// ���⼭ Ŭ���̾�Ʈ�� Ÿ�̸Ӹ� ����
	void SendElapesedTime( Player * pPlayer );

	// �̼� �Ϸ�� ȣ��
	void ClearMission( GameZone * pZone );

	// DB���
	// ClearTime broadcast : ���� ��� ����
	// ���κ� �ְ� ��� DB Update : ���� ��� ����
	void UpdateClearTime( Player * pPlayer );

	// ��Ƽ ��� DB Update : ���� ��� ����
	void UpdateClearTimeParty( GameZone * pZone );
	void ClearMissionTest( DWORD time, INT64 RegisterTime );
	void KillMonster( eNPC_GRADE grade, GameZone * pZone );

	void OnCreate();
	void OnDestroy();

private:
	__BOOL_PROPERTY( SatisfyCondition );					//< �̼� ��ŷ ���� ���� ���� üũ , (�����)
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