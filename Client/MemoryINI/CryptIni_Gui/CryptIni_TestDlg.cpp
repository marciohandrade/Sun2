// CryptIni_TestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "CryptIni_Test.h"
#include "CryptIni_TestDlg.h"
#include ".\cryptini_testdlg.h"
#include "MemoryIni.h"
#include "CryptFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCryptIni_TestDlg 대화 상자



CCryptIni_TestDlg::CCryptIni_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCryptIni_TestDlg::IDD, pParent)
	, m_edtText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCryptIni_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edtText);
}

BEGIN_MESSAGE_MAP(CCryptIni_TestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CCryptIni_TestDlg 메시지 처리기

BOOL CCryptIni_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	DragAcceptFiles();    //드래그 앤 드롭을 지원 활성화.
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CCryptIni_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CCryptIni_TestDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CCryptIni_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCryptIni_TestDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	TCHAR sFilter[] = _T("Ini (*.INI) |*.INI|All Files(*.*)|*.*||");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, sFilter);

	if (dlgFile.DoModal() == IDOK)
	{
		CString filename = dlgFile.GetPathName();

		MemoryIni mini;

		if ( 0 > mini.LoadFromFile((LPTSTR)(LPCTSTR) filename, SimpleXorCryption) )
		{
			AfxMessageBox(_T("파일이 너무 크거나 존재 하지 않습니다"), MB_OK);
		}


		DWORD  written = 0;
		ULONG  tmpsize = MAX_LINE * 1000;
		LPTSTR tmp = (LPTSTR)malloc(tmpsize);
				
		if ( 0 > mini.GetData(tmp, tmpsize, &written) )
		{
			free(tmp);
			AfxMessageBox(_T("읽어오는데 문제가 생겼습니다"), MB_OK);
			return;
		}

		tmp[written-1] = _T('\0');
		StringCchCat( tmp, tmpsize, _T("\r\n\r\n") );

		m_edtText = tmp;
		free(tmp);
        		
		UpdateData(FALSE);
	}
}

void CCryptIni_TestDlg::OnBnClickedButton2()
{
	TCHAR sFilter[] = _T("Ini (*.INI) |*.INI|All Files(*.*)|*.*||");
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY, sFilter);

	if (dlgFile.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		CString filename = dlgFile.GetPathName();


		//	확장자가 .ini가 아니라면 .ini를 붙임
		//

		int iPathExt = filename.GetLength() - (int)_tcslen(_T(".ini"));
		int iFound   = filename.Find(_T(".ini"));

		if ( 0 > iFound || iPathExt != iFound )
		{
			filename = filename + _T(".ini");
		}

	
		MemoryIni mini;
		
		if ( 0 > mini.SetData((LPTSTR)(LPCTSTR)m_edtText, m_edtText.GetLength()) )
		{
			AfxMessageBox(_T("저장하는데 문제가 생겼습니다"), MB_OK);
		}
		
		if ( 0 > mini.SaveToFile((LPTSTR)(LPCTSTR) filename, SimpleXorCryption) )
		{
			AfxMessageBox(_T("저장하는데 문제가 생겼습니다"), MB_OK);
		}

		UpdateData(FALSE);
	}
}

void CCryptIni_TestDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	m_edtText = _T("");
	UpdateData(FALSE);
}

void CCryptIni_TestDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT nFile = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for ( UINT i = 0; i < nFile; i++ )
	{
		UINT  nLength = DragQueryFile(hDropInfo, i, NULL, 0);    
		TCHAR IniPath[MAX_PATH] = {0};

		DragQueryFile(hDropInfo, i, IniPath, nLength + 1);


		MemoryIni mini;	

		if ( 0 > mini.LoadFromFile( IniPath, SimpleXorCryption ) )
		{
			AfxMessageBox(_T("파일이 너무 크거나 존재 하지 않습니다"), MB_OK);
			break;
		}

		DWORD  written = 0;
		ULONG  tmpsize = MAX_LINE * 1000;
		LPTSTR tmp = (LPTSTR)malloc(tmpsize);
						
		if ( 0 > mini.GetData(tmp, tmpsize, &written) )
		{
			free(tmp);
			AfxMessageBox(_T("읽어오는데 문제가 생겼습니다"), MB_OK);
			break;
		}

		tmp[written - 1] = _T('\0');
		m_edtText = tmp;
		free(tmp);
	}

	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

