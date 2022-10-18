/*
 * ActiveState.h
 */
#ifndef _LOTTO_ACTIVE_STATE_H_
#define _LOTTO_ACTIVE_STATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Timer.h"

#ifndef _LOTTO_STATE_H_
#include "Services/Lotto/LottoState.h"
#endif

// 하임 로또 활성화 상태 클래스
class LottoActiveState : public LottoState
{
public:
    LottoActiveState() : LottoState((BYTE)1) {}
    virtual ~LottoActiveState() {}

    virtual void OnEnter();
    virtual void OnExit();

    virtual void OnTick();

    virtual void OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet);
    virtual void OnEntryRequest(CHARGUID charGuid, const MSG_CW_LOTTO_ENTRY_SYN* packet);

    virtual void OnEntrySave(const MSG_DW_LOTTO_SAVE_ENTRY_ACK* packet);
    virtual void OnTicketAcquisition(const MSG_DW_LOTTO_ADD_TICKET_ACK* packet);
    virtual void OnHeimConsumption(const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF* packet);

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    virtual void OnTicketPurchase(const MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK* packet);
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    virtual void OnTicketActivation(const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* packet);
#endif

    virtual void OnTotalAccumChange(INT64 delta);

private:
    void BroadcastTotalAccum(INT64 totalAccum, bool dbWrite = true);

private:
    INT64 totalAccum_;              // 최근 공지한 총 적립액

    util::Timer tickTimer_;         // 1초 타이머
    util::Timer accumNotiTimer_;    // 총 적립액 갱신 공지 타이머

    ScheduleToken schedule_;        // 추첨 일정 토큰

    size_t reminderIndex_;
};

#endif // _LOTTO_ACTIVE_STATE_H_

