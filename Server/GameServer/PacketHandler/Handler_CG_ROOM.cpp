#include "StdAfx.h"
#include ".\handler_cg_room.h"
#include <Protocol_CG.h>
#include <PacketStruct.h>
#include <PacketStruct_CG.h>
#include "Player.h"
#include "PlayerManager.h"
#include <ServerSession.h>
#include "FieldInfo.h"
#include "Map.h"

Handler_CG_ROOM::Handler_CG_ROOM()
{
}

Handler_CG_ROOM::~Handler_CG_ROOM()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_ROOM::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_ROOM::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------
