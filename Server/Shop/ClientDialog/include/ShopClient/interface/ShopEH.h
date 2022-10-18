#pragma once

// 샵서버 연동 샘플

#include <ImportLibrary.h>		// 필요한 ClientStub 라이브러리 import
#include <Winsock2.h>
#include <WBANetwork/WBANetwork.h>
#include "ShopEventHandler.h"

class CShopEH : public CShopEventHandler
{
public:
	CShopEH(void);
	virtual  ~CShopEH(void);

protected:
	// 샵서버 로그 작성 이벤트 : 함수내부에서 로그를 남길수 있도록 해야 함.
	void WriteLog(char* szMsg);

	// 샵서버의 연결 성공 여부
	//		success	: 연결 성공 여부
	//		error	: 에러코드
	void OnNetConnect(bool success, DWORD error);
	
	// 샵서버로 보낸 데이타 전송 여부 
	//		size : 전송된 패킷 사이즈
	void OnNetSend(int size);
	
	// 샵서버와 세션이 끊겼을때 (샵서버가 죽은것으로 처리 해야한다.)	
	//		error: 에러코드
	void OnNetClose( DWORD error );	


protected:
	// 상품 샵 스크립트 최신 버전 정보 조회
	virtual void OnInquireSalesZoneScriptVersion(
		unsigned short	SalesZone, 
		unsigned short	Year, 
		unsigned short	YearIdentity, 
		long			ResultCode);

	// 배너 스크립트 최신 버전 정보 조회
	virtual void OnInquireBannerZoneScriptVersion(
		unsigned short	BannerZone, 
		unsigned short	Year, 
		unsigned short	YearIdentity, 
		long			ResultCode);

	// 캐시조회 결과 : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnInquireCash(
		DWORD	AccountSeq, 
		double	CashSum, 
		double	PointSum, 
		double	MileageSum, 
		int		DetailCount, 
		ShopProtocol::STCashDetail	Detail[], 
		long	ResultCode);

	// 캐시조회 결과 : MU_GB
	virtual void OnInquireCash(
		DWORD	AccountSeq, 
		double	CashSum, 
		double	PointSum, 
		double	MileageSum, 
		int		ListCount, 
		ShopProtocol::STCashDetail_GB	Detail[], 
		long	ResultCode);

	// 캐시조회 결과 : SUN_RU, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
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

	// 상품구매 결과 : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnBuyProduct(
		DWORD	AccountSeq, 
		long	ResultCode, 
		long	LeftProductCount);

	// 상품구매 결과 : SUN_RU, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
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

	// 상품선물 결과 : MU_KR, BT_KR, R2_KR, SUN_KR, MU_GB
	virtual void OnGiftProduct(
		DWORD	SenderSeq, 
		DWORD	ReceiverSeq, 
		double	LimitedCash, 
		long	LeftProductCount, 
		long	ResultCode);

	// 상품선물 결과 : SUN_RU, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
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

	// 캐시 선물 결과 : 모든 게임
	virtual void OnGiftCash(
		DWORD	SenderSeq, 
		DWORD	ReceiverSeq, 
		long	ResultCode, 
		double	GiftSendLimit);

	// 상품 구매/선물 가능 조회 결과 : 모든 게임
	virtual void OnInquireBuyGiftPossibility(
		DWORD	AccountID, 
		long	ResultCode, 
		long	ItemBuy, 
		long	Present, 
		double	MyPresentCash, 
		double	MyPresentSendLimit);

	// 이벤트 상품 리스트 조회 결과 : 모든 게임
	virtual void OnInquireEventProductList(
		DWORD	AccountSeq, 
		long	ProductDisplaySeq, 
		int		PackagesCount, 
		long	Packages[]);

	// 전시 상품 잔여 개수 조회 결과 : 모든 게임
	virtual void OnInquireProductLeftCount(
		long	PackageSeq,
		long	LeftCount);

	// 보관함 조회 결과 : 모든 게임
	virtual void OnInquireStorageList(
		DWORD	AccountID, 
		long	ListCount, 
		long	ResultCode, 
		ShopProtocol::STStorage	StorageList[]);

	// 보관함 페이지 조회 결과
	virtual void OnInquireStorageListPage(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorage	StorageList[]);

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
		long	ResultCode);

	// 보관함 상품 버리기 결과 : 모든 게임
	virtual void OnThrowStorage(
		DWORD	AccountSeq,
		long	ResultCode);

	// 마일리지 소진 : 모든 게임
	virtual void OnMileageDeduct(
		DWORD	AccountSeq,
		long	ResultCode);
	
	// 마일리지 적립 : 모든 게임
	virtual void OnMileageSave(
		DWORD	AccountSeq,
		long	ResultCode);

	// 실시간 마일리지 적립 : 모든 게임
	virtual void OnMileageLiveSaveUp(
		DWORD	AccountSeq,
		long	ResultCode);
	
	// 아이템 시리얼 코드 업데이트 : 모든 게임
	virtual void OnItemSerialUpdate(
		DWORD	AccountSeq,
		long	ResultCode);
	
	// 샵 스크립트 버전 정보 업데이트 : 모든 게임
	virtual void OnUpdateVersion(
		long			GameCode,
		unsigned short	SalesZone,
		unsigned short	year,
		unsigned short	yearIdentity);

	// 배너 스크립트 버전 정보 업데이트 : 모든 게임
	virtual void OnUpdateBannerVersion(
		long			GameCode,
		unsigned short	BannerZone,
		unsigned short	year, 
		unsigned short	yearIdentity);

	// 인게임 포인트 유형별 지급 퍼센트 조회 : HX 전용
	virtual void OnInquireInGamePointValue(
		long	ResultCode,
		long	PointCount,
		ShopProtocol::STPointDetail PointList[]);

private:
	CRITICAL_SECTION m_cs;		// 로깅을 위한 lock
	CString ConvertTime(DWORD dwTime);
};
