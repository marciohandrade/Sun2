#include "stdafx.h"
#include "PacketHandler.h"
#include <Macro.h>
#include "Player.h"

#include <Protocol_CG.h>
#include <Protocol_DG.h>
#include <Protocol_GM.h>
#include <Protocol_AG.h>
#include <Protocol_GZ.h>
#include <Protocol_GP.h>
#include <Protocol_GS.h>

#include "Handler_DG_CHARINFO.h"
#include "Handler_AG_CHARINFO.h"
#include "Handler_AG_ZONE.h"

#include "Handler_DG_CONNECTION.h"
#include "Handler_AG_CONNECTION.h"
#include "Handler_CG_CONNECTION.h"
#include "Handler_AG_EVENT.h"

#include "Handler_CG_CHARINFO.h"
#include "Handler_CG_MAP.h"
#include "Handler_CG_SYNC.h"
#include "Handler_CG_BATTLE.h"
#include "Handler_CG_SKILL.h"
#include "Handler_CG_STYLE.h"
#include "Handler_CG_CONVERSATION.h"
#include "Handler_CG_ROOM.h"
#include "Handler_CG_ITEM.h"
#include "Handler_CG_TRADE.h"
#include "Handler_CG_TRIGGER.h"
#include "Handler_CG_GM.h"
#include "Handler_CG_STATUS.h"
#include "Handler_AG_PARTY.h"
#include "Handler_CG_PARTY.h"
#include "Handler_CG_VENDOR.h"
#include "Handler_CG_WAREHOUSE.h"
#include "Handler_CG_SUMMON.h"
#include "Handler_CG_PVP.h"
#include "Handler_CG_GUILD.h"

#include "Handler_GZ_GUILD.h"
#include "Handler_CG_EVENT.h"
#include "Handler_CG_QUEST.h"
#include "Handler_GM.h"
#include "Handler_AG_GM.h"
#include "Handler_AG_CHUNTING.h"
#include "Handler_CG_CHUNTING.h"
#include "AgentServerSession.h"
#include "GameDBProxySession.h"
#include "MasterServerSession.h"
#include "Handler_CG_AREACONQUEST.h"
#include "Handler_CG_WAYPOINT.h"
#include "Handler_AG_CHAO.h"
#include "Handler_CG_CHAO.h"
#include "Handler_PortalServer.h"
#include "Handler_Client.h"
#include "Handler_AG_STATUS.h"
#include "Handler_AG_ITEM.h"
#include "Handler_ShopServer.h"
#include "Handler_CG_COLLECT.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    #include "Handler_CG_SHOP.h"
#endif


// 서버 정상종료 프로토콜 위해
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>


PacketHandler::PacketHandler()
{
	BOOL rt = Register();
	ASSERT(rt);
}

PacketHandler::~PacketHandler()
{

}

bool PacketHandler::Register()
{
#define BATCH_REGISTERING(group_name, handler_type, nodes) \
    if (!_RegisterPartialBlock((group_name), (handler_type), (nodes))) { \
        return false; \
    }
    //
    // f100819.1L
    BATCH_REGISTERING(_T("AG_CONNECTION"), PACKET_HANDLER_AGENT,
                      Handler_AG_CONNECTION::GetPacketHandlerNodes());

    BATCH_REGISTERING(_T("AG_CHAO"), PACKET_HANDLER_AGENT,
                      Handler_AG_CHAO::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_CHARINFO"), PACKET_HANDLER_AGENT,
                      Handler_AG_CHARINFO::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_CHUNTING"), PACKET_HANDLER_AGENT,
                      Handler_AG_CHUNTING::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_EVENT"), PACKET_HANDLER_AGENT,
                      Handler_AG_EVENT::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_GM"), PACKET_HANDLER_AGENT,
                      Handler_AG_GM::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_ITEM"), PACKET_HANDLER_AGENT,
                      Handler_AG_ITEM::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_PARTY"), PACKET_HANDLER_AGENT,
                      Handler_AG_PARTY::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_STATUS"), PACKET_HANDLER_AGENT,
                      Handler_AG_STATUS::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("AG_ZONE"), PACKET_HANDLER_AGENT,
                      Handler_AG_ZONE::GetPacketHandlerNodes());
    //
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    BATCH_REGISTERING(_T("CG_SHOP"), PACKET_HANDLER_AGENT,
                      Handler_CG_SHOP::GetPacketHandlerNodes());
#else
    BATCH_REGISTERING(_T("Game<-Shop"), PACKET_HANDLER_SHOP,
                      Handler_ShopServer::GetPacketHandlerNodes());
#endif //

#ifdef _NA_008012_20150130_RANKING_SYSTEM
    //
    BATCH_REGISTERING(_T("Game<-Ranking"), PACKET_HANDLER_RANKING,
                      Handler_RankingServer::GetPacketHandlerNodes());
#endif // _NA_008012_20150130_RANKING_SYSTEM

    //
    BATCH_REGISTERING(_T("Game<-Master"), PACKET_HANDLER_MASTER,
                      Handler_GM::GetPacketHandlerNodes());
    //
    BATCH_REGISTERING(_T("Game<-Guild"), PACKET_HANDLER_GUILDDB,
                      Handler_GZ_GUILD::GetPacketHandlerNodes());
    //
    BATCH_REGISTERING(_T("Game<-DBProxy CONNECTION"), PACKET_HANDLER_DBPROXY,
                      Handler_DG_CONNECTION::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("Game<-DBProxy CHARINFO"), PACKET_HANDLER_DBPROXY,
                      Handler_DG_CHARINFO::GetPacketHandlerNodes());
    //
    BATCH_REGISTERING(_T("Game<-(Client|Agent) generic|shop"), PACKET_HANDLER_AGENT,
                      Handler_Client::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_AREACONQUEST"), PACKET_HANDLER_AGENT,
                      Handler_CG_AREACONQUEST::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_BATTLE"), PACKET_HANDLER_AGENT,
                      Handler_CG_BATTLE::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_CHAO"), PACKET_HANDLER_AGENT,
                      Handler_CG_CHAO::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_CHARINFO"), PACKET_HANDLER_AGENT,
                      Handler_CG_CHARINFO::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_CHUNTING"), PACKET_HANDLER_AGENT,
                      Handler_CG_CHUNTING::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_COLLECT"), PACKET_HANDLER_AGENT,
                      Handler_CG_COLLECT::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_CONNECTION"), PACKET_HANDLER_AGENT,
                      Handler_CG_CONNECTION::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_CONVERSATION"), PACKET_HANDLER_AGENT,
                      Handler_CG_CONVERSATION::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_EVENT"), PACKET_HANDLER_AGENT,
                      Handler_CG_EVENT::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_GM"), PACKET_HANDLER_AGENT,
                      Handler_CG_GM::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_GUILD"), PACKET_HANDLER_AGENT,
                      Handler_CG_GUILD::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_ITEM"), PACKET_HANDLER_AGENT,
                      Handler_CG_ITEM::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_MAP"), PACKET_HANDLER_AGENT,
                      Handler_CG_MAP::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_PARTY"), PACKET_HANDLER_AGENT,
                      Handler_CG_PARTY::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_PVP"), PACKET_HANDLER_AGENT,
                      Handler_CG_PVP::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_QUEST"), PACKET_HANDLER_AGENT,
                      Handler_CG_QUEST::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_ROOM"), PACKET_HANDLER_AGENT,
                      Handler_CG_ROOM::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_SKILL"), PACKET_HANDLER_AGENT,
                      Handler_CG_SKILL::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_STATUS"), PACKET_HANDLER_AGENT,
                      Handler_CG_STATUS::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_STYLE"), PACKET_HANDLER_AGENT,
                      Handler_CG_STYLE::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_SUMMON"), PACKET_HANDLER_AGENT,
                      Handler_CG_SUMMON::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_SYNC"), PACKET_HANDLER_AGENT,
                      Handler_CG_SYNC::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_TRADE"), PACKET_HANDLER_AGENT,
                      Handler_CG_TRADE::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_TRIGGER"), PACKET_HANDLER_AGENT,
                      Handler_CG_TRIGGER::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_VENDOR"), PACKET_HANDLER_AGENT,
                      Handler_CG_VENDOR::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_WAREHOUSE"), PACKET_HANDLER_AGENT,
                      Handler_CG_WAREHOUSE::GetPacketHandlerNodes());
    BATCH_REGISTERING(_T("CG_WAYPOINT"), PACKET_HANDLER_AGENT,
                      Handler_CG_WAYPOINT::GetPacketHandlerNodes());
	return true;
}

// f100819.1L, a batched process for a partial protocol group
bool PacketHandler::_RegisterPartialBlock(const TCHAR* group_name, ePACKET_HANDLER handler_type,
                                          const nsPacket::PacketHandlerInfo& regist_info)
{
    util::FunctionMap* function_map = &m_FuncMap[handler_type];
    const nsPacket::PacketHandlerNode* it = regist_info.node_array_,
                                     * end = regist_info.node_array_ + regist_info.number_of_array_;
    for ( ; it != end; ++it)
    {
        HANDLER_FUNC* func_info = new HANDLER_FUNC;
        func_info->m_dwFunctionKey = it->category_and_protocol_;
        func_info->m_fnHandler = it->handler_;
        bool is_added = (function_map->Add(func_info) != false);
        if (is_added == false) {
            break;
        }
    }

    if (it != end)
    {   //occurred fail
        const int order = static_cast<int>(it - regist_info.node_array_);
        static const char* order_postfix[4] = { _T("st"), _T("nd"), _T("rd"), _T("th") };
        const TCHAR* postfix = order < 3 ? order_postfix[order] : order_postfix[3];
                                
        SUNLOG(eCRITICAL_LOG,
               _T("|Msg=conflict protocol (%3d:%3d)|Pos=%3d%s in '%s'|Action=stop registering|"),
               it->category_, it->protocol_,
               order + 1, postfix,
               group_name);
        __debugbreak(); // assert
        return false;
    }
    return true;
}

bool PacketHandler::AddHandler( ePACKET_HANDLER PHandler, BYTE category, BYTE protocol, fnHandler fnHandler )
{
	HANDLER_FUNC * pFuncInfo	= new HANDLER_FUNC;
	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_FuncMap[PHandler].Add(pFuncInfo) != false;
}

VOID PacketHandler::ParsePacket( ePACKET_HANDLER PHandler, ServerSession * pSession, MSG_BASE *pMsg, WORD wSize )
{
	if( pMsg->m_byCategory == 0xff )
	{
	}

	//SUNLOG( eFULL_LOG, "[PHandler:%u,C:%u,P:%u]", PHandler, pMsg->m_byCategory, pMsg->m_byProtocol );
	HANDLER_FUNC * pFuncInfo = (HANDLER_FUNC *)m_FuncMap[PHandler].Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		SUNLOG( eCRITICAL_LOG, "[PacketHandler::ParsePacket] Invalid packet! Category[%u] Protocol[%u] ", pMsg->m_byCategory, pMsg->m_byProtocol );
		ASSERT( ! "Invalid packet from Server!" ) ;

		return ;
	}

	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}

VOID PacketHandler::ParsePacket( ePACKET_HANDLER PHandler, ServerSession * pSession, MSG_BASE_INTERNAL *pMsg, WORD wSize )
{
	if( pMsg->m_byCategory == 0xff )
	{
	}

	//SUNLOG( eFULL_LOG, "[PHandler:%u,C:%u,P:%u]", PHandler, pMsg->m_byCategory, pMsg->m_byProtocol );
	HANDLER_FUNC * pFuncInfo = (HANDLER_FUNC *)m_FuncMap[PHandler].Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		DISPMSG( "[PacketHandler::ParsePacket] Invalid packet! Category[%u] Protocol[%u] ", pMsg->m_byCategory, pMsg->m_byProtocol );
		ASSERT( ! "Invalid packet from Server!" ) ;

		return ;
	}

	pFuncInfo->m_fnHandler( pSession, (MSG_BASE*)pMsg, wSize );
}
