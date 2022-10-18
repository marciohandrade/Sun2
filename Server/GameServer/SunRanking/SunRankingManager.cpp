#include "stdafx.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include <iostream>
#include "SunRanking.h"
#include "SunRankingManager.h"
#include "SunRankingPlayer.h"
#include "SunRankingParser.h"
#include "SunRankingGradeManager.h"
#include "GameBattleResult.h"
#include "Player.h"
#include "ServerOptionParserEx.h"

SunRankingManager::SunRankingManager()
{
    Initialize();
};

SunRankingManager::~SunRankingManager()
{
    Release();
};

void SunRankingManager::Initialize()
{
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        if (m_sun_rankings.find(type) != m_sun_rankings.end())
        {
            continue;
        }

        m_sun_rankings.insert(std::make_pair(type, new SunRanking()));
    }
}

void SunRankingManager::Release()
{
    { // Release SunRankings
        SunRankings::iterator cur_iter = m_sun_rankings.begin();
        SunRankings::iterator end_iter = m_sun_rankings.end();
        for ( ; cur_iter != end_iter; ++cur_iter)
        {
            if (cur_iter->second == NULL)
            {
                continue;
            }
            SAFE_DELETE(cur_iter->second);
        }
        m_sun_rankings.clear();
    }

    { // Release SunRankingPlayers
        SunRankingPlayers::iterator cur_iter = m_sun_ranking_players.begin();
        SunRankingPlayers::iterator end_iter = m_sun_ranking_players.end();
        for ( ; cur_iter != end_iter; ++cur_iter)
        {
            if (cur_iter->second == NULL)
            {
                continue;
            }
            destroy(cur_iter->second);
        }
        m_sun_ranking_players.clear();
    }
}

SunRanking* SunRankingManager::_GetSunRanking( eSUN_RANKING_TYPE rank_type )
{
    SunRankings::const_iterator cur_iter = m_sun_rankings.find(rank_type);
    SunRankings::const_iterator end_iter = m_sun_rankings.end();
    if (cur_iter == end_iter)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| Invalid rank type (%d)",
            __FUNCTION__, rank_type);
        return NULL;
    }

    return cur_iter->second;
}

void SunRankingManager::SetGuildGuid( CHARGUID char_guid, GUILDGUID guild_guid )
{
    if (SunRankingGradeManager::Instance()->GetState() != eSUN_RANKING_STATE_RUN)
    {
        return;
    }

    SunRankings::iterator cur_iter = m_sun_rankings.begin();
    SunRankings::iterator end_iter = m_sun_rankings.end();
    for ( ; cur_iter != end_iter; ++cur_iter)
    {
        SunRanking* sun_ranking = cur_iter->second;
        if (sun_ranking == NULL)
        {
            continue;
        }

        sun_ranking->SetGuildGuid(char_guid, guild_guid);
    }

    SunRankingPlayer* sun_ranking_player = GetSunRankingPlayer(char_guid);
    if (sun_ranking_player == NULL)
    {
        return;
    }
    sun_ranking_player->SetGuildGuid(guild_guid);

    MSG_DG_SUN_RANKING_UPDATE_CMD msg;
    sun_ranking_player->GetSunRankingPlayerInfo(msg.info);
    g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
}

void SunRankingManager::ProcessBattleZone( const BattleRecorder::Record& record, Player* player )
{
    if (SunRankingGradeManager::Instance()->GetState() != eSUN_RANKING_STATE_RUN)
    {
        return;
    }

    if (CreateSunRankingPlayer(player->GetSunRankingPlayerInfo()) == false)
    {
        return;
    }

    SunRankingPlayer* sun_ranking_player = GetSunRankingPlayer(player->GetCharGuid());
    if (sun_ranking_player == NULL)
    {
        return;
    }

    bool is_updated = false;
    is_updated = _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_STRONG_ONE_SHOT,
        record.max_damage_to_enemy) || is_updated;
    is_updated = _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_BEST_ATTACKER,
        record.total_damage_to_enemy) || is_updated;
    is_updated = _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_MASTER_OF_HEAL,
        record.total_heal_to_party) || is_updated;

    if (is_updated == true)
    {
        _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_BEST_OF_BEST, sun_ranking_player->GetBestOfBestScore());

        MSG_DG_SUN_RANKING_UPDATE_CMD msg;
        sun_ranking_player->GetSunRankingPlayerInfo(msg.info);
        g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
    }
}

void SunRankingManager::ProcessDisconnect( Player* player )
{
    if (SunRankingGradeManager::Instance()->GetState() != eSUN_RANKING_STATE_RUN)
    {
        return;
    }

    if (CreateSunRankingPlayer(player->GetSunRankingPlayerInfo()) == false)
    {
        return;
    }

    SunRankingPlayer* sun_ranking_player = GetSunRankingPlayer(player->GetCharGuid());
    if (sun_ranking_player == NULL)
    {
        return;
    }

    bool is_updated = false;
    is_updated = _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_LEVEL_KING,
        player->GetExp()) || is_updated;

    int total_equipment_score = 0;
    if (player->GetTotalEquipmentScore(total_equipment_score) == true)
    {
        is_updated = _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_LEGEND_EQUIP,
            total_equipment_score) || is_updated;
    }

    if (is_updated == true)
    {
        _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_BEST_OF_BEST, sun_ranking_player->GetBestOfBestScore());
        
        MSG_DG_SUN_RANKING_UPDATE_CMD msg;
        sun_ranking_player->GetSunRankingPlayerInfo(msg.info);
        g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
    }
}

void SunRankingManager::ProcessPvp( CHARGUID char_guid, SCORE score )
{
    if (SunRankingGradeManager::Instance()->GetState() != eSUN_RANKING_STATE_RUN)
    {
        return;
    }

    SunRankingPlayer* sun_ranking_player = GetSunRankingPlayer(char_guid);
    if (sun_ranking_player == NULL)
    {
        return;
    }

    SCORE* cur_pvp_count = sun_ranking_player->GetSunRankingScore(eSUN_RANKING_TYPE_ELETE_WARRIOR);
    if (cur_pvp_count == NULL)
    {
        return;
    }

    bool is_updated = false;
    is_updated = _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_ELETE_WARRIOR,
        *cur_pvp_count + score) || is_updated;

    if (is_updated == true)
    {
        _UpdateSunRanking(sun_ranking_player, eSUN_RANKING_TYPE_BEST_OF_BEST, sun_ranking_player->GetBestOfBestScore());

        MSG_DG_SUN_RANKING_UPDATE_CMD msg;
        sun_ranking_player->GetSunRankingPlayerInfo(msg.info);
        g_pGameServer->SendToServer(GAME_DBPROXY, &msg, sizeof(msg));
    }
}

bool SunRankingManager::CreateSunRankingPlayer( SunRankingPlayerInfo& info )
{
    SunRankingPlayers::iterator find_iter = m_sun_ranking_players.find(info.char_guid);
    if (find_iter != m_sun_ranking_players.end())
    {
        return true;
    }

    SunRankingPlayer* sun_ranking_player = construct();
    if (m_sun_ranking_players.insert(std::make_pair(info.char_guid, sun_ranking_player)).second == false)
    {
        destroy(sun_ranking_player);
        SUNLOG(eCRITICAL_LOG, "|[%s]| insert failed(%d)", __FUNCTION__, info.char_guid);
        return false;
    }

    sun_ranking_player->Initialize(info);

    return true;
}

bool SunRankingManager::_UpdateSunRanking( SunRankingPlayer* sun_ranking_player, eSUN_RANKING_TYPE ranking_type, SCORE score )
{
    if (sun_ranking_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| Player is null(rank type : %d)",
            __FUNCTION__, ranking_type);
        return false;
    }

    SCORE* current_score = sun_ranking_player->GetSunRankingScore(ranking_type);
    if (current_score == NULL)
    {
        return false;
    }

    if (*current_score == score)
    {
        return false;
    }

    if (eSUN_RANKING_TYPE_LEGEND_EQUIP != ranking_type 
        && eSUN_RANKING_TYPE_BEST_OF_BEST != ranking_type)
    {
        if (score <= 0 || *current_score >= score)
        {
            return false;
        }
    }

    SunRanking* sun_ranking = _GetSunRanking(ranking_type);
    if (sun_ranking == NULL)
    {
        return false;
    }

    if(sun_ranking->UpdateScore(sun_ranking_player->GetCharGuid(), sun_ranking_player->GetGuildGuid(), score) == true)
    {
        *current_score = score;
    }
    return true;
}

SunRankingPlayer* SunRankingManager::GetSunRankingPlayer( CHARGUID char_guid )
{
    SunRankingPlayers::iterator find_iter = m_sun_ranking_players.find(char_guid);
    if (find_iter == m_sun_ranking_players.end())
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| can't find sunrankingplayer(%d)",
            __FUNCTION__, char_guid);
        return NULL;
    }

    return find_iter->second;
}

void SunRankingManager::UpdateFromSunRankingPlayer( SunRankingPlayerInfo& info )
{
    if (CreateSunRankingPlayer(info) == false)
    {
        return;
    }

    SunRankingPlayer* sun_ranking_player = GetSunRankingPlayer(info.char_guid);
    if (sun_ranking_player == NULL)
    {
        return;
    }

    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        _UpdateSunRanking(sun_ranking_player, type, info.score[type]);
    }

    sun_ranking_player->SetGuildGuid(info.guild_guid);
}

RC::eSUN_RANKING_RESULT SunRankingManager::GetSunRankingInfos( MSG_CG_ETC_SUN_RANKING_ACK& msg, eSUN_RANKING_TYPE ranking_type, CHARGUID char_guid, int rank_count, bool is_guild_ranking, GUILDGUID guild_guid )
{
    if (SunRankingGradeManager::Instance()->GetState() != eSUN_RANKING_STATE_RUN)
    {
        return RC::RC_SUN_RANKING_SETTLE;
    }

    SunRanking* sun_ranking = _GetSunRanking(ranking_type);
    if (sun_ranking == NULL)
    {
        return RC::RC_SUN_RANKING_INVALID_RANKING_TYPE;
    }

    msg.my_score = sun_ranking->GetScore(char_guid);

    RANKINGINFOS infos;
    if (is_guild_ranking == true)
    {
        sun_ranking->GetGuildRanking(guild_guid, rank_count, infos);
        msg.my_ranking = sun_ranking->GetGuildRanking(msg.my_score, guild_guid);
    }
    else
    {
        sun_ranking->GetRanking(rank_count, infos);
        msg.my_ranking = sun_ranking->GetRanking(msg.my_score);
    }

    RANKINGINFOS::iterator cur_iter = infos.begin();
    RANKINGINFOS::iterator end_iter = infos.end();
    int count = 0;
    for (; cur_iter != end_iter; ++cur_iter, ++count)
    {
        if (MSG_CG_ETC_SUN_RANKING_ACK::MAX_INFO_COUNT <= count)
        {
            break;
        }
        const SunRankingInfo* const temp = *cur_iter;

        SunRankingInfoForClient& temp_info = msg.infos[count];

        SunRankingPlayer* ranking_player = GetSunRankingPlayer(temp->char_guid);
        if (ranking_player != NULL)
        {
            temp_info.class_code = ranking_player->GetCharClass();
            temp_info.char_name;
            strncpy(temp_info.char_name, ranking_player->GetCharName(), MAX_CHARNAME_LENGTH);
            temp_info.score = temp->score;
        }
    }

    return RC::RC_SUN_RANKING_SUCCESS;
}

void SunRankingManager::GetTopRankerEquipmentScore(SCORE& score, CHARGUID& char_guid)
{
    score = 0;
    char_guid = 0;
    SunRanking* sun_ranking_bob = _GetSunRanking(eSUN_RANKING_TYPE_BEST_OF_BEST);
    if (sun_ranking_bob == NULL)
    {
        return;
    }

    RANKINGINFOS infos;
    sun_ranking_bob->GetRanking(1, infos);
    if (infos.empty() == true)
    {
        return;
    }

    const SunRankingInfo* const temp = *infos.begin();
    if (infos.size() <= 0 || infos.begin() == infos.end())
    {
        return;
    }

    char_guid = temp->char_guid;

    SunRanking* sun_ranking_equip = _GetSunRanking(eSUN_RANKING_TYPE_LEGEND_EQUIP);
    if (sun_ranking_equip == NULL)
    {
        return;
    }

    score = sun_ranking_equip->GetScore(temp->char_guid);
}

RC::eSUN_RANKING_RESULT SunRankingManager::GetRanking( eSUN_RANKING_TYPE ranking_type, CHARGUID char_guid, int& rank )
{
    SunRanking* sun_ranking = _GetSunRanking(ranking_type);
    if (sun_ranking == NULL)
    {
        return RC::RC_SUN_RANKING_INVALID_RANKING_TYPE;
    }

    SCORE my_score = sun_ranking->GetScore(char_guid);

    rank = sun_ranking->GetRanking(my_score);

    return RC::RC_SUN_RANKING_SUCCESS;
}

void SunRankingManager::Display()
{
    static const int MAX_RANKING_STATE_STR = 64;

    SUNLOG(eCRITICAL_LOG, "[SunRanking] - SunRanking Count : %d", m_sun_rankings.size());

    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        SunRanking* sun_ranking = _GetSunRanking(type);
        if (sun_ranking == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "[SunRanking] - Rank Type(%d) is null", type);
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "[SunRanking] - Rank Type(%d) Count(%d)", type, sun_ranking->GetSize());
        }
    }

    SUNLOG(eCRITICAL_LOG, "[SunRanking] - SunRankingPlayer Count : %d", m_sun_ranking_players.size());
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM