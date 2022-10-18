#pragma once

// ������ ���� ����

#include <ImportLibrary.h>		// �ʿ��� ClientStub ���̺귯�� import
#include <Winsock2.h>
#include <WBANetwork/WBANetwork.h>
#include "ShopEventHandler.h"

class CShopEH : public CShopEventHandler
{
public:
	CShopEH(void);
	virtual  ~CShopEH(void);

protected:
	// ������ �α� �ۼ� �̺�Ʈ : �Լ����ο��� �α׸� ����� �ֵ��� �ؾ� ��.
	void WriteLog(char* szMsg);

	// �������� ���� ���� ����
	//		success	: ���� ���� ����
	//		error	: �����ڵ�
	void OnNetConnect(bool success, DWORD error);
	
	// �������� ���� ����Ÿ ���� ���� 
	//		size : ���۵� ��Ŷ ������
	void OnNetSend(int size);
	
	// �������� ������ �������� (�������� ���������� ó�� �ؾ��Ѵ�.)	
	//		error: �����ڵ�
	void OnNetClose( DWORD error );	


protected:
	// ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	virtual void OnInquireSalesZoneScriptVersion(
		unsigned short	SalesZone, 
		unsigned short	Year, 
		unsigned short	YearIdentity, 
		long			ResultCode);

	// ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
	virtual void OnInquireBannerZoneScriptVersion(
		unsigned short	BannerZone, 
		unsigned short	Year, 
		unsigned short	YearIdentity, 
		long			ResultCode);

	// ĳ����ȸ ��� : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnInquireCash(
		DWORD	AccountSeq, 
		double	CashSum, 
		double	PointSum, 
		double	MileageSum, 
		int		DetailCount, 
		ShopProtocol::STCashDetail	Detail[], 
		long	ResultCode);

	// ĳ����ȸ ��� : MU_GB
	virtual void OnInquireCash(
		DWORD	AccountSeq, 
		double	CashSum, 
		double	PointSum, 
		double	MileageSum, 
		int		ListCount, 
		ShopProtocol::STCashDetail_GB	Detail[], 
		long	ResultCode);

	// ĳ����ȸ ��� : SUN_RU, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
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

	// ��ǰ���� ��� : MU_KR, BT_KR, R2_KR, SUN_KR
	virtual void OnBuyProduct(
		DWORD	AccountSeq, 
		long	ResultCode, 
		long	LeftProductCount);

	// ��ǰ���� ��� : SUN_RU, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
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

	// ��ǰ���� ��� : MU_KR, BT_KR, R2_KR, SUN_KR, MU_GB
	virtual void OnGiftProduct(
		DWORD	SenderSeq, 
		DWORD	ReceiverSeq, 
		double	LimitedCash, 
		long	LeftProductCount, 
		long	ResultCode);

	// ��ǰ���� ��� : SUN_RU, SUN_JP, MU_JP(GameChu, GameOn), MU_TH, MU_TW, MU_VN
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

	// ĳ�� ���� ��� : ��� ����
	virtual void OnGiftCash(
		DWORD	SenderSeq, 
		DWORD	ReceiverSeq, 
		long	ResultCode, 
		double	GiftSendLimit);

	// ��ǰ ����/���� ���� ��ȸ ��� : ��� ����
	virtual void OnInquireBuyGiftPossibility(
		DWORD	AccountID, 
		long	ResultCode, 
		long	ItemBuy, 
		long	Present, 
		double	MyPresentCash, 
		double	MyPresentSendLimit);

	// �̺�Ʈ ��ǰ ����Ʈ ��ȸ ��� : ��� ����
	virtual void OnInquireEventProductList(
		DWORD	AccountSeq, 
		long	ProductDisplaySeq, 
		int		PackagesCount, 
		long	Packages[]);

	// ���� ��ǰ �ܿ� ���� ��ȸ ��� : ��� ����
	virtual void OnInquireProductLeftCount(
		long	PackageSeq,
		long	LeftCount);

	// ������ ��ȸ ��� : ��� ����
	virtual void OnInquireStorageList(
		DWORD	AccountID, 
		long	ListCount, 
		long	ResultCode, 
		ShopProtocol::STStorage	StorageList[]);

	// ������ ������ ��ȸ ���
	virtual void OnInquireStorageListPage(
		DWORD	AccountID,
		int		ResultCode,
		char	StorageType,
		int		NowPage,
		int		TotalPage,
		int		TotalCount,
		int		ListCount,
		ShopProtocol::STStorage	StorageList[]);

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
		long	ResultCode);

	// ������ ��ǰ ������ ��� : ��� ����
	virtual void OnThrowStorage(
		DWORD	AccountSeq,
		long	ResultCode);

	// ���ϸ��� ���� : ��� ����
	virtual void OnMileageDeduct(
		DWORD	AccountSeq,
		long	ResultCode);
	
	// ���ϸ��� ���� : ��� ����
	virtual void OnMileageSave(
		DWORD	AccountSeq,
		long	ResultCode);

	// �ǽð� ���ϸ��� ���� : ��� ����
	virtual void OnMileageLiveSaveUp(
		DWORD	AccountSeq,
		long	ResultCode);
	
	// ������ �ø��� �ڵ� ������Ʈ : ��� ����
	virtual void OnItemSerialUpdate(
		DWORD	AccountSeq,
		long	ResultCode);
	
	// �� ��ũ��Ʈ ���� ���� ������Ʈ : ��� ����
	virtual void OnUpdateVersion(
		long			GameCode,
		unsigned short	SalesZone,
		unsigned short	year,
		unsigned short	yearIdentity);

	// ��� ��ũ��Ʈ ���� ���� ������Ʈ : ��� ����
	virtual void OnUpdateBannerVersion(
		long			GameCode,
		unsigned short	BannerZone,
		unsigned short	year, 
		unsigned short	yearIdentity);

	// �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ : HX ����
	virtual void OnInquireInGamePointValue(
		long	ResultCode,
		long	PointCount,
		ShopProtocol::STPointDetail PointList[]);

private:
	CRITICAL_SECTION m_cs;		// �α��� ���� lock
	CString ConvertTime(DWORD dwTime);
};
