#include "stdafx.h"
#include "CrashHandler.h"
#include <WinSock2.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <atlimage.h>

#include "zlib.h"
#include "Zipper.h"

#pragma comment(lib,"zlibstat.lib")


#pragma comment(lib,"psapi.lib")
#pragma comment(lib,"ws2_32.lib")


extern CCrashHandler* g_hCrash;

void         UserSystemInfo(TCHAR* DumpName,TCHAR* OutputLogFileName);
void         UserEncryptSystem(TCHAR* Key,int KeyLength,TCHAR* InputData,TCHAR* OutputData);
void         UserEncryptSystem(TCHAR* Key,int KeyLength,TCHAR* InputData,TCHAR* OutputData,int size);
void         OnScreenSave(char* FileName);
//암호화 키값
char KeyEncrypt[21] = "!@#$%^&*())(*&^%$#@!";

DWORD WINAPI InternetOpenUrlThread(LPVOID vThreadParm)
{
    HTTP_PARM* pThreadParm;
    // Initialize local pointer to void pointer passed to thread
    pThreadParm = (HTTP_PARM*)vThreadParm;

    *(pThreadParm->pHttpConnection) = ::InternetOpenUrl(
        *(pThreadParm->pInternetSession),
        pThreadParm->szUrl,
        NULL,
        0,
        INTERNET_FLAG_NO_CACHE_WRITE,
        NULL);

    if (*(pThreadParm->pHttpConnection) == NULL)
        return 1;
    return 0;
}

// Unhandled exception callback set with SetUnhandledExceptionFilter()
// EXCEPTION_EXECUTE_HANDLER		Return from UnhandledExceptionFilter and execute the associated exception handler.
// EXCEPTION_CONTINUE_EXECUTION		Return from UnhandledExceptionFilter and continue execution from the point of the exception.
// EXCEPTION_CONTINUE_SEARCH		Proceed with normal execution of UnhandledExceptionFilter.
LONG WINAPI CustomUnhandledExceptionHandler(PEXCEPTION_POINTERS pExInfo)
{
    return g_hCrash->HandleException(pExInfo);
}

// Invalid parameter handler set with _set_invalid_parameter_handler()
void CustomInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
    // release 모드에서 사용되나 정확한 메시지가 출력이 되지 않아 사용하지 않음
    /*
    // Write errMsg.txt
    FILE *pFile;
    fopen_s(&pFile, "errMsg.txt", "w");

    if (pFile != NULL)
    {
    wprintf(L"Invalid parameter detected in function %s. File: %s Line: %d\n", function, file, line);
    wprintf(L"Expression: %s\n", expression);

    fwprintf(pFile, L"Invalid parameter detected in function %s. File: %s Line: %d\n", function, file, line);
    fwprintf(pFile, L"Expression: %s\n", expression);
    fclose(pFile);
    }

    g_hCrash->AddFile(_T("errMsg.txt"));
    */
}

// Unexpected error handler set with set_unexpected(), set_terminate()
void CustomUnexpectedErrorHandler()
{
    g_hCrash->HandleException(NULL, 0, UNEXPECTED_ERROR);
    exit(-1);
}

// Client-defined reporting function set with _CrtSetReportHook
int __cdecl CustomHook(int nReportType, char* szMsg, int* pnRet)
{
    // Write errMsg.txt
    FILE *pFile;
    fopen_s(&pFile, "ErrMsg.txt", "w");

    if (pFile != NULL)
    {
        fprintf(pFile, "%s", szMsg);
        fclose(pFile);
    }

    g_hCrash->AddFile(_T("ErrMsg.txt"));

    // Handle exception
    switch (nReportType)
    {
    case _CRT_WARN:
        break;
    case _CRT_ERROR:
        g_hCrash->HandleException(NULL, 0, C_RUNTIME_ERROR);
        exit(1);
        break;
    case _CRT_ASSERT: 
        break;
    default:
        break;
    }

    // A retVal value of zero continues execution, a value of 1 starts the debugger.
    if (pnRet)
        *pnRet = 1;

    // If the hook handles the message in question completely, so that no further reporting is required, it should return TRUE.
    // If it returns FALSE, _CrtDbgReport will report the message normally.
    return TRUE;
}

CCrashHandler::CCrashHandler(
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
                             LPCTSTR szDumpfile,
                             bool bWriteCrashLog)
                             : m_lpfnCallbackBefore(NULL), m_lpfnCallbackAfter(NULL), m_nProjectId(nProjectID), m_dumpType(dumpType), m_bCumulative(bCumulative), m_bStackTrace(bStackTrace),
                             m_bDeleteDumpfile(bDeleteDumpfile), m_bCatchException(true), m_bUpload(false), m_bReportSuccess(true), m_nFileCount(0)
{
    // Decide to write or not to write dumpfile
    if (szDumpfile == NULL)
        _stprintf_s(m_szDumpfile, BUFFER_SIZE, _T("CrashReport.dmp"));
    else
    {
        _stprintf_s(m_szDumpfile, BUFFER_SIZE, _T("%s"), szDumpfile);
        m_bCatchException = false;
    }

    // Initialize
    if (bWriteCrashLog)
        m_pLogger = new CLogger();
    else
        m_pLogger = NULL;

    m_pUploader = new CUploader(szFtpUrl, nPort, szId, szPassword, nUploadCount);
    _stprintf_s(m_szClientVersion, BUFFER_SIZE, _T("%s"), szClientVersion);
    _stprintf_s(m_szHttpUrl, BUFFER_SIZE, _T("%s"), szHttpUrl);
    STACKTRACE_FILENAME = _T("Stacktrace.txt");

    GetMacAddress(m_szMacAddress);

    // Save user supplied callback
    if (lpfnCallbackBefore)
        m_lpfnCallbackBefore = lpfnCallbackBefore;
    if (lpfnCallbackAfter)
        m_lpfnCallbackAfter = lpfnCallbackAfter;

    // Set filters
    if (m_bCatchException)
    {
        // Add this handler in the exception callback chain
        m_oldFilter = SetUnhandledExceptionFilter(CustomUnhandledExceptionHandler);

        // Establish a new terminate handler and a new unexpected handler
        m_oldUnexpectedHandler = set_unexpected(CustomUnexpectedErrorHandler);
        m_oldTerminateHandler = set_terminate(CustomUnexpectedErrorHandler);

        /*
        // Set a function to be called when the CRT detects an invalid argument
        // release 모드에서 사용되나 정확한 메시지가 출력이 되지 않아 사용하지 않음
        _invalid_parameter_handler newHandler;
        newHandler = CustomInvalidParameterHandler;
        m_oldHandler = _set_invalid_parameter_handler(newHandler);

        // Install a client-defined reporting function
        // debug mode에서 assert를 잡게되기 때문에 사용하지 않음
        m_oldReportFunction = _CrtSetReportHook(CustomHook);
        */

        WriteCrashLog(_T("Install"));
    }
    else
    {
        WriteCrashLog(_T("Install(without catching exception)"));
    }
}

CCrashHandler::~CCrashHandler()
{
    // Reset exception callback
    if (m_bCatchException)
    {
        if (m_oldFilter)
            SetUnhandledExceptionFilter(m_oldFilter);
        if (m_oldHandler)
            _set_invalid_parameter_handler(m_oldHandler);
        if (m_oldUnexpectedHandler)
            set_unexpected(m_oldUnexpectedHandler);
        if (m_oldTerminateHandler)
            set_terminate(m_oldTerminateHandler);
        if (m_oldReportFunction)
            _CrtSetReportHook(m_oldReportFunction);
    }

    WriteCrashLog(_T("Uninstall"));

    delete m_pLogger;
    delete m_pUploader;
}

void CCrashHandler::Destroyer()
{
    WriteCrashLog(_T("Terminate"));

    delete m_pLogger;
    delete m_pUploader;

    ::DeleteFile(_T("errMsg.txt"));
}

LONG CCrashHandler::HandleException(PEXCEPTION_POINTERS pExInfo)
{
    return HandleException(pExInfo, pExInfo->ExceptionRecord->ExceptionAddress, pExInfo->ExceptionRecord->ExceptionCode);
}

LONG CCrashHandler::HandleException(PEXCEPTION_POINTERS pExInfo, PVOID pExceptionAddress, DWORD dwExceptionCode)
{
    TCHAR szUrl[2048] = {0, };
    TCHAR szParameter[BUFFER_SIZE] = {0, };
    TCHAR szParameters[2048] = {0, };

    // Call user callback function(before handle exception)
    if (m_lpfnCallbackBefore)
    {
        WriteCrashLog(_T("User callback funtion(before) is called"));

        if (!m_lpfnCallbackBefore())
        {
            WriteCrashLog(_T("User callback funtion(before) returns false"));

            Destroyer();
            //return EXCEPTION_CONTINUE_SEARCH;
            return EXCEPTION_EXECUTE_HANDLER;
        }
    }

    WriteCrashLog(_T("Set the exception information"));

    // Set parameters
    _stprintf_s(szParameter, BUFFER_SIZE, _T("%p"), pExceptionAddress);
    SetParameter(szParameters, _T("address"), szParameter);

    GetExceptionCode(dwExceptionCode, szParameter);
    SetParameter(szParameters, _T("code"), szParameter);

    SetParameter(szParameters, _T("dump"), m_szDumpfile);

    SetParameter(szParameters, _T("mac"), m_szMacAddress);

    _stprintf_s(szParameter, BUFFER_SIZE, _T("%d"), m_nProjectId);
    SetParameter(szParameters, _T("id"), szParameter);

    _stprintf_s(szParameter, BUFFER_SIZE, _T("%d"), (m_bCumulative ? 1 : 0));
    SetParameter(szParameters, _T("isCumulative"), szParameter);

    _stprintf_s(szParameter, BUFFER_SIZE, _T("%s"), m_szClientVersion);
    SetParameter(szParameters, _T("clientVersion"), szParameter);

    SetParameter(szParameters, _T("stacktrace"), pExInfo != NULL && m_bStackTrace ? _T("1") : _T("0"));

    LPCTSTR szFilename;
    std::list<LPCTSTR>::iterator itor;
    int i = 0;
    for (itor = m_lstFile.begin(); itor != m_lstFile.end(); itor++)
    {
        _stprintf_s(szParameter, BUFFER_SIZE, _T("file%d"), (i + 1));

        szFilename = _tcsrchr(*itor, '/');
        if (szFilename == NULL)
            szFilename = *itor;
        else
            szFilename++;

        SetParameter(szParameters, szParameter, szFilename);
        i++;
    }

    // Set url
    _stprintf_s(szUrl, 2048, _T("http://%s/Insert.aspx?1%s"), m_szHttpUrl, szParameters);

    // Report exception to DB
    LONG lReturn = ReportException(szUrl);
    if (lReturn > 0)
    {
        WriteCrashLog(_T("Report failed"));
        m_bReportSuccess = false;

        if (lReturn == 2)
        {
            SYSTEMTIME t;
            ::GetLocalTime(&t);

            _stprintf_s(m_szDate, BUFFER_SIZE, _T("%04d-%02d-%02d"), t.wYear, t.wMonth, t.wDay);
            _stprintf_s(m_szTime, BUFFER_SIZE, _T("%02d-%02d-%02d"), t.wHour, t.wMinute, t.wSecond);
        }
    }

    return GenerateErrorReport(pExInfo, pExceptionAddress);
}

void CCrashHandler::GetExceptionCode(DWORD dwExceptionCode, TCHAR strExceptionCode[])
{
    switch (dwExceptionCode)
    {
    case STATUS_WAIT_0:						_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_WAIT_0")); break;
    case STATUS_ABANDONED_WAIT_0:			_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_ABANDONED_WAIT_0")); break;
    case STATUS_USER_APC:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_USER_APC")); break;
    case STATUS_TIMEOUT:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_TIMEOUT")); break;
    case STATUS_PENDING:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_PENDING")); break;
    case DBG_EXCEPTION_HANDLED:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_EXCEPTION_HANDLED")); break;
    case DBG_CONTINUE:						_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_CONTINUE")); break;
    case STATUS_SEGMENT_NOTIFICATION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_SEGMENT_NOTIFICATION")); break;
    case DBG_TERMINATE_THREAD:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_TERMINATE_THREAD")); break;
    case DBG_TERMINATE_PROCESS:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_TERMINATE_PROCESS")); break;
    case DBG_CONTROL_C:						_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_CONTROL_C")); break;
    case DBG_CONTROL_BREAK:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_CONTROL_BREAK")); break;
    case DBG_COMMAND_EXCEPTION:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_COMMAND_EXCEPTION")); break;
    case STATUS_GUARD_PAGE_VIOLATION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_GUARD_PAGE_VIOLATION")); break;
    case STATUS_DATATYPE_MISALIGNMENT:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_DATATYPE_MISALIGNMENT")); break;
    case STATUS_BREAKPOINT:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_BREAKPOINT")); break;
    case STATUS_SINGLE_STEP:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_SINGLE_STEP")); break;
    case DBG_EXCEPTION_NOT_HANDLED:			_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("DBG_EXCEPTION_NOT_HANDLED")); break;
    case STATUS_ACCESS_VIOLATION:			_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_ACCESS_VIOLATION")); break;
    case STATUS_IN_PAGE_ERROR:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_IN_PAGE_ERROR")); break;
    case STATUS_INVALID_HANDLE:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_INVALID_HANDLE")); break;
    case STATUS_NO_MEMORY:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_NO_MEMORY")); break;
    case STATUS_ILLEGAL_INSTRUCTION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_ILLEGAL_INSTRUCTION")); break;
    case STATUS_NONCONTINUABLE_EXCEPTION:	_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_NONCONTINUABLE_EXCEPTION")); break;
    case STATUS_INVALID_DISPOSITION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_INVALID_DISPOSITION")); break;
    case STATUS_ARRAY_BOUNDS_EXCEEDED:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_ARRAY_BOUNDS_EXCEEDED")); break;
    case STATUS_FLOAT_DENORMAL_OPERAND:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_DENORMAL_OPERAND")); break;
    case STATUS_FLOAT_DIVIDE_BY_ZERO:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_DIVIDE_BY_ZERO")); break;
    case STATUS_FLOAT_INEXACT_RESULT:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_INEXACT_RESULT")); break;
    case STATUS_FLOAT_INVALID_OPERATION:	_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_INVALID_OPERATION")); break;
    case STATUS_FLOAT_OVERFLOW:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_OVERFLOW")); break;
    case STATUS_FLOAT_STACK_CHECK:			_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_STACK_CHECK")); break;
    case STATUS_FLOAT_UNDERFLOW:			_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_UNDERFLOW")); break;
    case STATUS_INTEGER_DIVIDE_BY_ZERO:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_INTEGER_DIVIDE_BY_ZERO")); break;
    case STATUS_INTEGER_OVERFLOW:			_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_INTEGER_OVERFLOW")); break;
    case STATUS_PRIVILEGED_INSTRUCTION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_PRIVILEGED_INSTRUCTION")); break;
    case STATUS_STACK_OVERFLOW:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_STACK_OVERFLOW")); break;
    case STATUS_CONTROL_C_EXIT:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_CONTROL_C_EXIT")); break;
    case STATUS_FLOAT_MULTIPLE_FAULTS:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_MULTIPLE_FAULTS")); break;
    case STATUS_FLOAT_MULTIPLE_TRAPS:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_FLOAT_MULTIPLE_TRAPS")); break;
    case STATUS_REG_NAT_CONSUMPTION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_REG_NAT_CONSUMPTION")); break;
    case STATUS_SXS_EARLY_DEACTIVATION:		_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_SXS_EARLY_DEACTIVATION")); break;
    case STATUS_SXS_INVALID_DEACTIVATION:	_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("STATUS_SXS_INVALID_DEACTIVATION")); break;

    case 0xE06D7363:						_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("Microsoft C++ Exception")); break;

    case C_RUNTIME_WARNING:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("C_RUNTIME_WARNING")); break;
    case C_RUNTIME_ERROR:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("C_RUNTIME_ERROR")); break;
    case C_RUNTIME_ASSERTION:				_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("C_RUNTIME_ASSERTION")); break;
    case UNEXPECTED_ERROR:					_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("%s"), _T("UNEXPECTED_ERROR")); break;

    default:								_stprintf_s(strExceptionCode, BUFFER_SIZE, _T("0x%08X"), dwExceptionCode);
    }
}

LONG CCrashHandler::ReportException(LPCTSTR szUrl)
{
    WriteCrashLog(_T("Report the exception"));

    char szBuffer[BUFFER_SIZE] = {0, };
    LONG lReturn = 0;

    if (Submit(szUrl, szBuffer) == 1)
    {
        m_bUpload = true;
        return 2;
    }

    // Upload minidump file is needed
    if (szBuffer[0] == '1')
    {
        WriteCrashLog(_T("Uploading file is needed"));
        m_bUpload = true;
    }
    // Failed to insert log
    else if (szBuffer[0] == '2')
    {
        WriteCrashLog(_T("Database error: Failed to insert exception log"));
        return 2;
    }
    // Failed to insert log and upload minidump file is needed
    else if (szBuffer[0] == '3')
    {
        WriteCrashLog(_T("Database error: Failed to insert exception log2"));
        m_bUpload = true;
        lReturn = 1;
    }
    else if (szBuffer[0] != '0')
    {
        m_bUpload = true;
        return 2;
    }

    char *pDate, *pTime;
    pDate = strtok_s(&szBuffer[2], " ", (char **)&pTime);

#ifdef _UNICODE
    MultiByteToWideChar(CP_ACP, 0, pDate, -1, m_szDate, BUFFER_SIZE);
    MultiByteToWideChar(CP_ACP, 0, pTime, -1, m_szTime, BUFFER_SIZE);
#else
    sprintf_s(m_szDate, BUFFER_SIZE, "%s", pDate);
    sprintf_s(m_szTime, BUFFER_SIZE, "%s", pTime);
#endif

    return lReturn;
}

LONG CCrashHandler::Submit(LPCTSTR szUrl, char szBuffer[])
{
    HINTERNET hInternetSession, hHttpConnection;
    DWORD dwResultLength = 0;
    BOOL bResult = TRUE;

    hInternetSession = ::InternetOpen(
        NULL,
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0);

    if (!hInternetSession)
    {
        WriteCrashLog(_T("InternetOpen() failed"));
        return 1;
    }

    // Create a worker thread 
    HANDLE hThread; 
    DWORD dwThreadID;
    DWORD dwExitCode = 0;
    HTTP_PARM threadParm;
    threadParm.pInternetSession = &hInternetSession;
    threadParm.pHttpConnection = &hHttpConnection;
    threadParm.szUrl = szUrl;

    hThread = CreateThread(
        NULL,			// Pointer to thread security attributes 
        0,				// Initial thread stack size, in bytes 
        InternetOpenUrlThread,	// Pointer to thread function 
        &threadParm,	// The argument for the new thread
        0,				// Creation flags 
        &dwThreadID);	// Pointer to returned thread identifier 

    // Wait for the call to InternetConnect in worker function to complete
    if (WaitForSingleObject(hThread, TIME_OUT) == WAIT_TIMEOUT)
    {
        WriteCrashLog(_T("InternetOpenUrl() is timeout"));
        if (hInternetSession)
            InternetCloseHandle(hInternetSession);
        WaitForSingleObject(hThread, INFINITE);
        InternetCloseHandle(hInternetSession);
        return 1;
    }

    // The state of the specified object (thread) is signaled
    if (!GetExitCodeThread(hThread, &dwExitCode))
    {
        WriteCrashLog(_T("Error on GetExitCode of InternetOpenUrlThread"));
        if (hInternetSession)
            InternetCloseHandle(hInternetSession);
        return 1;
    }

    if (!hHttpConnection || dwExitCode)
    {
        WriteCrashLog(_T("InternetOpenUrl() failed"));
        if (hInternetSession)
            InternetCloseHandle(hInternetSession);
        return 1;
    }

    if (szBuffer != NULL)
    {
        bResult = ::InternetReadFile(
            hHttpConnection,
            szBuffer,
            BUFFER_SIZE - 1,
            &dwResultLength);
    }

    InternetCloseHandle(hHttpConnection);
    InternetCloseHandle(hInternetSession);

    if (bResult == FALSE)
    {
        WriteCrashLog(_T("InternetReadFile() failed"));
        return 1;
    }

    return 0;
}

LONG CCrashHandler::GenerateErrorReport(PEXCEPTION_POINTERS pExInfo, PVOID pExceptionAddress)
{
    TCHAR szServerFilePrefix[BUFFER_SIZE] = {0, };
    TCHAR szServerFilename[BUFFER_SIZE] = {0, };
    BOOL bCreateDumpSuccess = FALSE;

#define SUN_CRASH_NATION_FOLDER
#ifdef SUN_CRASH_NATION_FOLDER
    TCHAR dump_file_name[BUFFER_SIZE] = {0, };
    TCHAR nation_name[BUFFER_SIZE] = "ETC";
    _tcsncpy(dump_file_name, m_szDumpfile, BUFFER_SIZE);
    TCHAR* string_buffer = _tcsstr(_tcslwr(dump_file_name), "sungame_");

    if (string_buffer)
    {
        if (_tcsstr(string_buffer, "jp"))
        {
            _tcscpy(nation_name, "JP");
        }
        else if (_tcsstr(string_buffer, "cn"))
        {
            _tcscpy(nation_name, "CN");
        }
        else if (_tcsstr(string_buffer, "gsp"))
        {
            _tcscpy(nation_name, "GP");
        }
        else if (_tcsstr(string_buffer, "ru"))
        {
            _tcscpy(nation_name, "RU");
        }
        else if(isdigit(*(string_buffer+8)))
        {
            _tcscpy(nation_name, "KR");
        }
        else
        {
            _tcscpy(nation_name, "ETC");
        }
    }
    // Set file path
    _stprintf_s(
        szServerFilePrefix,
        BUFFER_SIZE,
        _T("%d/%s/%s/%p_%s_%s"),
        m_nProjectId,
        m_szDate,
        nation_name,
        pExceptionAddress,
        m_szTime,
        m_szMacAddress);
#else
    // Set file path
    _stprintf_s(
        szServerFilePrefix,
        BUFFER_SIZE,
        _T("%d/%s/%p_%s_%s"),
        m_nProjectId,
        m_szDate,
        pExceptionAddress,
        m_szTime,
        m_szMacAddress);
#endif //SUN_CRASH_NATION_FOLDER

    if (m_bCatchException)
    {
        // Create the file
        HANDLE hFile = CreateFile(
            m_szDumpfile,
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        // Write the minidump to the file
        if (hFile != INVALID_HANDLE_VALUE)
        {
            MINIDUMP_EXCEPTION_INFORMATION eInfo;
            eInfo.ThreadId = GetCurrentThreadId();
            eInfo.ExceptionPointers = pExInfo;
            eInfo.ClientPointers = FALSE;

            WriteCrashLog(_T("Write the minidump to the file"));

            bCreateDumpSuccess = MiniDumpWriteDump(
                GetCurrentProcess(),
                GetCurrentProcessId(),
                hFile,
                m_dumpType,
                pExInfo ? &eInfo : NULL,
                NULL,
                NULL);

            if (bCreateDumpSuccess)
                WriteCrashLog(_T("Succeeded to write the minidump file"));
            else
                WriteCrashLog(_T("Failed to write the minidump file"));
        }
        else
        {
            WriteCrashLog(_T("Failed to create the minidump file"));
        }

        // Close file
        CloseHandle(hFile);
    }

    TCHAR LogFileOutput[256];
    TCHAR BmpFileOutput[256];
    TCHAR JpgFileOutput[256];
    TCHAR ZipFileOutput[256];

    bool bUploadSuccess = true;
    if (bCreateDumpSuccess || !m_bCatchException)
    {
        // Upload minidump file & stacktrace file
        if (m_bUpload)
        {
            // Upload minidump file
            if (IsFile(m_szDumpfile))
            {
                _stprintf_s(szServerFilename, BUFFER_SIZE, _T("%s_%s"), szServerFilePrefix, m_szDumpfile);

                if (!m_pUploader->UploadFTP(m_szDumpfile, szServerFilename))
                {
                    WriteCrashLog(_T("Upload %s:\t%s"), m_szDumpfile, _T("Succeeded"));
                }
                else
                {
                    WriteCrashLog(_T("Upload %s:\t%s"), m_szDumpfile, _T("Failed"));
                    bUploadSuccess = false;
                }
            }
            // Minidump file not exist
            else
            {
                WriteCrashLog(_T("Upload %s:\t%s"), m_szDumpfile, _T("Failed(file not exist)"));
            }

            UserSystemInfo(m_szDumpfile,LogFileOutput);

            wsprintf(BmpFileOutput,"%s.bmp",m_szDumpfile);
            wsprintf(JpgFileOutput,"%s.jpg",m_szDumpfile);
            OnScreenSave(BmpFileOutput);

            CImage pImage;
            pImage.Load(BmpFileOutput);
            pImage.Save(JpgFileOutput,Gdiplus::ImageFormatJPEG);

            FILE* fp = fopen(JpgFileOutput,"rb");
            char* FileMem;
            char* FileMem2;
            DWORD size;
            if(fp)
            {
                fseek(fp,0,SEEK_END);
                size = ftell(fp);

                fseek(fp,0,SEEK_SET);
                FileMem = new char[size];
                FileMem2 = new char[size];

                fread(FileMem,size,1,fp);
                fclose(fp);
            }

            UserEncryptSystem(KeyEncrypt,20,FileMem,FileMem2,size);

            fp = fopen(JpgFileOutput,"wb");
            if(fp)
            {
                fwrite(FileMem2,size,1,fp);
                fclose(fp);
            }

            delete [] FileMem;
            delete [] FileMem2;

            ::DeleteFile(BmpFileOutput);     
        }

        // Delete minidump file
        if (m_bDeleteDumpfile && m_bCatchException)
        {
            ::DeleteFile(m_szDumpfile);
        }

        //  ::DeleteFile(LogFileOutput);
    }
    else
    {
        WriteCrashLog(_T("Failed to write the minidump file"));
    }

    // Call user callback function(after handle exception)
    if (m_lpfnCallbackAfter)
    {
        WriteCrashLog(_T("User callback funtion(after) is called"));
        if (!m_lpfnCallbackAfter())
            WriteCrashLog(_T("User callback funtion(after) returns FALSE"));
    }

    AddFile(JpgFileOutput);
    AddFile(LogFileOutput);

    wsprintf(ZipFileOutput,"%s.zip",m_szDumpfile);

    CZipper zip;
    bool reb = zip.OpenZip(ZipFileOutput, NULL,true);
    if(reb)
    {
        std::list<LPCTSTR>::iterator itor;
        for (itor = m_lstFile.begin(); itor != m_lstFile.end(); itor++)
        {
            if (!IsFile(*itor))
            {
                continue;
            }

            char* isFile = const_cast<char*>((*itor));
            int idx = 0;
            while(isFile[idx] != '\0')
            {
                if (isFile[idx] == '/')
                {
                    isFile[idx] = '\\';
                }
                ++idx;
            }

            bool isZip = zip.AddFileToZip(isFile);

            if(isZip == false)
            {
                MessageBox(NULL,"11111","11111",MB_OK);
            }
        }
        zip.CloseZip();

        m_lstFile.clear();

    }

    int idx = 0;
    while(ZipFileOutput[idx] != '\0')
    {
        if (ZipFileOutput[idx] == '\\')
        {
            ZipFileOutput[idx] = '/';
        }
        ++idx;
    }
    AddFile(ZipFileOutput);

    // Upload additional files
    LPCTSTR szFilename;

    int i = 0;
    std::list<LPCTSTR>::iterator itor;
    for (itor = m_lstFile.begin(); itor != m_lstFile.end(); itor++)
    {
        // Get filename from filepath
        szFilename = _tcsrchr(*itor, '/');
        if (szFilename == NULL)
            szFilename = *itor;
        else
            szFilename++;       

        // File not exist
        if (!IsFile(*itor))
        {
            WriteCrashLog(_T("Upload %s:\t%s"), szFilename, _T("Failed(file not exist)"));
            continue;
        }

        // Upload
        _stprintf_s(szServerFilename, BUFFER_SIZE, _T("%s_%s"), szServerFilePrefix, szFilename);
        if (!m_pUploader->UploadFTP(*itor, szServerFilename))
        {
            WriteCrashLog(_T("Upload %s:\t%s"), szFilename, _T("Succeeded"));
        }
        else
        {
            WriteCrashLog(_T("Upload %s:\t%s"), szFilename, _T("Failed"));
            bUploadSuccess = false;
        }
    }

    if (m_bReportSuccess && bCreateDumpSuccess && bUploadSuccess)
        WriteCrashLog(_T("Succeeded in handling the exception"));
    else
        WriteCrashLog(_T("Failed to handle the exception"));

    if (m_bCatchException)
        Destroyer();

    ::DeleteFile(LogFileOutput);
    ::DeleteFile(JpgFileOutput);

    return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CCrashHandler::AddFile(LPCTSTR szFilename)
{
    if (10 <= m_nFileCount)
        return false;

    m_lstFile.push_back(szFilename);
    m_nFileCount++;

    return true;
}

bool CCrashHandler::IsFile(LPCTSTR szFilename)
{
    FILE* pFile;
    _tfopen_s(&pFile, szFilename, _T("rb"));

    if (pFile == NULL)
        return false;

    fclose(pFile);
    return true;
}

void CCrashHandler::SetParameter(TCHAR szParameters[], LPCTSTR szKey, LPCTSTR szValue)
{
    TCHAR szParameter[BUFFER_SIZE] = {0, };

    _stprintf_s(szParameter, BUFFER_SIZE, _T("&%s=%s"), szKey, szValue);
    _tcscat_s(szParameters, 2048, szParameter);
}

void CCrashHandler::GetMacAddress(TCHAR szParameters[])
{
    WriteCrashLog(_T("Get the mac address"));

    PIP_ADAPTER_INFO Info;
    DWORD size;

    ZeroMemory(&Info, sizeof(PIP_ADAPTER_INFO));
    size = sizeof(PIP_ADAPTER_INFO);

    if (GetAdaptersInfo(Info, &size) == ERROR_BUFFER_OVERFLOW)
    {
        Info = (PIP_ADAPTER_INFO)malloc(size);
        GetAdaptersInfo(Info, &size);
    }

    _stprintf_s(szParameters, BUFFER_SIZE, _T("%0.2x-%0.2x-%0.2x-%0.2x-%0.2x-%0.2x"),
        Info->Address[0], Info->Address[1], Info->Address[2], Info->Address[3], Info->Address[4], Info->Address[5]);

    delete Info;
}

bool CCrashHandler::StackTrace(PEXCEPTION_POINTERS pExInfo)
{
    // Open file
    FILE *pFile = NULL;
    _tfopen_s(&pFile, STACKTRACE_FILENAME, _T("w"));
    if (pFile == NULL)
        return false;

    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();
    BOOL ret;

    SymSetOptions(SYMOPT_LOAD_LINES);

    if (SymInitialize(hProcess, 0, TRUE) == TRUE)
    {
        CONTEXT &context = *pExInfo->ContextRecord;

        // Set stackFrame
        STACKFRAME64 stackFrame = { 0, };
        DWORD imageType;
#ifdef _M_IX86
        imageType = IMAGE_FILE_MACHINE_I386;
        stackFrame.AddrPC.Offset = context.Eip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context.Ebp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context.Esp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
        imageType = IMAGE_FILE_MACHINE_AMD64;
        stackFrame.AddrPC.Offset = context.Rip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context.Rsp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context.Rsp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
        imageType = IMAGE_FILE_MACHINE_IA64;
        stackFrame.AddrPC.Offset = context.StIIP;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context.IntSp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrBStore.Offset = context.RsBSP;
        stackFrame.AddrBStore.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context.IntSp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

        for (int i = 0; i < MAX_STACKTRACE && stackFrame.AddrPC.Offset; i++)
        {
            ret = StackWalk64(imageType, hProcess, hThread, &stackFrame, &context, 0, 0, 0, 0);

            if (ret != FALSE)
            {
                DWORD64 dwAddress;
                DWORD dwDisplacement;

                ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
                PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

                pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
                pSymbol->MaxNameLen = MAX_SYM_NAME;

                // Get Information
                ret = SymFromAddr(hProcess, stackFrame.AddrPC.Offset, &dwAddress, pSymbol);
                if (ret == TRUE)
                {
                    IMAGEHLP_LINE64	errorLine = { sizeof(IMAGEHLP_LINE64) };
                    IMAGEHLP_LINE64	symbolLine = { sizeof(IMAGEHLP_LINE64) };

                    dwDisplacement = 0;
                    BOOL ret1, ret2;

                    ret1 = SymGetLineFromAddr64(hProcess, stackFrame.AddrPC.Offset, &dwDisplacement, &errorLine);
                    ret2 = SymGetLineFromAddr64(hProcess, stackFrame.AddrPC.Offset - (DWORD)dwAddress, &dwDisplacement, &symbolLine);
                    if (ret1 == TRUE && ret2 == TRUE)
                    {
                        fprintf(pFile, "%s(): %d line (%#x)\r\n", pSymbol->Name, errorLine.LineNumber - symbolLine.LineNumber, pSymbol->Address );

                        fprintf(pFile, "\t%s: %d line\r\n", errorLine.FileName, errorLine.LineNumber );
                    }
                    else if (ret1 == TRUE)
                        fprintf(pFile, "\t%s: %d line\r\n", errorLine.FileName, errorLine.LineNumber );
                    else if (ret2 == TRUE)
                        fprintf(pFile, "%s(): %d line(%#x)\r\n", pSymbol->Name, symbolLine.LineNumber, pSymbol->Address);
                    else
                        fprintf(pFile, "%s() (%#x)\r\n", pSymbol->Name, pSymbol->Address);
                }
                else
                {
                    fprintf(pFile, "StackWalk failed: %d\r\n", ::GetLastError());
                    break;
                }
            }
        }
    }

    SymCleanup(hProcess);

    fclose(pFile);

    return true;
}

void CCrashHandler::WriteCrashLog(LPCTSTR szFormat, ...)
{
    if (m_pLogger != NULL)
    {
        TCHAR szLine[256];
        va_list args;
        va_start(args, szFormat);
        _vstprintf_s(szLine, szFormat, args);

        m_pLogger->Write(szLine);
    }
}

void UserSystemInfo(TCHAR* DumpName,TCHAR* OutputLogFileName)
{
    //dumpname.log파일 시작점.
    //수집 정보 목록
    /*
    총 메모리.
    가상 메모리.
    물리 메모리.
    IP정보
    유저 컴퓨터 이름
    cpu갯수
    프로세스 종류 32,64bit
    그래픽카드
    */
    long totalmem = 0;
    long virtualmem = 0;
    long physcalmem = 0;
    long UserPersent = 0;
    TCHAR userIPAddr[20];
    memset(userIPAddr,0,sizeof(TCHAR)*20);
    TCHAR userLocalComName[256];
    memset(userLocalComName,0,sizeof(TCHAR)*256);
    TCHAR ProcessorArchitect[256];
    memset(ProcessorArchitect,0,sizeof(TCHAR)*256);
    TCHAR CPUMHz[256];
    memset(CPUMHz,0,sizeof(TCHAR)*256);
    TCHAR CPUCount[256];
    memset(CPUCount,0,sizeof(TCHAR)*256);
    TCHAR CPUModel[256];
    memset(CPUModel,0,sizeof(TCHAR)*256);

    TCHAR VGA[256];

    //임시 데이터 변환
    TCHAR		m_strTemp[256];		// 데이터 임시 변환용
    DWORD		m_dwTempSize;		// 데이터 임시 변환용의 크기	
    m_dwTempSize = (DWORD)sizeof(m_strTemp);

    //----------------------------------------------------------------------------------------------
    MEMORYSTATUS memstatus;

    memset(&memstatus,0,sizeof(MEMORYSTATUS));
    memstatus.dwLength = sizeof(memstatus);

    GlobalMemoryStatus(&memstatus);

    //메바이트 단위로 계산
    totalmem = (memstatus.dwTotalPhys / 1024)/1024;
    virtualmem = (memstatus.dwAvailVirtual / 1024)/1024;
    physcalmem = (memstatus.dwAvailPhys / 1024)/1024;
    UserPersent = memstatus.dwMemoryLoad;
    //로컬 메모리 계산 끝
    //----------------------------------------------------------------------------------------------
    //유저 IP정보 수집
    bool userIPData = true;
    TCHAR LocalName[256];

    WSADATA userWsaData;
    memset(&userWsaData,0,sizeof(WSADATA));

    if (WSAStartup(MAKEWORD(2,2),&userWsaData) != 0)
    {
        //윈속을 시작 못했다면 유저 IP정보를 수집 할수 없음.
        userIPData = false;
    }

    //유저 IP data
    if (userIPData)
    {

        if(gethostname(LocalName,256) != SOCKET_ERROR)
        {
            sprintf(userLocalComName,"%s",LocalName);
            HOSTENT* ptr = gethostbyname(LocalName);
            IN_ADDR addr;

            int i = 0;
            if (ptr != NULL)
            {
                while(ptr->h_addr_list[i] != NULL)
                {
                    memcpy(&addr,ptr->h_addr_list[i],ptr->h_length);
                    sprintf(userIPAddr,"%s",inet_ntoa(addr));

                    i++;

                }
            }
        }
        WSACleanup();
    }
    //유저 IP data 정보 수집 끝남.
    //----------------------------------------------------------------------------------------------
    //유저 시스템 정보 (CPU, window 정보)
    //유저의 레지스트리에 접근.
    HKEY hKey;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE ,"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",	0 ,KEY_QUERY_VALUE ,&hKey);
    RegQueryValueEx(hKey, "ProcessorNameString",  NULL, NULL, (LPBYTE)&m_strTemp, &m_dwTempSize);
    sprintf(ProcessorArchitect,"%s",m_strTemp);

    SYSTEM_INFO	sys;
    ZeroMemory(&sys, sizeof(SYSTEM_INFO));
    GetSystemInfo(&sys);
    switch(sys.dwNumberOfProcessors)
    {
    case 1:		sprintf(CPUCount,"싱글 코어"); break;
    case 2:		sprintf(CPUCount,"듀얼 코어"); break;
    case 4:		sprintf(CPUCount,"쿼드 코어"); break;
    case 8:		sprintf(CPUCount,"8 코어"); break;
    default:	sprintf(CPUCount,"%s 코어",sys.dwNumberOfProcessors); break;
    }

    long cpu_clock;
    DWORD dwSize = sizeof(long);
    RegQueryValueEx(hKey, TEXT("~MHz"),  NULL, NULL, (LPBYTE)&cpu_clock, &dwSize);
    sprintf(CPUMHz, "%ld MHz", cpu_clock);

    RegQueryValueEx(hKey, "Identifier",  NULL, NULL, (LPBYTE)&m_strTemp, &m_dwTempSize);
    sprintf(CPUModel, " %s", m_strTemp);

    //레지스터 종료
    RegCloseKey(hKey);

    //유저 시스템 정보 끝
    //----------------------------------------------------------------------------------------------
    //유저 그래픽 카드 정보
    DISPLAY_DEVICE disp;
    ZeroMemory(&disp, sizeof(DISPLAY_DEVICE));
    disp.cb =  sizeof(DISPLAY_DEVICE);

    int nVgaCnt = 0;
    while ( EnumDisplayDevices(NULL, nVgaCnt, &disp, 0) )
    {
        // main만 하게 될 경우 
        if(disp.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)	
        {
            sprintf(VGA,"%s",disp.DeviceString);
        }

        nVgaCnt++;
    }
    //유저 그래픽 카드 정보 끝
    //----------------------------------------------------------------------------------------------
    //수집된 정보를 토대로 임시 파일 만들기

    TCHAR LogFileName[256];
    wsprintf(LogFileName,"%s.log",DumpName);

    //무조건 쓰기모드로!
    FILE* pFile = fopen(LogFileName,"wb");



    if(pFile != NULL)
    {

        char FileText[256] = {"메모리 정보"};
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"총 메모리량: %ldMB",totalmem);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"물리 메모리량: %ldMB",physcalmem);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"가상 메모리량: %ldMB",virtualmem);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"유저메모리 사용량 : %ld%%",UserPersent);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"CPU 정보");
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"CPU 모델: %s",CPUModel);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"CPU MHz: %s",CPUMHz);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"CPU 갯수: %s",CPUCount);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"프로세스 정보: %s",ProcessorArchitect);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"IP 정보");
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"IP Addr: %s",userIPAddr);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"Local Name: %s",userLocalComName);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"VGA 정보");
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        memset(FileText,0,sizeof(char)*256);
        sprintf(FileText,"VGA: %s",VGA);
        UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
        fwrite(FileText,sizeof(char)*256,1,pFile);

        PROCESS_MEMORY_COUNTERS memcounters;
        memset(&memcounters,0,sizeof(memcounters));

        //snapshot정보
        HANDLE	hSnapShot;
        BOOL	bRet = false;
        hSnapShot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);

        TCHAR GameName[256];
        memset(GameName,0,sizeof(TCHAR)*256);
        sprintf(GameName,"%s","Sungame.exe");

        PROCESSENTRY32 pEntry;
        pEntry.dwSize = sizeof(pEntry);

        Process32First (hSnapShot,&pEntry);

        int counter = 0;
        while(1)
        {
            BOOL hRes=Process32Next (hSnapShot,&pEntry);
            if(hRes == FALSE)
                break;

            counter++;
        }

        fwrite(&counter,sizeof(int),1,pFile);

        PROCESSENTRY32 pEntry2;
        pEntry2.dwSize = sizeof(pEntry2);
        Process32First (hSnapShot,&pEntry2);

        while(1)
        {
            BOOL hRes=Process32Next (hSnapShot,&pEntry);
            if(hRes == FALSE)
                break;

            memset(FileText,0,sizeof(char)*256);
            sprintf(FileText,"%s",pEntry.szExeFile);
            UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
            fwrite(FileText,sizeof(char)*256,1,pFile);

            //이건 내 프로세스
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pEntry.th32ProcessID);

            memset(GameName,0,sizeof(TCHAR)*256);
            GetProcessMemoryInfo(hProcess,&memcounters,sizeof(memcounters));

            memset(FileText,0,sizeof(char)*256);
            sprintf(FileText,"%s - UseMem %d",pEntry.szExeFile,memcounters.WorkingSetSize);
            UserEncryptSystem(KeyEncrypt,20,FileText,FileText);
            fwrite(FileText,sizeof(char)*256,1,pFile);

        }


        fclose(pFile);
    }

    sprintf(OutputLogFileName,"%s",LogFileName);
}

void UserEncryptSystem( char* Key,int KeyLength,char* InputData,char* OutputData)
{
    for (int i = 0,j = 0 ; i < strlen(InputData)-1  ; ++i,++j)
    {
        OutputData[i] = InputData[i] ^ Key[j];
        if(j >= KeyLength)
        {
            j = 0;
        }
    }
}

void UserEncryptSystem( TCHAR* Key,int KeyLength,TCHAR* InputData,TCHAR* OutputData,int size )
{
    for (int i = 0,j = 0 ; i <  size ; ++i,++j)
    {
        OutputData[i] = InputData[i] ^ Key[j];
        if(j >= KeyLength)
        {
            j = 0;
        }
    }
}

void	SaveBitmap(char *szFilename,HBITMAP hBitmap)
{
    HDC					hdc=NULL;
    FILE*				fp=NULL;
    LPVOID				pBuf=NULL;
    BITMAPINFO			bmpInfo;
    BITMAPFILEHEADER	bmpFileHeader;

    hdc=GetDC(NULL);
    ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    GetDIBits(hdc,hBitmap,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

    if(bmpInfo.bmiHeader.biSizeImage<=0)
        bmpInfo.bmiHeader.biSizeImage=bmpInfo.bmiHeader.biWidth*abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount+7)/8;

    if((pBuf=malloc(bmpInfo.bmiHeader.biSizeImage))==NULL)
    {
        if(hdc)
        {
            ReleaseDC(NULL,hdc);
        }

        return;
    }

    bmpInfo.bmiHeader.biCompression=BI_RGB;
    GetDIBits(hdc,hBitmap,0,bmpInfo.bmiHeader.biHeight,pBuf,&bmpInfo,DIB_RGB_COLORS);	

    if((fp=fopen(szFilename,"wb"))==NULL)
    {
        if(hdc)
        {
            ReleaseDC(NULL,hdc);
        }

        if(pBuf)
        {
            free(pBuf);
        }
        return;
    }

    bmpFileHeader.bfReserved1=0;
    bmpFileHeader.bfReserved2=0;
    bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
    bmpFileHeader.bfType='MB';
    bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

    fwrite(&bmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);
    fwrite(&bmpInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
    fwrite(pBuf,bmpInfo.bmiHeader.biSizeImage,1,fp);

    if(hdc)
    {
        ReleaseDC(NULL,hdc);
    }

    if(pBuf)
    {
        free(pBuf);
    }

    if(fp)
    {
        fclose(fp);
    }
}

void    OnScreenSave(char* FileName)
{
    int		nWidth=GetSystemMetrics(SM_CXSCREEN);
    int		nHeight=GetSystemMetrics(SM_CYSCREEN);
    HDC     hScreen = GetDC(NULL);

    HDC		hBmpFileDC=CreateCompatibleDC(hScreen);
    HBITMAP	hBmpFileBitmap=CreateCompatibleBitmap(hScreen,nWidth,nHeight);
    HBITMAP hOldBitmap = (HBITMAP) SelectObject(hBmpFileDC,hBmpFileBitmap);
    BitBlt(hBmpFileDC,0,0,nWidth,nHeight,hScreen,0,0,SRCCOPY|CAPTUREBLT);
    SelectObject(hBmpFileDC,hOldBitmap);

    SaveBitmap(FileName,hBmpFileBitmap);

    DeleteDC(hBmpFileDC);
    DeleteObject(hBmpFileBitmap);

}