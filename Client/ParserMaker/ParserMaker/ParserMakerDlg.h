// ParserMakerDlg.h : ��� ����
//

#pragma once


// CParserMakerDlg ��ȭ ����
class CParserMakerDlg : public CDialog
{
// ����
public:
	CParserMakerDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_PARSERMAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMake();
	afx_msg void OnBnClickedBrowsebutton();
};
