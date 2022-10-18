#pragma once

#include <PacketStruct_AZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class WorldServerSession;

//-------------------------------------------------------------------------------------------------
#define Handler_FromWorldServer_DECL( p )	static VOID On##p( WorldServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_FromWorldServer_IMPL( p )	VOID Handler_FromWorldServer::On##p( WorldServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
//-------------------------------------------------------------------------------------------------

class Handler_FromWorldServer
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );

private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();

public:

	Handler_FromWorldServer_DECL( WZ_GUILD_RESTORE_SYN );
	Handler_FromWorldServer_DECL( WZ_GUILD_MEMBER_INFO_REQ_SYN );
    Handler_FromWorldServer_DECL( WZ_GUILD_CHANGE_NOTICE_SYN );
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    Handler_FromWorldServer_DECL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN );
};
