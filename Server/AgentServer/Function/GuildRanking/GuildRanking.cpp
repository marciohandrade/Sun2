#include <stdafx.h>
#include "GuildRanking.h"
#include <PacketStruct_AD.h>

GuildRankingSystem g_GuildRankingSystem;

GuildRankingSystem::GuildRankingSystem()
{
    update_timer_.SetTimer(1*60*1000);
    memset(&guild_ranking_, 0, sizeof(guild_ranking_));
}

GuildRankingSystem::~GuildRankingSystem()
{

}

void GuildRankingSystem::UpdateGuildRanking()
{
    // 로딩이 되어 있지 않으면 요청
    // 요청한뒤 1분이 지났어도 로딩이 되어 있지 않으면 다시 요청
    // 로딩이 되어 있다면 5분에 한번씩 날짜 검사 후 요청
    if (update_timer_.IsExpired() == false)
        return;

    if(guild_ranking_.is_loaded != false)
    {
        CTime current_time = util::TimeSync::_time64(NULL);
        int year = current_time.GetYear();
        int month = current_time.GetMonth();
        int day = current_time.GetDay();

        if ( year == 0 || month == 0 || day == 0) {
            //하나라도 0이 존재하면 CTime은 에러가 발생한다
            return;
        }

        CTime last_daily_init_time = \
            CTime(year, month, day, 4, 0, 0);

        if (last_daily_init_time > current_time) {
            last_daily_init_time -= CTimeSpan(1, 0, 0, 0);
        }

        if(last_daily_init_time < guild_ranking_.last_updated_datetime_)
            return;
    }

    ;{
        RequestGuildRankingLoad();
    };
}

void GuildRankingSystem::RequestGuildRankingLoad()
{
    guild_ranking_.is_attempted_loading = true;
    update_timer_.SetTimer(1*60*1000);

    MSG_AD_ETC_GUILD_RANKINGINFO_SYN guild_msg;
    AgentServer::GetInstance()->SendToDBPServer(&guild_msg, sizeof(guild_msg));
}

void GuildRankingSystem::SerializeGuildRanking( \
    GUILD_RANKING_INFO* guild_ranking_info, INT& guild_ranking_info_count, eSERIALIZE eType)
{
    switch(eType)
    {
        case SERIALIZE_LOAD:
        {
            memset(&guild_ranking_.guild_ranking_info_, 0, \
                sizeof(guild_ranking_.guild_ranking_info_));
            memcpy(&guild_ranking_.guild_ranking_info_, guild_ranking_info, \
                sizeof(GUILD_RANKING_INFO)*guild_ranking_info_count);
            guild_ranking_.guild_ranking_info_count_ = guild_ranking_info_count;

            guild_ranking_.last_updated_datetime_ = util::TimeSync::_time64(NULL);
            guild_ranking_.is_attempted_loading = false;
            guild_ranking_.is_loaded = true;
            update_timer_.SetTimer(5*60*1000);
        } break;

        case SERIALIZE_STORE:
        {
            if (guild_ranking_.is_loaded == false)
            {
                guild_ranking_info_count = 0;
                return;
            }

            memcpy(guild_ranking_info, guild_ranking_.guild_ranking_info_, \
                sizeof(GUILD_RANKING_INFO)*guild_ranking_.guild_ranking_info_count_);
            guild_ranking_info_count = guild_ranking_.guild_ranking_info_count_;
        } break;
    }
}