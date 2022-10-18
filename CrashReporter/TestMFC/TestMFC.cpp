// TestMFC.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "TestMFC.h"
#include "TestMFCDlg.h"

/*
#include "./CrashReporter.h"
#ifdef _DEBUG
#ifdef _DLL
#pragma comment(lib, "./CrashReporter_d.lib")
#else
#pragma comment(lib, "./CrashReporter_ds.lib")
#endif
#else
#ifdef _DLL
#pragma comment(lib, "./CrashReporter_r.lib")
#else
#pragma comment(lib, "./CrashReporter_rs.lib")
#endif
#endif
*/

#include "../Source/CrashReporter.h"
#ifdef _DEBUG
#ifdef _DLL
#pragma comment(lib, "../debug_dll/CrashReporter_d.lib")
#else
#pragma comment(lib, "../debug_static/CrashReporter_ds.lib")
#endif
#else
#ifdef _DLL
#pragma comment(lib, "../release_dll/CrashReporter_r.lib")
#else
#pragma comment(lib, "../release_static/CrashReporter_rs.lib")
#endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR szFile1[256], szFile2[256], szFile3[256], szFile4[256], szFile5[256];

BOOL WINAPI CrashCallbackBefore()
{
	/*
	::MessageBox(NULL, _T("Before handle exception"), _T("Message"), MB_OK);
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	*/
	Crash::AddFile(szFile1);
	Crash::AddFile(szFile2);
	Crash::AddFile(szFile3);
	Crash::AddFile(szFile4);
	Crash::AddFile(szFile5);

	return TRUE;
}

BOOL WINAPI CrashCallbackAfter()
{
	//::MessageBox(NULL, _T("After handle exception"), _T("Message"), MB_OK);

	return TRUE;
}

// CTestMFCApp

BEGIN_MESSAGE_MAP(CTestMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestMFCApp ����

CTestMFCApp::CTestMFCApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CTestMFCApp ��ü�Դϴ�.

CTestMFCApp theApp;


// CTestMFCApp �ʱ�ȭ

BOOL CTestMFCApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	bool bResult = Install();
	if (!bResult)
		return FALSE;

	CTestMFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}

	Crash::Uninstall();

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}

bool CTestMFCApp::Install()
{
	TCHAR szHttpUrl[256] = {0, };
	TCHAR szFtpUrl[256] = {0, };
	TCHAR szFtpID[256] = {0, };
	TCHAR szFtpPW[256] = {0, };
	TCHAR szTemp[256] = {0, };
	TCHAR szConfigFile[256] = _T("../config.ini");
	TCHAR szClientVersion[256] = {0, };
	TCHAR szText[256] = {0, };
	TCHAR szCaption[256] = {0, };
	int nFtpPort, nProjectID, nCount;
	bool bCumulative = false;
	bool bDeleteDumpfile = false;
	bool bStackTrace = false;

	_tcsnset(szHttpUrl, 0x00, 256);
	_tcsnset(szFtpUrl, 0x00, 256);
	_tcsnset(szFtpID, 0x00, 256);
	_tcsnset(szFtpPW, 0x00, 256);
	_tcsnset(szFile1, 0x00, 256);
	_tcsnset(szFile2, 0x00, 256);
	_tcsnset(szFile3, 0x00, 256);
	_tcsnset(szFile4, 0x00, 256);
	_tcsnset(szFile5, 0x00, 256);
	_tcsnset(szClientVersion, 256, 0x00);
	_tcsnset(szText, 256, 0x00);
	_tcsnset(szCaption, 256, 0x00);

	GetPrivateProfileString(_T("Option"), _T("HTTP_URL"), _T(""), szHttpUrl, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FTP_URL"), _T(""), szFtpUrl, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FTP_ID"), _T(""), szFtpID, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FTP_PW"), _T(""), szFtpPW, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FILE1"), _T(""), szFile1, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FILE2"), _T(""), szFile2, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FILE3"), _T(""), szFile3, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FILE4"), _T(""), szFile4, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("FILE5"), _T(""), szFile5, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("CLIENT_VERSION"), _T(""), szClientVersion, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("MESSAGE_TEXT"), _T(""), szText, 256, szConfigFile);
	GetPrivateProfileString(_T("Option"), _T("MESSAGE_CAPTION"), _T(""), szCaption, 256, szConfigFile);

	nFtpPort = GetPrivateProfileInt(_T("Option"), _T("FTP_PORT"), 0, szConfigFile);
	nProjectID = GetPrivateProfileInt(_T("Option"), _T("PROJECT_ID"), 0, szConfigFile);
	nCount = GetPrivateProfileInt(_T("Option"), _T("UPLOAD_COUNT"), 0, szConfigFile);

	_tcsnset(szTemp, 0x00, 256);
	GetPrivateProfileString(_T("Option"), _T("CUMULATIVE"), _T(""), szTemp, 256, szConfigFile);
	_stprintf(szTemp, _T("%s"), _tcslwr(szTemp));
	if (_tcscmp(szTemp, _T("true")) == 0)
		bCumulative = true;

	_tcsnset(szTemp, 0x00, 256);
	GetPrivateProfileString(_T("Option"), _T("DELETE_DUMPFILE"), _T(""), szTemp, 256, szConfigFile);
	_stprintf(szTemp, _T("%s"), _tcslwr(szTemp));
	if (_tcscmp(szTemp, _T("true")) == 0)
		bDeleteDumpfile = true;

	_tcsnset_s(szTemp, 256, 0x00, 256);
	GetPrivateProfileString(_T("Option"), _T("STACK_TRACE"), _T(""), szTemp, 256, szConfigFile);
	_stprintf_s(szTemp, 256, _T("%s"), _tcslwr(szTemp));
	if (_tcscmp(szTemp, _T("true")) == 0)
		bStackTrace = true;

	if (!Crash::Install(
		CrashCallbackBefore,
		CrashCallbackAfter,
		nProjectID,
		szClientVersion,
		MiniDumpNormal,
		szHttpUrl,
		szFtpUrl,
		nFtpPort,
		szFtpID,
		szFtpPW,
		nCount,
		bCumulative,
		bDeleteDumpfile,
		bStackTrace))
		return false;

	return true;
}