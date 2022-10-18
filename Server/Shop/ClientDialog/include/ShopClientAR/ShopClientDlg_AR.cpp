
#include "stdafx.h"
#include <ShopClientAR/ShopClientDlg_AR.h>
#include "atlbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CShopAboutDlgAR : public CDialog
{
public:
	CShopAboutDlgAR()  : CDialog(CShopAboutDlgAR::IDD) {};

	enum { IDD = IDD_SHOPAR_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}
	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CShopAboutDlgAR, CDialog)
END_MESSAGE_MAP()


CShopClientDlg_AR::CShopClientDlg_AR(CWnd* pParent /*=NULL*/)
	: CDialog(CShopClientDlg_AR::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_SHOPAR_MAINFRAME);
}

void CShopClientDlg_AR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShopClientDlg_AR, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
#ifdef VERSION_VS60
	ON_BN_CLICKED(IDC_SHOPAR_BUT_CONNECT, OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_SHOPINQUIRE, OnBnClickedButShopInquire)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_SHOPBUY, OnBnClickedButShopbuy)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_GIFT, OnBnClickedButGift)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_TW_SHOPINQUIRE, OnBnClickedButShopInquireTW)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_TW_SHOPBUY, OnBnClickedButShopbuyTW)
	//...

#else
	ON_BN_CLICKED(IDC_SHOPAR_BUT_CONNECT, &CShopClientDlg_AR::OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_SHOPINQUIRE, &CShopClientDlg_AR::OnBnClickedButShopInquire)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_SHOPBUY, &CShopClientDlg_AR::OnBnClickedButShopbuy)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_GIFT, &CShopClientDlg_AR::OnBnClickedButGift)
	ON_BN_CLICKED(IDCANCEL, &CShopClientDlg_AR::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_TW_SHOPINQUIRE, &CShopClientDlg_AR::OnBnClickedButShopInquireTW)
	ON_BN_CLICKED(IDC_SHOPAR_BUT_TW_SHOPBUY, &CShopClientDlg_AR::OnBnClickedButShopbuyTW)	
	//...

#endif
END_MESSAGE_MAP()


BOOL CShopClientDlg_AR::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_SHOPAR_ABOUTBOX & 0xFFF0) == IDM_SHOPAR_ABOUTBOX);
	ASSERT(IDM_SHOPAR_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_SHOPAR_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_SHOPAR_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//
	SetDlgItemText(IDC_SHOPAR_EDIT_IP, _T("127.0.0.1"));
	SetDlgItemText(IDC_SHOPAR_EDIT_PORT, _T("45611"));

	SetDlgItemText(IDC_SHOPAR_EDIT_TW_IC_GAMECODE,	_T("417"));
	SetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_GAMECODE,	_T("417"));

// AR_GB - Test Data
	// 캐시 조회
	SetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_TYPE,		_T("0"));
	SetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_ACCOUNT,		_T("103"));		// 원래는 샵서버에서 AccountID로 인증에 질의하여 얻기 때문에 값을 안넘겨 주는게 맞음
	SetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_GAMECODE,	_T("417"));
	SetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_ACCOUNTID,	_T("103Character"));		
	
	// 상품 구매
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_ACCOUNT,			_T("103"));		// 원래는 샵서버에서 AccountID로 인증에 질의하여 얻기 때문에 값을 안넘겨 주는게 맞음
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_ACCOUNTID,		_T("103Character"));	// 샵서버에서 인증쪽에 질의하여 얻음
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_PRODUCTCODE,		_T("71"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_GAMECODE,		_T("417"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_PURCHASE,		_T("0"));				// InGamePurchaseSeq
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_NAME,			_T("Elite Mana Potion"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_LEVEL,			_T("2"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_CLASS,			_T("1"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_ZONE,			_T("591"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_CHARNAME,		_T("103Character"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_TYPE,			_T("C"));				// AR은 Cash만 사용하는 걸로 알고 있음
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_PRICE,			_T("20"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_SERVERINDEX,		_T("0"));
	SetDlgItemText(IDC_SHOPAR_EDIT_BUY_METHOD,			_T("508"));				// CashTypeCode(캐시 유형 코드)

	// 상품 선물
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SENDER,			_T("103"));		// 원래는 샵서버에서 AccountID로 인증에 질의하여 얻기 때문에 값을 안넘겨 주는게 맞음
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RECEIVER,		_T("104"));		// 원래는 샵서버에서 AccountID로 인증에 질의하여 얻기 때문에 값을 안넘겨 주는게 맞음
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SENDERID,		_T("103Character"));				
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RECEIVERID,		_T("104Character"));				
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_MESSAGE,		_T("GiftMessage"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_GAMECODE,		_T("417"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PRODUCTCODE,	_T("71"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PRODUCTNAME,	_T("Elite Mana Potion"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PURCHASE,		_T("0"));				// InGamePurchaseSeq
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SCLASS,			_T("1"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SLEVEL,			_T("2"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SCHARNAME,		_T("103Character"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SSERVERINDEX,	_T("0"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RSERVERINDEX,	_T("0"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RCLASS,			_T("1"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RLEVEL,			_T("2"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RCHARNAME,		_T("104Character"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_ZONE,			_T("591"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PRICE,			_T("20"));
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_TYPE,			_T("C"));				// AR은 Cash만 사용하는 걸로 알고 있음
	SetDlgItemText(IDC_SHOPAR_EDIT_GIFT_METHOD,			_T("508"));


	m_Client.CreateSession(CShopClientDlg_AR::ErrorHandler); //클라이언트 초기화	

	return TRUE;
}

void CShopClientDlg_AR::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_SHOPAR_ABOUTBOX) {
		CShopAboutDlgAR dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CShopClientDlg_AR::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialog::OnPaint();
	}
}

HCURSOR CShopClientDlg_AR::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShopClientDlg_AR::OnBnClickedButConnect()
{
	CString strIP, strPort;
	
	GetDlgItemText(IDC_SHOPAR_EDIT_IP, strIP);
	GetDlgItemText(IDC_SHOPAR_EDIT_PORT, strPort);
	
	USES_CONVERSION;
	m_Client.Connect(T2A(strIP.GetBuffer(0)), _tstoi(strPort.GetBuffer(0)));
}

void CShopClientDlg_AR::OnBnClickedButShopInquire()
{
	bool bSum = false;

	if(BST_CHECKED == ((CButton*)GetDlgItem(IDC_SHOPAR_CHK_SUM))->GetCheck()) bSum = true;
	
	CString strType, strAccount, strGamecode, strID;

	GetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_TYPE, strType);
	GetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_ACCOUNT, strAccount);
	GetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_GAMECODE, strGamecode);	
	GetDlgItemText(IDC_SHOPAR_EDIT_INQUIRE_ACCOUNTID, strID);	

	CString strCnt;
	GetDlgItemText(IDC_EDIT_CNT, strCnt);
	int cnt = _tstoi(strCnt);
	if (cnt == 0) {
		cnt = 1;
	}
	
	for (int i=0; i<cnt; i++) {
		m_Client.InquireCash(_tstoi(strType), 
							 _tstoi(strAccount), 
							 (LPSTR)(LPCTSTR)strID, 
							 _tstoi(strGamecode), 
							 bSum);
	}
}

void CShopClientDlg_AR::OnBnClickedButShopbuy()
{
	CString strAccount, strGameCode, strProductCode, strProductName, strZone, 
			strPrice, strType, strpu, strClass, strLevel, 
			strCharName, strMothod, strServerIndex, strID, strAccessIP;

	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_ACCOUNT, strAccount);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_PRODUCTCODE, strProductCode);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_NAME, strProductName);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_ZONE, strZone);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_PRICE, strPrice);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_TYPE, strType);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_PURCHASE, strpu);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_CLASS, strClass);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_LEVEL, strLevel);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_CHARNAME, strCharName);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_METHOD, strMothod);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_SERVERINDEX, strServerIndex);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_ACCOUNTID, strID);
	GetDlgItemText(IDC_SHOPAR_EDIT_BUY_ACCESSIP, strAccessIP);

	USES_CONVERSION;

	// [deprecated]
	//m_Client.BuyProduct(_tstoi(strAccount), 
	//					T2A((LPTSTR)(LPCTSTR)strID), 
	//					_tstoi(strGameCode), 
	//					_tstoi(strProductCode), 
	//					T2W((LPTSTR)(LPCTSTR)strProductName), 
	//					_tstoi(strpu), 
	//					_tstoi(strClass), 
	//					_tstoi(strLevel), 
	//					T2W((LPTSTR)(LPCTSTR)strCharName), 
	//					_tstoi(strServerIndex), 
	//					_tstoi(strZone), 
	//					_tstof(strPrice), 
	//					T2A((LPTSTR)(LPCTSTR)strType)[0],
	//					_tstoi(strMothod));

	m_Client.BuyProduct(_tstoi(strAccount), 
						T2A((LPTSTR)(LPCTSTR)strID), 
						_tstoi(strGameCode), 
						_tstoi(strProductCode), 
						T2W((LPTSTR)(LPCTSTR)strProductName), 
						_tstoi(strpu), 
						_tstoi(strClass), 
						_tstoi(strLevel), 
						T2W((LPTSTR)(LPCTSTR)strCharName), 
						_tstoi(strServerIndex), 
						_tstoi(strZone), 
						_tstof(strPrice), 
						T2A((LPTSTR)(LPCTSTR)strType)[0],
						_tstoi(strMothod),
						inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));
}

void CShopClientDlg_AR::OnBnClickedButGift()
{
	
	CString strSender, strReceiver, strMessage, strGameCode, strProductCode, 
			strProductName, strZone, strPrice, strType, strpu, 
			strSClass, strSLevel, strSCharName, strRClass, strRLevel, 
			strRCharName, strMothod, strSServerIndex, strRServerIndex, strSenderID, 
			strRevID, strAccessIP;
	
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SENDER, strSender);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RECEIVER, strReceiver);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_MESSAGE, strMessage);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_GAMECODE, strGameCode);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PRODUCTCODE, strProductCode);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PRODUCTNAME, strProductName);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_ZONE, strZone);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PRICE, strPrice);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_TYPE, strType);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_PURCHASE, strpu);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SCLASS, strSClass);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SLEVEL, strSLevel);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SCHARNAME, strSCharName);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RCLASS, strRClass);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RLEVEL, strRLevel);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RCHARNAME, strRCharName);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_METHOD, strMothod);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SSERVERINDEX, strSServerIndex);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RSERVERINDEX, strRServerIndex);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_SENDERID, strSenderID);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_RECEIVERID, strRevID);
	GetDlgItemText(IDC_SHOPAR_EDIT_GIFT_ACCESSIP, strAccessIP);


	USES_CONVERSION;
	// [deprecated]
	//m_Client.GiftProduct(_tstoi(strSender), 
	//					 (LPSTR)(LPCTSTR)strSenderID, 
	//					 _tstoi(strReceiver), 
	//					 (LPSTR)(LPCTSTR)strRevID, 
	//					 A2W((LPSTR)(LPCTSTR)strMessage), 
	//					 _tstoi(strGameCode), 
	//					 _tstoi(strProductCode), 
	//					 A2W((LPSTR)(LPCTSTR)strProductName), 
	//					 _tstoi(strpu), 
	//					 _tstoi(strSClass), 
	//					 _tstoi(strSLevel), 
	//					 A2W((LPSTR)(LPCTSTR)strSCharName), 
	//					 _tstoi(strSServerIndex), 
	//					 _tstoi(strRClass), 
	//					 _tstoi(strRLevel), 
	//					 A2W((LPSTR)(LPCTSTR)strRCharName), 
	//					 _tstoi(strRServerIndex), 
	//					 _tstoi(strZone), 
	//					 _tstof(strPrice), 
	//					 T2A((LPTSTR)(LPCTSTR)strType)[0],
	//					 _tstoi(strMothod));
	m_Client.GiftProduct(_tstoi(strSender), 
						 (LPSTR)(LPCTSTR)strSenderID, 
						 _tstoi(strReceiver), 
						 (LPSTR)(LPCTSTR)strRevID, 
						 A2W((LPSTR)(LPCTSTR)strMessage), 
						 _tstoi(strGameCode), 
						 _tstoi(strProductCode), 
						 A2W((LPSTR)(LPCTSTR)strProductName), 
						 _tstoi(strpu), 
						 _tstoi(strSClass), 
						 _tstoi(strSLevel), 
						 A2W((LPSTR)(LPCTSTR)strSCharName), 
						 _tstoi(strSServerIndex), 
						 _tstoi(strRClass), 
						 _tstoi(strRLevel), 
						 A2W((LPSTR)(LPCTSTR)strRCharName), 
						 _tstoi(strRServerIndex), 
						 _tstoi(strZone), 
						 _tstof(strPrice), 
						 T2A((LPTSTR)(LPCTSTR)strType)[0],
						 _tstoi(strMothod),
						 inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP)));
}

void CShopClientDlg_AR::OnBnClickedCancel()
{
	m_Client.Close();	// m_Client.SetKIll(); 이 정석임.
	OnCancel();
}

void CShopClientDlg_AR::ErrorHandler( DWORD lastError, char* description )
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

	StringCchPrintf( szFilePath, MAX_PATH, _T("%s\\ClientDialog_ShopClientAR_NetWork.log"), szModule );

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

// AR_TW : 캐시 조회
void CShopClientDlg_AR::OnBnClickedButShopInquireTW()
{	
	CString 
		strGameCode,		// 게임 코드
		strServerIndex,		// 서버 번호
		strAccountID;		// ID

	GetDlgItemText(IDC_SHOPAR_EDIT_TW_IC_GAMECODE,		strGameCode);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_IC_SERVERINDEX,	strServerIndex);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_IC_ACCOUNTID,		strAccountID);

	m_Client.InquireCash(
		_tstoi(strGameCode), 
		 _tstoi(strServerIndex), 
		 (LPSTR)(LPCTSTR)strAccountID
	);
}

// AR_TW : 상품 구매
void CShopClientDlg_AR::OnBnClickedButShopbuyTW()
{
	CString 
		strGameCode,		// 게임 코드
		strServerIndex,		// 서버 번호
		strAccountID,		// ID
		strCharName,		// 캐릭터 이름
		strDeductPrice,		// 차감 캐시
		strItemNo,			// 아이템 번호
		strOrderNo,			// 주문 번호
		strOrderQuantity,	// 주문 수량
		strAccessIP;		// IP 어드레스

	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_GAMECODE,			strGameCode);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_SERVERINDEX,		strServerIndex);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_ACCOUNTID,		strAccountID);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_CHARNAME,			strCharName);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_DEDUCTPRICE,		strDeductPrice);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_ITEMNO,			strItemNo);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_ORDERNO,			strOrderNo);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_ORDERQUANTITY,	strOrderQuantity);
	GetDlgItemText(IDC_SHOPAR_EDIT_TW_BP_ACCESSIP,			strAccessIP);


	USES_CONVERSION;

	m_Client.BuyProduct(
		_tstoi(strGameCode),
		_tstoi(strServerIndex),
		(LPSTR)(LPCTSTR)strAccountID,
		A2W((LPSTR)(LPCTSTR)strCharName),
		_tstoi(strDeductPrice),
		_tstoi64(strItemNo),
		_tstoi64(strOrderNo),
		_tstoi(strOrderQuantity),
		inet_addr(T2A((LPTSTR)(LPCTSTR)strAccessIP))
	);
}
