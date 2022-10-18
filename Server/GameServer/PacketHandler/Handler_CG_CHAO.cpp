#include "StdAfx.h"
#include "Handler_CG_CHAO.h"
#include <PacketStruct_CG.h>
#include "PlayerManager.h"
#include <PacketStruct_AG.h>
#include "Player.h"
#include "ChaoState.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_CHAO::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_CHAO::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_CHAO,  CG_CHAO_ADD_REVENGE_CHAR_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_CHAO,  CG_CHAO_TRACK_REVENGE_CHAR_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_CHAO_IMPL(CG_CHAO_ADD_REVENGE_CHAR_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHAO_ADD_REVENGE_CHAR_SYN *pRecvMsg = (MSG_CG_CHAO_ADD_REVENGE_CHAR_SYN*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_CHAO_IMPL(CG_CHAO_ADD_REVENGE_CHAR_SYN)] Not Exist GameField" );
		return;
	}

	// 정말 PK를 당한 것이면 월드에 리벤지 추가를 요청한다.(G->A->W)
	if( pPlayer->GetChaoState().IsRevengeName( pRecvMsg->m_ptszRevengeName ) )
	{
		MSG_AG_CHAO_ADD_REVENGE_CHAR_SYN AGMsg;
		_tcsncpy( AGMsg.m_ptszRevengeName, pRecvMsg->m_ptszRevengeName, MAX_CHARNAME_LENGTH );
		AGMsg.m_ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
		pPlayer->SendPacket( &AGMsg, sizeof(AGMsg) );
	}
	else
	{
		MSG_CG_CHAO_ADD_REVENGE_CHAR_NAK CGMsg;
		CGMsg.m_byErrorCode = RC::RC_REVENGE_INVALID_SITUATION;
		pPlayer->SendPacket( &CGMsg, sizeof(CGMsg) );
	}
}

Handler_CG_CHAO_IMPL(CG_CHAO_TRACK_REVENGE_CHAR_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHAO_TRACK_REVENGE_CHAR_SYN *pRecvMsg = (MSG_CG_CHAO_TRACK_REVENGE_CHAR_SYN*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )	return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_CHAO_IMPL(CG_CHAO_TRACK_REVENGE_CHAR_SYN)] Not Exist GameField" );
		return;
	}

	// 사용할 수 있는 아이템인지 체크
	RC::eREVENGE_RESULT rt = pPlayer->GetChaoState().TrackRevenge( pRecvMsg->m_ptszRevengeName, pRecvMsg->m_atItemPos );
	if( rt != RC::RC_REVENGE_SUCCESS )
	{
		MSG_CG_CHAO_TRACK_REVENGE_CHAR_NAK nakMsg;
		nakMsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nakMsg, sizeof( nakMsg ) );
	}
}














































