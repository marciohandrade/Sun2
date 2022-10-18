#pragma once
#ifndef GAMESERVER_CS_SYNC_TUNER_H
#define GAMESERVER_CS_SYNC_TUNER_H

#include "KBMoveCheckManager.h"

//==================================================================================================

namespace nsSync {
;

class CSSyncTuner
{
public:
    static const ulong kMaxMovableRangeCheckInterval;
    CSSyncTuner();
    ~CSSyncTuner();

    void Initialize(Player* owner);
    void OnChangeAttribute();
    void OnWarp();
    void OnJump(const WzVector& destination);
    void OnFastMoved(const WzVector& destination);
    void Update();
    //
    void EnterField(MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd);
    bool CheckClientAbusedData(ulong client_test_result);
    bool ReceiveReply(const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK& reply_msg);
    bool EnabledMoving(ulong received_serial_key) const;
    bool EnabledStopping(ulong received_stop_serial_key) const;
    //bool UpdateKBMoveCheckRefresh(MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd);
    //
    bool ReceiveClientLocalTest(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN& test_msg);
    //bool UpdateClientLocatTestRefresh(MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK* const reply_msg);
private:
    Player* owner_;
    bool activated_;
    bool moving_blocked_;
    uint8_t jump_fault_tolerant_;
    float max_moving_speed_;
    float fault_tolerant_ranges_;
    util::ITimerBase max_movable_range_check_timer_;
    DWORD movable_check_tick_;
    WzVector movable_check_pos_;
    nsSync::KBMoveCheckManagerInServer kbmove_checker_;
    //
    __DISABLE_COPY(CSSyncTuner);
};

}; //end of namespace

//==================================================================================================

#endif //GAMESERVER_CS_SYNC_TUNER_H