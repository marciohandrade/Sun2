#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL

#pragma once

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_GS.h>
#include <Define.ShopServer.h>
#include <Protocol.ShopServer.h>


#pragma pack(push,1)




#define PACKET_GS_OPEN( c, p )	struct MSG_##p : public MSG_BASE_INTERNAL { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_GS_CLOSE		};


// (NOTE) 잔여 캐쉬 요청
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_CASH_REQ_SYN )
	DWORD	dwUserGuid;	//< 사용자 Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
#endif
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_CASH_REQ_ACK )
	DWORD	dwUserGuid;	//< 사용자 Guid
	DWORD	dwUserCash;	//< 잔여 캐쉬
#ifdef __CN_0_20070507_SHOP_MILEAGE
	DWORD	dwHonors;	//< 잔여 마일리지 갯수(해방군의 훈장)
#endif
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    DWORD   pp_cash;  //< 선불 카드로 결재한 캐쉬
#endif
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ITEM_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ITEM_LIST_REQ_ACK )
	DWORD 			dwItemCount;				//< 아이템 목록 갯수
	tblSellItem 	sellItems[MAX_ITEM_SIZE];	//< 아이템 목록	
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_PACK_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_PACK_LIST_REQ_ACK )
	DWORD 			dwPackageCount;					//< 패키지 목록 갯수
	tblSellPackage 	sellPackages[MAX_PACKAGE_SIZE];	//< 패키지 목록
PACKET_GS_CLOSE

// 무기, 방어구 아이템 확장옵션 요청
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_EXTEND_OPTION_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK )
	DWORD 					dwItemOptionCount;								//< 아이템옵션 목록 갯수
	tblItemExtendOption 	itemExtendOptions[MAX_ITEM_EXTEND_OPTION_SIZE];	//< 아이템옵션 목록	
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BRANCH_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BRANCH_LIST_REQ_ACK )
	DWORD 			dwItemCount;						//< 목록 갯수
	tblItemBranch 	branchItems[MAX_BRANCH_ITEM_SIZE];	//< 목록
PACKET_GS_CLOSE


// (NOTE) 아이템 구매 요청
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BUY_ITEM_SYN )
	DWORD	dwServerGuid;							//< 구매 요청 서버 Guid
	DWORD	dwUserGuid;								//< 사용자 Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
    DWORD   char_class;
    DWORD   char_level;
#endif
	DWORD	dwCharGuid;								//< 캐릭터 Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	//< 캐릭터 이름
	DWORD	dwCount;								//< 구매 아이템/패키지 갯수
	DWORD	dwPriceGuids[MAX_BUY_REQ_SIZE];	
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    enum {
        PURCHASE_DEFAULT = buyItemTransaction::PURCHASE_DEFAULT,
        PURCHASE_PP_CASH = buyItemTransaction::PURCHASE_PP_CASH
    };
    BYTE    buy_cash_types[MAX_BUY_REQ_SIZE];      //< 구매 캐쉬타입 (1:캐쉬, 2:선불카드_PP_)
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
    char        ip_address_[MAX_IP_LENGTH];
#endif
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BUY_ITEM_ACK )
	DWORD					dwServerGuid;					///> 구매 요청 서버 Guid
	DWORD					dwUserGuid;						///> 사용자 Guid
	DWORD					dwCharGuid;						///> 캐릭터 Guid
	DWORD					dwUserCash;						///> 사용자 잔여 캐쉬
#ifdef __CN_0_20070507_SHOP_MILEAGE
	DWORD					dwHonors;						///> 잔여 마일리지 갯수(해방군의 훈장)
#endif
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    DWORD                   pp_cash;                        // 선불카드 캐쉬 잔액
#endif
	DWORD					dwResult;						///> 구매 결과
	DWORD					dwCount;						///> 구매 transaction 갯수
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	///> 구매 transaction
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BUY_ITEM_NTF )
	DWORD					dwServerGuid;					//< 구매 요청 서버 Guid
	DWORD					dwUserGuid;						//< 사용자 Guid
	DWORD					dwCharGuid;						//< 캐릭터 Guid
	DWORD					dwCount;						//< 구매 transaction 갯수
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	//< 구매 transaction			
PACKET_GS_CLOSE



PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_BUY_HISTORY_SYN )
	DWORD	dwServerGuid;	//< 요청 서버 Guid
	DWORD	dwUserGuid;		//< 사용자 Guid
	DWORD	dwCharGuid;		//< 캐릭터 Guid
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_BUY_HISTORY_ACK )
	DWORD		dwServerGuid;					//< 요청 서버 Guid
	DWORD 		dwUserGuid;						//< 사용자 Guid
	DWORD		dwCharGuid;						//< 캐릭터 Guid
	DWORD 		dwCount;						//< 구매 이력 목록 갯수
	tblBuyItem 	buyItems[MAX_BUY_ITEM_SIZE]; 	//< 구매 이력 목록
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_RELOAD_DATA_NTF )
PACKET_GS_CLOSE


// 2007/1/17 오후 4:57:28
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_SYN )
	DWORD	dwServerGuid;								//< 구매 요청 서버 Guid
	DWORD 	dwUserGuid;									//< 사용자 Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char    user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
    DWORD   char_class;
    DWORD   char_level;
#endif
	DWORD	dwCharGuid;									//< 캐릭터 Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];		//< 캐릭터 이름
	DWORD	dwToServerGuid;								//< 선물을 받을 서버 Guid
	DWORD 	dwToUserGuid;								//< 선물을 받을 사용자 Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char    to_user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
    DWORD   to_char_class;
    DWORD   to_char_level;
#endif
	DWORD	dwToCharGuid;								//< 선물을 받을 캐릭터 Guid	
	char	szToCharName[MAX_CHARACTER_NAME_LENGTH];  	//< 선물을 받을 캐릭터 이름
	DWORD 	dwPriceGuid;								//< 판매 가격 고유 번호
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    enum {
        PURCHASE_DEFAULT = buyItemTransaction::PURCHASE_DEFAULT,
        PURCHASE_PP_CASH = buyItemTransaction::PURCHASE_PP_CASH
    };
    BYTE   buy_cash_type;                              //< 구매 캐쉬 타입(1:etc, 2:pp_cash)
#endif
	char 	szGiftMessage[MAX_GIFT_MESSAGE_LENGTH];		//< 선물 메세지
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
    char        ip_address_[MAX_IP_LENGTH];
#endif
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_ACK )
	DWORD	dwServerGuid;		//< 구매 요청 서버 Guid
	DWORD 	dwUserGuid;			//< 사용자 Guid
	DWORD	dwCharGuid;			//< 캐릭터 Guid
	DWORD	dwToServerGuid;		//< 선물을 받을 사용자 Guid
	DWORD	dwToUserGuid;		//< 선물을 받을 사용자 Guid
	DWORD	dwToCharGuid;		//< 선물을 받을 캐릭터 Guid
	DWORD	dwUserCash;			//< 사용자 잔여 캐쉬
#ifdef __CN_0_20070507_SHOP_MILEAGE
	DWORD	dwHonors;			///> 잔여 마일리지 갯수(해방군의 훈장)
#endif
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    DWORD   pp_cash;            //< 잔여 선불 캐쉬
#endif
	DWORD	dwTransactionGuid;	//< 구매 요청 Transaction Guid
	DWORD 	dwPriceGuid;		//< 판매 가격 고유 번호
	DWORD	dwResult;			//< 구매 결과
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_SYN )
	DWORD	dwServerGuid;	//< 서버 GUID
	DWORD 	dwUserGuid;		//< 사용자 GUID
	DWORD	dwCharGuid;		//< 캐릭터 GUID
	DWORD	dwGiftItemID;	//< 선물 ID
	DWORD 	dwAction;		//< 선물 처리 방법 (1:선물 받기, 2:선물 버리기, 3:목록에서 선물 삭제) 참고 : eGIFT_ACTION
	char 	szMessage[MAX_GIFT_MESSAGE_LENGTH];		//< 선물 메세지
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_ACK )
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD 	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD	dwGiftItemID;	///> 선물 ID
	DWORD 	dwAction;		///> 선물 처리 방법 (1:선물 받기, 2:선물 버리기, 3:목록에서 선물 삭제) 참고 : eGIFT_ACTION
	DWORD	dwResult;		///> 처리 결과
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_NTF )
	DWORD	dwServerGuid;	//< 서버 GUID
	DWORD 	dwUserGuid;		//< 사용자 GUID
	DWORD	dwCharGuid;		//< 캐릭터 GUID
	DWORD	dwGiftItemID;	//< 선물 GUID
	DWORD	dwResult;		//< 처리 결과(0 : 실패, 1 :  성공)
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_IS_RECV_GIFT_SYN )
	DWORD		dwServerGuid;	//< 서버 GUID
	DWORD 		dwUserGuid;		//< 사용자 GUID
	DWORD		dwCharGuid;		//< 캐릭터 GUID
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_IS_RECV_GIFT_ACK )
	DWORD	dwServerGuid;	//< 서버 GUID
	DWORD 	dwUserGuid;		//< 사용자 GUID
	DWORD	dwCharGuid;		//< 캐릭터 GUID
	DWORD 	dwGiftCount;	//< 선물 존재 여부 (0 : 받은 선물 없음 N : N개의 받은 선물이 있음)
PACKET_GS_CLOSE



PACKET_GS_OPEN( GS_SHOP, GS_SHOP_RECV_GIFT_LIST_SYN )
	DWORD	dwServerGuid;	//< 서버 GUID
	DWORD	dwUserGuid;		//< 사용자 GUID
	DWORD	dwCharGuid;		//< 캐릭터 GUID
	DWORD	dwMaxCount;		//< 최대 목록 갯수(0 : 기본, N : N개의 최대 목록)
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_RECV_GIFT_LIST_ACK )
	DWORD			dwServerGuid;						//< 서버 GUID
	DWORD 			dwUserGuid;							//< 사용자 GUID
	DWORD			dwCharGuid;							//< 캐릭터 GUID
	DWORD			dwCount;							//< 전송 목록 갯수
	tblRecvGiftItem recvGiftItems[MAX_GIFT_ITEM_SIZE];	//< 받은 선물 목록
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_LIST_SYN )
	DWORD	dwServerGuid;	//< 서버 GUID
	DWORD	dwUserGuid;		//< 사용자 GUID
	DWORD	dwCharGuid;		//< 캐릭터 GUID
	DWORD	dwMaxCount;		//< 최대 목록 갯수(0 : 기본, N : N개의 최대 목록)
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_LIST_ACK )
	DWORD			dwServerGuid;	//< 서버 GUID
	DWORD			dwUserGuid;		//< 사용자 GUID
	DWORD			dwCharGuid;		//< 캐릭터 GUID
	DWORD			dwCount;							//< 전송 목록 갯수
	tblSendGiftItem sendGiftItems[MAX_GIFT_ITEM_SIZE];	//< 보낸 선물 목록
PACKET_GS_CLOSE

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
// 마일리지 관련 패킷
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_MILEAGE_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_MILEAGE_LIST_REQ_ACK )
	DWORD 				dwItemCount;							//< 판매 마일리지 아이템 목록 갯수
	tblSellItem 	sellItems[MAX_ITEM_SIZE];					//< 아이템 목록	
	// 원래는 이 struct 임 tblSellItem == tblSellMileageItem
	// tblSellMileageItem 	sellMileageItems[MAX_ITEM_SIZE];	//< 판매 마일리지 아이템 목록	
PACKET_GS_CLOSE
#endif

#pragma pack(pop)

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
