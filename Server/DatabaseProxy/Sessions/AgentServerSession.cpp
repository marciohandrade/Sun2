#include "stdafx.h"
#include "./AgentServerSession.h"
#include "PacketHandler/PacketHandler.h"
#include "DBUser.h"
#include "QueryObjects/DBUnit.h"
#include "QueryObjects/DBHandler.h"

__IMPL_CUSTOMPOOL_PTR( AgentServerSession )
AgentServerSession::AgentServerSession()
{
}

AgentServerSession::~AgentServerSession()
{
}

void AgentServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_AGENT, this, (MSG_BASE *)pMsg, wSize );
}

void AgentServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ChildServerSession::OnRedirect(network_index, first_contact_msg);
    DBUnit* db_unit = g_DBHandler.FindDBUnit(db_unit_id_);
    {
        MSG_AD_CONNECTION_DBCONNECT_UPDATE_CMD msg_cmd;
        msg_cmd.db_connector_index_ = (db_unit == NULL) ? 0 : db_unit->GetDBConnectorIndex();
        this->SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
}

void AgentServerSession::OnDisconnect()
{
    // CHANGES: f110428.7L, linked a missing events
    g_DBUserManager.RemoveUsersByServerIndex(GetSessionIndex(), GetServerType());
	ChildServerSession::OnDisconnect();
}