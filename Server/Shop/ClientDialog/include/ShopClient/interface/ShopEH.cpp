
#include "stdafx.h"
#include "ShopEH.h"
#include "atlbase.h"

CShopEH::CShopEH(void)
{
	::InitializeCriticalSection(&m_cs);
}

CShopEH::~CShopEH(void)
{
	::DeleteCriticalSection(&m_cs);
}

void CShopEH::OnNetConnect(bool success, DWORD error)
{
	if(success) AfxMessageBox(_T("OnConnect succeed"));
	else AfxMessageBox(_T("OnConnect fali"));
}

void CShopEH::OnNetSend(int size)
{
	//AfxMessageBox("OnSend");
}

void CShopEH::OnNetClose(DWORD error)
{
	WriteLog("OnNetClose\r\n");
	AfxMessageBox(_T("OnClose"));
}

void CShopEH::OnInquireSalesZoneScriptVersion(
	unsigned short	SalesZone, 
	unsigned short	Year, 
	unsigned short	YearIdentity, 
	long			ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnInquireSalesZoneScriptVersion:%d.%d.%d, ResultCode:%d\r\n"), 
					SalesZone, Year, YearIdentity, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireBannerZoneScriptVersion(
	unsigned short	BannerZone, 
	unsigned short	Year, 
	unsigned short	YearIdentity, 
	long			ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnInquireBannerZoneScriptVersion:%d.%d.%d, ResultCode:%d\r\n"), 
					BannerZone, Year, YearIdentity, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireCash(
	DWORD	AccountSeq, 
	double	CashSum, 
	double	PointSum, 
	double	MileageSum, 
	int		DetailCount, 
	ShopProtocol::STCashDetail	Detail[], 
	long	ResultCode)
{
	CString strMsg, strDetail;
	strMsg.Format(_T("OnInquireCash\r\nAccountSeq:%d, CashSum:%f, PointSum:%f, MileageSum:%f, ResultCode:%d, \r\ndetail information:\r\n"), 
					AccountSeq, CashSum, PointSum, MileageSum, ResultCode);

	USES_CONVERSION;	
	for (int i=0 ; i<DetailCount; i++) {
		if (0 != Detail[i].Type) {
			strDetail.Format(_T("[Name:%s, Type:%c, Value:%f] \r\n"),
							W2A(Detail[i].Name), Detail[i].Type, Detail[i].Value);
			strMsg = strMsg + strDetail;
		}
	}
	strMsg += "\r\n";

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireCash(
	DWORD	AccountSeq, 
	double	CashSum, 
	double	PointSum, 
	double	MileageSum, 
	int		ListCount, 
	ShopProtocol::STCashDetail_GB	Detail[], 
	long	ResultCode)
{
	CString strMsg, strDetail;
	strMsg.Format(_T("OnInquireCash\r\nAccountSeq:%d, CashSum:%f, PointSum:%f, MileageSum:%f, ResultCode:%d, \r\ndetail information: \r\n"), 
					AccountSeq, CashSum, PointSum, MileageSum, ResultCode);

	USES_CONVERSION;	
	for (int i=0; i<ListCount; i++) {
		if (0 != Detail[i].Type) {
			strDetail.Format(_T("[Name:%s, Type:%c, Value:%f, CashTypeCode:%d] \r\n"), 
							W2A(Detail[i].Name), Detail[i].Type, Detail[i].Value, Detail[i].CashTypeCode);
			strMsg = strMsg + strDetail;
		}
	}
	strMsg += "\r\n";

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	//AfxMessageBox(strMsg);
}

void CShopEH::OnInquireCash(
	DWORD	AccountSeq, 
	long	ResultCode,
	long	OutBoundResultCode,
	double	CashSum, 
	double	MileageSum)
{
	CString strMsg, strDetail;
	strMsg.Format(_T("OnInquireCash\r\nAccountSeq:%d, CashSum:%f, MileageSum:%f, ResultCode:%d, OutBoundResultCode:%d\r\n"), 
					AccountSeq, CashSum, MileageSum, ResultCode, OutBoundResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}


void CShopEH::OnInquireCash(
	DWORD	AccountSeq, 
	char	AccoundID[MAX_ACCOUNTID_LENGTH], 
	double	CashSum, 
	double	PointSum, 
	double	MileageSum, 
	int	DetailCount, 
	ShopProtocol::STCashDetail	Detail[], 
	long	ResultCode,
	long	OutBoundResultCode)
{
	CString strMsg, strDetail;
	strMsg.Format(_T("OnInquireCash\r\nAccountSeq:%d, AccoundID:%s, CashSum:%f, PointSum:%f, MileageSum:%f, ResultCode:%d, OutBoundResultCode:%d, \r\ndetail information: \r\n"), 
					AccountSeq, AccoundID, CashSum, PointSum, MileageSum, ResultCode, OutBoundResultCode);

	USES_CONVERSION;	
	for (int i=0; i<DetailCount; i++) {
		if (0 != Detail[i].Type) {
			strDetail.Format(_T("[Name:%s, Type:%c, Value:%f] \r\n"), W2A(Detail[i].Name), Detail[i].Type, Detail[i].Value);
			strMsg = strMsg + strDetail;
		}
	}
	strMsg += "\r\n";

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}


void CShopEH::OnBuyProduct(
	DWORD	AccountSeq, 
	long	ResultCode, 
	long	LeftProductCount)
{
	CString strMsg;
	strMsg.Format(_T("OnBuyProduct\r\nAccountSeq:%d, ResultCode:%d, LeftProductCount:%d\r\n"), 
		AccountSeq, ResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnBuyProduct(
	DWORD	AccountSeq, 
	long	ResultCode,
	long	OutBoundResultCode,
	long	LeftProductCount)
{
	CString strMsg;
	strMsg.Format(_T("OnBuyProduct\r\nAccountSeq:%d, ResultCode:%d, OutBoundResultCode:%d, LeftProductCount:%d\r\n"), 
		AccountSeq, ResultCode, OutBoundResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnBuyProduct(
	DWORD	AccountSeq, 
	char	AccoundID[MAX_ACCOUNTID_LENGTH], 
	long	LeftProductCount, 
	long	ResultCode,
	long	OutBoundResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnBuyProduct\r\nAccountSeq:%d, AccoundID:%s, ResultCode:%d, OutBoundResultCode:%d, LeftProductCount:%d\r\n"), 
		AccountSeq, AccoundID, ResultCode, OutBoundResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnGiftProduct(
	DWORD	SenderSeq, 
	DWORD	ReceiverSeq, 
	double	LimitedCash, 
	long	LeftProductCount, 
	long	ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnGiftProduct\r\nSenderSeq:%d, ReceiverSeq:%d, ResultCode:%d, LimitedCash:%f, LeftProductCount:%d\r\n"), 
		SenderSeq, ReceiverSeq, ResultCode, LimitedCash, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnGiftProduct(
	DWORD	SenderAccountSeq, 
	long	ResultCode,
	long	OutBoundResultCode,
	DWORD	ReceiverAccountSeq, 
	long	LeftProductCount)
{
	CString strMsg;
	USES_CONVERSION;

	strMsg.Format(_T("OnGiftProduct\r\nSenderAccountSeq:%d, ReceiverAccountSeq:%d, ResultCode:%d, OutBoundResultCode:%d, LeftProductCount:%d\r\n"), 
				SenderAccountSeq, ReceiverAccountSeq, ResultCode, OutBoundResultCode, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnGiftProduct(
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
	long	OutBoundResultCode)
{
	USES_CONVERSION;
	CString strMsg;
	strMsg.Format(_T("OnGiftProduct\r\nSenderAccountSeq:%d, SenderAccountID:%s, SenderCharName:%s, ReceiverAccountSeq:%d, ReceiverAccountID:%s, ReceiverCharName:%s, Message:%s, ResultCode:%d, OutBoundResultCode:%d, LimitedCash:%f, LeftProductCount:%d\r\n"), 
		SenderAccountSeq, SenderAccountID, W2A(SenderCharName), ReceiverAccountSeq, ReceiverAccountID, W2A(ReceiverCharName), W2A(Message), 
		ResultCode, OutBoundResultCode, LimitedCash, LeftProductCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireStorageList(
	DWORD	AccountID, 
	long	ListCount, 
	long	ResultCode, 
	ShopProtocol::STStorage	StorageList[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireStorageList\r\nResultCode:%d, AccountID:%d, ListCount:%d\r\n"), 
		ResultCode, AccountID, ListCount);

	USES_CONVERSION;
	for (int i=0; i<ListCount; i++) {
		strProDuct.Format(_T("Seq:%d, ItemSeq:%d, GroupCode:%d, ShareFlag:%d, ProductSeq:%d, \
							 CashName:%s, CashPoint:%f, SendAccountID:%s, SendMessage:%s, ItemType:%c, \
							 RelationType:%d, PriceSeq:%d, ProductType:%d\r\n"), 
		StorageList[i].Seq, 
		StorageList[i].ItemSeq, 
		StorageList[i].GroupCode, 
		StorageList[i].ShareFlag, 
		StorageList[i].ProductSeq, 
		W2A(StorageList[i].CashName), 
		StorageList[i].CashPoint, 		
		W2A(StorageList[i].SendAccountID), 
		W2A(StorageList[i].SendMessage), 	
		StorageList[i].ItemType, 
		StorageList[i].RelationType, 
		StorageList[i].PriceSeq,
		StorageList[i].ProductType);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireStorageListPage(
	DWORD	AccountID, 
	int		ResultCode, 
	char	StorageType,
	int		NowPage,
	int		TotalPage,
	int		TotalCount,
	int		ListCount, 
	ShopProtocol::STStorage	StorageList[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireStorageListPage\r\nResultCode:%d, AccountID:%d, StorageType:%c, NowPage:%d, TotalPage:%d, TotalCount:%d, ListCount:%d\r\n"), 
		ResultCode, AccountID, StorageType ? StorageType : ' ', 
		NowPage, TotalPage, TotalCount, ListCount);

	USES_CONVERSION;

	for (int i=0 ; i<ListCount; i++) {
		strProDuct.Format(_T("Seq:%d, ItemSeq:%d, GroupCode:%d, ShareFlag:%d, ProductSeq:%d, CashName:%s, CashPoint:%f, \
							 SendAccountID:%s, SendMessage:%s, ItemType:%c, RelationType:%d, PriceSeq:%d, ProductType:%d\r\n"), 
		StorageList[i].Seq, 
		StorageList[i].ItemSeq, 
		StorageList[i].GroupCode, 
		StorageList[i].ShareFlag, 
		StorageList[i].ProductSeq, 
		W2A(StorageList[i].CashName), 
		StorageList[i].CashPoint, 		
		W2A(StorageList[i].SendAccountID), 
		W2A(StorageList[i].SendMessage), 	
		StorageList[i].ItemType, 
		StorageList[i].RelationType, 
		StorageList[i].PriceSeq,
		StorageList[i].ProductType);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireStorageListPageNoGiftMessage(
	DWORD	AccountID, 
	int		ResultCode, 
	char	StorageType,
	int		NowPage,
	int		TotalPage,
	int		TotalCount,
	int		ListCount, 
	ShopProtocol::STStorageNoGiftMessage StorageList[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireStorageListPageNoGiftMessage\r\nResultCode:%d, AccountID:%d, StorageType:%c, NowPage:%d, \
					 TotalPage:%d, TotalCount:%d, ListCount:%d\r\n"), 
		ResultCode, AccountID, StorageType ? StorageType : ' ', 
		NowPage, TotalPage, TotalCount, ListCount);

	USES_CONVERSION;
	for (int i=0; i<ListCount; i++) {
		strProDuct.Format(_T("Seq:%d, ItemSeq:%d, GroupCode:%d, ShareFlag:%d, ProductSeq:%d, CashName:%s, \
							 CashPoint:%f, SendAccountID:%s, ItemType:%c, RelationType:%d, PriceSeq:%d, ProductType:%d\r\n"), 
		StorageList[i].Seq, 
		StorageList[i].ItemSeq, 
		StorageList[i].GroupCode, 
		StorageList[i].ShareFlag, 
		StorageList[i].ProductSeq, 
		W2A(StorageList[i].CashName), 
		StorageList[i].CashPoint, 		
		W2A(StorageList[i].SendAccountID), 
		StorageList[i].ItemType, 
		StorageList[i].RelationType, 
		StorageList[i].PriceSeq,
		StorageList[i].ProductType);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireStorageListEx(
	DWORD	AccountID, 
	long	ListCount, 
	long	ResultCode, 
	ShopProtocol::STStorageEx	StorageListEx[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireStorageListEx\r\nResultCode:%d, AccountID:%d, ListCount:%d\r\n"), 
		ResultCode, AccountID, ListCount);

	USES_CONVERSION;
	for (int i=0; i<ListCount; i++) {
		DWORD DeleteDate = StorageListEx[i].DeleteDate;

		struct tm tDeleteDate;
		tDeleteDate.tm_hour = 0;
		tDeleteDate.tm_isdst = 0;
		tDeleteDate.tm_mday = 0;
		tDeleteDate.tm_min = 0;
		tDeleteDate.tm_mon = 0;
		tDeleteDate.tm_sec = 0;
		tDeleteDate.tm_wday = 0;
		tDeleteDate.tm_yday = 0;
		tDeleteDate.tm_year = 0;

		#ifdef VERSION_VS60
			struct tm* tDeleteDate1 = NULL;
			tDeleteDate1 = localtime((time_t*)&DeleteDate);
			if(tDeleteDate1) tDeleteDate = *tDeleteDate1;
		#elif VERSION_VS2003
			struct tm* tDeleteDate1 = NULL;
			tDeleteDate = *localtime((time_t*)&DeleteDate);
			if(tDeleteDate1) tDeleteDate = *tDeleteDate1;
		#else
			_localtime32_s(&tDeleteDate, (__time32_t*)&DeleteDate);
		#endif

			strProDuct.Format(_T("Seq:%d, ItemSeq:%d, GroupCode:%d, ShareFlag:%d, \
								 ProductSeq:%d, CashName:%s, CashPoint:%f, SendAccountID:%s, SendMessage:%s, \
								 ItemType:%c, RelationType:%d, PriceSeq:%d, ProductType:%d, DeleteDate:%d-%d-%d %d:%d:%d\r\n"), 
			StorageListEx[i].Seq, 
			StorageListEx[i].ItemSeq, 
			StorageListEx[i].GroupCode, 
			StorageListEx[i].ShareFlag, 
			StorageListEx[i].ProductSeq, 
			W2A(StorageListEx[i].CashName), 
			StorageListEx[i].CashPoint, 		
			W2A(StorageListEx[i].SendAccountID), 
			W2A(StorageListEx[i].SendMessage), 	
			StorageListEx[i].ItemType, 
			StorageListEx[i].RelationType, 
			StorageListEx[i].PriceSeq,
			StorageListEx[i].ProductType,
			tDeleteDate.tm_year+1900, tDeleteDate.tm_mon+1, tDeleteDate.tm_mday, tDeleteDate.tm_hour, tDeleteDate.tm_min, tDeleteDate.tm_sec);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireStorageListPageEx(
	DWORD	AccountID, 
	int		ResultCode, 
	char	StorageType,
	int		NowPage,
	int		TotalPage,
	int		TotalCount,
	int		ListCount, 
	ShopProtocol::STStorageEx	StorageListEx[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireStorageListPageEx\r\nResultCode:%d, AccountID:%d, StorageType:%c, \
					 NowPage:%d, TotalPage:%d, TotalCount:%d, ListCount:%d\r\n"), 
		ResultCode, AccountID, StorageType ? StorageType : ' ', 
		NowPage, TotalPage, TotalCount, ListCount);

	USES_CONVERSION;
	for (int i=0; i<ListCount; i++) {
		DWORD DeleteDate = StorageListEx[i].DeleteDate;

		struct tm tDeleteDate;
		tDeleteDate.tm_hour = 0;
		tDeleteDate.tm_isdst = 0;
		tDeleteDate.tm_mday = 0;
		tDeleteDate.tm_min = 0;
		tDeleteDate.tm_mon = 0;
		tDeleteDate.tm_sec = 0;
		tDeleteDate.tm_wday = 0;
		tDeleteDate.tm_yday = 0;
		tDeleteDate.tm_year = 0;

		#ifdef VERSION_VS60
			struct tm* tDeleteDate1 = NULL;
			tDeleteDate1 = localtime((time_t*)&DeleteDate);
			if(tDeleteDate1) tDeleteDate = *tDeleteDate1;
		#elif VERSION_VS2003
			struct tm* tDeleteDate1 = NULL;
			tDeleteDate= *localtime((time_t*)&DeleteDate);
			if(tDeleteDate1) tDeleteDate = *tDeleteDate1;
		#else
			_localtime32_s(&tDeleteDate, (__time32_t*)&DeleteDate);
		#endif

			strProDuct.Format(_T("Seq:%d, ItemSeq:%d, GroupCode:%d, ShareFlag:%d, \
								 ProductSeq:%d, CashName:%s, CashPoint:%f, SendAccountID:%s, SendMessage:%s, \
								 ItemType:%c, RelationType:%d, PriceSeq:%d, ProductType:%d, DeleteDate:%d-%d-%d %d:%d:%d\r\n"), 
			StorageListEx[i].Seq, 
			StorageListEx[i].ItemSeq, 
			StorageListEx[i].GroupCode, 
			StorageListEx[i].ShareFlag, 
			StorageListEx[i].ProductSeq, 
			W2A(StorageListEx[i].CashName), 
			StorageListEx[i].CashPoint, 		
			W2A(StorageListEx[i].SendAccountID), 
			W2A(StorageListEx[i].SendMessage), 	
			StorageListEx[i].ItemType, 
			StorageListEx[i].RelationType, 
			StorageListEx[i].PriceSeq,
			StorageListEx[i].ProductType,
			tDeleteDate.tm_year+1900, tDeleteDate.tm_mon+1, tDeleteDate.tm_mday, tDeleteDate.tm_hour, tDeleteDate.tm_min, tDeleteDate.tm_sec);


		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireStorageListPageNoGiftMessageEx(
	DWORD	AccountID, 
	int		ResultCode, 
	char	StorageType,
	int		NowPage,
	int		TotalPage,
	int		TotalCount,
	int		ListCount, 
	ShopProtocol::STStorageNoGiftMessageEx	StorageListEx[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireStorageListPageNoGiftMessageEx\r\nResultCode:%d, AccountID:%d, \
					 StorageType:%c, NowPage:%d, TotalPage:%d, TotalCount:%d, ListCount:%d\r\n"), 
		ResultCode, AccountID, StorageType ? StorageType : ' ', 
		NowPage, TotalPage, TotalCount, ListCount);

	USES_CONVERSION;
	for (int i=0; i<ListCount; i++) {
		DWORD DeleteDate = StorageListEx[i].DeleteDate;

		struct tm tDeleteDate;
		tDeleteDate.tm_hour = 0;
		tDeleteDate.tm_isdst = 0;
		tDeleteDate.tm_mday = 0;
		tDeleteDate.tm_min = 0;
		tDeleteDate.tm_mon = 0;
		tDeleteDate.tm_sec = 0;
		tDeleteDate.tm_wday = 0;
		tDeleteDate.tm_yday = 0;
		tDeleteDate.tm_year = 0;

		#ifdef VERSION_VS60
			struct tm* tDeleteDate1 = NULL;
			tDeleteDate1 = localtime((time_t*)&DeleteDate);
			if(tDeleteDate1) tDeleteDate = *tDeleteDate1;
		#elif VERSION_VS2003
			struct tm* tDeleteDate1 = NULL;
			tDeleteDate= *localtime((time_t*)&DeleteDate);
			if(tDeleteDate1) tDeleteDate = *tDeleteDate1;
		#else
			_localtime32_s(&tDeleteDate, (__time32_t*)&DeleteDate);
		#endif

			strProDuct.Format(_T("Seq:%d, ItemSeq:%d, GroupCode:%d, ShareFlag:%d, \
								 ProductSeq:%d, CashName:%s, CashPoint:%f, SendAccountID:%s, \
								 ItemType:%c, RelationType:%d, PriceSeq:%d, ProductType:%d, DeleteDate:%d-%d-%d %d:%d:%d\r\n"), 
			StorageListEx[i].Seq, 
			StorageListEx[i].ItemSeq, 
			StorageListEx[i].GroupCode, 
			StorageListEx[i].ShareFlag, 
			StorageListEx[i].ProductSeq, 
			W2A(StorageListEx[i].CashName), 
			StorageListEx[i].CashPoint, 		
			W2A(StorageListEx[i].SendAccountID), 
			StorageListEx[i].ItemType, 
			StorageListEx[i].RelationType, 
			StorageListEx[i].PriceSeq,
			StorageListEx[i].ProductType,
			tDeleteDate.tm_year+1900, tDeleteDate.tm_mon+1, tDeleteDate.tm_mday, tDeleteDate.tm_hour, tDeleteDate.tm_min, tDeleteDate.tm_sec);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnGiftCash(
	DWORD	SenderSeq, 
	DWORD	ReceiverSeq, 
	long	ResultCode, 
	double	GiftSendLimit)
{
	CString strMsg;
	strMsg.Format(_T("OnGiftCash\r\nSenderSeq:%d, ReceiverSeq:%d, ResultCode:%d, GiftSendLimit:%f\r\n"), 
		SenderSeq, ReceiverSeq, ResultCode, GiftSendLimit);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireBuyGiftPossibility(
	DWORD	AccountID, 
	long	ResultCode, 
	long	ItemBuy, 
	long	Present, 
	double	MyPresentCash, 
	double	MyPresentSendLimit)
{
	CString strMsg;
	strMsg.Format(_T("OnInquireBuyGiftPossibility\r\nAccountID:%d, ResultCode:%d, \
					 ItemBuy availability:%d, Present availability:%d, MyPresentCash(one month):%f, MyPresentSendLimit(one month):%f\r\n"), 
		AccountID, ResultCode, ItemBuy, Present, MyPresentCash, MyPresentSendLimit);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireEventProductList(
	DWORD	AccountSeq, 
	long	ProductDisplaySeq, 
	int		PackagesCount, 
	long	Packages[])
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireEventProductList\r\nAccountSeq:%d, ProductDisplaySeq:%d, PackagesCount:%d\r\n"), 
		AccountSeq, ProductDisplaySeq, PackagesCount);

	strMsg += "Package List \r\n";
	for (int i=0; i<PackagesCount; i++) {
		strProDuct.Format(_T("%d "), 	Packages[i]);

		strMsg += strProDuct;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireProductLeftCount(
	long	PackageSeq,
	long	LeftCount)
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnInquireProductLeftCount\r\nPackageSeq:%d, LeftCount:%d\r\n"), 
		PackageSeq, LeftCount);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnUpdateVersion(
	long			GameCode, 
	unsigned short	SalesZone, 
	unsigned short	year, 
	unsigned short	yearIdentity)
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnUpdateVersion\r\nGameCode:%d, SalesZone:%d, year:%d, year Identity:%d\r\n"), 
		GameCode, SalesZone, year, yearIdentity);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnUpdateBannerVersion(
	long			GameCode, 
	unsigned short	BannerZone, 
	unsigned short	year, 
	unsigned short	yearIdentity)
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnUpdateBannerVersion\r\nGameCode:%d, BannerZone:%d, year:%d, year Identity:%d\r\n"), 
		GameCode, BannerZone, year, yearIdentity);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnUseStorage(
	DWORD	AccountSeq, 
	long	ResultCode, 
	char	InGameProductID[MAX_TYPENAME_LENGTH], 
	BYTE	PropertyCount, 
	long	ProductSeq, 						   
	long	StorageSeq, 
	long	StorageItemSeq, 
	ShopProtocol::STItemProperty	PropertyList[])
{
	CString strMsg, strProperty;
	strMsg.Format(_T("OnUseStorage\r\nAccountSeq:%d, ResultCode:%d, InGameProductID:%s, PropertyCount:%d, ProductSeq:%d, StorageSeq:%d, StorageItemSeq:%d\r\n"), 
		AccountSeq, ResultCode, InGameProductID, PropertyCount, ProductSeq, StorageSeq, StorageItemSeq);

	for (int i=0; i<PropertyCount; i++) {
		strProperty.Format(_T("PropertySeq:%d, Value:%d\r\n"), PropertyList[i].PropertySeq, PropertyList[i].Value);
		strMsg += strProperty;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnUseStorageEx(
	DWORD	AccountSeq, 
	long	ResultCode, 
	char	InGameProductID[MAX_TYPENAME_LENGTH], 
	BYTE	PropertyCount, 
	long	ProductSeq, 						   
	long	StorageSeq, 
	long	StorageItemSeq, 
	ShopProtocol::STItemProperty	PropertyList[],
	int		CashType)
{
	CString strMsg, strProperty;
	strMsg.Format(_T("OnUseStorageEx\r\nAccountSeq:%d, ResultCode:%d, InGameProductID:%s, PropertyCount:%d, ProductSeq:%d, StorageSeq:%d, StorageItemSeq:%d, CashType:%d\r\n"), 
		AccountSeq, ResultCode, InGameProductID, PropertyCount, ProductSeq, StorageSeq, StorageItemSeq, CashType);

	for (int i=0; i<PropertyCount; i++) {
		strProperty.Format(_T("PropertySeq:%d, Value:%d\r\n"), PropertyList[i].PropertySeq, PropertyList[i].Value);
		strMsg += strProperty;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnUseStorageExDeductMileageFlag(
	DWORD	AccountSeq, 
	long	ResultCode, 
	char	InGameProductID[MAX_TYPENAME_LENGTH], 
	BYTE	PropertyCount, 
	long	ProductSeq, 						   
	long	StorageSeq, 
	long	StorageItemSeq, 
	ShopProtocol::STItemProperty	PropertyList[],
	bool	DeductMileageFlag)
{
	CString strMsg, strProperty;
	strMsg.Format(_T("OnUseStorageExDeductMileageFlag\r\nAccountSeq:%d, ResultCode:%d, InGameProductID:%s, PropertyCount:%d, ProductSeq:%d, StorageSeq:%d, StorageItemSeq:%d, DeductMileageFlag:%d\r\n"), 
		AccountSeq, ResultCode, InGameProductID, PropertyCount, ProductSeq, StorageSeq, StorageItemSeq, DeductMileageFlag);

	for (int i=0; i<PropertyCount; i++) {
		strProperty.Format(_T("PropertySeq:%d, Value:%d\r\n"), PropertyList[i].PropertySeq, PropertyList[i].Value);
		strMsg += strProperty;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnRollbackUseStorage(
	DWORD	AccountSeq,
	long	ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnRollbackUseStorage\r\nAccountSeq:%d, ResultCode:%d\r\n"), 
					AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnMileageDeduct(
	DWORD	AccountSeq,
	long	ResultCode)
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnMileageDeduct\r\nAccountSeq:%d, ResultCode:%d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnMileageSave(
	DWORD	AccountSeq,
	long	ResultCode)
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnMileageSave\r\nAccountSeq:%d, ResultCode:%d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnMileageLiveSaveUp(
	DWORD	AccountSeq,
	long	ResultCode)
{
	CString strMsg, strProDuct;
	strMsg.Format(_T("OnMileageLiveSaveUp\r\nAccountSeq:%d, ResultCode:%d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnThrowStorage(
	DWORD	AccountSeq,
	long	ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnThrowStorage\r\nAccountSeq:%d, ResultCode:%d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnItemSerialUpdate(
	DWORD	AccountSeq,
	long	ResultCode)
{
	CString strMsg;
	strMsg.Format(_T("OnItemSerialUpdate\r\nAccountSeq:%d, ResultCode:%d\r\n"), 
		AccountSeq, ResultCode);

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::OnInquireInGamePointValue(
	long	ResultCode,
	long	PointCount,
	ShopProtocol::STPointDetail	PointList[])
{
	CString strMsg, strProperty;
	strMsg.Format(_T("OnInquireInGamePointValue, PointCount:%d\r\n"), PointCount);

	for (int i=0; i<PointCount; i++) {
		strProperty.Format(_T("PointType:%d, PointTypeName:%s, PointValue:%d\r\n"), 
			PointList[i].PointType, PointList[i].PointTypeName, PointList[i].PointValue);
		strMsg += strProperty;
	}

	WriteLog((LPSTR)(LPCTSTR)strMsg);
	AfxMessageBox(strMsg);
}

void CShopEH::WriteLog(char* szMsg)
{
	::EnterCriticalSection(&m_cs);

	TCHAR szModule[MAX_PATH] = {0};
	TCHAR szFilePath[MAX_PATH] = {0};

	::GetModuleFileName( NULL, szModule, MAX_PATH );

	*(_tcsrchr( szModule, '\\' ) ) = 0;

	StringCchPrintf( szFilePath, MAX_PATH, _T("%s\\ClientDialog_ShopClient.log"), szModule );

	CTime t = CTime::GetCurrentTime();   	
	
	CString str;
	str.Format(_T("[%s] %s"), t.Format(_T("%Y-%m-%d %H:%M:%S")), szMsg);


	HANDLE	hFile;	
	DWORD	dwBytesWritten, dwPos;  

	hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  //File »ý¼º 
	if (hFile != INVALID_HANDLE_VALUE)
	{ 
		dwPos = SetFilePointer(hFile, 0, NULL, FILE_END); 

		LockFile(hFile, dwPos, 0, dwPos + str.GetLength(), 0); 
		WriteFile(hFile, str.GetBuffer(0), str.GetLength(), &dwBytesWritten, NULL); 
		str.ReleaseBuffer();
		UnlockFile(hFile, dwPos, 0, dwPos + str.GetLength(), 0);     
		CloseHandle(hFile); 
	}  		
	
	::LeaveCriticalSection(&m_cs);
}

CString CShopEH::ConvertTime(DWORD dwTime)
{	
	struct tm tEndDate;
	
	tEndDate.tm_hour = 0;
	tEndDate.tm_isdst = 0;
	tEndDate.tm_mday = 0;
	tEndDate.tm_min = 0;
	tEndDate.tm_mon = 0;
	tEndDate.tm_sec = 0;
	tEndDate.tm_wday = 0;
	tEndDate.tm_yday = 0;
	tEndDate.tm_year = 0;

#ifdef VERSION_VS60	
	struct tm *tEndDate1;	
	tEndDate1 = localtime((time_t*)&dwTime);
	if(tEndDate1) tEndDate = *tEndDate1;
#elif VERSION_VS2003
	struct tm *tEndDate1;	
	tEndDate1 = localtime((time_t*)&dwTime);
	if(tEndDate1) tEndDate = *tEndDate1;
#else	
	_localtime32_s(&tEndDate, (__time32_t*)&dwTime);
#endif

	CString strend;

	strend.Format(_T("%d-%d-%d %d:%d:%d"), 
		tEndDate.tm_year+1900, tEndDate.tm_mon+1, tEndDate.tm_mday, tEndDate.tm_hour, tEndDate.tm_min, tEndDate.tm_sec);
	
	return strend;
}
