#include "stdafx.h"
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "SonnenScheinManager.h"

#include "SonnenScheinRoom.h"
#include "DominationInfoParser.h"
#include "ChaosZoneInfoParser.h"

#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>
#include <Zone/InstanceDungeon.h>
#include <Zone/ZoneManager.h>
#include "ServerOptionParserEx.h"
#include "AgentServer.h"

VOID SonnenScheinManager::Init()
{
    sonnenschein_mapcode_ = ChaosZoneInfoParser::Instance()->GetSonnenScheinMapCode();

    room_map_.clear();
}

MAPCODE SonnenScheinManager::GetSonnenScheinMapCode()
{
    if (sonnenschein_mapcode_ == 0)
    {
        //���ڵ尡 0�� ���
        //solarauth�� ��ũ��Ʈ�� �ε��� ��� ��ũ��Ʈ�� �ڴʰ� �������� 0�� �� �ִ�.
        sonnenschein_mapcode_ = ChaosZoneInfoParser::Instance()->GetSonnenScheinMapCode();
    }

    return sonnenschein_mapcode_;
}

VOID SonnenScheinManager::ProcessPacket_CG( User* user, MSG_BASE* packet, WORD size )
{
    handler_cg_sonnenschein_.OnCG_ProcessPacket(user, packet, size);
}

VOID SonnenScheinManager::ProcessPacket_AG( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
    handler_ag_sonnenschein_.OnAG_ProcessPacket(server_session, packet, size);
}   

RC::eSONNENSCHEIN_RESULT SonnenScheinManager::CreateSonnenScheinRoom(User* user, BOOL is_gm_cmd /*= FALSE*/)
{
    // ������ ���� ������ �������� �˻�
    RC::eSONNENSCHEIN_RESULT check_result = CanUserCreateSonnenSchein(user, is_gm_cmd);
    if (check_result != RC::RC_SONNENSCHEIN_SUCCESS) {
        return check_result;
    }

    //sonnenschein�� ���ڵ�
    const MAPCODE map_code = GetSonnenScheinMapCode();

    user->BeginTransaction(TR_CREATE_INSTANCE_DUNGEON);

    // Instance Dungeon ����
    InstanceDungeon* dungeon = NULL;
    const RC::eROOM_RESULT result = 
        g_ZoneManager.CreateInstanceDungeon(user->GetZoneKey(), user, map_code, (InstanceDungeon*&)dungeon, 
        INDUN_KIND_CHAOS_ZONE, FALSE);
    if (dungeon == NULL || result != RC::RC_ROOM_SUCCESS)
    {
        //instance_dungeon��������
        user->EndTransaction();
        return RC::RC_SONNENSCHEIN_ROOM_CREATE_FAILED;
    }
    dungeon->SetRoomState(eROOMSTATE_GENERNAL);

    // SonnenScheinRoom ����
    KEYTYPE room_key = dungeon->GetKey();
    SonnenScheinRoom* new_room = new SonnenScheinRoom(room_key, user->GetSelectedCharGuildGuid());
   
    //Map�� Room ���
    room_map_[room_key] = new_room;

    user->EndTransaction();
    return RC::RC_SONNENSCHEIN_SUCCESS;
}

SonnenScheinRoom* SonnenScheinManager::FindRoomUsingGuildGuid( GUILDGUID guild_guid )
{
    SONNENSCHEIN_ROOM_MAP::iterator itr = room_map_.begin();
    for (itr; itr != room_map_.end(); ++itr)
    {
        SonnenScheinRoom* target = itr->second;
        if (target == NULL) {
            continue;
        }

        if (guild_guid == target->GetGuildGuid()) {
            return target;
        }
    }
    return NULL;
}

RC::eSONNENSCHEIN_RESULT SonnenScheinManager::CanUserCreateSonnenSchein( User* user, BOOL is_gm_cmd /*= FALSE*/ )
{
    // GM��ɿ� ���� ������ �ƴ� ��� �߰� �˻�
    // ���������: EventState�� Wait Ȥ�� Start�̾�� �Ѵ�
    if (is_gm_cmd == FALSE)
    {
        eSONNENSCHEIN_EVENT_STATE cur_state = _GetCurEventState();
        if (cur_state != SONNENSCHEIN_EVENT_STATE_START) {
                return RC::RC_SONNENSCHEIN_STATE_NOT_READY;
        }
    }

    RC::eSONNENSCHEIN_RESULT enter_result = _CheckUserProcessSonnenSchein(user);
    if ( enter_result != RC::RC_SONNENSCHEIN_SUCCESS) {
        return enter_result;
    }

    //�̹� �ش� ����� ���� �����Ǿ� ����
    SonnenScheinRoom* room =SonnenScheinManager::Instance()->FindRoomUsingGuildGuid(user->GetSelectedCharGuildGuid());
    if (room != NULL) {
        return RC::RC_SONNENSCHEIN_ALEADY_CREATED;
    }

    return RC::RC_SONNENSCHEIN_SUCCESS;
}

RC::eSONNENSCHEIN_RESULT SonnenScheinManager::CheckUserEnterSonnenSchein( User* user, BOOL is_gm_cmd /*= FALSE*/ )
{
    GUILDGUID user_guild_guid = user->GetSelectedCharGuildGuid();
    SonnenScheinRoom* room = FindRoomUsingGuildGuid(user_guild_guid);
    if (room == NULL) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenScheinRoom is NULL! [guild:%d]", user_guild_guid);
        return RC::RC_SONNENSCHEIN_ROOM_NO_EXIST;
    }

    //�̹� ������ �÷��̾��ΰ�?
    if (room->FindUser(user->GetPlayerKey()) != NULL) {
        return RC::RC_SONNENSCHEIN_ALREADY_ENTERED;
    }

    // ������ �Ϸ���� �������¿��� �Ѵ�
    if (room->IsStampComplete() == TRUE) {
        return RC::RC_SONNENSCHEIN_STATE_END;
    }

    // GM��ɿ� ���� ������ �ƴ� ��� �߰� �˻�
    if (is_gm_cmd == FALSE)
    {
        // ������ ����: �ð��� 'Start'�� �� ���Ŀ��� �Ѵ�
        if (_GetCurEventState() != SONNENSCHEIN_EVENT_STATE_START) {
                return RC::RC_SONNENSCHEIN_STATE_NOT_READY;
        }
    }

    RC::eSONNENSCHEIN_RESULT result = _CheckUserProcessSonnenSchein(user);
    if (result != RC::RC_SONNENSCHEIN_SUCCESS) {
        return result;
    }

    return RC::RC_SONNENSCHEIN_SUCCESS;
}

SonnenScheinRoom* SonnenScheinManager::FindRoomUsingRoomKey( KEYTYPE room_key )
{
    SONNENSCHEIN_ROOM_MAP::iterator itr = room_map_.find(room_key);

    if(itr != room_map_.end()) {
        return itr->second;
    }

    return NULL;
}

KEYTYPE SonnenScheinManager::GetZoneKeyUsingGuildGuid( GUILDGUID guild_guid )
{
    SonnenScheinRoom* room = FindRoomUsingGuildGuid(guild_guid);
    if (room == NULL) {
        return 0;
    }

    return room->GetZoneKey();
}

//��� room�� �÷��̾�� ��������, SonnenSchein����� ���
VOID SonnenScheinManager::KickAllPlayers()
{
    SONNENSCHEIN_ROOM_MAP::iterator itr = room_map_.begin();
    for (itr; itr != room_map_.end(); ++itr)
    {
        SonnenScheinRoom* room = itr->second;
        room->KickAllUsers();
    }
}

//Ư�� Room �� ��� �÷��̾� ��������
VOID SonnenScheinManager::KickRoomPlayers( KEYTYPE zone_key )
{
    if (zone_key == 0) {
        //zone_key�� 0�� ��� ������ �Լ��� ���Ⱑ �ƴϴ�
        return;
    }

    SONNENSCHEIN_ROOM_MAP::iterator itr = room_map_.find(zone_key);
    if (itr != room_map_.end())
    {
        SonnenScheinRoom* room = itr->second;
        room->KickAllUsers();
    }
}

RC::eSONNENSCHEIN_RESULT SonnenScheinManager::_CheckUserProcessSonnenSchein( User* user )
{
    //������ ä�ο����� ����
    AgentServer* pAgentServer = AgentServer::GetInstance();
    SERVER_KEY rKey(pAgentServer->GetKey());

    SERVER_COMMON_OPTION& server_option = ServerOptionParserEx::Instance()->GetServerOption();
    if ( rKey.GetChannelID() != server_option.m_byDominationChannel )
    {
        return RC::RC_SONNENSCHEIN_NOT_DOMINATION_CHANNEL;
    }
    // �ٸ��ൿ���̸� (�ŷ�/����) ����
    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)  {
        return RC::RC_SONNENSCHEIN_FAIL_TRANSACTION;
    }

    //���� �������
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager)
    {
        if (battle_ground_manager->IsValidReservationMember(user->GetUserKey())) {
            return RC::RC_SONNENSCHEIN_BATTLEGROUND_RESERVATION_STATE;
        }
    }

    //Ʈ����ǻ���
    if (user->IsBeginTransaction()) {
        return RC::RC_SONNENSCHEIN_FAIL_TRANSACTION;
    }

    return RC::RC_SONNENSCHEIN_SUCCESS;
}

VOID SonnenScheinManager::DestroyRoom(KEYTYPE zone_key)
{
    SONNENSCHEIN_ROOM_MAP::iterator itr = room_map_.find(zone_key);
    if (itr != room_map_.end())
    {
        SonnenScheinRoom* room = itr->second;
        room_map_.erase(itr);
        SAFE_DELETE(room);
    }
}

VOID SonnenScheinManager::DestroyRoom( GUILDGUID guild_guid )
{
    KEYTYPE zone_key = GetZoneKeyUsingGuildGuid(guild_guid);
    if (zone_key != 0) {
        DestroyRoom(zone_key);
    }
}

eSONNENSCHEIN_EVENT_STATE SonnenScheinManager::_GetCurEventState()
{
    DWORD cur_time = 0; //����ð�
    const WORD day_of_week = util::GetDateTime_Week_HHMMSS(cur_time);

    //�ҳٻ����� ��Ʈ���ΰ� ���� �̺�Ʈ�ð��� ������
    sDOMINATION_EVENT_INFO event_info = DominationInfoParser::Instance()->GetMondScheinEventInfo();

    //����üũ
    if (day_of_week != event_info.m_wDayOfWeek) {
        // �ҳٻ��� ���� ������ �ƴϴ�
        return SONNENSCHEIN_EVENT_STATE_NONE;
    }

    DWORD wait_start_time = event_info.m_dwTime[DOMINATION_EVENT_WAIT];
    DWORD battle_start_time = event_info.m_dwTime[DOMINATION_EVENT_START];
    DWORD battle_end_time = event_info.m_dwTime[DOMINATION_EVENT_END];

    if (Between(cur_time, wait_start_time, battle_start_time) == TRUE) {
        return SONNENSCHEIN_EVENT_STATE_WAIT;
    }
    else if (Between(cur_time, battle_start_time, battle_end_time) == TRUE) {
        return SONNENSCHEIN_EVENT_STATE_START;
    }
    else if (cur_time >= battle_end_time) {
        return SONNENSCHEIN_EVENT_STATE_END;
    }
}

VOID SonnenScheinManager::StampComplete( KEYTYPE zone_key )
{
    SonnenScheinRoom* room = FindRoomUsingRoomKey(zone_key);
    room->StampComplete();
}

BOOL SonnenScheinManager::GetEnterPlayerList_CharGuid( GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT)
{
    SonnenScheinRoom* room = FindRoomUsingGuildGuid(guild_guid);

    if (room == NULL) {
        return FALSE;
    }

    *player_count = room->GetPlayerList_CharGuid(list);

    return TRUE;
}

BOOL SonnenScheinManager::MoveToSonnenScheinRoom(User* user, BOOL is_gm_cmd /*= FALSE*/)
{
    GUILDGUID user_guild_guid = user->GetSelectedCharGuildGuid();
    if (user_guild_guid == 0) {
        return FALSE;
    }

    RC::eSONNENSCHEIN_RESULT enter_result = CheckUserEnterSonnenSchein(user, is_gm_cmd);
    if (enter_result != RC::RC_SONNENSCHEIN_SUCCESS) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][GM]SonnenSchein Enter Failed!");
        return FALSE;
    }

    KEYTYPE zone_key = GetZoneKeyUsingGuildGuid(user_guild_guid);

    // �̵��Ϸ��� ���� instance_dungeon
    InstanceDungeon* const target_dungeon = \
        static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(zone_key));
    if (target_dungeon == NULL) {
        return FALSE;
    }

    //���� ��ġ�� Zone
    ZoneInterface* current_zone = g_ZoneManager.FindZone(user->GetZoneKey());
    if (current_zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : Not found zone : %d", user->GetUserGUID());
        return FALSE;
    }

    user->BeginTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);
    user->SetTRRoomKey(zone_key);

    CheckMoveZonePolicy* const check_move_zone_policy = 
        MoveZonePolicy<MZTF_STATIC_TO_ROOM_FOR_UCUSER>::AlignInfo(user,
        user->GetZoneType(), target_dungeon->GetType(),
        user->GetZoneKey(),  target_dungeon->GetKey());

    RoomArgument argument(eZONEPUBLIC_PUBLIC, NULL);

    RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(check_move_zone_policy, RoomArgument::GetEmpty());
    if (rt != RC::RC_ROOM_SUCCESS) {
        user->EndTransaction();
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein Zone Move Failed!");
        return FALSE;
    }

    return TRUE;
}


#endif //_NA_008334_20150608_SONNENSCHEIN