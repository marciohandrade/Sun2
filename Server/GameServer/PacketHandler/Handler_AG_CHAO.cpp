#include "StdAfx.h"
#include "Handler_AG_CHAO.h"
#include <PacketStruct_AG.h>
#include "Player.h"
#include <ServerSession.h>
#include "ChaoState.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_CHAO::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_CHAO::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_REQUEST_REVENGEINFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_REQUEST_REVENGEINFO_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_REMOVE_REVENGE_CHAR_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_REMOVE_REVENGE_CHAR_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_REMOVE_REVENGE_CHAR_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_ADD_REVENGE_CHAR_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_ADD_REVENGE_CHAR_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CHAO,  AG_CHAO_TRACK_REVENGE_CHAR_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

// 리벤지 정보
Handler_AG_CHAO_IMPL( AG_CHAO_REQUEST_REVENGEINFO_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REQUEST_REVENGEINFO_ACK *pRecvMsg = (MSG_AG_CHAO_REQUEST_REVENGEINFO_ACK*)pMsg;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	if( pRecvMsg->m_byRevengeStatNum > MAX_REVENGE_STAT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[AG_CHAO_REQUEST_REVENGEINFO_ACK] Invalid m_byRevengeStatNum[%d]", pRecvMsg->m_byRevengeStatNum );
		return;
	}

	// 리벤지 정보 저장
	pPlayer->GetChaoState().ClearBadFriend();
	for( int i = 0; i < pRecvMsg->m_byRevengeStatNum; ++i )
	{
		STRUCT_REVENGE_INFO & sRevengeInfo = pRecvMsg->m_sRevengeUserInfo[i];
		pPlayer->GetChaoState().AddBadFriend( sRevengeInfo );
	}
}

Handler_AG_CHAO_IMPL( AG_CHAO_REQUEST_REVENGEINFO_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REQUEST_REVENGEINFO_NAK *pRecvMsg = (MSG_AG_CHAO_REQUEST_REVENGEINFO_NAK*)pMsg;

//	SUNLOG( eCRITICAL_LOG, "[AG_CHAO_REQUEST_REVENGEINFO_NAK] CharGuid[%d]  ErrorCode[%d]", pRecvMsg->m_dwCharGuid, pRecvMsg->m_byErrorCode );
}

// 리벤지 대상 삭제(클라이언트가 삭제했을 때)
Handler_AG_CHAO_IMPL( AG_CHAO_REMOVE_REVENGE_CHAR_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_CMD *pRecvMsg = (MSG_AG_CHAO_REMOVE_REVENGE_CHAR_CMD*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	pPlayer->GetChaoState().RemoveBadFriend( pRecvMsg->m_dwRevengeCharGuid );
}

// 리벤지 대상 한명 제거(PK를 통해 제거했을 때)
Handler_AG_CHAO_IMPL( AG_CHAO_REMOVE_REVENGE_CHAR_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_ACK *pRecvMsg = (MSG_AG_CHAO_REMOVE_REVENGE_CHAR_ACK*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	pPlayer->GetChaoState().RemoveBadFriend( pRecvMsg->m_dwRevengeCharGuid );
}

Handler_AG_CHAO_IMPL( AG_CHAO_REMOVE_REVENGE_CHAR_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_NAK *pRecvMsg = (MSG_AG_CHAO_REMOVE_REVENGE_CHAR_NAK*)pMsg;

	SUNLOG( eCRITICAL_LOG, "[AG_CHAO_REMOVE_REVENGE_CHAR_NAK] CharGuid[%d]  ErrorCode[%d]", pRecvMsg->m_dwCharGuid, pRecvMsg->m_byErrorCode );
}

// 리벤지 대상 추가
Handler_AG_CHAO_IMPL( AG_CHAO_ADD_REVENGE_CHAR_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_ADD_REVENGE_CHAR_ACK *pRecvMsg = (MSG_AG_CHAO_ADD_REVENGE_CHAR_ACK*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	STRUCT_REVENGE_INFO RevengeInfo;
	RevengeInfo.dwRevengeCharGuid = pRecvMsg->m_dwRevengeCharGuid;
	_tcsncpy( RevengeInfo.sPacketInfo.ptszRevengeName, pRecvMsg->m_ptszRevengeName, MAX_CHARNAME_LENGTH );
	RevengeInfo.sPacketInfo.ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
	pPlayer->GetChaoState().AddBadFriend( RevengeInfo );
}

Handler_AG_CHAO_IMPL( AG_CHAO_ADD_REVENGE_CHAR_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_ADD_REVENGE_CHAR_NAK *pRecvMsg = (MSG_AG_CHAO_ADD_REVENGE_CHAR_NAK*)pMsg;

	SUNLOG( eCRITICAL_LOG, "[AG_CHAO_ADD_REVENGE_CHAR_NAK] CharGuid[%d]  ErrorCode[%d]", pRecvMsg->m_dwCharGuid, pRecvMsg->m_byErrorCode );
}

// 리벤지 대상 추적 성공
Handler_AG_CHAO_IMPL( AG_CHAO_TRACK_REVENGE_CHAR_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_TRACK_REVENGE_CHAR_ACK *pRecvMsg = (MSG_AG_CHAO_TRACK_REVENGE_CHAR_ACK*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	// 복수의 돌 아이템을 삭제한다.
	pPlayer->GetChaoState().DeleteRevengeStone( pRecvMsg->m_atItemPos );
}
