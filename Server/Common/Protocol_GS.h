
#pragma once

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//		X : AccountDBP Server
//		Z : Guild Server
//		S : Shop Server
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


// 카테고리
enum eGS_CATEGORY
{
	GS_SHOP = 0,
};

// 프로토콜
// 프로토콜 번호 순서, ShopServer와 동기화해야함. 변경 주의!!!!!
enum eGS_SHOP_PROTOCOL
{
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GS_SHOP_USER_CASH_REQ_SYN = 30,
    GS_SHOP_USER_CASH_REQ_ACK = 2,
#else
	GS_SHOP_USER_CASH_REQ_SYN = 1,
    GS_SHOP_USER_CASH_REQ_ACK,
#endif
	

	GS_SHOP_ITEM_LIST_REQ_SYN,
	GS_SHOP_ITEM_LIST_REQ_ACK,

	GS_SHOP_PACK_LIST_REQ_SYN,
	GS_SHOP_PACK_LIST_REQ_ACK,

	GS_SHOP_BRANCH_LIST_REQ_SYN,
	GS_SHOP_BRANCH_LIST_REQ_ACK,

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GS_SHOP_BUY_ITEM_SYN = 32,
    GS_SHOP_BUY_ITEM_ACK = 10,				// 10
#else
    GS_SHOP_BUY_ITEM_SYN,
    GS_SHOP_BUY_ITEM_ACK,				// 10
#endif
	
	
	GS_SHOP_BUY_ITEM_NTF,

	GS_SHOP_USER_BUY_HISTORY_SYN,
	GS_SHOP_USER_BUY_HISTORY_ACK,

	GS_SHOP_RELOAD_DATA_NTF,

	// 2007/1/17 오후 4:57:28
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GS_SHOP_SEND_GIFT_SYN = 34,
    GS_SHOP_SEND_GIFT_ACK = 16,
#else
    GS_SHOP_SEND_GIFT_SYN,
    GS_SHOP_SEND_GIFT_ACK,
#endif	

	GS_SHOP_ACTION_RECV_GIFT_SYN,
	GS_SHOP_ACTION_RECV_GIFT_ACK,
	GS_SHOP_ACTION_RECV_GIFT_NTF,

	GS_SHOP_IS_RECV_GIFT_SYN,			// 20
	GS_SHOP_IS_RECV_GIFT_ACK,

	GS_SHOP_RECV_GIFT_LIST_SYN,
	GS_SHOP_RECV_GIFT_LIST_ACK,

	GS_SHOP_SEND_GIFT_LIST_SYN,
	GS_SHOP_SEND_GIFT_LIST_ACK,
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	GS_SHOP_MILEAGE_LIST_REQ_SYN,
	GS_SHOP_MILEAGE_LIST_REQ_ACK,
#endif

	GS_SHOP_EXTEND_OPTION_LIST_REQ_SYN = 28,
	GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK = 29,

};













