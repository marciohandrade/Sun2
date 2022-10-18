#include "StdAfx.h"
#include ".\Handler_CG_CHUNTING.h"
#include "GameZone.h"
#include "CompetitionInfo.h"
#include "GameCHuntingRoom.h"
#include "CHuntingManager.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_CHUNTING::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_CHUNTING::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_CHUNTING, CG_CHUNTING_POINT_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_CG_CHUNTING_IMPL(CG_CHUNTING_POINT_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHUNTING_POINT_INFO_SYN* pRecvMsg = (MSG_CG_CHUNTING_POINT_INFO_SYN*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_CHUNTING_IMPL(CG_CHUNTING_POINT_INFO_SYN)] Not Exist GameField" );
		return;
	}

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( !pZone )	return;

	if( pZone->GetZoneType() != eZONETYPE_CHUNTING )	return;

	GameCHuntingRoom *pCHunting = (GameCHuntingRoom*)pZone;

	MSG_CG_CHUNTING_POINT_INFO_ACK ackMsg;
	ackMsg.m_byCount = pCHunting->GetCHuntingManager()->GetAllPointInfo( 0, ackMsg.m_Point );
	pPlayer->SendPacket( &ackMsg, ackMsg.GetSize() );
}