#ifndef _SERVER_COMMON_PACKETSTRUCT_SMARTNPCSYSTEM_H_
#define _SERVER_COMMON_PACKETSTRUCT_SMARTNPCSYSTEM_H_

#include "PacketStruct_DG.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
//////////////////////////////////////////////////////////////////////////
//
enum eAttendanceType
{
    MONDSCHIEN_DOMINATION = 1, // 몬트샤인 공성전 참가
};

//////////////////////////////////////////////////////////////////////////
//
enum eSmartNPCSystemPacketTypeDG
{
    // Attendance process
    DG_SMARTNPC_SET_ATTENDANCE_SYN = 1,
    DG_SMARTNPC_SET_ATTENDANCE_ACK,
    DG_SMARTNPC_GET_ATTENDANCE_SYN,
    DG_SMARTNPC_GET_ATTENDANCE_ACK,
};

struct MSG_SMARTNPC_SET_ATTENDANCE_SYN : MSG_DG_EVENT_SMARTNPC_SYSTEM
{
    DWORD attendance_type;
    DWORD attendance_data;

    MSG_SMARTNPC_SET_ATTENDANCE_SYN(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;
        packet_type = DG_SMARTNPC_SET_ATTENDANCE_SYN;
        attendance_type = 0, attendance_data = 0;
    }
};

struct MSG_SMARTNPC_SET_ATTENDANCE_ACK : MSG_DG_EVENT_SMARTNPC_SYSTEM
{
    DWORD result;

    MSG_SMARTNPC_SET_ATTENDANCE_ACK(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;
        packet_type = DG_SMARTNPC_SET_ATTENDANCE_ACK;
    }
};

struct MSG_SMARTNPC_GET_ATTENDANCE_SYN : MSG_DG_EVENT_SMARTNPC_SYSTEM
{
    DWORD attendance_type;
    DWORD attendance_time;

    MSG_SMARTNPC_GET_ATTENDANCE_SYN(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;
        packet_type = DG_SMARTNPC_GET_ATTENDANCE_SYN;
        attendance_type = 0, attendance_time = 0;
    }
};

struct MSG_SMARTNPC_GET_ATTENDANCE_ACK : MSG_DG_EVENT_SMARTNPC_SYSTEM
{
    DWORD result;

    MSG_SMARTNPC_GET_ATTENDANCE_ACK(DWORD _CID, DWORD _NID, DWORD _EID, DWORD _STEP)
    {
        CID = _CID, NID = _NID, EID = _EID, STEP = _STEP;
        packet_type = DG_SMARTNPC_GET_ATTENDANCE_ACK;
    }
};

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#endif //_SERVER_COMMON_PACKETSTRUCT_SMARTNPCSYSTEM_H_