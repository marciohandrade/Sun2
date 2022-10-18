#include <stdafx.h>
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
#include "GuildPointSystem.h"
#include <CurrencyInfoParser.h>
#include <PointSystem/PacketStruct_CG_PointSystem.h>
#include <PacketStruct_PointSystem.h>

#include <QueryObjects/DBHandler.h>
#include <PacketHandler/PacketHandler.h>
#include <Services/Guild/GuildManager.h>
#include <Services/Guild/Guild.h>

GuildPointBank::GuildPointBank(DWORD CURRENCY_CODE) 
{
    currency_code_ = CURRENCY_CODE;
}

GuildPointBank::~GuildPointBank() 
{
}

void GuildPointBank::SetPlayerPoint( DWORD _guid, DWORD _uuid, INT _point )
{
    GuildPoint* guild_node = guild_point_bank_.find(_guid);
    if (guild_node == NULL) 
    {
        guild_node = guild_point_bank_.insert(_guid);
        if (!guild_node)
            return;
    }

    UserPoint* player_node = guild_node->find(_uuid);
    if (player_node == NULL) 
    {
        player_node = guild_node->insert(_uuid);
        if (!player_node)
            return;
    }

    player_node->SetValue(_point);
}

INT GuildPointBank::GetPlayerPoint( DWORD _guid, DWORD _uuid ) const
{
    const GuildPoint* guild_node = guild_point_bank_.find(_guid);
    if (guild_node == NULL)
        return 0;

    const UserPoint* player_node = guild_node->find(_uuid);
    if (player_node == NULL)
        return 0;

    return (player_node->GetValue());
}

INT GuildPointBank::ChangePlayerPoint( DWORD _guid, DWORD _uuid, int _point )
{
    int point = GetPlayerPoint(_guid, _uuid);
    
    point += _point;

    SetPlayerPoint(_guid, _uuid, point);

    return point;
}

INT GuildPointBank::GetGuildPointTotal( DWORD _guid ) const
{
    const GuildPoint* guild_node = guild_point_bank_.find(_guid);
    return ((guild_node) ? GetPlayerPoint(_guid, GuildPointSystem::GUILD_TOTAL_UUID) : -1);
}

DWORD GuildPointBank::GetCurrencyCode() const
{
    return currency_code_;
}

void GuildPointBank::ClearGuild( DWORD _guid )
{
    GuildPoint* guild_node = guild_point_bank_.find(_guid);
    if (guild_node) {
        guild_node->clear();
    }
}

void GuildPointBank::GuildPointErase( DWORD _guid, DWORD _uuid )
{
    GuildPoint* const guild_node = guild_point_bank_.find(_guid);
    if (guild_node)
        guild_node->erase(_uuid);
}

//////////////////////////////////////////////////////////////////////////
//
GuildPointSystem g_GuildPointSystem;

GuildPointSystem::GuildPointSystem()
{
}

GuildPointSystem::~GuildPointSystem()
{

}

void GuildPointSystem::Initialize()
{
    DWORD currencies[] = {
        CURRENCY_INFO_DONATION_GUILDCOIN,
        CURRENCY_INFO_GUILD_POINT,
    };

    int currency_count = sizeof(currencies)/sizeof(currencies[0]);
    for(int i = 0; i < currency_count; i++) {
        DWORD code = currencies[i];
        banks_.insert(PointBanks::value_type(code, GuildPointBank(code)));
    }
}

void GuildPointSystem::CheckIn( GUILDGUID _guid )
{
    for(PointBanks::iterator it = banks_.begin(); it != banks_.end(); ++it) {
        GuildPointBank& bank = it->second;
        
        INT guild_total = bank.GetGuildPointTotal(_guid);
        if (guild_total < 0) {
            MSG_ZD_GUILDPOINT_SELECT_SYN msg(_guid, bank.GetCurrencyCode());
            g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));
        }
    }
}

void GuildPointSystem::Create(GUILDGUID guid)
{
    for(PointBanks::iterator it = banks_.begin(); it != banks_.end(); ++it) 
    {
        GuildPointBank& bank = it->second;

        bank.SetPlayerPoint(guid, GuildPointSystem::GUILD_TOTAL_UUID, 0);
    }
}

void GuildPointSystem::Destroy(GUILDGUID guid)
{
    for(PointBanks::iterator it = banks_.begin(); it != banks_.end(); ++it) 
    {
        GuildPointBank& bank = it->second;

        bank.ClearGuild(guid);
    }
}

// 길드 탈퇴
void GuildPointSystem::Quit( GUILDGUID guid, CHARGUID uuid )
{
    // 기부 포인트 차감
    const DWORD code = CURRENCY_INFO_DONATION_GUILDCOIN;
    INT coin = GetGuildPoint(guid, uuid, code);
    if (coin > 0) {
        g_GuildManager.ProcessGuildCoinChange(guid, uuid, code, -coin);
    }

    GuildPointErase(guid, uuid, code);

    MSG_DZ_GUILDPOINT_DELETE_CMD delete_cmd(guid, uuid, code);
    g_DBProxyServer.SendToDBPServer(&delete_cmd, sizeof(delete_cmd));
}

void GuildPointSystem::GuildPointChange(GUILDGUID _guid, CHARGUID _uuid, DWORD _code, INT _point)
{
    PointBanks::iterator it = banks_.find(_code);
    if(it == banks_.end())
        return;
    
    const DWORD uuid_base = GuildPointSystem::GUILD_TOTAL_UUID;

    GuildPointBank& bank = it->second;

    // uuid가 0(길드 값)이 아닐 경우, char_guid 나타냄
    // 1. 캐릭터의 포인트값 기록
    if (_uuid != uuid_base) 
    {
        INT result_point = bank.ChangePlayerPoint(_guid, _uuid, _point);

        MSG_ZD_GUILDPOINT_CHANGE_CMD msg_user(_guid, _uuid, _code, result_point);
        g_DBProxyServer.SendToDBPServer(&msg_user, sizeof(msg_user));
    }

    // 2. 길드의 포인트값 기록 (uuid == 0)
    // 포인트 감소(-) 되는 상황에 대한 처리
#ifdef _NA_008606_20151222_MODIFY_GUILD_LEVEL
    if (_code == CURRENCY_INFO_DONATION_GUILDCOIN && _point < 0)
    {
        GuildRankInfoParser* const guild_rankinfo_paser = GuildRankInfoParser::Instance();
        INT cur_guild_point = bank.GetGuildPointTotal(_guid);

        BYTE cur_grade = guild_rankinfo_paser->GetMatchedRankInfoByGuildCoin(cur_guild_point);
        BYTE changed_grade = guild_rankinfo_paser->GetMatchedRankInfoByGuildCoin(cur_guild_point + _point);

        //길드레벨이 떨어진다면 길드레벨 다운시키지않고 현 레벨에서 경험치 0으로 세팅한다
        if (cur_grade != changed_grade)
        {
            const GuildRankInfo* rank_info = guild_rankinfo_paser->FindGuildRankInfo(cur_grade);
            INT minimum_grade_point = rank_info->need_guild_coin_to_rankup_;

            bank.SetPlayerPoint(_guid, uuid_base, minimum_grade_point);

            MSG_ZD_GUILDPOINT_CHANGE_CMD msg_user(_guid, uuid_base, _code, minimum_grade_point);
            g_DBProxyServer.SendToDBPServer(&msg_user, sizeof(msg_user));
        }
        else
        {
            INT point_total = bank.ChangePlayerPoint(_guid, uuid_base, _point);

            MSG_ZD_GUILDPOINT_CHANGE_CMD msg_user(_guid, uuid_base, _code, point_total);
            g_DBProxyServer.SendToDBPServer(&msg_user, sizeof(msg_user));
        }
    }
    else
    {
        INT point_total = bank.ChangePlayerPoint(_guid, uuid_base, _point);

        MSG_ZD_GUILDPOINT_CHANGE_CMD msg_user(_guid, uuid_base, _code, point_total);
        g_DBProxyServer.SendToDBPServer(&msg_user, sizeof(msg_user));
    }
#else
    INT point_total = bank.ChangePlayerPoint(_guid, uuid_base, _point);

    MSG_ZD_GUILDPOINT_CHANGE_CMD msg_user(_guid, uuid_base, _code, point_total);
    g_DBProxyServer.SendToDBPServer(&msg_user, sizeof(msg_user));
#endif //_NA_008606_20151222_MODIFY_GUILD_LEVEL

    // 3.
    ;{
        Guild* guild = g_GuildManager.FindGuild(_guid);
        if(guild == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Guild Not Found!");
            return;
        }

        GuildMember* member = guild->FindMember(_uuid);
        if (member) 
        {
            INT user_point = bank.GetPlayerPoint(_guid, _uuid);

            if(_code == CURRENCY_INFO_DONATION_GUILDCOIN)
                member->SetGuildCoin(user_point);
        }

        ;{
            INT total_point = bank.GetGuildPointTotal(_guid);

            if(_code == CURRENCY_INFO_DONATION_GUILDCOIN)
                guild->SetGuildCoin(total_point);
            else if(_code == CURRENCY_INFO_GUILD_POINT)
                guild->SetGuildPoint(total_point);
        };
    };
}

void GuildPointSystem::ClearGuildPoint( DWORD _guid, DWORD _code )
{
    PointBanks::iterator it = banks_.find(_code);
    if(it == banks_.end())
        return;

    GuildPointBank& bank = it->second;

    bank.ClearGuild(_guid);
}

void GuildPointSystem::SetGuildPoint( DWORD _guid, DWORD _uuid, DWORD _code, INT _point )
{
    PointBanks::iterator it = banks_.find(_code);
    if(it == banks_.end())
        return;

    GuildPointBank& bank = it->second;

    bank.SetPlayerPoint(_guid, _uuid, _point);

    ;{
        Guild* guild = g_GuildManager.FindGuild(_guid);
        if(!guild)
            return;

        if(_uuid == GUILD_TOTAL_UUID)
        {
            if(_code == CURRENCY_INFO_DONATION_GUILDCOIN)
                guild->SetGuildCoin(_point);
            else if(_code == CURRENCY_INFO_GUILD_POINT)
                guild->SetGuildPoint(_point);
        }
        else 
        {
            GuildMember* member = guild->FindMember(_uuid);
            if(!member)
                return;

            if(_code == CURRENCY_INFO_DONATION_GUILDCOIN)
                member->SetGuildCoin(_point);
        }
    };
}

INT GuildPointSystem::GetGuildPoint( DWORD _guid, DWORD _uuid, DWORD _code ) const
{
    PointBanks::const_iterator it = banks_.find(_code);
    if(it == banks_.end())
        return 0;

    const GuildPointBank& bank = it->second;

    return bank.GetPlayerPoint(_guid, _uuid);
}

INT GuildPointSystem::GetGuildPointTotal( DWORD _guid, DWORD _code )
{
    PointBanks::iterator it = banks_.find(_code);
    if(it == banks_.end())
        return 0;

    GuildPointBank& bank = it->second;

    return bank.GetGuildPointTotal(_guid);
}

void GuildPointSystem::GuildPointErase( DWORD _guid, DWORD _uuid, DWORD _code )
{
    PointBanks::iterator it = banks_.find(_code);
    if(it != banks_.end())
    {
        GuildPointBank& bank = it->second;
        bank.GuildPointErase(_guid, _uuid);
    }
}

#endif //_NA004034_20120102_GUILD_POINT_SYSTEM