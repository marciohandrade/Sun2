#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRushBattleResult.h"
#include "World/ChaosZoneSystem/GoldRush.h"
#include "GameZone.h"

bool GoldRushBattleResult::SendBattleResult( Player* player, int team_flag /*= GameBattleResult::HOME_TEAM*/ )
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
    Player* goldrush_member = NULL;
    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        if (user_count >= ZONE_UPDATE_BATTLE_PLAYER_INFO::MAX_PLAYER)
            break;

        guid = iter->first;
        exist_player = iter->second;
        goldrush_member = goldrush_->FindGoldRushMember(guid);

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
        battle_info.wKillCount           = record.player_kill_count;
        battle_info.assist_count         = record.assist_count;
        battle_info.gold_point           = record.gold_point;
        if (goldrush_member != NULL) {
            battle_info.series_kill = \
                goldrush_member->GetGoldRushPlayerInfo().GetMaxSeriesKill();
        }

        ++user_count;
    }

    update_selector_.DoUpdate(battle_player->update_key);

    // 결과값은 무조건 보내도록 한다.
    result_ack.m_ZonePlayers.m_PlayerCount = user_count;
    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player != NULL)
    {
        battle_find_player->SendPacket(&result_ack, result_ack.GetSize());
    }

    return true;
}

void GoldRushBattleResult::update_team( GameBattleResult::BattlePlayer* battle_player )
{
    Player* const battle_find_player = \
        PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player && goldrush_)
    {
        battle_player->team = battle_find_player->GetTeamKey();
    }
}

void GoldRushBattleResult::Leave( Player* player, bool abnormal_disconnect )
{
    CHARGUID guid = player->GetCharGuid();
    BattlePlayerHash::iterator iter = battle_player_hash_.find(guid);
    if (iter != battle_player_hash_.end())
    {
        BattlePlayer* battle_player = iter->second;

        // 접속종료자 골드포인트 초기화
        WORD gold_point = battle_player->battle_recorder.get_record().gold_point;
        battle_player->battle_recorder.DoRecord(BATTLE_RECORD_GOLDPOINT, static_cast<int>(gold_point) * -1);

        NotifyLeaveMessage(battle_player, abnormal_disconnect);
        //battle_player->player = NULL;
        battle_player->char_guid = 0;
    }
}

#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
bool GoldRushBattleResult::SendBattleResult_RegisterGameLog( Player* player, int team_flag /*= GameBattleResult::HOME_TEAM*/ )
{
    bool result = SendBattleResult(player, team_flag);

    if (result == true)
    {
        GameBattleResult::BattlePlayer* battle_player = FindBattlePlayer(player->GetCharGuid());
        // 골드러시 종료 시, BattleResult내용 일부를 DB로그로 남긴다
        TCHAR log_string[MAX_ADDINFO_SIZE];
        memset(log_string, 0, sizeof(log_string));
        GameZone* game_zone = player->GetGameZonePtr();
        ZONEKEY room_key = 0;
        if (game_zone != NULL) {
            room_key = player->GetGameZonePtr()->GetKey();
        }

        const BattleRecorder::Record& player_record = battle_player->battle_recorder.get_record();
        _sntprintf(log_string, MAX_ADDINFO_SIZE, "[Kill:%d/Death:%d/Assist:%d/Gold:%d/TotalDmg:%d], Room:%d",
            player_record.player_kill_count, player_record.dead_count, player_record.assist_count,
            player_record.gold_point, player_record.total_damage_to_enemy,room_key );
        GAMELOG->LogGoldRush(ACT_GOLDRUSH_BATTLERESULT, player, 0, 0, 0, log_string);
    }

    return result;
}
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH