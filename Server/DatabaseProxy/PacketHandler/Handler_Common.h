#pragma once

#include "./PacketHandler.h"

struct MSG_BASE;
class ChildServerSession;

class Handler_Common
{
public:
	Handler_Common();
	~Handler_Common();

    // NOTE, f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

	static VOID OnSERVERCOMMON_HEARTBEAT( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
};
