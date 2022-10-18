#pragma once
#ifndef GUILD_IN_GAME_LOG_H
#define GUILD_IN_GAME_LOG_H

#ifdef _NA_003923_20120130_GUILD_RENEWAL

typedef STLX_LIST<GuildInGameLogData> InGameLogList;

class Player;

class GuildInGameLog
{
public:
    GuildInGameLog(GUILDGUID guild_guid);
    ~GuildInGameLog();

    void InsertLogToLIst(
        const GuildInGameLogData& log_data
        );

    void WriteGuildLog(
        const BYTE log_type,//eLOG_GUILD
        const GUILDGUID request_guild_guid,
        const TCHAR* string_param = NULL,
        const DWORD param1 = 0,
        const DWORD param2 = 0,
        const DWORD param3 = 0
        );

    void InitializeGuildLog(
        const GuildInGameLogData* input_data,
        const WORD log_count
        );

    void SendInGameLogToPlayer(
        CHARGUID char_guid
        );

private:
    void _WriteGuildLogToDB(
        const GUILDGUID guild_guid, 
        const GuildInGameLogData& log_data
        );

    GuildInGameLogCategory _GetLogCategory(
        const GuildInGameLogType log_type
        )const;

    void _RequestLogListToDB(
        GUILDGUID guild_guid
        )const;

    BYTE _SerializeGuildLog(
        GuildInGameLogData* OUT output_data,
        const GuildInGameLogCategory log_type
        )const;

private:
    InGameLogList* log_list_[GuildMaxLog];
};

#endif //_NA_003923_20120130_GUILD_RENEWAL
#endif //GUILD_IN_GAME_LOG_H