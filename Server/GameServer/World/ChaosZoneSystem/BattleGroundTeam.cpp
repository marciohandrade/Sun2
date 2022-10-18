#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/BattleGroundTeam.h>
#include <../ProgramCommon/ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <World/ChaosZoneSystem/BattleGround.h>
#include "MapInfoParser.h"
#include "GameInstanceDungeon.h"
#include <ServerOptionParserEx.h>
#include <../ProgramCommon/BattleGroundInfoParser.h>
#include <StatusManager.h>
#include "MissionManager.h"
#include "MissionRewardManager.h"
#include "TriggerManagerEx.h"

void BattleGroundTeam::Update()
{
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    if (team_state_ != eCHAOSTEAM_STATE_MATCHED) {
        return;
    }

    PLAYER_HASH_ITR itr = player_hash_map_.begin();
    for (; itr != player_hash_map_.end(); ++itr)
    {
        Player* const player = itr->second;
        if (player == NULL || player->IsDeleted())
            continue;

        if (player->IsDead() &&
            (player->GetBattleGroundPlayerInfo().GetPlayerState() == kPlayerStateMatch ||
            player->GetBattleGroundPlayerInfo().GetPlayerState() == kPlayerStateDeath)
            )
        {
            if (player->GetBattleGroundPlayerInfo().IsExpiredMoveToSafeZone() == false)
            {
                continue;
            }
            player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateStandbyResurection);
            OnMoveToSafezone(player);
            player->OnResurrection( 0, 1.f, 1.f );

            MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD send_msg;
            send_msg.player_object_key = player->GetObjectKey();
            send_msg.player_state = kPlayerStateStandbyResurection;
            send_msg.resurection_time = battle_ground_->GetRemainTicks();
            player->SendPacketAround(&send_msg, sizeof(send_msg));
        }

        // 부활 스킬 및 사망 시 부활 아이템 옵션 동작 시..
        else if(player->IsAlive() && 
                player->GetBattleGroundPlayerInfo().GetPlayerState() == kPlayerStateDeath)
        {
            player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateMatch);
        }
    }

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    if (voting_timer_.IsExpiredManual(false) == true)
    {
        OnVoteResult(true);
    }
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    if (_IsExpired())
    {
        //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
        team_state_ = eCHAOSTEAM_STATE_END;
    }
}

void BattleGroundTeam::Start()
{
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    team_state_ = eCHAOSTEAM_STATE_MATCHED;

    PLAYER_HASH_ITR itr = player_hash_map_.begin();
    for (; itr != player_hash_map_.end(); ++itr)
    {
        Player* const player = itr->second;
        if (player == NULL || player->IsDeleted())
            continue;

        player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateMatch);

        MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD cmd_msg;
        cmd_msg.player_state = kPlayerStateMatch;
        player->SendPacket(&cmd_msg, sizeof(cmd_msg));
    }
}

void BattleGroundTeam::AddMember(Player* const player)
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
  
    BattleGroundRecord* const record = \
        player->GetExtraInfoManager().GetBattleGroundRecord(index);
    record->map_code = battle_ground_->GetInstanceDungeon()->GetMapCode();
    // 정보 초기화
    player->GetBattleGroundPlayerInfo().Init(record);
    player->GetBattleGroundPlayerInfo().SetTeamKey(team_key_);

    // 위치 이동
    OnMoveToSafezone(player);

    //CMD..
    MSG_CG_BATTLE_GROUND_TEAM_INFO_CMD cmd_msg;
    cmd_msg.team_key = team_key_;
    player->SendPacket(&cmd_msg, sizeof(cmd_msg));

    ready_user_keys_.erase(player->GetUserKey());
}

void BattleGroundTeam::AllDeathMemberResurrectionAtSafezone()
{
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted()) {
            continue;
        }

        BattleGroundPlayerState player_battle_ground_state = \
            player->GetBattleGroundPlayerInfo().GetPlayerState();

        if (player_battle_ground_state == kPlayerStateDeath || 
            player_battle_ground_state == kPlayerStateMatch)
        {
            continue;
        }

        MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD cmd_msg;
        cmd_msg.player_object_key = player->GetObjectKey();
        if (player_battle_ground_state == kPlayerStateStandbyResurection)
        {
            // 전장 부활 시 아이언바디(무적)
            ALLOC_STATUS_PAIR steel = \
                player->GetStatusManager()->AllocStatus(eCHAR_STATE_IMMUNITY_DAMAGE,
                                                        battle_ground_Info_->iron_mode * 1000);
            if (steel.second)
            {
                steel.second->SendStatusAddBRD();
            }

            cmd_msg.player_state = kPlayerStateMatch;
        }
        else if (player_battle_ground_state == kPlayerStateStandbyResurectionRefuse)
        {
            cmd_msg.player_state = kPlayerStateStandbyResurection;
            cmd_msg.resurection_time = battle_ground_->GetRemainTicks();
        }
        else 
        {
            continue;
        }

        player->GetBattleGroundPlayerInfo().SetPlayerState(cmd_msg.player_state);
        player->SendPacketAround(&cmd_msg, sizeof(cmd_msg));
    }
}

RC::eSTATUS_RESULT BattleGroundTeam::OnMoveToSafezone(Player* const player)
{
    GameField* const field = player->GetField();
    if (field == NULL)
    {
        return RC::RC_STATUS_FIELD_IS_NULL;
    }

    const sMAPINFO* const map_info = \
        MapInfoParser::Instance()->FindMapInfo(battle_ground_->GetInstanceDungeon()->GetMapCode());
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

void BattleGroundTeam::IncreasePlayerKillPoint(Player* const player, Player* const target_player)
{
    // 킬 증가
    if (player)
        player->GetBattleGroundPlayerInfo().IncreaseKill();
}

void BattleGroundTeam::IncreaseVictroyPoint(const int increase_victory_point)
{
    victory_point_ += increase_victory_point;
}

void BattleGroundTeam::OnResult(const ChaosZoneResultType result_type)
{
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        ChaosZoneResultType current_result_type = result_type;
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
        {
            continue;
        }
    
        // 개개인의 전투결과를 가지고 확인한다..
        BattleGroundBattleResult* const battle_ground_result = \
            battle_ground_->GetBattleGroundBattleResult();
        if (battle_ground_result == NULL)
        {
            continue;
        }
        const BattleRecorder::Record* const record = \
            battle_ground_result->GetPlayerBattleResult(player);
        if (record == NULL)
        {
            continue;
        }

        // 잠수 어뷰징 유저의 조건..
        // 2013.06.13 기준 (총 가해 데미지 + 총 피해 데미지 + 총 힐량 + 킬포인트 + 어시스트 포인트 + 거점공격 포인트 + 거점방어 포인트)
        const DWORD total_abusing_point = record->total_damage_to_enemy +           // 총 가해 데미지
                                          record->total_damage_by_enemy +           // 총 피해 데미지
                                          record->total_heal_to_party +             // 총 힐량
                                          (record->player_kill_count * 1000) +      // kill * 1000
                                          (record->assist_count * 100) +            // assist * 100
                                          (record->strongpoint_attack * 10000) +    // 거점 공격 * 10000
                                          (record->strongpoint_defense * 10000);    // 거점 방어 * 10000


        // abuse_point check..
        if ((player->GetTeamKey() == kRed || player->GetTeamKey() == kBlue) &&
            (total_abusing_point <= battle_ground_Info_->abusing_point)) 
        {
            current_result_type = kResultAbuse;
            battle_ground_->BattleGroundPenalty(player);
            SUNBATTLEGROUNDLOG(
                eCRITICAL_LOG, 
                TEXT("| [ZONE_KEY(%d)] | BattleGround Penalty Reason | Guid = %d , CharName = %s | total_abusing_point = %d, total_damage_to_enemy = %d, total_damage_by_enemy = %d, total_heal = %d, kill_count = %d, assist_count = %d, strongpoint_attack = %d, strongpoint_defense = %d |"), 
                battle_ground_->GetInstanceDungeon()->GetKey(),
                player->GetUserGuid(),
                player->GetCharName(),
                total_abusing_point,
                record->total_damage_to_enemy,
                record->total_damage_by_enemy,
                record->total_heal_to_party,
                (record->player_kill_count * 1000),
                (record->assist_count * 100),
                (record->strongpoint_attack * 10000),
                (record->strongpoint_defense * 10000));
        }

        // 전투 결과창 띄우기..
        battle_ground_result->SendBattleResult(player);

        // 보상 창 띄우기..
        if (player->GetTeamKey() == kRed || player->GetTeamKey() == kBlue) {
            _OnReward(player, current_result_type);
        }
        
        // 결과 정보 창 띄우기..
        _OnFinishResult(player, current_result_type);

        // obsever는 로그기록을 하지 않는다..
        if (player->GetTeamKey() == kObserve) {
            return;
        }

        switch (current_result_type)
        {
        case kResultWin:
            player->GetBattleGroundPlayerInfo().IncreaseWinTimes();
            GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_WIN, player);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            _SetWinFlag(true);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            break;
        case kResultDraw:
            player->GetBattleGroundPlayerInfo().IncreaseDrawTimes();
            GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DRAW, player);
            break;
        case kResultLose:
            player->GetBattleGroundPlayerInfo().IncreaseLoseTimes();
            GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_LOSE, player);
            break;
        case kResultAbuse:
            player->GetBattleGroundPlayerInfo().IncreaseDisTimes();
            GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DIS, player);
            break;
        }

        if (current_result_type != kResultAbuse)
            player->GetBattleGroundPlayerInfo().IncreasePlayTimes();
    }
}

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
bool BattleGroundTeam::IsInitialteTime()
{
    // time check..
    if (initialte_vote_timer_.IsExpiredManual(false) == false) {
        return false;
    }
    return true;
}

bool BattleGroundTeam::OnInitiateVote(Player* initiate_player, Player* van_payer)
{
    if (is_initialte_vote_ == true)
    {
        return false;
    }

    voting_timer_.SetTimer(30 * 1000);

    // 발의자 및 강제 추방 대상자에게는 투표 현황..
    ;{
        MSG_CG_BATTLE_GROUND_VOTE_STATUS_BRD brd_msg;
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
        MSG_CG_BATTLE_GROUND_INITIATE_VOTE_BRD brd_msg;
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

void BattleGroundTeam::OnVote(CHARGUID char_guid, bool vote_result)
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
            MSG_CG_BATTLE_GROUND_VOTE_STATUS_BRD brd_msg;
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

bool BattleGroundTeam::CheckVoteResult()
{
    const int current_memeber_count = player_hash_map_.size();
    const int vote_member_count = vote_player_hash_map_.size();

    if (current_memeber_count == vote_member_count)
    {
        return true;
    }

    return false;
}

void BattleGroundTeam::OnVoteResult(bool van_result)
{
    if (is_initialte_vote_ == false)
        return;

    // 투표결과 BRD..
    MSG_CG_BATTLE_GROUND_VOTE_RESULT_BRD brd_msg;
    brd_msg.vote_result = van_result;
    _tcsncpy(brd_msg.van_payer_name, van_player_name_, MAX_CHARNAME_LENGTH);

    if (van_result) 
    {
        // 해당 플에이어 강제 퇴장..
        OnVanPlayer();

        // 투표 가결 시.. 전장 모든 유저에게 BRD..
        battle_ground_->SendPacketToAll(&brd_msg, sizeof(brd_msg));

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

void BattleGroundTeam::OnVanPlayer()
{
    Player* const van_player = PlayerManager::Instance()->FindPlayerByName(van_player_name_);
    if (van_player == NULL) {
        return;
    }

    OnMoveToSafezone(van_player);
    battle_ground_->BattleGroundPenalty(van_player);

    MSG_CG_BATTLE_GROUND_VANP_PLAYER_CMD cmd_msg;
    van_player->SendPacket(&cmd_msg, sizeof(cmd_msg));
}
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

//--------------------------------------------------------------------------------------------------
bool BattleGroundTeam::_IsExpired()
{
    //목표점수 도달..
    if (victory_point_ >= battle_ground_Info_->goal_score) //script..
    {
        return true;
    }
    return false; 
}

void BattleGroundTeam::_OnReward(Player* const player, const ChaosZoneResultType result_type)
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

        printf("BattleGroundTeam::_OnReward :User [%u] MissonNumber[%u], MissionPoint[%u]\n",
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
            "[BattleGroundTeam::_OnReward] [ID:%s]:미션 보상창을 띄웠다.",player->GetUserID());

        TCHAR* result_type_string[] =
        {
            "Error!",
            "WIN",
            "LOSE",
            "DRAW",
            "ABUSE"
        };

        SUNBATTLEGROUNDLOG(
            eCRITICAL_LOG, 
            TEXT("| [ZONE_KEY(%d)] | Reward | Guid = %d, Match Result = %s, TEAM = %d, CharName = %s, Reward Code = %d |"), 
            battle_ground_->GetInstanceDungeon()->GetKey(),
            player->GetUserGuid(),
            result_type_string[result_type],
            player->GetTeamKey(),
            player->GetCharName(),
            reward_code);
    }
    FLOWCONTROL_END;
}

void BattleGroundTeam::_OnFinishResult(Player* const player, const ChaosZoneResultType result_type)
{
    MSG_CG_BATTLE_GROUND_FINISH_RESULT_BRD brd_msg;
    brd_msg.result_type = result_type;
    player->SendPacket(&brd_msg, sizeof(brd_msg));
}

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
