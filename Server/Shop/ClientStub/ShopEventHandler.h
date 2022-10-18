#pragma once

/**************************************************************************************************
�ۼ���: 2008-07-08
����: �� ������ ����ϴ� Ŭ���̾�Ʈ���� ���ŵ� ��Ŷ�� ������ �ֱ� ���� �̺�Ʈ ��ü 
      �̺�Ʈ�� �޴���(��Ŭ���̾�Ʈ)���� ��ӹ޾� ����ؾ� �Ѵ�.
**************************************************************************************************/

#include "ClientSession.h"
#include <public/protocol_shop.h>

class CShopEventHandler : public CClientSession
{
public:
	CShopEventHandler(void);
	virtual  ~CShopEventHandler(void);

	// �ʱ�ȭ �Լ� (���α׷� �����ϰ� �ѹ��� ����ؾ� �Ѵ�.)
	bool CreateSession(WBANetwork::LPFN_ErrorHandler errhandler = NULL);


// ĳ����ȸ
	// ĳ����ȸ : MU_KR, BT_KR, R2_KR, SUN_KR
	bool InquireCash(
		long	GameCode,
		BYTE	ViewType, 
		DWORD	AccountSeq, 
		bool	SumOnly, 
		long	MileageSection);

	// ĳ����ȸ : MU_GB, SUN_GB
	bool InquireCash(
		BYTE	ViewType, 
		DWORD	AccountSeq, 
		long	GameCode,
		bool	SumOnly, 
		long	MileageSection);

	// ĳ����ȸ : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_VN, MU_TW
	bool InquireCash(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		long	AccountSeq, 
		int		USN, 
		BYTE	ViewType,
		long	MileageSection,
		bool	SumOnly);

	// ĳ����ȸ : OB
	bool InquireCash(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH],
		BYTE	ViewType,
		long	MileageSection,
		bool	SumOnly);

	// ĳ����ȸ : BT_CNL
	bool InquireCash(
		long	GameCode,
		ShopProtocol::PaymentType	PaymentType,
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNTID_LENGTH],
		BYTE	ViewType,
		long	MileageSection,
		bool	SumOnly,
		WCHAR	strMemberNo[MAX_CHANNELING_LENGTH]);

// ��ǰ����
	// [deprecated] - [AccessIP �߰� ����] ��� ����
	// ��ǰ���� : MU_KR, BT_KR, R2_KR, SUN KR
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

	// [AccessIP �߰� ����]
	// ��ǰ���� : MU_KR, BT_KR, R2_KR, SUN KR
	bool BuyProduct(
		long	GameCode,
		DWORD	AccountSeq, 
		long	PackageProductSeq, 
		long	ProductDisplaySeq, 
		long	SalesZone, 
		long	PriceSeq, 
		long	Class, 
		long	Level,
		DWORD	AccessIP,						// <----- AccessIP �߰� ����
		WCHAR	CharName[MAX_CHARACTERID_LENGTH], 
		WCHAR	Rank[MAX_RANK_LENGTH], 
		long	ServerIndex);

	// [deprecated] - [DeductMileageFlag & AccessIP �߰� ����] ��� ����
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

	// [deprecated] - [DeductMileageFlag & AccessIP �߰� ����] ��� ����
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
		bool	DeductMileageFlag);				// <----- DeductMileageFlag �߰�

	// [deprecated] - [DeductMileageFlag & AccessIP �߰� ����] ��� ����
	// ��ǰ���� : MU_GB, SUN_GB
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
		bool	DeductMileageFlag,				// <----- DeductMileageFlag �߰�
		DWORD	AccessIP);

	// ��ǰ���� : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_VN, MU_TW
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

	// ��ǰ���� : MU_TH
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

	// [deprecated] - [AccessIP �߰� ����] ��� ����
	// ��ǰ���� : BT_CNL
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


	// [AccessIP �߰� ����]
	// ��ǰ���� : BT_CNL
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
		DWORD	AccessIP);						// <----- AccessIP �߰� ����

// ��ǰ����
	// [deprecated] - [AccessIP �߰� ����] ��� ����
	// ��ǰ ���� : MU_KR, BT_KR, R2_KR, SUN_KR
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

	// [AccessIP �߰� ����]
	// ��ǰ���� : MU_KR, BT_KR, R2_KR, SUN_KR
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
		DWORD	AccessIP);					// <----- AccessIP �߰� ����

	// [deprecated] - [DeductMileageFlag & AccessIP �߰� ����] ��� ����
	// ��ǰ���� : MU_GB
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

	// [deprecated] - [DeductMileageFlag & AccessIP �߰� ����] ��� ����
	// ��ǰ���� : MU_GB, SUN_GB
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
		bool	DeductMileageFlag);					// <----- DeductMileageFlag �߰�

	// [DeductMileageFlag & AccessIP �߰� ����]
	// ��ǰ���� : MU_GB, SUN_GB
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
		bool	DeductMileageFlag,					// <----- DeductMileageFlag �߰�
		DWORD	AccessIP);							// <----- AccessIP �߰�(client ip)

	// ��ǰ���� : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_VN, MU_TW
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

	// ��ǰ���� : MU_TH
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

	// [deprecated] - [AccessIP �߰� ����] ��� ����
	// ��ǰ���� : BT_CNL
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

	// [AccessIP �߰� ����]
	// ��ǰ���� : BT_CNL
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

// ���� ����
	
	// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ : ��� ����
	bool InquireSalesZoneScriptVersion(
		long	GameCode,
		long	SalesZone);
	
	// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ : ��� ����
	bool InquireBannerZoneScriptVersion(
		long	GameCode,
		long	BannerZone);

	// [deprecated] ĳ�ü��� : ��� ����, [AccessIP �߰� ����] ��� ����
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

	// ĳ�ü��� : ��� ����, [AccessIP �߰� ����]
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
		DWORD	AccessIP);	// <----- AccessIP �߰� ����

	
	// ��ǰ ����/���� ���� ���� ��ȸ : ��� ����
	bool InquireBuyGiftPossibility(
		DWORD	AccountSeq,
		long	GameCode);	
	
	// �̺�Ʈ ��ǰ ��� ��ȸ : ��� ����
	bool InquireEventProductList(
		long	GameCode,
		DWORD	AccountSeq,
		long	SalesZone,
		long	ProductDisplaySeq);

	// ���� ��ǰ �ܿ� ���� ��ȸ : ��� ����
	bool InquireProductLeftCount(
		long	GameCode, 
		long	PackageSeq);

	// [deprecated] ������ ����Ʈ ��ȸ : ��� ����, InquireStorageListEx() ��� ����
	bool InquireStorageList(
		DWORD	AccountSeq,
		long	GameCode,
		long	SalesZone);

	// [deprecated] ������ ����Ʈ ������ ��ȸ : ��� ����, InquireStorageListPageEx() ��� ����
	bool InquireStorageListPage(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage, 
		int		PageSize);

	// [deprecated] ������ ����Ʈ ������ ��ȸ(���� �޽��� ����) : ��� ����, InquireStorageListPageNoGiftMessageEx() ��� ����
	bool InquireStorageListPageNoGiftMessage(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage,
		int		PageSize);

	// ������ ����Ʈ�� ��ȸ(������� �߰�) : ��� ����
	bool InquireStorageListEx(
		DWORD	AccountSeq,
		long	GameCode,
		long	SalesZone);

	// ������ ����Ʈ �������� ��ȸ(������� �߰�) : ��� ����
	bool InquireStorageListPageEx(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage,
		int		PageSize);

	// ������ ����Ʈ �������� ��ȸ�Ѵ�(���� �޽��� ����, ������� �߰�) : ��� ����
	bool InquireStorageListPageNoGiftMessageEx(
		DWORD	AccountSeq,
		int		GameCode,
		int		SalesZone,
		char	StorageType,
		int		NowPage,
		int		PageSize);

	// ������ ��ǰ ��� : ��� ����
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

	// ������ ��ǰ ��� EX : SUN_GB
	//	����� CashType�� �߰� ������
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


	// ������ ��ǰ ��� EX : SUN_CN
	//	����� DeductMileageFlag�� �߰� ������. (false : ĳ�û�ǰ(0), true : ���ϸ��� ��ǰ(1))
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


	// ������ ��ǰ ����ϱ� �ѹ� : ��� ����
	bool RollbackUseStorage(
		DWORD	AccountSeq,
		long	GameCode,
		long	StorageSeq,
		long	StorageItemSeq);

	// ������ ��ǰ ���� : ��� ����
	bool ThrowStorage(
		DWORD	AccountSeq,
		long	GameCode,
		long	StorageSeq,
		long	StorageItemSeq, 
		char	StorageItemType);

	// ���ϸ��� ���� : ��� ����
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

	// ���ϸ��� ���� : ��� ����
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

	// �ǽð� ���ϸ��� ���� : ��� ����
	bool MileageLiveSaveUp(
		DWORD	AccountSeq,
		long	GameCode,
		long	MileageSection,
		long	SourceType);

	// ������ �ø��� �ڵ� ������Ʈ : ��� ����
	bool ItemSerialUpdate(
		DWORD	AccountSeq,
		long	GameCode,
		long	StorageSeq,
		long	StorageItemSeq,
		INT64	InGameUseCode);
	
	// �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ : ��� ����
	bool InquireInGamePointValue(
		long	GameCode,
		long	ServerType,
		long	AccessType);


protected:
	// [do not override] - �α��� ������ ����ó�� �Լ�(On...()) ȣ��
	void OnReceive(PBYTE buffer, int size);

	// [do not override] - �α븸 �ϰ� OnNetConnect() ȣ��
	void OnConnect(bool success, DWORD error);

	// [do not override] - �α븸 �ϰ� OnNetSend() ȣ��
	void OnSend(int size);

	// [do not override] - �α븸 �ϰ� OnNetClose() ȣ��
	void OnClose(DWORD error);


protected:	
	// �� ���� ������ ���
	virtual void OnNetConnect(bool success, DWORD error) = 0;

	// �� ���� ����Ÿ ���� ���
	virtual void OnNetSend(int size) = 0;
	
	// �� ���� ���� ����
	virtual void OnNetClose(DWORD error) = 0;	

	// �α�� ������ �̺�Ʈ �޼ҵ�
	virtual void WriteLog(char* szMsg) = 0;

// ĳ����ȸ ���
	// ĳ����ȸ ��� : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		double	CashSum,
		double	PointSum, 
		double	MileageSum,
		int		DetailCount,
		ShopProtocol::STCashDetail	Detail[], 
		long	ResultCode);

	// ĳ����ȸ ��� : MU_GB, SUN_GB
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		double	CashSum,
		double	PointSum,
		double	MileageSum, 
		int		ListCount,
		ShopProtocol::STCashDetail_GB	Detail[],
		long	ResultCode);

	// ĳ����ȸ ��� : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
	virtual void OnInquireCash(
		DWORD	AccountSeq,
		long	ResultCode,
		long	OutBoundResultCode,
		double	CashSum,
		double	MileageSum);

	// ĳ����ȸ ��� : BT_CNL
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

// ��ǰ���� ���
	// ��ǰ���� ��� : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnBuyProduct(
		DWORD	AccountSeq,
		long	ResultCode,
		long	LeftProductCount);

	// ��ǰ���� ��� : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
	virtual void OnBuyProduct(
		DWORD	AccountSeq,
		long	ResultCode,
		long	OutBoundResultCode,
		long	LeftProductCount);

	// ��ǰ���� ��� : BT_CNL
	virtual void OnBuyProduct(
		DWORD	AccountSeq,
		char	AccountID[MAX_ACCOUNTID_LENGTH],
		long	LeftProductCount,
		long	ResultCode,
		long	OutBoundResultCode);

// ��ǰ���� ���
	// ��ǰ���� ��� : MU_KR, BT_KR, R2_KR, SUN_KR, MU_GB, SUN_GB
	virtual void OnGiftProduct(
		DWORD	SenderAccountSeq,
		DWORD	ReceiverAccountSeq,
		double	GiftSendLimit,
		long	LeftProductCount,
		long	ResultCode);

	// ��ǰ���� ��� : SUN_CN, SUN_RU, BT_TH, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
	virtual void OnGiftProduct(
		DWORD	SenderAccountSeq,
		long	ResultCode,
		long	OutBoundResultCode,
		DWORD	ReceiverAccountSeq,
		long	LeftProductCount);

	// ��ǰ���� ��� : BT_CNL
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


// ���� ����
	// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ : ��� ����
	virtual void OnInquireSalesZoneScriptVersion(
		unsigned short	SalesZone,
		unsigned short	Year,
		unsigned short	YearIdentity,
		long			ResultCode) = 0;

	// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ : ��� ����
	virtual void OnInquireBannerZoneScriptVersion(
		unsigned short	BannerZone,
		unsigned short	Year,
		unsigned short	YearIdentity,
		long			ResultCode) = 0;

	// ĳ�� ���� ��� : ��� ����
	virtual void OnGiftCash(
		DWORD	SenderSeq, 
		DWORD	ReceiverSeq,
		long	ResultCode,
		double	GiftSendLimit) = 0;

	// ��ǰ ����/���� ���� ��ȸ ��� : ��� ����
	virtual void OnInquireBuyGiftPossibility(
		DWORD	AccountID,
		long	ResultCode,
		long	ItemBuy,
		long	Present,
		double	MyPresentCash,
		double	MyPresentSendLimit) = 0;	

	// �̺�Ʈ ��ǰ ����Ʈ ��ȸ ��� : ��� ����
	virtual void OnInquireEventProductList(
		DWORD	AccountSeq,
		long	ProductDisplaySeq,
		int		PackagesCount,
		long	Packages[]) = 0;

	// ���� ��ǰ �ܿ� ���� ��ȸ ��� : ��� ����
	virtual void OnInquireProductLeftCount(
		long	PackageSeq,
		long	LeftCount) = 0;

	// [deprecated] ������ ��ȸ ��� : ��� ����
	virtual void OnInquireStorageList(
		DWORD	AccountID,
		long	ListCount,
		long	ResultCode,
		ShopProtocol::STStorage	StorageList[]);

	// [deprecated] ������ ������ ��ȸ ��� : ��� ����
	virtual void OnInquireStorageListPage(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorage	StorageList[]) = 0;

	// [deprecated] ������ ������ ��ȸ ���(���� �޽��� ����) : ��� ����
	virtual void OnInquireStorageListPageNoGiftMessage(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType, 
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorageNoGiftMessage	StorageList[]);

	// ������ ��ȸ ���(������� �߰�) : ��� ����
	virtual void OnInquireStorageListEx(
		DWORD	AccountID,
		long	ListCount,
		long	ResultCode,
		ShopProtocol::STStorageEx	StorageListEx[]);

	// ������ ������ ��ȸ ���(������� �߰�) : ��� ����
	virtual void OnInquireStorageListPageEx(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorageEx	StorageListEx[]);

	// ������ ������ ��ȸ ���(���� �޽��� ����,������� �߰�) : ��� ����
	virtual void OnInquireStorageListPageNoGiftMessageEx(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorageNoGiftMessageEx StorageListEx[]);

	// ������ ��ǰ ����ϱ� ��� : ��� ����
	virtual void OnUseStorage(
		DWORD	AccountSeq,
		long	ResultCode,
		char	InGameProductID[MAX_TYPENAME_LENGTH],
		BYTE	PropertyCount,
		long	ProductSeq,
		long	StorageSeq,
		long	StorageItemSeq,
		ShopProtocol::STItemProperty PropertyList[]);

	// ������ ��ǰ ����ϱ� ��� Ex : SUN_GB
	//	����� CashType �߰�
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


	// ������ ��ǰ ����ϱ� ��� Ex : SUN_CN
	//	DeductMileageFlag �߰� (false : ĳ�û�ǰ(0), true : ���ϸ��� ��ǰ(1))
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
	

	// ������ ��ǰ ����ϱ� �ѹ� : ��� ����
	virtual void OnRollbackUseStorage(
		DWORD	AccountSeq,
		long	ResultCode) = 0;

	// ������ ��ǰ ������ ��� : ��� ����
	virtual void OnThrowStorage(
		DWORD	AccountSeq,
		long	ResultCode) = 0;

	// ���ϸ��� ���� : ��� ����
	virtual void OnMileageDeduct(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// ���ϸ��� ���� : ��� ����
	virtual void OnMileageSave(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// �ǽð� ���ϸ��� ���� : ��� ����
	virtual void OnMileageLiveSaveUp(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// ������ �ø��� �ڵ� ������Ʈ : ��� ����
	virtual void OnItemSerialUpdate(
		DWORD	AccountSeq,
		long	ResultCode) = 0;
	
	// �� ��ũ��Ʈ ���� ���� ������Ʈ : ��� ����
	virtual void OnUpdateVersion(
		long			GameCode,
		unsigned short	SalesZone,
		unsigned short	year,
		unsigned short	yearIdentity) = 0;

	// ��� ��ũ��Ʈ ���� ���� ������Ʈ : ��� ����
	virtual void OnUpdateBannerVersion(
		long			GameCode,
		unsigned short	BannerZone,
		unsigned short	year, 
		unsigned short	yearIdentity) = 0;

	// �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ : HX ����
	virtual void OnInquireInGamePointValue(
		long	ResultCode,
		long	PointCount,
		ShopProtocol::STPointDetail PointList[]);

private:
	// �α� ���
	void WriteLog(const char* szFormat, ...);
};
