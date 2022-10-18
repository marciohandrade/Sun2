
#include "stdafx.h"
#include <ClientDialog.h>
#include <ClientDialogDlg.h>
#include <ShopClient/ShopClientDlg.h>
#include <ShopClientAR/ShopClientDlg_AR.h>
#include <BillingClient/BillingClientDlg.h>
#include <ShopUpdate/ShopUpdateDlg.h>



IMPLEMENT_DYNAMIC(CClientDialogDlg, CDialog)

CClientDialogDlg::CClientDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDialogDlg::IDD, pParent)
{}

CClientDialogDlg::~CClientDialogDlg()
{
	// delete
	std::vector<CDialog*>::iterator it = dlgs_.begin();
	for (; it!=dlgs_.end(); ++it) {
		if (*it != NULL) {
			delete *it;
		}
	}
}

void CClientDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientDialogDlg, CDialog)
#ifdef VERSION_VS60
	ON_BN_CLICKED(IDC_BTN_SHOPCLIENT_DLG, OnBnClickedBtnShopclientDlg)
	ON_BN_CLICKED(IDC_BTN_SHOPCLIENT_DLG_AR, OnBnClickedBtnShopclientDlgAr)
	ON_BN_CLICKED(IDC_BTN_BILLINGCLIENT_DLG, OnBnClickedBtnBillingclientDlg)
	ON_BN_CLICKED(IDC_BTN_SHOPUPDATE_DLG, OnBnClickedBtnShopupdateDlg)
#else
	ON_BN_CLICKED(IDC_BTN_SHOPCLIENT_DLG, &CClientDialogDlg::OnBnClickedBtnShopclientDlg)
	ON_BN_CLICKED(IDC_BTN_SHOPCLIENT_DLG_AR, &CClientDialogDlg::OnBnClickedBtnShopclientDlgAr)
	ON_BN_CLICKED(IDC_BTN_BILLINGCLIENT_DLG, &CClientDialogDlg::OnBnClickedBtnBillingclientDlg)
	ON_BN_CLICKED(IDC_BTN_SHOPUPDATE_DLG, &CClientDialogDlg::OnBnClickedBtnShopupdateDlg)
#endif
END_MESSAGE_MAP()

// ShopClient
void CClientDialogDlg::OnBnClickedBtnShopclientDlg()
{
	modaless(new CShopClientDlg(), IDD_SHOPCLIENT_DIALOG);
}

// ShopClient AR
void CClientDialogDlg::OnBnClickedBtnShopclientDlgAr()
{
	modaless(new CShopClientDlg_AR(), IDD_SHOPARCLIENT_DIALOG);
}

// BillingClient
void CClientDialogDlg::OnBnClickedBtnBillingclientDlg()
{
	modaless(new CBillingClientDlg(), IDD_BILLINGCLIENT_DIALOG);
}

// ShopUpdate
void CClientDialogDlg::OnBnClickedBtnShopupdateDlg()
{
	modaless(new CShopUpdateDlg(), IDD_SHOPUPDATE_DIALOG);
}

void CClientDialogDlg::modaless(CDialog* dlg, UINT id)
{
	dlg->Create(id, this);
	dlg->ShowWindow(SW_SHOW);
	dlgs_.push_back(dlg);
}
