#include "StdAfx.h"
#include "Handler_AG_ITEM.h"
#include <PacketStruct_AG.h>
#include "Player.h"
#include "./Status/StatusManager.h"
#include "ItemManager.h"
#include "GameServerEx.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_ITEM::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_ITEM::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_ITEM,  AG_ITEM_PORTAL_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ITEM,  AG_ITEM_UNLOCK_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ITEM,  AG_ITEM_DECREASE_UNLOCK_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ITEM, AG_ITEM_USE_DECREE_ITEM_SYN, PHASE_SERVICE),

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
        HANDLER_NODE_INFO(AG_ITEM, AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK, PHASE_SERVICE),
#endif

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_AG_ITEM_IMPL( AG_ITEM_PORTAL_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_PORTAL_ACK *pRecvMsg = (MSG_AG_ITEM_PORTAL_ACK*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

    //{__NA001164_20081024_CHAR_WAY_POINT_ITEM
	//웨이포인트 리스트를 통한 이동 후처리.
	ItemManager* pItemManager = pPlayer->GetItemManager();
	if( !pItemManager )	return;

    ASSERT(pRecvMsg->m_ItemCode);
    if (pRecvMsg->m_ItemCode == 0)
        return;

	SCSlotContainer * pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer( SI_INVENTORY );	
	SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot( pRecvMsg->m_PortalPos );

    POSTYPE item_position = INVALID_POSTYPE_VALUE;
    if (rItemSlot.GetItemCode() == pRecvMsg->m_ItemCode)
    {
        item_position = pRecvMsg->m_PortalPos;
    }
    else
    {
        const SCSlotContainer::SLOT_LINEAR_LIST& slot_list = pContainer->GetSlotList();
        uint count = slot_list.size();
        for (uint i = 0 ; i < count ; ++i)
        {
            SCItemSlot& temp_slot = (SCItemSlot&)pContainer->GetSlot(i);
            if (pRecvMsg->m_ItemCode == temp_slot.GetItemCode())
            {
                item_position = temp_slot.GetPos();
                break;
            }
        }
    }

    if (item_position == INVALID_POSTYPE_VALUE)
        return;

    SCItemSlot& item_slot = (SCItemSlot&)pContainer->GetSlot(item_position);
   
    if (item_slot.IsWayPointItem())
        return;

    if (item_slot.GetItemInfo()->m_wType != eITEMTYPE_PORTAL)
        return;

	// 복수의 돌 아이템을 삭제한다.
	RC::eITEM_RESULT rcResult = pPlayer->GetItemManager()->Delete( SI_INVENTORY, pRecvMsg->m_PortalPos, 1, ITEM_DELETE_ITEM_PORTAL );
	if( rcResult != RC::RC_ITEM_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[AG_ITEM_PORTAL_ACK] Cannot Delete PortalItem! ErrorCode[%d]", rcResult );
		return;
	}
}

Handler_AG_ITEM_IMPL( AG_ITEM_UNLOCK_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_UNLOCK_CMD *pRecvMsg = (MSG_AG_ITEM_UNLOCK_CMD*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;
	
	ItemManager* pItemManager = pPlayer->GetItemManager();
	if( !pItemManager )	return;
	
	SCSlotContainer * pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer( pRecvMsg->m_ItemIndex );	

	if( !pItemManager->ValidState() ) 
		return;

	//if( !pItemManager->ValidPos( pRecvMsg->m_ItemIndex, pRecvMsg->m_ItemPos ) )
	//	return;

	SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot( pRecvMsg->m_ItemPos );
	if( !rItemSlot.GetItemInfo() )
		return;

	rItemSlot.SetLock(FALSE);

    // (f100528.2L) prevent a locked state after an instance dungeon moving transaction failed
    // if a item operation is fails, a post action is failed, too.
    // referenced by f100514.3L
    switch (pRecvMsg->post_action_)
    {
    case pRecvMsg->eAction_MoveFailed:
        {
            StatusManager* status_manager = pPlayer->GetStatusManager();
            status_manager->ChangeInterState(eCHAR_STATE_ZONE_TRANSACTION, false);
        };
        break;
    }
}

Handler_AG_ITEM_IMPL( AG_ITEM_DECREASE_UNLOCK_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_DECREASE_UNLOCK_CMD *pRecvMsg = (MSG_AG_ITEM_DECREASE_UNLOCK_CMD*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	ItemManager* pItemManager = pPlayer->GetItemManager();
	if( !pItemManager )	return;

	SCSlotContainer * pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer( pRecvMsg->m_ItemIndex );
	
	SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot( pRecvMsg->m_ItemPos );
	if( !rItemSlot.GetItemInfo() )
		return;

	rItemSlot.SetLock(FALSE);

	if( !pItemManager->ValidState() ) 
		return;

	if( !pItemManager->ValidPos( pRecvMsg->m_ItemIndex, pRecvMsg->m_ItemPos ) )
		return;	
	
	GAMELOG->LogItem( ITEM_USE, pPlayer, &rItemSlot );

	pItemManager->Lose( pContainer, pRecvMsg->m_ItemPos, 1);
	
	WORD wCoolTimeType = rItemSlot.GetItemInfo()->m_wCoolTimeType;
	pPlayer->ResetUseItem( wCoolTimeType );
}

Handler_AG_ITEM_IMPL( AG_ITEM_USE_DECREE_ITEM_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_USE_DECREE_ITEM_SYN* pRecvMsg = (MSG_AG_ITEM_USE_DECREE_ITEM_SYN *)pMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
	if(!pPlayer)					return;
	if(!pPlayer->GetField() )		return;

	ItemManager* pItemManager = pPlayer->GetItemManager();
	if(!pItemManager)	return;

	SCSlotContainer* pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer(SI_INVENTORY);
	if(!pContainer)	return;

	for(int pos = 0; pos < MAX_INVENTORY_SLOT_NUM; pos++)
	{
		if( pContainer->IsEmpty(pos) )
			continue;

		SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);

		if( rItemSlot.GetSerial() != 0 )
		{
			const BASE_ITEMINFO* const pInfo = rItemSlot.GetItemInfo();
			if(!pInfo)	break;

			if(pInfo->m_wType == eITEMTYPE_CHAT_SHOUT_CONSUME)
			{
				if(!pItemManager->ValidState())	return;
				if(!pItemManager->ValidPos(SI_INVENTORY, pos))	return;

                //__CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK
				RC::eITEM_RESULT rcResult = pPlayer->CanUseItem( rItemSlot.GetItemInfo() );
				if( rcResult != RC::RC_ITEM_SUCCESS )
				{
					MSG_AG_ITEM_USE_DECREE_ITEM_NAK msgNAK;
					msgNAK.m_dwKey = pPlayer->GetUserGuid();
					msgNAK.m_byErrorCode = rcResult;
					g_pGameServer->SendToServer(AGENT_SERVER, &msgNAK, sizeof(msgNAK));
					return;
				}
				pItemManager->Lose(pContainer, pos, 1);

				MSG_CG_ITEM_USE_ACK ackMsg;
				ackMsg.m_atIndex = static_cast<BYTE>(SI_INVENTORY);
				ackMsg.m_atPos = pos;
				pPlayer->SendPacket( &ackMsg, sizeof( ackMsg ) );

				MSG_AG_ITEM_USE_DECREE_ITEM_ACK msgACK;
				msgACK.m_dwKey = pPlayer->GetUserGuid();							
				g_pGameServer->SendToServer(AGENT_SERVER, &msgACK, sizeof(msgACK));

				return;
			}
		}
		continue;
	}

	//여기까지 오면 해당 아이템이 없는거다.
	MSG_AG_ITEM_USE_DECREE_ITEM_NAK msgNAK;
	msgNAK.m_dwKey = pPlayer->GetUserGuid();
	msgNAK.m_byErrorCode = RC::RC_ITEM_NOT_EXIST;
	g_pGameServer->SendToServer(AGENT_SERVER, &msgNAK, sizeof(msgNAK));
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// 하임행운 캐시 응모권 사용 응답
Handler_AG_ITEM_IMPL( AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK* msg = \
        static_cast<const MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK*>(pMsg);

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(msg->char_guid);
    if (player == NULL || player->GetField() == NULL) {
        return;
    }

    if (msg->result_code != RC::RC_ITEM_SUCCESS)
    {
        MSG_CG_ITEM_USE_NAK nak;
        nak.m_ErrorCode = (BYTE)msg->result_code;
        player->SendPacket(&nak, (WORD)sizeof(nak));
        // 실패의 경우에도 유효한 아이템의 unlock을 위해 일단 진행
    }

    ItemManager* item_manager = player->GetItemManager();
    if (item_manager == NULL) {
        return;
    }
    SCSlotContainer* container = (SCSlotContainer*)item_manager->GetItemSlotContainer(msg->index);
    if (container == NULL) {
        return;
    }
    SCItemSlot& item_slot = (SCItemSlot&)container->GetSlot(msg->pos);
    if (item_slot.GetSerial() == 0) {
        return;
    }
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info == NULL) {
        return;
    }

    item_slot.SetLock(FALSE);

    // 실패의 경우에는 여기서 리턴
    if (msg->result_code != RC::RC_ITEM_SUCCESS) {
        return;
    }

    // 성공의 경우에는 먼저 아이템 사용 로그부터 남긴다
    GAMELOG->LogItem(ITEM_USE, player, &item_slot);

    // 소모성 아이템 삭제
    if (item_info->m_wType == eITEMTYPE_HEIM_LOTTO_TICKET)
    {
        if(item_slot.GetNum() <= 1) {
            container->DeleteSlot(msg->pos, NULL);
        }
        else {
            container->UpdateSlot(msg->pos, -1);
        }
    }

    player->ResetUseItem(item_info->m_wCoolTimeType);

    MSG_CG_ITEM_USE_ACK ack;
    ack.m_atIndex = msg->index;
    ack.m_atPos   = msg->pos;
    player->SendPacket(&ack, (WORD)sizeof(ack));
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET
