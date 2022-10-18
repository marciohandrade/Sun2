
#pragma once


// ���������� ���Ǵ� ������� ���� ��Ʃ������� ���� ����� ���� ����.
// IBS���� ���� ���.
//
// COM ��� ��� :
//		�ܼ�> regsvr32 ScriptVersionUpdateNotify.dll
//
// COM ��� ���� :
//		�ܼ�> regsvr32 /U ScriptVersionUpdateNotify.dll
//
// COM ��� ��Ͻ� 0x8002801c ���� �߻��� :
//		������ ���� �ܼ�> regsvr32 ScriptVersionUpdateNotify.dll


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
