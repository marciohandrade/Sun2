#include "StdAfx.h"
#include ".\handler_ag_charinfo.h"
#include ".\ObjectFactory.h"
#include ".\PlayerManager.h"
#include "Player.h"
#include <PacketStruct_AG.h>
#include <PacketStruct_DG.h>
#include <Protocol_DG.h>
#include <Protocol_AG.h>
#include <ResultCode.h>

#include <ServerSession.h>
#include "GameServerEx.h"

Handler_AG_CHARINFO::Handler_AG_CHARINFO()
{
}

Handler_AG_CHARINFO::~Handler_AG_CHARINFO()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_CHARINFO::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_CHARINFO::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_CHARINFO, AG_CHARINFO_PCBANG_BILL_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_AG_CHARINFO_IMPL(AG_CHARINFO_PCBANG_BILL_CMD)
{
    MSG_AG_CHARINFO_PCBANG_BILL_CMD* packet = (MSG_AG_CHARINFO_PCBANG_BILL_CMD*)pMsg;

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if(player == NULL) return;
    
    player->SetReservedValue(packet->m_PcRoomId);
}