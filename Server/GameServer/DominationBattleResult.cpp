#include "StdAfx.h"
#include ".\DominationBattleResult.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT


DominationBattleResult::DominationBattleResult(void)
{
}

DominationBattleResult::~DominationBattleResult(void)
{
}

bool DominationBattleResult::Enter(Player* player)
{
    CHARGUID guid = player->GetCharGuid();
    BattlePlayer* battle_player = FindBattlePlayer(guid);

    if (battle_player)
    {
        // 썬랭킹에서 Leave시 삭제되지 않도록 수정되었으므로 아래 로그를 제거한다
        // SUNLOG(eCRITICAL_LOG, "GameBattleResult::Enter() Character GUID is Exist[%d]", guid);
        return false;
    }

    // 최초 진입시
    battle_player = (BattlePlayer*)battle_player_pool_.Alloc();
    ASSERT(battle_player);
    if (battle_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "GameBattleResult::Enter() Memory Alloc Fail");
        return false;
    }

    //battle_player->player = player;
    battle_player->char_guid = player->GetCharGuid();
    battle_player->team = player->GetGuildGuid();
    battle_player_hash_.insert(BattlePlayerHash::value_type(guid, battle_player));

    LEVELTYPE level = player->GetLevel();// battle_player->player->GetLevel();
    battle_player->battle_recorder.ClearRecord();
    battle_player->battle_recorder.DoRecord(BATTLE_RECORD_LEVEL, level);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SunRankingManager::Instance()->CreateSunRankingPlayer(player->GetSunRankingPlayerInfo());
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    update_selector_.ResetUpdateKey(battle_player->update_key);

#ifndef _NA_008270_20150511_MODIFY_OUT_OF_RANGE
    NotifyEnterMessage(battle_player);
#endif // _NA_008270_20150511_MODIFY_OUT_OF_RANGE

    return true;
}

void DominationBattleResult::Leave(Player* player, bool abnormal_disconnect)
{
#ifndef _NA_008012_20150204_SUN_RANKING_SYSTEM
    CHARGUID guid = player->GetCharGuid();
    BattlePlayerHash::iterator iter = battle_player_hash_.find(guid);
    if (iter != battle_player_hash_.end())
    {
        BattlePlayer* battle_player = iter->second;
#ifndef _NA_008270_20150511_MODIFY_OUT_OF_RANGE
        NotifyLeaveMessage(battle_player, abnormal_disconnect);
#endif // _NA_008270_20150511_MODIFY_OUT_OF_RANGE
        battle_player_pool_.Free(battle_player);
        battle_player_hash_.erase(iter);
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
}

// 유저의 출입및 업데이트 정보를 보냄
bool DominationBattleResult::SendBattleResult(Player* player, int team_flag)
{
#ifdef _NA_008270_20150511_MODIFY_OUT_OF_RANGE
    return false;
#else
    static MSG_CG_ZONE_PLAYER_BATTLE_RESULT_ACK result_ack;

    GameBattleResult::BattlePlayer* battle_player = FindBattlePlayer(player->GetCharGuid());
    if (battle_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "Character GUID[%d] is Not Exist", player->GetCharGuid());
        return false;
    }

    if (battle_player->char_guid != player->GetCharGuid())
    {
        SUNLOG(eCRITICAL_LOG, "Character GUID[%d] Player Pointer Miss", player->GetCharGuid());
        return false;
    }

    uint user_count = 0;
    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        if (user_count >= ZONE_UPDATE_BATTLE_PLAYER_INFO::MAX_PLAYER)
            break;

        exist_player = iter->second;
        Player* exist_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);
        if (exist_find_player == NULL)
            continue;

//         if (team_flag == HOME_TEAM)
//         {
//             if (exist_player->team != battle_player->team)
//                 continue;
//         }
//         else
//         {
//             if (exist_player->team == battle_player->team)
//                 continue;
//         }


        // 변경된 전투 정보 설정시 캐릭터 레벨은 여기에서 따로 체크한다.
        LEVELTYPE level = exist_find_player->GetLevel();
        DoRecord(exist_find_player, BATTLE_RECORD_LEVEL, level);

        if (update_selector_.IsUpdated(battle_player->update_key, exist_player->update_key))
            continue;

        const BattleRecorder::Record& record = exist_player->battle_recorder.get_record();

        GAMEROOM_BATTLE_UPDATE_INFO& battle_info = result_ack.m_ZonePlayers.m_Info[user_count];
        battle_info.CharGuid             = exist_find_player->GetCharGuid();
        battle_info.wLevel               = record.level;
        battle_info.dwMaxDamageToEnemy   = record.max_damage_to_enemy;
        battle_info.dwTotalDamageByEnemy = record.total_damage_by_enemy;
        battle_info.dwTotalDamageToEnemy = record.total_damage_to_enemy;
        battle_info.dwTotalHealToParty   = record.total_heal_to_party;
        battle_info.wDeadCount           = record.dead_count;
        battle_info.wKillCount = enable_pvp_ ? record.player_kill_count : record.monster_kill_count;

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
#endif // _NA_008270_20150511_MODIFY_OUT_OF_RANGE
}

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

void DominationBattleResult::ProcessSunRanking()
{
    BattlePlayer* battle_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; )
    {
        battle_player = iter->second;
        if (battle_player != NULL)
        {
            SCORE pvp_score = battle_player->battle_recorder.get_record().player_kill_count;
            SunRankingManager::Instance()->ProcessPvp(battle_player->char_guid, pvp_score);
            battle_player->battle_recorder.ClearRecord();
            update_selector_.ResetUpdateKey(battle_player->update_key);
            ++iter;
        }
    }
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
