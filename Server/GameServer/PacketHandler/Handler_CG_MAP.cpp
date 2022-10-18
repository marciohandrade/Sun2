#include "stdafx.h"
#include ".\Handler_CG_MAP.h"

#include <ServerSession.h>

#include "FieldServer.h"
#include "Player.h"

#include "GameField.h"
#include "Map.h"

#include "StatusManager.h"

#include "EquipmentSlotContainer.h"
#include "ItemManager.h"
#include "MovementChecker.h"

Handler_CG_MAP::Handler_CG_MAP()
{
}

Handler_CG_MAP::~Handler_CG_MAP()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_MAP::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_MAP::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_MAP, CG_MAP_TELEPORT_SYN, PHASE_SERVICE),
        ////_NA_0_20100722_AUTO_SYSTEM
        HANDLER_NODE_INFO(CG_MAP, CG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD, PHASE_SERVICE),
        //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
        HANDLER_NODE_INFO(CG_MAP, CG_MAP_KBMOVE_SERIAL_CHECK_ACK, PHASE_SERVICE),
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


VOID Handler_CG_MAP::OnCG_MAP_TELEPORT_SYN( ServerSession * pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_MAP_TELEPORT_SYN* pRecvMsg = (MSG_CG_MAP_TELEPORT_SYN *)pMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

    GameField* pField = pPlayer->GetField();
	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_MAP::OnCG_MAP_TELEPORT_SYN] Not Exist GameField" );
		return;
	}

	// 서버 모드에 따라 달리 처리될 필요가 있는지 확인할 필요가 있다.
	if( !pPlayer->IsGMUser() ) //sts 값이 100을 넘어서 GM이면....
	{
		MessageOut( eDEV_LOG, _T("ASSERTION LOGIC VIOLATION:TELEPORT SYN %s"), pPlayer->GetCharName() );
		return;
	}
	// GM인 경우 패킷 감시를 할까 말까...

	WzVector wvDestination = pRecvMsg->vPos;
	pField->TeleportObject( pPlayer, &wvDestination );
	//printf( "TELEPORT %.4f, %.4f, %.4f\n", wvDestination.x, wvDestination.y, wvDestination.z );
}

//_NA_0_20100722_AUTO_SYSTEM
VOID Handler_CG_MAP::OnCG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD(ServerSession* pServerSession, 
                                                             MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD* pRecvMsg = 
        (MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer ) return;

    GameField* pField = pPlayer->GetField();
    if( !pField )
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_MAP::OnCG_MAP_TELEPORT_SYN] Not Exist GameField" );
        return;
    }

    CEquipmentSlotContainer* pContainer = 
        static_cast<CEquipmentSlotContainer*>(
        pPlayer->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));

    POSTYPE pos = pContainer->GetPosEquipByItemType(eITEMTYPE_AUTO_HUNTING, 
                                                    eEQUIPCONTAINER_CHARGE1, eEQUIPCONTAINER_MAX);

    if (pos == eEQUIPCONTAINER_INVALID)
    {
        MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_ITEM_NOT_EXIST;
        pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
        return;
    }

    SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(pContainer->GetSlot(pos));

    if (rItemSlot.IsProgressTime() == false)
    {
        MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_ITEM_INVALIDSTATE;
        pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
        return;
    }

    if (RC::RC_PORTAL_CAN_SUCCESS != pPlayer->MovelastReturnScroll())
    {
        MSG_CG_MAP_MOVE_LAST_RETURNSCROLL_POS_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA;
        pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
        return;        
    }
}

//==================================================================================================
// CHANGES: f110729.3L, _NA000000_SPEED_HACK_PREVENTION_SUPPORT
void Handler_CG_MAP::OnCG_MAP_KBMOVE_SERIAL_CHECK_ACK(
    ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA000000_SPEED_HACK_PREVENTION_SUPPORT)
    ASSERT(!"current version not supported");
    return;
#endif
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK* recv_msg = \
        static_cast<MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK*>(pMsg);
    //
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    GameField* const game_field = player ? player->GetField() : NULL;
    if ((player == NULL) || (game_field == NULL)) {
        return;
    };

    nsSync::CSSyncTuner* const sync_tuner = player->GetSyncTuner();
    if (sync_tuner->ReceiveReply(*recv_msg) == false) {
        StatusManager* status_manager = player->GetStatusManager();
        status_manager->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, true);
        return;
    };

#endif
}

//==================================================================================================
