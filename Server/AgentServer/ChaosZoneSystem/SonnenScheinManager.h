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

    enum {UPDATE_TIME = 500}; //0.5�ʸ��� ������Ʈ ����
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

    //������ sonnenschein���� ������ �� �ִ°�?
    RC::eSONNENSCHEIN_RESULT CanUserCreateSonnenSchein(User* user, BOOL is_gm_cmd = FALSE);
    //���ڰ� sonnenschein�濡 ������ �� �ִ°�?
    RC::eSONNENSCHEIN_RESULT CheckUserEnterSonnenSchein(User* user, BOOL is_gm_cmd = FALSE);

    //��� sonnenschein���� �÷��̾���� �ϰ������� ƨ�ܳ���
    VOID KickAllPlayers();
    VOID KickRoomPlayers(KEYTYPE zone_key);

    VOID DestroyRoom(KEYTYPE zone_key);
    VOID DestroyRoom(GUILDGUID guild_guid);

    VOID StampComplete(KEYTYPE zone_key);

    BOOL GetEnterPlayerList_CharGuid(GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT);

    BOOL MoveToSonnenScheinRoom(User* user, BOOL is_gm_cmd = FALSE);
private:
    //�⺻���� ����/���� ���ɿ��� �˻�
    RC::eSONNENSCHEIN_RESULT _CheckUserProcessSonnenSchein(User* user);
    eSONNENSCHEIN_EVENT_STATE _GetCurEventState();

    //Packet Handler
    Handler_CG_SONNENSCHEIN handler_cg_sonnenschein_;
    Handler_AG_SONNENSCHEIN handler_ag_sonnenschein_;

    SONNENSCHEIN_ROOM_MAP room_map_;

    MAPCODE sonnenschein_mapcode_;

    util::Timer	sonnenshcein_timer_;    // ���� ���� Ÿ�̸�
};




#endif //_NA_008334_20150608_SONNENSCHEIN