#include "stdafx.h"
#include <BillingClient/BillingClientDlg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CBillAboutDlg : public CDialog
{
public:
	CBillAboutDlg() : CDialog(CBillAboutDlg::IDD) {};
	enum { IDD = IDD_BILL_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}
	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CBillAboutDlg, CDialog)
END_MESSAGE_MAP()


CBillingClientDlg::CBillingClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBillingClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_BILL_MAINFRAME);
}

void CBillingClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBillingClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
#ifdef VERSION_VS60
	ON_BN_CLICKED(IDC_BILL_BUT_CONNECT, OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_BILL_BUT_LOGIN, OnBnClickedButLogin)
	ON_BN_CLICKED(IDC_BILL_BUT_LOGOUT, OnBnClickedButLogout)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE, OnBnClickedButInquire)	
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_MULTI, OnBnClickedButInquireMulti)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_PCPOINT, OnBnClickedButInquirePcpoint)
	//ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_DEDUCTTYPE, OnBnClickedButInquireDeducttype)
	ON_BN_CLICKED(IDC_BILL_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_PERSON, OnBnClickedButInquirePerson)

#else
	ON_BN_CLICKED(IDC_BILL_BUT_CONNECT, &CBillingClientDlg::OnBnClickedButConnect)
	ON_BN_CLICKED(IDC_BILL_BUT_LOGIN, &CBillingClientDlg::OnBnClickedButLogin)
	ON_BN_CLICKED(IDC_BILL_BUT_LOGOUT, &CBillingClientDlg::OnBnClickedButLogout)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE, &CBillingClientDlg::OnBnClickedButInquire)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_MULTI, &CBillingClientDlg::OnBnClickedButInquireMulti)
	ON_BN_CLICKED(IDCANCEL, &CBillingClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_PCPOINT, &CBillingClientDlg::OnBnClickedButInquirePcpoint)
	//ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_DEDUCTTYPE, &CBillingClientDlg::OnBnClickedButInquireDeducttype)
	ON_BN_CLICKED(IDC_BILL_BUTTON2, &CBillingClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BILL_BUT_INQUIRE_PERSON, &CBillingClientDlg::OnBnClickedButInquirePerson)

#endif	
END_MESSAGE_MAP()


BOOL CBillingClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_BILL_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_BILL_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	
	SetDlgItemText(IDC_BILL_EDIT_IP, _T("127.0.0.1"));
	SetDlgItemText(IDC_BILL_EDIT_PORT, _T("45612"));
	SetDlgItemText(IDC_BILL_EDIT_LOGIN_COUNT, _T("1"));
	SetDlgItemText(IDC_BILL_EDIT_LOGIN_PC, _T("0"));
	SetDlgItemText(IDC_BILL_EDIT_LOGIN_GAMECODE, _T("115"));

	//클라이언트 초기화
	m_Client.CreateSession(CBillingClientDlg::ErrorHandler); 

	return TRUE;
}

void CBillingClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_BILL_ABOUTBOX) {
		CBillAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CBillingClientDlg::OnPaint()
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

HCURSOR CBillingClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBillingClientDlg::OnBnClickedButConnect()
{	
	CString strIP, strPort;
	
	GetDlgItemText(IDC_BILL_EDIT_IP, strIP);
	GetDlgItemText(IDC_BILL_EDIT_PORT, strPort);
	
	USES_CONVERSION;
	m_Client.Connect(T2A(strIP.GetBuffer(0)), _tstoi(strPort.GetBuffer(0)));
}

void CBillingClientDlg::OnBnClickedButLogin()
{
	//char* strAccountID, DWORD dwUserGUID, DWORD dwIPAddress, DWORD dwRoomGUID, DWORD dwGameCode, DWORD dwServerType
	//접속이 성공한후 로그인 해야한다. 로그인 여부는 OnConnected 메소드를 통해 확인한다.
	CString str, strCount, strPC, strGameCode;
	GetDlgItemText(IDC_BILL_EDIT_LOGIN, str);					// 계정 시작 : 계정 아이디

	GetDlgItemText(IDC_BILL_EDIT_LOGIN_COUNT, strCount);		// 생성할 숫자 : 
	GetDlgItemText(IDC_BILL_EDIT_LOGIN_PC, strPC);				// 피씨방번호 : PC방 GUID
	GetDlgItemText(IDC_BILL_EDIT_LOGIN_GAMECODE, strGameCode);	// 게임코드 : 

	int nID = _tstoi(str.GetBuffer(0));
	int nCount = _tstoi(strCount.GetBuffer(0));
	int nPC = _tstoi(strPC.GetBuffer(0));
	int nGameCode = _tstoi(strGameCode.GetBuffer(0));

	for (int i = 0 ; i < nCount ; i++) {
		m_Client.UserLogin(nID + i, inet_addr("127.0.0.1"), nPC, nGameCode, 1);
	}	
}

void CBillingClientDlg::OnBnClickedButLogout()
{	
	CString str;
	GetDlgItemText(IDC_BILL_EDIT_BILLINGID, str);
	
	m_Client.UserLogout( _tstoi(str.GetBuffer(0)));
}

void CBillingClientDlg::OnBnClickedButInquire()
{
	CString str;
	GetDlgItemText(IDC_BILL_EDIT_INQUIRE, str);

	m_Client.InquireUser( _tstoi(str.GetBuffer(0)));
	/*

	for(int i = 0 ; i < 100000 ; i++)	
	{
		Sleep(1000 * 60);
		m_Client.InquireUser( _tstoi(str.GetBuffer(0)));
	}
	*/
}

void CBillingClientDlg::OnBnClickedButInquireMulti()
{
	CString str, str1;
	GetDlgItemText(IDC_BILL_EDIT_MULTI, str);
	GetDlgItemText(IDC_BILL_EDIT_MULTIACCOUNT, str1);
	
	m_Client.InquireMultiUser( _tstoi(str1.GetBuffer(0)), _tstoi(str.GetBuffer(0)));
}

void CBillingClientDlg::OnBnClickedButInquirePerson()
{
	CString str1, str2;
	GetDlgItemText(IDC_BILL_EDIT_PERSON_ACCOUNT, str1);
	GetDlgItemText(IDC_BILL_EDIT_PERSON_GAMECODE, str2);

	m_Client.InquirePersonDeduct(_tstoi(str1.GetBuffer(0)), _tstoi(str2.GetBuffer(0)));
}

void CBillingClientDlg::OnBnClickedButInquirePcpoint()
{
	CString str1, str2, str3;
	GetDlgItemText(IDC_BILL_EDIT_PCPOINT_ACCOUNT, str1);
	GetDlgItemText(IDC_BILL_EDIT_PCPOINT_PCACCOUNT, str2);
	GetDlgItemText(IDC_BILL_EDIT_PCPOINT_GAMECODE, str3);

	m_Client.InquirePCRoomPoint(_tstoi(str1.GetBuffer(0)), _tstoi(str2.GetBuffer(0)), _tstoi(str3.GetBuffer(0)));
}
//
//void CBillingClientDlg::OnBnClickedButInquireDeducttype()
//{
//	CString str, str1;
//
//	GetDlgItemText(IDC_BILL_EDIT_ACCOUNT, str);
//	GetDlgItemText(IDC_BILL_EDIT_LOGIN_GAMECODE2, str1);	
//
//	m_Client.InquireDeductType(_tstoi(str.GetBuffer(0)), _tstoi(str1.GetBuffer(0)));
//}


void CBillingClientDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Client.Close();
	OnCancel();
}

void CBillingClientDlg::ErrorHandler( DWORD lastError, char* description )
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

	StringCchPrintf( szFilePath, STRSAFE_MAX_CCH, _T("%s\\ClientDialog_BillingClient_NetWork.log"), szModule );

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

void CBillingClientDlg::OnBnClickedButton2()
{
	m_Client.Close();
	//m_Client.SetKill();
}
