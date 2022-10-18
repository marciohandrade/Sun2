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
#include "../LottoState.h"
#endif

// ���� �ζ� Ȱ��ȭ ���� Ŭ����
class LottoActiveState : public LottoState
{
public:
    LottoActiveState() : LottoState((BYTE)1) {}
    virtual ~LottoActiveState() {}

    virtual void OnEnter();
    virtual void OnExit();

    virtual void OnTick();

    virtual void OnInfoRequest(CHARGUID charGuid, MSG_BASE* packet);
    virtual void OnEntryRequest(CHARGUID charGuid, MSG_BASE* packet);

    virtual void OnEntrySave(MSG_BASE* packet);
    virtual void OnTicketAcquisition(MSG_BASE* packet);
    virtual void OnHeimConsumption(MSG_BASE* packet);

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    virtual void OnTicketPurchase(MSG_BASE* packet);
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    virtual void OnTicketActivation(MSG_BASE* packet);
#endif

    virtual void OnTotalAccumChange(INT64 delta);

private:
    void BroadcastTotalAccum(INT64 totalAccum, bool dbWrite = true);

private:
    INT64 totalAccum_;              // �ֱ� ������ �� ������

    util::Timer tickTimer_;         // 1�� Ÿ�̸�
    util::Timer accumNotiTimer_;    // �� ������ ���� ���� Ÿ�̸�

    ScheduleToken schedule_;        // ��÷ ���� ��ū

    size_t reminderIndex_;
};

#endif // _LOTTO_ACTIVE_STATE_H_
