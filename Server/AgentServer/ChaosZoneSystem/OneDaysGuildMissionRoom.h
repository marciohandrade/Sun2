#pragma once

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/

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

class InstanceDungeon;

class OneDaysGuildMissionRoom
{
public:
	OneDaysGuildMissionRoom(KEYTYPE roomkey, GUILDGUID guildguid, BYTE maxusernum):
	room_key_(roomkey),
	guild_guid_(guildguid),
	stand_by_user_cnt(0),
	onedaysguildmission_state_(eGUILDMISSION_NONE),
	remind_time_(0),
	standbymember_init_check(false),
	max_user_num(maxusernum)
	{
		stand_by_member_pool_ = new StandByMemberPool();

		stand_by_member_pool_->Initialize(max_user_num);
	}

	~OneDaysGuildMissionRoom();

public:
	void Release();

public:
	const GUILDGUID GetGuildGuid() { return guild_guid_; }
	const KEYTYPE GetZoneKey() { return room_key_; }
	INT GetPlayerList_CharGuid(CHARGUID* list OUT);					// return enter_player_count
	INT Getstand_by_user_list_CharGuid(CHARGUID* list OUT);			// return stand_by_user_count
	User* FindUser(DWORD player_key);
	void KickAllUsers();
	void _KickUser(User* user);

	bool InsertStandByMember(const CHARGUID guid);
	void RemoveStandByMember(const CHARGUID guid);
	bool IsStandByMember(const CHARGUID guid);
	BYTE GetStandByMemberCnt() { return stand_by_user_cnt; }
	bool GetStandByMemberInitCheck() { return standbymember_init_check; }

	void GetCurrentPlayerCnt(INT& current_player_cnt);

	bool IsMaxStandByMemberCnt() { return (max_user_num <= stand_by_user_cnt) ? true : false; }

	void SetOneDaysGuildMissionState(BYTE state);
	BYTE GetOneDaysGuildMissionState() { return onedaysguildmission_state_; }

	void SetOneDaysGuildMissionRemindTime(DWORD remind_time) { remind_time_ = remind_time; }
	DWORD GetOneDaysGuildMissionRemindTime() { return (remind_time_ < 0) ? 0 : remind_time_; }
private:
	typedef STLX_HASH_MAP<CHARGUID,StandByMemberInfo*> StandByMemberMap;
	typedef util::CMemoryPoolFactory<StandByMemberInfo> StandByMemberPool;

	StandByMemberPool* stand_by_member_pool_;
	StandByMemberMap stand_by_total_members_;						// 스타트 업 유저 리스트 -> 요거는 방 라이프와 동일하게 간다.
	BYTE stand_by_user_cnt;											// 스타트 업 유저 리스트 개수
	KEYTYPE room_key_;												// 방 고유 키
	GUILDGUID guild_guid_;											// 해당 룸의 길드 고유번호
	BYTE onedaysguildmission_state_;								// 미션 상태
	DWORD remind_time_;												// 남은 시간
	bool standbymember_init_check;									// 스타트 유저 리스트 갱신 시점 체크
	BYTE max_user_num;												// 입장 가능 최대 유저수
};

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)