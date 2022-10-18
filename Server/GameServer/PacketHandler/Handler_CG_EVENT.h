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

    //  <Desc> ������ �ٸ� ü��. �ٷ� ���� �޴� ���´�.
    Handler_CG_EVENT_DECL(CG_EVENT_REQUEST_ITEM_SYN);   // ������ CG_EVENT_ITEM_REWARD_CMD
    Handler_CG_EVENT_DECL(CG_EVENT_DONATION_HEIM_SYN);  // ��� ��û

    Handler_CG_EVENT_DECL(CG_EVENT_BUFF_EVENT_SYN);         // ���� �̺�Ʈ ��û

    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    Handler_CG_EVENT_DECL(CG_EVENT_INVITATEFRIEND_REWARD_SYN);        //ģ���ʴ� �̺�Ʈ ������ ���� �ޱ�
    Handler_CG_EVENT_DECL(CG_EVENT_INVITATEFRIEND_RECALL_SYN);        //ģ���ʴ� �̺�Ʈ ģ�� ��ȯ
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