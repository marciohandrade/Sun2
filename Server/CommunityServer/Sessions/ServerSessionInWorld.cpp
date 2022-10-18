#include "WorldServerPch.hxx"
#include "./ServerSessionInWorld.h"

//==================================================================================================

namespace ns_world {
;
}; //end of namespace

//==================================================================================================

ns_world::ServerSessionInWorld::ServerSessionInWorld()
    : server_type_(UNKNOWN_SERVER)
    , server_key_(0)
{
    ZeroMemory(&handles_, sizeof(handles_));
}

ns_world::ServerSessionInWorld::~ServerSessionInWorld()
{
}

void ns_world::ServerSessionInWorld::Init()
{
    ServerSession::Init();
    server_type_ = UNKNOWN_SERVER;
    server_key_ = 0;
    ZeroMemory(&handles_, sizeof(handles_));
    ZeroMemory(&sub_info_, sizeof(sub_info_));
}

void ns_world::ServerSessionInWorld::ChangeServerType(eSERVER_TYPE server_type)
{
    switch (server_type)
    {
    case GAME_DBPROXY:
        return sessions::GameDBProxySession::ChangeServerType(this, server_type);
    case AGENT_SERVER:
        return sessions::AgentServerSession::ChangeServerType(this, server_type);
    case MASTER_SERVER:
        return sessions::MasterServerSession::ChangeServerType(this, server_type);
    };
    //
    assert(!"unexpected type");
}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
void ns_world::ServerSessionInWorld::OnRedirect(
    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    if (handles_.redirect_func) {
        return handles_.redirect_func(this, session_index, first_contact_msg);
    }
    else {
        return ServerSession::OnRedirect(session_index, first_contact_msg);
    };
}

void ns_world::ServerSessionInWorld::OnConnect(BOOL succeed, DWORD network_index)
{
    ServerSession::OnConnect(succeed, network_index);
    if (succeed && handles_.post_connect_func) {
        return handles_.post_connect_func(this, network_index);
    };
}

void ns_world::ServerSessionInWorld::OnDisconnect()
{
    if (handles_.prev_disconnect_func) {
        handles_.prev_disconnect_func(this);
    };
    return ServerSession::OnDisconnect();
}

void ns_world::ServerSessionInWorld::OnRecv(uint8_t* msg, uint16_t msg_size)
{
    if (handles_.receive_func) {
        handles_.receive_func(this, msg, msg_size);
    }
    else {
        
    };
}

BOOL ns_world::ServerSessionInWorld::SendPacket(MSG_BASE* msg_base, WORD msg_size)
{
    if (handles_.send_func) {
        return handles_.send_func(this, msg_base, msg_size);
    }
    else {
        return ServerSession::Send(reinterpret_cast<BYTE*>(msg_base), msg_size);
    };
}

void ns_world::ServerSessionInWorld::OnLogString(char* log_message)
{
    __TOUCHED(log_message);
}

//==================================================================================================

void ns_world::ServerSessionInWorld::BindChannel(ns_world::community::WorldChannel* bind_channel)
{
    if (FlowControl::FCAssert(server_type_ == AGENT_SERVER) == false) {
        return;
    };
    sub_info_.agent.bind_channel = bind_channel;
};

ns_world::community::WorldChannel* ns_world::ServerSessionInWorld::GetChannel() const
{
    if (FlowControl::FCAssert(server_type_ == AGENT_SERVER) == false) {
        return NULL;
    };
    return sub_info_.agent.bind_channel;
}

int ns_world::ServerSessionInWorld::GetChannelID() const
{
    if (FlowControl::FCAssert(server_type_ == AGENT_SERVER) == false) {
        return 0;
    };
    if (sub_info_.agent.bind_channel == NULL) {
        return 0;
    };
    return sub_info_.agent.bind_channel->GetChannelID();
}

void ns_world::ServerSessionInWorld::SetServerKey(DWORD server_key)
{
    server_key_ = server_key;
};

//==================================================================================================
