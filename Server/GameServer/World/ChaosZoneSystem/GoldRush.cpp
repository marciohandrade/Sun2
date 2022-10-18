#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRush.h"
#include "World/ChaosZoneSystem/GoldRushTeam.h"
#include <../ProgramCommon/BattleGroundInfoParser.h>
#include "World/ChaosZoneSystem/GoldRushBattleResult.h"
#include <../ProgramCommon/ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <../Server/Common/PacketStruct_AG_ChaosZoneSystem.h>
#include <../ProgramCommon/ChaosZoneSystem/GoldRushInfoParser.h>
#include <../ProgramCommon/ChaosZoneSystem/GoldRushMonsterPointParser.h>

#include <GameInstanceDungeon.h>
#include <TriggerManagerEx.h>
#include "StatusManager.h"
#include "SkillInfoParser.h"
#include "SkillManager.h"
#include "AbilityFactory.h"
#include "Ability.h"
#include "AbilityStatus.h"
#include "Monster.h"
#include "GMCmdManager.h"

void GoldRush::Init( GameInstanceDungeon* const instance_dungeon )
{
    instance_dugeon_ = instance_dungeon;

    for (int i = 0; i < kTeamMax; ++i)
    {
        goldrush_team_[i] = NULL;
    }

    //�������â ����
    goldrush_battle_result_ = new GoldRushBattleResult();
    goldrush_battle_result_->Init(true);
    goldrush_battle_result_->SetGoldRush(this);

    // ���� ��ũ��Ʈ �̸� �б�
    // battlegroundinfo
    MAPCODE map_code = instance_dugeon_->GetMapCode();
    const BattleGroundInfoParser* const battle_ground_Info_parser = BattleGroundInfoParser::Instance();
    if (battle_ground_Info_ == NULL)
    {
        const BattleGroundInfo* battle_ground_Info = battle_ground_Info_parser->FindData(map_code);
        if (battle_ground_Info)
        {
            battle_ground_Info_ = battle_ground_Info;
        }
    }
    // goldrushinfo
    const GoldRushInfoParser* const goldrush_info_parser = GoldRushInfoParser::Instance();
    if (goldrush_info_ == NULL)
    {
        const GoldRushInfo* goldrush_info = goldrush_info_parser->FindData(map_code);
        if (goldrush_info) {
            goldrush_info_ = goldrush_info;
        }
        else // if (goldrush_info == NULL) 
        {
            SUNLOG(eCRITICAL_LOG,  
                "[GoldRush::Init()] GoldRushInfo is NULL. MapCode[%d]. ",map_code);
        }
    }

    // goldrushmonsterpoint
    monsterpoint_parser_ = GoldRushMonsterPointParser::Instance();

    is_count_message_ = true;
    is_lastboss_dead_ = false;
}

void GoldRush::Update()
{
    switch(goldrush_state_)
    {
    case eGOLDRUSH_ENTERWAIT:
        {
            //��� ��� ����.
            if (_IsAllPlayerEntered() == true)
            {
                // �����غ���·� ��ȯ
                _StartWait();
                break;
            }

            // ���� �ð��� ������ ���� ����� ��� ��� ���� ���� ���..(��� ���� �߰��� ������ �������� �� �������� ���� ���)
            if (start_wait_timer_.IsExpiredManual(false) == false) break;

            // ���� �غ� ���·� ���� ��ȯ
            _StartWait();
        }break;
    case eGOLDRUSH_PREPAREWAIT:
        {
            if (is_count_message_ && start_wait_timer_.GetRemainedTime() <= 5 * kSecond)
            {
                const FIELDCODE field_code = static_cast<FIELDCODE>(instance_dugeon_->GetMapCode());
                GameField* const game_field = instance_dugeon_->FindGameField(field_code);
                if (game_field == NULL) {
                    return;
                }
                if (game_field->IsReady() == false) {
                    return;
                }

                TRIGGER_SERVER_EVENT trigger_msg;
                trigger_msg.event_id = 1;
                game_field->GetTriggerManager()->OnMsg(&trigger_msg);
                is_count_message_ = false;
            }

            // �غ�ð� �� ���� ����.
            if (start_wait_timer_.IsExpiredManual(false) == false) break;
            // ���� ����
            _Start();   
        }break;
    case eGOLDRUSH_MATCHED:
        {
            for (int i = 0; i < kBlue; ++i)
            {
                if (goldrush_team_[i])
                {
                    goldrush_team_[i]->Update();
                }
            }

            // ��Ȱ �����ٸ�..
            if (resurrection_timer_.IsExpired()) {
                _AllDeathMemberResurrectionAtSafezone();
            }

            // �������� �ǰų� �ð��� �� �Ǹ� ���ó���Ѵ�..
            if (_IsGoldRushEnd())
            {
                goldrush_state_ = eGOLDRUSH_RESULT;
            }
        }break;
    case eGOLDRUSH_RESULT:
        {
            // ���� ����
            // ����ó��..
            _Result();
            goldrush_state_ = eGOLDRUSH_END;
        }break;
    case eGOLDRUSH_END:
        {
            // ���� �����
        }break;
    }
}

void GoldRush::Release()
{
    _DestroyTeamAll();
    SAFE_RELEASENDELETE(goldrush_battle_result_);
}

bool GoldRush::EnterPlayer( Player* const player )
{
    if (player == NULL || player->IsDeleted())
        return false;
    
    GoldRushTeam* team = _FindReadyTeam(player);
    if (team == NULL || team->GetTeamKey() == kObserve)
    {
        //GM ������ ���
        GMCmdManager* const gm_manager = GMCmdManager::Instance();
        Player* gm_player = gm_manager->GetGMPlayer(player->GetUserGuid());
        if (gm_player == NULL) {
            return false;
        }

        team = _FindTeam(kObserve);
        if (team == NULL)
            team = _CreateTeam(kObserve);
        team->AddMember(player);

        // team_key setting..
        ChaosZoneTeam team_key = team->GetTeamKey();
        player->SetTeamKey(team_key);

        _SentToAllTeamInfo();
        return true;
    }
    
    team->AddMember(player);
    // team key setting
    ChaosZoneTeam team_key = team->GetTeamKey();
    player->SetTeamKey(team_key);

    // ���� Total List
    player_hash_.insert(PLAYER_HASH::value_type(player->GetCharGuid(), player));

    bool is_abnormal_leave = false;

    //������ ���� ������
    if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE) != NULL || //������ ������ ���� ���..
        goldrush_state_ == eGOLDRUSH_PREPAREWAIT || // �� �̵� ���� ������ ����Ǿ� �غ�ð��� ������ ���..
        goldrush_state_ == eGOLDRUSH_MATCHED) // �� �̵� ���� ������ ����Ǿ� ���� ���� �߿� ������ ���..
    {
        is_abnormal_leave = true;

        // �������� (������ ������ �ɸ� ����� ������ �ȵű� ������ ���¡�� �ش����� ����)
        if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE))
        {
            if (!player->GetStatusManager()->Remove(eCHAR_STATE_FUGITIVE))
            {
                MSG_CG_STATUS_REMOVE_BRD BrdMsg;
                BrdMsg.m_dwTargetKey = player->GetObjectKey();
                BrdMsg.m_dwStatusCode = eCHAR_STATE_FUGITIVE;
                player->SendPacketAround(&BrdMsg, BrdMsg.GetSize());
            }
            // dis count ����
            player->GetGoldRushPlayerInfo().DecreaseDisTimes();
        }

        player->GetGoldRushPlayerInfo().SetPlayerState(kPlayerStateMatch);

        MSG_CG_GOLDRUSH_TIME_INFO_CMD cmd_msg;
        ;{
            cmd_msg.elapsed_time = 1;
            player->SendPacket(&cmd_msg, sizeof(cmd_msg));
        };

        _SentToAllTeamInfo();

        MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN brd_msg;
        ;{
            if (goldrush_state_ == eGOLDRUSH_MATCHED)
            {
                if (is_lastboss_dead_ == true)
                {
                    brd_msg.remained_time = extra_timer_.GetRemainedTime();
                }
                else // if (is_lastboss_dead_ == false)
                {
                    brd_msg.remained_time = limit_timer_.GetRemainedTime();
                }
            }

            for(int i=0; i < kTeamMax-1; ++i)
            {
                brd_msg.team_info[i].team_key = goldrush_team_[i]->GetTeamKey();
                brd_msg.team_info[i].total_gold_point = goldrush_team_[i]->GetTotalTeamGoldPoint();
            }
            // ������������ �ʿ��� ���� ����
            player->SendPacket(&brd_msg,sizeof(brd_msg));
        };
    }
    else if (goldrush_state_ == eGOLDRUSH_MATCHED) // ���� �������� ���������������� ó��..
    {
        is_abnormal_leave = true;
    }


    TCHAR add_info[MAX_ADDINFO_SIZE] = { 0, };
    if (is_abnormal_leave)
        ::_sntprintf(add_info, MAX_ADDINFO_SIZE, _T("RETURN ABNORMAL EXIT!"));
    add_info[MAX_ADDINFO_SIZE-1] = _T('\0');

    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_ENTER, player, NULL, add_info);

    return true;
}

void GoldRush::LeavePlayer( Player* const player )
{
    if (player == NULL || player->IsDeleted())
        return;

    ChaosZoneTeam team_key = player->GetTeamKey();
    GoldRushTeam* const team = _FindTeam(team_key);
    if (team == NULL) {
        return ;
    }

    // ������ �������Ḧ ����� �־�д�.
    team->SetReadyPlayer(player);
    team->RemoveMember(player->GetCharGuid());

    // ���� Total List
    player_hash_.erase(player->GetCharGuid());

    ;{
        MSG_AG_GOLDRUSH_LEAVE_CMD packet;
        packet.zone_key = instance_dugeon_->GetKey();
        player->SendPacket(&packet, sizeof(packet));
    };

    //�г�Ƽ �ο� or ��ŷ�ý��ۿ� �����ͺ����� ------------------------------------------------------------
    // ���� ���ð� �� ���� �� ���� ���� �г�Ƽ ����
    if (goldrush_state_ == eGOLDRUSH_PREPAREWAIT ||
        goldrush_state_ == eGOLDRUSH_MATCHED) 
    {
            GoldRushPenalty(player);
    }
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    //���� ���� ��, ��ŷDB�� ������ ����(���������� ��츸)
    else
    {
        const BattleRecorder::Record* player_record = GetGoldRushBattleResult()->GetPlayerBattleResult(player);
        player->GetRankingSystemCtl()->BattleGroundLeaveRoom(player, player_record, team->GetIsWin());
    }
#endif // _NA_008012_20150130_RANKING_SYSTEM

    // Player�� �� Ű �ʱ�ȭ---------------------------------------
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* gm_player = gm_manager->GetGMPlayer(player->GetUserGuid());
    if (gm_player && gm_player->GetTeamKey() == kObserve)
    {
        player->SetTeamKey(kChaosZoneTeamNone);
        return;
    }
    // �÷��̾� ��Ű �ʱ�ȭ
    player->SetTeamKey(kChaosZoneTeamNone);

    //�������� �����ֱ�
    MSG_CG_CHARINFO_CHAOS_ZONE_RECORD_INFOS_CMD cmd_packet;
    const GoldRushRecordInfos record_infos = player->GetExtraInfoManager().GetGoldRushRecordInfos();
    memcpy(cmd_packet.chaos_zone_record_infos.goldrush_record, record_infos.goldrush_record,
            sizeof(cmd_packet.chaos_zone_record_infos.goldrush_record));

    player->SendPacket(&cmd_packet, sizeof(cmd_packet));

    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_LEAVE, player);
}

void GoldRush::EnterTeam( const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list )
{
    GoldRushTeam* const team = _CreateTeam(team_key);
    team->SetReadyPlayer(player_count, user_key_list);

    if (goldrush_state_ == eGOLDRUSH_NONE)
    {
        start_wait_timer_.SetTimer(battle_ground_Info_->entry_time * kSecond);
        goldrush_state_ = eGOLDRUSH_ENTERWAIT;
    }
}

void GoldRush::DeadCharacter( Character* const dead_character, Character* const kill_character )
{
    if (dead_character->IsEqualObjectKind(PLAYER_OBJECT) == true) {
        //1. �÷��̾ '�÷��̾' kill �� ���
        //2. ���Ͱ� '�÷��̾' kill�� ���
        DeadPlayer(static_cast<Player*>(dead_character), kill_character);
    }
    else if(dead_character->IsEqualObjectKind(MONSTER_OBJECT) == true) {
        // 1. �÷��̾ '���͸�' kill�� ���
        DeadMonster(static_cast<Monster*>(dead_character), kill_character);
    }
}

eUSER_RELATION_TYPE GoldRush::IsFriendPVP( Player* attacker,Player* target )
{
    if (goldrush_state_ != eGOLDRUSH_MATCHED)
    {
        return USER_RELATION_FRIEND;
    }

    if (target->GetGoldRushPlayerInfo().GetPlayerState() != kPlayerStateMatch ||
        attacker->GetGoldRushPlayerInfo().GetPlayerState() != kPlayerStateMatch)
    {
        return USER_RELATION_FRIEND;
    }

    if (attacker->GetTeamKey() == target->GetTeamKey())
    {
        return USER_RELATION_FRIEND;
    }

    return USER_RELATION_ENEMY;
}

Player* GoldRush::FindGoldRushMember( const CHARGUID CharGuid )
{
    PLAYER_HASH::iterator itr = player_hash_.find(CharGuid);
    if (itr == player_hash_.end())
    {
        return NULL;
    }
    return itr->second;
}

RC::eBATTLE_GROUND_RESULT GoldRush::OnInitiateVote( Player* initiate_player, Player* van_payer )
{
    if (initiate_player->GetTeamKey() != 
        van_payer->GetTeamKey())
    {
        return RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SAME_TEAM;
    }

    GoldRushTeam* initiate_vote_team = \
        _FindTeam(initiate_player->GetTeamKey());
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

GoldRushTeam* GoldRush::_FindTeam( const ChaosZoneTeam team_key )
{
    if (team_key == kChaosZoneTeamNone)
    {
        return NULL;
    }

    if (goldrush_team_[team_key -1] == NULL)
    {
        return NULL;
    }
    return goldrush_team_[team_key -1];
}

void GoldRush::OnVote( Player* player, bool vote_result )
{
    GoldRushTeam* vote_team = \
        _FindTeam(player->GetTeamKey());
    if (vote_team == NULL) {
        return;
    }
    vote_team->OnVote(player->GetCharGuid(), vote_result);
}

void GoldRush::SendPacketToAll( MSG_BASE_FORWARD* msg, WORD msg_size )
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (goldrush_team_[i])
        {
            goldrush_team_[i]->SendPacketToAll(msg, msg_size);
        }
    }
}

void GoldRush::GoldRushPenalty( Player* player )
{
    if (player == NULL || player->IsDeleted())
        return;

    // dis count ����
    player->GetGoldRushPlayerInfo().IncreaseDisTimes();
    
    // �г�Ƽ ����..(������ ����)
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

    // script �ð� ����..
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

    // �÷��̾ �׾ ������� �ʴ´�.
    SkillOption& skill_option = const_cast<SkillOption&>(ability_status->GetSkillOption());
    skill_option.AddOption(SkillOption::eOption_HoldupBuffPostDead);
    ability_status->SendStatusAddBRD();

    ability_factory->FreeAbility(ability);

    // �������Ʈ �г�Ƽ----------------------------------------------------
    //������ ������ ���, ���� ������ ���� �������Ʈ�� �ٸ� �����鿡�� �й��Ѵ�.
    ChaosZoneTeam team_key = player->GetTeamKey();
    GoldRushTeam* const team = _FindTeam(team_key);

    WORD gold_point = player->GetGoldRushPlayerInfo().GetGoldPoint();

    // �ʱ�ȭ
    // BattleResult������ ���� Leave�Լ����� ���� �ʱ�ȭ���ش�
    player->GetGoldRushPlayerInfo().SetGoldPoint(0);

    //�й�
    if (team != NULL) {
        team->DistributeGoldPoint(gold_point);
    }

    ;{
        MSG_CG_GOLDRUSH_TEAM_SCORE_INFO_BRD brd_packet;
        brd_packet.get_goldpoint = gold_point;
        if (team != NULL) {
            team->GetTeamMemberInfo(brd_packet.team);
        }
        else // if (team == NULL)
        {
            brd_packet.team.team_member_count = 0;
            brd_packet.team.team_key = kChaosZoneTeamNone;
            brd_packet.team.team_gold_point = 0;
        }

        //���� �÷��̾��� ������ ���Խ�Ų��(���Ÿ���)
        BYTE member_count = brd_packet.team.team_member_count++;
        brd_packet.team.team_player_info[member_count].gold_grade = player->GetGoldRushPlayerInfo().GetGoldGrade();
        _tcsncpy(brd_packet.team.team_player_info[member_count].character_name, player->GetCharName(), MAX_CHARNAME_LENGTH);

        _SendPacketToAll(&brd_packet, sizeof(brd_packet));
    };

#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    GAMELOG->LogGoldRush(ACT_GOLDRUSH_GOLDPOINT_LOSE, player, gold_point, 0);
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
}

void GoldRush::_DestroyTeamAll()
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        SAFE_DELETE(goldrush_team_[i]);
    }
}

void GoldRush::_SentToAllTeamInfo()
{
    MSG_CG_GOLDRUSH_TEAM_INFO_BRD brd_msg;
    int member_count = 0; 
    PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
    for(; const_itr != player_hash_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;
        // team key
        brd_msg.member_list[member_count].team_key = \
            player->GetTeamKey();

        // character name
        memset(brd_msg.member_list[member_count].character_name, 0, MAX_CHARNAME_LENGTH);														
        _tcsncpy(brd_msg.member_list[member_count].character_name, 
            player->GetCharName(), MAX_CHARNAME_LENGTH);
        brd_msg.member_list[member_count].character_name[MAX_GUILDNAME_LENGTH] = '\0';

        // gold_point
        brd_msg.member_list[member_count].gold_grade = player->GetGoldRushPlayerInfo().GetGoldGrade();
        ++member_count;
    }
    brd_msg.member_count = member_count;
    _SendPacketToAll(&brd_msg, sizeof(brd_msg));
}

void GoldRush::_SendPacketToAll( MSG_BASE_FORWARD* msg, WORD msg_size )
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (goldrush_team_[i])
        {
            goldrush_team_[i]->SendPacketToAll(msg, msg_size);
        }
    }
}

void GoldRush::_StartWait()
{
    //��� ������� �⺻ �������Ʈ�� ���޹޴´�
    if (goldrush_info_ == NULL)
    {
        // �� �κ����� ������ ������ ������ �� ����.
        SUNLOG(eCRITICAL_LOG,  
            "[GoldRush::Init()] GoldRushInfo is NULL.");
        return;
    }
    WORD start_goldpoint = goldrush_info_->start_point;
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (goldrush_team_[i] != NULL) {
            goldrush_team_[i]->ObtainGoldPoint(start_goldpoint);
        }
    }

    _SentToAllTeamInfo();
    _SendTotalTeamPoint();

    MSG_CG_GOLDRUSH_TIME_INFO_CMD cmd_msg;
    ;{
        cmd_msg.map_code = instance_dugeon_->GetMapCode();
        _SendPacketToAll(&cmd_msg, sizeof(cmd_msg));
    };

    // 45�� ���ð� ī��Ʈ�� �����Ѵ�. (���� ���� �غ�ð�..)
    start_wait_timer_.SetTimer(battle_ground_Info_->prepare_time * kSecond); // script..

    goldrush_state_ = eGOLDRUSH_PREPAREWAIT;
}

Player* GoldRush::_FindPlayer( const DWORD char_guid )
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (goldrush_team_[i] == NULL)
        {
            continue;
        }

        Player* player = goldrush_team_[i]->FindMember(char_guid);
        if (player)
        {
            return player;
        }
    }
    return NULL;
}

bool GoldRush::_IsAllPlayerEntered()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (goldrush_team_[i] == NULL)
        {
            return false;
        }

        if (goldrush_team_[i]->IsAllPlayerEntered() == false)
        {
            return false;
        }
    }

    return true;
}

void GoldRush::_Start()
{
    // team start
    for (int i = 0; i < kBlue; ++i)
    {
        if (goldrush_team_[i])
        {
            goldrush_team_[i]->Start();
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

    //��Ȱ Ÿ�� ����..30��
    resurrection_timer_.SetTimer(battle_ground_Info_->resurrection_period * kSecond); // script..
    //���� �÷��̽ð� ����
    limit_timer_.SetTimer(goldrush_info_->play_time * kSecond);

    goldrush_state_ = eGOLDRUSH_MATCHED;
}

void GoldRush::_AllDeathMemberResurrectionAtSafezone()
{
    for (int i = 0; i < kBlue; ++i)
    {
        if (goldrush_team_[i])
        {
            goldrush_team_[i]->AllDeathMemberResurrectionAtSafezone();
        }
    }
}

bool GoldRush::_IsGoldRushEnd()
{
    if ( is_lastboss_dead_ == true )
    {
        if (extra_timer_.IsExpiredManual(false)) {
            return true;
        }
    }
    else
    {
        if (limit_timer_.IsExpiredManual(false)) {
            return true;
        }
    }
    
    return false;
}

void GoldRush::_Result()
{
    // �׾������� �츮�� ���� ��������� �̵���Ų��.
    PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
    for(; const_itr != player_hash_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;

        GoldRushTeam* const team = _FindTeam(player->GetTeamKey());
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

    if (goldrush_team_[kRed-1] == NULL || goldrush_team_[kBlue-1] == NULL)
    {
        return;
    }

    // ���� �̰�°� �Ǵ�
    // �̱� �� �¸� üũ
    const WORD blue_team_goldpoint = goldrush_team_[kBlue-1]->GetTotalTeamGoldPoint();
    const WORD red_team_goldpoint = goldrush_team_[kRed-1]->GetTotalTeamGoldPoint();

    if (red_team_goldpoint > blue_team_goldpoint)
    {
        //Red Team Win
        goldrush_team_[kBlue-1]->OnResult(kResultLose);
        goldrush_team_[kRed-1]->OnResult(kResultWin);
    }
    else if (red_team_goldpoint < blue_team_goldpoint)
    {
        //Blue Team Win
        goldrush_team_[kBlue-1]->OnResult(kResultWin);
        goldrush_team_[kRed-1]->OnResult(kResultLose);
    }
    else //(red_team_goldpoint == blue_team_goldpoint), �׿�
    {
        //Draw
        goldrush_team_[kBlue-1]->OnResult(kResultDraw);
        goldrush_team_[kRed-1]->OnResult(kResultDraw);
    }

    if (goldrush_team_[kObserve-1] != NULL) {
        goldrush_team_[kObserve-1]->OnResult(kResultWin);
    }
}

GoldRushTeam* GoldRush::_CreateTeam( const ChaosZoneTeam team_key )
{
    if (team_key == kChaosZoneTeamNone)
    {
        return NULL;
    }

    if (goldrush_team_[team_key -1] == NULL)
    {
        goldrush_team_[team_key -1] = new GoldRushTeam(this);
        if (goldrush_team_[team_key -1] == NULL)
        {
            return NULL;
        }
        goldrush_team_[team_key -1]->SetBattleGroundInfo(battle_ground_Info_);
        goldrush_team_[team_key -1]->SetTeamKey(team_key);
        return goldrush_team_[team_key -1];
    }
    return goldrush_team_[team_key -1];
}

GoldRushTeam* GoldRush::_FindReadyTeam( Player* const player )
{
    for (int i = 0; i < kTeamMax; ++i)
    {
        if (goldrush_team_[i] && 
            goldrush_team_[i]->IsReadyPlayer(player->GetUserKey()) == true)
        {
            return goldrush_team_[i];
        }
    }
    return NULL;
}

void GoldRush::DeadPlayer( Player* const dead_player, Character* const kill_character )
{
    if(dead_player == NULL || dead_player->IsDeleted())
        return;

    //���� �÷��̾� ��Ȱ
    GoldRushPlayerInfo& dead_player_info = dead_player->GetGoldRushPlayerInfo();
    dead_player_info.SetMoveToSafeZoneTimer(battle_ground_Info_->dead_time * kSecond);
    dead_player_info.SetPlayerState(kPlayerStateDeath);
    // ���� Ƚ�� ����
    dead_player_info.IncreaseDeath();
    // ���� �÷��̾� �� ���Ѵ�
    GoldRushTeam* dead_team = _FindTeam(static_cast<ChaosZoneTeam>(dead_player->GetTeamKey()));

    // killer�� ������ �����Ѵ�.
    if (kill_character == NULL || kill_character->IsDeleted())
    {
        // ���忡�� ���� �αױ�� (ų���� �������.. Ʈ���ſ� ���� ����..)
        GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DEATH, dead_player);
        return;
    }

    // Killer�� ���� ���
    WORD steal_gold_point = 0; // ������ �������Ʈ
    GoldRushTeam* killer_team = NULL;
    Player* kill_player = NULL;
    if (kill_character->IsEqualObjectKind(PLAYER_OBJECT) == true) 
    {
        // �÷��̾�� �׾��� ���� ó��
        kill_player = static_cast<Player*>(kill_character);

        kill_player->GetGoldRushPlayerInfo().IncreaseKill();
        // ��� ����Ʈ ȹ��
        killer_team = _FindTeam(static_cast<ChaosZoneTeam>(kill_player->GetTeamKey()));

        BYTE goldpoint_steal_ratio = goldrush_info_->steal_point_ratio;
        //���� �÷��̾�Լ� ����Ʈ�� ���´�
        steal_gold_point = dead_player_info.LoseGoldPointRatio(goldpoint_steal_ratio);
        goldrush_battle_result_->DoRecord(static_cast<Character*>(dead_player), 
                                  BATTLE_RECORD_GOLDPOINT, static_cast<int>(steal_gold_point) * -1);

        killer_team->DistributeGoldPoint(steal_gold_point, kill_player->GetCharGuid());

        //��ý�Ʈ Ƚ�� ������Ų��--------------------------------------------------------------
        Player* assist_player = NULL;
        assist_player = dead_player->GetPlayerAssistMeter()->GetBestAssistPlayer(kill_player->GetCharGuid());
        if(assist_player != NULL)
        {
            goldrush_battle_result_->DoRecord(assist_player, BATTLE_RECORD_ASSIST, 1);
            assist_player->GetGoldRushPlayerInfo().IncreaseAssistTimes();
        }
        dead_player->GetPlayerAssistMeter()->Clear();
    }
    else if (kill_character->IsEqualObjectKind(SUMMON_OBJECT) == true) {
        kill_player = _FindPlayer(kill_character->GetPlayerOwner()->GetCharGuid());
    }
    else if (kill_character->IsEqualObjectKind(MONSTER_OBJECT) == true) 
    {
        // ���Ϳ��� �׾��� ����� ó��
        BYTE goldpoint_lose_ratio = goldrush_info_->lose_point_ratio;
        //���� �÷��̾ ����Ʈ�� �Ҵ´�
        steal_gold_point = dead_player_info.LoseGoldPointRatio(goldpoint_lose_ratio);
        goldrush_battle_result_->DoRecord(static_cast<Character*>(dead_player), 
                                 BATTLE_RECORD_GOLDPOINT, static_cast<int>(steal_gold_point) * -1);
    }

    //�����Ŷ
    ;{
        MSG_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD brd_msg;

        if (kill_player != NULL && kill_player->IsDeleted() == false)
        {
            // ų���� �÷��̾��̸�
            brd_msg.is_killer_monster = false;
            // ����ų
            if (kill_player->GetGoldRushPlayerInfo().GetSeriesKill() > 1)
            {
                brd_msg.is_series_kill = true;
                brd_msg.series_kill_count = kill_player->GetGoldRushPlayerInfo().GetSeriesKill();
            }
        }
        else
        {
            // ų���� ����
            brd_msg.is_killer_monster = true;
            brd_msg.is_series_kill = false;
        }
        // ����ų ����
        const WORD dead_player_series_kill_count = dead_player->GetGoldRushPlayerInfo().GetSeriesKill();
        if (dead_player_series_kill_count > 1) {
            brd_msg.is_series_kill_block = true;
        }

        _tcsncpy(brd_msg.kill_player_name, kill_character->GetCharName(), MAX_CHARNAME_LENGTH);
        _tcsncpy(brd_msg.dead_player_name, dead_player->GetCharName(), MAX_CHARNAME_LENGTH);

        // ������(���ѱ�) �������Ʈ
        brd_msg.steal_gold = steal_gold_point;

        if (killer_team != NULL) {
            // ų���� ����
            killer_team->GetTeamMemberInfo(brd_msg.killer_team);
        }
        if (dead_team != NULL) {
            //���� �÷��̾� �� ����
            dead_team->GetTeamMemberInfo(brd_msg.dead_team);
        }

        _SendPacketToAll(&brd_msg, sizeof(brd_msg));
    };

    // ���忡�� ���� �αױ��

    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_DEATH, dead_player, kill_character->GetCharName());
    // ���忡�� ų �αױ��
    GAMELOG->LogChaosZone(ACT_BATTLE_GROUND_KILL, kill_player, dead_player->GetCharName());
}

void GoldRush::DeadMonster(Monster* const dead_monster, Character* const kill_character)
{
    //���� ���Ͱ� ���������̸� �����÷��� Ȱ��
    if (dead_monster->GetBaseInfo()->m_MonsterCode == goldrush_info_->last_boss_code &&
        is_lastboss_dead_ == false) 
    {
        is_lastboss_dead_ = true;
        // �÷���Ÿ�� ����
        limit_timer_.DisableCheckTime();
        // ����Ʈ��Ÿ�� Ȱ��ȭ
        extra_timer_.SetTimer(goldrush_info_->extra_time * kSecond);
    }

    // killer�� ������ �����Ѵ�.
    if (kill_character == NULL || kill_character->IsDeleted())
    {
        // ���忡�� ���� �αױ�� (ų���� �������.. Ʈ���ſ� ���� ����..)
        return;
    }

    // Killer�� ���� ���
    Player* kill_player = NULL;
    if (kill_character->IsEqualObjectKind(PLAYER_OBJECT) == true) {
        kill_player = static_cast<Player*>(kill_character);
    }
    else if (kill_character->IsEqualObjectKind(SUMMON_OBJECT) == true) {
        kill_player = _FindPlayer(kill_character->GetPlayerOwner()->GetCharGuid());
    }

    if (kill_player == NULL || kill_player->IsDeleted())
        return;

    ChaosZoneTeam killer_team_key = static_cast<ChaosZoneTeam>(kill_player->GetTeamKey());
    GoldRushTeam* killer_team = _FindTeam(killer_team_key);

    // ��� ����Ʈ ȹ��--
    WORD gold_point = 0;
    BYTE monster_grade = dead_monster->GetBaseInfo()->m_byGrade;
    MONSTERCODE monster_code = dead_monster->GetBaseInfo()->m_MonsterCode;
    const GoldRushMonsterPoint* monster_info = monsterpoint_parser_->FindData(monster_code);
    if (monster_info != NULL) {
        // GoldRushMonsterPoint�� ��ϵǾ��ִ� ���͸� �ش� ��尪�� �����´�
        gold_point = monster_info->monster_goldpoint;
    }
    else if (monster_grade >= eNPC_GENERAL && monster_grade <= eNPC_BOSS) {
        // ��ũ��Ʈ�� ��ϵǾ����� ���� ���͸� �⺻ ��޺� ��尪�� �����´�
        gold_point = goldrush_info_->monster_grade_point[monster_grade-1];
    }
    //���� �� ����鿡�� ���й�
    killer_team->DistributeGoldPoint(gold_point);
    //--

    ;{
        MSG_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD brd_msg;
        
        _tcsncpy(brd_msg.kill_player_name, kill_player->GetCharName(), MAX_CHARNAME_LENGTH);
        brd_msg.dead_monster_code = dead_monster->GetBaseInfo()->m_MonsterCode;

        if (monster_grade == eNPC_BOSS) {
            brd_msg.is_boss_monster = true;
        }
        else {
            brd_msg.is_boss_monster = false;
        }

        // ���� óġ�� ȹ���� ���
        brd_msg.get_goldpoint = gold_point;
        // �������Ʈ ����
        killer_team->GetTeamMemberInfo(brd_msg.killer_team);

        _SendPacketToAll(&brd_msg, sizeof(brd_msg));
    };

#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    GAMELOG->LogGoldRush(ACT_GOLDRUSH_MONSTERKILL, kill_player, 0,0, 
                         dead_monster->GetBaseInfo()->m_MonsterCode);
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
}

void GoldRush::_SendTotalTeamPoint()
{
    MSG_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD brd_packet;

    // �������� ����
    for( int i=0; i < kTeamMax-1; ++i)
    {
        brd_packet.team_info[i].team_key = goldrush_team_[i]->GetTeamKey();
        brd_packet.team_info[i].total_gold_point = goldrush_team_[i]->GetTotalTeamGoldPoint();

    }
    _SendPacketToAll(&brd_packet, sizeof(brd_packet));

}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH