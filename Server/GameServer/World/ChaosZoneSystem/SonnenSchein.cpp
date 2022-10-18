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

    //몬트샤인 eventinfo를 저장해둔다
    //sonnenschein은 몬트샤인과 동시간대에 진행되기 때문에 같은 event time을 사용한다
    DominationInfoParser* domi_parser = DominationInfoParser::Instance();
    event_info_ = domi_parser->GetMondScheinEventInfo();
    if (event_info_.m_wMapCode != 0)  {
        event_loaded_ = TRUE;
    }

    //시간 세팅
    update_timer_.SetTimer(UPDATE_TIME);
    time_sync_cycle_.SetTimer(TIME_SYNC_CYCLE);
    _SetRunTime();

    // Agent에 GuildGuid정보요쳥
    MSG_AG_SONNENSCHEIN_REQUEST_GUID_SYN syn_msg;
    syn_msg.room_key = instance_dugeon_->GetKey();
    syn_msg.m_dwKey = user_key;
    g_pGameServer->SendToServer(AGENT_SERVER, &syn_msg, sizeof(syn_msg));

    //reward_status 초기화
    reward_status_.Init();

    //이벤트상태초기화
     SetEventState(SONNENSCHEIN_EVENT_STATE_NONE);
    _UpdateCurEventState();
    //다음등급 타겟 세팅
    _SetNextPhaseTargetInfo();
}

void SonnenSchein::Update()
{
    // event_loaded_가 false이면 몬트샤인 event정보를 가져오지 못했다는 것.
    // Update를 돌릴 의미가 없어진다
    // 소멸대기상태에서도 Update 수행하지않는다
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
            //플레이어 강제퇴장
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
                //플레이어 강제퇴장
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
                //End상태에서 유예시간 종료, 방 소멸
                SetEventState(SONNENSCHEIN_EVENT_STATE_DESTROY);

                MSG_AG_SONNENSCHEIN_ROOM_DESTROY_CMD cmd_msg;
                cmd_msg.room_key = instance_dugeon_->GetKey();
                g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
            }
        }

        //시간에의한 상태변화
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
    // 시작 전 대기
    case SONNENSCHEIN_EVENT_STATE_WAIT:
        {
        } break;
    // 시작, 진행
    case SONNENSCHEIN_EVENT_STATE_START:
        {
            sonnenschein_run_timer_.SetTimer(run_time_);
            _SendTimeSync();
        } break;
    // 종료, 소멸대기
    case SONNENSCHEIN_EVENT_STATE_END:
        {
            //1. 소넨샤인 등급관련 처리
            //소넨샤인 등급 업데이트 패킷 전달
            SetSonnenScheinRewardPhase();

            //2.Timer 처리
            //해당 시간이 지나면 방 소멸
            room_destroy_timer_.SetTimer(ROOM_DESTROY_TIME);
            //해당 시간이 지나면 플레이어 강제퇴장
            kick_user_wait_timer_.SetTimer(ROOM_END_WAIT_TIME);

            //3. 시간종료 트리거알림
            // 등급이 최대값이면 성공했다는 뜻. 실패메시지 띄우지 않는다
            if (reward_status_.GetCurRewardPhase() != reward_status_.GetMaxRewardPhase())
            {
                WzID trjgger_id = StrToWzID("FAIL");
                if(instance_dugeon_->TriggerActive(trjgger_id) == false)
                {
                    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] 트리거(%g)를 액티브하는데 실패하였습니다.", trjgger_id);
                }
            }
            
        } break;
    // 각인완료, 종료
    case SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE:
        {
            stamp_success_wait_timer_.SetTimer(ROOM_END_WAIT_TIME);
        } break;
    }
}

bool SonnenSchein::EnterPlayer( Player* const player )
{
    //입장조건체크
    if (player == NULL || player->IsDeleted()) {
        return false;
    }
    if (event_loaded_ == FALSE) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein EventInfo Not Loaded!");
        return false;
    }
    //GM이 아닐경우의 추가 검사
    if(player->IsGMUser() == FALSE) 
    {
        // 소넨샤인 시작한 상태인지 검사
        if (GetEventState() != SONNENSCHEIN_EVENT_STATE_START)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein Not Started!");
            return false;
        }
    }

    // 동일 길드인지 체크
    if (_CheckEnterPlayerGuild(player) == FALSE ) {
        return false;
    }

    //hash리스트에 플레이어 추가
    player_hash_.insert(PLAYER_HASH::value_type(player->GetCharGuid(), player));

    _SendTimeSyncForPlayer(player);
    {
        TCHAR add_info[MAX_ADDINFO_SIZE];
        memset(add_info, 0, sizeof(add_info));
        //등급값을 함께 찍어서 초기화여부를 체크한다
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

    //hash리스트에서 제거
    player_hash_.erase(player->GetCharGuid());

    {
        TCHAR add_info[MAX_ADDINFO_SIZE];
        memset(add_info, 0, sizeof(add_info));
        //등급값을 함께 찍어서 초기화여부를 체크한다
        _sntprintf( add_info, MAX_ADDINFO_SIZE, "GuildGuid:%d", player->GetGuildGuid());

        GAMELOG->LogChaosZone(ACT_SONNENSCHEIN_LEAVE, player, NULL, add_info);
    }
}

VOID SonnenSchein::SetInitialCreater( CHARGUID char_guid, GUILDGUID guild_guid )
{
    //생성자, 생성자길드정보 초기화
    creater_char_guid_ = char_guid;
    room_guild_guid_ = guild_guid;

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Open SonnenSchein![GuildGuid:%d][Character:%d]", 
           guild_guid, char_guid);
}

BOOL SonnenSchein::_CheckEnterPlayerGuild( Player* enter_player )
{
    GUILDGUID enter_player_guild_guid = enter_player->GetGuildGuid();

    //SonnenSchein 생성자와 길드가 동일하면 true
    if (room_guild_guid_ == enter_player_guild_guid) {
        return TRUE;
    }
    
    // 다른 길드이면 false
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
    
    //리젠지역 좌표
    WzVector start_pos; 
    WzID AreaID = _GetStartingArea();
    game_field->GetAreaRandomStartingVector(AreaID, start_pos);

    //부활 위치로 텔레포트 시킨다
    if (game_field->TeleportObject(player, &start_pos) == FALSE) {
        return RC::RC_STATUS_TELEPORT_FAIL;
    }
    //부활
    if (player->OnResurrection(0,0.5f,0.5f) == FALSE) {
        return RC::RC_STATUS_RESURRECTION_FAIL;
    }

    //무적 상태,...?
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
    //각인에 성공하면 보상단계를 최대값으로 설정한다
    reward_status_.SetLastRewardPhase();
    //소넨샤인 로그에 기록
    _WriteSonnenScheinLog();

    SetEventState(SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE);

    //Agent의 SonnenScheinMgr에 각인성공을 알림
    ;{
        MSG_AG_SONNENSCHEIN_STAMP_COMPLETE_CMD cmd_msg;
        cmd_msg.room_key = instance_dugeon_->GetKey();
        g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
    };
    ;{
        // 각인알림 트리거 호출
        WzID trjgger_id = StrToWzID("END0");
        if(instance_dugeon_->TriggerActive(trjgger_id) == false)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] 트리거(%g)를 액티브하는데 실패하였습니다.", trjgger_id);
        }
    };

    // 점령성공로그
    GAMELOG->LogSonnenSchein(ACT_SONNENSCHEIN_DOMINATE_SUCCESS, room_guild_guid_, player);
}

VOID SonnenSchein::SetStampStartTime()
{
    stamp_start_time_ = GetTickCount();
}

void SonnenSchein::DeadCharacter( Character* const dead_character, Character* const kill_character )
{
    //단계 상승 조건이 몬스터처치
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
    //단계 상승 조건이 맵오브젝트
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
        // END상태에서 등급업데이트를 하지 않고 종료되는 상황
        // 이 상황은 강제적종료(대표적으로 GM명령어 '파괴')시에 나타난다.
        // 강제 종료 전에 등급업데이트를 시킨다.
        SetSonnenScheinRewardPhase();
    }
    return;
}

VOID SonnenSchein::_IncreaseRewardPhase()
{
    reward_status_.IncreaseRewardPhase();
    _SetNextPhaseTargetInfo();

    //소넨샤인 로그에 기록
    _WriteSonnenScheinLog();
}

VOID SonnenSchein::_SetNextPhaseTargetInfo()
{
    SonnenScheinRewardInfoParser* reward_info_parser = SonnenScheinRewardInfoParser::Instance();
    const SScheinRewardInfo* first_reward_info = reward_info_parser->FindData(reward_status_.GetCurRewardPhase()+1);
    
    // 첫 단계 타겟정보 기억해두기
    reward_status_.SetCurTargetType(first_reward_info->target_type);
    reward_status_.InputTargetList(first_reward_info);
}

VOID SonnenSchein::_UpdateCurEventState()
{
    // 시간에 따른 SonnenSchein상태 전환 처리---------------------------------------------
    DWORD cur_time = 0; //현재시간
    const WORD day_of_week = util::GetDateTime_Week_HHMMSS(cur_time);

    //요일체크
    if (day_of_week != event_info_.m_wDayOfWeek) 
    {
        return;
    }
    // 시간체크-----------------------------------------------------------------------------------
    DWORD wait_start_time = event_info_.m_dwTime[DOMINATION_EVENT_WAIT];
    DWORD battle_start_time = event_info_.m_dwTime[DOMINATION_EVENT_START];
    DWORD battle_end_time = event_info_.m_dwTime[DOMINATION_EVENT_END];

    if (GetEventState() == SONNENSCHEIN_EVENT_STATE_NONE)
    {
        //None 상태에서의 상태업데이트  
        if (Between(cur_time, wait_start_time, battle_start_time) == TRUE) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_WAIT);
        }
        else if (Between(cur_time, battle_start_time, battle_end_time) == TRUE) 
        {
            SetEventState(SONNENSCHEIN_EVENT_STATE_START);
        }
        //END에서는 생성자체가 되지 말아야 한다
    }
    else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_WAIT)
    {
        //Wait 상태에서의 업데이트
        if (Between(cur_time, battle_start_time, battle_end_time) == TRUE) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_START);
        }
    }
    else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_START)
    {
        //Start상태에서의 업데이트
        if (cur_time >= battle_end_time) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_END);
        }
    }
    else if (GetEventState() == SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE)
    {
        //각인완료 상태에서의 업데이트
        if (cur_time >= battle_end_time) {
            SetEventState(SONNENSCHEIN_EVENT_STATE_END);
        }
    }
    //---------------------------------------------------------------------------------------
    // 정상적 방식의 소넨샤인이 None상태인 것은 방을 만들수 없는 시간대에 만들었다는 것이 된다.
    // Wait상태부터 만들 수 있어야 하며, 라이프타임은 Wait상태 ~ End상태
    if (GetEventState() == SONNENSCHEIN_EVENT_STATE_NONE) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] SonnenSchein State is None! GMCreated or Bug");
    }
}

VOID SonnenSchein::SetSonnenScheinRewardPhase()
{
    //소넨샤인 등급 업데이트 패킷 전달
    //reward_phase를 길드정보에 저장한다
    GameGuild* guild = g_GameGuildManager.FindGuild(GetRoomGuildGuid());
    if (guild != NULL) {
        //길드에 등급값 갱신은 dbp의 패킷응답시점에서 진행시킨다

        MSG_GZ_GUILD_SONNENSCHEIN_UPDATE_SYN syn_packet;
        syn_packet.guild_guid = GetRoomGuildGuid();
        syn_packet.sonnenschein_phase = reward_status_.GetCurRewardPhase();

        g_pGameServer->SendToServer(GUILD_SERVER, &syn_packet, sizeof(syn_packet));
    }

    //등급 업데이트 했음을 체크
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

    //최대 보상단계값
    WORD column_size = reward_info_parser->GetDataSize();
    SetMaxRewardPhase((BYTE)column_size);
}
#endif //_NA_008334_20150608_SONNENSCHEIN
