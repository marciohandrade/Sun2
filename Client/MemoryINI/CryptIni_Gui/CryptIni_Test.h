// CryptIni_Test.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CCryptIni_TestApp:
// �� Ŭ������ ������ ���ؼ��� CryptIni_Test.cpp�� �����Ͻʽÿ�.
//

class CCryptIni_TestApp : public CWinApp
{
public:
	CCryptIni_TestApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CCryptIni_TestApp theApp;
