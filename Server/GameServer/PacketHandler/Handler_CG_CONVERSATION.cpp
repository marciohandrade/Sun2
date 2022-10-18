#include "StdAfx.h"
#include ".\handler_cg_conversation.h"
#include "Player.h"
#include "GameField.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include <ServerSession.h>

Handler_CG_CONVERSATION::Handler_CG_CONVERSATION()
{
}

Handler_CG_CONVERSATION::~Handler_CG_CONVERSATION()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_CONVERSATION::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_CONVERSATION::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_CONVERSATION, CG_CONVERSATION_FIELDCHAT_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


VOID Handler_CG_CONVERSATION::OnCG_CONVERSATION_FIELDCHAT_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CONVERSATION_FIELDCHAT_SYN * pRecvMsg = (MSG_CG_CONVERSATION_FIELDCHAT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if (!pPlayer) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_CONVERSATION::OnCG_CONVERSATION_FIELDCHAT_SYN] Not Exist GameField" );
		return;
	}

	BOOL bBroadcast = TRUE;
	
	if( MAX_CHAT_LEN < pRecvMsg->m_byMsgLength)
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_CONVERSATION::OnCG_CONVERSATION_FIELDCHAT_SYN] Warnig: 클라이언트에서 보낸 채팅 최대 메세지 길이 초과!! ");
		return;
	}

	// 필드에 있는 모든 사람에게 broadcast
	GameField *pField = pPlayer->GetField();
	if( pField )
	{
		if( bBroadcast )
		{
			MSG_CG_CONVERSATION_FIELDCHAT_BRD sendMsg;
			sendMsg.m_byCategory	= CG_CONVERSATION;
			sendMsg.m_byProtocol	= CG_CONVERSATION_FIELDCHAT_BRD;
			sendMsg.m_dwObjectKey	= pRecvMsg->m_dwObjectKey;
			if( MSG_CG_CONVERSATION_FIELDCHAT_BRD::_MAX_CHATMSG_SIZE < pRecvMsg->m_byMsgLength )
				return;	// NAK 처리할 필요까징 없겠징?
			sendMsg.m_byMsgLength	= pRecvMsg->m_byMsgLength;

			memcpy( sendMsg.m_pszChatMsg, pRecvMsg->m_pszChatMsg, pRecvMsg->m_byMsgLength );
			// <Waverix> 유니코드일 때 문제점 생각해 볼 것
			sendMsg.m_pszChatMsg[pRecvMsg->m_byMsgLength] = '\0';
			pField->SendPacketToAll( &sendMsg, (WORD)sendMsg.GetSize(), pPlayer->GetObjectKey() );
		}
	}
	else
	{
		SUNLOG( eCRITICAL_LOG,  "[Handler_CG_CONVERSATION::OnCG_CONVERSATION_FIELDCHAT_SYN] Warning: 채팅 메시지를 보낸 플레이어(ObjectKey:%d)의 GameField 포인터가 NULL", pPlayer->GetObjectKey() );
	}
}