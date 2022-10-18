#ifndef _SERVER_COMMON_PACKETSTRUCT_POINTSYSTEM_H_
#define _SERVER_COMMON_PACKETSTRUCT_POINTSYSTEM_H_

#include "PacketStruct_DG.h"
#include "PacketStruct_GZ.h"
#include "PacketStruct_DZ.h"

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
//////////////////////////////////////////////////////////////////////////
// 
enum ePointWalletPacketTypeDG
{
    // point wallet system
    DG_POINTWALLET_SELECT_SYN = 1,
    DG_POINTWALLET_SELECT_ACK,

    DG_POINTWALLET_CHANGE_SYN,

    // guild point system
    DG_POINTWALLET_GUILDPOINT_DONATION_SYN,
    DG_POINTWALLET_GUILDPOINT_DONATION_ACK,
};
//DG_HONORSYSTEM_TITLES
struct MSG_DG_POINTWALLET_SELECT_SYN : MSG_DG_CHARINFO_POINTWALLET_SYSTEM
{
    MSG_DG_POINTWALLET_SELECT_SYN()
    {
        packet_type = DG_POINTWALLET_SELECT_SYN;
    }
};

struct MSG_DG_POINTWALLET_SELECT_ACK : MSG_DG_CHARINFO_POINTWALLET_SYSTEM
{
    WORD count_;

    struct {
        DWORD code_;
        DWORD point_;
    } pointwallet_data[MAX_POINTWALLET_COUNT];

    MSG_DG_POINTWALLET_SELECT_ACK()
    {
        packet_type = DG_POINTWALLET_SELECT_ACK;
        count_ = 0;
        memset(pointwallet_data, 0, sizeof(pointwallet_data));
    }

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(pointwallet_data);
        return static_cast<uint16_t>(kMinSize + sizeof(pointwallet_data[0]) * count_);
    };
};

struct MSG_DG_POINTWALLET_CHANGE_SYN : MSG_DG_CHARINFO_POINTWALLET_SYSTEM
{
    DWORD code_;
    DWORD point_;

    MSG_DG_POINTWALLET_CHANGE_SYN(DWORD code, DWORD point)
    {
        packet_type = DG_POINTWALLET_CHANGE_SYN;
        code_ = code, point_ = point;
    }
};
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
struct GUILDPOINTS
{
    DWORD guid_;
    DWORD uuid_;
    DWORD code_;
    DWORD point_;
};

struct MSG_DG_GUILDPOINT_DONATION_SYN : public MSG_DG_CHARINFO_POINTWALLET_SYSTEM
{
    DWORD guild_guid_;
    DWORD guild_point_code_;
    INT   donation_item_count_;
#ifdef _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA
    INT   max_guild_coin_donation_;
#endif // _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA

    MSG_DG_GUILDPOINT_DONATION_SYN()
    {
        packet_type = DG_POINTWALLET_GUILDPOINT_DONATION_SYN;
    }
};

struct MSG_DG_GUILDPOINT_DONATION_ACK : public MSG_DG_CHARINFO_POINTWALLET_SYSTEM
{
    INT donation_count_of_day_;
    INT donation_item_count_;

    MSG_DG_GUILDPOINT_DONATION_ACK()
    {
        packet_type = DG_POINTWALLET_GUILDPOINT_DONATION_ACK;
        donation_count_of_day_ = 0, donation_item_count_ = 0;
    }
};

//////////////////////////////////////////////////////////////////////////
// GuildServer -> DBProxyServer
//
enum eGuildPointPacketTypeDZ
{
    ZD_GUILDPOINT_SELECT_SYN = 1,
    ZD_GUILDPOINT_SELECT_ACK,

    ZD_GUILDPOINT_CHANGE_CMD,
    DZ_GUILDPOINT_DELETE_CMD,

    ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD,
};

struct MSG_ZD_GUILDPOINT_SELECT_SYN : public MSG_DZ_GUILDPOINT_SYSTEM
{
    DWORD guid_;
    DWORD code_;

    MSG_ZD_GUILDPOINT_SELECT_SYN(DWORD guid, DWORD code)
    {
        packet_type = ZD_GUILDPOINT_SELECT_SYN;
        guid_ = guid, code_ = code;
    }
};

struct MSG_ZD_GUILDPOINT_SELECT_ACK : public MSG_DZ_GUILDPOINT_SYSTEM
{
    DWORD guid_;
    DWORD code_;
    GUILDPOINTS guild_points[MAX_GUILD_MEMBER_NUM];
    DWORD count_;

    MSG_ZD_GUILDPOINT_SELECT_ACK()
    {
        packet_type = ZD_GUILDPOINT_SELECT_ACK;
        guid_ = 0, code_ = 0;
        memset(guild_points, 0, sizeof(guild_points));
        count_ = 0;
    }
};

struct MSG_ZD_GUILDPOINT_CHANGE_CMD : public MSG_DZ_GUILDPOINT_SYSTEM
{
    GUILDPOINTS guildpoint;

    MSG_ZD_GUILDPOINT_CHANGE_CMD(DWORD guid, DWORD uuid, DWORD code, DWORD point)
    {
        packet_type = ZD_GUILDPOINT_CHANGE_CMD;
        guildpoint.guid_ = guid;
        guildpoint.uuid_ = uuid;
        guildpoint.code_ = code;
        guildpoint.point_ = point;
    }
};

struct MSG_DZ_GUILDPOINT_DELETE_CMD : public MSG_DZ_GUILDPOINT_SYSTEM
{
    GUILDPOINTS guildpoint;

    MSG_DZ_GUILDPOINT_DELETE_CMD(DWORD guid, DWORD uuid, DWORD code)
    {
        packet_type = DZ_GUILDPOINT_DELETE_CMD;
        guildpoint.guid_ = guid;
        guildpoint.uuid_ = uuid;
        guildpoint.code_ = code;
        guildpoint.point_ = 0;
    }
};

struct MSG_ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD : public MSG_DZ_GUILDPOINT_SYSTEM
{
    DWORD guid_;
    BYTE level_;
    BYTE pre_level_;

    MSG_ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD(DWORD guid, BYTE level, BYTE pre_level)
    {
        packet_type = ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD;
        guid_ = guid, level_ = level, pre_level_ = pre_level;
    }
};
//////////////////////////////////////////////////////////////////////////
// GameServer -> GuildServer
//
enum eGuildPointPacketTypeGZ
{
    GZ_GUILDPOINT_CHANGE_CMD = 1,
    ZG_GUILDPOINT_CHANGE_BRD,
};

struct MSG_GZ_GUILDPOINT_CHANGE_CMD : public MSG_GZ_GUILDPOINTSYSTEM
{
    GUILDPOINTS guildpoint;

    MSG_GZ_GUILDPOINT_CHANGE_CMD(DWORD guid, DWORD uuid, DWORD code, DWORD point)
    {
        packet_type = GZ_GUILDPOINT_CHANGE_CMD;
        guildpoint.guid_ = guid;
        guildpoint.uuid_ = uuid;
        guildpoint.code_ = code;
        guildpoint.point_ = point;
    }
};

struct MSG_ZG_GUILDPOINT_CHANGE_BRD : public MSG_GZ_GUILDPOINTSYSTEM
{
    GUILDPOINTS guildpoint;

    MSG_ZG_GUILDPOINT_CHANGE_BRD(DWORD guid, DWORD uuid, DWORD code, DWORD point)
    {
        packet_type = ZG_GUILDPOINT_CHANGE_BRD;
        guildpoint.guid_ = guid;
        guildpoint.uuid_ = uuid;
        guildpoint.code_ = code;
        guildpoint.point_ = point;
    }
};
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#endif //_SERVER_COMMON_PACKETSTRUCT_POINTSYSTEM_H_