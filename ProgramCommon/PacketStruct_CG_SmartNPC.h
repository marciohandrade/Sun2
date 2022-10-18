#ifndef _PROGRAMCOMMON_PACKETSTRUCT_SMARTNPC_H_
#define _PROGRAMCOMMON_PACKETSTRUCT_SMARTNPC_H_

#include "Protocol_CG.h"
#include "PacketStruct_CG.h"

enum eMSG_CG_SMARTNPC_PACKET
{
    NPCDIALOG_OPERATION_CMD = 1,    // C -> S
    NPCDIALOG_SHOWMENU_CMD,         // S -> C
    NPCDIALOG_ON_CLICK_MENU_CMD,    // C -> S
    NPCDIALOG_SHOWMESSAGEBOX_CMD,   // S -> C
    NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD, // C -> S
};

struct MSG_CG_NPCDIALOG_OPERATION_CMD : public MSG_CG_CHARINFO_SMARTNPC_PACKET
{
    MSG_CG_NPCDIALOG_OPERATION_CMD(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        packet_type = NPCDIALOG_OPERATION_CMD;
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;
    }
};

struct MSG_CG_NPCDIALOG_SHOWMENU_CMD : public MSG_CG_CHARINFO_SMARTNPC_PACKET
{
    WORD data_length_;
    CHAR data_[1024];

    MSG_CG_NPCDIALOG_SHOWMENU_CMD(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        packet_type = NPCDIALOG_SHOWMENU_CMD;
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;

        data_length_ = 0;
        memset(data_, 0, sizeof(data_));
    }
};

struct MSG_CG_NPCDIALOG_ON_CLICK_MENU_CMD : public MSG_CG_CHARINFO_SMARTNPC_PACKET
{
    MSG_CG_NPCDIALOG_ON_CLICK_MENU_CMD(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        packet_type = NPCDIALOG_ON_CLICK_MENU_CMD;
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;
    }
};

struct MSG_CG_NPCDIALOG_SHOWMESSAGEBOX_CMD : public MSG_CG_CHARINFO_SMARTNPC_PACKET
{
    WORD data_length_;
    CHAR data_[1024];

    MSG_CG_NPCDIALOG_SHOWMESSAGEBOX_CMD(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        packet_type = NPCDIALOG_SHOWMESSAGEBOX_CMD;
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;

        data_length_ = 0;
        memset(data_, 0, sizeof(data_));
    }
};

struct MSG_CG_NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD : public MSG_CG_CHARINFO_SMARTNPC_PACKET
{
    WORD data_length_;
    CHAR data_[1024];

    MSG_CG_NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        packet_type = NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD;
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;

        data_length_ = 0;
        memset(data_, 0, sizeof(data_));
    }
};
#endif //_PROGRAMCOMMON_PACKETSTRUCT_SMARTNPC_H_