#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

class CClientDialogApp : public CWinApp
{
public:
	CClientDialogApp();
	virtual BOOL InitInstance();	// override

	DECLARE_MESSAGE_MAP()
};

extern CClientDialogApp theApp;