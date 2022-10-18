#pragma once

#include <ShopClient/interface/ShopEH.h>

class CShopClientDlgUIHelper;

class CShopClientDlg : public CScrollDialog
{
public:
	CShopClientDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_SHOPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	//네트웍 라이브러리 콜백함수
	static void ErrorHandler( DWORD lastError, char* description );	

public: 
	CShopEH				m_Client;

	long				m_dwGame;
	CComboBox			m_GameType;	// 게임 종류 설정하는 콤보박스

private:
	std::auto_ptr<CShopClientDlgUIHelper> helper_;

protected:
	HICON m_hIcon;
	
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
	afx_msg void OnBnClickedButGiftcash();
	afx_msg void OnBnClickedButInquireCheck();
	afx_msg void OnBnClickedButInquireProductlist();
	afx_msg void OnBnClickedButInquireEventproductlist();
	afx_msg void OnBnClickedButInquireStorage();
	afx_msg void OnBnClickedButInquireStoragePage();
	afx_msg void OnBnClickedButInquireStorageEx();
	afx_msg void OnBnClickedButInquireStoragePageEx();
	afx_msg void OnBnClickedButInquireStorageuse();
	afx_msg void OnBnClickedButInquireStoragedel();
	afx_msg void OnBnClickedButInquireProductleftcount();
	afx_msg void OnBnClickedButMileageSave();
	afx_msg void OnBnClickedButMileageDudect();	
	afx_msg void OnBnClickedButMileageLiveSave();
	afx_msg void OnBnClickedButItemSerial();	
	afx_msg void OnBnClickedInquireSalesVersion();
	afx_msg void OnBnClickedInquireBannerVersion();
	afx_msg void OnBnClickedStorageUseRollback();
	afx_msg void OnBnClickedInquirePointPercentage();
	afx_msg void OnCbnSelchangeComboGameType();
};
