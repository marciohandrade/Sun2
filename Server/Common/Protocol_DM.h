#ifndef __PROTOCOL_DM_H__
#define __PROTOCOL_DM_H__


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


/// 카테고리
enum eDM_CATEGORY
{
	DM_CONNECTION		= 130,  // (WAVERIX) (WARNING) 서버간 카테고리 시작 번호 주의할 것.
	DM_OPERATION,
    MZ_CONNECTION, // < SUN_CODE_DELETE_RESERVED
    MZ_GUILD, // CHANGES: f110523.5L
	DM_MAX,
};


/// 프로토콜
enum eDM_CONNECTION_PROTOCOL
{
	DM_CONNECTION_SERVER_INFO_CMD,

	DM_CONNECTION_DISCONNECT_CMD	= 200,
	DM_CONNECTION_PHASE_SHIFT_CMD	= 201,
	DM_CONNECTION_BLOCK_CMD			= 202,
    DM_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};

enum eDM_OPERATION_PROTOCOL
{
//	DM_OPERATION_SERVERSHUTDOWN_REQ,
//	DM_OPERATION_SERVERSHUTDOWN_ANS,
};

#if SUN_CODE_DELETE_RESERVED
enum eMZ_CONNECTION_PROTOCOL
{
	MZ_SERVER_INFO_CMD,

	MZ_CONNECTION_DISCONNECT_CMD	= 200,
	MZ_CONNECTION_PHASE_SHIFT_CMD	= 201,
	MZ_CONNECTION_BLOCK_CMD			= 202,
};
#endif

//__NA001254_20090109_RTTG_FUNC_ADD
enum eMZ_GUILD_PROTOCOL
{
	MZ_GUILD_NAME_CHANGE_SYN,
	MZ_GUILD_NAME_CHANGE_ACK,
	MZ_GUILD_NAME_CHANGE_NAK,
};

#endif // __PROTOCOL_DM_H__


