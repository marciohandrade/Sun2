#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include <Singleton.h>
class GameServerSession;
class GameChaosZoneManager : public util::Singleton<GameChaosZoneManager>
{
public:
    GameChaosZoneManager()
    {
    }
    ~GameChaosZoneManager()
    {
    }
    void ParseCGPacket(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void ParseAGPacket(ServerSession* server_session, MSG_BASE* packet, WORD size);

private:
    //BattleGround
    void OnCG_ProcessPacketBattleGround(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnCG_BATTLE_GROUND_RESURECTION_REFUSE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnCG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    void OnCG_BATTLE_GROUND_INITIATE_VOTE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnCG_BATTLE_GROUND_VOTE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

    void OnAG_ProcessPacketBattleGround(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_BATTLE_GROUND_ENTER_TEAM_CMD(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_BATTLE_GROUND_CAN_RESERVATION_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD(ServerSession* server_session, MSG_BASE* packet, WORD size);

    //FreePVP
    void OnCG_ProcessPacketFreePVP(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnCG_FREE_PVP_RESURRECTION_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    //GoldRush
    //CG
    void OnCG_ProcessPacketGoldRush(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnCG_GOLDRUSH_INITIATE_VOTE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnCG_GOLDRUSH_VOTE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);

    //AG
    void OnAG_ProcessPacketGoldRush(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_GOLDRUSH_ENTER_TEAM_CMD(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_GOLDRUSH_CAN_RESERVATION_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_GOLDRUSH_STATUS_CHANGE_STATE_CMD(ServerSession* server_session, MSG_BASE* packet, WORD size);
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID OnCG_ProcessPacketSonnenSchein(ServerSession* server_session, MSG_BASE* packet, WORD size);
    VOID OnAG_ProcessPacketSonnenSchein(ServerSession* server_session, MSG_BASE* packet, WORD size);

    VOID OnAG_SONNENSCHEIN_REQUEST_GUID_ACK(ServerSession* server_session, MSG_BASE* packet, WORD size);
    VOID OnAG_SONNENSCHEIN_ENTER_CHECK_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	void OnCG_ProcessPacketOneDaysGuildMission(ServerSession* server_session, MSG_BASE* packet, WORD size);

	void OnAG_ProcessPacketOneDaysGuildMission(ServerSession* server_session, MSG_BASE* packet, WORD size);
	void OnAG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size);
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND