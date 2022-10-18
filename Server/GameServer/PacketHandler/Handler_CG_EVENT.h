#pragma once
#ifndef GAMESERVER_CG_EVENT_H
#define GAMESERVER_CG_EVENT_H

#include "./PacketHandler.h"




#define Handler_CG_EVENT_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)
#define Handler_CG_EVENT_IMPL(p) \
    void Handler_CG_EVENT::On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)

class Handler_CG_EVENT
{
public:
    Handler_CG_EVENT(){}
    ~Handler_CG_EVENT(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_EVENT_DECL(CG_EVENT_SELECT_SYN);
    Handler_CG_EVENT_DECL(CG_EVENT_MOVE_ITEM_TO_INVEN_SYN);
    Handler_CG_EVENT_DECL(CG_EVENT_ITEM_STATUS_SYN);
    Handler_CG_EVENT_DECL(CG_EVENT_ITEM_REG_SYN);
    Handler_CG_EVENT_DECL(CG_EVENT_TIME_PERIOD_SYN);

    Handler_CG_EVENT_DECL(CG_EVENT_QUERY_EXIST_SYN);

    //  <Desc> 기존과 다른 체계. 바로 보상 받는 형태다.
    Handler_CG_EVENT_DECL(CG_EVENT_REQUEST_ITEM_SYN);   // 성공시 CG_EVENT_ITEM_REWARD_CMD
    Handler_CG_EVENT_DECL(CG_EVENT_DONATION_HEIM_SYN);  // 기부 요청

    Handler_CG_EVENT_DECL(CG_EVENT_BUFF_EVENT_SYN);         // 버프 이벤트 요청

    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    Handler_CG_EVENT_DECL(CG_EVENT_INVITATEFRIEND_REWARD_SYN);        //친구초대 이벤트 아이템 보상 받기
    Handler_CG_EVENT_DECL(CG_EVENT_INVITATEFRIEND_RECALL_SYN);        //친구초대 이벤트 친구 소환
    Handler_CG_EVENT_DECL(CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_ACK);        
    Handler_CG_EVENT_DECL(CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK);        
    //}
#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC 
    Handler_CG_EVENT_DECL(CG_EVENT_DOMINATION_REWARD_CMD);
    Handler_CG_EVENT_DECL(CG_EVENT_LOTTO_REWARD_CMD);
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    Handler_CG_EVENT_DECL( CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ );
};

#endif //GAMESERVER_CG_EVENT_H