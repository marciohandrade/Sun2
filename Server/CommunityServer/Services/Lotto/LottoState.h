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

    virtual void OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet);
    virtual void OnEntryRequest(CHARGUID charGuid, const MSG_CW_LOTTO_ENTRY_SYN* packet);
    virtual void OnNoticeRequest(CHARGUID charGuid, const MSG_CW_LOTTO_NOTICE_LIST_SYN* packet);
    virtual void OnNoticeConfirm(CHARGUID charGuid, const MSG_CW_LOTTO_NOTICE_CONFIRM* packet);

    virtual void OnEntryLoad(const MSG_DW_LOTTO_LOAD_ENTRY_ACK* packet);
    virtual void OnLoad(const MSG_DW_LOTTO_LOAD_ACK* packet) { __UNUSED(packet); }
    virtual void OnEntrySave(const MSG_DW_LOTTO_SAVE_ENTRY_ACK* packet) { __UNUSED(packet); }
    virtual void OnWinnerList(const MSG_DW_LOTTO_WINNER_LIST_ACK* packet) { __UNUSED(packet); }
    virtual void OnRollOver(const MSG_DW_LOTTO_ROLLOVER_ACK* packet) { __UNUSED(packet); }
    virtual void OnTicketAcquisition(const MSG_DW_LOTTO_ADD_TICKET_ACK* packet) { __UNUSED(packet); }
    virtual void OnReward(const MSG_DW_LOTTO_REWARD_ACK* packet);
    virtual void OnNoticeList(const MSG_DW_LOTTO_NOTICE_LIST_ACK* packet);
    virtual void OnNoticeDelete(const MSG_DW_LOTTO_NOTICE_DEL_ACK* packet);
    virtual void OnHeimConsumption(const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF* packet) { __UNUSED(packet); }
    virtual void OnActualReward(const MSG_AW_USER_LOTTO_REWARD_ACK* packet);
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    virtual void OnTicketPurchase(const MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK* packet) { __UNUSED(packet); }
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    virtual void OnTicketActivation(const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* packet);
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
    bool SendToClient(CHARGUID charGuid, MSG_BASE* msg, WORD wSize); // Ư�� ĳ���Ϳ��� ��Ŷ ����
    void BroadcastToClients(MSG_BASE* msg, WORD wSize);              // ��ü ����ڿ��� ��Ŷ ����
    bool SendToDBProxy(MSG_BASE* msg, WORD wSize);                   // DB Proxy�� ��Ŷ ����
    bool SendToAgentServer(CHARGUID charGuid, MSG_BASE* msg, WORD wSize); // Agent Server�� ��Ŷ ����

protected:
    Lotto& lotto_;  // ���� Lotto �ν��Ͻ� ������
    BYTE   status_; // Ȱ��ȭ ����
    //
private:
    LottoState();
    __DISABLE_COPY(LottoState);
};

#endif // _LOTTO_STATE_H_
