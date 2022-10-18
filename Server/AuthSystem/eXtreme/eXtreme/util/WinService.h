#ifndef  __eXtreme_WinService_H
#define  __eXtreme_WinService_H


namespace eXtreme {

enum EventLogType
{
    EVENTLOG_ERROR          = EVENTLOG_ERROR_TYPE,
    EVENTLOG_WARNING        = EVENTLOG_WARNING_TYPE,
    EVENTLOG_INFORMATION    = EVENTLOG_INFORMATION_TYPE
};

/*  Service Start Type

SERVICE_AUTO_START,
SERVICE_BOOT_START,
SERVICE_DEMAND_START,
SERVICE_DISABLED,
SERVICE_SYSTEM_START
*/

class WinService
{
    eSingleton(WinService);

    typedef bool (*FN_StartCallback)();
    typedef void (*FN_StopCallback)();
    typedef bool (*FN_UpdateCallback)();

public:
    static void AddEventLog(char* sourceName, EventLogType type, char* format, ...);

    bool UploadService(char* serviceName,
                       char* displayName, 
                       char* binaryPath,
                       char* serviceLogin = 0, 
                       char* password = 0, 
                       bool autoStart = false);
    bool UploadService(char* serviceName, 
                       char* displayName, 
                       char* binaryPath,
                       char* serviceLogin,
                       char* password,
                       char* dependency,
                       bool autoStart = false);

    bool RemoveService(char* serviceName);

    bool StartDispatcher(char* serviceName,
                         FN_StartCallback fnStart,
                         FN_StopCallback fnStop,
                         FN_UpdateCallback fnUpdate = NULL);
    void BreakOutDispatcher();

    bool ChangeServiceInfo(char* netMachine, char* serviceName,
                           char* account, char* password, DWORD startType);

    DWORD QueryService(char* netMachine, char* serviceName,
                       QUERY_SERVICE_CONFIG* config, DWORD bufSize);
    DWORD StartService(char* netMachine, char* serviceName);
    DWORD StopService(char* netMachine, char* serviceName);
    bool ConnectNet(char* account, char* password, char* netMachine);
    bool CancelNet(char* netMachine);

    bool LocalSystemReboot(LPTSTR msg = 0, DWORD timeout = 20);

    const char* GetServiceName() { return m_serviceName; }

private:
    static void __stdcall ServiceHandler(DWORD status);
    static void __stdcall ServiceLoop(DWORD argc, char** argv);

    bool OnServiceStart();
    void OnServiceStop();
    bool OnServiceUpdate();
    bool IsServiceUpdate();

    FN_StartCallback  m_serviceStartFunc;
    FN_StopCallback   m_serviceStopFunc;
    FN_UpdateCallback m_serviceUpdateFunc;

    char m_serviceName[64];
    SERVICE_STATUS_HANDLE m_handleServiceStatus;

    void* m_handleServiceEvent;
    DWORD m_statusEvent;
    SERVICE_STATUS m_ssQueryControl;
};

}

#endif