#include "stdafx.h"

#include <Const.h>
#include <PacketStruct_CG.h>
#include <PacketControl/PacketControl.h>
#include <PacketControl/CategoryLinkPair.h>
#include <PacketControl/PacketControl_Compress.h>
#include "PacketControlS2S.h"

namespace nsPacket { namespace Server {
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
BOOLEAN		Environment::Init()
{
	static BOOLEAN	sbInit = FALSE;
	if( sbInit )
		return TRUE;
	sbInit = TRUE;

	nsPacket::Environment::Init();

	return TRUE;
}

BYTE		PacketControl::ms_PacketStream[SIZEofSTREAM];

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<PacketControl::Pack_Unpack>
PacketInfo	PacketControl::SubControl<GAME_SERVER, AGENT_SERVER>::MakeStream( MSG_BASE* const pMsg, const WORD wSize )
{
	BYTE* const pIOStream = ms_PacketStream;

	PacketInfo packetInfo = { pMsg, wSize };

	const BYTE category = pMsg->m_byCategory;
	//	GameServer->in AgentServer(received)->forwarding to Client
	//	AgentServer에서 값을 알아야 하는 경우라면 압축/해제 등의 처리가 Agent->Client에서만 이뤄지도록 할 것.
	if( nsPacket::CategoryConverter::IsCategoryPacked( category ) )
		return packetInfo;

	const WORD protocol = MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol );
	switch( protocol )
	{
		//	GameServer->AgentServer
		//	in GameServer -> Packing
	case MAKEWORD( CG_SYNC,			CG_SYNC_VILLAGE_ALLPLAYERS_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLPLAYERS_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLPLAYERS_EQUIPINFO_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLMONSTERS_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLSUMMONEDS_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLMAPNPC_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLTOTEMNPC_CMD ):
	case MAKEWORD( CG_SYNC,			CG_SYNC_ALLFIELDITEMS_CMD ):
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    case MAKEWORD( CG_SYNC,			CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD ):
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
	case MAKEWORD( CG_CHARINFO,		CG_CHARINFO_CHAR_ITEM_CMD ):
	case MAKEWORD( CG_CHARINFO,		CG_CHARINFO_SKILL_CMD ):
	case MAKEWORD( CG_CHARINFO,		CG_CHARINFO_QUICK_CMD ):
	case MAKEWORD( CG_CHARINFO,		CG_CHARINFO_STYLE_CMD ):
    //_NA002217_100728_EP2_PERK_SYSTEM_
    case MAKEWORD(CG_CHARINFO,      CG_CHARINFO_SKILL_LIST_CMD):
    case MAKEWORD(CG_CHARINFO,      CG_CHARINFO_SKILL_PERK_CMD):
	//case MAKEWORD( AG_MISSION,		AG_MISSION_STREAM_CMD ):
    case MAKEWORD(CG_AREACONQUEST, CG_AREACONQUEST_STREAM_CMD): // CHANGES: f110610.3L
	case MAKEWORD( CG_GUILD,		CG_GUILD_TOTALINFO_REQ_ACK ):
		packetInfo = nsPacket::CompressUtil::MakeStream( TRUE, pIOStream, pMsg, wSize );
		break;
	};

	return packetInfo;
};

}}; //end of namespace

