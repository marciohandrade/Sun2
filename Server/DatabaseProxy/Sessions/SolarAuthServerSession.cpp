#include "stdafx.h"
#include ".\SolarAuthServerSession.h"
#include "PacketHandler/PacketHandler.h"
#include "Protocol_SX.h"
#include "PacketStruct_SX.h"
#include "QueryObjects/DBHandler.h"

__IMPL_CUSTOMPOOL_PTR( SolarAuthServerSession )
SolarAuthServerSession::SolarAuthServerSession(void)
{
}

SolarAuthServerSession::~SolarAuthServerSession(void)
{
}

// CHANGES: f110617.1L
void SolarAuthServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ChildServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID SolarAuthServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_SOLARAUTH, this, (MSG_BASE *)pMsg, wSize );
}

VOID SolarAuthServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ChildServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		db_unit_id_ = g_DBHandler.AllocateDBUnit( GetServerType(), GetSessionIndex() );
		ASSERT( 0 != db_unit_id_ );
	}
}

