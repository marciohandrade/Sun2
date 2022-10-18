#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include <World/ChaosZoneSystem/BattleGroundBattleResult.h>
#include <World/ChaosZoneSystem/BattleGround.h>

void BattleGroundBattleResult::update_team(GameBattleResult::BattlePlayer* battle_player)
{
    Player* const battle_find_player = \
        PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player && battle_ground_)
    {
        battle_player->team = battle_find_player->GetBattleGroundPlayerInfo().GetTeamKey();
    }
}

bool BattleGroundBattleResult::SendBattleResult(Player* player, int team_flag)
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
    Player* battle_ground_member = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        if (user_count >= ZONE_UPDATE_BATTLE_PLAYER_INFO::MAX_PLAYER)
            break;

        guid = iter->first;
        exist_player = iter->second;
        battle_ground_member = battle_ground_->FindBattleGroundMember(guid);
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        if (exist_player->team == kObserve) {
            continue;
        }
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

        const BattleRecorder::Record& record = exist_player->battle_recorder.get_record();
        GAMEROOM_BATTLE_UPDATE_INFO& battle_info = result_ack.m_ZonePlayers.m_Info[user_count];
        battle_info.CharGuid             = guid;
        battle_info.wLevel               = record.level;
        battle_info.dwMaxDamageToEnemy   = record.max_damage_to_enemy;
        battle_info.dwTotalDamageByEnemy = record.total_damage_by_enemy;
        battle_info.dwTotalDamageToEnemy = record.total_damage_to_enemy;
        battle_info.dwTotalHealToParty   = record.total_heal_to_party;
        battle_info.wDeadCount           = record.dead_count;
        battle_info.wKillCount           = record.player_kill_count;
        battle_info.strongpoint_attack   = record.strongpoint_attack;
        battle_info.strongpoint_defense  = record.strongpoint_defense;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        battle_info.assist_count         = record.assist_count;
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        if (battle_ground_member)
        {
            battle_info.series_kill = \
            battle_ground_member->GetBattleGroundPlayerInfo().GetMaxSeriesKill();
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
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND