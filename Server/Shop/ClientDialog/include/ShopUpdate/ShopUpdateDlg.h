
#pragma once


// 관리용으로 사용되는 기능으로 게임 스튜디오에선 전혀 사용할 일이 없다.
// IBS만을 위한 기능.
//
// COM 모듈 등록 :
//		콘솔> regsvr32 ScriptVersionUpdateNotify.dll
//
// COM 모듈 해제 :
//		콘솔> regsvr32 /U ScriptVersionUpdateNotify.dll
//
// COM 모듈 등록시 0x8002801c 에러 발생시 :
//		관리자 권한 콘솔> regsvr32 ScriptVersionUpdateNotify.dll


class CShopUpdateDlg : public CDialog
{
public:
	CShopUpdateDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_SHOPUPDATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnBnClickedButSalesScriptUpdate();
	afx_msg void OnBnClickedButBannerScriptUpdate();
	afx_msg void OnBnClickedButPcRoomIPUpdate();
};
