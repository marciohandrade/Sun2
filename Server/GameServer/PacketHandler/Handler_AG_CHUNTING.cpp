#include "StdAfx.h"
#include ".\handler_ag_chunting.h"
#include "GameZoneManager.h"
#include "GameCHuntingRoom.h"
#include "CHuntingManager.h"
#include <PacketStruct_AG.h>


Handler_AG_CHUNTING::Handler_AG_CHUNTING()
{
}

Handler_AG_CHUNTING::~Handler_AG_CHUNTING()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_CHUNTING::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_CHUNTING::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_CHUNTING, AG_CHUNTING_ENTER_TEAM_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_AG_CHUNTING_IMPL( AG_CHUNTING_ENTER_TEAM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHUNTING_ENTER_TEAM_CMD* pRecvMsg = (MSG_AG_CHUNTING_ENTER_TEAM_CMD*)pMsg;

	GameZone *pZone = g_GameZoneManager.GetZone( pRecvMsg->m_RoomKey );
	if( !pZone )	return;
	if( pZone->GetZoneType() != eZONETYPE_CHUNTING )	return;

	GameCHuntingRoom *pCHuntingRoom = (GameCHuntingRoom*)pZone;

	pCHuntingRoom->GetCHuntingManager()->EnterTeam( pRecvMsg->m_byReadyPlayerNum, pRecvMsg->m_UserKeyList, pRecvMsg->m_GuildGuid );
}














