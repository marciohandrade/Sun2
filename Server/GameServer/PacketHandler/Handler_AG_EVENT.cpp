#include "StdAfx.h"
#include ".\Handler_AG_EVENT.h"
#include <PacketStruct_AG.h>

#include "Player.h"
#include ".\FriendInvitation\GFriendInvitationController.h"


Handler_AG_EVENT::Handler_AG_EVENT()
{
}

Handler_AG_EVENT::~Handler_AG_EVENT()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_EVENT::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_EVENT::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        HANDLER_NODE_INFO(AG_EVENT, AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_EVENT, AG_EVENT_RECALL_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_EVENT, AG_EVENT_RECALL_NAK, PHASE_SERVICE),
        //}
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


//_KR_0_20091021_FIRENDINVITATION_EXTEND
Handler_AG_EVENT_IMPL( AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD )
{
    GFriendInvitationController::Instance()->OnInvitatedFriendListSelectCMD(pServerSession, pMsg, wSize );
}

Handler_AG_EVENT_IMPL( AG_EVENT_RECALL_ACK )
{
    GFriendInvitationController::Instance()->OnAGRecallAck(pServerSession, pMsg, wSize );
}

Handler_AG_EVENT_IMPL( AG_EVENT_RECALL_NAK )
{
    GFriendInvitationController::Instance()->OnAGRecallNak(pServerSession, pMsg, wSize );
}

