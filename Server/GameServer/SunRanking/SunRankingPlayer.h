#pragma once
#ifndef SERVER_GAMESERVER_SUNRANKINGPLAYER_H
#define SERVER_GAMESERVER_SUNRANKINGPLAYER_H

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

class Player;
class SunRankingManager;

class SunRankingPlayer
{
public:
    SunRankingPlayer();
    ~SunRankingPlayer(){};

    void Initialize(SunRankingPlayerInfo& info);

    SCORE  GetBestOfBestScore();
    SCORE* GetSunRankingScore(eSUN_RANKING_TYPE ranking_type);
    void   GetSunRankingPlayerInfo(SunRankingPlayerInfo& out_info);

    CHARGUID      GetCharGuid()  const { return m_char_guid; };
    GUILDGUID     GetGuildGuid() const { return m_guild_guid; };
    eCHAR_TYPE    GetCharClass() const { return m_class_code; };
    const TCHAR*  GetCharName()  const { return m_char_name; };
    void          SetGuildGuid(GUILDGUID guild_guid) { m_guild_guid = guild_guid; };

private:

    SCORE      m_sun_ranking_score[eSUN_RANKING_TYPE_END];
    char       m_char_name[MAX_CHARNAME_LENGTH+1];
    eCHAR_TYPE m_class_code;
    GUILDGUID  m_guild_guid;
    CHARGUID   m_char_guid;
};

#endif // SERVER_GAMESERVER_SUNRANKINGPLAYER_H

#endif // SERVER_GAMESERVER_SUNRAKINGPLAYER_H