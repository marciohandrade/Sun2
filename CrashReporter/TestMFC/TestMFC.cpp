// TestMFC.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CTestMFCApp 생성

CTestMFCApp::CTestMFCApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTestMFCApp 개체입니다.

CTestMFCApp theApp;


// CTestMFCApp 초기화

BOOL CTestMFCApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	bool bResult = Install();
	if (!bResult)
		return FALSE;

	CTestMFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	Crash::Uninstall();

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
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