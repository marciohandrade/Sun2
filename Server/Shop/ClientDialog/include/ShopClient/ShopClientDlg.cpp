#include "stdafx.h"

#include <ShopClient/ShopClientDlg.h>
#include <ShopClient/ShopClientDlgUIHelper.h>
#include <public/ShopDefine.h>
#include "atlbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CShopAboutDlg : public CDialog
{
public:
	CShopAboutDlg() : CDialog(CShopAboutDlg::IDD) {};
	enum { IDD = IDD_SHOP_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}
	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CShopAboutDlg, CDialog)
END_MESSAGE_MAP()



CShopClientDlg::CShopClientDlg(CWnd* pParent /*=NULL*/)
	: CScrollDialog(CShopClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_SHOP_MAINFRAME);	
	m_dwGame = MU_KR;
}

void CShopClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CScrollDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOP_COMBO_GAME_TYPE, m_GameType);
}

BEGIN_MESSAGE_MAP(CShopClientDlg, CScrollDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
#ifdef VERSION_VS60
	ON_BN_CLICKED(IDC_SHOP_BUT_CONNECT, OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_SHOP_BUT_SHOPINQUIRE, OnBnClickedButShopInquire)
	ON_BN_CLICKED(IDC_SHOP_BUT_SHOPBUY, OnBnClickedButShopbuy)
	ON_BN_CLICKED(IDC_SHOP_BUT_GIFT, OnBnClickedButGift)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOP_BUT_GIFTCASH, OnBnClickedButGiftcash)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_CHECK, OnBnClickedButInquireCheck)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_PRODUCTLIST, OnBnClickedButInquireProductlist)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_EVENTPRODUCTLIST, OnBnClickedButInquireEventproductlist)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGE, OnBnClickedButInquireStorage)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEPAGE, OnBnClickedButInquireStoragePage)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGE_EX, OnBnClickedButInquireStorageEx)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEPAGE_EX, OnBnClickedButInquireStoragePageEx)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEUSE, OnBnClickedButInquireStorageuse)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEDEL, OnBnClickedButInquireStoragedel)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_PRODUCTLEFTCOUNT, OnBnClickedButInquireProductleftcount)
	ON_BN_CLICKED(IDC_SHOP_BUT_MILEAGESAVE, OnBnClickedButMileageSave)	
	ON_BN_CLICKED(IDC_SHOP_BUT_MILEAGE_DUDECT, OnBnClickedButMileageDudect)	
	ON_BN_CLICKED(IDC_SHOP_BUT_MILEAGELIVELIVESAVE, OnBnClickedButMileageLiveSave)		
	ON_BN_CLICKED(IDC_SHOP_BUT_ITEMSERIAL, OnBnClickedButItemSerial)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_SHOPVERSION, OnBnClickedInquireSalesVersion)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_BANNERVERSION, OnBnClickedInquireBannerVersion)
	ON_BN_CLICKED(IDC_SHOP_BUT_STORAGEUSE_ROLLBACK, OnBnClickedStorageUseRollback)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_POINT_PERCENTAGE, OnBnClickedInquirePointPercentage)
	ON_CBN_SELCHANGE(IDC_SHOP_COMBO_GAME_TYPE, OnCbnSelchangeComboGameType)

	//

#else
	ON_BN_CLICKED(IDC_SHOP_BUT_CONNECT, &CShopClientDlg::OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_SHOP_BUT_SHOPINQUIRE, &CShopClientDlg::OnBnClickedButShopInquire)
	ON_BN_CLICKED(IDC_SHOP_BUT_SHOPBUY, &CShopClientDlg::OnBnClickedButShopbuy)
	ON_BN_CLICKED(IDC_SHOP_BUT_GIFT, &CShopClientDlg::OnBnClickedButGift)
	ON_BN_CLICKED(IDCANCEL, &CShopClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOP_BUT_GIFTCASH, &CShopClientDlg::OnBnClickedButGiftcash)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_CHECK, &CShopClientDlg::OnBnClickedButInquireCheck)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_PRODUCTLIST, &CShopClientDlg::OnBnClickedButInquireProductlist)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_EVENTPRODUCTLIST, &CShopClientDlg::OnBnClickedButInquireEventproductlist)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGE, &CShopClientDlg::OnBnClickedButInquireStorage)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEPAGE, &CShopClientDlg::OnBnClickedButInquireStoragePage)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGE_EX, &CShopClientDlg::OnBnClickedButInquireStorageEx)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEPAGE_EX, &CShopClientDlg::OnBnClickedButInquireStoragePageEx)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEUSE, &CShopClientDlg::OnBnClickedButInquireStorageuse)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_STORAGEDEL, &CShopClientDlg::OnBnClickedButInquireStoragedel)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_PRODUCTLEFTCOUNT, &CShopClientDlg::OnBnClickedButInquireProductleftcount)
	ON_BN_CLICKED(IDC_SHOP_BUT_MILEAGESAVE, &CShopClientDlg::OnBnClickedButMileageSave)	
	ON_BN_CLICKED(IDC_SHOP_BUT_MILEAGE_DUDECT, &CShopClientDlg::OnBnClickedButMileageDudect)	
	ON_BN_CLICKED(IDC_SHOP_BUT_MILEAGELIVELIVESAVE, &CShopClientDlg::OnBnClickedButMileageLiveSave)	
	ON_BN_CLICKED(IDC_SHOP_BUT_ITEMSERIAL, &CShopClientDlg::OnBnClickedButItemSerial)	
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_SHOPVERSION, &CShopClientDlg::OnBnClickedInquireSalesVersion)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_BANNERVERSION, &CShopClientDlg::OnBnClickedInquireBannerVersion)
	ON_BN_CLICKED(IDC_SHOP_BUT_STORAGEUSE_ROLLBACK, &CShopClientDlg::OnBnClickedStorageUseRollback)
	ON_BN_CLICKED(IDC_SHOP_BUT_INQUIRE_POINT_PERCENTAGE, &CShopClientDlg::OnBnClickedInquirePointPercentage)
	ON_CBN_SELCHANGE(IDC_SHOP_COMBO_GAME_TYPE, &CShopClientDlg::OnCbnSelchangeComboGameType)
	
	//..
	
#endif	
END_MESSAGE_MAP()

BOOL CShopClientDlg::OnInitDialog()
{
	CScrollDialog::OnInitDialog();

	ASSERT((IDM_SHOP_ABOUTBOX & 0xFFF0) == IDM_SHOP_ABOUTBOX);
	ASSERT(IDM_SHOP_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_SHOP_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_SHOP_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

// UI 관련 초기화
	MoveWindow(0,0,500,800);	// 창 크기

// UI 설정 : 테스트용 데이터 세팅
	helper_ = std::auto_ptr<CShopClientDlgUIHelper>(new CShopClientDlgUIHelper());
	helper_->init(this);

// 샵서버와 통신할 클라이언트 네트워크 초기화
	m_Client.CreateSession(CShopClientDlg::ErrorHandler); //클라이언트 초기화

	return TRUE;
}


void CShopClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_SHOP_ABOUTBOX) {
		CShopAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CScrollDialog::OnSysCommand(nID, lParam);
	}
}


void CShopClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CScrollDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CShopClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShopClientDlg::OnBnClickedButConnect()
{
	CString strIP, strPort;
	
	GetDlgItemText(IDC_SHOP_EDIT_IP, strIP);
	GetDlgItemText(IDC_SHOP_EDIT_PORT, strPort);
	
	USES_CONVERSION;
	m_Client.Connect(T2A(strIP.GetBuffer(0)), _tstoi(strPort.GetBuffer(0)));
}

void CShopClientDlg::OnBnClickedInquireSalesVersion()
{
	CString strSalesZoneCode, strGameCode;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_SALESZONE, strSalesZoneCode);

	USES_CONVERSION;

	m_Client.InquireSalesZoneScriptVersion(_tstoi(strGameCode), _tstoi(strSalesZoneCode));
}

void CShopClientDlg::OnBnClickedInquireBannerVersion()
{
	CString strBannerZoneCode, strGameCode;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_BANNERVERSION_BANNERZONE, strBannerZoneCode);

	USES_CONVERSION;

	m_Client.InquireBannerZoneScriptVersion(_tstoi(strGameCode), _tstoi(strBannerZoneCode));
}


void CShopClientDlg::OnBnClickedButShopInquire()
{	
	bool bSum = false;
	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SHOP_CHK_SUM))->GetCheck()) bSum = true;
	
	CString strType, strAccount, strAccountID, strGameCode, strMileage, strPaymentType, strMemberNo;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_ACCOUNT, strAccount);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR, strAccountID);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE, strPaymentType);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_TYPE, strType);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_MILEAGE, strMileage);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_MEMBERNO, strMemberNo);

	CString strSendCnt;
	GetDlgItemText(IDC_EDIT_SEND_CNT, strSendCnt);
	int send_cnt = _tstoi(strSendCnt);
	if (send_cnt == 0 ) { send_cnt = 1; }
	

	USES_CONVERSION;
	for (int i=0; i<send_cnt; ++i) {
	switch (m_dwGame)
	{
	case MU_KR	:
	case BT_KR	:
	case R2_KR	:
	case SUN_KR	:
		m_Client.InquireCash(_tstoi(strGameCode), (BYTE)_tstoi(strType), _tstoi(strAccount), bSum, _tstoi(strMileage));
		break;
	case SUN_GB	:
	case MU_GB	:
		m_Client.InquireCash((BYTE)_tstoi(strType), _tstoi(strAccount), _tstoi(strGameCode), bSum, _tstoi(strMileage));	
		break;
	case SUN_JP	:
	case MU_JP	: 
	case MU_VN	:
	case MU_TW	:
	case BT_TH	:
	case SUN_RU	:
	case SUN_CN	:
		m_Client.InquireCash(_tstoi(strGameCode),
							 (ShopProtocol::PaymentType)_tstoi(strPaymentType), 
							 _tstoi(strAccount), 
							 _tstoi(strAccountID), 
							 _tstoi(strType), 
							 _tstoi(strMileage), 
							 bSum);
		break;
	case MU_TH	:
		m_Client.InquireCash(_tstoi(strGameCode),
							 (ShopProtocol::PaymentType)_tstoi(strPaymentType), 
							 _tstoi(strAccount), 
							 T2A((LPTSTR)(LPCTSTR)strAccountID),
							 _tstoi(strType), 
							 _tstoi(strMileage), 
							 bSum);
		break;
	case BT_CNL	:
		if (_tstoi(strMemberNo) != 0) { // 멤버no 가 존재하면 채널링이라고 본다
			// 배터리 채널링
			USES_CONVERSION;			
			m_Client.InquireCash(_tstoi(strGameCode),
								 (ShopProtocol::PaymentType)_tstoi(strPaymentType), 
								 _tstoi(strAccount), 
								 T2A((LPTSTR)(LPCTSTR)strAccountID),
								 _tstoi(strType), 
								 _tstoi(strMileage), 
								 bSum,
								 T2W((LPTSTR)(LPCTSTR)strMemberNo));
		} else {
			AfxMessageBox(_T("not available InquireCash - Channeling"));
		}
		break;
	default:
		AfxMessageBox(_T("not available InquireCash"));
		break;
	}
	}
}

void CShopClientDlg::OnBnClickedButShopbuy()
{	
	CString strAccount, strAccountName, strGameCode, strPackageSeq, strZone, strPrice, 
		strDispalySeq, strType, strClass, strLevel, strCharName, 
		Rank, strServerIndex, strIP, strRefKey, strPaymentType, strCashTypeCode, strMemberNo;

	GetDlgItemText(IDC_SHOP_EDIT_BUY_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_ACCOUNT, strAccount);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_PACKAGESEQ, strPackageSeq);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_DISPLAYSEQ, strDispalySeq);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_ZONE, strZone);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_PRICE, strPrice);		
	GetDlgItemText(IDC_SHOP_EDIT_BUY_PRODUCTCLASS, strClass);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_LEVEL, strLevel);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_CHARNAME, strCharName);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_RANK, Rank);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_SERVERINDEX, strServerIndex);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_ACCOUNT_STR, strAccountName);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_IP, strIP);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_REFKEY, strRefKey);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_PAYMENTTYPE, strPaymentType);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_CASHTYPECODE, strCashTypeCode);
	GetDlgItemText(IDC_SHOP_EDIT_BUY_MEMBERNO, strMemberNo);

	CString strAccessIP;
	GetDlgItemText(IDC_SHOP_EDIT_BUY_ACCESSIP, strAccessIP);

	bool bMileage = false;
	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SHOP_CHK_BUY_MILEAGE))->GetCheck()) bMileage = true;

	USES_CONVERSION;
	switch(m_dwGame)
	{
	case MU_KR	:
	case BT_KR	:
	case R2_KR	:
	case SUN_KR	:
		// [deprecated] - [AccessIP 추가 버전] 사용 권장
		m_Client.BuyProduct(_tstoi(strGameCode),
							_tstoi(strAccount), 
							_tstoi(strPackageSeq), 
							_tstoi(strDispalySeq), 
							_tstoi(strZone), 
							_tstoi(strPrice), 
							_tstoi(strClass), 
							_tstoi(strLevel), 
							T2W((LPTSTR)(LPCTSTR)strCharName), 
							T2W((LPTSTR)(LPCTSTR)Rank), 
							_tstoi(strServerIndex));
		//
		// [AccessIP 추가 버전]
		//m_Client.BuyProduct(_tstoi(strGameCode),
		//					_tstoi(strAccount), 
		//					_tstoi(strPackageSeq), 
		//					_tstoi(strDispalySeq), 
		//					_tstoi(strZone), 
		//					_tstoi(strPrice), 
		//					_tstoi(strClass), 
		//					_tstoi(strLevel),
		//					inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)),
		//					T2W((LPTSTR)(LPCTSTR)strCharName), 
		//					T2W((LPTSTR)(LPCTSTR)Rank), 
		//					_tstoi(strServerIndex) );

		break;
	case SUN_GB	:
	case MU_GB	:
		// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
		//m_Client.BuyProduct(_tstoi(strAccount), 
		//					_tstoi(strGameCode),
		//					_tstoi(strPackageSeq), 
		//					_tstoi(strPrice), 
		//					_tstoi(strZone), 
		//					_tstoi(strDispalySeq), 
		//					_tstoi(strClass), 
		//					_tstoi(strLevel), 
		//					T2W((LPCTSTR)strCharName), 
		//					T2W((LPCTSTR)Rank), 
		//					_tstoi(strServerIndex),
		//					_tstoi(strCashTypeCode));
		//
		// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
		//m_Client.BuyProduct(_tstoi(strAccount), 
		//					_tstoi(strGameCode),
		//					_tstoi(strPackageSeq), 
		//					_tstoi(strPrice), 
		//					_tstoi(strZone), 
		//					_tstoi(strDispalySeq), 
		//					_tstoi(strClass), 
		//					_tstoi(strLevel), 
		//					T2W((LPTSTR)(LPCTSTR)strCharName), 
		//					T2W((LPTSTR)(LPCTSTR)Rank), 
		//					_tstoi(strServerIndex),
		//					_tstoi(strCashTypeCode),
		//					bMileage);		// <------------ 고블린을 포인트 사용을 지원하기 위해 DeductMileageFlag 추가된 버전

		// [DeductMileageFlag & AccessIP 추가 버전]
		m_Client.BuyProduct(_tstoi(strAccount), 
							_tstoi(strGameCode),
							_tstoi(strPackageSeq), 
							_tstoi(strPrice), 
							_tstoi(strZone), 
							_tstoi(strDispalySeq), 
							_tstoi(strClass), 
							_tstoi(strLevel), 
							T2W((LPTSTR)(LPCTSTR)strCharName), 
							T2W((LPTSTR)(LPCTSTR)Rank), 
							_tstoi(strServerIndex),
							_tstoi(strCashTypeCode),
							bMileage,		// <------------ 고블린을 포인트 사용을 지원하기 위해 DeductMileageFlag 추가된 버전
							inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));	// <------------ 클라이언트 아이피


		break;
	case SUN_JP	:
	case MU_JP	:
	case MU_VN	:
	case MU_TW	:
	case BT_TH	:
	case SUN_RU	:
	case SUN_CN	:
		m_Client.BuyProduct(_tstoi(strGameCode),
							(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
							_tstoi(strAccount), 
							_tstoi(strAccountName), 
							_tstoi(strPackageSeq), 
							_tstoi(strPrice), 
							_tstoi(strZone), 
							_tstoi(strDispalySeq), 
							_tstoi(strClass), 
							_tstoi(strLevel), 
							T2W((LPTSTR)(LPCTSTR)strCharName), 
							T2W((LPTSTR)(LPCTSTR)Rank), 
							_tstoi(strServerIndex),
							inet_addr(T2A((LPTSTR)(LPCTSTR)strIP)),
							bMileage);
		break;
	case MU_TH	:
		m_Client.BuyProduct(_tstoi(strGameCode),
							(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
							_tstoi(strAccount), 
							T2A((LPTSTR)(LPCTSTR)strAccountName), 
							_tstoi(strPackageSeq), 
							_tstoi(strPrice), 
							_tstoi(strZone), 
							_tstoi(strDispalySeq), 
							_tstoi(strClass), 
							_tstoi(strLevel), 
							T2W((LPTSTR)(LPCTSTR)strCharName), 
							T2W((LPTSTR)(LPCTSTR)Rank), 
							_tstoi(strServerIndex),
							inet_addr(T2A((LPTSTR)(LPCTSTR)strIP)),
							T2A((LPTSTR)(LPCTSTR)strRefKey),
							bMileage);
		break;
	case BT_CNL	:
		if (_tstoi(strMemberNo) != 0) {	// 멤버no 가 존재하면 채널링이라고 본다
			if (strAccessIP.GetLength() > 0) {	
				// [AccessIP 추가 버전]
				m_Client.BuyProduct(_tstoi(strGameCode),
									(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
									_tstoi(strAccount), 
									T2A((LPTSTR)(LPCTSTR)strAccountName), 
									_tstoi(strPackageSeq), 
									_tstoi(strPrice), 
									_tstoi(strZone), 
									_tstoi(strDispalySeq), 
									_tstoi(strClass), 
									_tstoi(strLevel), 
									T2W((LPTSTR)(LPCTSTR)strCharName), 
									T2W((LPTSTR)(LPCTSTR)Rank), 
									_tstoi(strServerIndex),
									_tstoi(strIP), 
									T2A((LPTSTR)(LPCTSTR)strRefKey),
									bMileage,
									T2W((LPTSTR)(LPCTSTR)strMemberNo),
									inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));
			} else {
				// [deprecated] - [AccessIP 추가 버전] 사용 권장
				m_Client.BuyProduct(_tstoi(strGameCode),
									(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
									_tstoi(strAccount), 
									T2A((LPTSTR)(LPCTSTR)strAccountName), 
									_tstoi(strPackageSeq), 
									_tstoi(strPrice), 
									_tstoi(strZone), 
									_tstoi(strDispalySeq), 
									_tstoi(strClass), 
									_tstoi(strLevel), 
									T2W((LPTSTR)(LPCTSTR)strCharName), 
									T2W((LPTSTR)(LPCTSTR)Rank), 
									_tstoi(strServerIndex),
									_tstoi(strIP), 
									T2A((LPTSTR)(LPCTSTR)strRefKey),
									bMileage,
									T2W((LPTSTR)(LPCTSTR)strMemberNo));
			}
		} else {
			AfxMessageBox(_T("not available BuyProduct - Channeling"));
		}
		break;
	default:
		AfxMessageBox(_T("not available BuyProduct"));
		break;
	}
}
	
void CShopClientDlg::OnBnClickedButGift()
{
	CString strSender, strSendServerIndex, strSenderCharName, strReceiver, 
			strReceiverServerIndex, strReceiverCharName, strMessage, strGameCode, 
			strPackageSeq, strZone, strPrice, strType, strDispalySeq, 
			strAccountName, strIP, strReceiverAccountName, strRefKey, strPaymentType, strCashTypeCode, strMemberNo;
	
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_SENDER, strSender);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_SENDSSERVERINDEX, strSendServerIndex);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_SENDCHAR, strSenderCharName);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_RECEIVER, strReceiver);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_RECSERVERINDEX, strReceiverServerIndex);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_RECCHAR, strReceiverCharName);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_MESSAGE, strMessage);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_PRODUCTCODE, strPackageSeq);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_ZONE, strZone);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_PRICE, strPrice);	
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_PRODUCTNAME, strDispalySeq);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_SENDER_STR, strAccountName);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_IP, strIP);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_RECEIVER_STR, strReceiverAccountName);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_REFKEY, strRefKey);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE, strPaymentType);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_CASHTYPECODE, strCashTypeCode);
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_MEMBERNO, strMemberNo);

	CString strAccessIP;
	GetDlgItemText(IDC_SHOP_EDIT_GIFT_ACCESSIP, strAccessIP);
	
	bool bMileage = false;
	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SHOP_CHK_GIFT_MILEAGE))->GetCheck()) bMileage = true;

	USES_CONVERSION;
	switch(m_dwGame)
	{
	case MU_KR	:
	case BT_KR	:
	case R2_KR	:
	case SUN_KR	:
		if (strAccessIP.GetLength() > 0)	{
			// [AccessIP 추가버전]
			m_Client.GiftProduct(_tstoi(strGameCode),
								 _tstoi(strSender), 
								 _tstoi(strSendServerIndex), 
								 T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
								 _tstoi(strReceiver), 
								 _tstoi(strReceiverServerIndex), 
								 T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
								 T2W((LPTSTR)(LPCTSTR)strMessage), 
								 _tstoi(strPackageSeq), 
								 _tstoi(strPrice), 
								 _tstoi(strZone), 
								 _tstoi(strDispalySeq),
								 inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP))
								 );
		} else {
			// [deprecated] - [AccessIP 추가 버전] 사용 권장
			m_Client.GiftProduct(_tstoi(strGameCode),
								 _tstoi(strSender), 
								 _tstoi(strSendServerIndex), 
								 T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
								 _tstoi(strReceiver), 
								 _tstoi(strReceiverServerIndex), 
								 T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
								 T2W((LPTSTR)(LPCTSTR)strMessage), 
								 _tstoi(strPackageSeq), 
								 _tstoi(strPrice), 
								 _tstoi(strZone), 
								 _tstoi(strDispalySeq));
		}
		break;
	case SUN_GB	:
	case MU_GB	:
		// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
		//m_Client.GiftProduct(_tstoi(strSender), 
		//					 T2W((LPCTSTR)strSenderCharName), 
		//					 _tstoi(strSendServerIndex), 
		//					 _tstoi(strReceiver), 
		//					 T2W((LPCTSTR)strReceiverCharName), 
		//					 _tstoi(strReceiverServerIndex), 
		//					 T2W((LPCTSTR)strMessage), 
		//					 _tstoi(strGameCode),
		//					 _tstoi(strPackageSeq), 
		//					 _tstoi(strPrice), 
		//					 _tstoi(strZone), 
		//					 _tstoi(strDispalySeq),
		//					_tstoi(strCashTypeCode));
		//
		// [deprecated] - [DeductMileageFlag & AccessIP 추가 버전] 사용 권장
		//m_Client.GiftProduct(_tstoi(strSender), 
		//					 T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
		//					 _tstoi(strSendServerIndex), 
		//					 _tstoi(strReceiver), 
		//					 T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
		//					 _tstoi(strReceiverServerIndex), 
		//					 T2W((LPTSTR)(LPCTSTR)strMessage), 
		//					 _tstoi(strGameCode),
		//					 _tstoi(strPackageSeq), 
		//					 _tstoi(strPrice), 
		//					 _tstoi(strZone), 
		//					 _tstoi(strDispalySeq),
		//					_tstoi(strCashTypeCode),
		//					bMileage);		//<-- 추가

		// [DeductMileageFlag & AccessIP 추가 버전]
		m_Client.GiftProduct(_tstoi(strSender), 
							 T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
							 _tstoi(strSendServerIndex), 
							 _tstoi(strReceiver), 
							 T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
							 _tstoi(strReceiverServerIndex), 
							 T2W((LPTSTR)(LPCTSTR)strMessage), 
							 _tstoi(strGameCode),
							 _tstoi(strPackageSeq), 
							 _tstoi(strPrice), 
							 _tstoi(strZone), 
							 _tstoi(strDispalySeq),
							_tstoi(strCashTypeCode),
							bMileage,											//<-- 추가
							inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));		//<-- 추가

		break;

	case SUN_JP	:
	case MU_JP	:
	case MU_VN	:
	case MU_TW	:
	case BT_TH	:
	case SUN_RU	:
	case SUN_CN	:
		m_Client.GiftProduct(_tstoi(strGameCode),
							(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
							_tstoi(strSender), 
							_tstoi(strAccountName), 
							T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
							_tstoi(strSendServerIndex), 
							_tstoi(strReceiver), 
							T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
							_tstoi(strReceiverServerIndex), 
							T2W((LPTSTR)(LPCTSTR)strMessage), 
							_tstoi(strPackageSeq), 
							_tstoi(strPrice), 
							_tstoi(strZone), 
							_tstoi(strDispalySeq),
							inet_addr(T2A((LPTSTR)(LPCTSTR)strIP)),
							bMileage);
		break;
	case MU_TH	:
		m_Client.GiftProduct(_tstoi(strGameCode),
							(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
							_tstoi(strSender), 
							T2A((LPTSTR)(LPCTSTR)strAccountName), 
							T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
							_tstoi(strSendServerIndex), 
							_tstoi(strReceiver), 
							T2A((LPTSTR)(LPCTSTR)strReceiverAccountName), 
							T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
							_tstoi(strReceiverServerIndex), 
							T2W((LPTSTR)(LPCTSTR)strMessage), 
							_tstoi(strPackageSeq), 
							_tstoi(strPrice), 
							_tstoi(strZone), 
							_tstoi(strDispalySeq),
							inet_addr(T2A((LPTSTR)(LPCTSTR)strIP)),
							T2A((LPTSTR)(LPCTSTR)strRefKey),
							bMileage);
		break;

	case BT_CNL	:
		if (_tstoi(strMemberNo) != 0) {	// 멤버no 가 존재하면 채널링이라고 본다

			if (strAccessIP.GetLength() > 0) {
				// [AccessIP 추가 버전]
				m_Client.GiftProduct(_tstoi(strGameCode),
									(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
									_tstoi(strSender), 
									T2A((LPTSTR)(LPCTSTR)strAccountName), 
									T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
									_tstoi(strSendServerIndex), 
									_tstoi(strReceiver), 
									T2A((LPTSTR)(LPCTSTR)strReceiverAccountName), 
									T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
									_tstoi(strReceiverServerIndex), 
									T2W((LPTSTR)(LPCTSTR)strMessage), 
									_tstoi(strPackageSeq), 
									_tstoi(strPrice), 
									_tstoi(strZone), 
									_tstoi(strDispalySeq),
									_tstoi(strIP), 
									T2A((LPTSTR)(LPCTSTR)strRefKey),
									bMileage,
									T2W((LPTSTR)(LPCTSTR)strMemberNo),
									inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));
			} else {
				// [deprecated] - [AccessIP 추가 버전] 사용 권장
				m_Client.GiftProduct(_tstoi(strGameCode),
									(ShopProtocol::PaymentType)_tstoi(strPaymentType), 
									_tstoi(strSender), 
									T2A((LPTSTR)(LPCTSTR)strAccountName), 
									T2W((LPTSTR)(LPCTSTR)strSenderCharName), 
									_tstoi(strSendServerIndex), 
									_tstoi(strReceiver), 
									T2A((LPTSTR)(LPCTSTR)strReceiverAccountName), 
									T2W((LPTSTR)(LPCTSTR)strReceiverCharName), 
									_tstoi(strReceiverServerIndex), 
									T2W((LPTSTR)(LPCTSTR)strMessage), 
									_tstoi(strPackageSeq), 
									_tstoi(strPrice), 
									_tstoi(strZone), 
									_tstoi(strDispalySeq),
									_tstoi(strIP), 
									T2A((LPTSTR)(LPCTSTR)strRefKey),
									bMileage,
									T2W((LPTSTR)(LPCTSTR)strMemberNo));
			}
		} else {
			AfxMessageBox(_T("not available GiftProduct - Channeling"));
		}
		break;
	default:
		AfxMessageBox(_T("not available GiftProduct"));
		break;
	}
}


void CShopClientDlg::OnBnClickedButGiftcash()
{
	CString str1, str2, str3, str4, str5, str6, str7, str8, str9;

	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_SENDER, str1);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_SENDSERVERINDEX, str6);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_SENDERCHAR, str7);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_RECEIVER, str2);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_REVSERVERINDEX, str8);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_REVCHAR, str9);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_CASHVALUE, str3);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_GAMECODE, str4);
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_MSG, str5);

	USES_CONVERSION;

	CString strAccessIP;
	GetDlgItemText(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP, strAccessIP);

	if (strAccessIP.GetLength() > 0) {
		// [AccessIP 추가 버전]
		m_Client.GiftCash(_tstoi((LPTSTR)(LPCTSTR)str1), _tstoi(str6), T2W((LPTSTR)(LPCTSTR)str7), _tstoi(str2), _tstoi(str8), 
			T2W((LPTSTR)(LPCTSTR)str9), T2W((LPTSTR)(LPCTSTR)str5), _tstof(str3), _tstoi(str4), inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));
	} else {
		// [deprecated] - [AccessIP 추가 버전] 사용 권장
		m_Client.GiftCash(_tstoi((LPTSTR)(LPCTSTR)str1), _tstoi(str6), T2W((LPTSTR)(LPCTSTR)str7), _tstoi(str2), _tstoi(str8), 
			T2W((LPTSTR)(LPCTSTR)str9), T2W((LPTSTR)(LPCTSTR)str5), _tstof(str3), _tstoi(str4));
	}
}

void CShopClientDlg::OnBnClickedButInquireCheck()
{
	CString str1, str2;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_CHECK_ACCOUNT, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_CHECK_GAMECODE, str2);

	m_Client.InquireBuyGiftPossibility(_tstoi(str1), _tstoi(str2));
}

void CShopClientDlg::OnBnClickedButInquireProductlist()
{
	MessageBox(_T("지원하지 않음"));
	/*
	CString str1;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_PRODUCTLIST_SalesZone, str1);

	m_Client.InquireProductList(_tstoi(str1));
	*/
}

void CShopClientDlg::OnBnClickedButInquireEventproductlist()
{	
	CString str1, str2, str3, str4;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_GAMECODE, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_ACCOUNT, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_SALESZONE, str3);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_SEQ, str4);

	m_Client.InquireEventProductList(_tstoi(str1), _tstoi(str2), _tstoi(str3), _tstoi(str4));
}

void CShopClientDlg::OnBnClickedButInquireStorage()
{
	CString str1, str2, str3;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGE_ACCOUNT, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGE_GAMECODE, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGE_SalesZone, str3);

	// [deprecated] - InquireStorageListEx() 사용 권장
	m_Client.InquireStorageList(_tstoi(str1), _tstoi(str2), _tstoi(str3));
}

void CShopClientDlg::OnBnClickedButInquireStoragePage()
{
	CString str1, str2, str3, str4, str5, str6;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_ACCOUNT, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_GAMECODE, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_SalesZone, str3);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_STORAGETYPE, str4);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_NOWPAGE, str5);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_PAGESIZE, str6);

	bool bChk= false;
	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SHOP_CHK_INQUIRE_STORAGEPAGE_NOGIFTMSG))->GetCheck()) 
		bChk = true;

	// [deprecated]
	// InquireStorageListPageEx() 사용 권장
	// InquireStorageListPageNoGiftMessageEx() 사용 권장
	USES_CONVERSION;
	if (bChk) {
		m_Client.InquireStorageListPageNoGiftMessage(_tstoi(str1), _tstoi(str2), _tstoi(str3),
			T2A((LPTSTR)(LPCTSTR)str4)[0], _tstoi(str5), _tstoi(str6));
	} else {
		m_Client.InquireStorageListPage(_tstoi(str1), _tstoi(str2), _tstoi(str3),
			T2A((LPTSTR)(LPCTSTR)str4)[0], _tstoi(str5), _tstoi(str6));
	}
}

void CShopClientDlg::OnBnClickedButInquireStorageEx()
{
	CString str1, str2, str3;
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGE_ACCOUNT_EX, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGE_GAMECODE_EX, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGE_SalesZone_EX, str3);

	m_Client.InquireStorageListEx(_tstoi(str1), _tstoi(str2), _tstoi(str3));
}

void CShopClientDlg::OnBnClickedButInquireStoragePageEx()
{
	CString str1, str2, str3, str4, str5, str6;
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_ACCOUNT_EX, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_GAMECODE_EX, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_SalesZone_EX, str3);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_STORAGETYPE_EX, str4);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_NOWPAGE_EX, str5);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_PAGESIZE_EX, str6);

	bool bChk= false;
	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SHOP_CHK_INQUIRE_STORAGEPAGE_NOGIFTMSG_EX))->GetCheck()) 
		bChk = true;

	USES_CONVERSION;
	if(bChk) {
		m_Client.InquireStorageListPageNoGiftMessageEx(
			_tstoi(str1), _tstoi(str2), _tstoi(str3), T2A((LPTSTR)(LPCTSTR)str4)[0], _tstoi(str5), _tstoi(str6));
	} else {
		m_Client.InquireStorageListPageEx(
			_tstoi(str1), _tstoi(str2), _tstoi(str3), T2A((LPTSTR)(LPCTSTR)str4)[0], _tstoi(str5), _tstoi(str6));
	}
}

void CShopClientDlg::OnBnClickedButInquireStorageuse()
{
	CString str1, str2, str3, str4, str5, str6, strClass, strLevel, strCharName, Rank, strServerIndex;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_ACCOUNT, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_GAMECODE, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_IP, str3);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_SSEQ, str4);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_PSEQ, str5);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_SEP, str6);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_CLASS, strClass);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_LEVEL, strLevel);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_CHARNAME, strCharName);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_RANK, Rank);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_SERVERINDEX, strServerIndex);
	
	USES_CONVERSION;

	switch(m_dwGame)
	{
	case SUN_GB :
		m_Client.UseStorageEx
			(_tstoi(str1), _tstoi(str2), inet_addr(T2A((LPTSTR)(LPCTSTR)str3)), _tstoi(str4), _tstoi(str5), 
			T2A((LPTSTR)(LPCTSTR)str6)[0], _tstoi(strClass), _tstoi(strLevel), T2W((LPTSTR)(LPCTSTR)strCharName), T2W((LPTSTR)(LPCTSTR)Rank), _tstoi(strServerIndex));
		break;
	case SUN_CN :
		m_Client.UseStorageExDeductMileageFlag
			(_tstoi(str1), _tstoi(str2), inet_addr(T2A((LPTSTR)(LPCTSTR)str3)), _tstoi(str4), _tstoi(str5), 
			T2A((LPTSTR)(LPCTSTR)str6)[0], _tstoi(strClass), _tstoi(strLevel), T2W((LPTSTR)(LPCTSTR)strCharName), T2W((LPTSTR)(LPCTSTR)Rank), _tstoi(strServerIndex));
		break;
	default:
		m_Client.UseStorage
			(_tstoi(str1), _tstoi(str2), inet_addr(T2A((LPTSTR)(LPCTSTR)str3)), _tstoi(str4), _tstoi(str5), 
			T2A((LPTSTR)(LPCTSTR)str6)[0], _tstoi(strClass), _tstoi(strLevel), T2W((LPTSTR)(LPCTSTR)strCharName), T2W((LPTSTR)(LPCTSTR)Rank), _tstoi(strServerIndex));
	}
}

void CShopClientDlg::OnBnClickedButInquireStoragedel()
{
	CString str1, str2, str3, str4, str5;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_ACCOUNT, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_GAMECODE, str2);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_SSEQ, str3);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_PSEQ, str4);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_SEP, str5);
	
	USES_CONVERSION;
	m_Client.ThrowStorage(_tstoi(str1), _tstoi(str2), _tstoi(str3), _tstoi(str4), T2A((LPTSTR)(LPCTSTR)str5)[0]);
}

void CShopClientDlg::OnBnClickedStorageUseRollback()
{
	CString strAccount, strGameCode, strStorageSeq, strStorageItemSeq;

	GetDlgItemText(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_ACCOUNT, strAccount);
	GetDlgItemText(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_SSEQ, strStorageSeq);
	GetDlgItemText(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_PSEQ, strStorageItemSeq);

	USES_CONVERSION;

	m_Client.RollbackUseStorage(_tstoi(strAccount), _tstoi(strGameCode), _tstoi(strStorageSeq), _tstoi(strStorageItemSeq));
}

void CShopClientDlg::OnBnClickedButInquireProductleftcount()
{
	CString str1, str2;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_PRODUCTLEFTCOUNT_GAMECODE, str1);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_PRODUCTLEFTCOUNT, str2);

	m_Client.InquireProductLeftCount(_tstoi(str1), _tstoi(str2));
}

void CShopClientDlg::OnBnClickedButMileageSave()
{
	CString strAccount, GameCode, Type, Section, Point, Class, Level, CharName, Rank, ServerIndex;

	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_ACCOUNT, strAccount);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_GAMECODE, GameCode);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_TYPE, Type);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_SECTION, Section);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_POINT, Point);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_CLASS, Class);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_LEVEL, Level);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_CHARNAME, CharName);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_RANK, Rank);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_SERVERINDEX, ServerIndex);

	USES_CONVERSION;
	
	m_Client.MileageSave(_tstoi(strAccount), _tstoi(GameCode), _tstoi(Type), _tstoi(Section), _tstoi(Point),
		_tstoi(Class), _tstoi(Level), T2W((LPTSTR)(LPCTSTR)CharName), T2W((LPTSTR)(LPCTSTR)Rank), _tstoi(ServerIndex));
}

void CShopClientDlg::OnBnClickedButMileageDudect()
{
	CString Account, GameCode, Type, Section, Point, Class, Level, CharName, Rank, ServerIndex;

	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_ACCOUNT, Account);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_GAMECODE, GameCode);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_TYPE, Type);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_SECTION, Section);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_POINT, Point);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_CLASS, Class);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_LEVEL, Level);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_CHARNAME, CharName);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_RANK, Rank);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGE_DUDECT_SERVERINDEX, ServerIndex);

	USES_CONVERSION;
	
	m_Client.MileageDeduct(_tstoi(Account), _tstoi(GameCode), _tstoi(Type), _tstoi(Section), _tstoi(Point),
		_tstoi(Class), _tstoi(Level), T2W((LPTSTR)(LPCTSTR)CharName), T2W((LPTSTR)(LPCTSTR)Rank), _tstoi(ServerIndex));
}

void CShopClientDlg::OnBnClickedButMileageLiveSave()
{
	CString Account, GameCode, Type, Section;

	GetDlgItemText(IDC_SHOP_EDIT_MILEAGELIVE_ACCOUNT, Account);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGELIVE_GAMECODE, GameCode);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGELIVE_TYPE, Type);
	GetDlgItemText(IDC_SHOP_EDIT_MILEAGELIVE_SECTION, Section);
	
	m_Client.MileageLiveSaveUp(_tstoi(Account), _tstoi(GameCode), _tstoi(Type), _tstoi(Section));
}

void CShopClientDlg::OnBnClickedButItemSerial()
{
	CString Account, GameCode, Type, Section, code;

	GetDlgItemText(IDC_SHOP_EDIT_ITEMSERIAL_ACCOUNT, Account);
	GetDlgItemText(IDC_SHOP_EDIT_ITEMSERIAL_GAMECODE, GameCode);
	GetDlgItemText(IDC_SHOP_EDIT_ITEMSERIAL_SSEQ, Type);
	GetDlgItemText(IDC_SHOP_EDIT_ITEMSERIAL_PSEQ, Section);
	GetDlgItemText(IDC_SHOP_EDIT_ITEMSERIAL_SERIAL, code);

	m_Client.ItemSerialUpdate(_tstoi(Account), _tstoi(GameCode), _tstoi(Type), _tstoi(Section), _tstoi64(code));
}

void CShopClientDlg::OnBnClickedInquirePointPercentage()
{
	CString GameCode, ServerType, AccessType;

	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_GAMECODE, GameCode);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_SERVERCODE, ServerType);
	GetDlgItemText(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_ACCESSTYPE, AccessType);

	m_Client.InquireInGamePointValue(_tstoi(GameCode), _tstoi(ServerType), _tstoi(AccessType));
}

void CShopClientDlg::OnBnClickedCancel()
{
	//
	//	m_Clinet.SetKill() 은 
	//		::PostQueuedCompletionStatus()
	//	로 연결 종료 (IOCP)
	//
	//	m_Clinet.Close() 는
	//		::shutdown(소캣핸들, SD_BOTH);
	//		::closesocket(소켓핸들);
	//	로 연결 종료
	//
	//	여기선 샘플이라 Close() 사용함.
	//

	m_Client.Close();
	OnCancel();
}


void CShopClientDlg::ErrorHandler( DWORD lastError, char* description )
{
	USES_CONVERSION;
	TCHAR* desc = A2T(description);

	CTime t = CTime::GetCurrentTime();   	
	
	CString str;
	str.Format(_T("[%s] %s (%d)\r\n"), t.Format(_T("%Y-%m-%d %H:%M:%S")), desc, lastError);

	TCHAR szModule[MAX_PATH] = {0};
	TCHAR szFilePath[MAX_PATH] = {0};

	::GetModuleFileName( NULL, szModule, MAX_PATH );

	*(_tcsrchr( szModule, '\\' ) ) = 0;

	StringCchPrintf( szFilePath, MAX_PATH, _T("%s\\ClientDialog_ShopClient_NetWork.log"), szModule );

	HANDLE	hFile;	
	DWORD		dwBytesWritten, dwPos;  

	hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  //File 생성 
	if (hFile == INVALID_HANDLE_VALUE)
	{ 
		return;
	}  	
		
	dwPos = SetFilePointer(hFile, 0, NULL, FILE_END); 

	LockFile(hFile, dwPos, 0, dwPos + str.GetLength(), 0); 
	WriteFile(hFile, str.GetBuffer(0), str.GetLength(), &dwBytesWritten, NULL); 
	str.ReleaseBuffer();
	UnlockFile(hFile, dwPos, 0, dwPos + str.GetLength(), 0);     
	CloseHandle(hFile); 
}


void CShopClientDlg::OnCbnSelchangeComboGameType()
{
	helper_->changeComboGameType();
}