#include "StdAfx.h"
#include <PacketStruct_CG.h>
#include <PacketStruct_DG.h>
#include <SunEventInfoParser.h>
#include <DropRatioInfoList.h>

#include "Handler_CG_Event.h"
#include "EventInventorySlotContainer.h"
#include "EventManager.h"

#include "Player.h"
#include "ItemManager.h"
#include "RewardManager.h"
#include "Skill.h"
#include ".\FriendInvitation\GFriendInvitationController.h"
#include "ChaoState.h"

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
#include "PostManager.h"
#endif

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
#include "DominationInfoParser.h"
#endif

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_EVENT::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_EVENT::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_SELECT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_MOVE_ITEM_TO_INVEN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_ITEM_STATUS_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_ITEM_REG_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_QUERY_EXIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_TIME_PERIOD_SYN, PHASE_SERVICE),

        //	<Desc> ������ �ٸ� ü��. �ٷ� ���� �޴� ���´�.
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_REQUEST_ITEM_SYN, PHASE_SERVICE),	// ������ CG_EVENT_ITEM_REWARD_CMD
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_DONATION_HEIM_SYN, PHASE_SERVICE),	// ��� ��û

        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_BUFF_EVENT_SYN, PHASE_SERVICE),			// ���� �̺�Ʈ ��û
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK, PHASE_SERVICE),
        //}
#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC 
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_DOMINATION_REWARD_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_LOTTO_REWARD_CMD, PHASE_SERVICE),
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ, PHASE_SERVICE),
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


struct sEventTransactionWrap
{
	sEventTransactionWrap( sTransaction& rTrans ) : m_rTrans(rTrans) { m_rTrans.Enter(); }
	~sEventTransactionWrap() { m_rTrans.Leave(); }
	sTransaction&	m_rTrans;
};

inline static VOID	_RegisterEventStatus( DWORD eventID, Player* IN pPlayer, EVENT_STATUS_EX** OUT ppOutEventStatus )
{
	EVENT_STATUS_EX eventStatus;	// auto clear
	eventStatus.m_dwEventID = eventID;

	IPlayerEventD& rEventD = pPlayer->PlayerEvent();
	rEventD.AddEventStatus( eventStatus );
	(*ppOutEventStatus) = rEventD.FindEventStatus( eventID );
}


#define CHECK_IS_NOT_FIELDSERVER()		(g_pGameServer->GetServerType() != FIELD_SERVER)


Handler_CG_EVENT_IMPL( CG_EVENT_SELECT_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_EVENT_SELECT_SYN* pMsg = (MSG_CG_EVENT_SELECT_SYN *)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	RC::eITEM_RESULT rt = pPlayer->GetEventSlotContainer()->Open();
	if( rt == RC::RC_ITEM_EVENT_SUCCESS /*RC::RC_EVENT_SUCCESS*/ )
	{
		MSG_DG_EVENT_SELECT_SYN msg;
		//msg.m_UserGuid = pPlayer->GetUserGuid();
		pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );
	}
	else
	{
		MSG_CG_EVENT_SELECT_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
	}
}

Handler_CG_EVENT_IMPL( CG_EVENT_MOVE_ITEM_TO_INVEN_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_SYN* pMsg = (MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_SYN *)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    EventInventorySlotContainer* const event_container = pPlayer->GetEventSlotContainer();

    MSG_DG_EVENT_UPDATE_SYN msg;
    RC::eITEM_RESULT rt = event_container->MoveItemToInventory(pMsg->m_ItemPos, msg.m_ItemSeq);
	if( rt == RC::RC_ITEM_EVENT_SUCCESS /*RC::RC_EVENT_SUCCESS*/ )
	{
		//msg.m_UserGuid	= pPlayer->GetUserGuid();
		msg.m_ItemPos	= pMsg->m_ItemPos;
		pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );
	}
	else
	{
        // CHANGES: f110316.4L, fixed locking state when the result of MoveItemToInventory is fails.
        if (rt != RC::RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION) {
            // NOTE: nothing to do except unlocking
            event_container->OnMoveItemToInventory(false, INVALID_POSTYPE_VALUE, NULL);
        }
		MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
	}
}

// �̺�Ʈ ���� ���� Ȯ��
Handler_CG_EVENT_IMPL( CG_EVENT_ITEM_STATUS_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	const MSG_CG_EVENT_ITEM_STATUS_SYN* pMsg = (MSG_CG_EVENT_ITEM_STATUS_SYN*)pRecvMsg;
	MSG_CG_EVENT_ITEM_STATUS_ACK msgACK;

	// �ش� ������ �̺�Ʈ ���� �����ش�
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !(pPlayer && pPlayer->GetField()) )
		return;

	IPlayerEventD& rPEvent = pPlayer->PlayerEvent();
	msgACK.m_wEventNum = rPEvent.GetEventNum();
	if( msgACK.m_wEventNum )
		rPEvent.GetEventStatus( (DWORD)_countof(msgACK.m_EventStatus), msgACK.m_EventStatus );

	pPlayer->SendPacket( &msgACK, msgACK.GetSize() );
	MessageOut( eFULL_LOG, "Event Status Check[%u][C:%u][Event:%u]", pMsg->m_dwKey, pPlayer->GetCharGuid(), msgACK.m_wEventNum );
}

//������������������������������������������������������������������������������������
//	<Handler for CG_EVENT_ITEM_REG_SYN>
//	-	SubHandler
//
struct SubHandler_for_CG_EVENT_ITEM_REG_SYN
{
	static const DWORD		ERRCODE_DEFAULT			= 1000000;
	static const DWORD		ERRCODE_RANDREWARD		= ERRCODE_DEFAULT+2;

	//������������������������������������������������������������������������������������
	//
	static VOID		On_eEVENT_REGITEM_GETITEM( FlowControl& flow, Player* const pPlayer, EVENT_INFO* const pEventInfo )
	{
        if( !pPlayer->GetField() )
        {
            SUNLOG( eCRITICAL_LOG, "[Handler_CG_EVENT_IMPL::On_eEVENT_REGITEM_GETITEM] Not Exist GameField" );
            return;
        }

		IPlayerEventD& rEventD = pPlayer->PlayerEvent();
		const CODETYPE eventID = pEventInfo->m_dwEventID;

		FLOWCONTROL_START()
        {
			// start LOGIC_BLOCK
			EVENT_STATUS_EX* pEventStatus = rEventD.FindEventStatus( eventID );
			if( NULL == pEventStatus )
				_RegisterEventStatus( eventID, pPlayer, &pEventStatus );

			{	// ���� ���� �ʰ� - �̹� ��� �Ϸ�
				const WORD registerLimit = (WORD)pEventInfo->m_byCountMax;
				if(registerLimit)
					IfFailThenBreak(registerLimit > pEventStatus->m_wRegisterCnt,
                                    RC::RC_EVENT_ITEM_NUM_OVERFLOW);
			};

			ItemManager* const pItemManager = pPlayer->GetItemManager();

			// ����� �������� ����� ������ŭ �����ϴ��� �˻�
			SLOTCODE delItem[_MAX_EVENT_REGITEM] = { 0, };
			POSTYPE delpos[_MAX_EVENT_REGITEM] = { 0, };

			typedef util::SEQ_MASK<DWORD, -1, _MAX_EVENT_REGITEM>		REG_ITEM_CODE_ARRAY;
			POSTYPE delitemUB = 0;
			FOREACH_ARR_CONTAIN( idx, REG_ITEM_CODE_ARRAY, pEventInfo->m_dwRegisterItemIndex )
			{
				const SLOTCODE itemCode = (SLOTCODE)pEventInfo->m_dwRegisterItemIndex[idx];
				const POSTYPE itemCnt = (POSTYPE)pEventInfo->m_wRegisterItemNum[idx];

				if( !itemCnt )
					break;

				IfFailThenBreak(pItemManager->ExistItem(SI_INVENTORY, itemCode, itemCnt),
                                RC::RC_EVENT_NOTEXISTITEM);

				delItem[idx] = itemCode;
				delpos[idx] = itemCnt;
				++delitemUB;
			}

			if( flow.IsErrorOccured() )
				break;

			MSG_CG_ITEM_LOSE_ACK loseMsg;
			// ������ �� �����ϸ� �Ѳ����� �����ؾ� ��Ż�� ����
			IfFailThenBreak(
                RC::RC_ITEM_SUCCESS == pItemManager->DeleteAllKindOfItem(delitemUB, delItem, delpos,
                                                                         &loseMsg.m_ItemInfo),
				RC::RC_EVENT_NOTEXISTITEM);

			GAMELOG->LogMultiItem( ITEM_EVENT_ETHER_REG, pPlayer, delitemUB, delItem, delpos );

			pPlayer->SendPacket( &loseMsg, loseMsg.GetSize() );	// ��� �� �����۵� ����~

			// ��� ����ϸ� ���� Ƚ�� �ϳ� ����
			if( pEventStatus->m_wRegisterCnt < (WORD)-1 )
				++pEventStatus->m_wRegisterCnt;

			MSG_DG_EVENT_ITEM_REG_SYN updateMsg;

			// DBProxy�� ��ǰ ���� ��� ��û
			updateMsg.m_dwKey = pPlayer->GetUserGuid();
			updateMsg.m_dwCharGUID = pPlayer->GetCharGuid();

			{	// �Ϸ� ���� üũ
				const WORD registerLimit = (WORD)pEventInfo->m_byCountMax;
				pEventStatus->m_Status =
					( registerLimit && (registerLimit <= pEventStatus->m_wRegisterCnt) ) ? 1 : 0;
				updateMsg.m_EventStatus = (*pEventStatus);
			}

			pPlayer->SendToGameDBPServer( &updateMsg, sizeof(updateMsg) );
			MessageOut( eFULL_LOG, "Event Item Register[%u][C:%u][Event:%u/%u]",
				updateMsg.m_dwKey, updateMsg.m_dwCharGUID, eventID, pEventStatus->m_wRegisterCnt );

			return /*RETURN TRUE VALUE*/;
        }
        FLOWCONTROL_END;

		return /*RETURN FALSE VALUE*/;
	}

	//������������������������������������������������������������������������������������
	//
	static VOID		On_eEVENT_REGITEM_RANDREWARD( FlowControl& flow, Player* const pPlayer, EVENT_INFO* const pEventInfo )
	{
        if( !pPlayer->GetField() )
        {
            SUNLOG( eCRITICAL_LOG, "[Handler_CG_EVENT_IMPL::On_eEVENT_REGITEM_RANDREWARD] Not Exist GameField" );
            return;
        }

		IPlayerEventD& rEventD = pPlayer->PlayerEvent();
		const CODETYPE eventID = pEventInfo->m_dwEventID;

		FLOWCONTROL_START()
        {
			// start LOGIC_BLOCK
			// �ش� �̺�Ʈ�� ���� ����, ���� üũ - �����ϸ� ������ ����
			// ���� üũ
			ItemManager* const pItemManager = pPlayer->GetItemManager();

			// ����� �������� ����� ������ŭ �����ϴ��� �˻�
			SLOTCODE delItem[_MAX_EVENT_REGITEM] = { 0, };
			POSTYPE delpos[_MAX_EVENT_REGITEM] = { 0, };

			typedef util::SEQ_MASK<DWORD, -1, _MAX_EVENT_REGITEM>		REG_ITEM_CODE_ARRAY;
			POSTYPE delitemUB = 0;
			FOREACH_ARR_CONTAIN( idx, REG_ITEM_CODE_ARRAY, pEventInfo->m_dwRegisterItemIndex )
			{
				const SLOTCODE itemCode = (SLOTCODE)pEventInfo->m_dwRegisterItemIndex[idx];
				const POSTYPE itemCnt = (POSTYPE)pEventInfo->m_wRegisterItemNum[idx];

				if( !itemCnt )
					break;

				IfFailThenBreak(pItemManager->ExistItem(SI_INVENTORY, itemCode, itemCnt),
                                RC::RC_EVENT_NOTEXISTITEM);

				delItem[idx] = itemCode;
				delpos[idx] = itemCnt;
				++delitemUB;
			}

			if( flow.IsErrorOccured() )
				break;

			// ��� �������� ���ش�
			// ���� ���ְ� �ִٰ� �����ϸ� �־��� ��� �������� �������Ƿ� �����ϸ� ������
			// �ְ� ���� ���ִٰ� �����ϸ� �־��� ��� ��¥�� �������� �Ѱ��ֹǷ� ������ �˾Ƴ��� �� ���������
			MSG_CG_ITEM_LOSE_ACK loseMsg;
			IfFailThenBreak(
				RC::RC_ITEM_SUCCESS == pItemManager->DeleteAllKindOfItem(delitemUB, delItem, delpos,
                                                                         &loseMsg.m_ItemInfo),
				RC::RC_EVENT_NOTEXISTITEM);

			GAMELOG->LogMultiItem( ITEM_EVENT_ETHER_DELETE, pPlayer, delitemUB, delItem, delpos );

			// ���� ������ ȹ�濡 ���� Ȯ�� ���� - �ִ� �͵鸸ŭ �ش� �������� �� Ȯ���� �ٲ��. 1���� 100%, 2���� 50%��, 3���� 33.3333...%
			DWORD rewardCnt = 0;
			FOREACH_ARR_CONTAIN( idx, REG_ITEM_CODE_ARRAY, pEventInfo->m_dwRewardInfo )
			{
				++rewardCnt;
			}

			IfFailThenBreak(rewardCnt != 0, RC::RC_EVENT_FAILED);

			// ���� ������ �ش�!
			const DWORD nRandIndex = (DWORD)rand() % rewardCnt;	// �ִ� ������ ������ŭ ���� ����
			const CODETYPE randRewardIndex = pEventInfo->m_dwRewardInfo[nRandIndex];

			MSG_CG_EVENT_ITEM_REWARD_CMD rewardMsg;
			IfFailThen(
				RC::RC_REWARD_SUCCESS == g_RewardManager.RewardItem(RewardManager::eREWARD_EVENT,
                    randRewardIndex, 1, pPlayer, rewardMsg.m_TotalInfo), RC::RC_EVENT_CANT_OBTAIN)
			{
				INVENTORY_TOTAL_INFO tempTotalInfo;	// �ӽ� ����
				// (WAVERIX) (090610) (CHANGES) ���� ó��
			    /*RC::RC_ITEM_NOTEXIST_ITEM != */
                pPlayer->GetItemManager()->ObtainAllKindOfItem(delitemUB, delItem, delpos,
                                                               &tempTotalInfo);
				break; //�̰� �ʿ� ������...
			}

			if( flow.IsErrorOccured() )
				break;

			pPlayer->SendPacket( &loseMsg, loseMsg.GetSize() );	// ������ ����߰�

			// ���� ������ Ȯ��
			SLOTCODE* pRewardCodes = g_RewardManager.GetRewardItemCode();
			IfFailThenBreak(pRewardCodes, ERRCODE_RANDREWARD);

			rewardMsg.m_dwEventID = eventID;
			rewardMsg.m_Money = pPlayer->GetMoney();

			pPlayer->SendPacket( &rewardMsg, rewardMsg.GetSize() );	// ��ϵ����� ������ �޾ƶ�!

		#define _WHATTHEHELL_ITEMREWARD_DBREG_20060603
		#ifdef _WHATTHEHELL_ITEMREWARD_DBREG_20060603	// ������ ������� �͵� DB�� ���
			EVENT_STATUS_EX* pEventStatus = rEventD.FindEventStatus( eventID );
			if( NULL == pEventStatus )
				_RegisterEventStatus( eventID, pPlayer, &pEventStatus );

			// �ִ밪 üũ
			if( pEventStatus->m_wRegisterCnt < (WORD)-1 )
				++pEventStatus->m_wRegisterCnt;

			// DBProxy�� ������ ��ϵ����� �˷��ش�
			MSG_DG_EVENT_ITEM_REG_SYN updateMsg;

			// DBProxy�� ��ǰ ���� ��� ��û
			updateMsg.m_dwKey = pPlayer->GetUserGuid();
			updateMsg.m_dwCharGUID = pPlayer->GetCharGuid();

			{	// �Ϸ� ���� üũ
				const WORD registerLimit = (WORD)pEventInfo->m_byCountMax;
				pEventStatus->m_Status = 0;	// ����� �Ϸᰡ ���ܴ�...
					//( registerLimit && (registerLimit <= pEventStatus->m_wRegisterCnt) ) ? 1 : 0;
				updateMsg.m_EventStatus = (*pEventStatus);
			}

			pPlayer->SendToGameDBPServer( &updateMsg, sizeof(updateMsg) );
		#endif
			MessageOut( eFULL_LOG, "Event Item Register And Reward[%u][C:%u][E:%u]",
				updateMsg.m_dwKey, updateMsg.m_dwCharGUID, eventID );

			return /*RETURN TRUE VALUE*/;
        }
        FLOWCONTROL_END;
		
		return /*RETURN FALSE VALUE*/;
	}
};

//������������������������������������������������������������������������������������
//	<MSG_CG_EVENT_ITEM_REG_SYN>
//	-	�̺�Ʈ ������ ���
//
Handler_CG_EVENT_IMPL( CG_EVENT_ITEM_REG_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	const MSG_CG_EVENT_ITEM_REG_SYN* pMsg = (const MSG_CG_EVENT_ITEM_REG_SYN*)pRecvMsg;

	if( CHECK_IS_NOT_FIELDSERVER() )
		return;

	const CODETYPE eventID = pMsg->m_dwEventID;
	const DWORD userKey = pMsg->m_dwKey;
	Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( userKey );
    if (NULL == pPlayer)
        return;
    
		if( !pPlayer->GetField() )
		{
			SUNLOG( eCRITICAL_LOG, "[Handler_CG_EVENT_IMPL( CG_EVENT_ITEM_REG_SYN )] Not Exist GameField" );
			return;
		}

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(pPlayer && pPlayer->GetField(),
                        SubHandler_for_CG_EVENT_ITEM_REG_SYN::ERRCODE_DEFAULT);

		IfFailThenBreak(pPlayer->GetChaoState().CanUseFuction(CHAO_USE_EVENT),
                        RC::RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE);
		IfFailThenBreak(pPlayer->GetBehave() != PLAYER_BEHAVE_UNEXPECTED_WAITING, RC::RC_EVENT_FAILED);

		SunEventInfoParser* const pSunEventInfoParser = SunEventInfoParser::Instance();
		EVENT_INFO* const pEventInfo = pSunEventInfoParser->FindEventInfo( eventID );
		IfFailThenBreak(pEventInfo, RC::RC_EVENT_INVALID_EVENT);

		// ...�̰� 99.9% ���� �� ���̴�. (EventManager ���� ��)
		IfFailThenBreak(pEventInfo->CheckEvent(), RC::RC_EVENT_INVALID_EVENT);

		IfFailThenBreak(pEventInfo->CheckLevel(pPlayer->GetLevel()), RC::RC_EVENT_UNQUALIFIED);

		{	// Check Inventory Constraint
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			IfFailThenBreak(rPROTECT.IsUsable(), RC::RC_EVENT_ITEM_INVENTORY_LOCKED);
		};

		IPlayerEventD& rEventD = pPlayer->PlayerEvent();

		const eEVENT_MODE eventMode = (eEVENT_MODE)pEventInfo->m_byEventMode;
		switch( eventMode )
		{
		case eEVENT_REGITEM_GETITEM:	//��ǰ ����
			SubHandler_for_CG_EVENT_ITEM_REG_SYN::On_eEVENT_REGITEM_GETITEM( flow, pPlayer, pEventInfo );
			break;
		case eEVENT_REGITEM_RANDREWARD:	// �����ϰ� ������ ����
			SubHandler_for_CG_EVENT_ITEM_REG_SYN::On_eEVENT_REGITEM_RANDREWARD( flow, pPlayer, pEventInfo );
			break;
		}

		if( flow.IsErrorOccured() )
			break;

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	switch( const DWORD errcode = flow.GetErrorCode() )
	{
	case RC::RC_EVENT_FAILED:
	case RC::RC_EVENT_INVALID_EVENT:
	case RC::RC_EVENT_ITEM_INVENTORY_LOCKED:
	case RC::RC_EVENT_ITEM_NUM_OVERFLOW:
	case RC::RC_EVENT_NOTEXISTITEM:
	case RC::RC_EVENT_UNQUALIFIED:
		{
			MSG_CG_EVENT_ITEM_REG_NAK msgNAK;
			msgNAK.m_dwEventID = eventID;
			msgNAK.m_dwErrorCode = errcode;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
		break;
	case SubHandler_for_CG_EVENT_ITEM_REG_SYN::ERRCODE_DEFAULT:
	case SubHandler_for_CG_EVENT_ITEM_REG_SYN::ERRCODE_RANDREWARD:
	default:
		SUNLOG( eCRITICAL_LOG, _T("[CG_EVENT_ITEM_REG_SYN] Unexpected Status [%u]"), errcode );
		break;
	}

	return /*RETURN FALSE VALUE*/;
}

//
//������������������������������������������������������������������������������������


Handler_CG_EVENT_IMPL( CG_EVENT_TIME_PERIOD_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_EVENT_TIME_PERIOD_SYN* pMsg = (MSG_CG_EVENT_TIME_PERIOD_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );

	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	time_t t;
	util::TimeSync::time(&t);

	MSG_CG_EVENT_TIME_PERIOD_ACK arkMsg(t);
	pPlayer->SendPacket( &arkMsg, sizeof(arkMsg) );
}

Handler_CG_EVENT_IMPL( CG_EVENT_QUERY_EXIST_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_EVENT_QUERY_EXIST_SYN* pMsg = (MSG_CG_EVENT_QUERY_EXIST_SYN*)pRecvMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !(pPlayer && pPlayer->GetField()) )
		return;

	RC::eITEM_RESULT rt = pPlayer->GetEventSlotContainer()->Open();
	if( rt == RC::RC_ITEM_EVENT_SUCCESS /*RC::RC_EVENT_SUCCESS*/ )
	{
		MSG_DG_EVENT_QUERY_EXIST_SYN msgSYN;
		//msg.m_UserGuid = pPlayer->GetUserGuid();
		pPlayer->SendToGameDBPServer( &msgSYN, sizeof(msgSYN) );
	}
	else
	{
		MSG_CG_EVENT_QUERY_EXIST_ACK msgACK;
		msgACK.m_bExist = MSG_CG_EVENT_QUERY_EXIST_ACK::DISABLED;
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
	}
}



//PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_REQUEST_ITEM_SYN )
//	CODETYPE				m_EventID;
//	BYTE					m_RequestItemIndex;
//
//	MSG_CG_EVENT_REQUEST_ITEM_SYN( const CODETYPE eventID, const BYTE requestItemIndex )
//	: MSG_CG_EVENT_REQUEST_ITEM_SYN(), m_EventID(eventID), m_RequestItemIndex(requestItemIndex) {}
//PACKET_CG_FWD_CLOSE
//
//PACKET_CG_FWD_OPEN( CG_EVENT, CG_EVENT_REQUEST_ITEM_NAK )
//	CODETYPE				m_EventID;
//	DWORD					m_ErrorCode;
//
//	inline VOID				SetInfo( const CODETYPE eventID, DWORD errorCode ) { m_EventID=eventID; m_ErrorCode = errorCode; }
//PACKET_CG_FWD_CLOSE
//

//
//	<Desc> ������ �ٸ� ü��. �ٷ� ���� �޴� ���´�.
Handler_CG_EVENT_IMPL( CG_EVENT_REQUEST_ITEM_SYN )	// ������ CG_EVENT_ITEM_REWARD_CMD
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	const MSG_CG_EVENT_REQUEST_ITEM_SYN* pMsg = (MSG_CG_EVENT_REQUEST_ITEM_SYN*)pRecvMsg;

	if( CHECK_IS_NOT_FIELDSERVER() )
		return;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
    if (NULL == pPlayer)
        return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_EVENT_IMPL( CG_EVENT_REQUEST_ITEM_SYN )] Not Exist GameField" );
		return;
	}

	DWORD eventID = 0;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		if( FALSE == flow.FCAssert( pPlayer && pPlayer->GetField() ) )
		{
			SetFailFc(pPlayer, 1000);
			break;
		}

		IfFailThenBreak(pPlayer->GetChaoState().CanUseFuction(CHAO_USE_EVENT),
                        RC::RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE);

		IfFailThenBreak(pPlayer->GetBehave() != PLAYER_BEHAVE_UNEXPECTED_WAITING, RC::RC_EVENT_FAILED);
		IfFailThenBreak(!pPlayer->EventTransaction().IsTransaction(), 1100);

		sEventTransactionWrap autoTrans(pPlayer->EventTransaction());

		eventID = pMsg->m_EventID;
		SunEventInfoParser* pEventParser = SunEventInfoParser::Instance();
		EVENT_INFO* pEventInfo = pEventParser->FindEventInfo( eventID );
		IfFailThenBreak(pEventInfo, RC::RC_EVENT_INVALID_EVENT);

		const DWORD eventMode = pEventInfo->m_byEventMode;
		IfFailThenBreak(eEVENT_ITEM_SELECT == eventMode && pEventInfo->CheckEvent(),
                        RC::RC_EVENT_INVALID_EVENT);

		// �̺�Ʈ ���� �ʿ�
		IfFailThenBreak(pEventInfo->CheckLevel(pPlayer->GetLevel()), RC::RC_EVENT_UNQUALIFIED);

		EVENT_STATUS_EX* pEventStatus = pPlayer->PlayerEvent().FindEventStatus( eventID );
		if( NULL == pEventStatus )
			_RegisterEventStatus( eventID, pPlayer, &pEventStatus );

		IfFailThenBreak(0 == pEventStatus->m_wRegisterCnt, RC::RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW);

		const BYTE requestIndex = pMsg->m_RequestItemIndex;
		const CODETYPE requestRewardIndex = pEventInfo->m_dwRewardInfo[0];
		IfFailThenBreak(_MAX_EVENT_REWARD > requestIndex && requestRewardIndex,
                        RC::RC_EVENT_ITEM_NUM_OVERFLOW);

		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		IfFailThenBreak(rPROTECT.IsUsable(), RC::RC_EVENT_ITEM_INVENTORY_LOCKED);

		//	������ ȹ��
		//		���� ���� (�Ϲ����� ����, �����ǰ� ���� �κ������� �ߺ� ��� ����, ~~)
		MSG_CG_EVENT_ITEM_REWARD_CMD msgREWARD;
		const RC::eREWARD_RESULT eRET = g_RewardManager.RewardItem(
														RewardManager::eREWARD_EVENT,
														requestRewardIndex,
														(1<<requestIndex), pPlayer,
														msgREWARD.m_TotalInfo
														);
		IfFailThenBreak(eRET == RC::RC_REWARD_SUCCESS,
                        RC::RC_EVENT_CANT_OBTAIN/*MSG_CG_EVENT_REQUEST_ITEM_NAK::_ERR_CANT_OBTAIN*/);

		msgREWARD.m_dwEventID = pMsg->m_EventID;
		msgREWARD.m_Money = pPlayer->GetMoney();
		pPlayer->SendPacket( &msgREWARD, msgREWARD.GetSize() );	// ��ϵ����� ������ �޾ƶ�!

		// SendToDB - ITEM_STATUS_REG (Heim, RegCnt~~)
		pEventStatus->m_wRegisterCnt = 1;	// �Ϸ�, 1���� ��û�Ѵ�.
		pEventStatus->m_Status = 1;

		MSG_DG_EVENT_STATUS_UPDATE_SYN msgSYN;
		msgSYN.m_EventStatus = *pEventStatus;
		pPlayer->SendToGameDBPServer( &msgSYN, sizeof(msgSYN) );

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	switch( flow.GetErrorCode() )
	{
	case RC::RC_EVENT_FAILED:
	case RC::RC_EVENT_INVALID_EVENT:		// �̺�Ʈ ���� �ƴϴ�
	case RC::RC_EVENT_ITEM_NUM_OVERFLOW:	// �̺�Ʈ ������ ��� ���� ������ �Ѿ���
	case RC::RC_EVENT_NOTEXISTITEM:		// �̺�Ʈ�� ������ �������� ����
	case RC::RC_EVENT_CANT_OBTAIN:		// ���� �������� ���� ���� ����
	case RC::RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW:
	case RC::RC_EVENT_ITEM_INVENTORY_LOCKED:
	case RC::RC_EVENT_UNQUALIFIED:
		{
			MSG_CG_EVENT_REQUEST_ITEM_NAK msgNAK;
			msgNAK.SetInfo( (CODETYPE)eventID, flow.GetErrorCode() );
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		};
		break;
	case 1000: break;	// fault-assertion
	case 1100:
		SUNLOG( eCRITICAL_LOG, _T("Irregular C/S Packet Policy [%u] %s"), pPlayer->GetCharGuid(), pPlayer->GetCharName() );
		break;
	default:
		SUNLOG( eCRITICAL_LOG, _T("Unhandled Error Occured! %u"), flow.GetErrorCode() );
		break;
	}
	return /*RETURN FALSE VALUE*/;
}

//	<MSG_CG_EVENT_DONATION_HEIM_SYN>
//	<returns>
//		MSG_CG_EVENT_DONATION_HEIM_ACK
//		MSG_CG_EVENT_ITEM_REWARD_CMD	<- Ư�� ���� ������ (Ư�� ���� �ּҰ�<=curheims<=�ִ밪, ȸ�� �̳�) ������ ����
Handler_CG_EVENT_IMPL( CG_EVENT_DONATION_HEIM_SYN )	// ��� ��û
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	const MSG_CG_EVENT_DONATION_HEIM_SYN* pMsg = (MSG_CG_EVENT_DONATION_HEIM_SYN*)pRecvMsg;

	if( CHECK_IS_NOT_FIELDSERVER() )
		return;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
    if (NULL == pPlayer)
        return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_EVENT_IMPL( CG_EVENT_DONATION_HEIM_SYN )] Not Exist GameField" );
		return;
	}

	const DWORD eventID = pMsg->m_EventID;
	MONEY donateHeims = 0;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		if( FALSE == flow.FCAssert( pPlayer && pPlayer->GetField() ) )
		{
			SetFailFc( pPlayer, 1000 );
			break;
		}

		IfFailThenBreak(pPlayer->GetChaoState().CanUseFuction(CHAO_USE_EVENT),
                        RC::RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE);
		IfFailThenBreak(pPlayer->GetBehave() != PLAYER_BEHAVE_UNEXPECTED_WAITING, RC::RC_EVENT_FAILED);
		IfFailThenBreak(!pPlayer->EventTransaction().IsTransaction(), 1100);

		//	Player* �� �̺�Ʈ Ʈ�������� DB�� �ٳ�;� �ϴ� ��� ����� �����̱� �ѵ�, �ʿ� ���� ���� �ִ�.
		//	���� ��츦 ���� �� ��.
		sEventTransactionWrap autoTrans(pPlayer->EventTransaction());

		SunEventInfoParser* pEventParser = SunEventInfoParser::Instance();
		EVENT_INFO* pEventInfo = pEventParser->FindEventInfo( eventID );
		IfFailThenBreak(pEventInfo, RC::RC_EVENT_FAILED);

		const DWORD eventMode = pEventInfo->m_byEventMode;
		IfFailThenBreak(eEVENT_DONATION_HEIM == eventMode && pEventInfo->CheckEvent(),
                        RC::RC_EVENT_FAILED);

		IfFailThenBreak(pEventInfo->CheckLevel(pPlayer->GetLevel()), RC::RC_EVENT_UNQUALIFIED);

		donateHeims = pMsg->m_DonatedHeims;
		IfFailThenBreak(pPlayer->GetMoney() >= donateHeims, RC::RC_EVENT_INSUFFICIENT_HEIM);

		// �̺�Ʈ ���� �ʿ�
		EVENT_STATUS_EX* pEventStatus = pPlayer->PlayerEvent().FindEventStatus( eventID );
		if( NULL == pEventStatus )
			_RegisterEventStatus( eventID, pPlayer, &pEventStatus );

		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		IfFailThenBreak(rPROTECT.IsUsable(), RC::RC_EVENT_ITEM_INVENTORY_LOCKED);

		// ��� �ݾ� ������ ���� ������ ���� ó�� ���, MONEY->DWORD ��ȯ�� �����ϰ� �ִ�. ��� �������� ���ܵд�.
		sEVENT_MINMAX_QUERY query( pEventInfo->m_EventRewardCode, pEventStatus->m_CurDonatedHeims, donateHeims, pEventInfo, pEventStatus->m_DonateHeimRewardCnt, pEventStatus->m_wRegisterCnt );
		sEVENT_MINMAX_RESULT rResult;
		pEventParser->FindMinMaxInfoByBoundary( query, rResult );

		if( sEVENT_MINMAX_RESULT::CNTL_FAIL == rResult.m_Result )
		{
			SetFailFc( sEVENT_MINMAX_RESULT::CNTL_FAIL != rResult.m_Result,
				rResult.m_ResultCode ? rResult.m_ResultCode : RC::RC_EVENT_FAILED );
			break;
		}

		if( sEVENT_MINMAX_RESULT::CNTL_SUCCESS == rResult.m_Result )
			IfFailThenBreak(rResult.m_DropCode, 1200);

		// �� ����, �� ���ķ� ���� ���� �� Ȯ���� ��, ���н� ���� ó��.
		IfFailThenBreak(pPlayer->MinusMoney(donateHeims), RC::RC_EVENT_FAILED);

		if( sEVENT_MINMAX_RESULT::CNTL_UPDATE == rResult.m_Result )
		{	// ��θ� ������
			// update event status 
			pEventStatus->m_wRegisterCnt = (WORD)rResult.m_TotalRounds;
			pEventStatus->m_DonateHeimRewardCnt = (BYTE)rResult.m_RewaredCount;
			pEventStatus->m_CurDonatedHeims = rResult.m_CalcedDonatedMoney;

			MSG_CG_EVENT_DONATION_HEIM_ACK msgACK;
			msgACK.SetInfo( eventID, RC::RC_EVENT_SUCCESS_HEIM_DONATION );
			msgACK.SetTotalHeim( pPlayer->GetMoney() );
			pPlayer->SendPacket( &msgACK, sizeof(msgACK) /*msgACK.GetSize()*/ );

			// SendToDB - ITEM_STATUS_REG (Heim, RegCnt~~)
			MSG_DG_EVENT_STATUS_UPDATE_SYN msgSYN;
			msgSYN.m_EventStatus = *pEventStatus;
			pPlayer->SendToGameDBPServer( &msgSYN, sizeof(msgSYN) );

			return;
		}

		//	sEVENT_MINMAX_RESULT::CNTL_SUCCESS ...
		//
		//	���� ����... ���⼭������ ������ ERR_CANT_OBTAIN_ITEM�μ� �� ���� ����ó���� ����� �Ѵ�.
		MSG_CG_EVENT_ITEM_REWARD_CMD msgREWARD;

		DropObj* pDropObj = DropRatioInfoList::Instance()->PopRandomItemTypeIndex( rResult.m_DropCode );
		if( FALSE == flow.FCAssert( pDropObj && DROP_OBJ_ITEM==pDropObj->GetType() ) )
		{
			SetFailFc( pDropObj, RC::RC_EVENT_CANT_OBTAIN );
			break;
		}

		DropItemObj* pDropItemObj = static_cast<DropItemObj*>( pDropObj );
		SLOTCODE itemCode = (SLOTCODE)pDropItemObj->GetItemCode();
		POSTYPE itemNum = 1;

		//	������ ȹ��
		//		���� ���� (�Ϲ����� ����, ~~)
		if( RC::RC_ITEM_SUCCESS != pPlayer->GetItemManager()->ObtainAllKindOfItem(
																	1, &itemCode, &itemNum,
																	&msgREWARD.m_TotalInfo
																	) )
		{
			SetFailFc( pDropObj, RC::RC_EVENT_CANT_OBTAIN );
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		//
		msgREWARD.m_dwEventID = pMsg->m_EventID;
		msgREWARD.m_Money = pPlayer->GetMoney();
		pPlayer->SendPacket( &msgREWARD, msgREWARD.GetSize() );	// ��ϵ����� ������ �޾ƶ�!

		// update event status 
		pEventStatus->m_wRegisterCnt = (WORD)rResult.m_TotalRounds;
		pEventStatus->m_DonateHeimRewardCnt = (BYTE)rResult.m_RewaredCount;
		pEventStatus->m_CurDonatedHeims = rResult.m_CalcedDonatedMoney;

		// SendToDB - ITEM_STATUS_REG (Heim, RegCnt~~)
		MSG_DG_EVENT_STATUS_UPDATE_SYN msgSYN;
		msgSYN.m_EventStatus = *pEventStatus;
		pPlayer->SendToGameDBPServer( &msgSYN, sizeof(msgSYN) );

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	switch( flow.GetErrorCode() )
	{
	case RC::RC_EVENT_FAILED:						// �⺻ ����,
	case RC::RC_EVENT_INSUFFICIENT_HEIM:			// ���� ����
	case RC::RC_EVENT_DONATION_HEIM_LIMIT:			// �ּ� ��� ���� ���� ����
	case RC::RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW:	// �� �̻� ���� ���� �� ����.
	case RC::RC_EVENT_ITEM_INVENTORY_LOCKED:
	case RC::RC_EVENT_UNQUALIFIED:
		{
			MSG_CG_EVENT_DONATION_HEIM_ACK msgFAIL;
			msgFAIL.SetInfo( (CODETYPE)eventID, (RC::eEVENT_RESULT)flow.GetErrorCode() );
			msgFAIL.SetTotalHeim( pPlayer->GetMoney() );
			pPlayer->SendPacket( &msgFAIL, sizeof(msgFAIL) /*msgFAIL.GetSize()*/ );
		};
		break;
	case RC::RC_EVENT_CANT_OBTAIN:
		{
			// Heim �� �̸� �����Ǿ� �־��� ����� ������ ����� �Ѵ�.
			pPlayer->PlusMoney( donateHeims );

			MSG_CG_EVENT_DONATION_HEIM_ACK msgFAIL;
			msgFAIL.SetInfo( (CODETYPE)eventID, (RC::eEVENT_RESULT)flow.GetErrorCode() );
			msgFAIL.SetTotalHeim( pPlayer->GetMoney() );
			pPlayer->SendPacket( &msgFAIL, sizeof(msgFAIL) /*msgFAIL.GetSize()*/ );
		}
		break;
	case 1000: break;	// fault-assertion
	case 1100:
		SUNLOG( eCRITICAL_LOG, _T("Irregular C/S Packet Policy [%u] %s"), pPlayer->GetCharGuid(), pPlayer->GetCharName() );
		break;
	case 1200:
		SUNLOG( eCRITICAL_LOG, _T("[ErrorCode:%u] {%s}"), flow.GetErrorCode(), flow.GetErrorString() );
		break;
	default:
		SUNLOG( eCRITICAL_LOG, _T("Unhandled Error Occured! %u"), flow.GetErrorCode() );
		break;
	}
	return /*RETURN FALSE VALUE*/;
}


#include <SkillInfoParser.h>
#include "SkillManager.h"
//	<CG_EVENT_BUFF_EVENT_SYN>
//	<returns>
//	-	CG_EVENT_BUFF_EVENT_ACK
//	-		RC_EVENT_SUCCESS	= ������
//	-		RC_EVENT_XXX		= ���� + �� ��ʺ� + Ư�� ��ʴ� �� ������ �߰��� �ٴ´�.
//	<NOTE>
//	-	�̰��� ����Ǹ鼭 ��� �̺�Ʈ�� ������ ���� ������ ���ԵǾ�� �Ѵ�.
//	-	RC_EVENT_UNQUALIFIED	= ���� (���� ������ �ƴϴ�.)
Handler_CG_EVENT_IMPL(CG_EVENT_BUFF_EVENT_SYN )			// ���� �̺�Ʈ ��û
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	const DWORD		ERRCODE_DEFAULT			= 1000;
	const DWORD		ERRCODE_TRANSACTION		= ERRCODE_DEFAULT+1;

	const MSG_CG_EVENT_BUFF_EVENT_SYN* pMsg = (const MSG_CG_EVENT_BUFF_EVENT_SYN*)pRecvMsg;

	if( CHECK_IS_NOT_FIELDSERVER() )
		return;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
    if (NULL == pPlayer)
        return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_EVENT_IMPL( CG_EVENT_BUFF_EVENT_SYN )] Not Exist GameField" );
		return;
	}

	typedef MSG_CG_EVENT_BUFF_EVENT_ACK::eRET_UNQUALIFIED		eRET_UNQUALIFIED;
	eRET_UNQUALIFIED addinInfo = MSG_CG_EVENT_BUFF_EVENT_ACK::RUC_EMPTY; // passing info for Error
	CODETYPE eventID = 0;	// passing info for Error

	const MONSTERCODE recvedNpcCode = pMsg->m_NpcCode;

    // (CHANGES) (100122) (WAVERIX) make packet with clear stream
    char __buffer_block[sizeof(MSG_CG_EVENT_BUFF_EVENT_ACK)] = { 0, };
	MSG_CG_EVENT_BUFF_EVENT_ACK& msgACK = *new (__buffer_block) MSG_CG_EVENT_BUFF_EVENT_ACK;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(pPlayer && pPlayer->GetField() && sizeof(*pMsg) == wSize, ERRCODE_DEFAULT);
		IfFailThenBreak(pPlayer->GetChaoState().CanUseFuction(CHAO_USE_EVENT),
                        RC::RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE);
		IfFailThenBreak(pPlayer->GetBehave() != PLAYER_BEHAVE_UNEXPECTED_WAITING, RC::RC_EVENT_FAILED);
		IfFailThenBreak(!pPlayer->EventTransaction().IsTransaction(), ERRCODE_TRANSACTION);

		//	Player* �� �̺�Ʈ Ʈ�������� DB�� �ٳ�;� �ϴ� ��� ����� �����̱� �ѵ�, �ʿ� ���� ���� �ִ�.
		//	���� ��츦 ���� �� ��.
		sEventTransactionWrap autoTrans(pPlayer->EventTransaction());

		EventManager* const pEventManager = EventManager::GetEventManager();
		SunEventInfoParser* const pEventParser = SunEventInfoParser::Instance();

		typedef EventQueryResult::BuffEventIDs		BuffEventIDs;
		BuffEventIDs queryResult;
		const BOOLEAN querySuccess = pEventManager->GetBuffEventIDbyNpcID( recvedNpcCode, &queryResult );
		IfFailThenBreak(querySuccess, RC::RC_EVENT_INVALID_EVENT);

		DWORD successCount = 0;
		DWORD lastError = RC::RC_EVENT_SUCCESS;
		BYTE count = queryResult.Count;
        FOREACH_ARRAY(const BuffEventIDs::ARRAY::type eventID, queryResult.List, BuffEventIDs::ARRAY)
		{
			if( !(count--) )
				break;

			if( !eventID )
				continue;

			EVENT_INFO* const pEventInfo = pEventParser->FindEventInfo( eventID );
			if( !( pEventInfo && pEventInfo->CheckEvent() ) )
			{ lastError = RC::RC_EVENT_INVALID_EVENT; continue; }

			const DWORD eventMode = pEventInfo->m_byEventMode;
			if( (eEVENT_BUFF_GRANT != eventMode) )
			{ lastError = RC::RC_EVENT_FAILED; continue; }

			//	<THINKING NOTE>
			//	-	1. BUFF�� �������ִ� ó�� ( eEVENT_BUFF_GRANT�� ���, rewardinfo[0]�� skill������ �Էµȴ�. )
			//	-		1) applicable level check
			//	-	2. �ߺ� ���� ����
			//	-		1) assert( applied buff count == 1)
			if( FALSE == pEventInfo->CheckLevel( pPlayer->GetLevel() ) )
			{ lastError = RC::RC_EVENT_UNQUALIFIED; continue; }
			// ���Ľɿ��� CheckEvent�� ����Ǿ���. �̺�Ʈ ���ǿ� �����ϰ� ���� ���̴�.

			// ���� ������ �ش�!
			const DWORD FIXED_SELECT_REWARD_INDEX = 0;		
		
			const DWORD skillInData = (pPlayer->CanApplyPCBangBenefit(true) == false)
				?	pEventInfo->m_dwRewardInfo[FIXED_SELECT_REWARD_INDEX]
				:	pEventInfo->m_dwPCRoomRewardInfo[FIXED_SELECT_REWARD_INDEX];
		
			const SLOTCODE rewardSkill = SAFE_NUMERIC_TYPECAST(DWORD, skillInData, SLOTCODE) ? (SLOTCODE)skillInData : 0;
			IfFailThenBreak(rewardSkill, RC::RC_EVENT_FAILED);

			SkillInfoParser* const pSkillInfoParser = SkillInfoParser::Instance();

			// ���ο� ��ų�� ���� �޸𸮸� �Ҵ�ް� ��ų������ �����Ѵ�.
			SkillScriptInfo* const pBaseSkillInfo = pSkillInfoParser->GetSkillInfo( rewardSkill );
			IfFailThenBreak(pBaseSkillInfo, RC::RC_EVENT_FAILED);

			WzVector wvCurPos;	pPlayer->GetPos(&wvCurPos);

            SkillInfo skill_info_data(pPlayer, rewardSkill, 0);
            skill_info_data.SetArgsObjectInfo(pPlayer->GetObjectKey(), &wvCurPos, &wvCurPos, &wvCurPos);

			pPlayer->GetSkillManager()->RegisterSkill( SKILL_TYPE_ACTIVE, &skill_info_data);

			msgACK.SetInfo_SUCCESS( eventID );
			pPlayer->SendPacket( &msgACK, msgACK.GetSize() );

			++successCount;
		}

		if( 0 == successCount )
		{
			SetFailFc( lastError, lastError );
			break;
		}
		return; // SUCCESS
    }
    FLOWCONTROL_END;

	switch( const DWORD errCode = flow.GetErrorCode() )
	{
	case RC::RC_EVENT_FAILED:						// (����:��Ÿ ������ ���н�)
		msgACK.SetInfo_FAIL();
		break;
	case RC::RC_EVENT_INVALID_EVENT:				// (����)
		msgACK.SetInfo_INVALID();
		break;
	case RC::RC_EVENT_CANT_BUFF_GRANTED:			// (�ű�:�̹� �ο��Ǿ� ���� ���)
		msgACK.SetInfo_CANT_BUFF_GRANTED( eventID );
		break;
	case RC::RC_EVENT_UNQUALIFIED:					// (�ű�:������ ���� �� ����. �ڰ��� �ȵǼ�) + �߰� ���� ���� �ʿ�
		msgACK.SetInfo_UNQUALIFIED( eventID, addinInfo );
		break;
	case RC::RC_EVENT_SUCCESS:
		break;
    case RC::RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE:
        // skip logging because of too many logged. average = { 1hit/2sec }
        break;
	case ERRCODE_DEFAULT:			// fault-assertion
	case ERRCODE_TRANSACTION:
	default:
		SUNLOG( eCRITICAL_LOG, _T("[ErrorCode:%u] {%s}:[%u]"), flow.GetErrorCode(), flow.GetErrorString(), wSize );
		return;
	}

	pPlayer->SendPacket( &msgACK, msgACK.GetSize() );

	return /*RETURN FALSE VALUE*/;
}

Handler_CG_EVENT_IMPL(CG_EVENT_INVITATEFRIEND_REWARD_SYN )
{
    GFriendInvitationController::Instance()->OnRewardSYN(pServerSession, pRecvMsg, wSize);
}

Handler_CG_EVENT_IMPL(CG_EVENT_INVITATEFRIEND_RECALL_SYN )
{
    GFriendInvitationController::Instance()->OnRecallSYN(pServerSession, pRecvMsg, wSize);
}

Handler_CG_EVENT_IMPL(CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_ACK )
{
    GFriendInvitationController::Instance()->OnRecallQuestionACK(pServerSession, pRecvMsg, wSize);
}

Handler_CG_EVENT_IMPL(CG_EVENT_INVITATEFRIEND_RECALL_QUESTION_NAK )
{
    GFriendInvitationController::Instance()->OnRecallQuestionNAK(pServerSession, pRecvMsg, wSize);
}

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC 
//���� Ÿ�԰� mapcode�� �ް� DB�� ����� �ݾ� Ȯ�� ��û
//���� ��ǥ ���� ������ �κ��丮 ����Ȯ�ι� Rock�� Ȯ���Ͽ� DB�� ��û
Handler_CG_EVENT_IMPL(CG_EVENT_DOMINATION_REWARD_CMD )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_CG_EVENT_DOMINATION_REWARD_CMD* recv_msg = 
        static_cast<MSG_CG_EVENT_DOMINATION_REWARD_CMD*>(pRecvMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return;
    }

    eSYSTEM_MEMO_TYPE recv_reward_type;
    MAPCODE recv_map_code;

    //--�޸� ���� �˻�
    
    if (player->GetPostManager()->GetValueInMemo(recv_msg->memo_subject_, 
                                                 recv_reward_type, recv_map_code))
    {
        MSG_DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN send_msg;

        send_msg.recv_char_guid_ = player->GetCharGuid();
        send_msg.reward_area_mapcode_ = recv_map_code;
        send_msg.reward_type_ = recv_reward_type;
        send_msg.memo_guid_ = recv_msg->memo_guid_;

        g_pGameServer->SendToServer(GAME_DBPROXY, &send_msg, sizeof(send_msg));
    }
    else
    {
        assert(!"Request SystemMemo Reward Value Is Incorrect");
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]User[%u] SystemMemo[%s] Is Incorrect", 
            player->GetCharGuid(),recv_msg->memo_subject_);
    }
}
//�÷��̾������� �ζ� index������ DB���� �� �ݾ� Ȯ�� ��û
Handler_CG_EVENT_IMPL(CG_EVENT_LOTTO_REWARD_CMD )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_CG_EVENT_LOTTO_REWARD_CMD* recv_msg =     
        static_cast<MSG_CG_EVENT_LOTTO_REWARD_CMD*>(pRecvMsg);

    const Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return;
    }

    MSG_DG_EVENT_LOTTO_REWARD_AND_DEL_SYN send_msg;
    send_msg.request_char_guid_ = player->GetCharGuid();
    send_msg.lotto_entry_index_ = recv_msg->lotto_entry_index_;
    send_msg.memo_guid_ = recv_msg->memo_guid_;

    g_pGameServer->SendToServer(GAME_DBPROXY, &send_msg, sizeof(send_msg));
}
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
//�ý��� �޸� GUID�� �ش� �ɸ��� GUID������ DBP���� ���� �������� Ȯ���Ѵ�.
Handler_CG_EVENT_IMPL(CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    const MSG_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ* recv_msg =
        static_cast<MSG_CG_EVENT_SYSTEMMEMO_ITEM_REWARD_REQ*>(pRecvMsg);

    const Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        assert(!"Player Not Found");
        return;
    }

    MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN syn_msg;
    {
        syn_msg.char_guid_ = player->GetCharGuid();
        syn_msg.memo_guid_ = recv_msg->memo_guid_;
        _tcsncpy(syn_msg.memo_contents_, recv_msg->memo_contents_, 
                 _countof(syn_msg.memo_contents_));
        syn_msg.memo_contents_[_countof(syn_msg.memo_contents_) - 1] = '\0';
    }
    g_pGameServer->SendToServer(GAME_DBPROXY, &syn_msg, sizeof(syn_msg));
}
