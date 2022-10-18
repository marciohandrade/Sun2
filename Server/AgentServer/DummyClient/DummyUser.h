#pragma once
#ifndef AGENTSERVER_DUMMY_USER_H
#define AGENTSERVER_DUMMY_USER_H
#if USING_DUMMY_CLIENT

#include <UserSessions/User.h>

namespace test_unit {
;
//==================================================================================================
class DummyClientSession;
struct DummyUserProtocolHandler;

class DummyUser : public User
{
public:
    //
    DummyUser();
    virtual ~DummyUser();

    //---------------------------------------------------------------------------------------------
    // <OBJECT CONTROL>
public:
    virtual void Init();
    virtual void Release();
    virtual eUSER_TYPE GetUserType() const { return UT_USER; };

    void CopyInfo(ns_object::UserAccountInfo* user_info);
    //---------------------------------------------------------------------------------------------
    // <NETWORK OBJECT><INHERITANCE>
    virtual void OnRecv(BYTE* msg_ptr, WORD msg_size);
    virtual void OnAccept(DWORD network_index);
    virtual void OnDisconnect();

    //---------------------------------------------------------------------------------------------
    // <NETWORK OBJECT CONTROL>
    virtual BOOL SendPacket(MSG_BASE* msg_base, WORD msg_size);
    //
    int configuration_step() const { return configuration_step_; }
    void change_configuration_step(int step) { configuration_step_ = step; }
    //
    bool Start();
    void SimulSend_CG_CONNECTION_ENTERSERVER_SYN();
    void SimulRecv_AD_CHARINFO_CHARLISTREQ_ACK();
    void SimulSend_CG_CONNECTION_ENTERVILLAGEREQ_SYN();
    void SimulSend_CG_SYNC_PLAYER_ENTER_SYN();

    bool Authentication(MSG_CG_CONNECTION_ENTERSERVER_SYN* msg_syn);

private:
    User* user_session_;
    DummyClientSession* dummy_session_;
    DummyUserProtocolHandler* dummy_protocol_handler_;
    int configuration_step_;
    //
    friend struct DummyUserProtocolHandler;
    friend class DummyClientSession;
};

void StartDummyClient();

}; //end of namespace

#endif //USING_DUMMY_CLIENT
#endif //AGENTSERVER_DUMMY_USER_H