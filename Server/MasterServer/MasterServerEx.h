#pragma once

#include <PacketStruct.h>
#include <ServerFrame.h>

#include <Function/MasterServerInfo.h>
#include <Function/MasterServerInfoStruct.h>

#define MASTER_SERVER_INFO_FILE_NAME    ".\\MasterServer.ini"

// -------------------------------------
// <Connect Sessions><MasterToServer>
class AccountDBProxySession;
class OpServerSession;
class SolarAuthServerSession;
// -------------------------------------

class ScriptParserManager;

class MasterServerEx : public ServerFrame
{
public:
    enum eIOHANDLER 
    { 
        INSIDE_IOHANDLER = 0, 
        OUTSIDE_IOHANDLER = 1, 
        NUM_IOHANDER = 2, 
        BOTH_IOHANDLER = 2, 
    };

public:
    MasterServerEx();
    ~MasterServerEx();

    static MasterServerEx* GetMainFrame();

public:
    // ServerFrame���� ��ȯ�ϸ鼭 ���ʴ�� ����
    static NetworkObject* Handler_AllocServerSession_NoInit(eSERVER_TYPE server_type);
    static VOID Handler_FreeServerSession_NoRelease(NetworkObject* network_object);

    static ServerSession* Handler_AllocServerSession(eSERVER_TYPE server_type);
    static VOID Handler_FreeServerSession(ServerSession* server_session);

    // Callback Handlers
    static NetworkObject* CreateServerSideAcceptedObject();
    static VOID DestroyServerSideAcceptedObject(NetworkObject* network_object);
    static VOID DestroyServerSideConnectedObject(NetworkObject* network_object);

    // =========================================================================================================
    // <IMPLEMENTATION><SERVERFRAME><INTERFACEs>
public:
    virtual eSERVER_TYPE GetServerType() 
    { 
        return MASTER_SERVER; 
    }

protected:
    virtual BOOL Init();
    virtual BOOL Update(DWORD tick);
    virtual VOID Release();
    virtual BOOL KeyEvent(CHAR pressed_key);
    virtual ServerSession* AllocServerSession(eSERVER_TYPE serverType);
    virtual VOID FreeServerSession(ServerSession* server_session);

public:
    // <NETWORK><MAINTAIN>

    // �̰��� ServerFrame�� ����ϸ鼭 ���� Release ȣ�� ���Ŀ� �߻��ϴ� ���ο� Instance�� �����ϱ� ����
    VOID Finalize();

    // =========================================================================================================
    // <IMPLEMENTATION><MASTER_SERVER><INTERFACEs>
public:
    VOID StartListen();
    const CHAR* GetIPAddress_Outside() 
    { 
        return GetListenIP_s(); 
    }

private:
    BOOL InitializeLogger();
    
    // =========================================================================================================
    // <IMPLEMENTATION><for SOLAR_AUTH><INTERFACEs>
public:
    // (CHANGES)(20100311)(hukim) GetPhaseStatus(), IsRestrictFuncFromFile(), GetAuthProcessWorkLoadKey(), 
    // GetParserManager() ���� ������ ����Ű�� �ʱ� ������ const �߰�
    // (CHANGES)(20100311)(hukim) GetMasterServerInfo(), RestrictManager() ȣ���ϴ� �޼���鵵 ���Ŀ�
    // const�� ���̱� ������ const ���� �߰�
    
    inline ePHASE_SHIFT GetPhaseStatus() const 
    { 
        return phase_shift_; 
    }
    inline VOID SetPhaseStatus(ePHASE_SHIFT phase) 
    { 
        phase_shift_ = phase; 
    }

    static VOID StartDataService();

    // <RUNTIME ENVIRONMENT>
    // <INTERFACEs><INFOs><Get/Set>
    inline BOOL GetServiceMode() const 
    { 
        return is_service_mode_; 
    }
    inline MasterServerInfo& GetMasterServerInfo()
    { 
        return master_server_info_; 
    }
    inline const MasterServerInfo& GetMasterServerInfo() const
    {
        return master_server_info_;
    }
    inline WORK_LOAD_CONTAINER_SESSION_WORK& GetWorkLoadContainer()
    { 
        return work_load_; 
    }
    inline sRESTRICT_MGR& RestrictManager()
    { 
        return restrict_manager_; 
    }
    inline const sRESTRICT_MGR& RestrictManager() const
    {
        return restrict_manager_;
    }
    inline BOOL IsRestrictFuncFromFile() const
    { 
        return is_restrict_from_file_; 
    }
    inline DWORD& StatisticsInterval()
    { 
        return statistics_interval_; 
    }
    inline WORK_KEY GetAuthProcessWorkLoadKey() const
    { 
        return auth_process_work_load_key_; 
    }
    inline LOGIC_VIOLATION_CONTOL_FLAGS& GetLogicViolationControl()
    { 
        return logic_violation_control_; 
    }
    inline ScriptParserManager* GetParserManager() const
    { 
        return master_script_parser_manager_; 
    }

private:
    BOOL ReloadFromFile_RestrictFunction();
    BOOL ReloadFromFile_StatisticsInterval();
    BOOL ReloadFromFile_LogicViolationControl();
    VOID InitializeDataLoad();
    //
    // =========================================================================================================

private:
    // (CHANGES)(20100311)(hukim) ���⼭�� unnamed struct�� ����ϴ°��� ����ȭ �̻��� �ǹ̰� ����
    // ������ �ּ�ȭ

    // Server Runtime Control Flags
    //struct
    //{ 
    BOOL is_service_mode_;
    BOOL is_restrict_from_file_;
    ePHASE_SHIFT phase_shift_;
    DWORD statistics_interval_;
    LOGIC_VIOLATION_CONTOL_FLAGS logic_violation_control_;
    //};

public:
    // ConnectServerSession
    class sConnectSession
    {   
    public:
        VOID Init()
        {
            account_dbproxy_session_ = NULL;
            op_server_session_ = NULL;
            solar_auth_server_session_ = NULL;
        }

        AccountDBProxySession*& SessionAccountDB()
        { 
            return account_dbproxy_session_; 
        }
        OpServerSession*& SessionOpServer()
        { 
            return op_server_session_; 
        }
        SolarAuthServerSession*& SessionSolarAuth()
        { 
            return solar_auth_server_session_;
        }
        
        // (CHANGES)(20100311)(hukim) ���� ��������� �����Ǿ� ������ ���� �޼��� �ּ�ȭ

        //BOOL SendToAccountDB(MSG_BASE* message, WORD size);
        //BOOL SendToOpServer(MSG_BASE* message, WORD size);
        //BOOL IsAccountDBConnected();
        //BOOL IsMasterConnected();
        //BOOL SendToSolarAuth(MSG_BASE* message, WORD size);
        //BOOL IsSolarAuthConnected();

    private:
        AccountDBProxySession* account_dbproxy_session_;
        OpServerSession* op_server_session_;
        SolarAuthServerSession* solar_auth_server_session_;

    } ConnectSessions;

private:
    MasterServerInfo master_server_info_;
    WORK_LOAD_CONTAINER_SESSION_WORK work_load_;
    sRESTRICT_MGR restrict_manager_;
    WORK_KEY auth_process_work_load_key_;
    ScriptParserManager* master_script_parser_manager_;

private:
    // (CHANGES)(20100309)(hukim) ���������� ���簡 �Ͼ�� �ȵǱ� ������ __DISABLE_COPY() �߰�
    __DISABLE_COPY(MasterServerEx);

}; // MasterServerEx
