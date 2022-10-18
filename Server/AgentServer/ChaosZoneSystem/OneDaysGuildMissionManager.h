#pragma once

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include <Singleton.h>
#include "ChaosZoneSystem/Handler_CG_ONEDAYSGUILDMISSION.h"
#include "ChaosZoneSystem/Handler_AG_ONEDAYSGUILDMISSION.h"

/*--------------------------------------------------------------------------------------------------
	-- Class Defined
--------------------------------------------------------------------------------------------------*/
class OneDaysGuildMissionRoom;

class OneDaysGuildMissionManager : public util::Singleton<OneDaysGuildMissionManager>
{
	typedef std::map<KEYTYPE, OneDaysGuildMissionRoom*> ONEDAYSGUILDMISSION_ROOM_MAP;

	enum {UPDATE_TIME = 500};																		//0.5초마다 업데이트 진행

public:
	OneDaysGuildMissionManager() : OneDaysGuildMission_mapcode_(0) {}
	~OneDaysGuildMissionManager() {}

public:
	void Init();
	void DestroyRoom(KEYTYPE zone_key);
	void DestroyRoom(GUILDGUID guild_guid);
	void Update();

	bool AllPlayerLeaveRoom(const ZONEKEY zone_key);
	bool LeaveRoom(const ZONEKEY zone_key, User* const user);

	MAPCODE GetOneDaysGuildMissionMapCode();

	RC::eONEDAYSGUILDMISSION_RESULT CreateOneDaysGuildMissionRoom(User* user, BOOL is_gm_cmd = FALSE);
	RC::eONEDAYSGUILDMISSION_RESULT UserCanCreateARoom(User* user, BOOL is_gm_cmd = FALSE);			//유저가 방을 생성할 수 있는가?
	RC::eONEDAYSGUILDMISSION_RESULT CanAUserEntersTheRoom(User* user, BOOL is_gm_cmd = FALSE);		//유자가 방에 입장할 수 있는가?

private:
	//기본적인 입장/생성 가능여부 검사
	RC::eONEDAYSGUILDMISSION_RESULT _CheckUserProcessOneDaysGuildMission(User* user);

public:
	OneDaysGuildMissionRoom* FindRoomUsingGuildGuid(GUILDGUID guild_guid);
	OneDaysGuildMissionRoom* FindRoomUsingRoomKey(KEYTYPE room_key);
	KEYTYPE GetZoneKeyUsingGuildGuid(GUILDGUID guild_guid);
	
	BOOL GetEnterPlayerList_CharGuid(GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT);
	BOOL GetStandByUserList_CharGuid(GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT);
	BOOL MoveToOneDaysGuildMissionRoom(User* user, BOOL is_gm_cmd = FALSE);

public:
	void ProcessPacket_CG(User* user, MSG_BASE* packet, WORD size);
	void ProcessPacket_AG(GameServerSession* server_session, MSG_BASE* packet, WORD size);

private:
	//Packet Handler
	Handler_CG_ONEDAYSGUILDMISSION handler_cg_onedaysguildmission_;
	Handler_AG_ONEDAYSGUILDMISSION handler_ag_onedaysguildmission_;

	ONEDAYSGUILDMISSION_ROOM_MAP room_map_;

	MAPCODE OneDaysGuildMission_mapcode_;
};

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)