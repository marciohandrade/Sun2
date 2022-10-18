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

        //	<Desc> 기존과 다른 체계. 바로 보상 받는 형태다.
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_REQUEST_ITEM_SYN, PHASE_SERVICE),	// 성공시 CG_EVENT_ITEM_REWARD_CMD
        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_DONATION_HEIM_SYN, PHASE_SERVICE),	// 기부 요청

        HANDLER_NODE_INFO(CG_EVENT, CG_EVENT_BUFF_EVENT_SYN, PHASE_SERVICE),			// 버프 이벤트 요청
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

// 이벤트 응모 상태 확인
Handler_CG_EVENT_IMPL( CG_EVENT_ITEM_STATUS_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	const MSG_CG_EVENT_ITEM_STATUS_SYN* pMsg = (MSG_CG_EVENT_ITEM_STATUS_SYN*)pRecvMsg;
	MSG_CG_EVENT_ITEM_STATUS_ACK msgACK;

	// 해당 유저의 이벤트 상태 보내준다
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<Handler for CG_EVENT_ITEM_REG_SYN>
//	-	SubHandler
//
struct SubHandler_for_CG_EVENT_ITEM_REG_SYN
{
	static const DWORD		ERRCODE_DEFAULT			= 1000000;
	static const DWORD		ERRCODE_RANDREWARD		= ERRCODE_DEFAULT+2;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

			{	// 제한 개수 초과 - 이미 등록 완료
				const WORD registerLimit = (WORD)pEventInfo->m_byCountMax;
				if(registerLimit)
					IfFailThenBreak(registerLimit > pEventStatus->m_wRegisterCnt,
                                    RC::RC_EVENT_ITEM_NUM_OVERFLOW);
			};

			ItemManager* const pItemManager = pPlayer->GetItemManager();

			// 등록할 아이템이 등록할 개수만큼 존재하는지 검사
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
			// 아이템 다 존재하면 한꺼번에 삭제해야 뒷탈이 없다
			IfFailThenBreak(
                RC::RC_ITEM_SUCCESS == pItemManager->DeleteAllKindOfItem(delitemUB, delItem, delpos,
                                                                         &loseMsg.m_ItemInfo),
				RC::RC_EVENT_NOTEXISTITEM);

			GAMELOG->LogMultiItem( ITEM_EVENT_ETHER_REG, pPlayer, delitemUB, delItem, delpos );

			pPlayer->SendPacket( &loseMsg, loseMsg.GetSize() );	// 대신 니 아이템도 내놔~

			// 모두 통과하면 응모 횟수 하나 증가
			if( pEventStatus->m_wRegisterCnt < (WORD)-1 )
				++pEventStatus->m_wRegisterCnt;

			MSG_DG_EVENT_ITEM_REG_SYN updateMsg;

			// DBProxy에 경품 응모 등록 요청
			updateMsg.m_dwKey = pPlayer->GetUserGuid();
			updateMsg.m_dwCharGUID = pPlayer->GetCharGuid();

			{	// 완료 여부 체크
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

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
			// 해당 이벤트를 얻은 다음, 조건 체크 - 성공하면 아이템 지급
			// 조건 체크
			ItemManager* const pItemManager = pPlayer->GetItemManager();

			// 등록할 아이템이 등록할 개수만큼 존재하는지 검사
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

			// 등록 아이템을 없앤다
			// 먼저 없애고 넣다가 실패하면 최악의 경우 아이템이 없어지므로 복구하면 되지만
			// 넣고 나서 없애다가 실패하면 최악의 경우 공짜로 아이템을 넘겨주므로 에러를 알아내기 더 어려워진다
			MSG_CG_ITEM_LOSE_ACK loseMsg;
			IfFailThenBreak(
				RC::RC_ITEM_SUCCESS == pItemManager->DeleteAllKindOfItem(delitemUB, delItem, delpos,
                                                                         &loseMsg.m_ItemInfo),
				RC::RC_EVENT_NOTEXISTITEM);

			GAMELOG->LogMultiItem( ITEM_EVENT_ETHER_DELETE, pPlayer, delitemUB, delItem, delpos );

			// 보상 아이템 획득에 랜덤 확률 적용 - 있는 것들만큼 해당 아이템을 줄 확률이 바뀐다. 1개면 100%, 2개면 50%씩, 3개면 33.3333...%
			DWORD rewardCnt = 0;
			FOREACH_ARR_CONTAIN( idx, REG_ITEM_CODE_ARRAY, pEventInfo->m_dwRewardInfo )
			{
				++rewardCnt;
			}

			IfFailThenBreak(rewardCnt != 0, RC::RC_EVENT_FAILED);

			// 보상 아이템 준다!
			const DWORD nRandIndex = (DWORD)rand() % rewardCnt;	// 있는 아이템 개수만큼 랜덤 적용
			const CODETYPE randRewardIndex = pEventInfo->m_dwRewardInfo[nRandIndex];

			MSG_CG_EVENT_ITEM_REWARD_CMD rewardMsg;
			IfFailThen(
				RC::RC_REWARD_SUCCESS == g_RewardManager.RewardItem(RewardManager::eREWARD_EVENT,
                    randRewardIndex, 1, pPlayer, rewardMsg.m_TotalInfo), RC::RC_EVENT_CANT_OBTAIN)
			{
				INVENTORY_TOTAL_INFO tempTotalInfo;	// 임시 변수
				// (WAVERIX) (090610) (CHANGES) 복구 처리
			    /*RC::RC_ITEM_NOTEXIST_ITEM != */
                pPlayer->GetItemManager()->ObtainAllKindOfItem(delitemUB, delItem, delpos,
                                                               &tempTotalInfo);
				break; //이건 필요 없지만...
			}

			if( flow.IsErrorOccured() )
				break;

			pPlayer->SendPacket( &loseMsg, loseMsg.GetSize() );	// 아이템 등록했고

			// 보상 아이템 확인
			SLOTCODE* pRewardCodes = g_RewardManager.GetRewardItemCode();
			IfFailThenBreak(pRewardCodes, ERRCODE_RANDREWARD);

			rewardMsg.m_dwEventID = eventID;
			rewardMsg.m_Money = pPlayer->GetMoney();

			pPlayer->SendPacket( &rewardMsg, rewardMsg.GetSize() );	// 등록됐으니 아이템 받아라!

		#define _WHATTHEHELL_ITEMREWARD_DBREG_20060603
		#ifdef _WHATTHEHELL_ITEMREWARD_DBREG_20060603	// 아이템 보상받은 것도 DB에 등록
			EVENT_STATUS_EX* pEventStatus = rEventD.FindEventStatus( eventID );
			if( NULL == pEventStatus )
				_RegisterEventStatus( eventID, pPlayer, &pEventStatus );

			// 최대값 체크
			if( pEventStatus->m_wRegisterCnt < (WORD)-1 )
				++pEventStatus->m_wRegisterCnt;

			// DBProxy에 아이템 등록됐음을 알려준다
			MSG_DG_EVENT_ITEM_REG_SYN updateMsg;

			// DBProxy에 경품 응모 등록 요청
			updateMsg.m_dwKey = pPlayer->GetUserGuid();
			updateMsg.m_dwCharGUID = pPlayer->GetCharGuid();

			{	// 완료 여부 체크
				const WORD registerLimit = (WORD)pEventInfo->m_byCountMax;
				pEventStatus->m_Status = 0;	// 여기는 완료가 없단다...
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<MSG_CG_EVENT_ITEM_REG_SYN>
//	-	이벤트 아이템 등록
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

		// ...이건 99.9% 참이 될 것이다. (EventManager 동작 하)
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
		case eEVENT_REGITEM_GETITEM:	//경품 응모
			SubHandler_for_CG_EVENT_ITEM_REG_SYN::On_eEVENT_REGITEM_GETITEM( flow, pPlayer, pEventInfo );
			break;
		case eEVENT_REGITEM_RANDREWARD:	// 랜덤하게 아이템 지급
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
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


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
//	<Desc> 기존과 다른 체계. 바로 보상 받는 형태다.
Handler_CG_EVENT_IMPL( CG_EVENT_REQUEST_ITEM_SYN )	// 성공시 CG_EVENT_ITEM_REWARD_CMD
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

		// 이벤트 상태 필요
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

		//	아이템 획득
		//		실패 조건 (일반적인 에러, 뺏지맨과 같은 인벤에서의 중복 허용 여부, ~~)
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
		pPlayer->SendPacket( &msgREWARD, msgREWARD.GetSize() );	// 등록됐으니 아이템 받아라!

		// SendToDB - ITEM_STATUS_REG (Heim, RegCnt~~)
		pEventStatus->m_wRegisterCnt = 1;	// 완료, 1개만 요청한다.
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
	case RC::RC_EVENT_INVALID_EVENT:		// 이벤트 중이 아니다
	case RC::RC_EVENT_ITEM_NUM_OVERFLOW:	// 이벤트 아이템 등록 제한 개수를 넘었다
	case RC::RC_EVENT_NOTEXISTITEM:		// 이벤트에 응모할 아이템이 없다
	case RC::RC_EVENT_CANT_OBTAIN:		// 보상 아이템을 넣을 수가 없다
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
//		MSG_CG_EVENT_ITEM_REWARD_CMD	<- 특정 조건 만족시 (특정 구간 최소값<=curheims<=최대값, 회수 이내) 아이템 보상
Handler_CG_EVENT_IMPL( CG_EVENT_DONATION_HEIM_SYN )	// 기부 요청
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

		//	Player* 의 이벤트 트랜젝션은 DB로 다녀와야 하는 경우 사용할 예정이긴 한데, 필요 없을 수도 있다.
		//	좀더 경우를 따져 볼 것.
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

		// 이벤트 상태 필요
		EVENT_STATUS_EX* pEventStatus = pPlayer->PlayerEvent().FindEventStatus( eventID );
		if( NULL == pEventStatus )
			_RegisterEventStatus( eventID, pPlayer, &pEventStatus );

		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		IfFailThenBreak(rPROTECT.IsUsable(), RC::RC_EVENT_ITEM_INVENTORY_LOCKED);

		// 기부 금액 변동에 따른 구간별 보상 처리 계산, MONEY->DWORD 변환이 존재하고 있다. 경고 목적으로 남겨둔다.
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

		// 돈 차감, 이 이후로 성공 여부 꼭 확인할 것, 실패시 복구 처리.
		IfFailThenBreak(pPlayer->MinusMoney(donateHeims), RC::RC_EVENT_FAILED);

		if( sEVENT_MINMAX_RESULT::CNTL_UPDATE == rResult.m_Result )
		{	// 기부만 성공시
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
		//	보상 조건... 여기서부터의 에러는 ERR_CANT_OBTAIN_ITEM로서 돈 차감 복구처리를 해줘야 한다.
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

		//	아이템 획득
		//		실패 조건 (일반적인 에러, ~~)
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
		pPlayer->SendPacket( &msgREWARD, msgREWARD.GetSize() );	// 등록됐으니 아이템 받아라!

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
	case RC::RC_EVENT_FAILED:						// 기본 에러,
	case RC::RC_EVENT_INSUFFICIENT_HEIM:			// 하임 부족
	case RC::RC_EVENT_DONATION_HEIM_LIMIT:			// 최소 기부 하임 만족 못함
	case RC::RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW:	// 더 이상 보상 받을 수 없다.
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
			// Heim 이 미리 차감되어 있었던 관계로 복구를 해줘야 한다.
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
//	-		RC_EVENT_SUCCESS	= 성공시
//	-		RC_EVENT_XXX		= 실패 + 각 사례별 + 특정 사례는 그 원인이 추가로 붙는다.
//	<NOTE>
//	-	이것이 적용되면서 모든 이벤트에 다음과 같은 내용이 포함되어야 한다.
//	-	RC_EVENT_UNQUALIFIED	= 실패 (적용 레벨이 아니다.)
Handler_CG_EVENT_IMPL(CG_EVENT_BUFF_EVENT_SYN )			// 버프 이벤트 요청
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

		//	Player* 의 이벤트 트랜젝션은 DB로 다녀와야 하는 경우 사용할 예정이긴 한데, 필요 없을 수도 있다.
		//	좀더 경우를 따져 볼 것.
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
			//	-	1. BUFF를 적용해주는 처리 ( eEVENT_BUFF_GRANT일 경우, rewardinfo[0]에 skill정보가 입력된다. )
			//	-		1) applicable level check
			//	-	2. 중복 적용 방지
			//	-		1) assert( applied buff count == 1)
			if( FALSE == pEventInfo->CheckLevel( pPlayer->GetLevel() ) )
			{ lastError = RC::RC_EVENT_UNQUALIFIED; continue; }
			// 노파심에서 CheckEvent가 수행되었다. 이벤트 조건에 만족하고 있을 것이다.

			// 보상 아이템 준다!
			const DWORD FIXED_SELECT_REWARD_INDEX = 0;		
		
			const DWORD skillInData = (pPlayer->CanApplyPCBangBenefit(true) == false)
				?	pEventInfo->m_dwRewardInfo[FIXED_SELECT_REWARD_INDEX]
				:	pEventInfo->m_dwPCRoomRewardInfo[FIXED_SELECT_REWARD_INDEX];
		
			const SLOTCODE rewardSkill = SAFE_NUMERIC_TYPECAST(DWORD, skillInData, SLOTCODE) ? (SLOTCODE)skillInData : 0;
			IfFailThenBreak(rewardSkill, RC::RC_EVENT_FAILED);

			SkillInfoParser* const pSkillInfoParser = SkillInfoParser::Instance();

			// 새로운 스킬에 대해 메모리를 할당받고 스킬정보를 셋팅한다.
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
	case RC::RC_EVENT_FAILED:						// (기존:기타 이유로 실패시)
		msgACK.SetInfo_FAIL();
		break;
	case RC::RC_EVENT_INVALID_EVENT:				// (기존)
		msgACK.SetInfo_INVALID();
		break;
	case RC::RC_EVENT_CANT_BUFF_GRANTED:			// (신규:이미 부여되어 있을 경우)
		msgACK.SetInfo_CANT_BUFF_GRANTED( eventID );
		break;
	case RC::RC_EVENT_UNQUALIFIED:					// (신규:보상을 받을 수 없다. 자격이 안되서) + 추가 사유 기재 필요
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
//보상 타입과 mapcode를 받고 DB에 내용및 금액 확인 요청
//왕의 증표 사용시 유저의 인벤토리 공간확인및 Rock을 확인하여 DB에 요청
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

    //--메모 내용 검사
    
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
//플레이어정보와 로또 index정보로 DB내용 및 금액 확인 요청
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
//시스템 메모 GUID와 해당 케릭터 GUID정보를 DBP보내 보상 아이템을 확인한다.
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
