#ifndef _PROGRAMCOMMON_PACKETSTRUCT_MONSTERBOOK_H
#define _PROGRAMCOMMON_PACKETSTRUCT_MONSTERBOOK_H

#include "Protocol_CG.h"
#include "PacketStruct_CG.h"

enum eCG_CHARINFO_MONSTERBOOK_PACKET
{ 
    MONSTERBOOK_SERIALIZE_CMD = 1,
    MONSTERBOOK_ADD_CMD,
};

struct MSG_CG_MONSTERBOOK_SERIALIZE_CMD : public MSG_CG_CHARINFO_MONSTERBOOK_PACKET
{
    enum { MONSTERBOOK_MAX_COUNT = 1000 };

    DWORD monster_code_count;
    DWORD monster_code[MONSTERBOOK_MAX_COUNT];

    MSG_CG_MONSTERBOOK_SERIALIZE_CMD()
    {
        packet_type = MONSTERBOOK_SERIALIZE_CMD;
        monster_code_count = 0;
        memset(&monster_code, 0, sizeof(monster_code));
    }
};

struct MSG_CG_MONSTERBOOK_ADD_CMD : public MSG_CG_CHARINFO_MONSTERBOOK_PACKET
{
    DWORD monster_code_;

    MSG_CG_MONSTERBOOK_ADD_CMD()
    {
        packet_type = MONSTERBOOK_ADD_CMD;
        monster_code_ = 0;
    }
};
#endif //_PROGRAMCOMMON_PACKETSTRUCT_MONSTERBOOK_H