#pragma once
#ifndef GAMESERVER_CG_BATTLE_H
#define GAMESERVER_CG_BATTLE_H

#include "./PacketHandler.h"



#define Handler_CG_BATTLE_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_BATTLE_IMPL(p) \
    void Handler_CG_BATTLE::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_CG_BATTLE
{
public:
    Handler_CG_BATTLE();
    ~Handler_CG_BATTLE();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    // CG_BATTLE
    Handler_CG_BATTLE_DECL(CG_BATTLE_PLAYER_ATTACK_SYN);
    // CG_DOMINATION
    //_KR_001385_20091013_DOMINATION_BASICINFO_SETTING
    Handler_CG_BATTLE_DECL(CG_DOMINATION_MINIMAP_LIST_SYN);//지역 점령전 미니맵 정보 전송
    Handler_CG_BATTLE_DECL(CG_DOMINATION_APPLY_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_NOTICE_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_BATTING_HEIM_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_MANAGE_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_ACCEP_HEIM_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_SAVE_GATESTAT_SYN);
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT 
    Handler_CG_BATTLE_DECL(CG_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN);

#ifdef _NA_20100322_AGGRO_DISPLAY 
    Handler_CG_BATTLE_DECL(CG_MONSTER_SELECT_INFO_SYN);
#endif
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    Handler_CG_BATTLE_DECL(CG_DOMINATION_MEMBER_JOIN_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_MEMBER_LEAVE_SYN);
    Handler_CG_BATTLE_DECL(CG_DOMINATION_REFRESH_MEMBERLIST_REQ);

    static void OnCG_BATTLE_VKR_ATTACK_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);    //사용안함
    //static void OnCG_BATTLE_P2O_ATTACK_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);  //사용안함
    static void OnCG_BATTLE_VKR_RELOAD_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);    //사용안함
};

#endif //GAMESERVER_CG_BATTLE_H