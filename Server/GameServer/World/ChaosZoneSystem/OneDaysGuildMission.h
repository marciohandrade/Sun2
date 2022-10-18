#pragma once

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include <World/ChaosZoneSystem/ChaosZone.h>

/*--------------------------------------------------------------------------------------------------
	-- Class Defined
--------------------------------------------------------------------------------------------------*/
struct StandByMemberInfo
{
	CHARGUID user_guid;                 //���� ���̵�

	StandByMemberInfo() : 
	user_guid(0)
	{
	}
};

typedef STLX_VECTOR<StandByMemberInfo*> StandByMemberList;

struct OneDaysGuildMissionInfo;
class OneDaysGuildMissionInfoParser;
class OneDaysGuildMissionResult;
class OneDaysGuildMission : public ChaosZone
{
	enum{ kSecond = 1000, };
	enum 
	{
		// �ð�����ȭ �ֱ�
		TIME_SYNC_CYCLE = 30 * kSecond,	
		ROOM_DESTROY_WAIT_TIME = 5 * kSecond,								// �� �ı� �� ��� �ð�
	}; 

public:
	OneDaysGuildMission() :
	  guild_mission_state_(eGUILDMISSION_NONE),
	  onedays_guild_mission_result_(NULL),
	  onedays_guild_mission_info_(NULL)
	  {}

	virtual ~OneDaysGuildMission() {}

public:
	virtual void Init(GameInstanceDungeon* const instance_dungeon);
	virtual void Release();
	virtual void Update();

	virtual bool EnterPlayer(Player* const player);
	virtual void LeavePlayer(Player* const player);
	virtual void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list);
	virtual eUSER_RELATION_TYPE IsFriendPVP(Player* attacker,Player* target);
	virtual void DeadCharacter(Character* const dead_character, Character* const kill_character);

	void SetOneDaysGuildMissionState(eGUILDMISSION_STATE state);
	eGUILDMISSION_STATE GetOneDaysGuildMissionState() { return guild_mission_state_; }

	// ���ΰ��� ó�� �Լ�
	VOID SuccessStamp(Player* player);

	VOID SetStampStartTime();
	VOID InitStampCheckTime() {stamp_start_time_ = 0;}

public:
	bool InsertStandByMember(const CHARGUID guid);
	void RemoveStandByMember(const CHARGUID guid);
	bool IsStandByMember(const CHARGUID guid);

private:
	void _Start();															// ����
	void _Result();															// ����
	bool _IsGuildMissionEnd	();												// ��� �̼� ���� üũ �Լ�
	void _SentToAllPlayerInfo(BYTE infoflag, CHARGUID user_guid);			// ��� �÷��̾�� ���� ����

	void _SendTimeSync();
	void _SendTimeSyncForPlayer(Player* player);

	bool _ItemDelete(Player* player);

	//packet
	void _SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);			// ����������� ��Ŷ������
	void _SendPacketToAllEx(MSG_BASE_FORWARD* msg, WORD msg_size, CHARGUID charguid);		// �� �ڽ��� �� ����������� ��Ŷ������

	virtual RC::eSTATUS_RESULT ResurrectionAtSafezone( Player *player );	// ��Ȱ ��û ó�� �Լ�

	RC::eSTATUS_RESULT OnMoveToSafezone(Player* const player);				// ��������� �̵�

	Player* _FindPlayer(const DWORD char_guid);

private:
	PLAYER_HASH player_hash_;												// �������� ���
    const OneDaysGuildMissionInfo* onedays_guild_mission_info_;				// ��ũ��Ʈ ����

	OneDaysGuildMissionResult* onedays_guild_mission_result_;				// ����

	typedef STLX_HASH_MAP<CHARGUID,StandByMemberInfo*> StandByMemberMap;
	typedef util::CMemoryPoolFactory<StandByMemberInfo> StandByMemberPool;

private:
	StandByMemberPool* stand_by_member_pool_;
	StandByMemberMap stand_by_total_members_;						// ��ŸƮ �� ���� ����Ʈ -> ��Ŵ� �� �������� �����ϰ� ����.
	
	// Time Info
	struct
	{
		eGUILDMISSION_STATE guild_mission_state_;							// ��� �̼� ���� ����
		DWORD stamp_start_time_;											// ���� �ð�
		util::Timer mission_limit_timer_;									// ���� ���̼� ���� �ð�.
		util::Timer start_wait_timer_;										// ���� ��� �ð� �� ���� �� ���� ���� ���� �ð�
		util::Timer end_wait_timer_;										// ���� ���� �� ��� ���� ���÷��� �ð�
		util::Timer room_destroy_timer_;									// �� �ı��ð�
		bool is_stamp_state_;												// ä�� ����(true�̸� ä�� ���� ����)
		util::Timer time_sync_cycle_;										// ���� ���̼� ���� �� �� �ð� ���� ��ũ Ÿ��
	};
};

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)