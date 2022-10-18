/*
 * LottoState.h
 */
#ifndef _LOTTO_STATE_H_
#define _LOTTO_STATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _LOTTO_DATA_H_
#include "LottoData.h"
#endif

struct MSG_BASE;
class Lotto;
class WorldSystemMemo;

// ���� �ζ� ���� ��� Ŭ����
class LottoState
{
public:
    LottoState(BYTE status);
    virtual ~LottoState() {}

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void OnTick() {}

    virtual void OnPlayerJoin(CHARGUID charGuid);
    virtual void OnPlayerLeave(CHARGUID charGuid);

    virtual void OnInfoRequest(CHARGUID charGuid, MSG_BASE* packet);
    virtual void OnEntryRequest(CHARGUID charGuid, MSG_BASE* packet);
    virtual void OnNoticeRequest(CHARGUID charGuid, MSG_BASE* packet);
    virtual void OnNoticeConfirm(CHARGUID charGuid, MSG_BASE* packet);

    virtual void OnEntryLoad(MSG_BASE* packet);
    virtual void OnLoad(MSG_BASE* packet) { __UNUSED(packet); }
    virtual void OnEntrySave(MSG_BASE* packet) { __UNUSED(packet); }
    virtual void OnWinnerList(MSG_BASE* packet) { __UNUSED(packet); }
    virtual void OnRollOver(MSG_BASE* packet) { __UNUSED(packet); }
    virtual void OnTicketAcquisition(MSG_BASE* packet) { __UNUSED(packet); }
    virtual void OnReward(MSG_BASE* packet);
    virtual void OnNoticeList(MSG_BASE* packet);
    virtual void OnNoticeDelete(MSG_BASE* packet);
    virtual void OnHeimConsumption(MSG_BASE* packet) { __UNUSED(packet); }
    virtual void OnActualReward(MSG_BASE* packet);
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    virtual void OnTicketPurchase(MSG_BASE* packet) { __UNUSED(packet); }
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    virtual void OnTicketActivation(MSG_BASE* packet);
#endif

    virtual void OnTotalAccumChange(INT64 delta) { __UNUSED(delta); }

protected:
    // Ŭ���̾�Ʈ ��Ŷ �۽� �Լ���
    //
    // [OUT] MSG_CW_LOTTO_START_BRD
    void BroadcastLottoStart(int drawNum);
    // [OUT] MSG_CW_LOTTO_COUNTDOWN_BRD
    void BroadcastCountdown(int secondsTo);
    // [OUT] MSG_CW_LOTTO_FINISH_BRD
    void BroadcastLottoFinish();
    // [OUT] MSG_CW_LOTTO_INFO_ACK
    void SendInfoResponse(CHARGUID charGuid, BYTE flag);
    // [OUT] MSG_CW_LOTTO_TICKET_NTF
    void NotifyTicketAcquisition(CHARGUID charGuid, int numRemainingTickets);
    
    // ���� ��Ŷ �۽� �Լ���
    //
    // [OUT] MSG_DW_LOTTO_LOAD_ENTRY_SYN
    void RequestEntryLoad(CHARGUID charGuid, BYTE flag, bool inPreLoading = false);
    // [OUT] MSG_DW_LOTTO_LOAD_SYN
    void RequestLoad();
    // [OUT] MSG_DW_LOTTO_WINNER_LIST_SYN
    void RequestWinnerList(int page);
    // [OUT] MSG_DW_LOTTO_ADD_TICKET_SYN
    void RequestTicketAdd(CHARGUID charGuid, int numRemainingTickets);
    // [OUT] MSG_DW_LOTTO_SAVE_ENTRY_SYN
    void RequestEntrySave(CHARGUID charGuid, BYTE isAuto, BYTE status, BYTE* pickedNum);
    // [OUT] MSG_DW_LOTTO_ROLLOVER_SYN
    void RequestRollOver(WORD currentDrawNum, INT8 numWinners, 
        INT64 jackpot, INT64 prize2, INT64 prize3, BYTE* winningNum);
    // [OUT] MSG_DW_LOTTO_ACCUM_NTF
    void RequestAccumulate(WORD currentDrawNum, INT64 totalAccum);
    // [OUT] MSG_DW_LOTTO_REWARD_SYN
    void RequestReward(LottoWinner& winner);
    // [OUT] MSG_DW_LOTTO_NOTICE_LIST_SYN
    void RequestNoticeList(CHARGUID charGuid);
    // [OUT] MSG_DW_LOTTO_PAYMENT_UPDATE_NTF
    void RequestPaymentUpdate(CHARGUID charGuid, DWORD effectivePayment);

    // ��Ŷ �۽� ��ƿ��Ƽ �Լ���
    bool SendToClient(CHARGUID charGuid, BYTE* msg, WORD wSize); // Ư�� ĳ���Ϳ��� ��Ŷ ����
    void BroadcastToClients(BYTE* msg, WORD wSize);              // ��ü ����ڿ��� ��Ŷ ����
    bool SendToDBProxy(BYTE* msg, WORD wSize);                   // DB Proxy�� ��Ŷ ����
    bool SendToAgentServer(CHARGUID charGuid, BYTE* msg, WORD wSize); // Agent Server�� ��Ŷ ����

protected:
    Lotto& lotto_;  // ���� Lotto �ν��Ͻ� ������
    BYTE   status_; // Ȱ��ȭ ����
    //
private:
    LottoState();
    __DISABLE_COPY(LottoState);
};

#endif // _LOTTO_STATE_H_
