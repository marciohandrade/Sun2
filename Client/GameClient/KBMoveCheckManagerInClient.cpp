#include "SunClientPrecompiledHeader.h"

#include "KBMoveCheckManagerInClient.h"

//------------------------------------------------------------------------------ 
KBMoveCheckManagerInClient::KBMoveCheckManagerInClient() : bad_count_(0)
{

}
//------------------------------------------------------------------------------ 
KBMoveCheckManagerInClient::~KBMoveCheckManagerInClient()
{

}
//------------------------------------------------------------------------------ 
bool KBMoveCheckManagerInClient::ReceiveReplyFromServer(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK& reply_msg)
{
    if (CheckReceivedLocalTest(reply_msg) == false)
    {
        ++bad_count_;
        return false;
    }

    //clock_function::GetTickCount();

    ulong client_elapsed_tick = GetTickCount() - GetClientSendTick();

    if (client_elapsed_tick > (ulong)(reply_msg.server_elapsed_tick * 1.5f))
    {
        ++bad_count_;
        return false;
    }

    bad_count_ = 0;
    return true;
}