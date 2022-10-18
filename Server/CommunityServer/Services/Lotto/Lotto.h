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

// 하임 로또 클래스
class Lotto
{
public:
    ~Lotto();

    // 서버 메인 루프로부터 호출되는 OnTick() 함수
    void Update() { currentState_->OnTick(); }

    // 플레이어 캐릭터 접속/해제 핸들러
    void OnPlayerJoin(CHARGUID charGuid) { currentState_->OnPlayerJoin(charGuid); }
    void OnPlayerLeave(CHARGUID charGuid) { currentState_->OnPlayerLeave(charGuid); }

    // 클라이언트 수신 패킷 핸들러
    //
    // [IN] MSG_CW_LOTTO_INFO_SYN
    void OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet) { currentState_->OnInfoRequest(charGuid, packet); }
    // [IN] MSG_CW_LOTTO_ENTRY_SYN
    void OnEntryRequest(CHARGUID charGuid, const MSG_CW_LOTTO_ENTRY_SYN* packet) { currentState_->OnEntryRequest(charGuid, packet); }
    // [IN] MSG_CW_LOTTO_NOTICE_LIST_SYN
    void OnNoticeRequest(CHARGUID charGuid, const MSG_CW_LOTTO_NOTICE_LIST_SYN* packet) { currentState_->OnNoticeRequest(charGuid, packet); }
    // [IN] MSG_CW_LOTTO_NOTICE_CONFIRM
    void OnNoticeConfirm(CHARGUID charGuid, const MSG_CW_LOTTO_NOTICE_CONFIRM* packet) { currentState_->OnNoticeConfirm(charGuid, packet); }

    // 서버 수신 패킷 핸들러
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

    // 현재 진행 중인 회차 번호를 리턴한다
    int GetCurrentDrawNum() const { return (numResults_ > 0 ? results_[0].drawNum : 0); }
    // 현재 진행 중인 회차의 당첨 번호를 채운다 (GM 기능)
    void GetCurrentWinningNum(BYTE* numbers) const;
    // 강제로 추첨 상태로 넘어간다 (GM 기능)
    void ForceRollOver();

    // Lotto 인스턴스 참조를 리턴한다
    static Lotto& GetInstance() { return instance_; }

    // 종료 상태로 전환한다
    void Shutdown() { TransitState(Lotto::FINAL_STATE); }

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    TicketActivationXactions& GetTicketActivationXactions() {
        return ticket_activation_xactions_;
    }
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

private:
    enum State {
        INITIAL_STATE = 0   // 초기 상태
      , ACTIVE_STATE        // 활성화 (적립, 복권 용지 발급 및 응모 가능)
      , DRAWING_STATE       // 추첨중
      , INACTIVE_STATE      // 비활성화 (추첨 직후 차회 시작 전까지, 보상 지급)
      , FINAL_STATE         // 종료중
      , NUM_STATES          // 상태의 개수
    };

    // 직접 인스턴스화 금지
    Lotto();

    // 현재 진행 중인 회차의 총 적립액을 리턴한다
    INT64 GetCurrentTotalAccum() const { return (numResults_ > 0 ? results_[0].totalAccum : 0); }
    // 현재 진행 중인 회차의 총 적립액에 주어진 금액을 더해 결과를 리턴한다
    INT64 AddCurrentTotalAccum(INT64 delta);

    // 주어진 인덱스 위치의 LottoResult 객체 포인터를, 인덱스가 유효하지 않으면 NULL 리턴
    LottoResult* GetResult(int index);

    // 응모 건을 처리한다 
    bool ProcessTicket(CHARGUID charGuid, int drawNum, bool isAuto, BYTE* pickedNum, int* status, int* errorCode);
    // 응모 내역을 추가한다
    void EnterTicket(CHARGUID charGuid, int drawNum, int status, const BYTE* pickedNum);

    // 주어진 캐릭터의 응모 내역을 검색해 리턴한다. 없으면 NULL을 리턴한다.
    LottoEntry* FindEntry(CHARGUID charGuid);
    // 주어진 캐릭터에 대해 빈 응모 내역 캐시 항목을 생성해 리턴한다.
    LottoEntry* NewEntry(CHARGUID charGuid);
    // 주어진 캐릭터의 응모 내역 캐시를 해제한다
    void ReleaseEntry(CHARGUID charGuid);

    // 캐릭터의 하임 소비에 대한 처리
    // 리턴 값이 0보다 작으면 소유한 복권 용지 수에 변화 없음을 의미하고, 0 이상이면 남은 용지 수
    int OnHeimConsumption(CHARGUID charGuid, eHEIM_CONSUMPTION_TYPE type, INT64 amount, int percentage);

    // 차회 당첨 번호를 지정해 추첨/이월 처리를 완료한다
    void RollOver(BYTE* nextWinningNum);

    // 당첨금 계산
    void CalculatePrizes();

    // 상태 전이 함수
    void TransitState(Lotto::State newState);

    // 주어진 바이트 배열이 현재 회차의 당첨 번호와 몇 개가 일치하는지 검사
    int CheckMatches(BYTE* numbers) const;
    // 주어진 바이트 배열을 자동 선택 난수로 채운다
    static void PickRandomNumbers(BYTE* numbers);
    // 주어진 바이트 배열을 오름차순으로 정렬한다
    static void Sort(BYTE* numbers);
    // 주어진 바이트 배열이 유효한 응모 번호인지 검사
    static bool AreValidPicks(BYTE* numbers);

    friend class LottoState;
    // 구체적인 상태 클래스들을 friend로 선언
    friend class LottoInitialState;
    friend class LottoActiveState;
    friend class LottoDrawingState;
    friend class LottoInactiveState;

    int             numResults_;                                // 관리되는 추첨 결과의 수
    LottoResult     results_[LottoConfig::NUM_RECENT_DRAWS];    // 현재와 지난 회차 추첨 결과
    LottoEntryMap   entries_;                                   // 캐릭터별 응모 내역
    int             lastDrawNum_;                               // 최근 추첨 회차 번호
    int*            numWinners_;                                // 등위별 당첨자 수
    INT64*          prizes_;                                    // 등위별 당첨금
    LottoWinnerList winners_;                                   // 당첨자 목록

    LottoConfig config_;                                        // 하임 로또 설정 객체

    LottoState* currentState_;                                  // 현재 상태 객체 포인터
    LottoState* states_[Lotto::NUM_STATES];                     // 상태 객체 포인터 배열

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    TicketActivationXactions ticket_activation_xactions_;
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

    static Lotto instance_;                                     // Singleton 인스턴스
};

#endif // _LOTTO_H_
