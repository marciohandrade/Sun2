// CryptIni_TestDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
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


// CCryptIni_TestDlg ��ȭ ����



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


// CCryptIni_TestDlg �޽��� ó����

BOOL CCryptIni_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	DragAcceptFiles();    //�巡�� �� ����� ���� Ȱ��ȭ.
	
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CCryptIni_TestDlg::OnPaint() 
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
HCURSOR CCryptIni_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCryptIni_TestDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	TCHAR sFilter[] = _T("Ini (*.INI) |*.INI|All Files(*.*)|*.*||");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, sFilter);

	if (dlgFile.DoModal() == IDOK)
	{
		CString filename = dlgFile.GetPathName();

		MemoryIni mini;

		if ( 0 > mini.LoadFromFile((LPTSTR)(LPCTSTR) filename, SimpleXorCryption) )
		{
			AfxMessageBox(_T("������ �ʹ� ũ�ų� ���� ���� �ʽ��ϴ�"), MB_OK);
		}


		DWORD  written = 0;
		ULONG  tmpsize = MAX_LINE * 1000;
		LPTSTR tmp = (LPTSTR)malloc(tmpsize);
				
		if ( 0 > mini.GetData(tmp, tmpsize, &written) )
		{
			free(tmp);
			AfxMessageBox(_T("�о���µ� ������ ������ϴ�"), MB_OK);
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


		//	Ȯ���ڰ� .ini�� �ƴ϶�� .ini�� ����
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
			AfxMessageBox(_T("�����ϴµ� ������ ������ϴ�"), MB_OK);
		}
		
		if ( 0 > mini.SaveToFile((LPTSTR)(LPCTSTR) filename, SimpleXorCryption) )
		{
			AfxMessageBox(_T("�����ϴµ� ������ ������ϴ�"), MB_OK);
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
			AfxMessageBox(_T("������ �ʹ� ũ�ų� ���� ���� �ʽ��ϴ�"), MB_OK);
			break;
		}

		DWORD  written = 0;
		ULONG  tmpsize = MAX_LINE * 1000;
		LPTSTR tmp = (LPTSTR)malloc(tmpsize);
						
		if ( 0 > mini.GetData(tmp, tmpsize, &written) )
		{
			free(tmp);
			AfxMessageBox(_T("�о���µ� ������ ������ϴ�"), MB_OK);
			break;
		}

		tmp[written - 1] = _T('\0');
		m_edtText = tmp;
		free(tmp);
	}

	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

