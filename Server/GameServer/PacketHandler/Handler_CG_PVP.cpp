#include "StdAfx.h"
#include ".\handler_cg_pvp.h"
#include ".\PVPManager.h"
#include ".\Player.h"
#include ".\GamePVPRoom.h"

Handler_CG_PVP::Handler_CG_PVP(void)
{
}

Handler_CG_PVP::~Handler_CG_PVP(void)
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_PVP::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_PVP::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_PVP, CG_PVP_PRESS_KEY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_PVP_IMPL(CG_PVP_PRESS_KEY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_PVP_PRESS_KEY_SYN * pRecvMsg = (MSG_CG_PVP_PRESS_KEY_SYN *)pMsg;

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_PVP_IMPL(CG_PVP_PRESS_KEY_SYN)] Not Exist GameField" );
		return;
	}

	GameZone * pEnterZone	= NULL;
	MAPCODE EnterFieldCode	= 0;
	eZONE_STATE eState		= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
	if( !(eState & ePRS_AT_ZONE) )
	{
		MSG_CG_PVP_PRESS_KEY_NAK msg;
		msg.m_byErrorCode = RC::RC_PVP_NOT_AT_ZONE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}

	RC::ePVP_RESULT eRET = RC::RC_PVP_NOT_AT_ZONE;
	if( pEnterZone && (pEnterZone->GetZoneType() == eZONETYPE_PVP) )
	{
		GamePVPRoom* pPVPRoom = (GamePVPRoom *)pEnterZone;
		if( pPVPRoom->GetPVPManager() )
			eRET = pPVPRoom->GetPVPManager()->PressKeyPlayer( pPlayer );
	}

	if( eRET == RC::RC_PVP_SUCCESS )
	{
		MSG_CG_PVP_PRESS_KEY_ACK msgACK;
		pPlayer->SendPacket( &msgACK, sizeof(msgACK) );
	}
	else
	{
		MSG_CG_PVP_PRESS_KEY_NAK msgNAK;
		msgNAK.m_byErrorCode = eRET;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}