#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

class CClientDialogApp : public CWinApp
{
public:
	CClientDialogApp();
	virtual BOOL InitInstance();	// override

	DECLARE_MESSAGE_MAP()
};

extern CClientDialogApp theApp;