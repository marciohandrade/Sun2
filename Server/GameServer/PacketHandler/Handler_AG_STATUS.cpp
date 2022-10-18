#include "StdAfx.h"
#include "Handler_AG_STATUS.h"
#include <PacketStruct_AG.h>
#include "Player.h"
#include <ServerSession.h>
#include "ItemManager.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_STATUS::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_STATUS::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_DECREASE_DURA_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_INTERCEPTION_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_INTERCEPTION_NAK, PHASE_SERVICE),
        //__KR_001355_20090608_GROUPINGSYSTEM_GW
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_GROUP_SET_MEMBERINFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_GROUP_ADDEXP_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_GROUP_ADDMONEY_CMD, PHASE_SERVICE),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_LOTTO_REWARD_SYN, PHASE_SERVICE),
    #ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        HANDLER_NODE_INFO(AG_STATUS, AG_STATUS_LOTTO_TICKET_PURCHASE_SYN, PHASE_SERVICE),
    #endif //
        //}
        HANDLER_NODE_INFO(AG_STATUS, AG_USER_DOMINATION_RETURNHIEM_CMD, PHASE_SERVICE),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

// 내구도 감소
Handler_AG_STATUS_IMPL( AG_STATUS_DECREASE_DURA_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_DECREASE_DURA_CMD *pRecvMsg = (MSG_AG_STATUS_DECREASE_DURA_CMD*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	// 내구도 감소
	POSTYPE RandomPos = random( eEQUIPCONTAINER_WEAPON, eEQUIPCONTAINER_SHIRTS );
    if (nsSlot::DurabilityImpl* const impl = pPlayer->GetItemManager()->GetDurabilityImpl()) {
        impl->DecreaseDurabilityUsingReason(DECDURA_TYPE_LEAVE_ROOM, (eEQUIPCONTAINER_POS)RandomPos);
    };
}

Handler_AG_STATUS_IMPL( AG_STATUS_INTERCEPTION_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_INTERCEPTION_ACK *pRecvMsg = (MSG_AG_STATUS_INTERCEPTION_ACK *)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
	if(!pPlayer)	return;

	if(!pPlayer->SetInterceptionOption(pRecvMsg->m_byInterceptionCode, pRecvMsg->m_bRequestFlag))
	{
		MSG_CG_STATUS_INTERCEPTION_NAK nakMSG;
		
		switch(pRecvMsg->m_byInterceptionCode)
		{
		case eINTERCEPTION_WHISPER:
			nakMSG.m_byErrorCode = RC::RC_STATUS_WHISPER_INTERCEPTION_FAILED;
			break;
		default:
			break;
		}

		pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
		
	}

	MSG_CG_STATUS_INTERCEPTION_ACK ackMSG;
	ackMSG.m_byInterceptionCode = eINTERCEPTION_WHISPER;
	ackMSG.m_bRequestFlag = pRecvMsg->m_bRequestFlag;

	pPlayer->SendPacket(&ackMSG, sizeof(ackMSG));
}

Handler_AG_STATUS_IMPL( AG_STATUS_INTERCEPTION_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_INTERCEPTION_NAK *pRecvMsg = (MSG_AG_STATUS_INTERCEPTION_NAK *)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
	if(!pPlayer)	return;

	MSG_CG_STATUS_INTERCEPTION_NAK nakMSG;
	nakMSG.m_byErrorCode = RC::RC_STATUS_WHISPER_INTERCEPTION_FAILED;
	pPlayer->SendPacket(&nakMSG, sizeof(nakMSG));
}

Handler_AG_STATUS_IMPL( AG_STATUS_GROUP_SET_MEMBERINFO_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_GROUP_SET_MEMBERINFO_CMD* pRecvMsg = 
        static_cast<MSG_AG_STATUS_GROUP_SET_MEMBERINFO_CMD*>(pMsg);

    Player *pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pRecvMsg->m_CharGuid );

    if (pPlayer == NULL)
        return;

    pPlayer->SetGroupHighMemberInfo( pRecvMsg->m_HighGroupInfo );
    pPlayer->SetGroupLowMemberInfo( pRecvMsg->m_LowGroupInfo );
}
Handler_AG_STATUS_IMPL( AG_STATUS_GROUP_ADDEXP_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_GROUP_ADDEXP_CMD* recv_msg = 
        static_cast<MSG_AG_STATUS_GROUP_ADDEXP_CMD*>(pMsg);

    Player *player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->m_CharGuid);
    if (player == NULL) 
    {
        return;
    }

    EXPTYPE old_exp = player->GetExp();
    player->AddExp(recv_msg->m_Amount, 0, 0, TRUE, GAME_CONTENT_GROUPING);

#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
    GAMELOG->LogExp(EXP_GROUPSYSTEM, player, player->GetExp(), old_exp, recv_msg->m_szCharName);
#endif //_NA_000000_20131031_GAMELOG_REDUCE
}

Handler_AG_STATUS_IMPL( AG_STATUS_GROUP_ADDMONEY_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_GROUP_ADDMONEY_CMD* recv_msg = 
        static_cast<MSG_AG_STATUS_GROUP_ADDMONEY_CMD*>(pMsg);

    Player *player = PlayerManager::Instance()->FindPlayerByCharKey(recv_msg->m_CharGuid);
    if (player == NULL)
    {
        return;
    }

    player->PlusMoney(recv_msg->m_Amount, Player::eMONEY_GROUP);

#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
    GAMELOG->LogMoney(MONEY_GROUPSYSTEM_GETMONEY, player, 
                      recv_msg->m_Amount, 0, NULL, recv_msg->m_szCharName );
#endif //_NA_000000_20131031_GAMELOG_REDUCE
}

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { AG_STATUS_LOTTO_~ }

Handler_AG_STATUS_IMPL( AG_STATUS_LOTTO_REWARD_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_STATUS_LOTTO_REWARD_SYN* pRecvMsg = (MSG_AG_STATUS_LOTTO_REWARD_SYN*)pMsg;

    MSG_AG_STATUS_LOTTO_REWARD_ACK ack;
    
    Player *pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pRecvMsg->m_CharGuid );
    
    if (pPlayer != NULL)
    {
        // 하임 로또 상금 지급
        pPlayer->PlusMoney(pRecvMsg->m_Prize);

        // 하임 획득 게임 로그
        GAMELOG->LogMoney(MONEY_LOTTO_REWARD, pPlayer, pRecvMsg->m_Prize);

        // 클라이언트에게 하임 획득 통보
        MSG_CG_ITEM_PICK_MONEY_ACK ntf;
        ntf.m_Money      = pPlayer->GetMoney();
        ntf.m_BonusRatio = (WORD)0;
        pPlayer->SendPacket(&ntf, sizeof(ntf));

        ack.m_iResult  = MSG_AG_STATUS_LOTTO_REWARD_ACK::RESULT_OK;
    }
    else {
        ack.m_iResult  = MSG_AG_STATUS_LOTTO_REWARD_ACK::ERROR_CHAR_NOT_ONLINE;
    }
    ack.m_CharGuid = pRecvMsg->m_CharGuid;
    ack.m_Prize    = pRecvMsg->m_Prize;
    pServerSession->Send((BYTE*)&ack, (WORD)sizeof(ack));
}
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
Handler_AG_STATUS_IMPL( AG_STATUS_LOTTO_TICKET_PURCHASE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_SYN* pRecvMsg = \
        static_cast<MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_SYN*>(pMsg);

    MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_ACK ack;

    Player *pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pRecvMsg->iCharGuid );

    if (pPlayer != NULL)
    {
        if (pPlayer->GetMoney() < pRecvMsg->i64TicketPrice) {
            ack.iResult = MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_ACK::ERROR_NOT_ENOUGH_MONEY;
        }
        else
        {
            // 복권용지 가격 차감
            pPlayer->MinusMoney(pRecvMsg->i64TicketPrice);

            // 하임 소비 게임 로그
            GAMELOG->LogMoney(MONEY_LOTTO_TICKET_PURCHASE, pPlayer, pRecvMsg->i64TicketPrice);

            ack.iResult = MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_ACK::RESULT_OK;
        }
    }
    else {
        ack.iResult = MSG_AG_STATUS_LOTTO_REWARD_ACK::ERROR_CHAR_NOT_ONLINE;
    }
    ack.iCharGuid = pRecvMsg->iCharGuid;
    ack.wDrawNum = pRecvMsg->wDrawNum;
    ack.bAuto = pRecvMsg->bAuto;
    for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
        ack.bPickedNum[i] = pRecvMsg->bPickedNum[i];
    }
    pServerSession->Send((BYTE*)&ack, (WORD)sizeof(ack));
}
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

Handler_AG_STATUS_IMPL( AG_USER_DOMINATION_RETURNHIEM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_USER_DOMINATION_RETURNHIEM_CMD* recv_msg = (MSG_AG_USER_DOMINATION_RETURNHIEM_CMD*)pMsg;

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey( recv_msg->m_CharGuid );

    if (player != NULL)
    {
        player->PlusMoney(recv_msg->m_TenderCost);

        if (recv_msg->m_Type == SYSMEMO_DOMINATION_RETURN_MONEY)
        {
            GAMELOG->LogMoney(MONEY_DOMINATION_ACTIONMONEY_RETURN, player, 
                recv_msg->m_TenderCost);
        }
        else if(recv_msg->m_Type == SYSMEMO_DOMINATION_REWARD_MONEY)
        {
            GAMELOG->LogMoney(MONEY_DOMINATION_ACTIONMONEY_REWARD, player, 
                recv_msg->m_TenderCost);
        }

        MSG_CG_ITEM_PICK_MONEY_ACK notify_msg;
        notify_msg.m_Money      = player->GetMoney();
        notify_msg.m_BonusRatio = static_cast<WORD>(0);
        player->SendPacket(&notify_msg, sizeof(notify_msg));
    }
}
