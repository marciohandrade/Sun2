#include "stdafx.h"
#include "./DummyClientSession.h"
#include "./DummyUser.h"

#if USING_DUMMY_CLIENT

#include <AgentServer.h>
#include <PacketHandler/PacketHandler.h>

//==================================================================================================
namespace test_unit {
;

DummyClientSession::DummyClientSession()
    : dummy_user_(NULL)
    , network_index_(0)
{
}

DummyClientSession::~DummyClientSession()
{
}

void DummyClientSession::Init()
{
    ServerSessionEx::Init();
}

void DummyClientSession::Release()
{
    ServerSessionEx::Release();
}

void DummyClientSession::OnConnect(BOOL is_success, DWORD network_index)
{
    SetIsManualConnect(false);
    SetConnecting(false);

    if(is_success)
    {
        network_index_ = network_index;
        dummy_user_->configuration_step_ = 1;
    }
    else
    {
        printf("|Connection Failed|you should be check below contents|\n"
               "|#1. is connected 'auth agent'|\n"
               "|#2. is connected 'dbproxy and field server'?|\n"
               "|#3. is correct input values about a test user information|\n");
    }
}

void DummyClientSession::OnRecv(BYTE* msg_ptr, WORD msg_size)
{
    MSG_BASE* msg_base = reinterpret_cast<MSG_BASE*>(msg_ptr);
    printf("|"__FUNCTION__"|\n");
    if (dummy_user_->configuration_step_ >= 3) {
        return;
    }
}

BOOL DummyClientSession::SendPacket(MSG_BASE* msg_base, WORD msg_size)
{
    return false;
}

void DummyClientSession::OnDisconnect()
{
    ServerSessionEx::OnDisconnect();
}

BOOL DummyClientSession::Send(BYTE* msg_ptr, WORD msg_size)
{
    return true;
}

}; //end of namespace
#endif //USING_DUMMY_CLIENT
