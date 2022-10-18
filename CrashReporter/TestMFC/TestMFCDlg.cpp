// TestMFCDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TestMFC.h"
#include "TestMFCDlg.h"
#include <vector>

/*
#include "./CrashReporter.h"
#ifdef _DEBUG
#ifdef _DLL
#pragma comment(lib, "./CrashReporter_d.lib")
#else
#pragma comment(lib, "./CrashReporter_ds.lib")
#endif
#else
#ifdef _DLL
#pragma comment(lib, "./CrashReporter_r.lib")
#else
#pragma comment(lib, "./CrashReporter_rs.lib")
#endif
#endif
*/

#include "../Source/CrashReporter.h"
#ifdef _DEBUG
#ifdef _DLL
#pragma comment(lib, "../debug_dll/CrashReporter_d.lib")
#else
#pragma comment(lib, "../debug_static/CrashReporter_ds.lib")
#endif
#else
#ifdef _DLL
#pragma comment(lib, "../release_dll/CrashReporter_r.lib")
#else
#pragma comment(lib, "../release_static/CrashReporter_rs.lib")
#endif
#endif


// CTestMFCDlg 대화 상자
CTestMFCDlg::CTestMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CListBox);
}

BEGIN_MESSAGE_MAP(CTestMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTestMFCDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestMFCDlg 메시지 처리기

BOOL CTestMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	AddOptions();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestMFCDlg::OnPaint()
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
HCURSOR CTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestMFCDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nIndex = m_CListBox.GetCurSel();
	int nCount = m_CListBox.GetCount();
	CString strSelectedText;

	if ((nIndex != LB_ERR) && (nCount > 1))
	{
		std::vector< int > a;
		int b[1];
		int e, f = 0;

		char c[10];
		char d[] = "this is sparta!!";

		switch (nIndex + 1)
		{
			case 1: RaiseException(EXCEPTION_INT_OVERFLOW, 0, 0, NULL); break;
			case 2:
				//RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL); break;
				a[2] = 3;
				break;
			case 3:
				//RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL); break;
				b[1000] = 3;
				break;
			case 4: 
				//RaiseException(EXCEPTION_INT_DIVIDE_BY_ZERO, 0, 0, NULL); break;
				e = 100 / f;
				break;
			case 5:
				strcpy(c, d);
				break;
			case 6:
				_ASSERTE(0);
				break;
			case 7:
				unexpected();
				break;
			default:
				::MessageBox(NULL, _T("Invalid error number"), _T("Error"), MB_OK);
				break;
		}
	}

	OnOK();
}

void CTestMFCDlg::AddOptions()
{
	int i = 0;
	m_CListBox.InsertString(i++, _T("EXCEPTION_INT_OVERFLOW"));
	m_CListBox.InsertString(i++, _T("EXCEPTION_ACCESS_VIOLATION"));
	m_CListBox.InsertString(i++, _T("EXCEPTION_ARRAY_BOUNDS_EXCEEDED"));
	m_CListBox.InsertString(i++, _T("EXCEPTION_INT_DIVIDE_BY_ZERO"));
	m_CListBox.InsertString(i++, _T("Buffer Overrun"));
	m_CListBox.InsertString(i++, _T("Assert"));
	m_CListBox.InsertString(i++, _T("Unexpected"));
}