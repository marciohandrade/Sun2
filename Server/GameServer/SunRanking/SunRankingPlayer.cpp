#include "stdafx.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking.h"
#include "SunRankingPlayer.h"
#include "SunRankingManager.h"
#include "SunRankingParser.h"
#include "Player.h"

SunRankingPlayer::SunRankingPlayer()
{
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        m_sun_ranking_score[type] = 0;
    }

    m_char_guid = 0;
    m_class_code = eCHAR_NONE;
    m_guild_guid = 0;
    memset(m_char_name, 0, MAX_CHARNAME_LENGTH);
}

void SunRankingPlayer::Initialize( SunRankingPlayerInfo& info )
{
    m_char_guid = info.char_guid;
    m_class_code = info.class_code;
    m_guild_guid = info.guild_guid;
    strncpy(m_char_name, info.char_name, MAX_CHARNAME_LENGTH);
}

SCORE* SunRankingPlayer::GetSunRankingScore( eSUN_RANKING_TYPE ranking_type )
{
    switch(ranking_type)
    {
    case eSUN_RANKING_TYPE_BEST_OF_BEST:
    case eSUN_RANKING_TYPE_ELETE_WARRIOR:
    case eSUN_RANKING_TYPE_LEVEL_KING:
    case eSUN_RANKING_TYPE_LEGEND_EQUIP:
    case eSUN_RANKING_TYPE_STRONG_ONE_SHOT:
    case eSUN_RANKING_TYPE_BEST_ATTACKER:
    case eSUN_RANKING_TYPE_MASTER_OF_HEAL:
        return &m_sun_ranking_score[ranking_type];
    default: // eSUN_RANKING_TYPE_END
        SUNLOG(eCRITICAL_LOG, __FUNCTION__ "invalid ranking type(%d)", ranking_type);
        return NULL;
        break;
    }
}

SCORE SunRankingPlayer::GetBestOfBestScore()
{
    static SunRankingParser* sun_ranking_parser = SunRankingParser::Instance();
    SCORE fame =
        m_sun_ranking_score[eSUN_RANKING_TYPE_ELETE_WARRIOR] * sun_ranking_parser->GetWeight(eSUN_RANKING_TYPE_ELETE_WARRIOR)
        + m_sun_ranking_score[eSUN_RANKING_TYPE_LEVEL_KING] * sun_ranking_parser->GetWeight(eSUN_RANKING_TYPE_LEVEL_KING)
        + m_sun_ranking_score[eSUN_RANKING_TYPE_LEGEND_EQUIP] * sun_ranking_parser->GetWeight(eSUN_RANKING_TYPE_LEGEND_EQUIP)
        + m_sun_ranking_score[eSUN_RANKING_TYPE_STRONG_ONE_SHOT] * sun_ranking_parser->GetWeight(eSUN_RANKING_TYPE_STRONG_ONE_SHOT)
        + m_sun_ranking_score[eSUN_RANKING_TYPE_BEST_ATTACKER] * sun_ranking_parser->GetWeight(eSUN_RANKING_TYPE_BEST_ATTACKER)
        + m_sun_ranking_score[eSUN_RANKING_TYPE_MASTER_OF_HEAL] * sun_ranking_parser->GetWeight(eSUN_RANKING_TYPE_MASTER_OF_HEAL);

    return fame;
}

void SunRankingPlayer::GetSunRankingPlayerInfo( SunRankingPlayerInfo& out_info )
{
    strncpy(out_info.char_name, GetCharName(), MAX_CHARNAME_LENGTH);
    out_info.char_guid = GetCharGuid();
    out_info.guild_guid = GetGuildGuid();
    out_info.class_code = GetCharClass();

    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        SCORE* score = GetSunRankingScore(type);
        out_info.score[type] = 0;
        if (score != NULL)
        {
            out_info.score[type] = *score;
        }
    }
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM