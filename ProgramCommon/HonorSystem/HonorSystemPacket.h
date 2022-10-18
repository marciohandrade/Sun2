#ifndef _PROGRAMCOMMON_HONORSYSTEM_HONORSYSTEMPACKET_H
#define _PROGRAMCOMMON_HONORSYSTEM_HONORSYSTEMPACKET_H

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "Protocol_CG.h"

//////////////////////////////////////////////////////////////////////////
// category : CG_CHARINFO
// protocol : MSG_CG_HONORSYSTEM_PACKET
// packet_type : eCG_HONORSYSTEM_PACKET
enum eCG_HONORSYSTEM_PACKET
{
    CG_HONORSYSTEM_TITLE = 1,
    CG_HONORSYSTEM_FAMEREPUTE = 2,
};

//
struct CG_HONORSYSTEM_TITLE_PACKET : public MSG_CG_HONORSYSTEM_PACKET
{
    BYTE packet_type2; // sub packet type

    CG_HONORSYSTEM_TITLE_PACKET()
    {
        packet_type = CG_HONORSYSTEM_TITLE;
    }
};

// 
struct CG_HONORSYSTEM_FAMEREPUTE_PACKET : public MSG_CG_HONORSYSTEM_PACKET
{
    BYTE packet_type2; // sub packet type

    CG_HONORSYSTEM_FAMEREPUTE_PACKET()
    {
        packet_type = CG_HONORSYSTEM_FAMEREPUTE;
    }
};

//////////////////////////////////////////////////////////////////////////
// category : CG_CHARINFO
// protocol : MSG_CG_HONORSYSTEM_PACKET
// packet_type  : eCG_HONORSYSTEM_PACKET
// packet_type2 : eCG_HONORSTSTEM_TITLE_PACKET
enum eCG_HONORSTSTEM_TITLE_PACKET
{
    CG_HONORSYSTEM_TITLE_CHANGE_CMD = 1,
    CG_HONORSYSTEM_TITLE_CHANGE_BRD,
    CG_HONORSYSTEM_TITLE_ADD_ACK,
    CG_HONORSYSTEM_TITLE_REMOVE_ACK,
    CG_HONORSYSTEM_TITLE_LIST_ACK,
    
    CG_HONORSYSTEM_TITLE_NAK,
};

struct PACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD : public CG_HONORSYSTEM_TITLE_PACKET
{
    WORD honor_title_index_;

    PACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD(WORD index)
    {
        packet_type2 = CG_HONORSYSTEM_TITLE_CHANGE_CMD;
        honor_title_index_ = index;
    }
};

struct PACKET_CG_HONORSYSTEM_TITLE_CHANGE_BRD : public CG_HONORSYSTEM_TITLE_PACKET
{
    PLAYERKEY player_key_;
    WORD honor_title_index_;

    PACKET_CG_HONORSYSTEM_TITLE_CHANGE_BRD(PLAYERKEY player_key, WORD index)
    {
        packet_type2 = CG_HONORSYSTEM_TITLE_CHANGE_BRD;
        player_key_ = player_key;
        honor_title_index_ = index;
    }
};

struct PACKET_CG_HONORSYSTEM_TITLE_ADD_ACK : public CG_HONORSYSTEM_TITLE_PACKET
{
    WORD honor_title_index_;

    PACKET_CG_HONORSYSTEM_TITLE_ADD_ACK(WORD index)
    {
        packet_type2 = CG_HONORSYSTEM_TITLE_ADD_ACK;
        honor_title_index_ = index;
    }
};

struct PACKET_CG_HONORSYSTEM_TITLE_REMOVE_ACK : public CG_HONORSYSTEM_TITLE_PACKET
{
    WORD honor_title_index_;

    PACKET_CG_HONORSYSTEM_TITLE_REMOVE_ACK(WORD index)
    {
        packet_type2 = CG_HONORSYSTEM_TITLE_REMOVE_ACK;
        honor_title_index_ = index;
    }
};

struct PACKET_CG_HONORSYSTEM_TITLE_LIST_ACK : public CG_HONORSYSTEM_TITLE_PACKET
{
    HonorTitleTable titles_;

    PACKET_CG_HONORSYSTEM_TITLE_LIST_ACK()
    {
        packet_type2 = CG_HONORSYSTEM_TITLE_LIST_ACK;
        memset(&titles_, 0, sizeof(titles_));
    }

    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(titles_);
        return static_cast<uint16_t>(kMinSize + titles_.GetSize());
    };
};

enum eHonorSystemTitleErrorCode
{
    HONORSYSTEM_TITLE_ERROR = 0, // general error...
    HONORSYSTEM_TITLE_ERROR_CHANGE_FAIL, // title change fail error
    HONORSYSTEM_TITLE_ERROR_ADD_FAIL,
    HONORSYSTEM_TITLE_ERROR_REMOVE_FAIL,
};

struct PACKET_CG_HONORSYSTEM_TITLE_NAK : public CG_HONORSYSTEM_TITLE_PACKET
{
    WORD error_code_;

    PACKET_CG_HONORSYSTEM_TITLE_NAK()
        : error_code_(0)
    {
        packet_type2 = CG_HONORSYSTEM_TITLE_NAK;
    }
};

//////////////////////////////////////////////////////////////////////////
// category : CG_CHARINFO
// protocol : MSG_CG_HONORSYSTEM_PACKET
// packet_type  : eCG_HONORSYSTEM_PACKET
// packet_type2 : eCG_HONORSTSTEM_FAMEREPUTE_PACKET
enum eCG_HONORSTSTEM_FAMEREPUTE_PACKET
{
    CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK = 1,// 명성/평판 전체 점수 정보(S->C)
    CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD,     // 명성/평판 변경 커맨드(S->C)
    
    CG_HONORSYSTEM_FAMEREPUTE_NAK,
};

enum eHonorSystemFameReputeErrorCode
{
    HONORSYSTEM_FAMEREPUTE_ERROR = 0, // general error...

};

struct PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK : public CG_HONORSYSTEM_FAMEREPUTE_PACKET
{
    HonorPointTable honor_point_table_;

    PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK()
    {
        packet_type2 = CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK;
        memset(&honor_point_table_, 0, sizeof(honor_point_table_));
    }

    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(honor_point_table_);
        return static_cast<uint16_t>(kMinSize + honor_point_table_.GetSize());
    };
};

struct PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD : public CG_HONORSYSTEM_FAMEREPUTE_PACKET
{
    BYTE type_;
    WORD code_;   // 명성 코드
    DWORD point_; // 해당 명성의 변경된 점수

    PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD(BYTE _type, WORD _code, DWORD _point)
    {
        packet_type2 = CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD;
        type_ = _type, code_ = _code, point_ = _point;
    }
};

struct PACKET_CG_HONORSYSTEM_FAMEREPUTE_NAK : public CG_HONORSYSTEM_FAMEREPUTE_PACKET
{
    WORD error_code_;

    PACKET_CG_HONORSYSTEM_FAMEREPUTE_NAK()
        : error_code_(0)
    {
        packet_type2 = CG_HONORSYSTEM_FAMEREPUTE_NAK;
    }
};
#endif //_NA_003027_20111013_HONOR_SYSTEM
#endif //_PROGRAMCOMMON_HONORSYSTEM_HONORSYSTEMPACKET_H