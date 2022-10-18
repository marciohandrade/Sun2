// TestWinAPI.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "TestWinAPI.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTWINAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	int nCrash = Install();
	if (nCrash < 0)
		return FALSE;

	std::vector< int > a;
	int b[1];
	int e, f = 0;

	char c[10];
	char d[] = "this is sparta!!";

	switch (nCrash)
	{
		case 1: RaiseException(WAIT_IO_COMPLETION, 0, 0, NULL); break;
		case 2: RaiseException(STILL_ACTIVE, 0, 0, NULL); break;
		case 3:
			//RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL); break;
			a[2] = 3;
			break;
		case 4:	RaiseException(EXCEPTION_DATATYPE_MISALIGNMENT, 0, 0, NULL); break;
		case 5: RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL); break;
		case 6: RaiseException(EXCEPTION_SINGLE_STEP, 0, 0, NULL); break;
		case 7:
			//RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL); break;
			b[1000] = 3;
			break;
		case 8: RaiseException(EXCEPTION_FLT_DENORMAL_OPERAND, 0, 0, NULL); break;
		case 9:	RaiseException(EXCEPTION_FLT_DIVIDE_BY_ZERO, 0, 0, NULL); break;
		case 10: RaiseException(EXCEPTION_FLT_INEXACT_RESULT, 0, 0, NULL); break;
		case 11: RaiseException(EXCEPTION_FLT_INVALID_OPERATION, 0, 0, NULL); break;
		case 12: RaiseException(EXCEPTION_FLT_OVERFLOW, 0, 0, NULL); break;
		case 13: RaiseException(EXCEPTION_FLT_STACK_CHECK, 0, 0, NULL); break;
		case 14: RaiseException(EXCEPTION_FLT_UNDERFLOW, 0, 0, NULL); break;
		case 15: 
			//RaiseException(EXCEPTION_INT_DIVIDE_BY_ZERO, 0, 0, NULL); break;
			e = 100 / f;
			break;
		case 16: RaiseException(EXCEPTION_INT_OVERFLOW, 0, 0, NULL); break;
		case 17: RaiseException(EXCEPTION_PRIV_INSTRUCTION, 0, 0, NULL); break;
		case 18: RaiseException(EXCEPTION_IN_PAGE_ERROR, 0, 0, NULL); break;
		case 19: RaiseException(EXCEPTION_ILLEGAL_INSTRUCTION, 0, 0, NULL); break;
		case 20: RaiseException(EXCEPTION_NONCONTINUABLE_EXCEPTION, 0, 0, NULL); break;
		case 21: RaiseException(EXCEPTION_STACK_OVERFLOW, 0, 0, NULL); break;
		case 22: RaiseException(EXCEPTION_INVALID_DISPOSITION, 0, 0, NULL); break;
		case 23: RaiseException(EXCEPTION_GUARD_PAGE, 0, 0, NULL); break;
		case 24: RaiseException(EXCEPTION_INVALID_HANDLE, 0, 0, NULL); break;
		case 25: RaiseException(CONTROL_C_EXIT, 0, 0, NULL); break;
		case 26:
			strcpy(c, d);
			break;
		case 27:
			_ASSERTE(0);
			break;
		case 28:
			unexpected();
			break;
		default:
			::MessageBox(NULL, _T("Invalid error number"), _T("Error"), MB_OK);
			break;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTWINAPI));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWINAPI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTWINAPI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ::MessageBox(hWnd, _T("Message"), _T("Message"), MB_OK);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴��� ���� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int Install()
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
	int nFtpPort, nProjectID, nCrash, nCount;
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
	nCrash = GetPrivateProfileInt(_T("Option"), _T("CRASH_NO"), 0, szConfigFile);
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
		return -1;

	return nCrash;
}