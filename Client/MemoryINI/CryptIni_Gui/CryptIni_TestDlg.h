// CryptIni_TestDlg.h : ��� ����
//

#pragma once


// CCryptIni_TestDlg ��ȭ ����
class CCryptIni_TestDlg : public CDialog
{
// ����
public:
	CCryptIni_TestDlg(CWnd* pParent = NULL);	// ǥ�� ������
	
// ��ȭ ���� ������
	enum { IDD = IDD_CRYPTINI_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString m_edtText;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

