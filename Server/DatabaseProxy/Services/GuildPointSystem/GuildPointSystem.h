#ifndef _GUILD_POINT_SYSTEM_H
#define _GUILD_POINT_SYSTEM_H
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM

#include <PointSystem/PointSystem.h>
#include "CurrencyInfoParser.h"

class GuildPointBank
{
    typedef PointSystem::CodeNode UserPoint;
    typedef PointSystem::PointNodeMap<DWORD /*uuid*/, UserPoint> GuildPoint;
    typedef PointSystem::PointNodeMap<DWORD /*guid*/, GuildPoint> GuildPointMap;
    GuildPointMap guild_point_bank_;
    DWORD currency_code_;

public:
    GuildPointBank(DWORD CURRENCY_CODE);
    ~GuildPointBank();

    DWORD GetCurrencyCode() const;;

    void SetPlayerPoint(DWORD _guid, DWORD _uuid, INT _point);
    INT GetPlayerPoint(DWORD _guid, DWORD _uuid) const;
    INT ChangePlayerPoint(DWORD _guid, DWORD _uuid, INT _point);
    void GuildPointErase(DWORD _guid, DWORD _uuid);

    INT GetGuildPointTotal(DWORD _guid) const;
    void ClearGuild(DWORD _guid);
};

class GuildPointSystem
{
private:
    typedef STLX_HASH_MAP<DWORD, GuildPointBank> PointBanks;
    PointBanks banks_;

public:
    enum { GUILD_TOTAL_UUID = 0 };

    GuildPointSystem();
    ~GuildPointSystem();

    void Initialize();

    void CheckIn(GUILDGUID _guid);
    void Quit(GUILDGUID _guid, CHARGUID _uuid);

    void Create(GUILDGUID guid);
    void Destroy(GUILDGUID guid);

    void ClearGuildPoint(DWORD _guid, DWORD _code);
    INT  GetGuildPointTotal( DWORD _guid, DWORD _code );

    void SetGuildPoint(DWORD _guid, DWORD _uuid, DWORD _code, INT _point);
    INT  GetGuildPoint(DWORD _guid, DWORD _uuid, DWORD _code) const;
    void GuildPointChange(GUILDGUID _guid, CHARGUID _uuid, DWORD _code, INT _point);
    void GuildPointErase(DWORD _guid, DWORD _uuid, DWORD _code);
};

extern GuildPointSystem g_GuildPointSystem;

#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#endif //_GUILD_POINT_SYSTEM_H