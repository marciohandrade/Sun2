#pragma once
#ifndef GAMESERVER_CG_SUMMON_H
#define GAMESERVER_CG_SUMMON_H

#include "./PacketHandler.h"


class Summoned;

class Handler_CG_SUMMON
{
public:
    Handler_CG_SUMMON();
    ~Handler_CG_SUMMON();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_SUMMON_COMMAND_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_SUMMON_SKILL_ACTION_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_CRYSTALWARP_COMMAND_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    //
    static BOOL CheckMainTarget(Summoned *pSummoned, SLOTCODE SkillCode, DWORD &dwMainTragetKey, WzVector &vMainTragetPos);
};

#endif //GAMESERVER_CG_SUMMON_H