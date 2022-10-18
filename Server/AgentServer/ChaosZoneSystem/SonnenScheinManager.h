#pragma once

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <Singleton.h>
#include "ChaosZoneSystem/Handler_CG_SONNENSCHEIN.h"
#include "ChaosZoneSystem/Handler_AG_SONNENSCHEIN.h"

class SonnenScheinRoom;
//------------------------------------------------------------------------------

class SonnenScheinManager : public util::Singleton<SonnenScheinManager>
{
    typedef std::map<KEYTYPE, SonnenScheinRoom*> SONNENSCHEIN_ROOM_MAP;

    enum {UPDATE_TIME = 500}; //0.5초마다 업데이트 진행
public:
    SonnenScheinManager() : sonnenschein_mapcode_(0) {}
    ~SonnenScheinManager() {}

    VOID Init();

    VOID ProcessPacket_CG(User* user, MSG_BASE* packet, WORD size);
    VOID ProcessPacket_AG(GameServerSession* server_session, MSG_BASE* packet, WORD size);

    RC::eSONNENSCHEIN_RESULT CreateSonnenScheinRoom(User* user, BOOL is_gm_cmd = FALSE);

    SonnenScheinRoom* FindRoomUsingGuildGuid(GUILDGUID guild_guid);
    SonnenScheinRoom* FindRoomUsingRoomKey(KEYTYPE room_key);
    KEYTYPE GetZoneKeyUsingGuildGuid(GUILDGUID guild_guid);

    MAPCODE GetSonnenScheinMapCode();

    //유저가 sonnenschein방을 생성할 수 있는가?
    RC::eSONNENSCHEIN_RESULT CanUserCreateSonnenSchein(User* user, BOOL is_gm_cmd = FALSE);
    //유자거 sonnenschein방에 입장할 수 있는가?
    RC::eSONNENSCHEIN_RESULT CheckUserEnterSonnenSchein(User* user, BOOL is_gm_cmd = FALSE);

    //모든 sonnenschein룸의 플레이어들을 일괄적으로 튕겨낸다
    VOID KickAllPlayers();
    VOID KickRoomPlayers(KEYTYPE zone_key);

    VOID DestroyRoom(KEYTYPE zone_key);
    VOID DestroyRoom(GUILDGUID guild_guid);

    VOID StampComplete(KEYTYPE zone_key);

    BOOL GetEnterPlayerList_CharGuid(GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT);

    BOOL MoveToSonnenScheinRoom(User* user, BOOL is_gm_cmd = FALSE);
private:
    //기본적인 입장/생성 가능여부 검사
    RC::eSONNENSCHEIN_RESULT _CheckUserProcessSonnenSchein(User* user);
    eSONNENSCHEIN_EVENT_STATE _GetCurEventState();

    //Packet Handler
    Handler_CG_SONNENSCHEIN handler_cg_sonnenschein_;
    Handler_AG_SONNENSCHEIN handler_ag_sonnenschein_;

    SONNENSCHEIN_ROOM_MAP room_map_;

    MAPCODE sonnenschein_mapcode_;

    util::Timer	sonnenshcein_timer_;    // 전투 진행 타이머
};




#endif //_NA_008334_20150608_SONNENSCHEIN