#pragma once

#include <ShopClientAR/interface/ShopEH_AR.h>

class CShopClientDlg_AR : public CDialog
{
public:
	CShopClientDlg_AR(CWnd* pParent = NULL);
	enum { IDD = IDD_SHOPARCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	//네트웍 라이브러리 콜백함수
	static void ErrorHandler( DWORD lastError, char* description );	

public: 
	CShopEH_AR m_Client;

protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButConnect();
	afx_msg void OnBnClickedButShopInquire();
	afx_msg void OnBnClickedButShopbuy();
	afx_msg void OnBnClickedButGift();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButShopInquireTW();		// AR_TW : 캐시조회
	afx_msg void OnBnClickedButShopbuyTW();			// AR_TW : 상품구매
};
