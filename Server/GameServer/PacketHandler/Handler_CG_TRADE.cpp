
#include "stdafx.h"
#include ".\Handler_CG_TRADE.h"
#include ".\TradeManager.h"
#include ".\Player.h"
#include ".\PlayerManager.h"
#include ".\TradeSlotContainer.h"
#include "GameServerEx.h"
#include "GameZone.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_TRADE::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_TRADE::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_OPEN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_PUT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_PUT_MONEY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_GET_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_GET_MONEY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_PROPOSAL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_MODIFY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_ACCEPT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_CANCEL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_TRADE, CG_TRADE_MODIFY_RESPONSE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_CG_TRADE_IMPL( CG_TRADE_OPEN_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_OPEN_SYN * pRecvMsg = (MSG_CG_TRADE_OPEN_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

#if NATION_BUILD_VERSION >= 817
	#define __NA000000_CONSTRAINTS_CHANGE_IN_SSQ__
#endif

#ifdef __NA000000_CONSTRAINTS_CHANGE_IN_SSQ__
    // (WAVERIX) (090220) (AddinRules) Ÿ���� ��� �� �ŷ� �Ұ� + �ʵ� �� ���� �������� ����
    // (WAVERIX) (090226) (Change Conditional Concept) ���� ������ ���� ó���� ȥ��������.

    GameField* pGameField = pPlayer->GetField();
    if(pGameField == NULL)
        return;

    sPOLICY_ZONE& rPolicyment = pGameField->GetGameZone()->Policyment;
    Player* pTargetPlayer = PlayerManager::Instance()->FindPlayerByObjKey(pRecvMsg->m_dwPlayerKey);
    BOOLEAN bValidState = pTargetPlayer != NULL &&
                          pPlayer != pTargetPlayer &&
                          pGameField == pTargetPlayer->GetField() &&
                          (rPolicyment.FIELD & rPolicyment.ENABLE_SSQ_CONTROL) == FALSE &&
                          pPlayer->IsDead() == FALSE &&
                          pTargetPlayer->IsDead() == FALSE &&
                          (PLAYER_BEHAVE_IDLE_STATE == pPlayer->GetBehave() &&
                           PLAYER_BEHAVE_IDLE_STATE == pTargetPlayer->GetBehave());

    if(!bValidState)
    {
        MSG_CG_TRADE_OPEN_NAK msg;
        msg.m_byErrorCode = RC::RC_TRADE_COUNTERPARTNER_CANNOT_STATE;
        pPlayer->SendPacket(&msg, sizeof(msg));
        return;
    }

#else //!__NA000000_CONSTRAINTS_CHANGE_IN_SSQ__
	    if( !pPlayer->GetField() ) return;

	    Player * pTargetPlayer = PlayerManager::Instance()->FindPlayerByObjKey( pRecvMsg->m_dwPlayerKey );
	    if( !pTargetPlayer || 
		    !pTargetPlayer->GetField() || 
		    pTargetPlayer->GetBehave() != PLAYER_BEHAVE_IDLE_STATE || 
		    pPlayer->GetBehave() != PLAYER_BEHAVE_IDLE_STATE ||
		    pPlayer->IsDead() ||
		    pRecvMsg->m_dwPlayerKey == pRecvMsg->m_dwKey )
	    {
		    MSG_CG_TRADE_OPEN_NAK nmsg;
		    nmsg.m_byErrorCode = RC::RC_TRADE_COUNTERPARTNER_CANNOT_STATE;
		    pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		    return;
	    }
#endif //!__NA000000_CONSTRAINTS_CHANGE_IN_SSQ__

	if(pTargetPlayer->GetInterceptionOption(eINTERCEPTION_TRADE))
	{
		MSG_CG_TRADE_OPEN_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_TRADE_INTERCEPTION;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	RC::eTRADE_RESULT rt = RC::RC_TRADE_SUCCESS;
	if( pPlayer->GetGameZonePtr() && pTargetPlayer->GetGameZonePtr() && 
		pPlayer->GetGameZonePtr()->GetKey() == pTargetPlayer->GetGameZonePtr()->GetKey() )
	{
		if(  DISTANCE_NPC_MEET >= pPlayer->GetDist(pTargetPlayer) )
		{
			rt = g_TradeManager.StartTrade( pPlayer, pTargetPlayer );
		}
		else
		{
			rt = RC::RC_TRADE_LENGTH_CANNOT_TRADE;
		}		
	}
	else
	{
		rt = RC::RC_TRADE_DIFFERENT_ZONE;
		SUNLOG(eCRITICAL_LOG, "�ٸ� Zone ���� Player���� Trade �õ�(hacking) - shogen" );
	}

	switch(rt)
	{
	case RC::RC_TRADE_SUCCESS:
		{

			MSG_CG_TRADE_OPEN_ACK amsg;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			MSG_CG_TRADE_OPEN_CMD cmsg;
			cmsg.m_dwPlayerKey = pPlayer->GetObjectKey();
			pTargetPlayer->SendPacket( &cmsg, sizeof(cmsg) );
		}
		break;
	default:
		{
			MSG_CG_TRADE_OPEN_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_TRADE_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_TRADE_IMPL( CG_TRADE_PUT_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_PUT_SYN * pRecvMsg = (MSG_CG_TRADE_PUT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	MSG_CG_TRADE_PUT_CMD cmsg;
	RC::eTRADE_RESULT rt = g_TradeManager.PutItem( pPlayer, pRecvMsg->m_OrgPos, cmsg.m_TradePos, cmsg.m_ItemStream );
	switch(rt)
	{
	case RC::RC_TRADE_SUCCESS:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();

			MSG_CG_TRADE_PUT_ACK amsg;
			amsg.m_OrgPos = pRecvMsg->m_OrgPos;
			amsg.m_TradePos = cmsg.m_TradePos;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );

		}
		break;
		/*
	case RC::RC_TRADE_CANCEL:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			if( g_TradeManager.DestroyTradeContainer( pPlayer ) )
			{			
				MSG_CG_TRADE_CANCEL_CMD amsg;
				pPlayer->SendPacket( &amsg, sizeof(amsg) );

				MSG_CG_TRADE_CANCEL_CMD cmsg;
				pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
			}
		}
		break;
		*/
	default:
		{
			MSG_CG_TRADE_PUT_NAK nmsg;
			nmsg.m_byErrorCode = rt;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}
	}
}

Handler_CG_TRADE_IMPL( CG_TRADE_PUT_MONEY_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_PUT_MONEY_SYN * pRecvMsg = (MSG_CG_TRADE_PUT_MONEY_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	RC::eTRADE_RESULT rt = g_TradeManager.PutMoney( pPlayer, pRecvMsg->m_money );
	switch(rt)
	{
	case RC::RC_TRADE_SUCCESS:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();

			MSG_CG_TRADE_PUT_MONEY_ACK amsg;
			amsg.m_money = pRecvMsg->m_money;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			MSG_CG_TRADE_PUT_MONEY_CMD cmsg;
			cmsg.m_money = pRecvMsg->m_money;
			pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
		}
		break;
		/*
	case RC::RC_TRADE_CANCEL:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			if( g_TradeManager.DestroyTradeContainer( pPlayer ) )
			{			
				MSG_CG_TRADE_CANCEL_CMD amsg;
				pPlayer->SendPacket( &amsg, sizeof(amsg) );

				MSG_CG_TRADE_CANCEL_CMD cmsg;
				pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
			}
		}
		break;
		*/
	default:
		{
			MSG_CG_TRADE_PUT_NAK nmsg;
			nmsg.m_byErrorCode = rt;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}
	}
}

Handler_CG_TRADE_IMPL( CG_TRADE_GET_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_GET_SYN * pRecvMsg = (MSG_CG_TRADE_GET_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	RC::eTRADE_RESULT rt = g_TradeManager.GetItem( pPlayer, pRecvMsg->m_TradePos );
	switch(rt)
	{
	case RC::RC_TRADE_SUCCESS:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();

			MSG_CG_TRADE_GET_ACK amsg;
			amsg.m_TradePos = pRecvMsg->m_TradePos;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			MSG_CG_TRADE_GET_CMD cmsg;
			cmsg.m_TradePos = pRecvMsg->m_TradePos;
			pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );

		}
		break;
		/*
	case RC::RC_TRADE_CANCEL:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			if( g_TradeManager.DestroyTradeContainer( pPlayer ) )
			{			
				MSG_CG_TRADE_CANCEL_CMD amsg;
				pPlayer->SendPacket( &amsg, sizeof(amsg) );

				MSG_CG_TRADE_CANCEL_CMD cmsg;
				pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
			}
		}
		break;
		*/
	default:
		{
			MSG_CG_TRADE_GET_NAK nmsg;
			nmsg.m_byErrorCode = rt;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}
	}
}

Handler_CG_TRADE_IMPL( CG_TRADE_GET_MONEY_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_GET_MONEY_SYN * pRecvMsg = (MSG_CG_TRADE_GET_MONEY_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	RC::eTRADE_RESULT rt = g_TradeManager.GetMoney( pPlayer, pRecvMsg->m_money );
	switch(rt)
	{
	case RC::RC_TRADE_SUCCESS:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();

			MSG_CG_TRADE_GET_MONEY_ACK amsg;
			amsg.m_money = pRecvMsg->m_money;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			MSG_CG_TRADE_GET_MONEY_CMD cmsg;
			cmsg.m_money = pRecvMsg->m_money;
			pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
		}
		break;
		/*
	case RC::RC_TRADE_CANCEL:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			if( g_TradeManager.DestroyTradeContainer( pPlayer ) )
			{			
				MSG_CG_TRADE_CANCEL_CMD amsg;
				pPlayer->SendPacket( &amsg, sizeof(amsg) );

				MSG_CG_TRADE_CANCEL_CMD cmsg;
				pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
			}
		}
		break;
		*/
	default:
		{
			MSG_CG_TRADE_GET_MONEY_NAK nmsg;
			nmsg.m_byErrorCode = rt;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}
	}
}


Handler_CG_TRADE_IMPL( CG_TRADE_PROPOSAL_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_PROPOSAL_SYN * pRecvMsg = (MSG_CG_TRADE_PROPOSAL_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	if( pPlayer->IsDead() )
	{
		MSG_CG_TRADE_PROPOSAL_NAK nak;
		nak.m_byErrorCode =  RC::RC_TRADE_COUNTERPARTNER_CANNOT_STATE;
		pPlayer->SendPacket( &nak, sizeof(nak) );
		return;
	}

	RC::eTRADE_RESULT rt = g_TradeManager.Proposal( pPlayer );
	switch(rt)
	{
	case RC::RC_TRADE_SUCCESS:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();

			MSG_CG_TRADE_PROPOSAL_ACK amsg;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			MSG_CG_TRADE_PROPOSAL_CMD cmsg;
			pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
		}
		break;
		/*
	case RC::RC_TRADE_CANCEL:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			if( g_TradeManager.DestroyTradeContainer( pPlayer ) )
			{			
				MSG_CG_TRADE_CANCEL_CMD amsg;
				pPlayer->SendPacket( &amsg, sizeof(amsg) );

				MSG_CG_TRADE_CANCEL_CMD cmsg;
				pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
			}
		}
		break;
		*/
	default:
		{
			MSG_CG_TRADE_PROPOSAL_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_TRADE_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

//	���� ��û ��Ŷ�� ������ ���� �������� ���� ��û�� ���� ���θ� �����Ѵ�.
Handler_CG_TRADE_IMPL( CG_TRADE_MODIFY_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_MODIFY_SYN * pRecvMsg = (MSG_CG_TRADE_MODIFY_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	if( pPlayer->IsDead() )
	{
		MSG_CG_TRADE_PROPOSAL_NAK nak;
		nak.m_byErrorCode =  RC::RC_TRADE_COUNTERPARTNER_CANNOT_STATE;
		pPlayer->SendPacket( &nak, sizeof(nak) );
		return;
	}

	if( pPlayer->GetTradeContainer() && pPlayer->GetTradeContainer()->GetCounterPlayer() )
	{
		MSG_CG_TRADE_MODIFY_CMD cmsg;
		pPlayer->GetTradeContainer()->GetCounterPlayer()->SendPacket( &cmsg, sizeof(cmsg) );
	}
	else
	{
		MSG_CG_TRADE_MODIFY_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_TRADE_CANCEL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
	}
}

//	���� ��û�� �� ������ ���� ��û�� �����ϴ� ������ �߸� �Ǵ��� �� �ִµ�
//  CG_TRADE_MODIFY_RESPONSE_SYN�� ���� ��û�� �����ϴ� ������ ������ ��Ŷ�̹Ƿ�
//	pPlayer�� ���� ��û�� �����ϴ� �����̴�...

//	�����û�� ���� �������� �����ϸ� Modify( ��û�� ���� )
//	�޾Ƶ����� ������ ��û�� �������� ����ڵ� ����
Handler_CG_TRADE_IMPL( CG_TRADE_MODIFY_RESPONSE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_MODIFY_RESPONSE_SYN * pRecvMsg = (MSG_CG_TRADE_MODIFY_RESPONSE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );												// ��û�� ���� ����
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;
	if( !pPlayer->GetTradeContainer() || 
		!pPlayer->GetTradeContainer()->GetCounterPlayer() ) 
	{
		MSG_CG_TRADE_MODIFY_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_TRADE_CANCEL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	Player* pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
	ASSERT( pCounterPlayer );										// ��û�� �� ����
	if( !pCounterPlayer ) return;
	if( !pCounterPlayer->GetField() ) return;
	if( !pCounterPlayer->GetTradeContainer() ) return;

	if( pRecvMsg->m_bAccept )
	{	// �޾� ����
		RC::eTRADE_RESULT rt = g_TradeManager.Modify( pCounterPlayer );
		if ( RC::RC_TRADE_SUCCESS == rt )
		{
			MSG_CG_TRADE_MODIFY_RESPONSE_ACK cmsg;					// �����û�� ������ ��������
			pPlayer->SendPacket( &cmsg, sizeof(cmsg) );

			MSG_CG_TRADE_MODIFY_ACK amsg;							// ������ ��û�� ��������
			pCounterPlayer->SendPacket( &amsg, sizeof(amsg) );

			return;
		}
	}

	MSG_CG_TRADE_MODIFY_RESPONSE_NAK responseNak;
	responseNak.m_byErrorCode = RC::RC_TRADE_MODIFY_FAIL;
	pPlayer->SendPacket( &responseNak, sizeof(responseNak) );

	MSG_CG_TRADE_MODIFY_NAK modifyNak;
	modifyNak.m_byErrorCode = RC::RC_TRADE_MODIFY_CANCEL;
	pCounterPlayer->SendPacket( &modifyNak, sizeof(modifyNak) );
}

Handler_CG_TRADE_IMPL( CG_TRADE_ACCEPT_SYN )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_ACCEPT_SYN * pRecvMsg = (MSG_CG_TRADE_ACCEPT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	static MSG_CG_TRADE_ACCEPT_BRD bmsg1;
	static MSG_CG_TRADE_ACCEPT_BRD bmsg2;

	RC::eTRADE_RESULT rt = g_TradeManager.Exchange( pPlayer, bmsg1.m_TradeInfo, bmsg1.m_Money, bmsg2.m_TradeInfo, bmsg2.m_Money );
	switch(rt)
	{
	case RC::RC_TRADE_COUNTERPARTNER_NOTACCEPT:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();

			MSG_CG_TRADE_ACCEPT_ACK amsg;
			pPlayer->SendPacket( &amsg, sizeof(amsg) );

			// ����� �˷��ش�.
			MSG_CG_TRADE_ACCEPT_CMD cmsg;
			pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
		}
		break;
		/*
	case RC::RC_TRADE_CANCEL:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			if( g_TradeManager.DestroyTradeContainer( pPlayer ) )
			{			
				MSG_CG_TRADE_CANCEL_CMD amsg;
				pPlayer->SendPacket( &amsg, sizeof(amsg) );

				MSG_CG_TRADE_CANCEL_CMD cmsg;
				pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
			}
		}
		break;
		*/
	case RC::RC_TRADE_SUCCESS:
		{
			Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
			SUNLOG( eFULL_LOG, "Exchange����[%s][%s]", pPlayer->GetCharName(), pCounterPlayer->GetCharName() );

			pPlayer->SendPacket( &bmsg1, bmsg1.GetSize() );

			// ����� �˷��ش�.
			pCounterPlayer->SendPacket( &bmsg2, bmsg2.GetSize() );


			RC::eTRADE_RESULT rt = g_TradeManager.EndTrade( pPlayer );
			if( rt != RC::RC_TRADE_SUCCESS )
			{
				SUNLOG( eFULL_LOG, "�ŷ���EndTrade����[%s][%s]", pPlayer->GetCharName(), pCounterPlayer->GetCharName() );
			}

			SUNLOG( eFULL_LOG, "EndTrade[%s][%s]", pPlayer->GetCharName(), pCounterPlayer->GetCharName() );
			// ��������
			pPlayer->SerializeInfoToDBProxy();
			pCounterPlayer->SerializeInfoToDBProxy();

			SUNLOG( eFULL_LOG, "�ŷ���DB����[%s][%s]", pPlayer->GetCharName(), pCounterPlayer->GetCharName() );
		}
		break;
	default:
		{
			MSG_CG_TRADE_ACCEPT_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_TRADE_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_TRADE_IMPL( CG_TRADE_CANCEL_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_TRADE_CANCEL_SYN * pRecvMsg = (MSG_CG_TRADE_CANCEL_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	
	if( !g_TradeManager.ValidTradeState(pPlayer) )
	{
		MSG_CG_TRADE_CANCEL_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_TRADE_INVALID_STATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	Player * pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
	if( RC::RC_TRADE_SUCCESS == g_TradeManager.EndTrade( pPlayer ) )
	{			
		SUNLOG( eFULL_LOG, "�ŷ�����Ͽ�����[%s][%s]", pPlayer->GetCharName(), pCounterPlayer->GetCharName() );
		MSG_CG_TRADE_CANCEL_ACK amsg;
		pPlayer->SendPacket( &amsg, sizeof(amsg) );

		MSG_CG_TRADE_CANCEL_CMD cmsg;
		pCounterPlayer->SendPacket( &cmsg, sizeof(cmsg) );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "�ŷ���ҽ���[%s][%s:%u]", 
			pPlayer->GetCharName(), pCounterPlayer->GetCharName(), pCounterPlayer->GetBehave() );
	}
}
