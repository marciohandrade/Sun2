#include "StdAfx.h"
#include "Handler_CG_STYLE.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include "GameField.h"
#include "Player.h"
#include "SCSkillSlotContainer.h"
#include "ItemManager.h"
#include "QuickStyleManager.h"

#include "SkillSlotManager.h"

Handler_CG_STYLE::Handler_CG_STYLE()
{
}

Handler_CG_STYLE::~Handler_CG_STYLE()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_STYLE::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_STYLE::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_STYLE,  CG_STYLE_SELECT_STYLE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STYLE,  CG_STYLE_LINK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STYLE,  CG_STYLE_UNLINK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_STYLE,  CG_STYLE_LINKMOVE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


VOID Handler_CG_STYLE::OnCG_STYLE_SELECT_STYLE_SYN( ServerSession * pServerSession, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    ASSERT(!"don't support anymore");
    return;
#else //if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
	MSG_CG_STYLE_SELECT_STYLE_SYN *pRecvMsg = (MSG_CG_STYLE_SELECT_STYLE_SYN*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STYLE::OnCG_STYLE_SELECT_STYLE_SYN] Not Exist GameField" );
		return;
	}

	RC::eSKILL_RESULT rcResult = pPlayer->GetSkillSlotManager()->CanSelectStyle( pRecvMsg->m_NewStyleCode );

	if( rcResult == RC::RC_SKILL_SUCCESS )
	{
		// 새로운 스타일을 지정한다.(내부에서 브로드를 날린다.)
		pPlayer->GetSkillSlotManager()->SetSelectStyle( pRecvMsg->m_NewStyleCode );
	}
	else
	{
		MSG_CG_STYLE_SELECT_STYLE_NAK NakMsg;
		NakMsg.m_CurStyleCode = pPlayer->GetSelectedStyle();
		NakMsg.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &NakMsg, sizeof(NakMsg) );
	}
#endif //_NA002217_100728_EP2_PERK_SYSTEM_
}

Handler_CG_STYLE_IMPL( CG_STYLE_LINK_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    ASSERT(!"don't support anymore");
    return;
#else //if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
	MSG_CG_STYLE_LINK_SYN *pRecvMsg = (MSG_CG_STYLE_LINK_SYN*)pMsg;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STYLE_IMPL( CG_STYLE_LINK_SYN )] Not Exist GameField" );
		return;
	}

	// Style은 클라가 임의로 등록, 해제할 수 없으므로 무조건 Fail!
	MSG_CG_STYLE_LINK_NAK msg;
	msg.m_byErrorCode	= RC::RC_STYLE_FAILED;
	pPlayer->SendPacket( &msg, sizeof( msg ) );

/*	RC::eSTYLE_RESULT rt = pPlayer->GetStyleManager()->Link( pRecvMsg->m_StyleCode, pRecvMsg->m_ToPos );
	switch( rt )
	{
	case RC::RC_STYLE_SUCCESS:
		{
			MSG_CG_STYLE_LINK_ACK msg;
			msg.m_StyleCode		= pRecvMsg->m_StyleCode;
			msg.m_ToPos			= pRecvMsg->m_ToPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_STYLE_LINK_NAK msg;
			msg.m_dwErrorCode	= rt;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
	}*/
#endif //_NA002217_100728_EP2_PERK_SYSTEM_
}

Handler_CG_STYLE_IMPL( CG_STYLE_UNLINK_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    ASSERT(!"don't support anymore");
    return;
#else //if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
	MSG_CG_STYLE_UNLINK_SYN *pRecvMsg = (MSG_CG_STYLE_UNLINK_SYN*)pMsg;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STYLE_IMPL( CG_STYLE_UNLINK_SYN )] Not Exist GameField" );
		return;
	}

	// Style은 클라가 임의로 등록, 해제할 수 없으므로 무조건 Fail!
	MSG_CG_STYLE_UNLINK_NAK msg;
	msg.m_byErrorCode	= RC::RC_STYLE_FAILED;
	pPlayer->SendPacket( &msg, sizeof( msg ) );

/*	RC::eSTYLE_RESULT rt = pPlayer->GetStyleManager()->Unlink( pRecvMsg->m_atPos );
	switch( rt )
	{
	case RC::RC_STYLE_SUCCESS:
		{
			MSG_CG_STYLE_UNLINK_ACK msg;
			msg.m_atPos			= pRecvMsg->m_atPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_STYLE_UNLINK_NAK msg;
			msg.m_dwErrorCode	= rt;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
	}*/
#endif //_NA002217_100728_EP2_PERK_SYSTEM_
}

Handler_CG_STYLE_IMPL( CG_STYLE_LINKMOVE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    ASSERT(!"don't support anymore");
    return;
#else //if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
	MSG_CG_STYLE_LINKMOVE_SYN *pRecvMsg = (MSG_CG_STYLE_LINKMOVE_SYN*)pMsg;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_STYLE_IMPL( CG_STYLE_LINKMOVE_SYN )] Not Exist GameField" );
		return;
	}

	// Style은 클라가 임의로 등록, 해제할 수 없으므로 무조건 Fail!
	MSG_CG_STYLE_LINKMOVE_NAK msg;
	msg.m_byErrorCode	= RC::RC_STYLE_FAILED;
	pPlayer->SendPacket( &msg, sizeof( msg ) );

/*	RC::eSTYLE_RESULT rt = pPlayer->GetStyleManager()->Move( pRecvMsg->m_fromPos, pRecvMsg->m_toPos );
	switch( rt )
	{
	case RC::RC_STYLE_SUCCESS:
		{
			MSG_CG_STYLE_LINKMOVE_ACK msg;
			msg.m_fromPos		= pRecvMsg->m_fromPos;
			msg.m_toPos			= pRecvMsg->m_toPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_STYLE_LINKMOVE_NAK msg;
			msg.m_dwErrorCode	= rt;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
	}*/
#endif //_NA002217_100728_EP2_PERK_SYSTEM_
}


















