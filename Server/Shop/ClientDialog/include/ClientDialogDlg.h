#pragma once

#include <vector>

class CClientDialogDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientDialogDlg)

public:
	CClientDialogDlg(CWnd* pParent = NULL);
	virtual ~CClientDialogDlg();

	enum { IDD = IDD_CLIENTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnShopclientDlg();
	afx_msg void OnBnClickedBtnShopclientDlgAr();
	afx_msg void OnBnClickedBtnBillingclientDlg();

private:
	void modaless(CDialog* dlg, UINT id);
	std::vector<CDialog*> dlgs_;
public:
	afx_msg void OnBnClickedBtnShopupdateDlg();
};
