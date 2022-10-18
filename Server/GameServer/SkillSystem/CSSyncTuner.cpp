#include "stdafx.h"
#include "./CSSyncTuner.h"

#include <PublicMath.h>

#include "StatusManager.h"
#include "MoveStateControl.h"

//==================================================================================================

const ulong nsSync::CSSyncTuner::kMaxMovableRangeCheckInterval = 5 * 1000;

//==================================================================================================

nsSync::CSSyncTuner::CSSyncTuner()
    : owner_(NULL)
    , activated_(false)
    , moving_blocked_(false)
    , jump_fault_tolerant_(0)
    , max_moving_speed_(0.0f)
    , fault_tolerant_ranges_(0.0f)
    , movable_check_tick_(0)
{
}

nsSync::CSSyncTuner::~CSSyncTuner()
{
}

void nsSync::CSSyncTuner::Initialize(Player* owner)
{
    owner_ = owner;
    activated_ = false;
    moving_blocked_ = false;
    jump_fault_tolerant_ = 0;
    max_moving_speed_ = 0.0f;
    fault_tolerant_ranges_ = 0.0f;
    max_movable_range_check_timer_.Clear();
    movable_check_tick_ = 0;
    ZeroMemory(&movable_check_pos_, sizeof(movable_check_pos_));
    kbmove_checker_.Initialize(&kbmove_checker_);
}

void nsSync::CSSyncTuner::OnChangeAttribute()
{
    if (activated_ == false) {
        return;
    };
    float current_moving_speed = owner_->GetMoveStateControl()->GetPlayerSpeedInMoving();
    max_moving_speed_ = max(max_moving_speed_, current_moving_speed);
}

void nsSync::CSSyncTuner::OnWarp()
{
    max_movable_range_check_timer_.SetTimer(kMaxMovableRangeCheckInterval);
    jump_fault_tolerant_ = 0;
    fault_tolerant_ranges_ = 0.0f;
    movable_check_pos_ = *owner_->GetPosPtr();
    movable_check_tick_ = ::GetTickCount();
}

void nsSync::CSSyncTuner::OnJump(const WzVector& destination)
{
    if (activated_ == false) {
        return;
    };
    if (jump_fault_tolerant_ >= 2) {
        return;
    };
    if ((moving_blocked_ != false) ||
        (owner_->GetSectorIndex() == INVALID_SECTOR_INDEX))
    {
        return;
    };
    //
    ++jump_fault_tolerant_;
    //
    const WzVector& owner_pos = *owner_->GetPosPtr();
    WzVector diff = movable_check_pos_;
    ;        diff -= owner_pos;
    float estimated_distance = Math_Vector2DLength(&diff);
    ;        diff = movable_check_pos_;
    ;        diff -= destination;
    float post_distance = Math_Vector2DLength(&diff);
    //
    if (estimated_distance < post_distance)
    {
        fault_tolerant_ranges_ += (post_distance - estimated_distance);
    };
};

void nsSync::CSSyncTuner::OnFastMoved(const WzVector& destination)
{
    if (activated_ == false) {
        return;
    };
    if ((moving_blocked_ != false) ||
        (owner_->GetSectorIndex() == INVALID_SECTOR_INDEX))
    {
        return;
    };
    const WzVector& owner_pos = *owner_->GetPosPtr();
    WzVector diff = movable_check_pos_;
    ;        diff -= owner_pos;
    float estimated_distance = Math_Vector2DLength(&diff);
    ;        diff = movable_check_pos_;
    ;        diff -= destination;
    float post_distance = Math_Vector2DLength(&diff);
    //
    if (estimated_distance < post_distance)
    {
        fault_tolerant_ranges_ += (post_distance - estimated_distance);
    };
};

void nsSync::CSSyncTuner::Update()
{
    MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK msg_test_cmd;
    if (kbmove_checker_.UpdateClientLocatTestRefresh(&msg_test_cmd)) {
        owner_->SendPacket(&msg_test_cmd, sizeof(msg_test_cmd));
    };
    //
    GameField* const game_field = owner_->GetField();
    if (game_field == NULL || game_field->IsReady() == false) {
        return;
    };
    if (activated_ == false) {
        return;
    };

    MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD msg_serial_cmd;
    if (kbmove_checker_.UpdateKBMoveCheckRefresh(&msg_serial_cmd)) {
        owner_->SendPacket(&msg_serial_cmd, sizeof(msg_serial_cmd));
    };
    //return; // for debug
    if ((moving_blocked_ == false) &&
        (owner_->GetSectorIndex() != INVALID_SECTOR_INDEX) &&
        (max_movable_range_check_timer_.IsExpired()))
    {
        // TODO: max movable range check routine
        const WzVector* cur_pos = owner_->GetPosPtr();
        WzVector diff = movable_check_pos_;
        ;        diff -= (*cur_pos);
        float distance = Math_Vector2DLength(&diff);

        const DWORD cur_tick = ::GetTickCount();
        uint64_t tick = cur_tick;
        if (movable_check_tick_ > tick) {
            tick += ULONG_MAX;
        };

        DWORD elapsed_tick = static_cast<DWORD>(tick - movable_check_tick_);
        //const DWORD kJumpFaultTolerantModifier = MAX_JUMP_DISTANCE - (MAX_JUMP_DISTANCE >> 2);
        //const float jump_fault_tolerant_length = jump_fault_tolerant_ ?
        //    ((jump_fault_tolerant_ * kJumpFaultTolerantModifier) + 1.0f) : 1.0f;
        //                                                      floating error correction
        const float jump_fault_tolerant_length = fault_tolerant_ranges_ + 1.0f;
        float max_movable_length = max_moving_speed_ * elapsed_tick;
        ;     max_movable_length += jump_fault_tolerant_length;
        if (distance > max_movable_length) {
             owner_->GetStatusManager()->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, true);
             moving_blocked_ = true;
             max_movable_range_check_timer_.SetTimer(kMaxMovableRangeCheckInterval * 2);
        };
        jump_fault_tolerant_ = 0;
        fault_tolerant_ranges_ = 0.0f;
        movable_check_tick_ = cur_tick;
        movable_check_pos_ = *cur_pos;
        max_moving_speed_ = 0.0f;
        this->OnChangeAttribute(); // re-calculate moving speed
    }
    else if ((moving_blocked_ != false) &&
             (max_movable_range_check_timer_.IsExpired()))
    {
        owner_->GetStatusManager()->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, false);
        moving_blocked_ = false;
        max_movable_range_check_timer_.SetTimer(kMaxMovableRangeCheckInterval);
        //
        jump_fault_tolerant_ = 0;
        fault_tolerant_ranges_ = 0.0f;
        movable_check_tick_ = ::GetTickCount();
        movable_check_pos_ = *owner_->GetPosPtr();
        max_moving_speed_ = 0.0f;
        this->OnChangeAttribute(); // re-calculate moving speed
    };

    // NOTE: f110801.2L, abusing check routine in client
    if (kbmove_checker_.client_battle_serial_failed &&
        kbmove_checker_.client_battle_serial_failed_pending_timer.IsExpired())
    {
        owner_->GetStatusManager()->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, true);
        kbmove_checker_.client_battle_serial_failed_pending_timer.Clear();
    }
}

//
void nsSync::CSSyncTuner::EnterField(MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd)
{
    max_movable_range_check_timer_.SetTimer(kMaxMovableRangeCheckInterval);
    movable_check_pos_ = *owner_->GetPosPtr();
    kbmove_checker_.EnterField(msg_cmd);
    activated_ = true;
    this->OnChangeAttribute();
    moving_blocked_ = false;
    owner_->GetStatusManager()->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, false);
}

bool nsSync::CSSyncTuner::CheckClientAbusedData(ulong client_test_result)
{
    //return true; // for debug
    return kbmove_checker_.CheckClientBattleSerial(client_test_result);
}

bool nsSync::CSSyncTuner::ReceiveReply(const MSG_CG_MAP_KBMOVE_SERIAL_CHECK_ACK& reply_msg)
{
    return kbmove_checker_.ReceiveReply(reply_msg);
}

bool nsSync::CSSyncTuner::EnabledMoving(ulong received_serial_key) const
{
    if (kbmove_checker_.EnabledMoving(received_serial_key) == false) {
        owner_->GetStatusManager()->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, true);
        return false;
    };

    return true;
}

bool nsSync::CSSyncTuner::EnabledStopping(ulong received_stop_serial_key) const
{
    if (kbmove_checker_.EnabledStopping(received_stop_serial_key) == false) {
        owner_->GetStatusManager()->ChangeInterState(eCHAR_STATE_SPEED_HACK_PREVENTION, true);
        return false;
    };

    return true;
}

//bool nsSync::CSSyncTuner::UpdateKBMoveCheckRefresh(
//    MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD* const msg_cmd)
//{
//}
//
bool nsSync::CSSyncTuner::ReceiveClientLocalTest(
    const MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN& test_msg)
{
    return kbmove_checker_.ReceiveClientLocalTest(test_msg);
}

//bool nsSync::CSSyncTuner::UpdateClientLocatTestRefresh(
//    MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK* const reply_msg)
//{
//}

//==================================================================================================

