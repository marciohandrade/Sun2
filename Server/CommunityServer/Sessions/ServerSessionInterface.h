#pragma once
#ifndef WORLDSERVER_SERVER_SESSION_INTERFACE_H
#define WORLDSERVER_SERVER_SESSION_INTERFACE_H

//==================================================================================================

namespace ns_world {
;

class ServerSessionInWorld;

}; //end of namespace

//==================================================================================================

namespace ns_world { namespace sessions {
;

typedef void (*OnRedirectFunc)(ns_world::ServerSessionInWorld* root,
                               DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
typedef void (*OnPostConnectFunc)(ns_world::ServerSessionInWorld* root, DWORD network_index);
typedef void (*OnPrevDisconnectFunc)(ns_world::ServerSessionInWorld* root);
typedef void (*OnRecvFunc)(ns_world::ServerSessionInWorld* root,
                           uint8_t* msg, WORD msg_size);
typedef bool (*OnSendFunc)(ns_world::ServerSessionInWorld* root,
                           MSG_BASE* msg_base, WORD msg_size);

// template
#if SUN_CODE_SAMPLE
struct IServerSession
{
    static void ChangeServerType(ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type);
    static void OnRedirect(ns_world::ServerSessionInWorld* root,
                           DWORD session_index,
                           MSG_SERVER_TYPE* const first_contact_msg);
    static void OnPostConnect(ns_world::ServerSessionInWorld* root, DWORD network_index);
    static void OnPrevDisconnectFunc(ns_world::ServerSessionInWorld* root);
    static void OnRecv(ns_world::ServerSessionInWorld* root,
                       uint8_t* msg, WORD msg_size);
    static bool SendPacket(ns_world::ServerSessionInWorld* root,
                           MSG_BASE* msg_base, WORD msg_size);
};
#endif //

struct GameDBProxySession
{
    static const bool kIsConnector = true;
    static const bool kIsAcceptor = !kIsConnector;
    static void ChangeServerType(ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type);
    //static void OnRedirect(ns_world::ServerSessionInWorld* root,
    //    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    static void OnPostConnect(ns_world::ServerSessionInWorld* root, DWORD network_index);
    static void OnPrevDisconnectFunc(ns_world::ServerSessionInWorld* root);
    static void OnRecv(ns_world::ServerSessionInWorld* root,
                       uint8_t* msg, WORD msg_size);
    static bool SendPacket(ns_world::ServerSessionInWorld* root,
                           MSG_BASE* msg_base, WORD msg_size);
};

struct MasterServerSession
{
    static const bool kIsConnector = true;
    static const bool kIsAcceptor = !kIsConnector;
    static void ChangeServerType(ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type);
    //static void OnRedirect(ns_world::ServerSessionInWorld* root,
    //    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    static void OnPostConnect(ns_world::ServerSessionInWorld* root, DWORD network_index);
    static void OnPrevDisconnectFunc(ns_world::ServerSessionInWorld* root);
    static void OnRecv(ns_world::ServerSessionInWorld* root,
                       uint8_t* msg, WORD msg_size);
    static bool SendPacket(ns_world::ServerSessionInWorld* root,
                           MSG_BASE* msg_base, WORD msg_size);
};

struct AgentServerSession
{
    static const bool kIsConnector = false;
    static const bool kIsAcceptor = !kIsConnector;
    static void ChangeServerType(ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type);
    static void OnRedirect(ns_world::ServerSessionInWorld* root,
        DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    static void OnPostConnect(ns_world::ServerSessionInWorld* root, DWORD network_index);
    static void OnPrevDisconnectFunc(ns_world::ServerSessionInWorld* root);
    static void OnRecv(ns_world::ServerSessionInWorld* root,
                       uint8_t* msg, WORD msg_size);
    static bool SendPacket(ns_world::ServerSessionInWorld* root,
                           MSG_BASE* msg_base, WORD msg_size);
};

}}; //end of namespace

//==================================================================================================

#endif //WORLDSERVER_SERVER_SESSION_INTERFACE_H
