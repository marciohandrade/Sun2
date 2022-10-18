
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#pragma once
#ifndef SOLAR_SHOP_SHOP_SERVER_PROTOCOL_DEFINITION_H
#define SOLAR_SHOP_SHOP_SERVER_PROTOCOL_DEFINITION_H

#if SUN_CODE_BACKUP

// NOTE: this file declares protocols related to a ShopServer communication.
// the SUN server redefine a below protocol information to fit the SUN protocol format.

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////////////////
/// protocol define
/// GAME	: Game Server
///	SHOP	: ItemShop Server
/// Manager : ItemShop Manager

//////////////////////////////////////////////////////////////////////////
///  protocol id (GAME <-> SHOP)
#define GTOS_USER_CASH_REQ				0x0100		///> 잔여캐쉬 조회 요청    
#define STOG_USER_CASH_ANS				0x0200		///> 잔여캐쉬 조회 전송    
#define GTOS_ITEM_LIST_REQ				0x0300		///> 유료 아이템 목록 요청
#define STOG_ITEM_LIST_ANS				0x0400		///> 유료 아이템 목록 전송 
#define GTOS_PACKAGE_LIST_REQ			0x0500		///> 패키지 목록 요청	   
#define STOG_PACKAGE_LIST_ANS			0x0600		///> 패키지 목록 전송      
#define GTOS_BRANCH_ITEM_LIST_REQ		0x0700		///> 특화 분료 아이템 목록 요청	
#define STOG_BRANCH_ITEM_LIST_ANS		0x0800		///> 특화 분료 아이템 목록 전송
#define GTOS_BUY_ITEM_REQ				0x0900		///> 아이템 구매 요청      
#define STOG_BUY_ITEM_ANS				0x0A00		///> 아이템 구매 전송      
#define GTOS_BUY_ITEM_NTF				0x0B00		///> 아이템 구매 게임내 처리 결과
#define GTOS_USER_BUY_HISTORY_REQ		0x0C00		///> 사용자 구매 이력 요청 
#define STOG_USER_BUY_HISTORY_ANS		0x0D00		///> 사용자 구매 이력 전송 
#define STOG_RELOAD_SHOP_DATA_NTF		0x0E00		///> SHOP DATA 갱신 요청
#define GTOS_SEND_GIFT_ITEM_REQ			0x0F00		///> 아이템 선물 하기 요청
#define STOG_SEND_GIFT_ITEM_ANS			0x1000		///> 아이템 선물 하기 결과 전송
#define GTOS_ACTION_RECV_GIFT_REQ		0x1100		///> 받은 선물 처리 요청
#define STOG_ACTION_RECV_GIFT_ANS		0x1200		///> 받은 선물 처리 요청 결과 전송
#define GTOS_ACTION_RECV_GIFT_NTF		0x1300		///> 게임내 처리 결과 전송
#define GTOS_IS_RECV_GIFT_REQ			0x1400		///> 받은 선물 존재 여부
#define STOG_IS_RECV_GIFT_ANS			0x1500		///> 받은 선물 존재 여부 확인 결과 전송
#define GTOS_RECV_GIFT_LIST_REQ			0x1600		///> 받은 선물 목록 요청
#define STOG_RECV_GIFT_LIST_ANS			0x1700		///> 받은 선물 목록 전송
#define GTOS_SEND_GIFT_LIST_REQ			0x1800		///> 보낸 선물 목록 요청
#define STOG_SEND_GIFT_LIST_ANS			0x1900		///> 보낸 선물 목록 전송

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
//2007/6/5 추가
#define GTOS_MILEAGE_ITEM_LIST_REQ		0x1A00		///> 마일리지 아이템 목록 요청
#define STOG_MILEAGE_ITEM_LIST_ANS		0x1B00		///> 마일리지 아이템 목록 전송
#endif

//////////////////////////////////////////////////////////////////////////
///  protocol id (Manager <-> SHOP)
#define MTOS_RELOAD_SHOP_DATA_NTF		0x0101		///> SHOP DATA 갱신 요청

namespace protocol {

class HDRMessageID4
{
public:
	BYTE category;
	BYTE cessageID;
};

class HDRMessageID8
{
public:
	WORD category;
	WORD messageID;
};

class MSG_HEADER4
{
public:
	WORD		size;				
	union
	{
		WORD			id;			//  Message ID
		HDRMessageID4	hdrid;
	};
};

class MSG_HEADER8
{
public:
	DWORD		size;				
	union
	{
		DWORD			id;			//  Message ID
		HDRMessageID8	hdrid;
	};	
};

/*
	SUN은 4바이트 헤더에 사이즈 2byte가 포함되지 않은 헤더를 사용한다.
	#define __HEADER4_SIZE_2__ 
	MU는 8바이트 해더에 전체 헤더 사이즈가 포함된 헤더를 사용한다.
*/

#define __HEADER4_SIZE_2__ 

#ifdef	__HEADER4_SIZE_2__
	///////////////////////////////////////////////////////////////////////////////
	/// 4Byte Header	size에 header size 2byte만 포함
	//#define  MSG_HEADER		MSG_HEADER4
    //#define  SIZEOFHDR		2
    typedef MSG_HEADER4 MSG_HEADER;
    const WORD SIZEOFHDR = 2;

#elif	__HEADER8_SIZE_8__
	///////////////////////////////////////////////////////////////////////////////
	/// 8Byte Header	size에 header size 미포함
	//#define  MSG_HEADER		MSG_HEADER8
	//#define  SIZEOFHDR		8
    typedef MSG_HEADER8 MSG_HEADER;
    const WORD SIZEOFHDR = 8;

#else
	///////////////////////////////////////////////////////////////////////////////
	/// 8Byte Header	size에 header size 포함
	//#define  MSG_HEADER		MSG_HEADER8
	//#define  SIZEOFHDR		0
    typedef MSG_HEADER8 MSG_HEADER;
    const WORD SIZEOFHDR = 8;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 잔여캐쉬 조회 요청
class MSG_GTOS_USER_CASH_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_USER_CASH_REQ()
	{
		size	= sizeof(MSG_GTOS_USER_CASH_REQ) - SIZEOFHDR;
		id		= GTOS_USER_CASH_REQ;
	}
	~MSG_GTOS_USER_CASH_REQ()
	{
	}

	DWORD	dwUserGuid;	///> 사용자 Guid
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 잔여캐쉬 조회 전송
class MSG_STOG_USER_CASH_ANS : public MSG_HEADER
{
public:
	MSG_STOG_USER_CASH_ANS()
	{
		size	= sizeof(MSG_STOG_USER_CASH_ANS) - SIZEOFHDR;
		id		= STOG_USER_CASH_ANS;
	}
	~MSG_STOG_USER_CASH_ANS()
	{
	}

	DWORD	dwUserGuid;	///> 사용자 Guid
	DWORD	dwUserCash;	///> 잔여 캐쉬
#ifdef SHOPSERVER_FOR_CHINA
	DWORD	dwHonors;	///> 잔여 마일리지 갯수(해방군의 훈장)
#endif
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 유료 아이템 목록 요청
class MSG_GTOS_ITEM_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_ITEM_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_ITEM_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_ITEM_LIST_REQ;
	}
	~MSG_GTOS_ITEM_LIST_REQ()
	{
	}	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 유료 아이템 목록 전송
class MSG_STOG_ITEM_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_ITEM_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_ITEM_LIST_ANS) - SIZEOFHDR;
		id		= STOG_ITEM_LIST_ANS;
	}
	~MSG_STOG_ITEM_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_ITEM_SIZE - (WORD)dwItemCount)* sizeof(tblSellItem);
	}
	
	DWORD 			dwItemCount;				///> 아이템 목록 갯수
	tblSellItem 	sellItems[MAX_ITEM_SIZE];	///> 아이템 목록	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 패키지 목록 요청
class MSG_GTOS_PACKAGE_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_PACKAGE_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_PACKAGE_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_PACKAGE_LIST_REQ;
	}
	~MSG_GTOS_PACKAGE_LIST_REQ()
	{
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 패키지 목록 전송
class MSG_STOG_PACKAGE_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_PACKAGE_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_PACKAGE_LIST_ANS) - SIZEOFHDR;
		id		= STOG_PACKAGE_LIST_ANS;
	}
	~MSG_STOG_PACKAGE_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_PACKAGE_SIZE - (WORD)dwPackageCount)* sizeof(tblSellPackage);
	}
	
	DWORD 			dwPackageCount;					///> 패키지 목록 갯수
	tblSellPackage 	sellPackages[MAX_PACKAGE_SIZE];	///> 패키지 목록
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 분료 아이템 목록 요청
class MSG_GTOS_BRANCH_ITEM_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_BRANCH_ITEM_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_BRANCH_ITEM_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_BRANCH_ITEM_LIST_REQ;
	}
	~MSG_GTOS_BRANCH_ITEM_LIST_REQ()
	{
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 특화 분료 아이템 목록 전송
class MSG_STOG_BRANCH_ITEM_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_BRANCH_ITEM_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_BRANCH_ITEM_LIST_ANS) - SIZEOFHDR;
		id		= STOG_BRANCH_ITEM_LIST_ANS;
	}
	~MSG_STOG_BRANCH_ITEM_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_BRANCH_ITEM_SIZE - (WORD)dwItemCount)* sizeof(tblItemBranch);
	}
	
	DWORD 			dwItemCount;						///> 목록 갯수
	tblItemBranch 	branchItems[MAX_BRANCH_ITEM_SIZE];	///> 목록
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 아이템 구매 요청
class MSG_GTOS_BUY_ITEM_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_BUY_ITEM_REQ()
	{
		size	= sizeof(MSG_GTOS_BUY_ITEM_REQ) - SIZEOFHDR;
		id		= GTOS_BUY_ITEM_REQ;
	}
	~MSG_GTOS_BUY_ITEM_REQ()
	{
	}

	void resize()
	{
		size = size - (MAX_BUY_REQ_SIZE - (WORD)dwCount)* sizeof(DWORD);
	}

	DWORD	dwServerGuid;							///> 구매 요청 서버 Guid
	DWORD	dwUserGuid;								///> 사용자 Guid
	DWORD	dwCharGuid;								///> 캐릭터 Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	///> 캐릭터 이름
	DWORD	dwCount;								///> 구매 아이템/패키지 갯수
	DWORD	dwPriceGuids[MAX_BUY_REQ_SIZE];			///> 판매 가격 고유 번호 목록	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 아이템 구매 전송
class MSG_STOG_BUY_ITEM_ANS : public MSG_HEADER
{
public:
	MSG_STOG_BUY_ITEM_ANS()
	{
		size	= sizeof(MSG_STOG_BUY_ITEM_ANS) - SIZEOFHDR;
		id		= STOG_BUY_ITEM_ANS;
	}
	~MSG_STOG_BUY_ITEM_ANS()
	{
	}

	void resize()
	{
		size = size - (MAX_BUY_REQ_SIZE - (WORD)dwCount)* sizeof(tblBuyItemTransaction);
	}
	
	DWORD					dwServerGuid;					///> 구매 요청 서버 Guid
	DWORD					dwUserGuid;						///> 사용자 Guid
	DWORD					dwCharGuid;						///> 캐릭터 Guid
	DWORD					dwUserCash;						///> 사용자 잔여 캐쉬
#ifdef SHOPSERVER_FOR_CHINA
	DWORD					dwHonors;						///> 잔여 마일리지 갯수(해방군의 훈장)
#endif
	DWORD					dwResult;						///> 구매 결과
	DWORD					dwCount;						///> 구매 transaction 갯수
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	///> 구매 transaction
	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 아이템 구매 게임내 처리 결과
class MSG_GTOS_BUY_ITEM_NTF : public MSG_HEADER
{
public:
	MSG_GTOS_BUY_ITEM_NTF()
	{
		size	= sizeof(MSG_GTOS_BUY_ITEM_NTF) - SIZEOFHDR;
		id		= GTOS_BUY_ITEM_NTF;
	}
	~MSG_GTOS_BUY_ITEM_NTF()
	{
	}

	void resize()
	{
		size = size - (MAX_BUY_REQ_SIZE - (WORD)dwCount)* sizeof(tblBuyItemTransaction);
	}

	DWORD					dwServerGuid;					///> 구매 요청 서버 Guid
	DWORD					dwUserGuid;						///> 사용자 Guid
	DWORD					dwCharGuid;						///> 캐릭터 Guid
	DWORD					dwCount;						///> 구매 transaction 갯수
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	///> 구매 transaction		
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 사용자 구매 이력 요청
class MSG_GTOS_USER_BUY_HISTORY_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_USER_BUY_HISTORY_REQ()
	{
		size	= sizeof(MSG_GTOS_USER_BUY_HISTORY_REQ) - SIZEOFHDR;
		id		= GTOS_USER_BUY_HISTORY_REQ;
	}
	~MSG_GTOS_USER_BUY_HISTORY_REQ()
	{
	}

	DWORD	dwServerGuid;	///> 요청 서버 Guid
	DWORD	dwUserGuid;		///> 사용자 Guid
	DWORD	dwCharGuid;		///> 캐릭터 Guid
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 사용자 구매 이력 전송
class MSG_STOG_USER_BUY_HISTORY_ANS : public MSG_HEADER
{
public:
	MSG_STOG_USER_BUY_HISTORY_ANS()
	{
		size	= sizeof(MSG_STOG_USER_BUY_HISTORY_ANS) - SIZEOFHDR;
		id		= STOG_USER_BUY_HISTORY_ANS;
	}
	~MSG_STOG_USER_BUY_HISTORY_ANS()
	{
	}
	void resize()
	{
		size = size - (MAX_BUY_ITEM_SIZE - (WORD)dwCount)* sizeof(tblBuyItem);
	}

	DWORD		dwServerGuid;					///> 요청 서버 Guid
	DWORD 		dwUserGuid;						///> 사용자 Guid
	DWORD		dwCharGuid;						///> 캐릭터 Guid
	DWORD 		dwCount;						///> 구매 이력 목록 갯수
	tblBuyItem 	buyItems[MAX_BUY_ITEM_SIZE]; 	///> 구매 이력 목록
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 샵 데이타 변경에 대한 갱신 요청을 한다.
class MSG_STOG_RELOAD_SHOP_DATA_NTF : public MSG_HEADER
{
public:
	MSG_STOG_RELOAD_SHOP_DATA_NTF()
	{
		size	= sizeof(MSG_STOG_RELOAD_SHOP_DATA_NTF) - SIZEOFHDR;
		id		= STOG_RELOAD_SHOP_DATA_NTF;
	}
	~MSG_STOG_RELOAD_SHOP_DATA_NTF()
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : Manager -> GAME
//  Description : 샵 데이타 변경에 대한 갱신 요청을 한다.
class MSG_MTOS_RELOAD_SHOP_DATA_NTF : public MSG_HEADER8
{
public:
	MSG_MTOS_RELOAD_SHOP_DATA_NTF()
	{
		size	= sizeof(MSG_MTOS_RELOAD_SHOP_DATA_NTF);
		id		= MTOS_RELOAD_SHOP_DATA_NTF;
	}
	~MSG_MTOS_RELOAD_SHOP_DATA_NTF()
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 아이템 선물하기
class MSG_GTOS_SEND_GIFT_ITEM_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_SEND_GIFT_ITEM_REQ()
	{
		size	= sizeof(MSG_GTOS_SEND_GIFT_ITEM_REQ) - SIZEOFHDR;
		id		= GTOS_SEND_GIFT_ITEM_REQ;
	}
	~MSG_GTOS_SEND_GIFT_ITEM_REQ()
	{
	}
	
	DWORD	dwServerGuid;								///> 구매 요청 서버 Guid
	DWORD 	dwUserGuid;									///> 사용자 Guid
	DWORD	dwCharGuid;									///> 캐릭터 Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];		///> 캐릭터 이름
	DWORD	dwToServerGuid;								///> 선물을 받을 서버 Guid
	DWORD 	dwToUserGuid;								///> 선물을 받을 사용자 Guid
	DWORD	dwToCharGuid;								///> 선물을 받을 캐릭터 Guid	
	char	szToCharName[MAX_CHARACTER_NAME_LENGTH];  	///> 선물을 받을 캐릭터 이름
	DWORD 	dwPriceGuid;								///> 판매 가격 고유 번호
	char 	szGiftMessage[MAX_GIFT_MESSAGE_LENGTH];		///> 선물 메세지
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 아이템 선물 하기 결과 전송
class MSG_STOG_SEND_GIFT_ITEM_ANS : public MSG_HEADER
{
public:
	MSG_STOG_SEND_GIFT_ITEM_ANS()
	{
		size	= sizeof(MSG_STOG_SEND_GIFT_ITEM_ANS) - SIZEOFHDR;
		id		= STOG_SEND_GIFT_ITEM_ANS;
	}
	~MSG_STOG_SEND_GIFT_ITEM_ANS()
	{
	}
	
	DWORD	dwServerGuid;		///> 구매 요청 서버 Guid
	DWORD 	dwUserGuid;			///> 사용자 Guid
	DWORD	dwCharGuid;			///> 캐릭터 Guid
	DWORD	dwToServerGuid;		///> 선물을 받을 서버 Guid
	DWORD	dwToUserGuid;		///> 선물을 받을 사용자 Guid
	DWORD	dwToCharGuid;		///> 선물을 받을 캐릭터 Guid
	DWORD	dwUserCash;			///> 사용자 잔여 캐쉬
#ifdef SHOPSERVER_FOR_CHINA
	DWORD	dwHonors;			///> 잔여 마일리지 갯수(해방군의 훈장)
#endif
	DWORD	dwTransactionGuid;	///> 구매 요청 Transaction Guid
	DWORD 	dwPriceGuid;		///> 판매 가격 고유 번호
	DWORD	dwResult;			///> 구매 결과
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 아이템 선물 받기 요청
class MSG_GTOS_ACTION_RECV_GIFT_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_ACTION_RECV_GIFT_REQ()
	{
		size	= sizeof(MSG_GTOS_ACTION_RECV_GIFT_REQ) - SIZEOFHDR;
		id		= GTOS_ACTION_RECV_GIFT_REQ;
	}
	~MSG_GTOS_ACTION_RECV_GIFT_REQ()
	{
	}
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD 	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD	dwGiftItemID;	///> 선물 ID
	DWORD 	dwAction;		///> 선물 처리 방법 (1:선물 받기, 2:선물 버리기, 3:목록에서 선물 삭제) 참고 : eGIFT_STATE
	char 	szMessage[MAX_GIFT_MESSAGE_LENGTH];		///> 선물 메세지
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 아이템 선물 받기 결과 전송
class MSG_STOG_ACTION_RECV_GIFT_ANS : public MSG_HEADER
{
public:
	MSG_STOG_ACTION_RECV_GIFT_ANS()
	{
		size	= sizeof(MSG_STOG_ACTION_RECV_GIFT_ANS) - SIZEOFHDR;
		id		= STOG_ACTION_RECV_GIFT_ANS;
	}
	~MSG_STOG_ACTION_RECV_GIFT_ANS()
	{
	}
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD 	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD	dwGiftItemID;	///> 선물 ID
	DWORD 	dwAction;		///> 선물 처리 방법 (1:선물 받기, 2:선물 버리기, 3:목록에서 선물 삭제) 참고 : eGIFT_STATE
	DWORD	dwResult;		///> 처리 결과
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 아이템 선물 받기 결과 전송
class MSG_GTOS_ACTION_RECV_GIFT_NTF : public MSG_HEADER
{
public:
	MSG_GTOS_ACTION_RECV_GIFT_NTF()
	{
		size	= sizeof(MSG_GTOS_ACTION_RECV_GIFT_NTF) - SIZEOFHDR;
		id		= GTOS_ACTION_RECV_GIFT_NTF;
	}
	~MSG_GTOS_ACTION_RECV_GIFT_NTF()
	{
	}
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD 	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD	dwGiftItemID;	///> 선물 GUID
	DWORD	dwResult;		///> 처리 결과(0 : 실패, 1 :  성공)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 받은 선물 존재 여부
class MSG_GTOS_IS_RECV_GIFT_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_IS_RECV_GIFT_REQ()
	{
		size	= sizeof(MSG_GTOS_IS_RECV_GIFT_REQ) - SIZEOFHDR;
		id		= GTOS_IS_RECV_GIFT_REQ;
	}
	~MSG_GTOS_IS_RECV_GIFT_REQ()
	{
	}
	
	DWORD		dwServerGuid;	///> 서버 GUID
	DWORD 		dwUserGuid;		///> 사용자 GUID
	DWORD		dwCharGuid;		///> 캐릭터 GUID
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 받은 선물 존재 여부 확인 결과 전송
class MSG_STOG_IS_RECV_GIFT_ANS : public MSG_HEADER
{
public:
	MSG_STOG_IS_RECV_GIFT_ANS()
	{
		size	= sizeof(MSG_STOG_IS_RECV_GIFT_ANS) - SIZEOFHDR;
		id		= STOG_IS_RECV_GIFT_ANS;
	}
	~MSG_STOG_IS_RECV_GIFT_ANS()
	{
	}
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD 	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD 	dwGiftCount;	///> 선물 존재 여부 (0 : 받은 선물 없음 N : N개의 받은 선물이 있음)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 받은 선물 목록 요청
class MSG_GTOS_RECV_GIFT_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_RECV_GIFT_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_RECV_GIFT_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_RECV_GIFT_LIST_REQ;
	}
	~MSG_GTOS_RECV_GIFT_LIST_REQ()
	{
	}
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD	dwMaxCount;		///> 최대 목록 갯수(0 : 기본, N : N개의 최대 목록)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 받은 선물 목록 전송
class MSG_STOG_RECV_GIFT_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_RECV_GIFT_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_RECV_GIFT_LIST_ANS) - SIZEOFHDR;
		id		= STOG_RECV_GIFT_LIST_ANS;
	}
	~MSG_STOG_RECV_GIFT_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_GIFT_ITEM_SIZE - (WORD)dwCount)* sizeof(tblRecvGiftItem);
	}
	
	DWORD			dwServerGuid;						///> 서버 GUID
	DWORD 			dwUserGuid;							///> 사용자 GUID
	DWORD			dwCharGuid;							///> 캐릭터 GUID
	DWORD			dwCount;							///> 전송 목록 갯수
	tblRecvGiftItem recvGiftItems[MAX_GIFT_ITEM_SIZE];	///> 받은 선물 목록
};


//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 보낸 선물 목록 요청
class MSG_GTOS_SEND_GIFT_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_SEND_GIFT_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_SEND_GIFT_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_SEND_GIFT_LIST_REQ;
	}
	~MSG_GTOS_SEND_GIFT_LIST_REQ()
	{
	}
	DWORD	dwServerGuid;	///> 서버 GUID
	DWORD	dwUserGuid;		///> 사용자 GUID
	DWORD	dwCharGuid;		///> 캐릭터 GUID
	DWORD	dwMaxCount;		///> 최대 목록 갯수(0 : 기본, N : N개의 최대 목록)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 보낸 선물 목록 전송
class MSG_STOG_SEND_GIFT_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_SEND_GIFT_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_SEND_GIFT_LIST_ANS) - SIZEOFHDR;
		id		= STOG_SEND_GIFT_LIST_ANS;
	}
	~MSG_STOG_SEND_GIFT_LIST_ANS()
	{
	}

	void resize()
	{
		size = size - (MAX_GIFT_ITEM_SIZE - (WORD)dwCount)* sizeof(tblSendGiftItem);
	}

	DWORD			dwServerGuid;	///> 서버 GUID
	DWORD			dwUserGuid;		///> 사용자 GUID
	DWORD			dwCharGuid;		///> 캐릭터 GUID
	DWORD			dwCount;							///> 전송 목록 갯수
	tblSendGiftItem sendGiftItems[MAX_GIFT_ITEM_SIZE];	///> 보낸 선물 목록
};

#ifdef __NA_00387_20070605_SHOP_MILEAGE2

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : 판매 마일리지 아이템 목록 요청
class MSG_GTOS_MILEAGE_ITEM_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_MILEAGE_ITEM_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_MILEAGE_ITEM_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_MILEAGE_ITEM_LIST_REQ;
	}
	~MSG_GTOS_MILEAGE_ITEM_LIST_REQ()
	{
	}	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : 판매 마일리지 아이템 목록 전송
class MSG_STOG_MILEAGE_ITEM_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_MILEAGE_ITEM_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_MILEAGE_ITEM_LIST_ANS) - SIZEOFHDR;
		id		= STOG_MILEAGE_ITEM_LIST_ANS;
	}
	~MSG_STOG_MILEAGE_ITEM_LIST_ANS()
	{
	}

	void resize()
	{
		size = size - (MAX_ITEM_SIZE - (WORD)dwItemCount)* sizeof(tblSellMileageItem);
	}

	DWORD 				dwItemCount;														///> 판매 마일리지 아이템 목록 갯수
	tblSellMileageItem 	sellMileageItems[MAX_ITEM_SIZE];	///> 판매 마일리지 아이템 목록	
};

#endif

};// namespace protocol

using namespace protocol;
#pragma pack()

#endif //SUN_CODE_BACKUP

#endif //SOLAR_SHOP_SHOP_SERVER_PROTOCOL_DEFINITION_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
