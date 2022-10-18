#ifndef _PROGRAMCOMMON_POINTSYSTEM_PACKETSTRUCT_CG_POINTSYSTEM_H_
#define _PROGRAMCOMMON_POINTSYSTEM_PACKETSTRUCT_CG_POINTSYSTEM_H_

#include <PacketStruct_CG.h>

enum ePointWalletPacketTypeCG
{
    CG_POINTWALLET_SELECT_CMD = 1,
    //
    CG_POINTWALLET_CHANGE_CMD,
    //
    CG_POINTWALLET_CHANGE_WITH_COUNT_SYN,
    CG_POINTWALLET_CHANGE_WITH_COUNT_ACK,
    //
    CG_POINTWALLET_GUILDCOIN_DONATION_SYN,
    CG_POINTWALLET_GUILDCOIN_DONATION_ACK,

    CG_POINTWALLET_GUILDPOINT_CHANGE_BRD,
};

struct MSG_CG_POINTWALLET_SELECT_CMD : public MSG_CG_POINTWALLET_PACKET
{
    WORD count_;

    struct {
        DWORD code_;
        DWORD point_;
    } pointwallet_data[MAX_POINTWALLET_COUNT];

    MSG_CG_POINTWALLET_SELECT_CMD()
    {
        packet_type = CG_POINTWALLET_SELECT_CMD;
        count_ = 0;
        memset(pointwallet_data, 0, sizeof(pointwallet_data));
    }

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(pointwallet_data);
        return static_cast<uint16_t>(kMinSize + sizeof(pointwallet_data[0]) * count_);
    };
};

struct MSG_CG_POINTWALLET_CHANGE_CMD : public MSG_CG_POINTWALLET_PACKET
{
    DWORD code_;
    DWORD point_;

    MSG_CG_POINTWALLET_CHANGE_CMD(DWORD code, DWORD point)
    {
        packet_type = CG_POINTWALLET_CHANGE_CMD;
        code_ = code, point_ = point;
    }
};

struct MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN : public MSG_CG_POINTWALLET_PACKET
{
    SLOTIDX slot_index_;
    POSTYPE item_position_;
    WORD item_count_;

    MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN( \
        SLOTIDX slot_index, POSTYPE item_position, WORD item_count)
    {
        packet_type = CG_POINTWALLET_CHANGE_WITH_COUNT_SYN;
        slot_index_ = slot_index;
        item_position_ = item_position;
        item_count_ = item_count;
    }
};

struct MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_ACK : public MSG_CG_POINTWALLET_PACKET
{
    BYTE error_code_;
    SLOTIDX	slot_index_;
    POSTYPE item_position_;
    WORD item_count_;

    MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_ACK(BYTE error_code)
    {
        packet_type = CG_POINTWALLET_CHANGE_WITH_COUNT_ACK;
        error_code_ = error_code;
        slot_index_ = 0, item_position_ = 0; item_count_ = 0;
    }
};

//!~ _NA004034_20120102_GUILD_POINT_SYSTEM
struct MSG_CG_POINTWALLET_GUILDCOIN_DONATION_SYN : public MSG_CG_POINTWALLET_PACKET
{
    DWORD point_;

    MSG_CG_POINTWALLET_GUILDCOIN_DONATION_SYN()
    {
        packet_type = CG_POINTWALLET_GUILDCOIN_DONATION_SYN;
    }
};

struct MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK : public MSG_CG_POINTWALLET_PACKET
{
    BYTE result_code_;
    DWORD today_donation_count_;

    MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK()
    {
        packet_type = CG_POINTWALLET_GUILDCOIN_DONATION_ACK;
        result_code_ = 0, today_donation_count_ = 0;
    }
};

struct MSG_CG_POINTWALLET_GUILDPOINT_CHANGE_BRD : public MSG_CG_POINTWALLET_PACKET
{
    DWORD guid_;
    DWORD code_;
    DWORD uuid_;
    DWORD point_;

    MSG_CG_POINTWALLET_GUILDPOINT_CHANGE_BRD(DWORD guid, DWORD code, DWORD uuid, DWORD point)
    {
        packet_type = CG_POINTWALLET_GUILDPOINT_CHANGE_BRD;
        guid_ = guid, code_ = code, uuid_ = uuid, point_ = point;
    }
};
//~! _NA004034_20120102_GUILD_POINT_SYSTEM

#endif //_PROGRAMCOMMON_POINTSYSTEM_PACKETSTRUCT_CG_POINTSYSTEM_H_