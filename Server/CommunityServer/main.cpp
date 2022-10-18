#include "WorldServerPch.hxx"

#include <signal.h>

#include <SolarSimpleConsole.h>
#include <SolarDump.h>

#include "WorldServer.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_world.txt"
// @desc 공유된 메모리 누수 출력 파일명
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "./memory_leak_World_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================

//#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Utility_sd.lib")
//#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/ProgramCommon_sd.lib")
//#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarSlot_sd.lib")
//#pragma comment(lib, "../../lib/.lib")
//#pragma comment(lib, "../../lib/.lib")
//#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/Network_d.lib")
//#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/ServerCommon_d.lib")

//#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarLog_d.lib")
//#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SunWriteLogMng_d.lib")

namespace ns_world {
;

static SolarSimpleConsole solar_console;
static SolarDump solar_dump;

static void _InitCircumstance();
static void _PauseWithErrorMessage(const char* error_message);
static void ErrorHandler_new();
static void signal_redirect(int signo);

};

//==================================================================================================

extern "C"
{
    int WinMainCRTStartup();
}

int SunEntryPoint()
{
    return WinMainCRTStartup();
}

//==================================================================================================

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    __UNUSED((hInstance, hPrevInstance, lpCmdLine, nCmdShow));

    char current_directory[256];
    ::GetCurrentDirectory(_countof(current_directory), current_directory);
    //
    ns_world::_InitCircumstance();

    if (ns_world::WorldServer* world_server = ns_world::WorldServer::Instance())
    {
        bool result = !!world_server->DoInit(WORLD_SERVER_INFO_FILE_NAME,
            &world_server->CreateServerSideAcceptedObject,
            &world_server->DestroyServerSideAcceptedObject,
            &world_server->DestroyServerSideConnectedObject,
            NULL, NULL, NULL);
        if (result == false)
        {
            ns_world::_PauseWithErrorMessage("Server initialization fails");
        };

        SUNLOG(eFULL_LOG, _T("SUN Online World Server Initialization Complete"));
        //
        world_server->DoProcess();
        world_server->DoRelease();
    };

    CONSOLELOG(eCRITICAL_LOG, _T("World Server is terminated..."));

    return 0;
}

//==================================================================================================

static void ns_world::ErrorHandler_new()
{
    printf("Can't Alloc Memory\n");
    if (g_pSunLog)
    {
        MessageOut(eCRITICAL_LOG, _T("Can't Alloc Memory\n"));
        g_pSunLog->Flush(CSunLog::cOUTPUT_BOTH);
    }

    __asm { int 3 }
}

static void ns_world::_InitCircumstance()
{
#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
    signal(SIGABRT, signal_redirect);
    _set_error_mode(_OUT_TO_STDERR);
#endif
    std::set_new_handler(ErrorHandler_new);
    // initialize console
    CONSOLE_DESC console_desc;
    ZeroMemory(&console_desc, sizeof(console_desc));
    console_desc.bUsingConsoleInput = false;
    console_desc.bUsingRedirect = true;
    ns_world::solar_console.Init(&console_desc);
}

static void ns_world::_PauseWithErrorMessage(const char* error_message)
{
    puts(error_message);
    system("pause");
    __asm int 3;
}

//==================================================================================================
