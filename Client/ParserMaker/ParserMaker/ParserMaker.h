// ParserMaker.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CParserMakerApp:
// �� Ŭ������ ������ ���ؼ��� ParserMaker.cpp�� �����Ͻʽÿ�.
//

class CParserMakerApp : public CWinApp
{
public:
	CParserMakerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CParserMakerApp theApp;
