
#ifndef __PROTOCOL_GP_H__
#define __PROTOCOL_GP_H__

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

#include "Protocol_DG.h"

// CHANGES: f110509.4L, removed by _NA002635_GP_DP_PROTOCOL_UINIFICATION
// moved = { eGP_CATEGORY, eGP_RANKING_PROTOCOL }

#endif // __PROTOCOL_GP_H__
