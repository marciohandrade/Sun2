#include "StdAfx.h"
#include ".\handler_cg_waypoint.h"
#include ".\Player.h"
#include ".\PlayerManager.h"
#include <PacketStruct_CG.h>
#include <PacketStruct_DG.h>
#include <WayPoint.h>
#include "GameZone.h"
#include "ChaoState.h"
#include "ItemManager.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_WAYPOINT::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_WAYPOINT::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_WAYPOINT,  CG_WAYPOINT_INFO_LOAD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAYPOINT,  CG_WAYPOINT_INFO_SAVE_SYN, PHASE_SERVICE),
        //{__NA001164_20081024_CHAR_WAY_POINT_PACKET
        HANDLER_NODE_INFO(CG_WAYPOINT,  CG_CHAR_WAYPOINT_INFO_LOAD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAYPOINT,  CG_CHAR_WAYPOINT_INFO_SAVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAYPOINT,  CG_CHAR_WAYPOINT_INFO_DEL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_WAYPOINT,  CG_CHAR_WAYPOINT_INFO_EXE_CMD, PHASE_SERVICE),
        //}__NA001164_20081024_CHAR_WAY_POINT_PACKET
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_WAYPOINT_IMPL(CG_WAYPOINT_INFO_LOAD_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_WAYPOINT_INFO_LOAD_SYN * pRecvMsg = (MSG_CG_WAYPOINT_INFO_LOAD_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	if( pPlayer->GetChaoState().IsChaoState() )
	{
		MSG_CG_WAYPOINT_INFO_LOAD_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}
	else if( pPlayer->GetWayPoint()->IsLoaded() )
	{
		MSG_CG_WAYPOINT_INFO_LOAD_ACK msg;
		pPlayer->GetWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
		pPlayer->SendPacket( &msg, msg.GetSize() );

		return;
	}
	else
	{
		MSG_DG_WAYPOINT_INFO_LOAD_SYN msg;
		if( !pPlayer->SendToGameDBPServer( &msg, sizeof(msg) ) )
		{
			MSG_CG_WAYPOINT_INFO_LOAD_NAK nmsg;
			nmsg.m_dwErrorCode = RC::RC_WP_CANNOT_LOAD_FROM_DB;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}

		return;
	}
}

Handler_CG_WAYPOINT_IMPL(CG_WAYPOINT_INFO_SAVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_WAYPOINT_INFO_SAVE_SYN * pRecvMsg = (MSG_CG_WAYPOINT_INFO_SAVE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if( !pPlayer ) return;
    if( !pPlayer->GetField() ) return;

    GameZone* pGameZone = pPlayer->GetGameZonePtr();
    if( !pGameZone ) 
        return;

    S_TRY
    {
        ChaoState& rChaoState = pPlayer->GetChaoState();
	    if( rChaoState.IsChaoState() )
            S_THROW(RC::RC_WP_UNABLE_FUNCTION_FOR_CHAOSTATE);

        WayPoint* pWayPoint = pPlayer->GetWayPoint();
        if( NULL == pWayPoint )
            S_THROW(RC::RC_WP_PORTAL_FAIL);

        MAPCODE CurMapCode = pGameZone->GetMapCode();
	    if( pWayPoint->IsExist(CurMapCode) )
            S_THROW(RC::RC_WP_ALREADY_SAVE_MAPCODE);
    }
    S_CATCH(DWORD, dwErrorCode)
    {
        MSG_CG_WAYPOINT_INFO_SAVE_NAK msg;
        msg.m_dwErrorCode = dwErrorCode;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }

	MSG_DG_WAYPOINT_INFO_SAVE_SYN msg;
	msg.m_MapCode = pGameZone->GetMapCode();
	pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );
}

Handler_CG_WAYPOINT_IMPL(CG_CHAR_WAYPOINT_INFO_LOAD_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHAR_WAYPOINT_INFO_LOAD_SYN * pRecvMsg = (MSG_CG_CHAR_WAYPOINT_INFO_LOAD_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    //---------------------------------------------------------------------------------------------
    // CHANGES: f110829.5L, internal automatic events moved from CG_SHOP_IS_RECV_GIFT_SYN 
    // CHANGES: f100908.2L, change a process order
    // because a server that is disconnected with a shop can't send a related information to player

    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    ;{  // (CHANGES) (f100714.2L) pre-fetch to support a delayed event query process
        // 응모한 이벤트 정보를 요청
        MSG_DG_EVENT_ITEM_STATUS_SYN msg_status;
        msg_status.m_dwKey = pPlayer->GetUserGuid();
        msg_status.m_CharGUID = pPlayer->GetCharGuid();
        pPlayer->SendToGameDBPServer(&msg_status, sizeof(msg_status));
    };
    ;{
        MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK msg_ack;
        msg_ack.changed = pPlayer->inventory_config();
        msg_ack.result = RC::RC_ITEM_SUCCESS;
        pPlayer->SendPacket(&msg_ack, sizeof(msg_ack));
    };
    //---------------------------------------------------------------------------------------------
#endif //
	
	if( pPlayer->GetCharWayPoint()->IsLoaded() )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_LOAD_ACK msg;
		pPlayer->GetCharWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
		pPlayer->SendPacket( &msg, msg.GetSize() );

		return;
	}
	else
	{
		MSG_DG_CHAR_WAYPOINT_INFO_LOAD_SYN msg;
		if( !pPlayer->SendToGameDBPServer( &msg, sizeof(msg) ) )
		{
			MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
			nmsg.m_dwErrorCode = RC::RC_WP_CANNOT_LOAD_FROM_DB;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
			return;
		}

		return;
	}
}


Handler_CG_WAYPOINT_IMPL(CG_CHAR_WAYPOINT_INFO_SAVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHAR_WAYPOINT_INFO_SAVE_SYN * pRecvMsg = (MSG_CG_CHAR_WAYPOINT_INFO_SAVE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );

    if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;
  
    GameZone* pGameZone = pPlayer->GetGameZonePtr();
    if( !pGameZone ) 
        return;

    S_TRY
    {
        CharWayPoint* pWP = pPlayer->GetCharWayPoint();
        if( NULL == pWP )
            S_THROW(RC::RC_WP_PORTAL_FAIL);

        if( !pWP->IsLoaded() )
            S_THROW(RC::RC_WP_NOT_YET_LOAD_FROM_DB);

	    eZONETYPE ZoneType = pGameZone->GetZoneType();
	    if( ZoneType != eZONETYPE_VILLAGE	&&
		    ZoneType != eZONETYPE_FIELD		)
            S_THROW(RC::RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE);

        //{__NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP
        MAPCODE CurMapCode = pGameZone->GetMapCode();
        if( CurMapCode == HardCode::MAPCODE_for_GM ||
            CurMapCode == HardCode::MAPCODE_for_TUTORIAL_FIELD  )
            S_THROW(RC::RC_WP_UNABLE_FUNCTION_FOR_CUR_ZONE);
        //}
	    if( MAX_CHAR_WAYPOINT_NUM < pWP->GetCount() )
            S_THROW(RC::RC_WP_UNABLE_FUNCTION_FOR_OVERCOUNT);

	    //item 이 있는지 체크
	    ItemManager* pItemManager = pPlayer->GetItemManager();
	    if(!pItemManager)
            S_THROW(RC::RC_WP_PORTAL_FAIL);

	    SCSlotContainer* pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer(SI_INVENTORY);
	    if(!pContainer)	
            S_THROW(RC::RC_WP_PORTAL_FAIL);

        int pos = 0;
	    for(; pos < MAX_INVENTORY_SLOT_NUM; ++pos)
	    {
		    if( pContainer->IsEmpty(pos) )
			    continue;

		    SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);

		    if( rItemSlot.GetSerial() != 0 && 
			    rItemSlot.IsWayPointItem() &&
			    rItemSlot.IsProgressTime() )
			    break;
	    }

	    if( pos == MAX_INVENTORY_SLOT_NUM )
            S_THROW(RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM);
    }
    S_CATCH(DWORD, dwErrorCode)
    {
        MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
        nmsg.m_dwErrorCode = dwErrorCode;
        pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
        return;
    }

	WzVector wvCurPos;
	pPlayer->GetPos(&wvCurPos);

	MSG_DG_CHAR_WAYPOINT_INFO_SAVE_SYN msg;
	msg.m_MapCode = pGameZone->GetMapCode();
	msg.m_X = (SHORT)wvCurPos.x;
	msg.m_Y = (SHORT)wvCurPos.y;
	msg.m_Z = (SHORT)wvCurPos.z;

	pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );
}


Handler_CG_WAYPOINT_IMPL(CG_CHAR_WAYPOINT_INFO_DEL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHAR_WAYPOINT_INFO_DEL_SYN * pRecvMsg = (MSG_CG_CHAR_WAYPOINT_INFO_DEL_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;
	if( !pPlayer->GetGameZonePtr() ) return;

	if( !pPlayer->GetCharWayPoint()->IsLoaded() )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_NOT_YET_LOAD_FROM_DB;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	//item 이 있는지, 삭제 가능 한지 체크
	ItemManager* pItemManager = pPlayer->GetItemManager();
	if(!pItemManager)	
		return;

	SCSlotContainer* pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer(SI_INVENTORY);
	if(!pContainer)	
		return;

    int pos = 0;
	for(; pos < MAX_INVENTORY_SLOT_NUM; ++pos)
	{
		if( pContainer->IsEmpty(pos) )
			continue;

		SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);

		if( rItemSlot.GetSerial() != 0 && 
			rItemSlot.IsWayPointItem() &&
			rItemSlot.IsProgressTime() )
				break;
	}

	if( pos == MAX_INVENTORY_SLOT_NUM )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);
	if( !rItemSlot.CanWayPointDelete() )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_UNABLE_DELETE_FOR_DELTIME;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	if( !pPlayer->GetCharWayPoint()->IsExist(pRecvMsg->m_IdxWayPoint) )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK msg;
		msg.m_dwErrorCode = RC::RC_WP_CANNOT_FIND_WP_IDX;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}

	//.//

	MSG_DG_CHAR_WAYPOINT_INFO_DEL_SYN msg;
	msg.m_IdxWayPoint = pRecvMsg->m_IdxWayPoint;
	if( !pPlayer->SendToGameDBPServer( &msg, sizeof(msg) ) )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_CANNOT_DELETE_FROM_DB;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}
}


Handler_CG_WAYPOINT_IMPL(CG_CHAR_WAYPOINT_INFO_EXE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHAR_WAYPOINT_INFO_EXE_CMD * pRecvMsg = (MSG_CG_CHAR_WAYPOINT_INFO_EXE_CMD *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    GameZone* pGameZone = pPlayer->GetGameZonePtr();
	if( !pGameZone ) 
        return;

    CharWayPoint* pWP = pPlayer->GetCharWayPoint();
	if( !pWP->IsLoaded() )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_NOT_YET_LOAD_FROM_DB;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

    if( !pWP->IsExist(pRecvMsg->m_IdxWayPoint) )
    {
        MSG_CG_CHAR_WAYPOINT_INFO_NAK msg;
        msg.m_dwErrorCode = RC::RC_WP_CANNOT_FIND_WP_IDX;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }

	//item 이 있는지 체크
	ItemManager* pItemManager = pPlayer->GetItemManager();
	if(!pItemManager)	
		return;

	SCSlotContainer* pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer(SI_INVENTORY);
	if(!pContainer)	
		return;

    int pos = 0;
	for(; pos < MAX_INVENTORY_SLOT_NUM; ++pos)
	{
		if( pContainer->IsEmpty(pos) )
			continue;

		SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);

		if( rItemSlot.GetSerial() != 0 && 
			rItemSlot.IsWayPointItem() &&
			rItemSlot.IsProgressTime() )
			break;
	}

	if( pos == MAX_INVENTORY_SLOT_NUM )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_CANNOT_FIND_WAYPOINT_ITEM;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	//아이템 쿨타임 검사
	SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);
	if( RC::RC_ITEM_SUCCESS != pPlayer->CanUseItem(rItemSlot.GetItemInfo()) )
	{
		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_WP_PORTAL_FAIL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;		
	}

    //{__NA001164_20081024_CHAR_WAY_POINT_PORTAL
    const RC::ePORTAL_RESULT retValue = pPlayer->CanPortal();
    if( RC::RC_PORTAL_CAN_SUCCESS != retValue )
    {
        MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;

        switch(retValue)
        {
        case RC::RC_PORTAL_UNABLE_CUR_ZONE: 
            nmsg.m_dwErrorCode = RC::RC_WP_UNABLE_FUNCTION_FOR_ZONETYPE; 
            break;
        case RC::RC_PORTAL_FIELD_IS_NULL:   
            nmsg.m_dwErrorCode = RC::RC_WP_FAILED;              
            break;
        case RC::RC_PORTAL_INVALIDSTATE:    
            nmsg.m_dwErrorCode = RC::RC_WP_PORTAL_FAIL;               
            break;
        }

        pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
        return;
    }
    //}

 	WAYPOINTSLOT stWayPointSlot = pPlayer->GetCharWayPoint()->GetWayPointSlot(pRecvMsg->m_IdxWayPoint);
 	
	WzVector vecDestPos;
	vecDestPos.x = stWayPointSlot.m_X;
	vecDestPos.y = stWayPointSlot.m_Y;
	vecDestPos.z = stWayPointSlot.m_Z;

    RC::ePORTAL_RESULT rt = pPlayer->Portal( ePORTAL_TYPE_ITEM_EX, stWayPointSlot.m_MapCode, vecDestPos, pos );
 	if( RC::RC_PORTAL_FAIL == rt )
 	{
 		MSG_CG_CHAR_WAYPOINT_INFO_NAK nmsg;
 		nmsg.m_dwErrorCode = RC::RC_WP_PORTAL_FAIL;
 		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
 		return;
 	}

	WORD wCoolTimeType = rItemSlot.GetItemInfo()->m_wCoolTimeType;
	pPlayer->ResetUseItem( wCoolTimeType );
}
