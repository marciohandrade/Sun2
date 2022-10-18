#include "stdafx.h"

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "SonnenSchein.h"
#include "Player.h"

#include "GameInstanceDungeon.h"
#include "MapInfoParser.h"
#include "DominationInfoParser.h"
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "ChaosZoneSystem/SonnenScheinRewardInfoParser.h"
#include "Monster.h"

#include "PacketStruct_AG_ChaosZoneSystem.h"
#include "PacketStruct_GZ.h"

void SonnenSchein::Init( GameInstanceDungeon* const instance_dungeon )
{
    instance_dugeon_ = instance_dungeon;
    CHARGUID user_key = instance_dugeon_->GetMasterUserKey();

    //��Ʈ���� eventinfo�� �����صд�
    //sonnenschein�� ��Ʈ���ΰ� ���ð��뿡 ����Ǳ� ������ ���� event time�� ����Ѵ�
    DominationInfoParser* domi_parser = DominationInfoParser::Instance();
    event_info_ = domi_parser->GetMondScheinEventInfo();
    if (event_info_.m_wMapCode != 0)  {
        event_loaded_ = TRUE;
    }

    //�ð� ����
    update_timer_.SetTimer(UPDATE_TIME);
    time_sync_cycle_.SetTimer(TIME_SYNC_CYCLE);
    _SetRunTime();

    // Agent�� GuildGuid�����䫊
    MSG_AG_SONNENSCHEIN_REQUEST_GUID_SYN syn_msg;
    syn_msg.room_key = instance_dugeon_->GetKey();
    syn_msg.m_dwKey = user_key;
    g_pGameServer->SendToServer(AGENT_SERVER, &syn_msg, sizeof(syn_msg));

    //reward_status �ʱ�ȭ
    reward_status_.Init();

    //�̺�Ʈ�����ʱ�ȭ
     SetEventState(SONNENSCHEIN_EVENT_STATE_NONE);
    _UpdateCurEventState();
    //������� Ÿ�� ����
    _SetNextPhaseTargetInfo();
}

void SonnenSchein::Update()
{
    // event_loaded_�� false�̸� ��Ʈ���� event������ �������� ���ߴٴ� ��.
    // Update�� ���� �ǹ̰� ��������
    // �Ҹ�����¿����� Update ���������ʴ´�
    if (event_loaded_ == FALSE ||
        GetEventState() == SONNENSCHEIN_EVENT_STATE_DESTROY) {
        return;
    } 

    if (update_timer_.IsExpired() == TRUE )
    {
        //State Stamp_Success
        if (GetEventState() == SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE &&
            stamp_success_wait_timer_.IsExpiredManual(FALSE))
        {
            //�÷��̾� ��������
            if (player_hash_.empty() != true)
            {
                MSG_AG_SONNENSCHEIN_KICKUSER_CMD cmd_msg;
                cmd_msg.room_key = instance_dugeon_->GetKey();
                g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
            }
        }
        else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_END) 
        {
            if (kick_user_wait_timer_.IsExpiredManual(FALSE) == TRUE)
            {
                //�÷��̾� ��������
                if (player_hash_.empty() != true)
                {
                    MSG_AG_SONNENSCHEIN_KICKUSER_CMD cmd_msg;
                    cmd_msg.room_key = instance_dugeon_->GetKey();
                    g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
                }
            }
            else if (room_destroy_timer_.IsExpiredManual(FALSE) == TRUE)
            {
                //State End -> Destroy
                //End���¿��� �����ð� ����, �� �Ҹ�
                SetEventState(SONNENSCHEIN_EVENT_STATE_DESTROY);

                MSG_AG_SONNENSCHEIN_ROOM_DESTROY_CMD cmd_msg;
                cmd_msg.room_key = instance_dugeon_->GetKey();
                g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
            }
        }

        //�ð������� ���º�ȭ
        _UpdateCurEventState();
    }

    if (time_sync_cycle_.IsExpired() ==TRUE)
    {
        _SendTimeSync();
    }
}

VOID SonnenSchein::SetEventState( const eSONNENSCHEIN_EVENT_STATE& state )
{
    event_state_ = state;

    switch(event_state_)
    {
    // ���� �� ���
    case SONNENSCHEIN_EVENT_STATE_WAIT:
        {
        } break;
    // ����, ����
    case SONNENSCHEIN_EVENT_STATE_START:
        {
            sonnenschein_run_timer_.SetTimer(run_time_);
            _SendTimeSync();
        } break;
    // ����, �Ҹ���
    case SONNENSCHEIN_EVENT_STATE_END:
        {
            //1. �ҳٻ��� ��ް��� ó��
            //�ҳٻ��� ��� ������Ʈ ��Ŷ ����
            SetSonnenScheinRewardPhase();

            //2.Timer ó��
            //�ش� �ð��� ������ �� �Ҹ�
            room_destroy_timer_.SetTimer(ROOM_DESTROY_TIME);
            //�ش� �ð��� ������ �÷��̾� ��������
            kick_user_wait_timer_.SetTimer(ROOM_END_WAIT_TIME);

            //3. �ð����� Ʈ���ž˸�
            // ����� �ִ밪�̸� �����ߴٴ� ��. ���и޽��� ����� �ʴ´�
            if (reward_status_.GetCurRewardPhase() != reward_status_.GetMaxRewardPhase())
            {
                WzID trjgger_id = StrToWzID("FAIL");
                if(instance_dugeon_->TriggerActive(trjgger_id) == false)
                {
                    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Ʈ����(%g)�� ��Ƽ���ϴµ� �����Ͽ����ϴ�.", trjgger_id);
                }
            }
            
        } break;
    // ���οϷ�, ����
    case SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE:
        {
            stamp_success_wait_timer_.SetTimer(ROOM_END_WAIT_TIME);
        } break;
    }
}

bool SonnenSchein::EnterPlayer( Player* const player )
{
    //��������üũ
    if (player == NULL || player->IsDeleted()) {
        return false;
    }
    if (event_loaded_ == FALSE) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein EventInfo Not Loaded!");
        return false;
    }
    //GM�� �ƴҰ���� �߰� �˻�
    if(player->IsGMUser() == FALSE) 
    {
        // �ҳٻ��� ������ �������� �˻�
        if (GetEventState() != SONNENSCHEIN_EVENT_STATE_START)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein Not Started!");
            return false;
        }
    }

    // ���� ������� üũ
    if (_CheckEnterPlayerGuild(player) == FALSE ) {
        return false;
    }

    //hash����Ʈ�� �÷��̾� �߰�
    player_hash_.insert(PLAYER_HASH::value_type(player->GetCharGuid(), player));

    _SendTimeSyncForPlayer(player);
    {
        TCHAR add_info[MAX_ADDINFO_SIZE];
        memset(add_info, 0, sizeof(add_info));
        //��ް��� �Բ� �� �ʱ�ȭ���θ� üũ�Ѵ�
        _sntprintf( add_info, MAX_ADDINFO_SIZE, "GuildGuid:%d, CurPhase:%d", 
                    player->GetGuildGuid(), player->GetGuild()->GetSonnenScheinRewardPhase());

        GAMELOG->LogChaosZone(ACT_SONNENSCHEIN_ENTER, player, NULL, add_info);
    }
    return true;
}


void SonnenSchein::LeavePlayer( Player* const player )
{
    if (player == NULL || player->IsDeleted())
        return;

    //hash����Ʈ���� ����
    player_hash_.erase(player->GetCharGuid());

    {
        TCHAR add_info[MAX_ADDINFO_SIZE];
        memset(add_info, 0, sizeof(add_info));
        //��ް��� �Բ� �� �ʱ�ȭ���θ� üũ�Ѵ�
        _sntprintf( add_info, MAX_ADDINFO_SIZE, "GuildGuid:%d", player->GetGuildGuid());

        GAMELOG->LogChaosZone(ACT_SONNENSCHEIN_LEAVE, player, NULL, add_info);
    }
}

VOID SonnenSchein::SetInitialCreater( CHARGUID char_guid, GUILDGUID guild_guid )
{
    //������, �����ڱ������ �ʱ�ȭ
    creater_char_guid_ = char_guid;
    room_guild_guid_ = guild_guid;

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Open SonnenSchein![GuildGuid:%d][Character:%d]", 
           guild_guid, char_guid);
}

BOOL SonnenSchein::_CheckEnterPlayerGuild( Player* enter_player )
{
    GUILDGUID enter_player_guild_guid = enter_player->GetGuildGuid();

    //SonnenSchein �����ڿ� ��尡 �����ϸ� true
    if (room_guild_guid_ == enter_player_guild_guid) {
        return TRUE;
    }
    
    // �ٸ� ����̸� false
    return FALSE;
}

eUSER_RELATION_TYPE SonnenSchein::IsFriendPVP( Player* attacker,Player* target )
{
    return USER_RELATION_FRIEND;
}

RC::eSTATUS_RESULT SonnenSchein::ResurrectionAtSafezone( Player *player )
{
    if (player->IsAlive() == TRUE) {
        return RC::RC_STATUS_CHAR_IS_ALIVE;
    }
    GameField* game_field = player->GetField();
    if (game_field == NULL) {
        return RC::RC_STATUS_ZONE_IS_NULL;
    }
    
    //�������� ��ǥ
    WzVector start_pos; 
    WzID AreaID = _GetStartingArea();
    game_field->GetAreaRandomStartingVector(AreaID, start_pos);

    //��Ȱ ��ġ�� �ڷ���Ʈ ��Ų��
    if (game_field->TeleportObject(player, &start_pos) == FALSE) {
        return RC::RC_STATUS_TELEPORT_FAIL;
    }
    //��Ȱ
    if (player->OnResurrection(0,0.5f,0.5f) == FALSE) {
        return RC::RC_STATUS_RESURRECTION_FAIL;
    }

    //���� ����,...?
    return RC::RC_STATUS_SUCCESS;
}

WzID SonnenSchein::_GetStartingArea()
{
    MAPCODE map_code = instance_dugeon_->GetMapCode();
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);

    return map_info->wStartAreaID;
}

//About Stamp
VOID SonnenSchein::SuccessStamp(Player* player)
{
#ifdef _DEBUG
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein Stamp Success![Guild:%d]", room_guild_guid_);
#endif //(_DEBUG)
    //���ο� �����ϸ� ����ܰ踦 �ִ밪���� �����Ѵ�
    reward_status_.SetLastRewardPhase();
    //�ҳٻ��� �α׿� ���
    _WriteSonnenScheinLog();

    SetEventState(SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE);

    //Agent�� SonnenScheinMgr�� ���μ����� �˸�
    ;{
        MSG_AG_SONNENSCHEIN_STAMP_COMPLETE_CMD cmd_msg;
        cmd_msg.room_key = instance_dugeon_->GetKey();
        g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
    };
    ;{
        // ���ξ˸� Ʈ���� ȣ��
        WzID trjgger_id = StrToWzID("END0");
        if(instance_dugeon_->TriggerActive(trjgger_id) == false)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Ʈ����(%g)�� ��Ƽ���ϴµ� �����Ͽ����ϴ�.", trjgger_id);
        }
    };

    // ���ɼ����α�
    GAMELOG->LogSonnenSchein(ACT_SONNENSCHEIN_DOMINATE_SUCCESS, room_guild_guid_, player);
}

VOID SonnenSchein::SetStampStartTime()
{
    stamp_start_time_ = GetTickCount();
}

void SonnenSchein::DeadCharacter( Character* const dead_character, Character* const kill_character )
{
    //�ܰ� ��� ������ ����óġ
    if (reward_status_.GetCurTargetType() == SScheinRewardInfo::TARGET_TYPE_MONSTER &&
        dead_character->IsEqualObjectKind(MONSTER_OBJECT) == true)
    {
        Monster* monster = static_cast<Monster*>(dead_character);
        BOOL pop_success = reward_status_.PopTarget(monster->GetBaseInfo()->m_MonsterCode);

        if (pop_success == TRUE) 
        {
            SUNLOG(eDEV_LOG, "["__FUNCTION__"] PhaseTarget Killed!");

            if (reward_status_.IsTargetAllDeath() == TRUE) {
                _IncreaseRewardPhase();
            }
        }
    }
    //�ܰ� ��� ������ �ʿ�����Ʈ
    else if (reward_status_.GetCurTargetType() == SScheinRewardInfo::TARGET_TYPE_MAPOBJECT &&
        dead_character->IsEqualObjectKind(MAP_OBJECT) == true) 
    {
        //
    }
}

void SonnenSchein::Release()
{
    if (is_process_phase_update_ == FALSE)
    {
        // END���¿��� ��޾�����Ʈ�� ���� �ʰ� ����Ǵ� ��Ȳ
        // �� ��Ȳ�� ����������(��ǥ������ GM��ɾ� '�ı�')�ÿ� ��Ÿ����.
        // ���� ���� ���� ��޾�����Ʈ�� ��Ų��.
        SetSonnenScheinRewardPhase();
    }
    return;
}

VOID SonnenSchein::_IncreaseRewardPhase()
{
    reward_status_.IncreaseRewardPhase();
    _SetNextPhaseTargetInfo();

    //�ҳٻ��� �α׿� ���
    _WriteSonnenScheinLog();
}

VOID SonnenSchein::_SetNextPhaseTargetInfo()
{
    SonnenScheinRewardInfoParser* reward_info_parser = SonnenScheinRewardInfoParser::Instance();
    const SScheinRewardInfo* first_reward_info = reward_info_parser->FindData(reward_status_.GetCurRewardPhase()+1);
    
    // ù �ܰ� Ÿ������ ����صα�
    reward_status_.SetCurTargetType(first_reward_info->target_type);
    reward_status_.InputTargetList(first_reward_info);
}

VOID SonnenSchein::_UpdateCurEventState()
{
    // �ð��� ���� SonnenSchein���� ��ȯ ó��---------------------------------------------
    DWORD cur_time = 0; //����ð�
    const WORD day_of_week = util::GetDateTime_Week_HHMMSS(cur_time);

    //����üũ
    if (day_of_week != event_info_.m_wDayOfWeek) 
    {
        return;
    }
    // �ð�üũ-----------------------------------------------------------------------------------
    DWORD wait_start_time = event_info_.m_dwTime[DOMINATION_EVENT_WAIT];
    DWORD battle_start_time = event_info_.m_dwTime[DOMINATION_EVENT_START];
    DWORD battle_end_time = event_info_.m_dwTime[DOMINATION_EVENT_END];

    if (GetEventState() == SONNENSCHEIN_EVENT_STATE_NONE)
    {
        //None ���¿����� ���¾�����Ʈ  
        if (Between(cur_time, wait_start_time, battle_start_time) == TRUE) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_WAIT);
        }
        else if (Between(cur_time, battle_start_time, battle_end_time) == TRUE) 
        {
            SetEventState(SONNENSCHEIN_EVENT_STATE_START);
        }
        //END������ ������ü�� ���� ���ƾ� �Ѵ�
    }
    else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_WAIT)
    {
        //Wait ���¿����� ������Ʈ
        if (Between(cur_time, battle_start_time, battle_end_time) == TRUE) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_START);
        }
    }
    else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_START)
    {
        //Start���¿����� ������Ʈ
        if (cur_time >= battle_end_time) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_END);
        }
    }
    else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE)
    {
        //���οϷ� ���¿����� ������Ʈ
        if (cur_time >= battle_end_time) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_END);
        }
    }
    //---------------------------------------------------------------------------------------
    // ������ ����� �ҳٻ����� None������ ���� ���� ����� ���� �ð��뿡 ������ٴ� ���� �ȴ�.
    // Wait���º��� ���� �� �־�� �ϸ�, ������Ÿ���� Wait���� ~ End����
    if (GetEventState() == SONNENSCHEIN_EVENT_STATE_NONE) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein State is None! GMCreated or Bug");
    }
}

VOID SonnenSchein::SetSonnenScheinRewardPhase()
{
    //�ҳٻ��� ��� ������Ʈ ��Ŷ ����
    //reward_phase�� ��������� �����Ѵ�
    GameGuild* guild = g_GameGuildManager.FindGuild(GetRoomGuildGuid());
    if (guild != NULL) {
        //��忡 ��ް� ������ dbp�� ��Ŷ����������� �����Ų��

        MSG_GZ_GUILD_SONNENSCHEIN_UPDATE_SYN syn_packet;
        syn_packet.guild_guid = GetRoomGuildGuid();
        syn_packet.sonnenschein_phase = reward_status_.GetCurRewardPhase();

        g_pGameServer->SendToServer(GUILD_SERVER, &syn_packet, sizeof(syn_packet));
    }

    //��� ������Ʈ ������ üũ
    is_process_phase_update_ = TRUE;
}

VOID SonnenSchein::_SendTimeSync()
{
    MSG_CG_ZONE_EX_SONNENSCHEIN_TIME_SYNC_BRD brd_msg;
    brd_msg.event_state_ = GetEventState();
    brd_msg.remain_time_ = sonnenschein_run_timer_.GetRemainedTime();

    GameInstanceDungeon* dungeon = GetInstanceDungeon();
    dungeon->SendPacketToAll(&brd_msg, sizeof(brd_msg));
}

VOID SonnenSchein::_SendTimeSyncForPlayer( Player* player )
{
    MSG_CG_ZONE_EX_SONNENSCHEIN_TIME_SYNC_BRD brd_msg;
    brd_msg.event_state_ = GetEventState();
    brd_msg.remain_time_ = sonnenschein_run_timer_.GetRemainedTime();

    player->SendPacket(&brd_msg, sizeof(brd_msg));
}

DWORD SonnenSchein::_GetRunTime( const WORD& day_of_week, const DWORD time ) const
{
    const DWORD start_time = event_info_.m_dwTime[DOMINATION_EVENT_START];
    const DWORD end_time = event_info_.m_dwTime[DOMINATION_EVENT_END];
    return util::CarcHHMMSS(end_time, max(start_time, time), false);
}

VOID SonnenSchein::_SetRunTime()
{
    DWORD  dwTime = 0;
    const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);
    const DWORD wzTime = _GetRunTime(wDayOfWeek,dwTime);
    run_time_ = util::HHMMSSToSecond(wzTime) * 1000;
}

VOID SonnenSchein::_WriteSonnenScheinLog()
{
    GameGuild* guild = g_GameGuildManager.FindGuild(GetRoomGuildGuid());
    GameInstanceDungeon* dungeon = GetInstanceDungeon();
    DWORD player_count = dungeon->GetPlayerNum();

    if (guild == NULL) {
        SUNSONNENSCHEINLOG(eCRITICAL_LOG, TEXT("[GuildGuid:%d]\t[ClearPhase:%d]\t[PlayerCount:%d]"),
            GetRoomGuildGuid(), reward_status_.GetCurRewardPhase(), player_count);
        return;
    }
    
    SUNSONNENSCHEINLOG(eCRITICAL_LOG, TEXT("[GuildName:%s]\t[ClearPhase:%d]\t[PlayerCount:%d]"),
        guild->GetGuildName(), reward_status_.GetCurRewardPhase(), player_count);
}

//===========================================================================================

VOID SonnenScheinRewardStatus::IncreaseRewardPhase()
{
    if (reward_phase_ < MAX_REWARD_PHASE ) {
        ++reward_phase_;
    }
}

BOOL SonnenScheinRewardStatus::PopTarget( MONSTERCODE target_code )
{
    std::vector<MONSTERCODE>::iterator itr = std::find(cur_target_list_.begin(), 
                                                       cur_target_list_.end(), target_code);

    if (itr != cur_target_list_.end() ) 
    {
        cur_target_list_.erase(itr);
        return TRUE;
    }
    return FALSE;
}

BOOL SonnenScheinRewardStatus::IsTargetAllDeath()
{
    if (cur_target_list_.empty() == true) {
        return TRUE;
    }
    return FALSE;
}

VOID SonnenScheinRewardStatus::InputTargetList( const SScheinRewardInfo* info )
{
    for (int count=0; count < SScheinRewardInfo::TARGET_SIZE; ++count)
    {
        _PushTarget(info->target_array[count]);
    }
}

VOID SonnenScheinRewardStatus::_PushTarget( MONSTERCODE target_code )
{
    if (target_code != 0) {
        cur_target_list_.push_back(target_code);
    }
}

VOID SonnenScheinRewardStatus::Init()
{
    SonnenScheinRewardInfoParser* reward_info_parser = SonnenScheinRewardInfoParser::Instance();

    //�ִ� ����ܰ谪
    WORD column_size = reward_info_parser->GetDataSize();
    SetMaxRewardPhase((BYTE)column_size);
}
#endif //_NA_008334_20150608_SONNENSCHEIN
