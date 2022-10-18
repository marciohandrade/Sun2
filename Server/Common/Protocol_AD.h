
#ifndef __PROTOCOL_AD_H__
#define __PROTOCOL_AD_H__

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//
// * Postfix
//		SYN - Server에 요청
//		ACK - 요청에 대한 허락
//		NAK - 요청에 대한 거부
//		CMD - Server의 명령
//		BRD - Server가 동기화 관련 내용을 알림
//		DBR - DBProxy에서만 사용함!
//
// * 프로토콜 Naming Rule
//		Prefix_카테고리명_프로토콜명_Postfix
//		ex) CG_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------
#include <Protocol_AG.h>

/// 카테고리
enum eAD_CATEGORY
{
	AD_CONNECTION		= 101, // (WAVERIX) (WARNING) 서버간 카테고리 시작 번호 주의할 것.
	AD_CHARINFO,
    AD_EVENT, // added by _KR_0_20091021_FIRENDINVITATION_EXTEND
    AD_ETC, // added by _NA_0_20110329_GAMEOPTION_SETTING
    //
	AD_MAX,
};

enum eAD_CONNECTION_PROTOCOL
{
	AD_CONNECTION_DISCONNECT_CMD,
	AD_CONNECTION_CHARLIST_SYN,
	AD_CONNECTION_CHARLIST_ACK,
    AD_CONNECTION_DBCONNECT_UPDATE_CMD, // _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    AD_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
    // NOTE: f110622.2L, when an user return to the character selection zone,
    // the user may rarely miss the activated updating server in case the user that was leaving
    // on a field of a game server. this protocol would notify the database proxy server
    // in order to refresh the zombie user detection timeout.
    AD_CONNECTION_HE_IS_HERE_CMD,
};

enum eAD_CHARINFO_PROTOCOL
{
	AD_CHARINFO_CHARLISTREQ_SYN,
	AD_CHARINFO_CHARLISTREQ_ACK,
	AD_CHARINFO_CHARLISTREQ_NAK,
	AD_CHARINFO_CHARLISTREQ_DBR,

	AD_CHARINFO_CREATE_SYN,
	AD_CHARINFO_CREATE_ACK,
	AD_CHARINFO_CREATE_DBR,
	AD_CHARINFO_CREATE_NAK,

	AD_CHARINFO_DESTROY_SYN,
	AD_CHARINFO_DESTROY_ACK,
	AD_CHARINFO_DESTROY_DBR,
	AD_CHARINFO_DESTROY_NAK,

	AD_CHARINFO_RECOVER_SYN,
	AD_CHARINFO_RECOVER_ACK,
	AD_CHARINFO_RECOVER_DBR,
	AD_CHARINFO_RECOVER_NAK,

	AD_CHARINFO_STATE_SELECT_DBR,
	AD_CHARINFO_STATE_UPDATE_DBR,

	AD_CHARINFO_CHECK_CHARACTERID_SYN,	// __NA_001044_ADD_CUSTOM_UI
	AD_CHARINFO_CHECK_CHARACTERID_ACK,
	AD_CHARINFO_CHECK_CHARACTERID_DBR,
	AD_CHARINFO_CHECK_CHARACTERID_NAK,

	AD_CHARINFO_ADDIN_NTF	,

    AD_CHARINFO_PCBANG_BILLING_NTF,  // PC방 빌링 정보 변환, _PCBANG_POLICY_CHANGE_POST_EXPIRED
    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    AD_CHARINFO_CHARLISTREQ_INTER_CHAR_DBR,
    AD_CHARINFO_CHARLISTREQ_INTER_INVEN_DBR,
    AD_CHARINFO_CHARLISTREQ_INTER_PETLIST_DBR,
    AD_CHARINFO_CHARLISTREQ_INTER_EQUIP_ITEMSLOTINFO_DBR,

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    AD_CHARNAME_CHANGE_SYN,
    AD_CHARNAME_CHANGE_ACK,
    AD_CHARNAME_CHANGE_DBR,
    AD_CHARNAME_CHANGE_NAK,
#endif

};

// used by _KR_0_20091021_FIRENDINVITATION_EXTEND
enum eAD_EVENT_PROTOCOL
{
    AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN,// 친구 초대 이벤트에 대한 보상 목록 요청 (A->D)
    AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK,	    	
    AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK,
    AD_EVENT_INVITATEFRIEND_REWARD_LIST_DBR,    

    AD_EVENT_INVITATIONRELATION_INSERT_DBR,    //친구 추천 관계 생성
    AD_EVENT_INVITATIONR_INSERT_CMD,
};

// declared by _NA_0_20110329_GAMEOPTION_SETTING
enum eAD_ETC_PROTOCOL
{
    AD_ETC_GAMEOPTION_SELECT_SYN,
    AD_ETC_GAMEOPTION_SELECT_ACK,
    AD_ETC_GAMEOPTION_SELECT_NAK,
    AD_ETC_GAMEOPTION_SELECT_DBR,
    AD_ETC_GAMEOPTION_UPDATE_CMD,
    AD_ETC_GAMEOPTION_UPDATE_DBR,
    AD_ETC_GAMEOPTION_UPDATE_ACK, 

    //!~ _NA004034_20120102_GUILD_POINT_SYSTEM
    AD_ETC_GUILD_RANKINGINFO_SYN,
    AD_ETC_GUILD_RANKINGINFO_DBR,
    AD_ETC_GUILD_RANKINGINFO_ACK,

    AD_ETC_GUILDINFO_SYN,
    AD_ETC_GUILDINFO_ACK,
    //~! _NA004034_20120102_GUILD_POINT_SYSTEM
};

#endif // __PROTOCOL_AD_H__









