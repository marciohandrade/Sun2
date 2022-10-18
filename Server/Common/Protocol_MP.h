
#ifndef __PROTOCOL_MP_H__
#define __PROTOCOL_MP_H__

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
enum eMP_CATEGORY
{
	MP_CONNECTION		= 1,
	MP_MAX,
};
	
enum eMP_CONNECTION_PROTOCOL
{
	MP_CONNECTION_SERVER_INFO_CMD,

	MP_CONNECTION_DISCONNECT_CMD	= 200,
	MP_CONNECTION_PHASE_SHIFT_CMD	= 201,
	MP_CONNECTION_BLOCK_CMD			= 202,
    MP_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};







#endif // __PROTOCOL_MP_H__