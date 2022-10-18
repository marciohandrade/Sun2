#include "stdafx.h"

#include "PacketHandler.Shared.h"
#include <PacketStruct_ServerCommon.h>

#include <ServerSessions/ServerSessionEx.h>

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&PacketHandlerShared::On##p }

sPACKET_HANDLER_INFO*	PacketHandlerShared::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_HEARTBEAT, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

//
//
//

HANDLER_COMMON_IMPL(SERVERCOMMON_HEARTBEAT)
{	// 0:1
	//SERVERCOMMON_HEARTBEAT
    __TOUCHED((pServerSessionEx, pMsg, wSize));
//	printf( "heartbeat\n" );
}


VOID	PacketHandlerShared::OnUSER_EMPTY_HANDLER( User* pSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pSession, pMsg, wSize));
	// SECURE_LOG???
	SECURELOG( SECURELOG_USERINFO(pSession, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_UNKNOWN_PROTOCOL),
		_T("[Request Message] Unknown Protocol - old tool version") );
}


#define PACKET_CATEGORY_SKIP_NODE( node )	case node:
#define PACKET_CATEGORY_SKIP_FILTER()					\
	PACKET_CATEGORY_SKIP_NODE( CG_SYNC )				\
	PACKET_CATEGORY_SKIP_NODE( CG_CONVERSATION )		\
	PACKET_CATEGORY_SKIP_NODE( CG_TRIGGER )				\
	PACKET_CATEGORY_SKIP_NODE( CG_BATTLE )

#define PACKET_PROTOCOL_SKIP_NODE( node )	case node:
#define PACKET_PROTOCOL_SKIP_FILTER()					\
	PACKET_PROTOCOL_SKIP_NODE( MAKEWORD( CG_CHARINFO, CG_CHARINFO_SELECT_INFO_SYN ) )


VOID	PacketHandlerShared::OnGAME_FORWARD_HANDLER( User* pSession, MSG_BASE* pMsg, WORD wSize )
{
	const DWORD		ERR_UNLINKED_SERVER		= 1000;
	const DWORD		ERR_FORWARDING_FAIL		= 1001;
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		IfFailThenBreak(pSession->GetLinkedServer(), ERR_UNLINKED_SERVER);
		
		IfFailThenBreak(pSession->SendToLinkedServer((MSG_BASE_FORWARD *)pMsg, wSize), ERR_FORWARDING_FAIL);

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	#ifndef _DEBUG
		switch( pMsg->m_byCategory )
		{		// 프로토콜 그룹별
		PACKET_CATEGORY_SKIP_FILTER();
			//	(080522) - 현 시점 (그래도 주의해야 하는 패킷)
			//	- MSG_CG_TRIGGER_LEAVE_AREA_SYN, 클라 존 이동 관련 동기화 (순서) 문제로 많이 발생 중...
			break;
		default:
			switch( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) )
			{	// 프로토콜 세부 사례별...
			PACKET_PROTOCOL_SKIP_FILTER();
				break;
			default:
				SUNLOG( eCRITICAL_LOG,
					_T("CA ParsePacket, Try to Send Unlinked Server [%03u:%03u]"),
					pMsg->m_byCategory, pMsg->m_byProtocol );
				break;
			}
			break;
		}
	#else
		SUNLOG( eCRITICAL_LOG,
			_T("CA ParsePacket, Try to Send Unlinked Server [%03u:%03u]"),
			pMsg->m_byCategory, pMsg->m_byProtocol );
	#endif
	
	return /*RETURN FALSE VALUE*/;
}

//_NA000000_CW_PROTOCOL_UNIFICATION
void PacketHandlerShared::OnWORLD_FORWARD_HANDLER(User* session, MSG_BASE* msg, WORD msg_size)
{
    // TODO: f110914.1L, check enabled_world_service per session
    bool sent = (session->SendToWorldServer(msg, msg_size) != false);
    __TOUCHED(sent);
    // connected check.... hummm
}