// TestMFC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTestMFCApp:
// �� Ŭ������ ������ ���ؼ��� TestMFC.cpp�� �����Ͻʽÿ�.
//

class CTestMFCApp : public CWinApp
{
public:
	CTestMFCApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	bool Install();
	void GenerateError();
};

extern CTestMFCApp theApp;