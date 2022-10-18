#include "WorldServerPch.hxx"
#include "./ServerSessionInterface.h"

#include <PacketStruct_AW.h>

#include "WorldServer.h"
#include "./ServerSessionInWorld.h"

#include "Channel/WorldChannelManager.h"
#include "Services/ChattingService.h"
#include "PacketHandler/PacketHandler.h"

//==================================================================================================

void ns_world::sessions::GameDBProxySession::ChangeServerType(
    ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type)
{
    assert(server_type == GAME_DBPROXY); __TOUCHED(server_type);
    root->server_type_ = GAME_DBPROXY;
    if (ns_world::ServerSessionInWorld::Handles* handles = &root->handles_)
    {
        handles->receive_func = &OnRecv;
        handles->send_func = &SendPacket;
        handles->post_connect_func = &OnPostConnect;
        handles->prev_disconnect_func = &OnPrevDisconnectFunc;
    };
}

void ns_world::sessions::GameDBProxySession::OnPostConnect(
    ns_world::ServerSessionInWorld* root, DWORD network_index)
{
    __UNUSED(network_index);
    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    world_server->EnableSessionService(root, true);
    ns_world::ChattingService::EnableService(&world_server->GetTimeInfoPerFrame());
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
#else
    if (world_server->IsServerInfoFromFile()) {
        world_server->Listen(SERVER_IOHANDLER);
    };
#endif
}

void ns_world::sessions::GameDBProxySession::OnPrevDisconnectFunc(
    ns_world::ServerSessionInWorld* root)
{
    ns_world::WorldServer::Instance()->EnableSessionService(root, false);
}

void ns_world::sessions::GameDBProxySession::OnRecv(
    ns_world::ServerSessionInWorld* root, uint8_t* msg, WORD msg_size)
{
    ns_world::PacketHandler* packet_handler = ns_world::PacketHandler::Instance();
    packet_handler->ParsePacket(ns_world::PACKET_HANDLER_DBPROXY, root,
        reinterpret_cast<MSG_BASE*>(msg), msg_size);
}

bool ns_world::sessions::GameDBProxySession::SendPacket(
    ns_world::ServerSessionInWorld* root,
    MSG_BASE* msg_base, WORD msg_size)
{
    return root->Send(reinterpret_cast<BYTE*>(msg_base), msg_size) != false;
}

void ns_world::sessions::MasterServerSession::ChangeServerType(
    ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type)
{
    assert(server_type == MASTER_SERVER); __TOUCHED(server_type);
    root->server_type_ = MASTER_SERVER;
    if (ns_world::ServerSessionInWorld::Handles* handles = &root->handles_)
    {
        handles->receive_func = &OnRecv;
        handles->send_func = &SendPacket;
        handles->post_connect_func = &OnPostConnect;
        handles->prev_disconnect_func = &OnPrevDisconnectFunc;
    };
}

void ns_world::sessions::MasterServerSession::OnPostConnect(
    ns_world::ServerSessionInWorld* root, DWORD network_index)
{
    __UNUSED(network_index);
    ns_world::WorldServer::Instance()->EnableSessionService(root, true);
}

void ns_world::sessions::MasterServerSession::OnPrevDisconnectFunc(
    ns_world::ServerSessionInWorld* root)
{
    ns_world::WorldServer::Instance()->EnableSessionService(root, false);
}

void ns_world::sessions::MasterServerSession::OnRecv(
    ns_world::ServerSessionInWorld* root, uint8_t* msg, WORD msg_size)
{
    ns_world::PacketHandler* packet_handler = ns_world::PacketHandler::Instance();
    packet_handler->ParsePacket(ns_world::PACKET_HANDLER_MASTER, root,
        reinterpret_cast<MSG_BASE*>(msg), msg_size);
}

bool ns_world::sessions::MasterServerSession::SendPacket(
    ns_world::ServerSessionInWorld* root,
    MSG_BASE* msg_base, WORD msg_size)
{
    return root->Send(reinterpret_cast<BYTE*>(msg_base), msg_size) != false;
}

void ns_world::sessions::AgentServerSession::ChangeServerType(
    ns_world::ServerSessionInWorld* root, eSERVER_TYPE server_type)
{
    assert(server_type == AGENT_SERVER); __TOUCHED(server_type);
    root->server_type_ = AGENT_SERVER;
    if (ns_world::ServerSessionInWorld::Handles* handles = &root->handles_)
    {
        handles->redirect_func = &ns_world::sessions::AgentServerSession::OnRedirect;
        handles->receive_func = &OnRecv;
        handles->send_func = &SendPacket;
        handles->post_connect_func = &OnPostConnect;
        handles->prev_disconnect_func = &OnPrevDisconnectFunc;
    };
}

void ns_world::sessions::AgentServerSession::OnRedirect(
    ns_world::ServerSessionInWorld* root,
    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    root->OnBaseRedirect(session_index, first_contact_msg);
    first_contact_msg->m_byCategory = AW_CONNECTION;
    first_contact_msg->m_byProtocol = AW_CONNECTION_GROUND_CMD;
}

void ns_world::sessions::AgentServerSession::OnPostConnect(
    ns_world::ServerSessionInWorld* root, DWORD network_index)
{
    __UNUSED(network_index);
    ns_world::WorldServer::Instance()->EnableSessionService(root, true);
}

void ns_world::sessions::AgentServerSession::OnPrevDisconnectFunc(
    ns_world::ServerSessionInWorld* root)
{
    ns_world::WorldServer::Instance()->EnableSessionService(root, false);
    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();
    channel_manager->RemoveChannel(root);
}

void ns_world::sessions::AgentServerSession::OnRecv(
    ns_world::ServerSessionInWorld* root, uint8_t* msg, WORD msg_size)
{
    ns_world::PacketHandler* packet_handler = ns_world::PacketHandler::Instance();
    packet_handler->ParsePacket(ns_world::PACKET_HANDLER_AGENT, root,
        reinterpret_cast<MSG_BASE*>(msg), msg_size);
}

bool ns_world::sessions::AgentServerSession::SendPacket(
    ns_world::ServerSessionInWorld* root,
    MSG_BASE* msg_base, WORD msg_size)
{
    return root->Send(reinterpret_cast<BYTE*>(msg_base), msg_size) != false;
}
