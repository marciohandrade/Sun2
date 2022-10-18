#include "stdafx.h"
#include ".\Handler_CG_CONNECTION.h"

#include "StatusManager.h"
#include "MovementChecker.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_CONNECTION::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_CONNECTION::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
        HANDLER_NODE_INFO(CG_CONNECTION, CG_CONNECTION_CLIENT_TICK_TEST_SYN, PHASE_SERVICE),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_CONNECTION_IMPL(CG_CONNECTION_CLIENT_TICK_TEST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA000000_SPEED_HACK_PREVENTION_SUPPORT)
    ASSERT(!"current version not supported");
    return;
#endif
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    const MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN* recv_msg = \
        static_cast<MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN*>(pMsg);
    //
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    GameField* const game_field = player ? player->GetField() : NULL;
    if ((player == NULL) || (game_field == NULL)) {
        return;
    };

    nsSync::CSSyncTuner* const sync_tuner = player->GetSyncTuner();
    if (sync_tuner->ReceiveClientLocalTest(*recv_msg) == false) {
    #if 0
        StatusManager* status_manager = player->GetStatusManager();
        status_manager->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, true);
        return;
    #endif
    };
#endif
}
