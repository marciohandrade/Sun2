/*
 * Lotto.h
 */
#ifndef _LOTTO_H_
#define _LOTTO_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _LOTTO_STATE_H_
#include "LottoState.h"
#endif

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
struct TicketActivationXactionInfo
{
    SLOTIDX index;
    POSTYPE pos;
};

class TicketActivationXactions
{
public:
    TicketActivationXactions() {}

    void Add(CHARGUID char_guid, SLOTIDX index, POSTYPE pos);
    void Remove(CHARGUID char_guid);
    void Clear();

    const TicketActivationXactionInfo* Find(CHARGUID char_guid) const;
private:
    typedef stdext::hash_map<CHARGUID, TicketActivationXactionInfo> XactionInfoMap;

    XactionInfoMap map_;
};
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

// ���� �ζ� Ŭ����
class Lotto
{
public:
    ~Lotto();

    // ���� ���� �����κ��� ȣ��Ǵ� OnTick() �Լ�
    void Update() { currentState_->OnTick(); }

    // �÷��̾� ĳ���� ����/���� �ڵ鷯
    void OnPlayerJoin(CHARGUID charGuid) { currentState_->OnPlayerJoin(charGuid); }
    void OnPlayerLeave(CHARGUID charGuid) { currentState_->OnPlayerLeave(charGuid); }

    // Ŭ���̾�Ʈ ���� ��Ŷ �ڵ鷯
    //
    // [IN] MSG_CW_LOTTO_INFO_SYN
    void OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet) { currentState_->OnInfoRequest(charGuid, packet); }
    // [IN] MSG_CW_LOTTO_ENTRY_SYN
    void OnEntryRequest(CHARGUID charGuid, const MSG_CW_LOTTO_ENTRY_SYN* packet) { currentState_->OnEntryRequest(charGuid, packet); }
    // [IN] MSG_CW_LOTTO_NOTICE_LIST_SYN
    void OnNoticeRequest(CHARGUID charGuid, const MSG_CW_LOTTO_NOTICE_LIST_SYN* packet) { currentState_->OnNoticeRequest(charGuid, packet); }
    // [IN] MSG_CW_LOTTO_NOTICE_CONFIRM
    void OnNoticeConfirm(CHARGUID charGuid, const MSG_CW_LOTTO_NOTICE_CONFIRM* packet) { currentState_->OnNoticeConfirm(charGuid, packet); }

    // ���� ���� ��Ŷ �ڵ鷯
    //
    // [IN] MSG_DW_LOTTO_LOAD_ENTRY_ACK
    void OnEntryLoad(const MSG_DW_LOTTO_LOAD_ENTRY_ACK* packet) { currentState_->OnEntryLoad(packet); }
    // [IN] MSG_DW_LOTTO_LOAD_ACK
    void OnLoad(const MSG_DW_LOTTO_LOAD_ACK* packet) { currentState_->OnLoad(packet); }
    // [IN] MSG_DW_LOTTO_SAVE_ENTRY_ACK
    void OnEntrySave(const MSG_DW_LOTTO_SAVE_ENTRY_ACK* packet) { currentState_->OnEntrySave(packet); }
    // [IN] MSG_DW_LOTTO_WINNER_LIST_ACK
    void OnWinnerList(const MSG_DW_LOTTO_WINNER_LIST_ACK* packet) { currentState_->OnWinnerList(packet); }
    // [IN] MSG_DW_LOTTO_ROLLOVER_ACK
    void OnRollOver(const MSG_DW_LOTTO_ROLLOVER_ACK* packet) { currentState_->OnRollOver(packet); }
    // [IN] MSG_DW_LOTTO_ADD_TICKET_ACK
    void OnTicketAcquisition(const MSG_DW_LOTTO_ADD_TICKET_ACK* packet) { currentState_->OnTicketAcquisition(packet); }
    // [IN] MSG_DW_LOTTO_REWARD_ACK
    void OnReward(const MSG_DW_LOTTO_REWARD_ACK* packet) { currentState_->OnReward(packet); }
    // [IN] MSG_DW_LOTTO_NOTICE_LIST_ACK
    void OnNoticeList(const MSG_DW_LOTTO_NOTICE_LIST_ACK* packet) { currentState_->OnNoticeList(packet); }
    // [IN] MSG_DW_LOTTO_NOTICE_DEL_ACK
    void OnNoticeDelete(const MSG_DW_LOTTO_NOTICE_DEL_ACK* packet) { currentState_->OnNoticeDelete(packet); }
    // [IN] MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF
    void OnHeimConsumption(const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF* packet) { currentState_->OnHeimConsumption(packet); }
    // [IN] MSG_AW_USER_LOTTO_REWARD_ACK
    void OnActualReward(const MSG_AW_USER_LOTTO_REWARD_ACK* packet) { currentState_->OnActualReward(packet); }
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    // [IN] MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK
    void OnTicketPurchase(const MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK* packet) { currentState_->OnTicketPurchase(packet); }
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // [IN] MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN
    void OnTicketActivation(const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* packet) { currentState_->OnTicketActivation(packet); }
#endif //

    void OnTotalAccumChange(INT64 delta) { currentState_->OnTotalAccumChange(delta); }

    // ���� ���� ���� ȸ�� ��ȣ�� �����Ѵ�
    int GetCurrentDrawNum() const { return (numResults_ > 0 ? results_[0].drawNum : 0); }
    // ���� ���� ���� ȸ���� ��÷ ��ȣ�� ä��� (GM ���)
    void GetCurrentWinningNum(BYTE* numbers) const;
    // ������ ��÷ ���·� �Ѿ�� (GM ���)
    void ForceRollOver();

    // Lotto �ν��Ͻ� ������ �����Ѵ�
    static Lotto& GetInstance() { return instance_; }

    // ���� ���·� ��ȯ�Ѵ�
    void Shutdown() { TransitState(Lotto::FINAL_STATE); }

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    TicketActivationXactions& GetTicketActivationXactions() {
        return ticket_activation_xactions_;
    }
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

private:
    enum State {
        INITIAL_STATE = 0   // �ʱ� ����
      , ACTIVE_STATE        // Ȱ��ȭ (����, ���� ���� �߱� �� ���� ����)
      , DRAWING_STATE       // ��÷��
      , INACTIVE_STATE      // ��Ȱ��ȭ (��÷ ���� ��ȸ ���� ������, ���� ����)
      , FINAL_STATE         // ������
      , NUM_STATES          // ������ ����
    };

    // ���� �ν��Ͻ�ȭ ����
    Lotto();

    // ���� ���� ���� ȸ���� �� �������� �����Ѵ�
    INT64 GetCurrentTotalAccum() const { return (numResults_ > 0 ? results_[0].totalAccum : 0); }
    // ���� ���� ���� ȸ���� �� �����׿� �־��� �ݾ��� ���� ����� �����Ѵ�
    INT64 AddCurrentTotalAccum(INT64 delta);

    // �־��� �ε��� ��ġ�� LottoResult ��ü �����͸�, �ε����� ��ȿ���� ������ NULL ����
    LottoResult* GetResult(int index);

    // ���� ���� ó���Ѵ� 
    bool ProcessTicket(CHARGUID charGuid, int drawNum, bool isAuto, BYTE* pickedNum, int* status, int* errorCode);
    // ���� ������ �߰��Ѵ�
    void EnterTicket(CHARGUID charGuid, int drawNum, int status, const BYTE* pickedNum);

    // �־��� ĳ������ ���� ������ �˻��� �����Ѵ�. ������ NULL�� �����Ѵ�.
    LottoEntry* FindEntry(CHARGUID charGuid);
    // �־��� ĳ���Ϳ� ���� �� ���� ���� ĳ�� �׸��� ������ �����Ѵ�.
    LottoEntry* NewEntry(CHARGUID charGuid);
    // �־��� ĳ������ ���� ���� ĳ�ø� �����Ѵ�
    void ReleaseEntry(CHARGUID charGuid);

    // ĳ������ ���� �Һ� ���� ó��
    // ���� ���� 0���� ������ ������ ���� ���� ���� ��ȭ ������ �ǹ��ϰ�, 0 �̻��̸� ���� ���� ��
    int OnHeimConsumption(CHARGUID charGuid, eHEIM_CONSUMPTION_TYPE type, INT64 amount, int percentage);

    // ��ȸ ��÷ ��ȣ�� ������ ��÷/�̿� ó���� �Ϸ��Ѵ�
    void RollOver(BYTE* nextWinningNum);

    // ��÷�� ���
    void CalculatePrizes();

    // ���� ���� �Լ�
    void TransitState(Lotto::State newState);

    // �־��� ����Ʈ �迭�� ���� ȸ���� ��÷ ��ȣ�� �� ���� ��ġ�ϴ��� �˻�
    int CheckMatches(BYTE* numbers) const;
    // �־��� ����Ʈ �迭�� �ڵ� ���� ������ ä���
    static void PickRandomNumbers(BYTE* numbers);
    // �־��� ����Ʈ �迭�� ������������ �����Ѵ�
    static void Sort(BYTE* numbers);
    // �־��� ����Ʈ �迭�� ��ȿ�� ���� ��ȣ���� �˻�
    static bool AreValidPicks(BYTE* numbers);

    friend class LottoState;
    // ��ü���� ���� Ŭ�������� friend�� ����
    friend class LottoInitialState;
    friend class LottoActiveState;
    friend class LottoDrawingState;
    friend class LottoInactiveState;

    int             numResults_;                                // �����Ǵ� ��÷ ����� ��
    LottoResult     results_[LottoConfig::NUM_RECENT_DRAWS];    // ����� ���� ȸ�� ��÷ ���
    LottoEntryMap   entries_;                                   // ĳ���ͺ� ���� ����
    int             lastDrawNum_;                               // �ֱ� ��÷ ȸ�� ��ȣ
    int*            numWinners_;                                // ������ ��÷�� ��
    INT64*          prizes_;                                    // ������ ��÷��
    LottoWinnerList winners_;                                   // ��÷�� ���

    LottoConfig config_;                                        // ���� �ζ� ���� ��ü

    LottoState* currentState_;                                  // ���� ���� ��ü ������
    LottoState* states_[Lotto::NUM_STATES];                     // ���� ��ü ������ �迭

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    TicketActivationXactions ticket_activation_xactions_;
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

    static Lotto instance_;                                     // Singleton �ν��Ͻ�
};

#endif // _LOTTO_H_
