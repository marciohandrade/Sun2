#ifndef __PROTOCOL_GM_H__
#define __PROTOCOL_GM_H__

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


// GameServer <-> MasterServer Protocol


enum eGM_CATEGORY
{
	GM_CONNECTION		= 1,
	GM_OPERATION,
	GM_MAX,
};


// CONNECTION = 1,		// 서버 접속

enum ePROTOCOL_GM_CONNECTION
{
	GM_CONNECTION_SERVER_INFO_CMD		= 0x01,

	GM_CONNECTION_DISCONNECT_CMD	= 200,
	GM_CONNECTION_PHASE_SHIFT_CMD	= 201,
	GM_CONNECTION_BLOCK_CMD			= 202,
    GM_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};

// OTHER, 	// 기타

enum ePROTOCOL_GM_OPERATION
{
	GM_RELOAD_DATA_CMD,
	GM_USERCOUNT_SYN,
	GM_USERMONEY_NTF,

	GM_OPERATION_USERMONEY_REQ,	// 통화량 요청
	GM_OPERATION_USERMONEY_ANS,	// 통화량 보고

//	GM_OPERATION_SERVERSHUTDOWN_REQ,
//	GM_OPERATION_SERVERSHUTDOWN_ANS,

	GM_OPERATION_FUNCRESTRICT_CMD,	// 특정 기능 금지하도록 명령

	GM_OPERATION_STATISTICS_REQ,	// 게임 통계 보고
	GM_OPERATION_STATISTICS_ANS,

	//__NA000000_070523_REPORT_TO_SOLARAUTH
	GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF,
};


#endif // __PROTOCOL_GM_H__


