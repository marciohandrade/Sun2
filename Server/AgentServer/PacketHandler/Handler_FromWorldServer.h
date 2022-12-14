#pragma once

#include <PacketStruct_AW.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class WorldServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_AW_DECL( p )	\
	static VOID On##p( WorldServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define HANDLER_AW_IMPL( p )	\
	VOID Handler_AW::On##p( WorldServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

//-------------------------------------------------------------------------------------------------

class Handler_AW
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();

public:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
    HANDLER_AW_DECL(AW_CONNECTION_GROUND_CMD);
	HANDLER_AW_DECL( AW_PREPARE_NEW_USER_ACK );
	HANDLER_AW_DECL( AW_PREPARE_NEW_USER_NAK );
	HANDLER_AW_DECL( AW_USER_RESTORE_SYN );
	//AW_FRIENDINVITATE 친구 초대 이벤트, _KR_0_20091021_FIRENDINVITATION_EXTEND
	HANDLER_AW_DECL( AW_FRIENDINVITATE_INVITATELIST_CMD );	
	// AW_CHAO
	HANDLER_AW_DECL( AW_CHAO_REQUEST_REVENGEINFO_ACK );
	HANDLER_AW_DECL( AW_CHAO_REQUEST_REVENGEINFO_NAK );

	HANDLER_AW_DECL( AW_CHAO_REMOVE_REVENGE_CHAR_CMD );
	HANDLER_AW_DECL( AW_CHAO_REMOVE_REVENGE_CHAR_ACK );
	HANDLER_AW_DECL( AW_CHAO_REMOVE_REVENGE_CHAR_NAK );
	HANDLER_AW_DECL( AW_CHAO_ADD_REVENGE_CHAR_ACK );
	HANDLER_AW_DECL( AW_CHAO_ADD_REVENGE_CHAR_NAK );

	HANDLER_AW_DECL( AW_ITEM_CHANGE_SPECIAL_ITEM_SYN );
	HANDLER_AW_DECL( AW_ITEM_USE_DECREE_ITEM_SYN );
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    HANDLER_AW_DECL( AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK );
#endif

    HANDLER_AW_DECL( AW_USER_STATUS_INTERCEPTION_ACK );
	HANDLER_AW_DECL( AW_USER_STATUS_INTERCEPTION_NAK );
	HANDLER_AW_DECL( AW_OPERATION_ROOM_CTRL_CMD );
	HANDLER_AW_DECL( AW_ABUSING_USER_VERIFY_QUERY_CMD);

    HANDLER_AW_DECL( AW_STATUS_GROUP_SET_MEMBERINFO_CMD );
    HANDLER_AW_DECL( AW_STATUS_GROUP_ADDEXP_CMD );
    HANDLER_AW_DECL( AW_STATUS_GROUP_ADDMONEY_CMD );
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    HANDLER_AW_DECL( AW_USER_LOTTO_REWARD_SYN );
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    HANDLER_AW_DECL( AW_USER_LOTTO_TICKET_PURCHASE_SYN );
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    //}
    HANDLER_AW_DECL( AW_USER_DOMINATION_RETURNHIEM_CMD );
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // 파티 모집에서 파티 참가 신청이 오면 호출된다.
    HANDLER_AW_DECL(AW_PARTY_FINDING_JOIN_SYN);
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
};
