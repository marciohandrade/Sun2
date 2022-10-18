// TestMFCDlg.cpp : ���� ����
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


// CTestMFCDlg ��ȭ ����
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


// CTestMFCDlg �޽��� ó����

BOOL CTestMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	AddOptions();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTestMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestMFCDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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