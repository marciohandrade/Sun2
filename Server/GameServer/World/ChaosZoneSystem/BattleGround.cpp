#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/BattleGround.h>
#include <World/ChaosZoneSystem/BattleGroundTeam.h>
#include <GameInstanceDungeon.h>
#include <../ProgramCommon/ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <../ProgramCommon/BattleGroundInfoParser.h>
#include <../Server/Common/PacketStruct_AG_ChaosZoneSystem.h>
#include <TriggerManagerEx.h>
#include <CollectionParser.h>
#include <World/ChaosZoneSystem/BattleGroundBattleResult.h>
#include <BattleGroundFlagInfoParser.h>
#include <StatusManager.h>
#include <SkillInfoParser.h>
#include <SkillManager.h>
#include <AbilityFactory.h>
#include <Ability.h>
#include <AbilityStatus.h>
#include <CollectionManager.h>
#include <EquipmentSlotContainer.h>
#include <ItemManager.h>
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include "GMCmdManager.h"
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

void BattleGround::Init(GameInstanceDungeon* const instance_dungeon)
{
    instance_dugeon_ = instance_dungeon;

    // 팀 초기화
    for (int i = 0; i < kTeamMax; ++i)
    {
        battle_ground_team_[i] = NULL;
    }

    // 전투결과창 생성
    battle_ground_battle_result_ = new BattleGroundBattleResult();
    battle_ground_battle_result_->Init(true);
    battle_ground_battle_result_->SetBattleGround(this);

    // 전장 스크립트 미리 읽기
    const BattleGroundInfoParser* const battle_ground_Info_parser = BattleGroundInfoParser::Instance();
    if (battle_ground_Info_ == NULL)
    {
        const BattleGroundInfo* battle_ground_Info = battle_ground_Info_parser->FindData(instance_dugeon_->GetMapCode());
        if (battle_ground_Info)
        {
            battle_ground_Info_ = battle_ground_Info;
        }
    }

    is_count_message_ = true;
}

void BattleGround::Release()
{
    //거점 제거
    _DestroyStrongPointAll();

    //팀 제거
    _DestroyTeamAll();

    // 전투결과창 제거
    SAFE_RELEASENDELETE(battle_ground_battle_result_);
}

void BattleGround::Update()
{
    switch(battle_ground_state_)
    {
    case kAllEnterWait:
        {
            // 양팀 멤버가 모두 들어 왔을 경우
            if (_IsAllPlayerEntered())
            {
                // 전장 준비 상태로 전환
                _StartWait();
                break;
            }

            // 일정 시간이 지나도 양팀 멤버가 모두 들어 오지 않은 경우..(들어 오는 중간에 비정상 접속종료 된 유저들이 있을 경우)
            if (start_wait_timer_.IsExpiredManual(false) == false) break;

            // 전장 준비 상태로 강제 전환
            _StartWait();
        }
        break;
    
    case kPrepareWait:
        {
            if (is_count_message_ && start_wait_timer_.GetRemainedTime() <= 5 * kSecond)
            {
                const FIELDCODE field_code = static_cast<FIELDCODE>(instance_dugeon_->GetMapCode());
                GameField* const game_field = instance_dugeon_->FindGameField(field_code);
                if (game_field == NULL)
                    return;

                if (game_field->IsReady() == false) 
                    return;

                TRIGGER_SERVER_EVENT trigger_msg;
                trigger_msg.event_id = 1;
                game_field->GetTriggerManager()->OnMsg(&trigger_msg);

                is_count_message_ = false;
            }

            // 준비시간 후 전장 시작.
            if (start_wait_timer_.IsExpiredManual(false) == false) break;
         
            // 전장 시작
            _Start();           
        }
        break;
    
    case kMatch:
        {
            for (int i = 0; i < kBlue; ++i)
            {
                if (battle_ground_team_[i])
                {
                    battle_ground_team_[i]->Update();
                }
            }

			// 부활 스케줄링..
            if (resurrection_timer_.IsExpired())
            {
                _AllDeathMemberResurrectionAtSafezone();
            }

            // 거점 스케줄링..
            _UpdateStrongpoint();

            // 승점 스케줄링..
            if (victory_point_timer_.IsExpired())
            {
                _IncreaseVictoryPoint();
            }

            // 종료조건 되거나 시간이 다 되면 결과처리한다..
            if (_IsBattleGroundEnd())
            {
                battle_ground_state_ = kResult;
            }
        }
        break;

    case kResult:
        {
            // 보상처리..
            _Result();
            battle_ground_state_ = kEnd;
            // 전장 종료..
            SUNBATTLEGROUNDLOG(
                eCRITICAL_LOG, 
                TEXT("| [ZONE_KEY(%d)] | End | 전장 종료!"), 
                instance_dugeon_->GetKey());
        }
        break;

    case kEnd:
        {   
            // 전장 종료..
        }
        break;
    }
}

void BattleGround::EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, 
                             DWORD* user_key_list)
{
    BattleGroundTeam* const team = _CreateTeam(team_key);
    team->SetReadyPlayer(player_count, user_key_list);

    if (battle_ground_state_ == kNone)
    {
        // 팀 생성 단계에서 팀매칭 인원이 모두 들어 오기까지 기다리는 시간 카운트..
        start_wait_timer_.SetTimer(battle_ground_Info_->entry_time * kSecond); //script..
        battle_ground_state_ = kAllEnterWait;
        SUNBATTLEGROUNDLOG(
            eCRITICAL_LOG, 
            TEXT("| [ZONE_KEY(%d)] | Ready All Player Enter | 전장 입장 대기! |"), 
            instance_dugeon_->GetKey());
    }

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | EnterTeam | TEAM KEY = %d, Player count = %d |"), 
        instance_dugeon_->GetKey(), 
        team_key,
        player_count);
}

bool BattleGround::EnterPlayer(Player* const player)
{
    if (player == NULL || player->IsDeleted())
        return false;

    BattleGroundTeam* team = _FindReadyTeam(player);
    if (team == NULL || team->GetTeamKey() == kObserve)
    {
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        GMCmdManager* const gm_manager = GMCmdManager::Instance();
        Player* gm_player = gm_manager->GetGMPlayer(player->GetUserGuid());
        if (gm_player == NULL)
            return false;
        else
        {
            team = _FindTeam(kObserve);
            if (team == NULL)
                team = _CreateTeam(kObserve);
        }
        team->AddMember(player);

        // team_key setting..
        ChaosZoneTeam team_key = team->GetTeamKey();
        player->SetTeamKey(team_key);
        
        _SentToAllTeamInfo();
        return true;
#else
        return false;
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    }
    team->AddMember(player);

    // team_key setting..
    ChaosZoneTeam team_key = team->GetTeamKey();
    player->SetTeamKey(team_key);

    //전장 Total list;
    player_hash_.insert(PLAYER_HASH::value_type(player->GetCharGuid(), player));

    bool is_abnormal_leave = false;

    // 비정상 접속 종료자.
    if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE) != NULL || //도망자 버프가 있을 경우..
        battle_ground_state_ == kPrepareWait || // 존 이동 도중 비정상 종료되어 준비시간에 들어왔을 경우..
        battle_ground_state_ == kMatch) // 존 이동 도중 비정상 종료되어 전장 시합 중에 들어왔을 경우..
    {
        is_abnormal_leave = true;

        // 상태제거 (도망자 버프가 걸린 사람은 예약이 안돼기 때문에 어뷰징에 해당하지 않음)
        if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE))
        {
            if (!player->GetStatusManager()->Remove(eCHAR_STATE_FUGITIVE))
            {
                MSG_CG_STATUS_REMOVE_BRD BrdMsg;
                BrdMsg.m_dwTargetKey = player->GetObjectKey();
                BrdMsg.m_dwStatusCode = eCHAR_STATE_FUGITIVE;
                player->SendPacketAround(&BrdMsg, BrdMsg.GetSize());
            }
            // dis count 감소
            player->GetBattleGroundPlayerInfo().DecreaseDisTimes();
        }

        player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateMatch);

        MSG_CG_BATTLE_GROUND_TIME_INFO_CMD cmd_msg;
        ;{
            cmd_msg.elapsed_time = 1;
            player->SendPacket(&cmd_msg, sizeof(cmd_msg));
        };

        _SentToAllTeamInfo();

        MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN brd_msg;
        ;{
            brd_msg.strongpoint_count = static_cast<DWORD>(strongpoint_trigger_hash_.size());
            int index = 0;
            STRONGPOINT_TRIGGER_HASH::iterator itr = strongpoint_trigger_hash_.begin();
            for (; itr != strongpoint_trigger_hash_.end(); ++itr, ++index)
            {
                StrongpointTrigger* const strongpoint = itr->second;
                brd_msg.strongpoint_info[index].strongpoint_id = strongpoint->strongpoint_id;
                brd_msg.strongpoint_info[index].team_key = strongpoint->conquer_team_key;
                brd_msg.strongpoint_info[index].strongpoint_state = strongpoint->trigger_state;
            }
            player->SendPacket(&brd_msg,sizeof(brd_msg));
        };
    }

    else if (battle_ground_state_ == kMatch) // 전장 들어오기전 비정상접송종료자 처리..
    {
        is_abnormal_leave = true;
    }


    TCHAR add_info[MAX_ADDINFO_SIZE] = { 0, };
    if (is_abnormal_leave)
        ::_sntprintf(add_info, MAX_ADDINFO_SIZE, _T("RETURN ABNORMAL EXIT!"));
    add_info[MAX_ADDINFO_SIZE-1] = _T('\0');

    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_ENTER, player, NULL, add_info);

    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer* const>(
        player->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
    const ns_formula::EquipSlotsManageInfo* const equip_manage_info = \
        equip_container->GetEquipmentManageInfo();
    const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = \
        equip_manage_info->item_score_info;

    TCHAR* class_type[] = 
    {
        "Error!",
        "BERSERKER",
        "DRAGON",
        "SHADOW",
        "VALKYRIE",
        "ELEMENTALIST",
        "MYSTIC",
        "HELROID", //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        "WITCHBLADE", //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | EnterPlayer | Guid = %d, TeamKey = %d, CharName = %s, Class = %s, Lev = %d, ItemScore = %d, abnormal exit = %s |"), 
        instance_dugeon_->GetKey(), 
        player->GetUserGuid(),
        player->GetTeamKey(),
        player->GetCharName(),
        class_type[player->GetCharType()],
        player->GetLevel(),
        item_score_info.GetTotalScore(),
        is_abnormal_leave ? TEXT("true") : TEXT("false"));

    return true;
}

void BattleGround::LeavePlayer(Player* const player)
{
    if (player == NULL || player->IsDeleted())
        return;

    ChaosZoneTeam team_key = player->GetBattleGroundPlayerInfo().GetTeamKey();
    BattleGroundTeam* const team = _FindTeam(team_key);
    if (team == NULL)
    {
        return;
    }

    // 비정상 접속 종료를 대비해 넣어 둔다.
    team->SetReadyPlayer(player);

    team->RemoveMember(player->GetCharGuid());

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* gm_player = gm_manager->GetGMPlayer(player->GetUserGuid());
    if (gm_player && gm_player->GetTeamKey() == kObserve)
    {
        player->SetTeamKey(kChaosZoneTeamNone);
        return;
    }
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

    //플레이어 팀키 초기화
    player->SetTeamKey(kChaosZoneTeamNone);

    //전장 Total list;
    player_hash_.erase(player->GetCharGuid());

    MSG_AG_BATTLE_GROUND_LEAVE_CMD msg;
    msg.zone_key = instance_dugeon_->GetKey();
    player->SendPacket(&msg, sizeof(msg));

    //시작 대기시간 및 시합 중 나간 유저 패널티 적용
    if (battle_ground_state_ == kPrepareWait || 
        battle_ground_state_ == kMatch)
    {
        BattleGroundPenalty(player);
    }
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //전장 종료 시(정상접속에 한해서), 랭킹DB의 전장기록을 갱신
    else
    {
        const BattleRecorder::Record* player_record = GetBattleGroundBattleResult()->GetPlayerBattleResult(player);
        player->GetRankingSystemCtl()->BattleGroundLeaveRoom(player, player_record, team->GetIsWin());
    }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif // _NA_008012_20150130_RANKING_SYSTEM

    // 전장 전적 보내주기..
    MSG_CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD cmd_msg;
    const BattleGroundRecordInfos record_infos = \
        player->GetExtraInfoManager().GetBattleGroundRecordInfos();
    memcpy(cmd_msg.chaos_zone_record_infos.battle_ground_record, 
        record_infos.battle_ground_record, 
        sizeof(cmd_msg.chaos_zone_record_infos.battle_ground_record));

    player->SendPacket(&cmd_msg, sizeof(cmd_msg));

    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_LEAVE, player);

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | LeavePlayer | Guid = %d , CharName = %s |"), 
        instance_dugeon_->GetKey(),
        player->GetUserGuid(),
        player->GetCharName());
}

void BattleGround::DeadCharacter(Character* const dead_character, Character* const kill_character)
{
    if(dead_character == NULL || dead_character->IsDeleted())
        return;

    if (dead_character->IsEqualObjectKind(PLAYER_OBJECT) == false)
        return;

    // 죽은 플레이어 부활
    Player* const dead_player = static_cast<Player*>(dead_character);
    dead_player->GetBattleGroundPlayerInfo().SetMoveToSafeZoneTimer(battle_ground_Info_->dead_time * 1000);//script..
    dead_player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateDeath);

    // 죽은 횟수 증가
    const WORD dead_player_series_kill_count = dead_player->GetBattleGroundPlayerInfo().GetSeriesKill();
    dead_player->GetBattleGroundPlayerInfo().IncreaseDeath(); 
    
    // 킬러가 없으면 리턴
    if (kill_character == NULL || kill_character->IsDeleted())
    {
        // 전장에서 죽을 로그기록 (킬러가 없을경우.. 트리거에 의한 죽음..)
        GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DEATH, dead_player);
        return;
    }

    // 킬러가 있으면
    Player* kill_player = NULL;
    if (kill_character->IsEqualObjectKind(PLAYER_OBJECT))
    {
        kill_player = static_cast<Player*>(kill_character);
    }
    else if (kill_character->IsEqualObjectKind(SUMMON_OBJECT))
    {
        kill_player = _FindPlayer(kill_character->GetPlayerOwner()->GetCharGuid());
    }

    if (kill_player == NULL || kill_player->IsDeleted())
        return;

    kill_player->GetBattleGroundPlayerInfo().IncreaseKill(); // 죽인 횟수 증가
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
    battle_ground_team_[kill_player->GetTeamKey()-1]->IncreaseVictroyPoint(battle_ground_Info_->pk_point);
#endif

#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    //어시스트 횟수 증가시킨다
    Player* assist_player = NULL;
    assist_player = dead_player->GetPlayerAssistMeter()->GetBestAssistPlayer(kill_player->GetCharGuid());
    if(assist_player != NULL)
    {
        battle_ground_battle_result_->DoRecord(assist_player, BATTLE_RECORD_ASSIST, 1);
        assist_player->GetBattleGroundPlayerInfo().IncreaseAssistTimes();
    }
    dead_player->GetPlayerAssistMeter()->Clear();
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    

#ifdef _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY
    ;{
        MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD brd_msg;
        _tcsncpy(brd_msg.kill_payer_name, kill_player->GetCharName(), MAX_CHARNAME_LENGTH);
        _tcsncpy(brd_msg.death_payer_name, dead_player->GetCharName(), MAX_CHARNAME_LENGTH);

        // 연속킬 차단
        if (dead_player_series_kill_count > 1)
            brd_msg.is_series_kill_block = true;

        // 연속킬
        if (kill_player->GetBattleGroundPlayerInfo().GetSeriesKill() > 1)
        {
            brd_msg.is_series_kill = true;
            brd_msg.series_kill_count = kill_player->GetBattleGroundPlayerInfo().GetSeriesKill();
        }

#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
        //PKPOINT ( 적 처치시 얻는 점수)
        brd_msg.pk_point = battle_ground_Info_->pk_point;
        //추가점수 획득 후 갱신된 총 승점
        for (int i = 0; i < kBlue; ++i)
        {
            if(battle_ground_team_[i] == NULL)
                continue;

            brd_msg.team_score_info[i].team_key = battle_ground_team_[i]->GetTeamKey();
            brd_msg.team_score_info[i].strongpoint_count = battle_ground_team_[i]->GetStrongpointCount();
            brd_msg.team_score_info[i].victory_point = battle_ground_team_[i]->GetVictoryPoint();
        }
#endif
        _SendPacketToAll(&brd_msg, sizeof(brd_msg));
    };
#endif //_NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY
    
    // 전장에서 죽은 로그기록
    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DEATH, dead_player, kill_player->GetCharName());
    
    // 전장에서 킬 로그기록
    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_KILL, kill_player, dead_player->GetCharName());

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | KILL Player | Kill Player Guid = %d, CharName = %s | --> | Death Player Guid = %d, CharName = %s |"), 
        instance_dugeon_->GetKey(),
        kill_player->GetUserGuid(),
        kill_player->GetCharName(),
        dead_player->GetUserGuid(),
        dead_player->GetCharName());
}

eUSER_RELATION_TYPE	BattleGround::IsFriendPVP(Player* attacker,Player* target)
{
    if (battle_ground_state_ != kMatch)
    {
        return USER_RELATION_FRIEND;
    }

    if (target->GetBattleGroundPlayerInfo().GetPlayerState() != kPlayerStateMatch ||
        attacker->GetBattleGroundPlayerInfo().GetPlayerState() != kPlayerStateMatch)
    {
        return USER_RELATION_FRIEND;
    }

    if (attacker->GetBattleGroundPlayerInfo().GetTeamKey() == target->GetBattleGroundPlayerInfo().GetTeamKey())
    {
        return USER_RELATION_FRIEND;
    }

    return USER_RELATION_ENEMY;
}

bool BattleGround::StartSealing(Player* player, WzID collection_id)
{
    if (battle_ground_state_ != kMatch)
    {
        return false;
    }

    if (player->GetBattleGroundPlayerInfo().GetPlayerState() != kPlayerStateMatch)
    {
        return false;
    }

    StrongpointTrigger* const strongpoint = _FindStrongPoint(collection_id);
    if (strongpoint == NULL)
    {
        return false;
    }

    if (strongpoint->is_start_sealing == true)
    {
        if (strongpoint->sealing_timer.IsExpired(false) == false)
            return false;
    }

    switch(strongpoint->trigger_state)
    {
    case kStrongpointStateWait:
    case kStrongpointStateWaitAttack:
        {
            if (strongpoint->conquer_wait_team_key == player->GetBattleGroundPlayerInfo().GetTeamKey())
            {
                return false;
            }
        }
        break;

    case kStrongpointStateConquer:
        {
            if (strongpoint->conquer_team_key == player->GetBattleGroundPlayerInfo().GetTeamKey())
            {
                return false;
            }
        }
        break;
    }

    strongpoint->is_start_sealing = true;
    strongpoint->sealing_player_key = player->GetCharGuid();

    //각인 시작 
    strongpoint->sealing_timer.SetTimer(battle_ground_Info_->sealing_time * 1000); //script..

    return true;
}

void BattleGround::CancelSealing(Player* player, WzID collection_id)
{
    if (battle_ground_state_ != kMatch)
    {
        return;
    }

    StrongpointTrigger* const strongpoint = _FindStrongPoint(collection_id);
    if (strongpoint == NULL || strongpoint->is_start_sealing != true)
    {
        return;
    }

    if (player && strongpoint->sealing_player_key != player->GetCharGuid()) {
        return;
    }

    strongpoint->is_start_sealing = false;
    strongpoint->sealing_player_key = 0;
}

void BattleGround::SuccessSealing(Player* player, WzID collection_id)
{
    if (battle_ground_state_ != kMatch)
    {
        return;
    }

    StrongpointTrigger* const strongpoint = _FindStrongPoint(collection_id);
    if (strongpoint == NULL)
    {
        return;
    }

    if (strongpoint->sealing_player_key != player->GetCharGuid())
    {
        strongpoint->is_start_sealing = false;
        strongpoint->sealing_player_key = 0;
        return;
    }

    //각인 시간 체크..
    if (strongpoint->sealing_timer.IsExpired(false) == false)
    {
        strongpoint->is_start_sealing = false;
        strongpoint->sealing_player_key = 0;
        return;
    }

    strongpoint->is_start_sealing = false;
    strongpoint->sealing_player_key = 0;

    // 점령 대기 성공 or 방어 성공
    strongpoint->is_sealing = true;
    strongpoint->sealing_success_player_key = player->GetCharGuid();

    switch(strongpoint->trigger_state)
    {
    case kStrongpointStateNone:
    case kStrongpointStateWait:
        {
            strongpoint->conquer_wait_team_key = player->GetBattleGroundPlayerInfo().GetTeamKey();
            strongpoint->conquer_wait_timer.SetTimer(battle_ground_Info_->conquer_wait_time * 1000);
            strongpoint->trigger_state = kStrongpointStateWait;

            SUNBATTLEGROUNDLOG(
                eCRITICAL_LOG, 
                TEXT("| [ZONE_KEY(%d)] | Success Sealing | Guid = %d , CharName = %s, Stronpoint ID = %d |"), 
                instance_dugeon_->GetKey(),
                player->GetUserGuid(),
                player->GetCharName(),
                strongpoint->strongpoint_id);
        }
        break;

    case kStrongpointStateConquer:
    case kStrongpointStateDefence:
        {
            if (strongpoint->conquer_team_key == player->GetBattleGroundPlayerInfo().GetTeamKey())
            {
                return;
            }

            BattleGroundTeam* const team = _FindTeam(strongpoint->conquer_team_key);
            if (team == NULL)
            {
                return;
            }
            team->DecreaseStrongpointCount();

            strongpoint->is_conquer = false;
            strongpoint->conquer_wait_team_key = player->GetBattleGroundPlayerInfo().GetTeamKey();
            strongpoint->conquer_wait_timer.SetTimer(battle_ground_Info_->conquer_wait_time * 1000);
            strongpoint->trigger_state = kStrongpointStateWaitAttack;

            SUNBATTLEGROUNDLOG(
                eCRITICAL_LOG, 
                TEXT("| [ZONE_KEY(%d)] | Success Sealing | Guid = %d , CharName = %s, Stronpoint ID = %d |"), 
                instance_dugeon_->GetKey(),
                player->GetUserGuid(),
                player->GetCharName(),
                strongpoint->strongpoint_id);
        }
        break;

    case kStrongpointStateWaitAttack: // 대기상태(공격)
        {
            //방어성공..
            if (strongpoint->is_conquer == false &&
                strongpoint->conquer_team_key == player->GetBattleGroundPlayerInfo().GetTeamKey())
            {
                strongpoint->is_conquer = true;
                strongpoint->conquer_wait_team_key = kChaosZoneTeamNone;
                strongpoint->trigger_state = kStrongpointStateDefence;
                player->GetBattleGroundPlayerInfo().IncreaseStrongpointDefense();

                // 전투결과창에 기록
                battle_ground_battle_result_->DoRecord(static_cast<Character*>(player),
                    BATTLE_RECORD_STRONGPOINT_DEFENSE, 1);

                BattleGroundTeam* const team = _FindTeam(strongpoint->conquer_team_key);
                if (team == NULL)
                {
                    return;
                }
                team->IncreaseStrongpointCount();
            }

            // 거점 방어성공 로그기록
            GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_STRONGPOINT_DEFENSE, player);

            SUNBATTLEGROUNDLOG(
                eCRITICAL_LOG, 
                TEXT("| [ZONE_KEY(%d)] | Success Stronpoint Defense | Guid = %d , CharName = %s, Stronpoint ID = %d |"), 
                instance_dugeon_->GetKey(),
                player->GetUserGuid(),
                player->GetCharName(),
                strongpoint->strongpoint_id);

            SUNBATTLEGROUNDLOG(
                eCRITICAL_LOG, 
                TEXT("| [ZONE_KEY(%d)] | Current StronPoint State | [Team = %d, StrogPointCount = %d, VictoryPoint = %d] vs [Team = %d, StrogPointCount = %d, VictoryPoint = %d] |)"), 
                instance_dugeon_->GetKey(),
                battle_ground_team_[0]->GetTeamKey(),
                battle_ground_team_[0]->GetStrongpointCount(),
                battle_ground_team_[0]->GetVictoryPoint(),
                battle_ground_team_[1]->GetTeamKey(),
                battle_ground_team_[1]->GetStrongpointCount(),
                battle_ground_team_[1]->GetVictoryPoint());
        }
        break;
    }

    MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD state_brd_msg;
    {
        state_brd_msg.strongpoint_info.strongpoint_id = collection_id;
        state_brd_msg.strongpoint_info.team_key = player->GetBattleGroundPlayerInfo().GetTeamKey();
        state_brd_msg.strongpoint_info.strongpoint_state = strongpoint->trigger_state;
        _SendPacketToAll(&state_brd_msg,sizeof(state_brd_msg));
    }

    // 변경 된 거점 정보 BRD..
    _SendVictoryPointToAll();
}

bool BattleGround::CanCheckSealing(Player* player, WzID collection_id) const
{
    StrongpointTrigger* const strongpoint = _FindStrongPoint(collection_id);
    if (strongpoint == NULL)
    {
        return false;
    }

    // 점령대기 중일 때..
    if (strongpoint->conquer_wait_team_key == player->GetBattleGroundPlayerInfo().GetTeamKey())
    {
        return false;
    }

    // 점령 중일때..
    if (strongpoint->conquer_wait_team_key == kChaosZoneTeamNone && 
        strongpoint->conquer_team_key == player->GetBattleGroundPlayerInfo().GetTeamKey())
    {
        return false;
    }

    return true;
}

void BattleGround::BattleGroundPenalty(Player* player)
{
    if (player == NULL || player->IsDeleted())
        return;

    // dis count 증가
    player->GetBattleGroundPlayerInfo().IncreaseDisTimes();

    // 패널티 적용..(도망자 버프)
    int skill_code = battle_ground_Info_->exit_condition;
    SkillInfoParser* const skill_info_parser = SkillInfoParser::Instance();
    SkillScriptInfo* const skill_info = skill_info_parser->GetSkillInfo(skill_code);
    if (skill_info == NULL)
    {
        return;
    }
    BASE_ABILITYINFO* ability_info = skill_info->GetAbilityInfo(eABILITY_FUGITIVE);
    if (ability_info == NULL)
    {
        return;
    }

    // script 시간 셋팅..
    ability_info->m_iParam[2] = battle_ground_Info_->exit_period * kSecond;

    AbilityFactory* const ability_factory = AbilityFactory::Instance();
    Ability* ability = ability_factory->AllocAbility(eABILITY_FUGITIVE, SKILL_TYPE_ACTIVE);
    if (ability == NULL)
    {
        return;
    }
    ability->InitDetailed(static_cast<Character*>(player), skill_code, NULL, 0, ability_info);
    AbilityStatus* const ability_status = \
        player->GetStatusManager()->AllocAbilityStatus(player,ability);
    if (ability_status == NULL)
    {
        return;
    }

    // 플레이어가 죽어도 사라지지 않는다.
    SkillOption& skill_option = const_cast<SkillOption&>(ability_status->GetSkillOption());
    skill_option.AddOption(SkillOption::eOption_HoldupBuffPostDead);
    ability_status->SendStatusAddBRD();

    ability_factory->FreeAbility(ability);

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | BattleGround Penalty | Guid = %d , CharName = %s , fugitive buff time = %d |"), 
        instance_dugeon_->GetKey(),
        player->GetUserGuid(),
        player->GetCharName(),
        battle_ground_Info_->exit_period);
}

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

bool BattleGround::IsCanVoteByVictoryPoint()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i] && battle_ground_team_[i]->GetVictoryPoint() >= 1000) {
            return true;
        }
    }
    return false;
}

RC::eBATTLE_GROUND_RESULT BattleGround::OnInitiateVote(Player* initiate_player, Player* van_payer)
{
    if (initiate_player->GetBattleGroundPlayerInfo().GetTeamKey() != 
        van_payer->GetBattleGroundPlayerInfo().GetTeamKey())
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SAME_TEAM;
    }

    // victory point check..
    if (IsCanVoteByVictoryPoint() == false)
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_VICTORYPOINT;
    }

    BattleGroundTeam* initiate_vote_team = \
        _FindTeam(initiate_player->GetBattleGroundPlayerInfo().GetTeamKey());
    if (initiate_vote_team == NULL)
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
    }

    if (initiate_vote_team->GetIsInitiateVote() == true)
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
    }

    if (initiate_vote_team->GetMemberCount() <= 4)
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT;
    }

    if (initiate_vote_team->IsInitialteTime() == false)
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_LIMIT_TIME;
    }

    if (initiate_vote_team->OnInitiateVote(initiate_player, van_payer) == false)
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
    }

    return RC::RC_BATTLE_GROUND_INITIATE_VOTE_SUCCESS;
}

void BattleGround::OnVote(Player* player, bool vote_result)
{
    BattleGroundTeam* vote_team = \
        _FindTeam(player->GetBattleGroundPlayerInfo().GetTeamKey());
    if (vote_team == NULL) {
        return;
    }
    vote_team->OnVote(player->GetCharGuid(), vote_result);
}
void BattleGround::SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size)
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (battle_ground_team_[i])
        {
            battle_ground_team_[i]->SendPacketToAll(msg, msg_size);
        }
    }
}
#endif //_ NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM


//--------------------------------------------------------------------------------------------------
void BattleGround::_StartWait()
{
    _LoadTrigger();      

    _SentToAllTeamInfo();

    MSG_CG_BATTLE_GROUND_TIME_INFO_CMD cmd_msg;
    ;{
        cmd_msg.map_code = instance_dugeon_->GetMapCode();
        _SendPacketToAll(&cmd_msg, sizeof(cmd_msg));
    };

    // 45초 대기시간 카운트를 시작한다. (전정 시작 준비시간..)
    start_wait_timer_.SetTimer(battle_ground_Info_->prepare_time * kSecond); // script..

    battle_ground_state_ = kPrepareWait;
}

void BattleGround::_Start()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i])
        {
            battle_ground_team_[i]->Start();
        }
    }

    const FIELDCODE field_code = static_cast<FIELDCODE>(instance_dugeon_->GetMapCode());
    GameField* const game_field = instance_dugeon_->FindGameField(field_code);
    if (game_field == NULL)
        return;

    if (game_field->IsReady() == false) 
        return;

    TRIGGER_SERVER_EVENT trigger_msg;
    trigger_msg.event_id = battle_ground_Info_->gate_trigger_id;
    game_field->GetTriggerManager()->OnMsg(&trigger_msg);
    
    //부활 타임 설정..30초
    resurrection_timer_.SetTimer(battle_ground_Info_->resurrection_period * kSecond); // script..

    //승점 타임 설정.. 10초
    victory_point_timer_.SetTimer(battle_ground_Info_->gather_period* kSecond); // script..

    battle_ground_state_ = kMatch;

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | Start | 전장 시작!"), 
        instance_dugeon_->GetKey());
}

bool BattleGround::_IsAllPlayerEntered()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i] == NULL)
        {
            return false;
        }

        if (battle_ground_team_[i]->IsAllPlayerEntered() == false)
        {
            return false;
        }
    }

    return true;
}

void BattleGround::_SentToAllTeamInfo()
{
    MSG_CG_BATTLE_GROUND_TEAM_INFO_BRD brd_msg;
    int member_count = 0; 
    PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
    for(; const_itr != player_hash_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;
        
        brd_msg.member_list[member_count].team_key = \
            player->GetBattleGroundPlayerInfo().GetTeamKey();

        memset(brd_msg.member_list[member_count].character_name, 0, MAX_CHARNAME_LENGTH);														
        _tcsncpy(brd_msg.member_list[member_count].character_name, 
            player->GetCharName(), MAX_CHARNAME_LENGTH);
        brd_msg.member_list[member_count].character_name[MAX_GUILDNAME_LENGTH] = '\0';

        ++member_count;
    }
    brd_msg.member_count = member_count;
    _SendPacketToAll(&brd_msg, sizeof(brd_msg));
}

void BattleGround::_LoadTrigger()
{
    const BattleGroundFlagInfoParser* const flag_info_parser = \
                                                 BattleGroundFlagInfoParser::Instance();
    const BATTLE_GROUND_FLAG_INFO_MAP& flag_info_hash = \
                                                 flag_info_parser->GetBattleGroundFlagInfoHashMap();

    BATTLE_GROUND_FLAG_INFO_MAP::const_iterator const_itr = flag_info_hash.begin();
    for (; const_itr != flag_info_hash.end(); ++const_itr)
    {
        MAPCODE map_code = const_itr->first;
        if (map_code != instance_dugeon_->GetEntryFieldCode())
        {
            continue;
        }
        const BattleGroundFlagInfo* const flag_info = const_itr->second;
        if (flag_info == NULL)
        {
            continue;
        }
        _CreateStrongPoint(flag_info->collection_id);
    }
}

void BattleGround::_UpdateStrongpoint()
{
    STRONGPOINT_TRIGGER_HASH::const_iterator const_itr = strongpoint_trigger_hash_.begin();
    for (; const_itr != strongpoint_trigger_hash_.end(); ++const_itr)
    {
        StrongpointTrigger* const strongpoint = const_itr->second;
        switch(strongpoint->trigger_state)
        {
        case kStrongpointStateWait:
        case kStrongpointStateWaitAttack:
            {
                if (strongpoint->conquer_wait_timer.IsExpired(false) == false)
                {
                    continue;
                }
                if (strongpoint->is_sealing == false)
                {
                    continue;
                }

                if (strongpoint->sealing_player_key != 0 || strongpoint->is_start_sealing == true)
                {
                    Player* const cancel_player = _FindPlayer(strongpoint->sealing_player_key);
                    if (cancel_player && cancel_player->IsDeleted() == false)
                    {
                        GameField* field = cancel_player->GetField();
                        if (field == NULL) 
                        {
                            continue;
                        }

                        CollectionManager* collection_manager = field->GetCollectManeger();
                        if (collection_manager == NULL)
                        {
                            continue;
                        }
                        collection_manager->Cancel(cancel_player);
                    }
                }
                
                BattleGroundTeam* const team = _FindTeam(strongpoint->conquer_wait_team_key);
                if (team == NULL)
                {
                    continue;
                }
                team->IncreaseStrongpointCount();

                // 점령 성공
                strongpoint->is_conquer = true;
                strongpoint->conquer_team_key = strongpoint->conquer_wait_team_key;
                strongpoint->trigger_state = kStrongpointStateConquer;

                MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD brd_msg;
                {
                    brd_msg.strongpoint_info.strongpoint_id = strongpoint->strongpoint_id;
                    brd_msg.strongpoint_info.team_key = strongpoint->conquer_wait_team_key;
                    brd_msg.strongpoint_info.strongpoint_state = strongpoint->trigger_state;
                    _SendPacketToAll(&brd_msg,sizeof(brd_msg));
                }

                _SendVictoryPointToAll();

                // 전투결과창에 개인 정보 기록
                Player* const player = _FindPlayer(strongpoint->sealing_success_player_key);
                if (player != NULL && player->IsDeleted() == false)
                {
                    player->GetBattleGroundPlayerInfo().IncreaseStrongpointAttack();
                    battle_ground_battle_result_->DoRecord(static_cast<Character*>(player),
                        BATTLE_RECORD_STRONGPOINT_ATTACK, 1);

                    // 거점 공격성공 개인 로그기록
                    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_STRONGPOINT_ATTACK, player);

                    SUNBATTLEGROUNDLOG(
                        eCRITICAL_LOG, 
                        TEXT("| [ZONE_KEY(%d)] | Success Stronpoint Attack | Guid = %d , CharName = %s , Stronpoint ID = %d |)"), 
                        instance_dugeon_->GetKey(),
                        player->GetUserGuid(),
                        player->GetCharName(),
                        strongpoint->strongpoint_id);
                }

                // 전장 점수 로그 기록
                SUNBATTLEGROUNDLOG(
                    eCRITICAL_LOG, 
                    TEXT("| [ZONE_KEY(%d)] | Current StronPoint State | [Team = %d, StrogPointCount = %d, VictoryPoint = %d] vs [Team = %d, StrogPointCount = %d, VictoryPoint = %d] |)"), 
                    instance_dugeon_->GetKey(),
                    battle_ground_team_[0]->GetTeamKey(),
                    battle_ground_team_[0]->GetStrongpointCount(),
                    battle_ground_team_[0]->GetVictoryPoint(),
                    battle_ground_team_[1]->GetTeamKey(),
                    battle_ground_team_[1]->GetStrongpointCount(),
                    battle_ground_team_[1]->GetVictoryPoint());

                strongpoint->is_sealing = false;
                strongpoint->sealing_success_player_key = 0;
                strongpoint->conquer_wait_team_key = kChaosZoneTeamNone;
            }
            break;
        }
    }
}

void BattleGround::_IncreaseVictoryPoint()
{
    // 기본 승점
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i])
        {
            battle_ground_team_[i]->IncreaseVictroyPoint(battle_ground_Info_->gather_point);
        }
    }

    // 거점 점령 승점.. 추가..
    STRONGPOINT_TRIGGER_HASH::const_iterator const_itr = strongpoint_trigger_hash_.begin();
    for (; const_itr != strongpoint_trigger_hash_.end(); ++const_itr)
    {
        StrongpointTrigger* const strongpoint = const_itr->second;
        if (strongpoint->is_conquer && 
            (strongpoint->trigger_state == kStrongpointStateConquer || 
            strongpoint->trigger_state == kStrongpointStateDefence)
            )
        {
            BattleGroundTeam* const team = _FindTeam(strongpoint->conquer_team_key);
            if (team == NULL)
            {
                continue;
            }
            team->IncreaseVictroyPoint(battle_ground_Info_->gather_point);
        }
    }

    _SendVictoryPointToAll();
}

void BattleGround::_Result()
{
    // 죽어있으면 살리고 각자 안전지대로 이동시킨다.
    PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
    for(; const_itr != player_hash_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;

        BattleGroundTeam* const team = _FindTeam(player->GetBattleGroundPlayerInfo().GetTeamKey());
        if (team == NULL)
        {
            continue;
        }
        team->OnMoveToSafezone(player);

        if (player->IsDead())
        {
            player->OnResurrection( 0, 1.f, 1.f );
        }
    }

    if (battle_ground_team_[kRed-1] == NULL || battle_ground_team_[kBlue-1] == NULL)
    {
        return;
    }

    // 보상..
    const WORD blue_team_victory_point = \
        battle_ground_team_[kRed-1]->GetVictoryPoint();
    const WORD red_team_victory_point = \
        battle_ground_team_[kBlue-1]->GetVictoryPoint();

    if (blue_team_victory_point == red_team_victory_point)
    {
        battle_ground_team_[kRed-1]->OnResult(kResultDraw);
        battle_ground_team_[kBlue-1]->OnResult(kResultDraw);
    }
    else if (blue_team_victory_point > red_team_victory_point)
    {
        battle_ground_team_[kRed-1]->OnResult(kResultWin);
        battle_ground_team_[kBlue-1]->OnResult(kResultLose);
    }
    else if (blue_team_victory_point < red_team_victory_point)
    {
        battle_ground_team_[kRed-1]->OnResult(kResultLose);
        battle_ground_team_[kBlue-1]->OnResult(kResultWin);
    }
    else
    {
        //error..
    }

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    if (battle_ground_team_[kObserve-1])
        battle_ground_team_[kObserve-1]->OnResult(kResultWin);
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

    SUNBATTLEGROUNDLOG(
        eCRITICAL_LOG, 
        TEXT("| [ZONE_KEY(%d)] | Final Score | [Team = %d, StrogPointCount = %d, VictoryPoint = %d] vs [Team = %d, StrogPointCount = %d, VictoryPoint = %d] |)"), 
        instance_dugeon_->GetKey(),
        battle_ground_team_[0]->GetTeamKey(),
        battle_ground_team_[0]->GetStrongpointCount(),
        battle_ground_team_[0]->GetVictoryPoint(),
        battle_ground_team_[1]->GetTeamKey(),
        battle_ground_team_[1]->GetStrongpointCount(),
        battle_ground_team_[1]->GetVictoryPoint());
}

BattleGroundTeam* BattleGround::_CreateTeam(const ChaosZoneTeam team_key)
{
    if (team_key == kChaosZoneTeamNone)
    {
        return NULL;
    }

    if (battle_ground_team_[team_key -1] == NULL)
    {
        battle_ground_team_[team_key -1] = new BattleGroundTeam(this);
        if (battle_ground_team_[team_key -1] == NULL)
        {
            return NULL;
        }
        battle_ground_team_[team_key -1]->SetBattleGroundInfo(battle_ground_Info_);
        battle_ground_team_[team_key -1]->SetTeamKey(team_key);
        return battle_ground_team_[team_key -1];
    }
    return battle_ground_team_[team_key -1];
}

void BattleGround::_DestroyTeamAll()
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        SAFE_DELETE(battle_ground_team_[i]);
    }
}

BattleGroundTeam* BattleGround::_FindTeam(const ChaosZoneTeam team_key)
{
    if (team_key == kChaosZoneTeamNone)
    {
        return NULL;
    }

    if (battle_ground_team_[team_key -1] == NULL)
    {
        return NULL;
    }
    return battle_ground_team_[team_key -1];
}

BattleGroundTeam* BattleGround::_FindReadyTeam(Player* const player)
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (battle_ground_team_[i] && 
            battle_ground_team_[i]->IsReadyPlayer(player->GetUserKey()) == true)
        {
            return battle_ground_team_[i];
        }
    }
    return NULL;
}

Player* BattleGround::_FindPlayer(const DWORD char_guid)
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i] == NULL)
        {
            continue;
        }

        Player* player = battle_ground_team_[i]->FindMember(char_guid);
        if (player)
        {
            return player;
        }
    }
    return NULL;
}

void BattleGround::_AllDeathMemberResurrectionAtSafezone()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i])
        {
            battle_ground_team_[i]->AllDeathMemberResurrectionAtSafezone();
        }
    }
}

RC::eSTATUS_RESULT BattleGround::_ResurrectionAtSafezone(Player* const player)
{
    BattleGroundTeam* const team = _FindTeam(player->GetBattleGroundPlayerInfo().GetTeamKey());
    if(team == NULL)
    {
        return RC::RC_STATUS_TEAM_IS_NULL;
    }

    return team->OnMoveToSafezone(player);
}

bool BattleGround::_IsBattleGroundEnd()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (battle_ground_team_[i] == NULL)
        {
            return false;
        }

        if (battle_ground_team_[i]->IsExpired())
        {
            return true;
        }
    }
    return false;
}

StrongpointTrigger* BattleGround::_CreateStrongPoint(WzID strongpoint_id)
{
    StrongpointTrigger* strongpoint = _FindStrongPoint(strongpoint_id);
    if (strongpoint == NULL)
    {
        strongpoint = new StrongpointTrigger();
        strongpoint->strongpoint_id = strongpoint_id;
        strongpoint_trigger_hash_.insert(STRONGPOINT_TRIGGER_HASH::value_type(strongpoint_id,strongpoint));
    }
    return strongpoint;
}

void BattleGround::_DestroyStrongPointAll()
{
    STRONGPOINT_TRIGGER_HASH::iterator itr = strongpoint_trigger_hash_.begin();
    for (; itr != strongpoint_trigger_hash_.end(); ++itr)
    {
        StrongpointTrigger* strongpoint = itr->second;
        SAFE_DELETE(strongpoint);
    }
    strongpoint_trigger_hash_.clear();
}

StrongpointTrigger* BattleGround::_FindStrongPoint(WzID strongpoint_id) const
{
    STRONGPOINT_TRIGGER_HASH::const_iterator const_itr = strongpoint_trigger_hash_.find(strongpoint_id);
    if (const_itr == strongpoint_trigger_hash_.end())
    {
        return NULL;
    }
    return const_itr->second;
}

void BattleGround::_SendVictoryPointToAll()
{
    MSG_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD brd_msg;
    for (int i = 0; i < kBlue; ++i)
    {
        if(battle_ground_team_[i] == NULL)
            continue;

        brd_msg.team_score_info[i].team_key = battle_ground_team_[i]->GetTeamKey();
        brd_msg.team_score_info[i].strongpoint_count = battle_ground_team_[i]->GetStrongpointCount();
        brd_msg.team_score_info[i].victory_point = battle_ground_team_[i]->GetVictoryPoint();
    }
    _SendPacketToAll(&brd_msg, sizeof(brd_msg));
}

void BattleGround::_SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size)
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (battle_ground_team_[i])
        {
            battle_ground_team_[i]->SendPacketToAll(msg, msg_size);
        }
    }
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND