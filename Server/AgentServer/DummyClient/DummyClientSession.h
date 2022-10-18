#pragma once
#ifndef AGENTSERVER_DUMMY_CLIENT_SESSION_H
#define AGENTSERVER_DUMMY_CLIENT_SESSION_H
#if USING_DUMMY_CLIENT

#include <ServerSessions/ServerSessionEx.h>

//==================================================================================================
namespace test_unit {
;

class DummyClientSession : public ServerSessionEx
{
public:
    DummyClientSession();
    virtual ~DummyClientSession();

    eSERVER_TYPE GetServerType() { return CLIENT_SERVER; }
    virtual BOOL SendPacket(MSG_BASE* msg_base, WORD msg_size);

protected:
    virtual void Init(); 
    virtual void Release();
    virtual void OnRecv(BYTE* msg_ptr, WORD msg_size);
    virtual void OnConnect(BOOL is_success, DWORD network_index);
    virtual void OnDisconnect();
    virtual BOOL Send(BYTE* msg_ptr, WORD msg_size);

    //
    test_unit::DummyUser* dummy_user_;
    DWORD network_index_;
    //
    USING_DUMMY_CLIENT_FRIEND_ACCESS;
};

}; //end of namespace

#endif //USING_DUMMY_CLIENT
#endif //AGENTSERVER_DUMMY_CLIENT_SESSION_H