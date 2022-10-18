#include "stdafx.h"
#include <ShopUpdate/ShopUpdateDlg.h>
#include <ShopUpdate/ShopNotifyImpl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CShopUpdateAboutDlg : public CDialog
{
public:
	CShopUpdateAboutDlg() : CDialog(CShopUpdateAboutDlg::IDD) {};

	enum { IDD = IDD_SHOPUPDATE_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}
	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CShopUpdateAboutDlg, CDialog)
END_MESSAGE_MAP()





CShopUpdateDlg::CShopUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopUpdateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_SHOPUPDATE_MAINFRAME);
}

void CShopUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShopUpdateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP	
#ifdef VERSION_VS60
	ON_BN_CLICKED(IDC_SHOPUPDATE_BUT_SALES_SCRIPT_UPDATE, OnBnClickedButSalesScriptUpdate)
	ON_BN_CLICKED(IDC_SHOPUPDATE_BUT_BANNER_SCRIPT_UPDATE, OnBnClickedButBannerScriptUpdate)
	ON_BN_CLICKED(IDC_SHOPUPDATE_BUT_PCROOM_IP_UPDATE, OnBnClickedButPcRoomIPUpdate)
#else
	ON_BN_CLICKED(IDC_SHOPUPDATE_BUT_SALES_SCRIPT_UPDATE, &CShopUpdateDlg::OnBnClickedButSalesScriptUpdate)
	ON_BN_CLICKED(IDC_SHOPUPDATE_BUT_BANNER_SCRIPT_UPDATE, &CShopUpdateDlg::OnBnClickedButBannerScriptUpdate)
	ON_BN_CLICKED(IDC_SHOPUPDATE_BUT_PCROOM_IP_UPDATE, &CShopUpdateDlg::OnBnClickedButPcRoomIPUpdate)
#endif
END_MESSAGE_MAP()

BOOL CShopUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_SHOPUPDATE_ABOUTBOX & 0xFFF0) == IDM_SHOPUPDATE_ABOUTBOX);
	ASSERT(IDM_SHOPUPDATE_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_SHOPUPDATE_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_SHOPUPDATE_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetDlgItemText(IDC_SHOPUPDATE_EDIT_IP , _T("127.0.0.1"));
	SetDlgItemText(IDC_SHOPUPDATE_EDIT_PORT , _T("45613"));

	return TRUE;
}

void CShopUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_SHOPUPDATE_ABOUTBOX) {
		CShopUpdateAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CShopUpdateDlg::OnPaint()
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

HCURSOR CShopUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShopUpdateDlg::OnBnClickedButSalesScriptUpdate()
{
	CString strIP, strPort, strGameCode, strUpdateType, strProcessType, strValue;

	GetDlgItemText(IDC_SHOPUPDATE_EDIT_IP , strIP);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_PORT , strPort);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_GAMECODE , strGameCode);	
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_UPDATETYPE , strUpdateType);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_PROCESSTYPE , strProcessType);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_VALUE , strValue);

	USES_CONVERSION;
	CShopNotifyImpl Notify;	
	bool ret = Notify.SendSalesScriptVersionUpdate(T2W((LPTSTR)(LPCTSTR)strIP), _tstoi(strPort), _tstoi(strGameCode), _tstoi(strUpdateType), _tstoi(strProcessType), _tstoi(strValue));

	if (ret) {
		MessageBox(_T("Success Send Sales Script Version Update"));
	} else {
		MessageBox(_T("Fail Send Sales Script Version Update"));
	}
}

void CShopUpdateDlg::OnBnClickedButBannerScriptUpdate()
{
	CString strIP, strPort, strGameCode, strUpdateType, strProcessType, strValue;

	GetDlgItemText(IDC_SHOPUPDATE_EDIT_IP , strIP);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_PORT , strPort);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_GAMECODE , strGameCode);	
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_UPDATETYPE , strUpdateType);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_PROCESSTYPE , strProcessType);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_VALUE , strValue);

	USES_CONVERSION;
	CShopNotifyImpl Notify;
	bool ret = Notify.SendBannerScriptVersionUpdate(T2W((LPTSTR)(LPCTSTR)strIP), _tstoi(strPort), _tstoi(strGameCode), _tstoi(strUpdateType), _tstoi(strProcessType), _tstoi(strValue));

	if (ret) {
		MessageBox(_T("Success Send Banner Script Version Update"));
	} else {
		MessageBox(_T("Fail Send Banner Script Version Update"));
	}
}

void CShopUpdateDlg::OnBnClickedButPcRoomIPUpdate()
{
	CString strIP, strPort, strGameCode, strPCRoomGUID;

	GetDlgItemText(IDC_SHOPUPDATE_EDIT_IP, strIP);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_PORT, strPort);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_GAMECODE3, strGameCode);
	GetDlgItemText(IDC_SHOPUPDATE_EDIT_PCROOMGUID, strPCRoomGUID);

	USES_CONVERSION;
	CShopNotifyImpl Notify;
	bool ret = Notify.SendPCBangIPAddressUpdate(T2W((LPTSTR)(LPCTSTR)strIP), _tstoi(strPort), _tstoi(strGameCode), _tstoi(strPCRoomGUID));

	if (ret) {
		MessageBox(_T("Success Send PCBang IP Update"));
	} else {
		MessageBox(_T("Fail Send PCBang IP Update"));
	}
}
