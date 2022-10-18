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
	CHARGUID user_guid;                 //유저 아이디

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
		// 시간동기화 주기
		TIME_SYNC_CYCLE = 30 * kSecond,	
		ROOM_DESTROY_WAIT_TIME = 5 * kSecond,								// 룸 파괴 전 대기 시간
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

	// 각인관련 처리 함수
	VOID SuccessStamp(Player* player);

	VOID SetStampStartTime();
	VOID InitStampCheckTime() {stamp_start_time_ = 0;}

public:
	bool InsertStandByMember(const CHARGUID guid);
	void RemoveStandByMember(const CHARGUID guid);
	bool IsStandByMember(const CHARGUID guid);

private:
	void _Start();															// 시작
	void _Result();															// 종료
	bool _IsGuildMissionEnd	();												// 길드 미션 종료 체크 함수
	void _SentToAllPlayerInfo(BYTE infoflag, CHARGUID user_guid);			// 모든 플레이어에게 정보 전달

	void _SendTimeSync();
	void _SendTimeSyncForPlayer(Player* player);

	bool _ItemDelete(Player* player);

	//packet
	void _SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);			// 모든유저에게 패킷보내기
	void _SendPacketToAllEx(MSG_BASE_FORWARD* msg, WORD msg_size, CHARGUID charguid);		// 내 자신을 뺀 모든유저에게 패킷보내기

	virtual RC::eSTATUS_RESULT ResurrectionAtSafezone( Player *player );	// 부활 요청 처리 함수

	RC::eSTATUS_RESULT OnMoveToSafezone(Player* const player);				// 안전지대로 이동

	Player* _FindPlayer(const DWORD char_guid);

private:
	PLAYER_HASH player_hash_;												// 진행중인 멤버
    const OneDaysGuildMissionInfo* onedays_guild_mission_info_;				// 스크립트 정보

	OneDaysGuildMissionResult* onedays_guild_mission_result_;				// 보상

	typedef STLX_HASH_MAP<CHARGUID,StandByMemberInfo*> StandByMemberMap;
	typedef util::CMemoryPoolFactory<StandByMemberInfo> StandByMemberPool;

private:
	StandByMemberPool* stand_by_member_pool_;
	StandByMemberMap stand_by_total_members_;						// 스타트 업 유저 리스트 -> 요거는 방 라이프와 동일하게 간다.
	
	// Time Info
	struct
	{
		eGUILDMISSION_STATE guild_mission_state_;							// 길드 미션 관련 상태
		DWORD stamp_start_time_;											// 각인 시간
		util::Timer mission_limit_timer_;									// 일일 길드미션 제한 시간.
		util::Timer start_wait_timer_;										// 입장 대기 시간 및 입장 후 강제 게임 시작 시간
		util::Timer end_wait_timer_;										// 게임 종료 후 결과 관련 디스플레이 시간
		util::Timer room_destroy_timer_;									// 방 파괴시간
		bool is_stamp_state_;												// 채집 상태(true이면 채집 성공 상태)
		util::Timer time_sync_cycle_;										// 일일 길드미션 상태 값 및 시간 관련 싱크 타임
	};
};

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)