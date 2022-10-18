#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRushTeam.h"

#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "world/ChaosZoneSystem/GoldRush.h"
#include <../ProgramCommon/BattleGroundInfoParser.h>
#include "GameInstanceDungeon.h"
#include "MapInfoParser.h"
#include <StatusManager.h>
#include "MissionManager.h"
#include "MissionRewardManager.h"
#include "TriggerManagerEx.h"
#include "World/ChaosZoneSystem/GoldRushBattleResult.h"
#include <../ProgramCommon/ChaosZoneSystem/GoldRushInfoParser.h>

#include "../../MovingPlanner.h"

void GoldRushTeam::Update()
{
    if (team_state_ != eCHAOSTEAM_STATE_MATCHED) {
        return;
    }

    //사망자 부활처리 로직
    PLAYER_HASH_ITR itr = player_hash_map_.begin();
    for (; itr != player_hash_map_.end(); ++itr)
    {
        Player* const player = itr->second;
        if (player == NULL || player->IsDeleted())
            continue;
        GoldRushPlayerInfo& goldrush_player_info = player->GetGoldRushPlayerInfo();
        
        if (player->IsDead() && 
            (goldrush_player_info.GetPlayerState() == kPlayerStateMatch ||
             goldrush_player_info.GetPlayerState() == kPlayerStateDeath))
        {
            if (goldrush_player_info.IsExpiredMoveToSafeZone() == false) {
                continue;
            }
            goldrush_player_info.SetPlayerState(kPlayerStateStandbyResurection);
            OnMoveToSafezone(player);
            player->OnResurrection(0, 1.f, 1.f);

            MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD send_packet;
            send_packet.player_object_key = player->GetObjectKey();
            send_packet.player_state = kPlayerStateStandbyResurection;
            send_packet.resurection_time = goldrush_->GetRemainTicks();
            player->SendPacketAround(&send_packet, sizeof(send_packet));
        }

        // 부활 스킬 및 사망 시 부활 아이템 옵션 동작 시..
        else if(player->IsAlive() && goldrush_player_info.GetPlayerState() == kPlayerStateDeath) {
            goldrush_player_info.SetPlayerState(kPlayerStateMatch);
        }
    }
}

//Vote System
bool GoldRushTeam::IsInitialteTime()
{
    // time check
    if (initialte_vote_timer_.IsExpiredManual(false) == false) {
        return false;
    }
    return true;
}

bool GoldRushTeam::OnInitiateVote( Player* initiate_player, Player* van_payer )
{
    if (is_initialte_vote_ == true)
    {
        return false;
    }

    voting_timer_.SetTimer(30 * 1000);

    // 발의자 및 강제 추방 대상자에게는 투표 현황..
    ;{
        MSG_CG_GOLDRUSH_VOTE_STATUS_BRD brd_msg;
        _tcsncpy(brd_msg.van_payer_name, van_payer->GetCharName(), MAX_CHARNAME_LENGTH);
        brd_msg.yes_count = 1;
        brd_msg.no_count = 0;   
        brd_msg.remain_vote_time = voting_timer_.GetRemainedTime();

        initiate_player->SendPacket(&brd_msg, sizeof(brd_msg));
        van_payer->SendPacket(&brd_msg, sizeof(brd_msg));

        _tcsncpy(van_player_name_, van_payer->GetCharName(), MAX_CHARNAME_LENGTH);
        vote_player_hash_map_.insert(PLAYER_HASH::value_type(initiate_player->GetCharGuid(), initiate_player));
        vote_player_hash_map_.insert(PLAYER_HASH::value_type(van_payer->GetCharGuid(), van_payer));
    };

    // 나머지 유권자에게는 투표권..
    ;{
        MSG_CG_GOLDRUSH_INITIATE_VOTE_BRD brd_msg;
        _tcsncpy(brd_msg.van_payer_name, van_payer->GetCharName(), MAX_CHARNAME_LENGTH);
        PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
        for (; const_itr != player_hash_map_.end(); ++const_itr)
        {
            Player* const player = const_itr->second;
            if (player == NULL || player->IsDeleted())
                continue;
            if (player->GetCharGuid() == initiate_player->GetCharGuid() || 
                player->GetCharGuid() == van_payer->GetCharGuid())
                continue;
            player->SendPacket(&brd_msg, sizeof(brd_msg));
        }
    };

    is_initialte_vote_ = true;

    return true;
}

void GoldRushTeam::OnVote( CHARGUID char_guid, bool vote_result )
{
    Player* player = FindMember(char_guid);
    if (player == NULL || player->IsDeleted()) {
        return;
    }

    if (is_initialte_vote_ == false) {
        return;
    }

    if (voting_timer_.IsExpired(false) == true) {
        return;
    }

    if (vote_result == true)
    {
        vote_player_hash_map_.insert(PLAYER_HASH::value_type(char_guid, player));

        // 결과 체크..
        if (CheckVoteResult() == true) // 투표가 종료 됨..
        {
            OnVoteResult(true);
        }

        else // 투표 진행 중..
        {
            // 투표 현황 BRd..
            MSG_CG_GOLDRUSH_VOTE_STATUS_BRD brd_msg;
            _tcsncpy(brd_msg.van_payer_name, van_player_name_, MAX_CHARNAME_LENGTH);
            brd_msg.yes_count = static_cast<BYTE>(vote_player_hash_map_.size() - 1);
            brd_msg.no_count = 0;
            brd_msg.remain_vote_time = voting_timer_.GetRemainedTime();

            PLAYER_HASH::const_iterator const_itr = vote_player_hash_map_.begin();
            for (; const_itr != vote_player_hash_map_.end(); ++const_itr)
            {
                Player* const player = const_itr->second;
                if (player == NULL || player->IsDeleted()) {
                    continue;
                }
                player->SendPacket(&brd_msg, sizeof(brd_msg));
            }
        }
    }

    // 부결..
    else
    {
        OnVoteResult(false);
    }
}

bool GoldRushTeam::CheckVoteResult()
{
    const int current_memeber_count = player_hash_map_.size();
    const int vote_member_count = vote_player_hash_map_.size();

    if (current_memeber_count == vote_member_count)
    {
        return true;
    }

    return false;
}

void GoldRushTeam::OnVoteResult( bool van_result )
{
    if (is_initialte_vote_ == false)
        return;

    // 투표결과 BRD..
    MSG_CG_GOLDRUSH_VOTE_RESULT_BRD brd_msg;
    brd_msg.vote_result = van_result;
    _tcsncpy(brd_msg.van_payer_name, van_player_name_, MAX_CHARNAME_LENGTH);

    if (van_result) 
    {
        // 해당 플에이어 강제 퇴장..
        OnVanPlayer();

        // 투표 가결 시.. 전장 모든 유저에게 BRD..
        goldrush_->SendPacketToAll(&brd_msg, sizeof(brd_msg));

    }
    else 
    {
        // 투표 부결 시..같은 팀 유저에게만 BRD..
        SendPacketToAll(&brd_msg, sizeof(brd_msg));
    }

    initialte_vote_timer_.SetTimer(60 * 1000); 
    is_initialte_vote_ = false;
    vote_player_hash_map_.clear();
}

void GoldRushTeam::OnVanPlayer()
{
    Player* const van_player = PlayerManager::Instance()->FindPlayerByName(van_player_name_);
    if (van_player == NULL) {
        return;
    }

    OnMoveToSafezone(van_player);
    goldrush_->GoldRushPenalty(van_player);

    MSG_CG_GOLDRUSH_VANP_PLAYER_CMD cmd_msg;
    van_player->SendPacket(&cmd_msg, sizeof(cmd_msg));
}

void GoldRushTeam::AddMember( Player* const player )
{
    if (player_hash_map_.size() > battle_ground_Info_->team_max)
    {
        return;
    }

    const DWORD char_guid = player->GetCharGuid();
    ;{
        Player* const member = FindMember(char_guid);
        //이미 팀에 포함되어 있을 경우..
        if (member != NULL) {
            player_hash_map_.erase(char_guid);
        }
    };

    player_hash_map_.insert(PLAYER_HASH::value_type(char_guid, player));

    int offset = 1;
    int index = (battle_ground_Info_->index) - offset;

    
    GoldRushRecord* const record = \
        player->GetExtraInfoManager().GetGoldRushRecord(index);
    if (record == NULL)
    {
        // Critical Error!
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GoldRush Record Error! Check BattlegroundInfo_Index");
    }

    record->map_code = goldrush_->GetInstanceDungeon()->GetMapCode();
    // 정보 초기화
    player->GetGoldRushPlayerInfo().Init(record);
    player->GetGoldRushPlayerInfo().SetTeamKey(team_key_);
    

    // 위치 이동
    OnMoveToSafezone(player);

    //CMD..
    MSG_CG_GOLDRUSH_TEAM_INFO_CMD cmd_msg;
    cmd_msg.team_key = team_key_;
    player->SendPacket(&cmd_msg, sizeof(cmd_msg));

    ready_user_keys_.erase(player->GetUserKey());
}

RC::eSTATUS_RESULT GoldRushTeam::OnMoveToSafezone( Player* const player )
{
    GameField* const field = player->GetField();
    if (field == NULL)
    {
        return RC::RC_STATUS_FIELD_IS_NULL;
    }

    const sMAPINFO* const map_info = \
        MapInfoParser::Instance()->FindMapInfo(goldrush_->GetInstanceDungeon()->GetMapCode());
    if (map_info == NULL)
    {
        return RC::RC_STATUS_MAPINFO_IS_NULL;
    }

    DWORD area_id = 0;
    if (team_key_ == kRed)
    {
        area_id = map_info->wStartAreaID;
    }
    else
    {
        area_id = map_info->wStartAreaID2;
    }

    WzVector dest_pos;
    if (field->GetAreaRandomStartingVector(area_id, dest_pos) == false)
    {
        return RC::RC_STATUS_IS_NOT_SAFEZONE;
    }

    // 시작위치로 텔레포트 시킨다.
    if(field->TeleportObject( player, &dest_pos, TRUE ) == false)
    {
        return RC::RC_STATUS_TELEPORT_FAIL;
    }

    return RC::RC_STATUS_SUCCESS;
}

void GoldRushTeam::_OnFinishResult( Player* const player, const ChaosZoneResultType result_type )
{
    MSG_CG_GOLDRUSH_FINISH_RESULT_BRD brd_msg;
    brd_msg.result_type = result_type;
    player->SendPacket(&brd_msg, sizeof(brd_msg));
}

void GoldRushTeam::Start()
{
    team_state_ = eCHAOSTEAM_STATE_MATCHED;

    PLAYER_HASH_ITR itr = player_hash_map_.begin();
    for (; itr != player_hash_map_.end(); ++itr)
    {
        Player* const player = itr->second;
        if (player == NULL || player->IsDeleted())
            continue;

        player->GetGoldRushPlayerInfo().SetPlayerState(kPlayerStateMatch);

        MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD cmd_msg;
        cmd_msg.player_state = kPlayerStateMatch;
        player->SendPacket(&cmd_msg, sizeof(cmd_msg));
    }
}

void GoldRushTeam::AllDeathMemberResurrectionAtSafezone()
{
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted()) {
            continue;
        }

        BattleGroundPlayerState player_goldrush_state = \
            player->GetGoldRushPlayerInfo().GetPlayerState();

        if (player_goldrush_state == kPlayerStateDeath || 
            player_goldrush_state == kPlayerStateMatch)
        {
            continue;
        }

        MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD cmd_msg;
        cmd_msg.player_object_key = player->GetObjectKey();
        if (player_goldrush_state == kPlayerStateStandbyResurection)
        {
            // 전장 부활 시 아이언바디(무적)
            ALLOC_STATUS_PAIR steel = \
                player->GetStatusManager()->AllocStatus(eCHAR_STATE_IMMUNITY_DAMAGE,
                battle_ground_Info_->iron_mode * 1000);
            if (steel.second) {
                steel.second->SendStatusAddBRD();
            }

            cmd_msg.player_state = kPlayerStateMatch;
        }
        else if (player_goldrush_state == kPlayerStateStandbyResurectionRefuse)
        {
            cmd_msg.player_state = kPlayerStateStandbyResurection;
            cmd_msg.resurection_time = goldrush_->GetRemainTicks();
        }
        else 
        {
            continue;
        }

        player->GetGoldRushPlayerInfo().SetPlayerState(cmd_msg.player_state);
        player->SendPacketAround(&cmd_msg, sizeof(cmd_msg));
    }
}

void GoldRushTeam::OnResult( const ChaosZoneResultType result_type )
{
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        ChaosZoneResultType current_result_type = result_type;
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted()) {
            continue;
        }

        // 개개인의 전투결과를 가지고 확인한다
        GoldRushBattleResult* const goldrush_result = goldrush_->GetGoldRushBattleResult();
        if (goldrush_result == NULL) {
            continue;
        } 
        const BattleRecorder::Record* const record = goldrush_result->GetPlayerBattleResult(player);
        if (record == NULL) {
            continue;
        }

        //Abusing Check
        if ((player->GetTeamKey() == kRed || player->GetTeamKey() == kBlue) &&
            record->cumulative_required_gold_point <= battle_ground_Info_->abusing_point)
        {
            current_result_type = kResultAbuse;
            goldrush_->GoldRushPenalty(player);
        }

        // 전투결과창 띄우기
#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
         goldrush_result->SendBattleResult_RegisterGameLog(player);
#else
         goldrush_result->SendBattleResult(player);
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
        // 보상창 띄우기
        if (player->GetTeamKey() == kRed || player->GetTeamKey() == kBlue) {
            _OnReward(player, current_result_type);
        }
        //결과 정보창 띄우기
        _OnFinishResult(player, current_result_type);

        //Observer는 로그기록 하지 않는다
        if (player->GetTeamKey() == kObserve){
            return;
        }

        GoldRushPlayerInfo& player_info = player->GetGoldRushPlayerInfo();
        switch(current_result_type)
        {
        case kResultWin:
            {
                player_info.IncreaseWinTimes();
                GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_WIN, player);
                _SetWinFlag(true);
            }break;
        case kResultDraw:
            {
                player_info.IncreaseDrawTimes();
                GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DRAW, player);
            }break;
        case kResultLose:
            {
                player_info.IncreaseLoseTimes();
                GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_LOSE, player);
            }break;
        case kResultAbuse:
            {
                player_info.IncreaseDisTimes();
                GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DIS, player);
            }break;
        }
        if (current_result_type != kResultAbuse) {
            player->GetGoldRushPlayerInfo().IncreasePlayTimes();
        }

    }
}

void GoldRushTeam::_OnReward( Player* const player, const ChaosZoneResultType result_type )
{
    FlowControl flow;
    FLOWCONTROL_START()
    {
        MissionManager* const mission_manager = player->GetMissionManager();
        mission_manager->CheckEnableWindowOpen(); // 그럴리 없다고 단정지어진 것이지만, 중복된 보상이 존재할 경우를 대비
        if (mission_manager->EnableWindowOpen())
        {
            TriggerManagerEx::Check_RewardWindow(player, true);
            return;
        }

        DWORD mission_number = 0;
        switch(result_type)
        {
        case kResultWin:
            mission_number = battle_ground_Info_->win_item_code;
            break;
        case kResultDraw:
        case kResultLose:
            mission_number = battle_ground_Info_->base_item_code;
            break;
        case kResultAbuse:
            mission_number = battle_ground_Info_->penalty_item_code;
            break;
        }

        if (!mission_number || !(mission_number < (DWORD)MAX_MISSION_NUM))
        {
            return;
        }

        const DWORD mission_point = mission_manager->GetMissionPoint(mission_number);

        printf("GoldRushTeam::_OnReward :User [%u] MissonNumber[%u], MissionPoint[%u]\n",
            player->GetUserKey(), mission_number, mission_point);

        MissionRewardManager* reward_manager = MissionRewardManager::Instance();
        reward_manager->Init();

        MissionRewardArgs& mission_reward_args = reward_manager->GetMissionRewardArgs();
        mission_reward_args.SetPlayer(player);
        mission_reward_args.SetMissionManager(mission_manager);
        mission_reward_args.SetNum(mission_number);

        const WORD reward_code = reward_manager->GetRewardCode(true);
        ASSERT(0xFFFF != reward_code);
        if (0xFFFF != reward_code)
        {
            MSG_CG_TRIGGER_REWARD_WINDOW_CMD cmsg;
            cmsg.m_RewardCode = reward_code;
            player->SendPacket(&cmsg, sizeof(cmsg));
        }

        mission_manager->SetMissionState(mission_number, MISSION_REWARD);
        mission_manager->SetMissionPoint(mission_number, mission_point + 1);

        SUNLOG(eFULL_LOG,  
            "[GoldRushTeam::_OnReward] [ID:%s]:미션 보상창을 띄웠다.",player->GetUserID());
    }
    FLOWCONTROL_END;
}

WORD GoldRushTeam::DistributeGoldPoint( WORD gold, const CHARGUID bonus_player /* = 0*/ )
{
    //각각 팀원들이 얻을 골드 계산
    // [NOTE]: 나누고 남은 값들을 어떻게 처리할까? -> 버린다
    BYTE member_count = GetMemberCount();
    if ( member_count <=0 ) {
        return 0;
    }
    WORD gold_foreach = gold / member_count;
    WORD result_distributed_point = 0;
    WORD base_kill_point = goldrush_->GetGoldRushInfo()->base_kill_point;
    if (gold <= 0 || gold_foreach <= 0) {
        return 0;
    }
    //분배
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted()) {
            continue;
        }
        //시작지점에선 골드를 받지 못한다
        const nsAI::TileEvent* tile_event = 0;
        if (player->PlanEvent->GetTileEvent(&tile_event) == true &&
            (tile_event->Attr & PTA_EXTRA_TILE)) {
                continue;
        }

        
        if (player->GetCharGuid() == bonus_player) {
            result_distributed_point = gold_foreach + base_kill_point;
        }
        else {
            result_distributed_point = gold_foreach;
        }
        goldrush_->GetGoldRushBattleResult()->DoRecord(static_cast<Character*>(player), 
                                          BATTLE_RECORD_GOLDPOINT, static_cast<int>(result_distributed_point));
        player->GetGoldRushPlayerInfo().AddGoldPoint(result_distributed_point);
    }

    return gold_foreach;
}

void GoldRushTeam::ObtainGoldPoint( WORD gold )
{
    if (gold == 0) {
        return;
    }

    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;
        //시작지점인가?

        goldrush_->GetGoldRushBattleResult()->DoRecord(static_cast<Character*>(player), 
                                                   BATTLE_RECORD_GOLDPOINT, static_cast<int>(gold));
        player->GetGoldRushPlayerInfo().AddGoldPoint(gold);
    }
}

const WORD GoldRushTeam::GetTotalTeamGoldPoint()
{
    WORD total_goldpoint = 0;
    BYTE member_count = GetMemberCount();
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;
        //시작지점인가?

        total_goldpoint += player->GetGoldRushPlayerInfo().GetGoldPoint();
    }

    return total_goldpoint;
}

void GoldRushTeam::GetTeamMemberInfo( OUT GOLDRUSH_TEAM_SCORE_INFO& team_info )
{
    BYTE member_count = GetMemberCount();
    if (member_count <= 0) 
    {
        team_info.team_member_count = 0;
        team_info.team_key = kChaosZoneTeamNone;
        team_info.team_gold_point = 0;
        return;
    }

    team_info.team_member_count = member_count;
    team_info.team_key = GetTeamKey();
    team_info.team_gold_point = GetTotalTeamGoldPoint();

    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for( int i=0; const_itr != player_hash_map_.end(); ++const_itr, ++i)
    {
        Player* const player = const_itr->second;
        if (player == NULL) {
            continue;
        }
        _tcsncpy(team_info.team_player_info[i].character_name, player->GetCharName(), MAX_CHARNAME_LENGTH);
        team_info.team_player_info[i].gold_grade = player->GetGoldRushPlayerInfo().GetGoldGrade();
    }
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

