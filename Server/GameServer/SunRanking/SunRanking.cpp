#include "stdafx.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include "SunRanking.h"
#include <boost/tuple/tuple.hpp>

SunRanking::SunRanking()
:   m_indexed_by_char_guid(boost::multi_index::get<TagCharGuid>(m_sun_ranking_container)),
    m_indexed_by_guild_guid(boost::multi_index::get<TagGuildGuid>(m_sun_ranking_container)),
    m_indexed_by_ranking_value(boost::multi_index::get<TagScore>(m_sun_ranking_container))
{
}

SunRanking::~SunRanking()
{
    CharGuidIter it;
    for(it = m_indexed_by_char_guid.begin(); it != m_indexed_by_char_guid.end();)
    {
        destroy(*it++);
    }
    m_indexed_by_char_guid.clear();
}

template <class T_ITER>
void GetRankingInfos(T_ITER begin, T_ITER end, RANKINGINFOS& ranking_infos, int maximum)
{
    for (int rank = 0; (rank < maximum) && (begin != end) ; ++begin, ++rank)
    {
        SunRankingInfo* sun_ranking_info = *begin;
        if (sun_ranking_info == NULL)
        {
            continue;
        }

        ranking_infos.push_back(sun_ranking_info);
    }
}

template <class T_ITER>
int CalculateRanking(T_ITER begin, T_ITER end, T_ITER cur)
{
    return (cur == end) ? 0 : std::distance(begin, cur) + 1;
}

int SunRanking::GetRanking(SCORE score) const
{
    ScoreIter begin_iter = m_indexed_by_ranking_value.begin();
    ScoreIter cur_iter = m_indexed_by_ranking_value.find(score);
    ScoreIter end_iter = m_indexed_by_ranking_value.end();

    return CalculateRanking(begin_iter, end_iter, cur_iter);
};

int SunRanking::GetGuildRanking( SCORE score, GUILDGUID guild_guid ) const
{
    std::pair<GuildGuidIter, GuildGuidIter> equal_iter =
        m_indexed_by_guild_guid.equal_range(guild_guid);

    GuildGuidIter begin_iter = equal_iter.first;
    GuildGuidIter cur_iter = m_indexed_by_guild_guid.find(boost::tuples::make_tuple(guild_guid, score));
    GuildGuidIter end_iter = m_indexed_by_guild_guid.end();

    return CalculateRanking(begin_iter, end_iter, cur_iter);
}

bool SunRanking::GetRanking( int rank_count, RANKINGINFOS& ranking_infos ) const
{
    GetRankingInfos(m_indexed_by_ranking_value.begin(), m_indexed_by_ranking_value.end(),
        ranking_infos, rank_count);

    return true;
}

bool SunRanking::GetGuildRanking( GUILDGUID guild_guid, int rank_count, RANKINGINFOS& ranking_infos ) const
{
    std::pair<GuildGuidIter, GuildGuidIter> equal_iter =
        m_indexed_by_guild_guid.equal_range(guild_guid);

    GetRankingInfos(equal_iter.first, equal_iter.second, ranking_infos, rank_count);

    return true;
}

const SunRankingInfo* const SunRanking::_GetSunRankingInfo( CHARGUID char_guid ) const
{
    CharGuidIter cur_iter = m_indexed_by_char_guid.find(char_guid);
    CharGuidIter end_iter = m_indexed_by_char_guid.end();

    return (cur_iter == end_iter) ? NULL : *cur_iter;
}

bool SunRanking::UpdateScore(CHARGUID char_guid, GUILDGUID guild_guid, SCORE score)
{
    CharGuidIter cur_iter = m_indexed_by_char_guid.find(char_guid);
    if (cur_iter == m_indexed_by_char_guid.end())
    {
        SunRankingInfo* ranking_info = construct();
        ranking_info->char_guid = char_guid;
        ranking_info->guild_guid = guild_guid;
        ranking_info->score = score;
        m_indexed_by_char_guid.insert(ranking_info);
    }
    else
    {
        SunRankingInfo* ranking_info = *cur_iter;
        ranking_info->char_guid = char_guid;
        ranking_info->guild_guid = guild_guid;
        ranking_info->score = score;

        if (m_indexed_by_char_guid.replace(cur_iter, ranking_info) == false)
        {
            return false;
        }
    }
    return true;
};

SCORE SunRanking::GetScore( CHARGUID char_guid ) const
{
    const SunRankingInfo* const info = _GetSunRankingInfo(char_guid);
    if (info == NULL)
    {
        return 0;
    }

    return info->score;
}

bool SunRanking::SetGuildGuid( CHARGUID player_guid, GUILDGUID guild_guid )
{
    CharGuidIter cur_iter = m_indexed_by_char_guid.find(player_guid);
    if (cur_iter == m_indexed_by_char_guid.end())
    {
        return false;
    }
    else
    {
        SunRankingInfo* ranking_info = *cur_iter;
        ranking_info->char_guid = player_guid;
        ranking_info->guild_guid = guild_guid;

        if (m_indexed_by_char_guid.replace(cur_iter, ranking_info) == false)
        {
            return false;
        }
    }
    return true;
}

int SunRanking::GetSize()
{
    return m_sun_ranking_container.size();
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
