#include "stdafx.h"
#include "PacketHandler.h"
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>
#include <Protocol_CW.h>
#include <PacketStruct_CW.h>
#include <Protocol_MW.h>
#include <Protocol_AW.h>
#include <Protocol_DW.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_DW.h>
#include <PacketStruct_WZ.h>
#include "Handler_CW.h"
#include "Handler_MW.h"
#include "Handler_AW.h"
#include "Handler_DW.h"
#include "Handler_WZ.h"

PACKET_CONTROL::uPACKET_STRUCTURE	PACKET_CONTROL::sRECV_PACKET_STRUCTURE;
PACKET_CONTROL::uPACKET_STRUCTURE	PACKET_CONTROL::sSEND_PACKET_STRUCTURE;

PacketHandler::PacketHandler()
{
	m_pFuncMap_CW	= new util::FunctionMap;
	m_pFuncMap_DW	= new util::FunctionMap;
	m_pFuncMap_TW	= new util::FunctionMap;
	m_pFuncMap_GW	= new util::FunctionMap;
	m_pFuncMap_BW	= new util::FunctionMap;
	m_pFuncMap_FW	= new util::FunctionMap;
	m_pFuncMap_AW	= new util::FunctionMap;
	m_pFuncMap_MW	= new util::FunctionMap;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	m_pFuncMap_WZ	= new util::FunctionMap;
#endif
}

PacketHandler::~PacketHandler()
{
	if( m_pFuncMap_CW )		delete m_pFuncMap_CW;
	if( m_pFuncMap_DW )		delete m_pFuncMap_DW;
	if( m_pFuncMap_TW )		delete m_pFuncMap_TW;
	if( m_pFuncMap_GW )		delete m_pFuncMap_GW;
	if( m_pFuncMap_BW )		delete m_pFuncMap_BW;
	if( m_pFuncMap_FW )		delete m_pFuncMap_FW;
	if( m_pFuncMap_AW )		delete m_pFuncMap_AW;
	if( m_pFuncMap_MW )		delete m_pFuncMap_MW;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	if( m_pFuncMap_WZ )		delete m_pFuncMap_WZ;
#endif
}

BOOL PacketHandler::RegisterPackets()
{
	// From client
	if( !RegClientPacket( CW_CHAT, CW_WHISPER_SYN, Handler_CW::OnCW_WHISPER_SYN ) )			return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_NOTICE_SYN, Handler_CW::OnCW_NOTICE_SYN ) )			return FALSE;
	if( !RegClientPacket( CW_CONNECTION, CW_HEARTBEAT, Handler_CW::OnCW_HEARTBEAT ) )		return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_WHISPER_SET_SYN, Handler_CW::OnCW_WHISPER_SET_SYN ) )		return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_NORMAL_SYN, Handler_CW::OnCW_CHAT_NORMAL_SYN ) )	return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_SHOUT_SYN, Handler_CW::OnCW_CHAT_SHOUT_SYN ) )	return FALSE;
	if( !RegClientPacket( CW_GM, CW_GM_NOTICE_SYN, Handler_CW::OnCW_GM_NOTICE_SYN ) )		return FALSE;
	if( !RegClientPacket( CW_GM, CW_GM_STRING_CMD_SYN, Handler_CW::OnCW_GM_STRING_CMD_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_VIEWPORT, CW_VIEWPORT_CHARSTATE, Handler_CW::OnCW_VIEWPORT_CHARSTATE ) )	return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_NEWLIST_SYN, Handler_CW::OnCW_FRIEND_NEWLIST_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_ADD_SYN, Handler_CW::OnCW_FRIEND_ADD_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_DEL_SYN, Handler_CW::OnCW_FRIEND_DEL_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_BLOCK_SYN, Handler_CW::OnCW_FRIEND_BLOCK_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_BLOCK_FREE_SYN, Handler_CW::OnCW_FRIEND_BLOCK_FREE_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_CHAT_SYN, Handler_CW::OnCW_FRIEND_CHAT_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_CHATBLOCK_SYN, Handler_CW::OnCW_FRIEND_CHATBLOCK_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_FRIEND, CW_FRIEND_STATUS_REQ, Handler_CW::OnCW_FRIEND_STATUS_REQ ) ) return FALSE;
    if( !RegClientPacket( CW_FRIEND, CW_FRIEND_STATUS_JOIN_SYN, Handler_CW::OnCW_FRIEND_STATUS_JOIN_SYN ) ) return FALSE;
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if( !RegClientPacket( CW_FRIEND, CW_FRIEND_MEMO_UPDATE_SYN, Handler_CW::OnCW_FRIEND_MEMO_UPDATE_SYN ) ) return FALSE;

	if( !RegClientPacket( CW_CHAT, CW_CHAT_PARTY_SYN, Handler_CW::OnCW_CHAT_PARTY_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_ACK, Handler_CW::OnCW_CRYPTOGRAPHY_ACK ) ) return FALSE;
	if( !RegClientPacket( CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_NAK, Handler_CW::OnCW_CRYPTOGRAPHY_NAK ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_VOICE_REQUEST_SYN, Handler_CW::OnCW_CHAT_VOICE_REQUEST_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_VOICE_ACCEPT_SYN, Handler_CW::OnCW_CHAT_VOICE_ACCEPT_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_VOICE_REJECT_SYN, Handler_CW::OnCW_CHAT_VOICE_REJECT_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_VOICE_DISCONNECT_SYN, Handler_CW::OnCW_CHAT_VOICE_DISCONNECT_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_SYN, Handler_CW::OnCW_CHAT_VOICE_MIC_STATUS_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_REQ, Handler_CW::OnCW_CHAT_PARTY_MIC_STATUS_REQ ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_REQ, Handler_CW::OnCW_CHAT_PLAYER_MIC_STATUS_REQ ) ) return FALSE;
	if( !RegClientPacket( CW_COMMUNITY, CW_COMM_USERID_REQ, Handler_CW::OnCW_COMM_USERID_REQ ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_CHANNEL_JOIN_SYN, Handler_CW::OnCW_CHAT_CHANNEL_JOIN_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_CHANNEL_SEND_SYN, Handler_CW::OnCW_CHAT_CHANNEL_SEND_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_CHANNEL_INVITE_SYN, Handler_CW::OnCW_CHAT_CHANNEL_INVITE_SYN ) ) return FALSE;
    if (!RegClientPacket(
        CW_CHAT, CW_CHAT_CHANNEL_INVITE_NAK, Handler_CW::OnCW_CHAT_CHANNEL_INVITE_NAK))
    {
        return FALSE;
    }
	if( !RegClientPacket( CW_CHAT, CW_CHAT_CHANNEL_OUT_SYN, Handler_CW::OnCW_CHAT_CHANNEL_OUT_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_CHAT, CW_CHAT_CHANNEL_INFO_REQ, Handler_CW::OnCW_CHAT_CHANNEL_INFO_REQ ) ) return FALSE;
	if( !RegClientPacket( CW_ARMORKIT, CW_ARMORKIT_FINDHACK, Handler_CW::OnCW_ARMORKIT_FINDHACK_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_REVENGE, CW_REVENGE_DEL_SYN, Handler_CW::OnCW_REVENGE_DEL_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_MEMO, CW_MEMO_SEND_SYN, Handler_CW::OnCW_MEMO_SEND_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_MEMO, CW_MEMO_LIST_REQ, Handler_CW::OnCW_MEMO_LIST_REQ ) ) return FALSE;
	if( !RegClientPacket( CW_MEMO, CW_MEMO_VIEW_REQ, Handler_CW::OnCW_MEMO_VIEW_REQ ) ) return FALSE;
	if( !RegClientPacket( CW_MEMO, CW_MEMO_DELETE_SYN, Handler_CW::OnCW_MEMO_DELETE_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_GUILD, CW_GUILD_CHAT_SYN, Handler_CW::OnCW_GUILD_CHAT_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_REQ, Handler_CW::OnCW_GUILD_WAREHOUSE_LOGLIST_REQ ) ) return FALSE;

    if( !RegClientPacket( CW_GUILD, CW_GUILD_CHANGE_NOTICE_SYN, Handler_CW::OnCW_GUILD_CHANGE_NOTICE_SYN ) ) return FALSE;
	if( !RegClientPacket( CW_GUILD, CW_GUILD_SELECT_NOTICE_SYN, Handler_CW::OnCW_GUILD_SELECT_NOTICE_SYN ) ) return FALSE;
    if( !RegClientPacket( CW_GUILD, CW_DOMINATION_RETURN_HEIM_CMD, Handler_CW::OnCW_DOMINATION_RETURN_HEIM_CMD ) ) return FALSE;

    //__KR_001355_20090608_GROUPINGSYSTEM_CW
    if( !RegClientPacket( CW_GROUP, CW_GROUP_ADD_SYN, Handler_CW::OnCW_GROUP_ADD_SYN ) ) return FALSE;                      //그룹 맴버 추가 요청
    if( !RegClientPacket( CW_GROUP, CW_GROUP_GROUPLIST_REQ, Handler_CW::OnCW_GROUP_GROUPLIST_REQ )) return FALSE;           //그룹 맴버 리스트 요청
    if( !RegClientPacket( CW_GROUP, CW_GROUP_ADD_RESPONSE_ACK, Handler_CW::OnCW_GROUP_ADD_RESPONSE_ACK ) ) return FALSE;    //그룹 맴버 추가 성공 응답
    if( !RegClientPacket( CW_GROUP, CW_GROUP_ADD_RESPONSE_NAK, Handler_CW::OnCW_GROUP_ADD_RESPONSE_NAK ) ) return FALSE;    //그룹 맴버 추가 실패 응답
    if( !RegClientPacket( CW_GROUP, CW_GROUP_STATUS_REQ, Handler_CW::OnCW_GROUP_STATUS_REQ ) ) return FALSE;                //그룹 맴버 상세 정보 요청
    if( !RegClientPacket( CW_GROUP, CW_GROUP_DEL_SYN, Handler_CW::OnCW_GROUP_DEL_SYN ) ) return FALSE;                      //그룹 맴버 삭제 요청    
    if( !RegClientPacket( CW_GROUP, CW_GROUP_ADDBLOCK_SYN, Handler_CW::OnCW_GROUP_ADDBLOCK_SYN ) ) return FALSE;            //그룹 맴버 추가 거부 토글
    if( !RegClientPacket( CW_GROUP, CW_GROUP_RESTOR_CMD, Handler_CW::OnCW_GROUP_RESTOR_CMD ) ) return FALSE;                //그룹 맴버 추가 거부 토글

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    if( !RegClientPacket( CW_LOTTO, CW_LOTTO_INFO_SYN, Handler_CW::OnCW_LOTTO_INFO_SYN) ) return FALSE;
    if( !RegClientPacket( CW_LOTTO, CW_LOTTO_ENTRY_SYN, Handler_CW::OnCW_LOTTO_ENTRY_SYN) ) return FALSE;
    if( !RegClientPacket( CW_LOTTO, CW_LOTTO_NOTICE_LIST_SYN, Handler_CW::OnCW_LOTTO_NOTICE_LIST_SYN) ) return FALSE;
    if( !RegClientPacket( CW_LOTTO, CW_LOTTO_NOTICE_CONFIRM, Handler_CW::OnCW_LOTTO_NOTICE_CONFIRM) ) return FALSE;
    //}

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    if( !RegClientPacket( CW_NOTIFICATION,	CW_NOTIFICATION_PREV_SYN, Handler_CW::OnCW_NOTIFICATION_PREV_SYN ) ) return FALSE;
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    if (!RegClientPacket(CW_PARTY, 
                         CW_PARTY_FINDING_REGISTER_SYN, 
                         Handler_CW::OnCW_PARTY_FINDING_REGISTER_SYN))
    {
        return FALSE;
    }

    if (!RegClientPacket(CW_PARTY, 
                         CW_PARTY_FINDING_GET_LIST_SYN, 
                         Handler_CW::OnCW_PARTY_FINDING_GET_LIST_SYN))
    {
        return FALSE;
    }

    if (!RegClientPacket(CW_PARTY, 
                         CW_PARTY_FINDING_JOIN_REQUEST_SYN, 
                         Handler_CW::OnCW_PARTY_FINDING_JOIN_REQUEST_SYN))
    {
        return FALSE;
    }

    if (!RegClientPacket(CW_PARTY, 
                         CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK, 
                         Handler_CW::OnCW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK))
    {
        return FALSE;
    }

    if (!RegClientPacket(CW_PARTY, 
                         CW_PARTY_FINDING_CANCEL_SYN, 
                         Handler_CW::OnCW_PARTY_FINDING_CANCEL_SYN))
    {
        return FALSE;
    }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

	// From Game DB proxy
	if( !RegGameDBProxyPacket( DW_CONNECTION, DW_CONNECTION_SERVER_INFO_CMD, Handler_DW::OnDW_CONNECTION_SERVER_INFO_CMD ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_GETLIST_ACK, Handler_DW::OnDW_FRIEND_GETLIST_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_GETLIST_NAK, Handler_DW::OnDW_FRIEND_GETLIST_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_ADD_ACK, Handler_DW::OnDW_FRIEND_ADD_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_ADD_NAK, Handler_DW::OnDW_FRIEND_ADD_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_DEL_ACK, Handler_DW::OnDW_FRIEND_DEL_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_DEL_NAK, Handler_DW::OnDW_FRIEND_DEL_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_BLOCK_ACK, Handler_DW::OnDW_FRIEND_BLOCK_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_BLOCK_NAK, Handler_DW::OnDW_FRIEND_BLOCK_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_BLOCK_FREE_ACK, Handler_DW::OnDW_FRIEND_BLOCK_FREE_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_BLOCK_FREE_NAK, Handler_DW::OnDW_FRIEND_BLOCK_FREE_NAK ) ) return FALSE;
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_DETAIL_INFO_ACK, Handler_DW::OnDW_FRIEND_DATAIL_INFO_ACK ) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_DETAIL_INFO_NAK, Handler_DW::OnDW_FRIEND_DETAIL_INFO_NAK ) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_MEMO_UPDATE_ACK, Handler_DW::OnDW_FRIEND_MEMO_UPDATE_ACK ) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_FRIEND, DW_FRIEND_MEMO_UPDATE_NAK, Handler_DW::OnDW_FRIEND_MEMO_UPDATE_NAK ) ) return FALSE;

	// 채팅금지/허용
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATBLOCK_ACK, Handler_DW::OnDW_GM_CHATBLOCK_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATBLOCK_NAK, Handler_DW::OnDW_GM_CHATBLOCK_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATALLOW_ACK, Handler_DW::OnDW_GM_CHATALLOW_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATALLOW_NAK, Handler_DW::OnDW_GM_CHATALLOW_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATBLOCK_UPDATE_NAK, Handler_DW::OnDW_GM_CHATBLOCK_UPDATE_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATBLOCKTIME_ANS, Handler_DW::OnDW_GM_CHATBLOCKTIME_ANS ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GM, DW_GM_CHATBLOCKTIME_NAK, Handler_DW::OnDW_GM_CHATBLOCKTIME_NAK ) ) return FALSE;
	// 리벤지 관련
	if( !RegGameDBProxyPacket( DW_CHAO,	DW_CHAO_ADD_REVENGE_CHAR_ACK, Handler_DW::OnDW_CHAO_ADD_REVENGE_CHAR_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_CHAO,	DW_CHAO_ADD_REVENGE_CHAR_NAK, Handler_DW::OnDW_CHAO_ADD_REVENGE_CHAR_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_CHAO,	DW_CHAO_REMOVE_REVENGE_CHAR_ACK, Handler_DW::OnDW_CHAO_REMOVE_REVENGE_CHAR_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_CHAO,	DW_CHAO_REMOVE_REVENGE_CHAR_NAK, Handler_DW::OnDW_CHAO_REMOVE_REVENGE_CHAR_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_CHAO,	DW_CHAO_PK_REVENGE_CHAR_ACK, Handler_DW::OnDW_CHAO_PK_REVENGE_CHAR_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_CHAO,	DW_CHAO_PK_REVENGE_CHAR_NAK, Handler_DW::OnDW_CHAO_PK_REVENGE_CHAR_NAK ) ) return FALSE;

	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_SEND_ACK, Handler_DW::OnDW_MEMO_SEND_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_SEND_NAK, Handler_DW::OnDW_MEMO_SEND_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_LIST_ANS, Handler_DW::OnDW_MEMO_LIST_ANS ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_LIST_NAK, Handler_DW::OnDW_MEMO_LIST_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_VIEW_ANS, Handler_DW::OnDW_MEMO_VIEW_ANS ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_VIEW_NAK, Handler_DW::OnDW_MEMO_VIEW_NAK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_DELETE_ACK, Handler_DW::OnDW_MEMO_DELETE_ACK ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_MEMO,	DW_MEMO_DELETE_NAK, Handler_DW::OnDW_MEMO_DELETE_NAK ) ) return FALSE;

	if( !RegGameDBProxyPacket( DW_STATUS, DW_STATUS_INTERCEPTION_ANS, Handler_DW::OnDW_STATUS_INTERCEPTION_ANS ) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_STATUS, DW_STATUS_INTERCEPTION_NAK, Handler_DW::OnDW_STATUS_INTERCEPTION_NAK ) ) return FALSE;

	if( !RegGameDBProxyPacket( DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_ANS, Handler_DW::OnDW_GUILD_WAREHOUSE_LOGLIST_ANS) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_NAK, Handler_DW::OnDW_GUILD_WAREHOUSE_LOGLIST_NAK ) ) return FALSE;

	if( !RegGameDBProxyPacket( DW_GUILD, DW_GUILD_SELECT_NOTICE_ACK, Handler_DW::OnDW_GUILD_SELECT_NOTICE_ACK) ) return FALSE;
	if( !RegGameDBProxyPacket( DW_GUILD, DW_GUILD_SELECT_NOTICE_NAK, Handler_DW::OnDW_GUILD_SELECT_NOTICE_NAK) ) return FALSE;

    if( !RegGameDBProxyPacket( DW_GUILD, DW_DOMINATION_FINDMONEY_ACK, Handler_DW::OnDW_DOMINATION_FINDMONEY_ACK) ) return FALSE;
    //__NA001254_20090109_RTTG_FUNC_ADD
	if ( !RegGameDBProxyPacket( DW_MEMO, DW_MULTIMEMO_SEND_ACK, Handler_DW::OnDW_MULTIMEMO_SEND_ACK))		return FALSE;
    // __KR_001355_20090608_GROUPINGSYSTEM_DW = { //그룹 맺기 시스템 
	if ( !RegGameDBProxyPacket( DW_GROUP, DW_GROUP_ADD_ACK, Handler_DW::OnDW_GROUP_ADD_ACK)) return FALSE; //그룹 맴버 추가 성공
	if ( !RegGameDBProxyPacket( DW_GROUP, DW_GROUP_ADD_NAK, Handler_DW::OnDW_GROUP_ADD_NAK)) return FALSE; //그룹 맴버 추가 실패

	if ( !RegGameDBProxyPacket( DW_GROUP, DW_GROUP_DEL_ACK, Handler_DW::OnDW_GROUP_DEL_ACK)) return FALSE; //그룹 맴버 삭제 성공
	if ( !RegGameDBProxyPacket( DW_GROUP, DW_GROUP_DEL_NAK, Handler_DW::OnDW_GROUP_DEL_NAK)) return FALSE; //그룹 맴버 삭제 실패

	if ( !RegGameDBProxyPacket( DW_GROUP, DW_GROUP_LIST_ACK, Handler_DW::OnDW_GROUP_LIST_ACK)) return FALSE; //그룹 맴버 삭제 성공
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DW'

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_LOAD_ENTRY_ACK, Handler_DW::OnDW_LOTTO_LOAD_ENTRY_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_LOAD_ACK, Handler_DW::OnDW_LOTTO_LOAD_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_WINNER_LIST_ACK, Handler_DW::OnDW_LOTTO_WINNER_LIST_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_SAVE_ENTRY_ACK, Handler_DW::OnDW_LOTTO_SAVE_ENTRY_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_ROLLOVER_ACK, Handler_DW::OnDW_LOTTO_ROLLOVER_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_ADD_TICKET_ACK, Handler_DW::OnDW_LOTTO_ADD_TICKET_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_REWARD_ACK, Handler_DW::OnDW_LOTTO_REWARD_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_NOTICE_LIST_ACK, Handler_DW::OnDW_LOTTO_NOTICE_LIST_ACK) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_LOTTO, DW_LOTTO_NOTICE_DEL_ACK, Handler_DW::OnDW_LOTTO_NOTICE_DEL_ACK) ) return FALSE;
    //}

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    if( !RegGameDBProxyPacket( DW_NOTIFICATION,	DW_NOTIFICATION_ITEM_SYN, Handler_DW::OnDW_NOTIFICATION_ITEM_SYN ) ) return FALSE;
    if( !RegGameDBProxyPacket( DW_NOTIFICATION,	DW_NOTIFICATION_CLEAR_SYN, Handler_DW::OnDW_NOTIFICATION_CLEAR_SYN ) ) return FALSE;
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

    // From account DB proxy


	// From game server


	// From agent server
	if( !RegAgentServerPacket( AW_CONNECTION, AW_AGENTSERVER_INFO_SYN, Handler_AW::OnAW_AGENTSERVER_INFO_SYN ) )		return FALSE;
	if( !RegAgentServerPacket( AW_CONNECTION, AW_PREPARE_NEW_USER_SYN, Handler_AW::OnAW_PREPARE_NEW_USER_SYN ) )		return FALSE;
	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_SET_CHARNAME_SYN, Handler_AW::OnAW_USER_SET_CHARNAME_SYN ) )		return FALSE;
	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_UNSET_CHARNAME_SYN, Handler_AW::OnAW_USER_UNSET_CHARNAME_SYN ) )	return FALSE;
	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_ENTER_ZONE_SYN, Handler_AW::OnAW_USER_ENTER_ZONE_SYN ) )			return FALSE;
	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_LEAVE_ZONE_SYN, Handler_AW::OnAW_USER_LEAVE_ZONE_SYN ) )			return FALSE;
	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_LOGOUT, Handler_AW::OnAW_USER_LOGOUT ) )							return FALSE;
	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_RESTORE_CMD, Handler_AW::OnAW_USER_RESTORE_CMD ) )				return FALSE;
	if( !RegAgentServerPacket( AW_PARTY, AW_PARTY_MAKE_SYN, Handler_AW::OnAW_PARTY_MAKE_SYN ) )							return FALSE;
	if( !RegAgentServerPacket( AW_PARTY, AW_PARTY_JOIN_MEMBER_SYN, Handler_AW::OnAW_PARTY_JOIN_MEMBER_SYN ) )			return FALSE;
	if( !RegAgentServerPacket( AW_PARTY, AW_PARTY_WITHDRAW_MEMBER_SYN, Handler_AW::OnAW_PARTY_WITHDRAW_MEMBER_SYN ) )	return FALSE;
	if( !RegAgentServerPacket( AW_PARTY, AW_PARTY_DESTROY_SYN, Handler_AW::OnAW_PARTY_DESTROY_SYN ) )					return FALSE;
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    if( !RegAgentServerPacket( AW_PARTY, AW_PARTY_ROOM_KEY_SYN, Handler_AW::OnAW_PARTY_ROOM_KEY_SYN ) )					return FALSE;
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    if (!RegAgentServerPacket(AW_PARTY, 
                              AW_PARTY_CHANGE_MASTER_SYN, 
                              Handler_AW::OnAW_PARTY_CHANGE_MASTER_SYN))
    {
        return FALSE;
    }

    if (!RegAgentServerPacket(AW_PARTY, 
                              AW_PARTY_FINDING_JOIN_RESULT_ACK, 
                              Handler_AW::OnAW_PARTY_FINDING_JOIN_RESULT_ACK))
    {
        return FALSE;
    }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

/*
	if( !RegAgentServerPacket( AW_OPERATION, AW_USERCOUNT_SYN, Handler_AW::OnAW_USERCOUNT_SYN ) )
		return FALSE;
	if( !RegAgentServerPacket( AW_OPERATION, AW_USERMONEY_SYN, Handler_AW::OnAW_USERMONEY_SYN ) )
		return FALSE;
*/
	if( !RegAgentServerPacket( AW_GM, AW_GM_WHISPER_SET_CMD, Handler_AW::OnAW_GM_WHISPER_SET_CMD ) )					return FALSE;
	if( !RegAgentServerPacket( AW_GM, AW_GM_USERCHAT_BLOCK_CMD, Handler_AW::OnAW_GM_USERCHAT_BLOCK_CMD ) )				return FALSE;
	if( !RegAgentServerPacket( AW_GM, AW_GM_NOTICE_CMD, Handler_AW::OnAW_GM_NOTICE_CMD ) )								return FALSE;

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    if( !RegAgentServerPacket( AW_GM, AW_GM_LOTTO_NUM_CMD, Handler_AW::OnAW_GM_LOTTO_NUM_CMD ) ) return FALSE;
    if( !RegAgentServerPacket( AW_GM, AW_GM_LOTTO_SETTLE_CMD, Handler_AW::OnAW_GM_LOTTO_SETTLE_CMD ) ) return FALSE;
    if( !RegAgentServerPacket( AW_GM, AW_GM_LOTTO_FUND_CMD, Handler_AW::OnAW_GM_LOTTO_FUND_CMD ) ) return FALSE;
    //}

    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_LEVELUP_CMD, Handler_AW::OnAW_USER_LEVELUP_CMD ) )				return FALSE;
	if( !RegAgentServerPacket( AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_SYN, Handler_AW::OnAW_CHAO_REQUEST_REVENGEINFO_SYN ) )		return FALSE;
	if( !RegAgentServerPacket( AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_SYN, Handler_AW::OnAW_CHAO_REMOVE_REVENGE_CHAR_SYN ) )		return FALSE;
	if( !RegAgentServerPacket( AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_SYN, Handler_AW::OnAW_CHAO_ADD_REVENGE_CHAR_SYN ) )	return FALSE;
	if( !RegAgentServerPacket( AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD, Handler_AW::OnAW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD))	return FALSE;
	if( !RegAgentServerPacket( AW_ITEM, AW_ITEM_SPECIAL_ITEM_CLEAR_CMD, Handler_AW::OnAW_ITEM_SPECIAL_ITEM_CLEAR_CMD))		return FALSE;
	if( !RegAgentServerPacket( AW_ITEM, AW_ITEM_USE_DECREE_ITEM_ACK, Handler_AW::OnAW_ITEM_USE_DECREE_ITEM_ACK))		return FALSE;
	if( !RegAgentServerPacket( AW_ITEM, AW_ITEM_USE_DECREE_ITEM_NAK, Handler_AW::OnAW_ITEM_USE_DECREE_ITEM_NAK))		return FALSE;
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    if( !RegAgentServerPacket( AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN, Handler_AW::OnAW_ITEM_USE_HEIM_LOTTO_TICKET_SYN)) return FALSE;
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

	if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_SYN, Handler_AW::OnAW_USER_STATUS_INTERCEPTION_SYN ) )		return FALSE;
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_STATUS_GROUP_EXP_ACQUISITION_CMD, Handler_AW::OnAW_STATUS_GROUP_EXP_ACQUISITION_CMD ) ) return FALSE;
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_STATUS_HEIM_ACQUISITION_NTF, Handler_AW::OnAW_USER_STATUS_HEIM_ACQUISITION_NTF ) ) return FALSE;
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_STATUS_HEIM_CONSUMPTION_NTF, Handler_AW::OnAW_USER_STATUS_HEIM_CONSUMPTION_NTF ) ) return FALSE;
    if( !RegAgentServerPacket( AW_OPERATION, AW_OPERATION_ROOM_CTRL_CMD, Handler_AW::OnAW_OPERATION_ROOM_CTRL_CMD ) )		return FALSE;
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_STATUS_GROUP_START_CMD, Handler_AW::OnAW_STATUS_GROUP_START_CMD ) )		return FALSE;

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_LOTTO_REWARD_ACK, Handler_AW::OnAW_USER_LOTTO_REWARD_ACK) ) return FALSE;
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_ACK, Handler_AW::OnAW_USER_LOTTO_TICKET_PURCHASE_ACK) ) return FALSE;
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_STATUS_PCBANG_BILLING_NTF, Handler_AW::OnAW_USER_STATUS_PCBANG_BILLING_NTF)) return FALSE;
#endif
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    if( !RegAgentServerPacket( AW_USER_STATUS, AW_USER_SYSTEMMEMO_SEND_CMD, Handler_AW::OnAW_USER_SYSTEMMEMO_SEND_CMD)) return FALSE;
    if( !RegAgentServerPacket( AW_FRIENDINVITATE, AW_FRIEND_INVITATIONR_INSERT_CMD, Handler_AW::OnAW_FRIEND_INVITATIONR_INSERT_CMD)) return FALSE;
	// From master server
	if( !RegMasterServerPacket( MW_CONNECTION, MW_SERVER_INFO_CMD, Handler_MW::OnMW_SERVER_INFO_CMD ) )			return FALSE;
	if( !RegMasterServerPacket( MW_OPERATION, MW_NOTICE_CMD, Handler_MW::OnMW_NOTICE_CMD ) )					return FALSE;
	if( !RegMasterServerPacket( MW_OPERATION, MW_BATTLEZONEINFO_REQ, Handler_MW::OnMW_BATTLEZONEINFO_REQ ) )	return FALSE;
	if( !RegMasterServerPacket( SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ, Handler_MW::OnSERVERCOMMON_SERVERSHUTDOWN_REQ ) )	return FALSE;
    //__NA001254_20090109_RTTG_FUNC_ADD
	if ( !RegMasterServerPacket( MW_OPERATION, MW_MULTIMEMO_SEND_SYN, Handler_MW::OnMW_MULTIMEMO_SEND_SYN))		return FALSE;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( !RegMasterServerPacket( MW_CONNECTION, MW_CONNECTION_DISCONNECT_CMD, Handler_MW::OnMW_CONNECTION_DISCONNECT_CMD) )		return FALSE;
	if( !RegMasterServerPacket( MW_CONNECTION, MW_CONNECTION_PHASE_SHIFT_CMD, Handler_MW::OnMW_CONNECTION_PHASE_SHIFT_CMD) )	return FALSE;
	if( !RegMasterServerPacket( MW_CONNECTION, MW_CONNECTION_BLOCK_CMD, Handler_MW::OnMW_CONNECTION_BLOCK_CMD) )	return FALSE;

	if( !RegMasterServerPacket( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, Handler_MW::OnSERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK) )	return FALSE;
	if( !RegMasterServerPacket( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, Handler_MW::OnSERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK) )	return FALSE;
	if( !RegMasterServerPacket( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, Handler_MW::OnSERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD) )		return FALSE;

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__


	// From Guild Server
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_DESTROY_GUILD_CMD, Handler_WZ::OnWZ_GUILD_DESTROY_GUILD_CMD ) )	return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_JOIN_GUILD_CMD, Handler_WZ::OnWZ_GUILD_JOIN_GUILD_CMD ) )	return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_WITHDRAW_GUILD_CMD, Handler_WZ::OnWZ_GUILD_WITHDRAW_GUILD_CMD ) )	return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_NOTICE_CMD, Handler_WZ::OnWZ_GUILD_NOTICE_CMD ) )			return FALSE;
	//if ( !RegGuildServerPacket( SERVERCOMMON, SERVERCOMMON_HEARTBEAT, Handler_WZ::OnSERVERCOMMON_HEARTBEAT ) ) return FALSE;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHUNTING_PVP_RESULT_CMD, Handler_WZ::OnWZ_GUILD_CHUNTING_PVP_RESULT_CMD ) )	return FALSE;
#endif
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHANGE_DUTY_CMD, Handler_WZ::OnWZ_GUILD_CHANGE_DUTY_CMD))			return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHANGE_MASTER_CMD, Handler_WZ::OnWZ_GUILD_CHANGE_MASTER_CMD))		return FALSE;

    if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHANGE_COMMANDER_CMD, Handler_WZ::OnWZ_GUILD_CHANGE_COMMANDER_CMD ))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHANGE_CORPS_CMD, Handler_WZ::OnWZ_GUILD_CHANGE_CORPS_CMD ))				return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CREATE_REAR_CAMP_CMD, Handler_WZ::OnWZ_GUILD_CREATE_REAR_CAMP_CMD ))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_DESTROY_REAR_CAMP_CMD, Handler_WZ::OnWZ_GUILD_DESTROY_REAR_CAMP_CMD ))	return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CREATE_CMD, Handler_WZ::OnWZ_GUILD_CREATE_CMD ))							return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_RESTORE_ACK, Handler_WZ::OnWZ_GUILD_RESTORE_ACK) )						return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_LOGIN_CHAR_CMD, Handler_WZ::OnWZ_GUILD_LOGIN_CHAR_CMD ) )				return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_LOGOUT_CHAR_CMD, Handler_WZ::OnWZ_GUILD_LOGOUT_CHAR_CMD ) )				return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_KICK_PLAYER_CMD, Handler_WZ::OnWZ_GUILD_KICK_PLAYER_CMD ) )				return FALSE;

	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_RECOVERY_GUILD_CMD, Handler_WZ::OnWZ_GUILD_RECOVERY_GUILD_CMD))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_DESTROY_WAIT_CMD, Handler_WZ::OnWZ_GUILD_DESTROY_WAIT_CMD))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_GUILDMARK_REGISTER_CMD, Handler_WZ::OnWZ_GUILD_GUILDMARK_REGISTER_CMD))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_GUILDMARK_DELETE_CMD, Handler_WZ::OnWZ_GUILD_GUILDMARK_DELETE_CMD))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_CMD, Handler_WZ::OnWZ_GUILD_CHANGE_NOTICE_CMD))		return FALSE;
	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_NAK, Handler_WZ::OnWZ_GUILD_CHANGE_NOTICE_NAK))		return FALSE;

	if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_NAME_CHANGE_CMD, Handler_WZ::OnWZ_GUILD_NAME_CHANGE_CMD))		return FALSE;
    if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_RENAME_CHAR_CMD, Handler_WZ::OnWZ_GUILD_RENAME_CHAR_CMD)) return FALSE;
    if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_DOMINATION_RETURNHIME_CMD, Handler_WZ::OnWZ_GUILD_DOMINATION_RETURNHIME_CMD))		return FALSE;

    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK, 
                                Handler_WZ::OnWZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK))		
        return FALSE;

    if ( !RegGuildServerPacket( WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK, 
                                Handler_WZ::OnWZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK))		
        return FALSE;

    return TRUE;
}

// Client를 위한 패킷 핸들러
BOOL PacketHandler::RegClientPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_CW fnHandler )
{
	FUNC_CW *pFuncInfo = new FUNC_CW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_CW->Add( pFuncInfo );
}

// Game DB Proxy를 위한 패킷 핸들러
BOOL PacketHandler::RegGameDBProxyPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_DW fnHandler )
{
	FUNC_DW *pFuncInfo = new FUNC_DW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_DW->Add( pFuncInfo );
}

// Account DB Proxy를 위한 패킷 핸들러
BOOL PacketHandler::RegAccountDBProxyPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_TW fnHandler )
{
	FUNC_TW *pFuncInfo	= new FUNC_TW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_TW->Add( pFuncInfo );
}

// 게임 서버를 위한 패킷 핸들러 추가
BOOL PacketHandler::RegGameServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_GW fnHandler )
{
	FUNC_GW *pFuncInfo	= new FUNC_GW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_GW->Add( pFuncInfo );
}

// 배틀 서버를 위한 패킷 핸들러 추가
BOOL PacketHandler::RegBattleServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_BW fnHandler )
{
	FUNC_BW *pFuncInfo	= new FUNC_BW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_BW->Add( pFuncInfo );
}

// 필드 서버를 위한 패킷 핸들러 추가
BOOL PacketHandler::RegFieldServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_FW fnHandler )
{
	FUNC_FW *pFuncInfo	= new FUNC_FW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_FW->Add( pFuncInfo );
}

// 에이전트 서버를 위한 패킷 핸들러 추가
BOOL PacketHandler::RegAgentServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_AW fnHandler )
{
	FUNC_AW *pFuncInfo	= new FUNC_AW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_AW->Add( pFuncInfo );
}

// 마스터 서버를 위한 패킷 추가
BOOL PacketHandler::RegMasterServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_MW fnHandler )
{
	FUNC_MW *pFuncInfo	= new FUNC_MW;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;

	return m_pFuncMap_MW->Add( pFuncInfo );
}

#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
// 길드 서버를 위한 패킷 추가
bool PacketHandler::RegGuildServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_DW fnHandler )
{
    FUNC_DW* function_info = new FUNC_DW;
    function_info->m_dwFunctionKey = MAKEWORD(category, protocol);
    function_info->m_fnHandler = fnHandler;
    return (m_pFuncMap_DW->Add(function_info) != false);
}
#else //if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
// 길드 서버를 위한 패킷 추가
BOOL PacketHandler::RegGuildServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_WZ fnHandler )
{
	FUNC_WZ *pFuncInfo	= new FUNC_WZ;

	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_WZ->Add( pFuncInfo );
}
#endif //if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)



BOOL PacketHandler::ParsePacket_CW( User *pUser, MSG_BASE *pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_CW *pFuncInfo = (FUNC_CW*)m_pFuncMap_CW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Invalid packet from game client! : Category[%d]Protocol[%d]",pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

    pFuncInfo->m_fnHandler( pUser, pMsg, wSize );

	return TRUE;
}


BOOL PacketHandler::ParsePacket_DW( GameDBProxySession *pGameDBProxySession, MSG_BASE * pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_DW *pFuncInfo = (FUNC_DW*)m_pFuncMap_DW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Invalid packet from game DB proxy! : Category[%d]Protocol[%d]",pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pGameDBProxySession, pMsg, wSize );

	return TRUE;
}

BOOL PacketHandler::ParsePacket_TW( AccountDBProxySession *pAccountDBProxySession, MSG_BASE * pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_TW *pFuncInfo = (FUNC_TW*)m_pFuncMap_TW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Invalid packet from account DB proxy! : Category[%d]Protocol[%d]", pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pAccountDBProxySession, pMsg, wSize );

	return TRUE;
}

// 게임 서버로부터 오는 패킷 파싱
BOOL PacketHandler::ParsePacket_GW( GameServerSession *pGameServerSession, MSG_BASE * pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_GW *pFuncInfo = (FUNC_GW*)m_pFuncMap_GW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Invalid packet from game server! : Category[%d]Protocol[%d]", pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pGameServerSession, pMsg, wSize );

	return TRUE;
}


// 배틀 서버로부터 오는 패킷 파싱
BOOL PacketHandler::ParsePacket_BW( BattleServerSession *pBattleServerSession, MSG_BASE * pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_BW *pFuncInfo = (FUNC_BW*)m_pFuncMap_BW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pBattleServerSession, pMsg, wSize );

	return TRUE;
}


// 필드 서버로부터 오는 패킷 파싱
BOOL PacketHandler::ParsePacket_FW( FieldServerSession *pFieldServerSession, MSG_BASE * pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_FW *pFuncInfo = (FUNC_FW*)m_pFuncMap_FW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pFieldServerSession, pMsg, wSize );

	return TRUE;
}

// 에이전트 서버로부터 오는 패킷 파싱
BOOL PacketHandler::ParsePacket_AW( AgentServerSession *pAgentServerSession, MSG_BASE *pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_AW *pFuncInfo = (FUNC_AW*)m_pFuncMap_AW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pAgentServerSession, pMsg, wSize );

	return TRUE;
}

// 마스터 서버로부터 오는 패킷 파싱
BOOL PacketHandler::ParsePacket_MW( MasterServerSession *pMasterServerSession, MSG_BASE *pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_MW *pFuncInfo = (FUNC_MW*)m_pFuncMap_MW->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Warning: Unregistered packet(%d)(%d)", pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pMasterServerSession, pMsg, wSize );

	return TRUE;
}

#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
BOOL PacketHandler::ParsePacket_WZ(GameDBProxySession* server_session, MSG_BASE* msg, WORD msg_size)
{
    FUNC_DW* const function_info = static_cast<FUNC_DW*>(
        m_pFuncMap_DW->Find(MAKEWORD(msg->m_byCategory, msg->m_byProtocol)));
    if (function_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unregistered message, C:P={%d:%d}"),
               msg->m_byCategory, msg->m_byProtocol);
        return false;
    };
    function_info->m_fnHandler(server_session, msg, msg_size);
    return true;
}
#else //if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
// 길드 서버로부터 오는 패킷 파싱
BOOL PacketHandler::ParsePacket_WZ( GuildServerSession *pGuildServerSession, MSG_BASE *pMsg, WORD wSize )
{
	//카테고리에 따라 암호화 해독처리
	if( pMsg->m_byCategory == 0xff )
	{
		// ......해독
		// 해독한 데이터에서 카테고리와 패킷타입 추출 : byPacketCategory,byPacketType
	}

	FUNC_WZ *pFuncInfo = (FUNC_WZ*)m_pFuncMap_WZ->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Warning: Unregistered packet(%d)(%d)", pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}

	pFuncInfo->m_fnHandler( pGuildServerSession, pMsg, wSize );

	return TRUE;
}
#endif //if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
