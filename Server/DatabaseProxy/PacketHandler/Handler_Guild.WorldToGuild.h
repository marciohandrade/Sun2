#pragma once

#include <PacketStruct_AZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
//class WorldServerSession;

//-------------------------------------------------------------------------------------------------
#define Handler_Guild_WorldToGuild_DECL( p )	static VOID On##p( ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_Guild_WorldToGuild_IMPL( p )	VOID Handler_Guild_WorldToGuild::On##p( ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
//-------------------------------------------------------------------------------------------------

class Handler_Guild_WorldToGuild
{
public:
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

	Handler_Guild_WorldToGuild_DECL( WZ_GUILD_RESTORE_SYN );
	Handler_Guild_WorldToGuild_DECL( WZ_GUILD_MEMBER_INFO_REQ_SYN );
    Handler_Guild_WorldToGuild_DECL( WZ_GUILD_CHANGE_NOTICE_SYN );
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    Handler_Guild_WorldToGuild_DECL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN );
};
