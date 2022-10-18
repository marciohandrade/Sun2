#ifndef _CRASHREPORT_H_
#define _CRASHREPORT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <DbgHelp.h>

#ifndef CRASHRPTAPI

// This header file is included by an EXE - export
#define CRASHRPTAPI extern "C" __declspec(dllimport)

#endif

typedef BOOL (CALLBACK *LPCALLBACK) (void);

namespace Crash
{
	CRASHRPTAPI BOOL Install(
		LPCALLBACK lpfnCallbackBefore,
		LPCALLBACK lpfnCallbackAfter,
		int nProjectID,
		LPCTSTR szClientVersion,
		MINIDUMP_TYPE dumpType,
		LPCTSTR szHttpUrl,
		LPCTSTR szFtpUrl,
		int nPort,
		LPCTSTR szId,
		LPCTSTR szPassword,
		int nUploadCount,
		bool bCumulative,
		bool bDeleteDumpfile,
		bool bStackTrace,
		LPCTSTR szDumpfile = NULL,
		bool bWriteCrashLog = false);
	CRASHRPTAPI void Uninstall();
	CRASHRPTAPI BOOL AddFile(LPCTSTR szFilename);
	CRASHRPTAPI LONG HandleException(PVOID pExceptionAddress, DWORD dwExceptionCode);
	CRASHRPTAPI LONG HandleExceptionWithInfo(PEXCEPTION_POINTERS pExInfo);
}
#endif;