#include "stdafx.h"
#include ".\Handler_CG_COLLECT.h"

#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include <ServerSession.h>
#include <PacketControl/WarMessages_Sync.h>

#include "CollectionManager.h"

Handler_CG_COLLECT::Handler_CG_COLLECT()
{
}

Handler_CG_COLLECT::~Handler_CG_COLLECT()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_COLLECT::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_COLLECT::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //{__NA001283_20090221_COLLECTION_PACKET
        HANDLER_NODE_INFO(CG_COLLECT,  CG_COLLECT_ACTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_COLLECT,  CG_COLLECT_CANCEL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_COLLECT,  CG_COLLECT_ALLCOLLECTIVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_COLLECT,  CG_COLLECT_SUCCESS_SYN, PHASE_SERVICE),
        //}
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------



VOID Handler_CG_COLLECT::OnCG_COLLECT_ALLCOLLECTIVE_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_COLLECT_ALLCOLLECTIVE_SYN* pRecvMsg = (MSG_CG_COLLECT_ALLCOLLECTIVE_SYN*)pMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;	//< 플레이어 세션이 끊어졌을 때

	GameField* pField = pPlayer->GetField();
	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_COLLECT::OnCG_COLLECT_ALLCOLLECTIVE_SYN] Invalid Packet" );
		return;
	}

	pPlayer->SendAllFieldCollectionInfo();
}

VOID Handler_CG_COLLECT::OnCG_COLLECT_ACTION_SYN (ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	const DWORD ERRCODE_HACKED = 1000000;
	const BOOLEAN packet_integrity = (sizeof(MSG_CG_COLLECT_ACTION_SYN) == wSize);

    Player* player = NULL;

    //////////////////////////////////////////////////////////////////////////
	FlowControl flow;
	FLOWCONTROL_START()
    {
		IfFailThenBreak(packet_integrity, ERRCODE_HACKED);
        MSG_CG_COLLECT_ACTION_SYN* const recv_msg = static_cast<MSG_CG_COLLECT_ACTION_SYN*>(pMsg);

        player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
        if (player == NULL)
        {
            return;
        }

        GameField* field = player->GetField();
        if (field == NULL) 
        {
            return;
        }

		CollectionManager* collection_manager = field->GetCollectManeger();
		RC::eCOLLECT_RESULT result_code = 
            collection_manager->Collect(recv_msg->m_ObjKeyCollection, player, recv_msg->m_wvCurPos );

		IfFailThenBreak(result_code == RC::RC_COLLECT_SUCCESS, result_code);

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	const DWORD error_code = flow.GetErrorCode();
	switch(error_code)
	{
	case ERRCODE_HACKED:
		SUNLOG(eDEV_LOG, "[RNDPROTOCOL_REPORT][SIZE] MSG_CG_COLLECT_ACTION_SYN %u!=%u",
			   sizeof(MSG_CG_COLLECT_ACTION_SYN), wSize );
		return;
	}
    //////////////////////////////////////////////////////////////////////////

	player->StopMoving();
	player->SetMoveState( CMS_STOP );

	MSGSUB_SYNC_STOP_BRD brd_msg;
	brd_msg.m_dwObjectKey = player->GetObjectKey();
	brd_msg.m_wvCurPos = *player->GetPosPtr(); 
	player->SendPacketAroundForSync(&brd_msg);

	assert(SAFE_NUMERIC_TYPECAST(DWORD, error_code, BYTE));

	MSG_CG_COLLECT_ACTION_NAK nak_msg;
	nak_msg.m_byErrorCode = (BYTE)error_code;
	player->SendPacket(&nak_msg, sizeof(nak_msg));
	return;
}

VOID Handler_CG_COLLECT::OnCG_COLLECT_CANCEL_SYN( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_COLLECT_CANCEL_SYN* pRecvMsg = (MSG_CG_COLLECT_CANCEL_SYN*)pMsg;

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;	//< 플레이어 세션이 끊어졌을 때

	GameField* pField = pPlayer->GetField();
	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_COLLECT::OnCG_COLLECT_CANCEL_SYN] Invalid Packet" );
		return;
	}

	CollectionManager* pMng = pField->GetCollectManeger();
	RC::eCOLLECT_RESULT rc  = pMng->Cancel( pPlayer );
	if( rc != RC::RC_COLLECT_SUCCESS )
    {
        MSG_CG_COLLECT_ACTION_NAK msgNAK;
        msgNAK.m_byErrorCode = (BYTE)rc;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }
}


VOID Handler_CG_COLLECT::OnCG_COLLECT_SUCCESS_SYN( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_COLLECT_SUCCESS_SYN* pRecvMsg = (MSG_CG_COLLECT_SUCCESS_SYN*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if( !pPlayer ) return ;	//< 플레이어 세션이 끊어졌을 때

    GameField* pField = pPlayer->GetField();
    if( !pField )
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_COLLECT::OnCG_COLLECT_SUCCESS_SYN] Invalid Packet" );
        return;
    }

    CollectionManager* pMng = pField->GetCollectManeger();
    RC::eCOLLECT_RESULT rc  = pMng->Success( pPlayer );
    if( rc != RC::RC_COLLECT_SUCCESS )
    {
        MSG_CG_COLLECT_ACTION_NAK msgNAK;
        msgNAK.m_byErrorCode = (BYTE)rc;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }
}
