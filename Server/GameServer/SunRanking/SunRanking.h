#pragma once
#ifndef SERVER_GAMESERVER_SUNRANKING_H
#define SERVER_GAMESERVER_SUNRANKING_H

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include <vector>

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

struct SunRankingInfo
{
    SunRankingInfo()
    {
        char_guid   = 0;
        guild_guid    = 0;
        score = 0;
    }

    CHARGUID  char_guid;
    GUILDGUID guild_guid;
    SCORE     score;
};

typedef std::vector<const SunRankingInfo* const> RANKINGINFOS;

class SunRanking : protected boost::object_pool<SunRankingInfo>
{
private:
    struct TagCharGuid{};
    struct TagGuildGuid{};
    struct TagScore{};

    typedef boost::multi_index::multi_index_container
    <
        SunRankingInfo*,
        boost::multi_index::indexed_by
        <
            boost::multi_index::ordered_unique
            <
                boost::multi_index::tag<TagCharGuid>,
                BOOST_MULTI_INDEX_MEMBER(SunRankingInfo, CHARGUID, char_guid)
            >,
            boost::multi_index::ordered_non_unique
            <
                boost::multi_index::tag<TagGuildGuid>,
                boost::multi_index::composite_key
                <
                    SunRankingInfo*,
                    BOOST_MULTI_INDEX_MEMBER(SunRankingInfo, GUILDGUID, guild_guid),
                    BOOST_MULTI_INDEX_MEMBER(SunRankingInfo, SCORE, score)
                >,
                boost::multi_index::composite_key_compare
                <
                    std::less<GUILDGUID>,
                    std::greater<SCORE>
                >
            >,
            boost::multi_index::ordered_non_unique
            <
                boost::multi_index::tag<TagScore>,
                BOOST_MULTI_INDEX_MEMBER(SunRankingInfo, SCORE, score),
                std::greater<SCORE>
            >
        >
    > SunRankingInfoContainer;

    typedef boost::multi_index::index<SunRankingInfoContainer, TagCharGuid>::type  IndexByCharGuid;
    typedef boost::multi_index::index<SunRankingInfoContainer, TagGuildGuid>::type IndexByGuildGuid;
    typedef boost::multi_index::index<SunRankingInfoContainer, TagScore>::type     IndexByScore;

    typedef IndexByCharGuid::iterator  CharGuidIter;
    typedef IndexByGuildGuid::iterator GuildGuidIter;
    typedef IndexByScore::iterator     ScoreIter;

public:
    SunRanking();
    ~SunRanking();

    int GetSize();
    int  GetGuildRanking(SCORE score, GUILDGUID guild_guid) const;
    bool GetGuildRanking(GUILDGUID guild_guid, int rank_count, RANKINGINFOS& ranking_infos) const;

    int  GetRanking(SCORE score) const;
    bool GetRanking(int rank_count, RANKINGINFOS& ranking_infos) const;

    SCORE GetScore(CHARGUID char_guid) const;
    bool  SetGuildGuid(CHARGUID char_guid, GUILDGUID guild_guid);
    bool  UpdateScore(CHARGUID char_guid, GUILDGUID guild_guid, SCORE score);

private:
    const SunRankingInfo* const _GetSunRankingInfo(CHARGUID char_guid) const;

    SunRankingInfoContainer  m_sun_ranking_container;
    IndexByCharGuid&         m_indexed_by_char_guid;
    IndexByGuildGuid&        m_indexed_by_guild_guid;
    IndexByScore&            m_indexed_by_ranking_value;
};

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#endif // SERVER_GAMESERVER_SUNRANKING_H