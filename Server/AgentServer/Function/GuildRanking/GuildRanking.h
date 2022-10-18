#ifndef _AGENTSERVER_FUNCTION_GUILDRANKING_GUILDRANKING_H
#define _AGENTSERVER_FUNCTION_GUILDRANKING_GUILDRANKING_H
#include <Singleton.h>
#include <atltime.h>

class GuildRankingSystem
{
    struct {
        CTime last_updated_datetime_;
        bool is_loaded;
        bool is_attempted_loading;
        GUILD_RANKING_INFO guild_ranking_info_[GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO];
        int guild_ranking_info_count_;
    } guild_ranking_;

    util::ITimerBase update_timer_;

public:
    GuildRankingSystem();
    virtual ~GuildRankingSystem();

    void UpdateGuildRanking();
    void RequestGuildRankingLoad();

    void SerializeGuildRanking( \
        GUILD_RANKING_INFO* guild_ranking_info, INT& guild_ranking_info_count, eSERIALIZE eType);
};

extern GuildRankingSystem g_GuildRankingSystem;

#endif //_AGENTSERVER_FUNCTION_GUILDRANKING_GUILDRANKING_H