#pragma once
#ifndef WORLDSERVER_SERVERSESSION_IN_WORLD_H
#define WORLDSERVER_SERVERSESSION_IN_WORLD_H

#include <ServerSession.h>
#include <TempServerSession.h>

#include "./ServerSessionInterface.h"
#include "Channel/WorldChannel.h"

struct MSG_BASE;
//==================================================================================================

namespace ns_world {
;

class ServerSessionInWorld : public ServerSession
{
public:
    ServerSessionInWorld();
    virtual ~ServerSessionInWorld();
    // NetworkObject interfaces = {
    virtual eSERVER_TYPE GetServerType();
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    virtual void OnConnect(BOOL succeed, DWORD network_index);
    virtual void OnDisconnect();
    virtual void OnRecv(uint8_t* msg, uint16_t msg_size);
    virtual BOOL SendPacket(MSG_BASE* msg_base, WORD msg_size);
    // }; end of NetworkObject interfaces
    void ChangeServerType(eSERVER_TYPE server_type);
    void BindChannel(ns_world::community::WorldChannel* bind_channel);
    int GetChannelID() const;
    ns_world::community::WorldChannel* GetChannel() const;
    void SetServerKey(DWORD server_key);
private:
    // ServerSession interfaces = {
    virtual void Init();
    // };
    void OnBaseRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    void OnBaseConnect(BOOL bSuccess, DWORD network_index);
    void OnBaseDisconnect();

    virtual void OnLogString(char* log_message);
    // data fields...
    eSERVER_TYPE server_type_;
    DWORD server_key_;
    struct Handles {
        sessions::OnRecvFunc receive_func; // replacement
        sessions::OnSendFunc send_func; // replacement
        sessions::OnPostConnectFunc post_connect_func; // post-event
        sessions::OnPrevDisconnectFunc prev_disconnect_func; // prev-event
        sessions::OnRedirectFunc redirect_func; // replacement
    } handles_;
    union SubInfo {
        struct AgentInfo {
            ns_world::community::WorldChannel* bind_channel;
        } agent;
    } sub_info_;
    //
    friend struct sessions::GameDBProxySession;
    friend struct sessions::MasterServerSession;
    friend struct sessions::AgentServerSession;
    __DISABLE_COPY(ServerSessionInWorld);
};

}; //end of namespace

//==================================================================================================

inline eSERVER_TYPE ns_world::ServerSessionInWorld::GetServerType() {
    return server_type_;
}

inline void ns_world::ServerSessionInWorld::OnBaseRedirect(
    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(session_index, first_contact_msg);
}

inline void ns_world::ServerSessionInWorld::OnBaseConnect(BOOL succeed, DWORD network_index)
{
    ServerSession::OnConnect(succeed, network_index);
}

inline void ns_world::ServerSessionInWorld::OnBaseDisconnect()
{
    ServerSession::OnDisconnect();
}


#endif //WORLDSERVER_SERVERSESSION_IN_WORLD_H
