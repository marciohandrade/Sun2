#include "eXtreme/eXtreme.h"
#include <objbase.h>

HANDLE                      handleMutex = INVALID_HANDLE_VALUE;
eXtreme::LPFN_ErrorHandler  lpfnHandler = 0;


void
Finalize()
{
    if(handleMutex != INVALID_HANDLE_VALUE)
        ::CloseHandle(handleMutex);

    ::WSACleanup();
    ::CoUninitialize();


#ifdef  _CRT_DEBUG
    _CrtCheckMemory();
    _CrtDumpMemoryLeaks();
#endif
}

void
DefaultErrorHandler(DWORD /*lastError*/, char* /*desc*/)
{
    //  Nothing to do...
}

bool
eXtreme::Initialize(char* mutexName, LPFN_ErrorHandler handler)
{
    CoInitializeEx(0, COINIT_MULTITHREADED);


    //  lpfnHandler는 Null이 되어서는 안된다.
    if(handler == 0) lpfnHandler = DefaultErrorHandler;
    else             lpfnHandler = handler;

    if(mutexName != 0)
    {
        handleMutex = ::OpenMutex(MUTEX_ALL_ACCESS, 0, mutexName);
        if(handleMutex != 0)
            return false;

        handleMutex = ::CreateMutex(0, 0, mutexName);
    }

    //  Winsock 2.2 초기화
    {
        WSADATA wsa;

        if(::WSAStartup(0x202, &wsa) != 0)
        {
            return false;
        }
    }

#ifdef  _CRT_DEBUG
    int crtFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    crtFlag |= _CRTDBG_LEAK_CHECK_DF;
    crtFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
    crtFlag |= _CRTDBG_CHECK_ALWAYS_DF;

    _CrtSetDbgFlag(crtFlag);
#endif

    atexit(Finalize);
    return true;
}

void
eXtreme::CallbackErrorHandler(DWORD lastError, char* desc)
{
    lpfnHandler(lastError, desc);
}

DWORD
eXtreme::StringToHash(char* target)
{
    DWORD ch = 0;
    DWORD result = 0, xor = 0;

    DWORD len = (DWORD)strlen(target);
    for(DWORD i = 0 ; i < len ; i++)
    {
        ch      = (DWORD)target[i];
        result  = ((result << 8) | ch) % 16777213UL;
        xor ^= ch;
    }

    return (result | (xor << 24));
}

