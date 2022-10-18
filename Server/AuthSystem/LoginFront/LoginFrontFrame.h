#pragma once

#include <serverframe.h>

#include "ClientSession.h"
#include "AuthServerSession.h"
#include <Singleton.h>


enum { FRONT_READY_GAMEVERSION, FRONT_READY_CONTROL_IPLIST, FRONT_READY_MAX };


class LoginFrontFrame : public ServerFrame, public util::Singleton<LoginFrontFrame>
{
public:
    //__CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__
    enum eService { eService_Init, eService_Ok, eService_Fail, eService_Count, };

    LoginFrontFrame();
    ~LoginFrontFrame();

    static NetworkObject* DoAllocServerSession();
    static VOID DoFreeServerSession(NetworkObject* pNetObject);
    static VOID DoFreeConnectSession(NetworkObject* pNetObject);

    static NetworkObject* DoAllocClient();
    static VOID DoFreeClient(NetworkObject* pNetObject);
    static VOID DoFreeConnect_NotUsed(NetworkObject* pNetObject);

    virtual eSERVER_TYPE GetServerType() { return LOGIN_FRONT_SERVER; }

    BOOL    SendAuthServer(MSG_BASE_INTERNAL* pMsg, WORD wSize);
    BOOL    IsAuthServerConnected();

    BOOL    ConnectTo(eSERVER_TYPE type);
    ServerSession*  GetSession(eSERVER_TYPE type);

    //__CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__
    void    SetServiceControl(eService service);

    VOID    DisplayVersionInfo();

protected:
    virtual BOOL Init();
    virtual BOOL Update(DWORD tick);
    virtual void Release();

    virtual BOOL KeyEvent(CHAR ch);
    VOID         DisplayServerInfo();

    virtual ServerSession* AllocServerSession(eSERVER_TYPE);
    virtual void FreeServerSession(ServerSession*);

    BOOL CreateSunLog();

private:
    void InitServerData(BOOLEAN reload); //_GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_

    util::CMemoryPoolFactory<ClientSession> m_ClientSessionPool;
    util::CMemoryPoolFactory<AuthServerSession> m_AuthServerSessionPool;

    AuthServerSession* m_pAuthServerSession;

    __PROPERTY(DWORD, LoginTick);
    __PROPERTY(BYTE, LoginFrontID);
private:
    eService m_ServiceControl; //__CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__

    __REF_PROPERTY(GAME_VERSION, VerProtocol);
    __REF_PROPERTY(GAME_VERSION, VerLauncher);
    __REF_PROPERTY(GAME_VERSION, VerMajor);
    __REF_PROPERTY(GAME_VERSION, VerMinor);
};












