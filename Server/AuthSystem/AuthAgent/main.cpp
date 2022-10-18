#include "stdafx.h"
#include <SolarSimpleConsole.h>
#include "AuthAgentFrame.h"
#include <eXtreme/util/WinService.h>

// Global
SolarSimpleConsole	g_Console;

#include <SolarLog.h>
#include <CrashHandler.h>
#include <SMTPClient.h>

// 덤프 관련 ---------------------------------------------------------------------------------------------------------
void
ProcCrashHandling(LPCTSTR stackWalkLog, LPCTSTR dumpFileName)
{
    FILE*       fp;
    SYSTEMTIME  systemTime;
    TCHAR       temp[127];

    GetLocalTime(&systemTime);
    sprintf(temp, "%s_CrashLog_%d%02d%02d_%02d%02d%02d.txt",
            SERVICE_NAME,
            systemTime.wYear, systemTime.wMonth, systemTime.wDay,
            systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

    fp = fopen(temp, "w");
    fprintf(fp, "Dump file path : %s\r\n\r\n", dumpFileName);
    fprintf(fp, stackWalkLog);

    fclose(fp);

    //  Dump의 내용을 메일로 발송한다.
    {
        SMTPClient	smtp;

        if(smtp.Connect("mail.webzen.co.kr") == TRUE)
        {
            smtp.MailFrom(SERVICE_NAME, "CrashHandler@webzen.co.kr");
            smtp.MailTo("poong@webzen.co.kr");
            smtp.MailContents("Crash Report", stackWalkLog);
            smtp.MailAttachFile(temp);
            smtp.MailAttachFile((LPTSTR)dumpFileName);
            smtp.MailSubmit();
        }
    }

}

LPCTSTR
GenerateDmpName()
{
    static	char		fileName[64];
    SYSTEMTIME	time;

    ::GetLocalTime(&time);
    ::sprintf(fileName, "%s_%d%d%d_%d%d%d.dmp",
              SERVICE_NAME,
              time.wYear, time.wMonth, time.wDay,
              time.wHour, time.wMinute, time.wSecond);

    return fileName;
}

void
ErrorHandler(DWORD lastError, char* desc)
{
    SUNLOG(eCRITICAL_LOG, "Error = %d,  %s\n", lastError, desc);
}

bool
OnServiceStart()
{
    // 콘솔창 초기화
    CONSOLE_DESC cdesc;
    ZeroMemory(&cdesc, sizeof(cdesc));
    cdesc.bUsingConsoleInput	= FALSE;
    cdesc.bUsingRedirect		= TRUE;
    g_Console.Init(&cdesc);

    AuthAgentFrame* pAuthAgentFrame = AuthAgentFrame::Instance();
    return (TRUE == pAuthAgentFrame->DoInit("./AuthAgent.ini",
                                            AuthAgentFrame::DoAllocServerSession,
                                            AuthAgentFrame::DoFreeServerSession,
                                            AuthAgentFrame::DoFreeConnectSession,
                                            AuthAgentFrame::DoAllocClient_NotUsed,
                                            AuthAgentFrame::DoFreeClient_NotUsed,
                                            AuthAgentFrame::DoFreeConnect_NotUsed));
}

void
OnServiceProcess()
{
    AuthAgentFrame::Instance()->DoProcess();
}

bool
OnServiceUpdate()
{
    return (TRUE == AuthAgentFrame::Instance()->DoUpdate());
}

void
OnServiceStop()
{
    AuthAgentFrame::Instance()->DoRelease();
    AuthAgentFrame::DestroyInstance();

    // 콘솔창 종료
    g_Console.Release();
}

int
main(int argc, char* argv[])
{
    SharedMemoryPool::CreateInstance();

    //IfNotDebug()
    {
        TCHAR fileName[1024];
        int len, i;

        ::GetModuleFileName(0, fileName, 1024);
        len = (int)::strlen(fileName);
        for(i = len - 1; i >= 0; --i)
        {
            if(fileName[i] == '\\')
                break;
        }

        fileName[i] = 0;
        SetCurrentDirectory(fileName);
    }

    CrashHandler::Install(true, GenerateDmpName, ProcCrashHandling);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //  For Windows service
    //IfNotDebug()
    //{
    //	char moduleName[1024];

    //	::GetModuleFileName(0, moduleName, 1024);

    //	if(argc > 1)
    //	{
    //		::_tcsupr(argv[1]);

    //		if(::_tcsncmp(argv[1], _T("INSTALL"), 7) == 0)
    //		{
    //			if(argc == 4)
    //				eXtreme::WinService::Instance()->UploadService(
    //				SERVICE_NAME, SERVICE_NAME, moduleName, argv[2], argv[3], true);
    //			else if(argc == 2)
    //				eXtreme::WinService::Instance()->UploadService(
    //				SERVICE_NAME, SERVICE_NAME, moduleName, 0, 0, true);

    //			return 0;
    //		}
    //		else if(::_tcsncmp(argv[1], _T("REMOVE"), 6) == 0)
    //		{
    //			eXtreme::WinService::Instance()->RemoveService(SERVICE_NAME);

    //			return 0;
    //		}
    //	}
    //}

    if(eXtreme::Initialize(0, ErrorHandler) == false)
    {
        SUNLOG(eCRITICAL_LOG, "Already same server running.\n");
        getch();

        return 0;
    }

	//IfDebug()
    {
        OnServiceStart();
        OnServiceProcess();
        OnServiceStop();
    }
    //else
    //{
    //	//  Release의 경우 Background Service로 동작해야한다.
    //	//  프로그램이 종료되기 전 까지 StartDispatcher에서 Block된다.
    //	if(eXtreme::WinService::Instance()->StartDispatcher(
    //		SERVICE_NAME, OnServiceStart, OnServiceStop, OnServiceUpdate) == false)
    //		return 0;

    //}

    return 0;
}

