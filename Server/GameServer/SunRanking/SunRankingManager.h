#pragma once
#ifndef SERVER_GAMESERVER_SUNRANKINGMANAGER_H
#define SERVER_GAMESERVER_SUNRANKINGMANAGER_H

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

#include <map>
#include <vector>

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

#include "GameBattleResult.h"

class Player;
class SunRanking;
struct SunRankingInfo;
typedef std::vector<const SunRankingInfo* const> RANKINGINFOS;
class SunRankingPlayer;

class SunRankingManager : public util::Singleton<SunRankingManager>, protected boost::object_pool<SunRankingPlayer>
{
public:
    typedef std::map<eSUN_RANKING_TYPE, SunRanking*>    SunRankings;
    typedef std::map<CHARGUID, SunRankingPlayer*>       SunRankingPlayers;

    SunRankingManager();
    ~SunRankingManager();

    void Initialize();
    void Release();
    void Display();

    void ProcessPvp(CHARGUID char_guid, SCORE score);
    void ProcessDisconnect(Player* player);
    void ProcessBattleZone(const BattleRecorder::Record& record, Player* player);

    void SetGuildGuid(CHARGUID char_guid, GUILDGUID guild_guid);
    void UpdateFromSunRankingPlayer(SunRankingPlayerInfo& info);
    bool CreateSunRankingPlayer(SunRankingPlayerInfo& info);

    RC::eSUN_RANKING_RESULT GetSunRankingInfos(MSG_CG_ETC_SUN_RANKING_ACK& msg, 
        eSUN_RANKING_TYPE ranking_type, 
        CHARGUID char_guid,
        int rank_count,
        bool is_guild_ranking, 
        GUILDGUID guild_guid);

    void GetTopRankerEquipmentScore(SCORE& score, CHARGUID& char_guid);
    SunRankingPlayer* GetSunRankingPlayer(CHARGUID char_guid);
    RC::eSUN_RANKING_RESULT GetRanking(eSUN_RANKING_TYPE ranking_type, CHARGUID char_guid, int& rank);

private:
    bool _UpdateSunRanking(SunRankingPlayer* sun_ranking_player, eSUN_RANKING_TYPE ranking_type, SCORE score);

    SunRanking*       _GetSunRanking(eSUN_RANKING_TYPE rank_type);

    SunRankings       m_sun_rankings;
    SunRankingPlayers m_sun_ranking_players;
};

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#endif // SERVER_GAMESERVER_SUNRANKINGMANAGER_H