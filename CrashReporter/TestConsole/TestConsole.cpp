// MakeCrash.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include <math.h>
#include <crtdbg.h>

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

TCHAR szFile1[256], szFile2[256], szFile3[256], szFile4[256], szFile5[256];

BOOL WINAPI CrashCallbackBefore()
{
	::MessageBox(NULL, _T("Before handle exception"), _T("Message"), MB_OK);
	/*
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	std::cout << "AddFile: " << Crash::AddFile(szFile1) << std::endl;
	*/
	/*
	Crash::AddFile(szFile1);
	Crash::AddFile(szFile2);
	Crash::AddFile(szFile3);
	Crash::AddFile(szFile4);
	Crash::AddFile(szFile5);
	*/
	return TRUE;
}

BOOL WINAPI CrashCallbackAfter()
{
	::MessageBox(NULL, _T("After handle exception"), _T("Message"), MB_OK);

	return TRUE;
}

void makeException()
{
	char aaaa[10];
	char d[] = "this is sparta!!";

	strcpy(aaaa, d);
}

int _tmain(int argc, _TCHAR* argv[])
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

	_tcsnset_s(szHttpUrl, 256, 0x00, 256);
	_tcsnset_s(szFtpUrl, 256, 0x00, 256);
	_tcsnset_s(szFtpID, 256, 0x00, 256);
	_tcsnset_s(szFtpPW, 256, 0x00, 256);
	_tcsnset_s(szFile1, 256, 0x00, 256);
	_tcsnset_s(szFile2, 256, 0x00, 256);
	_tcsnset_s(szFile3, 256, 0x00, 256);
	_tcsnset_s(szFile4, 256, 0x00, 256);
	_tcsnset_s(szFile5, 256, 0x00, 256);
	_tcsnset_s(szClientVersion, 256, 0x00, 256);
	_tcsnset_s(szText, 256, 0x00, 256);
	_tcsnset_s(szCaption, 256, 0x00, 256);

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

	nFtpPort = GetPrivateProfileInt(_T("Option"), _T("FTP_PORT"), 0, szConfigFile);
	nProjectID = GetPrivateProfileInt(_T("Option"), _T("PROJECT_ID"), 0, szConfigFile);
	nCrash = GetPrivateProfileInt(_T("Option"), _T("CRASH_NO"), 0, szConfigFile);
	nCount = GetPrivateProfileInt(_T("Option"), _T("UPLOAD_COUNT"), 0, szConfigFile);

	_tcsnset_s(szTemp, 256, 0x00, 256);
	GetPrivateProfileString(_T("Option"), _T("CUMULATIVE"), _T(""), szTemp, 256, szConfigFile);
	_stprintf_s(szTemp, 256, _T("%s"), _tcslwr(szTemp));
	if (_tcscmp(szTemp, _T("true")) == 0)
		bCumulative = true;

	_tcsnset_s(szTemp, 256, 0x00, 256);
	GetPrivateProfileString(_T("Option"), _T("DELETE_DUMPFILE"), _T(""), szTemp, 256, szConfigFile);
	_stprintf_s(szTemp, 256, _T("%s"), _tcslwr(szTemp));
	if (_tcscmp(szTemp, _T("true")) == 0)
		bDeleteDumpfile = true;

	_tcsnset_s(szTemp, 256, 0x00, 256);
	GetPrivateProfileString(_T("Option"), _T("STACK_TRACE"), _T(""), szTemp, 256, szConfigFile);
	_stprintf_s(szTemp, 256, _T("%s"), _tcslwr(szTemp));
	if (_tcscmp(szTemp, _T("true")) == 0)
		bStackTrace = true;

	std::wcout << (LPCTSTR)szHttpUrl << std::endl;
	std::wcout << szFtpUrl << std::endl;
	std::wcout << nFtpPort << std::endl;
	std::wcout << szFtpID << std::endl;
	std::wcout << szFtpPW << std::endl;
	std::wcout << nProjectID << std::endl;
	std::wcout << bCumulative << std::endl;
	std::wcout << bDeleteDumpfile << std::endl;
	std::wcout << bStackTrace << std::endl;
	std::wcout << szFile1 << std::endl;
	std::wcout << szFile2 << std::endl;
	std::wcout << szFile3 << std::endl;
	std::wcout << szFile4 << std::endl;
	std::wcout << szFile5 << std::endl;
	std::wcout << nCrash << std::endl;
	std::wcout << szClientVersion << std::endl;

	/*
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
		bStackTrace,
		_T("Readme.txt")))
		return 1;

	__try
	{
		RaiseException(STILL_ACTIVE, 0, 0, NULL);
	}
	__except(Crash::HandleExceptionWithInfo(GetExceptionInformation()))
	{

	}
*/

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
		bStackTrace,
		NULL,
		true ))
		return 1;

	std::vector< int > a;
	int b[1];
	int e, f = 0;

	switch (nCrash)
	{
		case 1: RaiseException(WAIT_IO_COMPLETION, 0, 0, NULL); break;
		case 2: RaiseException(STILL_ACTIVE, 0, 0, NULL); break;
		case 3:
			//RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL); break;
			//std::cout << "EXCEPTION_ACCESS_VIOLATION" << std::endl;
			//a[2] = 3;
			throw(1);
			break;
		case 4:	RaiseException(EXCEPTION_DATATYPE_MISALIGNMENT, 0, 0, NULL); break;
		case 5: RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL); break;
		case 6: RaiseException(EXCEPTION_SINGLE_STEP, 0, 0, NULL); break;
		case 7:
			//RaiseException(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL); break;
			std::cout << "EXCEPTION_ARRAY_BOUNDS_EXCEEDED" << std::endl;
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
			std::cout << "EXCEPTION_INT_DIVIDE_BY_ZERO" << std::endl;
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
			//string 관련
			makeException();
			break;
		case 27:
			std::cout << "Assert example" << std::endl;
			_ASSERTE(0);
			break;
		case 28:
			unexpected();
			break;
		default:
			std::cout << "Invalid error number" << std::endl;
			break;
	}

	Crash::Uninstall();

	return 0;
}