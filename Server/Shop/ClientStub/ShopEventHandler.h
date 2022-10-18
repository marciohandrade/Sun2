#pragma once

/**************************************************************************************************
작성일: 2008-07-08
설명: 샵 서버를 사용하는 클라이언트에게 수신된 패킷을 전달해 주기 위한 이벤트 객체 
      이벤트를 받는쪽(샵클라이언트)에서 상속받아 사용해야 한다.
**************************************************************************************************/

#include "ClientSession.h"
#include <public/protocol_shop.h>

class CShopEventHandler : public CClientSession
{
public:
	CShopEventHandler(void);
	virtual  ~CShopEventHandler(void);

	// 초기화 함수 (프로그램 실행하고 한번만 사용해야 한다.)
	bool CreateSession(WBANetwork::LPFN_ErrorHandler errhandler = NULL);


// 캐시조회
	// 캐시조회 : MU_KR, BT_KR, R2_KR, SUN_KR
	bool InquireCash(
		long	GameCode,
		BYTE	ViewType, 
		DWORD	AccountSeq, 
		bool	SumOnly, 
		long	MileageSection);

	// 캐시조회 : MU_GB, SUN_GB
	bool InquireCash(
		BYTE	ViewType, 
		DWORD	AccountSeq, 
		long	GameCode,
		bool	SumOnly, 
		long	MileageSection);

	// 캐시조회 : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_VN, MU_TW
	bool InquireCash(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		long	AccountSeq, 
		int		USN, 
		BYTE	ViewType,
		long	MileageSection,
		bool	SumOnly);

	// 캐시조회 : OB
	bool InquireCash(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH],
		BYTE	ViewType,
		long	MileageSection,
		bool	SumOnly);

	// 캐시조회 : BT_CNL
	bool InquireCash(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH],
		BYTE	ViewType,
		long	MileageSection,
		bool	SumOnly,
		WCHAR	strMemberNo[MAX_CHANNELING_LENGTH]);

// 상품구매
	// [deprecated] - [AccessIP 추가 버전] 사용 권장
	// 상품구매 : MU_KR, BT_KR, R2_KR, SUN KR
	bool BuyProduct(
		long	GameCode,
		DWORD	AccountSeq, 
		long	PackageProductSeq, 
		long	ProductDisplaySeq, 
		long	SalesZone, 
		long	PriceSeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);

	// [AccessIP 추가 버전]
	// 상품구매 : MU_KR, BT_KR, R2_KR, SUN KR
	bool BuyProduct(
		long	GameCode,
		DWORD	AccountSeq, 
		long	PackageProductSeq, 
		long	ProductDisplaySeq, 
		long	SalesZone, 
		long	PriceSeq, 
		long	Class, 
		long	Level,
		DWORD	AccessIP,						// <----- AccessIP 추가 버전
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);

	// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
	bool BuyProduct(
		DWORD	AccountSeq, 
		long	GameCode,
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		int		CashTypeCode);

	// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
	bool BuyProduct(
		DWORD	AccountSeq, 
		long	GameCode,
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		int		CashTypeCode,
		bool	DeductMileageFlag);				// <----- DeductMileageFlag 추가

	// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
	// 상품구매 : MU_GB, SUN_GB
	bool BuyProduct(
		DWORD	AccountSeq, 
		long	GameCode,
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		int		CashTypeCode,
		bool	DeductMileageFlag,				// <----- DeductMileageFlag 추가
		DWORD	AccessIP);

	// 상품구매 : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_VN, MU_TW
	bool BuyProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		int		USN, 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		DWORD	dwIPAddress, 
		bool	DeductMileageFlag);

	// 상품구매 : MU_TH
	bool BuyProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		DWORD	dwIPAddress, 
		char	RefKey[MAX_TYPENAME_LENGTH], 
		bool	DeductMileageFlag);

	// [deprecated] - [AccessIP 추가 버전] 사용 권장
	// 상품구매 : BT_CNL
	bool BuyProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		DWORD	dwIPAddress, 
		char	RefKey[MAX_TYPENAME_LENGTH], 
		bool	DeductMileageFlag,
		WCHAR	strMemberNo[MAX_CHANNELING_LENGTH]);


	// [AccessIP 추가 버전]
	// 상품구매 : BT_CNL
	bool BuyProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex,
		DWORD	dwIPAddress, 
		char	RefKey[MAX_TYPENAME_LENGTH], 
		bool	DeductMileageFlag,
		WCHAR	strMemberNo[MAX_CHANNELING_LENGTH],
		DWORD	AccessIP);						// <----- AccessIP 추가 버전

// 상품선물
	// [deprecated] - [AccessIP 추가 버전] 사용 권장
	// 상품 선물 : MU_KR, BT_KR, R2_KR, SUN_KR
	bool GiftProduct(
		long	GameCode,
		DWORD	SenderSeq, 
		long	SenderServerIndex, 
		WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
		DWORD	ReceiverSeq, 
		long	ReceiverServerIndex, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq);

	// [AccessIP 추가 버전]
	// 상품선물 : MU_KR, BT_KR, R2_KR, SUN_KR
	bool GiftProduct(
		long	GameCode,
		DWORD	SenderSeq, 
		long	SenderServerIndex, 
		WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
		DWORD	ReceiverSeq, 
		long	ReceiverServerIndex, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq,
		DWORD	AccessIP);					// <----- AccessIP 추가 버전

	// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
	// 상품선물 : MU_GB
	bool GiftProduct(
		DWORD	SenderAccountSeq, 
		WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
		long	SenderServerIndex,
		DWORD	ReceiverAccountSeq, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex,
		WCHAR	SendMessage[MAX_MESSAGE_LENGTH], 
		long	GameCode,
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		int		CashTypeCode);

	// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
	// 상품선물 : MU_GB, SUN_GB
	bool GiftProduct(
		DWORD	SenderAccountSeq, 
		WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
		long	SenderServerIndex,
		DWORD	ReceiverAccountSeq, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex,
		WCHAR	SendMessage[MAX_MESSAGE_LENGTH], 
		long	GameCode,
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		int		CashTypeCode,
		bool	DeductMileageFlag);					// <----- DeductMileageFlag 추가

	// [DeductMileageFlag & AccessIP 추가 버전]
	// 상품선물 : MU_GB, SUN_GB
	bool GiftProduct(
		DWORD	SenderAccountSeq, 
		WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
		long	SenderServerIndex,
		DWORD	ReceiverAccountSeq, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex,
		WCHAR	SendMessage[MAX_MESSAGE_LENGTH], 
		long	GameCode,
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		int		CashTypeCode,
		bool	DeductMileageFlag,					// <----- DeductMileageFlag 추가
		DWORD	AccessIP);							// <----- AccessIP 추가(client ip)

	// 상품선물 : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_VN, MU_TW
	bool GiftProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		int		USN, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		long	ServerIndex, 
		DWORD	ReceiverAccountSeq, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex, 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		DWORD	dwIPAddress, 
		bool	DeductMileageFlag);

	// 상품선물 : MU_TH
	bool GiftProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		long	ServerIndex, 
		DWORD	ReceiverAccountSeq, 
		char	ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex, 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		DWORD	dwIPAddress, 
		char	RefKey[MAX_TYPENAME_LENGTH],
		bool	DeductMileageFlag);

	// [deprecated] - [AccessIP 추가 버전] 사용 권장
	// 상품선물 : BT_CNL
	bool GiftProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		long	ServerIndex, 
		DWORD	ReceiverAccountSeq, 
		char	ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex, 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		DWORD	dwIPAddress, 
		char	RefKey[MAX_TYPENAME_LENGTH],
		bool	DeductMileageFlag,
		WCHAR	strMemberNo[MAX_CHANNELING_LENGTH]);

	// [AccessIP 추가 버전]
	// 상품선물 : BT_CNL
	bool GiftProduct(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		long	ServerIndex, 
		DWORD	ReceiverAccountSeq, 
		char	ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		long	ReceiverServerIndex, 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		long	PackageProductSeq, 
		long	PriceSeq, 
		long	SalesZone, 
		long	ProductDisplaySeq, 
		DWORD	dwIPAddress, 
		char	RefKey[MAX_TYPENAME_LENGTH],
		bool	DeductMileageFlag,
		WCHAR	strMemberNo[MAX_CHANNELING_LENGTH],
		DWORD	AccessIP);

// 공통 서비스
	
	// 상품 샵 스크립트 최신 버전 정보 조회 : 모든 게임
	bool InquireSalesZoneScriptVersion(
		long	GameCode,
		long	SalesZone);
	
	// 배너 스크립트 최신 버전 정보 조회 : 모든 게임
	bool InquireBannerZoneScriptVersion(
		long	GameCode,
		long	BannerZone);

	// [deprecated] 캐시선물 : 모든 게임, [AccessIP 추가 버전] 사용 권장
	bool GiftCash(
		DWORD	SenderAccountSeq, 
		long	SendServerIndex, 
		WCHAR	SendCharName[MAX_CHARACTERID_LENGTH], 
		DWORD	ReceiverAccountSeq, 
		long	ReceiverServerIndex, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		double	CashValue, 
		long	GameCode);

	// 캐시선물 : 모든 게임, [AccessIP 추가 버전]
	bool GiftCash(
		DWORD	SenderAccountSeq, 
		long	SendServerIndex, 
		WCHAR	SendCharName[MAX_CHARACTERID_LENGTH], 
		DWORD	ReceiverAccountSeq, 
		long	ReceiverServerIndex, 
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Message[MAX_MESSAGE_LENGTH], 
		double	CashValue, 
		long	GameCode,
		DWORD	AccessIP);	// <----- AccessIP 추가 버전

	
	// 상품 구매/선물 가능 여부 조회 : 모든 게임
	bool InquireBuyGiftPossibility(
		DWORD	AccountSeq,
		long	GameCode);	
	
	// 이벤트 상품 목록 조회 : 모든 게임
	bool InquireEventProductList(
		long	GameCode,
		DWORD	AccountSeq,
		long	SalesZone,
		long	ProductDisplaySeq);

	// 전시 상품 잔여 개수 조회 : 모든 게임
	bool InquireProductLeftCount(
		long	GameCode, 
		long	PackageSeq);

	// [deprecated] 보관함 리스트 조회 : 모든 게임, InquireStorageListEx() 사용 권장
	bool InquireStorageList(
		DWORD	AccountSeq,
		long	GameCode,
		long	SalesZone);

	// [deprecated] 보관함 리스트 페이지 조회 : 모든 게임, InquireStorageListPageEx() 사용 권장
	bool InquireStorageListPage(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage, 
		int		PageSize);

	// [deprecated] 보관함 리스트 페이지 조회(선물 메시지 제외) : 모든 게임, InquireStorageListPageNoGiftMessageEx() 사용 권장
	bool InquireStorageListPageNoGiftMessage(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage,
		int		PageSize);

	// 보관함 리스트를 조회(만료시점 추가) : 모든 게임
	bool InquireStorageListEx(
		DWORD	AccountSeq,
		long	GameCode,
		long	SalesZone);

	// 보관함 리스트 페이지를 조회(만료시점 추가) : 모든 게임
	bool InquireStorageListPageEx(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage,
		int		PageSize);

	// 보관함 리스트 페이지를 조회한다(선물 메시지 제외, 만료시점 추가) : 모든 게임
	bool InquireStorageListPageNoGiftMessageEx(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage,
		int		PageSize);

	// 보관함 상품 사용 : 모든 게임
	bool UseStorage(
		DWORD	AccountSeq, 
		long	GameCode, 
		DWORD	IPAddress, 
		long	StorageSeq, 
		long	StorageItemSeq, 
		char	StorageItemType, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);

	// 보관함 상품 사용 EX : SUN_GB
	//	결과로 CashType을 추가 리턴함
	bool UseStorageEx(
		DWORD	AccountSeq, 
		long	GameCode, 
		DWORD	IPAddress, 
		long	StorageSeq, 
		long	StorageItemSeq, 
		char	StorageItemType, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);


	// 보관함 상품 사용 EX : SUN_CN
	//	결과로 DeductMileageFlag를 추가 리턴함. (false : 캐시상품(0), true : 마일리지 상품(1))
	bool UseStorageExDeductMileageFlag(
		DWORD	AccountSeq, 
		long	GameCode, 
		DWORD	IPAddress, 
		long	StorageSeq, 
		long	StorageItemSeq, 
		char	StorageItemType, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);


	// 보관함 상품 사용하기 롤백 : 모든 게임
	bool RollbackUseStorage(
		DWORD	AccountSeq,
		long	GameCode,
		long	StorageSeq,
		long	StorageItemSeq);

	// 보관함 상품 버림 : 모든 게임
	bool ThrowStorage(
		DWORD	AccountSeq,
		long	GameCode,
		long	StorageSeq,
		long	StorageItemSeq, 
		char	StorageItemType);

	// 마일리지 소진 : 모든 게임
	bool MileageDeduct(
		DWORD	AccountSeq, 
		long	GameCode, 
		long	DeductCategory, 
		long	MileageSection, 
		long	MileageDeductPoint, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);

	// 마일리지 적립 : 모든 게임
	bool MileageSave(
		DWORD	AccountSeq, 
		long	GameCode, 
		long	MileageType, 
		long	MileageSection, 
		long	MileagePoint, 
		long	Class, 
		long	Level, 
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);

	// 실시간 마일리지 적립 : 모든 게임
	bool MileageLiveSaveUp(
		DWORD	AccountSeq,
		long	GameCode,
		long	MileageSection,
		long	SourceType);

	// 아이템 시리얼 코드 업데이트 : 모든 게임
	bool ItemSerialUpdate(
		DWORD	AccountSeq,
		long	GameCode,
		long	StorageSeq,
		long	StorageItemSeq,
		INT64	InGameUseCode);
	
	// 인게임 포인트 유형별 지급 퍼센트 조회 : 모든 게임
	bool InquireInGamePointValue(
		long	GameCode,
		long	ServerType,
		long	AccessType);


protected:
	// [do not override] - 로깅후 적절한 응답처리 함수(On...()) 호출
	void OnReceive(PBYTE buffer, int size);

	// [do not override] - 로깅만 하고 OnNetConnect() 호출
	void OnConnect(bool success, DWORD error);

	// [do not override] - 로깅만 하고 OnNetSend() 호출
	void OnSend(int size);

	// [do not override] - 로깅만 하고 OnNetClose() 호출
	void OnClose(DWORD error);


protected:	
	// 샵 서버 연결한 결과
	virtual void OnNetConnect(bool success, DWORD error) = 0;

	// 샵 서버 데이타 전송 결과
	virtual void OnNetSend(int size) = 0;
	
	// 샵 서버 접속 종료
	virtual void OnNetClose(DWORD error) = 0;	

	// 로깅시 보내는 이벤트 메소드
	virtual void WriteLog(char* szMsg) = 0;

// 캐시조회 결과
	// 캐시조회 결과 : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		double	CashSum,
		double	PointSum, 
		double	MileageSum,
		int		DetailCount,
		ShopProtocol::STCashDetail	Detail[], 
		long	ResultCode);

	// 캐시조회 결과 : MU_GB, SUN_GB
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		double	CashSum,
		double	PointSum,
		double	MileageSum, 
		int		ListCount,
		ShopProtocol::STCashDetail_GB	Detail[],
		long	ResultCode);

	// 캐시조회 결과 : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		long	ResultCode,
		long	OutBoundResultCode,
		double	CashSum,
		double	MileageSum);

	// 캐지조회 결과 : BT_CNL
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		char	AccountID[MAX_ACCOUNTID_LENGTH], 
		double	CashSum,
		double	PointSum,
		double	MileageSum, 
		int		DetailCount,
		ShopProtocol::STCashDetail	Detail[], 
		long	ResultCode,
		long	OutBoundResultCode);

// 상품구매 결과
	// 상품구매 결과 : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnBuyProduct(
		DWORD	AccountSeq,
		long	ResultCode,
		long	LeftProductCount);

	// 상품구매 결과 : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
	virtual void OnBuyProduct(
		DWORD	AccountSeq,
		long	ResultCode,
		long	OutBoundResultCode,
		long	LeftProductCount);

	// 상품구매 결과 : BT_CNL
	virtual void OnBuyProduct(
		DWORD	AccountSeq,
		char	AccountID[MAX_ACCOUNTID_LENGTH],
		long	LeftProductCount,
		long	ResultCode,
		long	OutBoundResultCode);

// 상품선물 결과
	// 상품선물 결과 : MU_KR, BT_KR, R2_KR, SUN_KR, MU_GB, SUN_GB
	virtual void OnGiftProduct(
		DWORD	SenderAccountSeq,
		DWORD	ReceiverAccountSeq,
		double	GiftSendLimit,
		long	LeftProductCount,
		long	ResultCode);

	// 상품선물 결과 : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
	virtual void OnGiftProduct(
		DWORD	SenderAccountSeq,
		long	ResultCode,
		long	OutBoundResultCode,
		DWORD	ReceiverAccountSeq,
		long	LeftProductCount);

	// 상품선물 결과 : BT_CNL
	virtual void OnGiftProduct(
		DWORD	SenderAccountSeq,
		char	SenderAccountID[MAX_ACCOUNTID_LENGTH],
		WCHAR	SenderCharName[MAX_CHARACTERID_LENGTH], 
		DWORD	ReceiverAccountSeq,
		char	ReceiverAccountID[MAX_ACCOUNTID_LENGTH],
		WCHAR	ReceiverCharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Message[MAX_MESSAGE_LENGTH],
		double	LimitedCash,
		long	LeftProductCount, 
		long	ResultCode,
		long	OutBoundResultCode);


// 공통 서비스
	// 상품 샵 스크립트 최신 버전 정보 조회 : 모든 게임
	virtual void OnInquireSalesZoneScriptVersion(
		unsigned short	SalesZone,
		unsigned short	Year,
		unsigned short	YearIdentity,
		long			ResultCode) = 0;

	// 배너 스크립트 최신 버전 정보 조회 : 모든 게임
	virtual void OnInquireBannerZoneScriptVersion(
		unsigned short	BannerZone,
		unsigned short	Year,
		unsigned short	YearIdentity,
		long			ResultCode) = 0;

	// 캐시 선물 결과 : 모든 게임
	virtual void OnGiftCash(
		DWORD	SenderSeq, 
		DWORD	ReceiverSeq,
		long	ResultCode,
		double	GiftSendLimit) = 0;

	// 상품 구매/선물 가능 조회 결과 : 모든 게임
	virtual void OnInquireBuyGiftPossibility(
		DWORD	AccountID,
		long	ResultCode,
		long	ItemBuy,
		long	Present,
		double	MyPresentCash,
		double	MyPresentSendLimit) = 0;	

	// 이벤트 상품 리스트 조회 결과 : 모든 게임
	virtual void OnInquireEventProductList(
		DWORD	AccountSeq,
		long	ProductDisplaySeq,
		int		PackagesCount,
		long	Packages[]) = 0;

	// 전시 상품 잔여 개수 조회 결과 : 모든 게임
	virtual void OnInquireProductLeftCount(
		long	PackageSeq,
		long	LeftCount) = 0;

	// [deprecated] 보관함 조회 결과 : 모든 게임
	virtual void OnInquireStorageList(
		DWORD	AccountID,
		long	ListCount,
		long	ResultCode,
		ShopProtocol::STStorage	StorageList[]);

	// [deprecated] 보관함 페이지 조회 결과 : 모든 게임
	virtual void OnInquireStorageListPage(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorage	StorageList[]) = 0;

	// [deprecated] 보관함 페이지 조회 결과(선물 메시지 제외) : 모든 게임
	virtual void OnInquireStorageListPageNoGiftMessage(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType, 
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorageNoGiftMessage	StorageList[]);

	// 보관함 조회 결과(만료시점 추가) : 모든 게임
	virtual void OnInquireStorageListEx(
		DWORD	AccountID,
		long	ListCount,
		long	ResultCode,
		ShopProtocol::STStorageEx	StorageListEx[]);

	// 보관함 페이지 조회 결과(만료시점 추가) : 모든 게임
	virtual void OnInquireStorageListPageEx(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorageEx	StorageListEx[]);

	// 보관함 페이지 조회 결과(선물 메시지 제외,만료시점 추가) : 모든 게임
	virtual void OnInquireStorageListPageNoGiftMessageEx(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorageNoGiftMessageEx StorageListEx[]);

	// 보관함 상품 사용하기 결과 : 모든 게임
	virtual void OnUseStorage(
		DWORD	AccountSeq,
		long	ResultCode,
		char	InGameProductID[MAX_TYPENAME_LENGTH],
		BYTE	PropertyCount,
		long	ProductSeq,
		long	StorageSeq,
		long	StorageItemSeq,
		ShopProtocol::STItemProperty PropertyList[]);

	// 보관함 상품 사용하기 결과 Ex : SUN_GB
	//	결과값 CashType 추가
	virtual void OnUseStorageEx(
		DWORD	AccountSeq,
		long	ResultCode,
		char	InGameProductID[MAX_TYPENAME_LENGTH],
		BYTE	PropertyCount,
		long	ProductSeq,
		long	StorageSeq,
		long	StorageItemSeq,
		ShopProtocol::STItemProperty PropertyList[],
		int		CashType);


	// 보관함 상품 사용하기 결과 Ex : SUN_CN
	//	DeductMileageFlag 추가 (false : 캐시상품(0), true : 마일리지 상품(1))
	virtual void OnUseStorageExDeductMileageFlag(
		DWORD	AccountSeq,
		long	ResultCode,
		char	InGameProductID[MAX_TYPENAME_LENGTH],
		BYTE	PropertyCount,
		long	ProductSeq,
		long	StorageSeq,
		long	StorageItemSeq,
		ShopProtocol::STItemProperty PropertyList[],
		bool	DeductMileageFlag);
	

	// 보관함 상품 사용하기 롤백 : 모든 게임
	virtual void OnRollbackUseStorage(
		DWORD	AccountSeq,
		long	ResultCode) = 0;

	// 보관함 상품 버리기 결과 : 모든 게임
	virtual void OnThrowStorage(
		DWORD	AccountSeq,
		long	ResultCode) = 0;

	// 마일리지 소진 : 모든 게임
	virtual void OnMileageDeduct(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// 마일리지 적립 : 모든 게임
	virtual void OnMileageSave(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// 실시간 마일리지 적립 : 모든 게임
	virtual void OnMileageLiveSaveUp(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// 아이템 시리얼 코드 업데이트 : 모든 게임
	virtual void OnItemSerialUpdate(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// 샵 스크립트 버전 정보 업데이트 : 모든 게임
	virtual void OnUpdateVersion(
		long			GameCode,
		unsigned short	SalesZone,
		unsigned short	year,
		unsigned short	yearIdentity) = 0;

	// 배너 스크립트 버전 정보 업데이트 : 모든 게임
	virtual void OnUpdateBannerVersion(
		long			GameCode,
		unsigned short	BannerZone,
		unsigned short	year, 
		unsigned short	yearIdentity) = 0;

	// 인게임 포인트 유형별 지급 퍼센트 조회 : HX 전용
	virtual void OnInquireInGamePointValue(
		long	ResultCode,
		long	PointCount,
		ShopProtocol::STPointDetail PointList[]);

private:
	// 로그 사용
	void WriteLog(const char* szFormat, ...);
};
