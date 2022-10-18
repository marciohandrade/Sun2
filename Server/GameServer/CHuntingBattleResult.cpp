#include "StdAfx.h"
#include ".\CHuntingBattleResult.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "CompetitionInfo.h"
#include "CHuntingManager.h"


CHuntingBattleResult::CHuntingBattleResult(void) :
competition_manager_(NULL)
{
}

CHuntingBattleResult::~CHuntingBattleResult(void)
{
}

// 유저의 출입및 업데이트 정보를 보냄
bool CHuntingBattleResult::SendBattleResult(Player* player, int team_flag)
{
    static MSG_CG_ZONE_PLAYER_BATTLE_RESULT_ACK result_ack;

    GameBattleResult::BattlePlayer* battle_player = FindBattlePlayer(player->GetCharGuid());
    if (battle_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "Character GUID[%d] is Not Exist", player->GetCharGuid());
        return false;
    }

    //if (battle_player->player != player)
    if (battle_player->char_guid != player->GetCharGuid())
    {
        SUNLOG(eCRITICAL_LOG, "Character GUID[%d] Player Pointer Miss", player->GetCharGuid());
        return false;
    }

    uint user_count = 0;
    CHARGUID guid = 0;
    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        if (user_count >= ZONE_UPDATE_BATTLE_PLAYER_INFO::MAX_PLAYER)
            break;

        guid = iter->first;

        exist_player = iter->second;

        Player* exist_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);

        if (exist_find_player)
        {
            // 변경된 전투 정보 설정시 캐릭터 레벨은 여기에서 따로 체크한다.
            LEVELTYPE level = exist_find_player->GetLevel();
            DoRecord(exist_find_player, BATTLE_RECORD_LEVEL, level);
        }

        if (update_selector_.IsUpdated(battle_player->update_key, exist_player->update_key))
            continue;


        const BattleRecorder::Record& record = exist_player->battle_recorder.get_record();

        GAMEROOM_BATTLE_UPDATE_INFO& battle_info = result_ack.m_ZonePlayers.m_Info[user_count];
        battle_info.CharGuid             = guid;
        battle_info.wLevel               = record.level;
        battle_info.dwMaxDamageToEnemy    = record.max_damage_to_enemy;
        battle_info.dwTotalDamageByEnemy = record.total_damage_by_enemy;
        battle_info.dwTotalDamageToEnemy = record.total_damage_to_enemy;
        battle_info.dwTotalHealToParty   = record.total_heal_to_party;
        battle_info.wDeadCount           = record.dead_count;
        battle_info.wKillCount = enable_pvp_ ? record.player_kill_count : record.monster_kill_count;

        ++user_count;
    }

    update_selector_.DoUpdate(battle_player->update_key);


    MSG_CG_ZONE_CHUNTING_BATTLE_INFO_CMD info_ack;
    info_ack.m_HomeTeamWinCount = 0;
    info_ack.m_AwayTeamWinCount = 0;

    if (competition_manager_ != NULL)
    {
        competition_manager_->GetTeamWinCount(&(info_ack.m_HomeTeamWinCount),
            &(info_ack.m_AwayTeamWinCount),
            battle_player->team);
    }

    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player)
    {
        battle_find_player->SendPacket(&info_ack, sizeof(info_ack));


        // 결과값은 무조건 보내도록 한다.
        result_ack.m_ZonePlayers.m_PlayerCount = user_count;
        battle_find_player->SendPacket(&result_ack, result_ack.GetSize());
    }

    return true;
}


void CHuntingBattleResult::update_team(GameBattleResult::BattlePlayer* battle_player)
{
    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player != NULL)
    {
        battle_player->team =battle_find_player->GetCompetitionInfo().GetTeamKey();
    }
}
