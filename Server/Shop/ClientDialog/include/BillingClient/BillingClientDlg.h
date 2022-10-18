#pragma once

#include <BillingClient/interface/LibClientSession.h>

class CBillingClientDlg : public CDialog
{
public:
	CBillingClientDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_BILLINGCLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	//네트웍 라이브러리 콜백함수
	static void ErrorHandler( DWORD lastError, char* description );

private:
	CLibClientSession m_Client;

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButConnect();
	afx_msg void OnBnClickedButLogin();
	afx_msg void OnBnClickedButLogout();
	afx_msg void OnBnClickedButInquire();	
	afx_msg void OnBnClickedButInquireMulti();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButInquirePcpoint();
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButInquirePerson();
};
