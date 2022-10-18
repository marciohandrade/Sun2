#include "stdafx.h"

#include "misc.h"

#include "LottoStates/InitialState.h"
#include "LottoStates/ActiveState.h"
#include "LottoStates/DrawingState.h"
#include "LottoStates/InactiveState.h"
#include "LottoStates/FinalState.h"

#include "Lotto.h"

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

static int compareByte(const void* arg1, const void* arg2);

Lotto Lotto::instance_;

Lotto::Lotto()
  : numResults_(0)
  , lastDrawNum_(0)
  , numWinners_(NULL)
  , prizes_(NULL)
{
    srand((unsigned)time(NULL));

    // 하임 로또 설정 로딩
    config_.Init();

    // 당첨 보상 자료 공간 할당
    numWinners_ = new int[config_.GetNumPrizes()];
    prizes_ = new INT64[config_.GetNumPrizes()];

    // 각 상태 객체 초기화
    states_[Lotto::INITIAL_STATE]   = new LottoInitialState();
    states_[Lotto::ACTIVE_STATE]    = new LottoActiveState();
    states_[Lotto::DRAWING_STATE]   = new LottoDrawingState();
    states_[Lotto::INACTIVE_STATE]  = new LottoInactiveState();
    states_[Lotto::FINAL_STATE]     = new LottoFinalState();

    // 초기 전이
    currentState_ = NULL;
    TransitState(Lotto::INITIAL_STATE);
}

Lotto::~Lotto()
{
    if (currentState_ != NULL) {
        currentState_->OnExit();
        currentState_ = NULL;
    }
    
    // 상태 객체 해제
    for (int i = 0; i < Lotto::NUM_STATES; ++i) {
        if (states_[i] != NULL) { delete states_[i]; }
    }

    // 당첨 보상 자료 공간 해제
    if (prizes_ != NULL) { delete[] prizes_; }
    if (numWinners_ != NULL) { delete[] numWinners_; }
}

INT64 Lotto::AddCurrentTotalAccum(INT64 delta)
{
    if (numResults_ <= 0) {
        return 0;
    }
    INT64& totalAccum = results_[0].totalAccum;
    totalAccum += delta;
    if (totalAccum < 0) {
        totalAccum = 0;
    }
    return totalAccum;
}

LottoResult* Lotto::GetResult(int index)
{
    ASSERT(0 <= index && index < numResults_);
    if (index < 0 || numResults_ <= index) { return NULL; }
    return &results_[index];
}

bool Lotto::ProcessTicket(CHARGUID charGuid, int drawNum, bool isAuto, BYTE* pickedNum, 
                          int* status, int* errorCode)
{
    *errorCode = -1;

    if (drawNum != GetCurrentDrawNum()) {
        *errorCode = 2;
        return false;
    }

    LottoEntry* entry = FindEntry(charGuid);
    if (entry == NULL) {
        *errorCode = 0;
        return false;
    }

    if (entry->numRemainingTickets <= 0) {
        *errorCode = 1;
        return false;
    }

    if (isAuto) {
        PickRandomNumbers(pickedNum);
    }

    // 지금 구현은 응모와 동시에 당첨 여부 판단
    // [TODO] 당첨 판단 논리를 당첨 조건으로 옮기는 것 검토
    int numMatches = CheckMatches(pickedNum);
    switch (numMatches)
    {
    case 4:
        *status = 1;
        break;
    case 3:
        *status = 2;
        break;
    case 2:
        *status = 3;
        break;
    default:
        *status = 0;
    }
    return true;
}

void Lotto::EnterTicket(CHARGUID charGuid, int drawNum, int status, BYTE* pickedNum)
{
    LottoEntry* entry = FindEntry(charGuid);
    if (entry == NULL) { return; }

    LottoTicket ticket;

    ticket.drawNum = drawNum;
    ticket.status = status;
    ticket.pickedNum1 = pickedNum[0];
    ticket.pickedNum2 = pickedNum[1];
    ticket.pickedNum3 = pickedNum[2];
    ticket.pickedNum4 = pickedNum[3];

    entry->tickets.push_front(ticket);

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    // 복권 판매 대금 적립
    results_[0].totalAccum += config_.GetTicketPrice();
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    --entry->numRemainingTickets;
    ++entry->numUsedTickets;
}


LottoEntry* Lotto::FindEntry(CHARGUID charGuid)
{
    LottoEntryMap::iterator it = entries_.find(charGuid);
    if (it == entries_.end()) { return NULL; }
    return &(it->second);
}


LottoEntry* Lotto::NewEntry(CHARGUID charGuid)
{
    LottoEntryMap::iterator it = entries_.find(charGuid);
    if (it == entries_.end()) {
        LottoEntry entry;
        entries_.insert(std::pair<CHARGUID, LottoEntry>(charGuid, entry));
        it = entries_.find(charGuid);
        ASSERT(it != entries_.end());
    }
    return &(it->second);
}

void Lotto::ReleaseEntry(CHARGUID charGuid)
{
    entries_.erase(charGuid);
}

int Lotto::OnHeimConsumption(CHARGUID charGuid, eHEIM_CONSUMPTION_TYPE type, INT64 amount, int percentage)
{
    if (config_.IsEffectiveConsumption(type) == false) { return -1; }

    // 적립율 적용
    float rate;
    if (percentage > -1) {
        // 적립율이 지정되어 있으면 해당 적립율 사용
        rate = (float)percentage * 0.01f;
    }
    else {
        // 아니면 글로벌 적립율 사용
        rate = config_.GetAccumulationRate();
    }

    // 총 적립액 갱신
    results_[0].totalAccum += (INT64)(amount * rate);

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // 응모권 캐시 판매 기능이 활성화되면 하임 적립에 따른 자동 지급은 사용되지 않는다.
    return -1;
#else
    // 캐릭터 별로 지출에 따라 복권 용지 자동 지급
    LottoEntry* entry = FindEntry(charGuid);
    if (entry == NULL) { return -1; }
    int numTotalTickets = entry->numRemainingTickets + entry->numUsedTickets;
    if (numTotalTickets >= LottoConfig::MAX_TICKETS) { return -1; }

    INT64 payment = entry->payment;
    payment += amount;
    if (payment < config_.GetTicketPrice()) {
        entry->payment = (DWORD)payment;
        return -1;
    }

    int numAffordableTickets = (int)(payment / config_.GetTicketPrice());
    entry->payment = (DWORD)(payment % config_.GetTicketPrice());
    int numNewTickets = min(numAffordableTickets, (LottoConfig::MAX_TICKETS - numTotalTickets));
    int numTickets = entry->numRemainingTickets + numNewTickets;
    numTotalTickets = numTickets + entry->numUsedTickets;
    if (numTotalTickets >= LottoConfig::MAX_TICKETS) { entry->payment = 0; }

    return numTickets;
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET
}

void Lotto::RollOver(BYTE* nextWinningNum)
{
    // 결과 목록
    for (int i = 0; i < min(numResults_, LottoConfig::NUM_RECENT_DRAWS - 1); ++i) {
        results_[i + 1] = results_[i];
    }

    LottoResult& currentDrawResult = results_[0];
    currentDrawResult.Reset();
    if (numResults_ > 0) {
        currentDrawResult.drawNum = results_[1].drawNum + 1;
        // 전회 1등 당첨자가 없으면 1등 당첨금 이월 처리
        if (results_[1].numWinners == 0) {
            currentDrawResult.totalAccum = results_[1].jackpot;
            results_[1].jackpot = 0;
        }
    }
    else {
        currentDrawResult.drawNum = 1;
    }
    // 차회 당첨 번호 저장
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        currentDrawResult.winningNum[i] = nextWinningNum[i];
    }
    numResults_ = min(numResults_ + 1, LottoConfig::NUM_RECENT_DRAWS);
    ASSERT(numResults_ > 0);
    if (numResults_ <= 0) { return; }

    // 과거 응모 내역 삭제 및 새로운 현재 회차 응모 내역 초기화
    int minDrawNum = results_[numResults_ - 1].drawNum;
    LottoEntryMap::iterator it(entries_.begin());
    LottoEntryMap::iterator end(entries_.end());
    for ( ; it != end; ++it) {
        LottoEntry& entry = it->second;
        entry.payment = 0;
        // 보유 중인 복권 용지는 유지
        entry.numUsedTickets = 0;
        LottoTicketList& tickets = entry.tickets;
        while (!tickets.empty() && (int)tickets.back().drawNum < minDrawNum) {
            tickets.pop_back();
        }
    }
}

int Lotto::CheckMatches(BYTE* numbers) const
{
    // 현재 진행 중인 회차가 없으면 -1을 리턴
    if (numResults_ < 1) { return -1; }
    // [TODO] 개선 필요
    int n = 0;
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        for (int j = 0; j < LottoConfig::NUM_PICKS; ++j) {
            register BYTE refNum = results_[0].winningNum[j];
            if (numbers[i] == refNum) {
                ++n;
                break;
            }
            // 오름차순으로 정렬되어 있기 때문에
            else if (numbers[i] < refNum) {
                break;
            }
        }
    }
    return n;
}

void Lotto::PickRandomNumbers(BYTE* numbers)
{
    // [TODO] 개선 필요 : 일단은 순진하게 단순한 구현
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        int pick;
        bool isUnique = false;
        do {
            pick = random(LottoConfig::MIN_NUMBER, LottoConfig::MAX_NUMBER);
            int j = 0;
            for (; j < i; ++j) {
                if (pick == numbers[j]) { break; }
            }
            if (j == i) { isUnique = true; }
        } while (!isUnique);
        numbers[i] = pick;
    }
    Sort(numbers);
}

void Lotto::Sort(BYTE* numbers)
{
    qsort(numbers, LottoConfig::NUM_PICKS, sizeof(BYTE), compareByte);
}

bool Lotto::AreValidPicks(BYTE* numbers)
{
    DWORD32 marks = 0;
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        BYTE n = numbers[i];
        if ( n < LottoConfig::MIN_NUMBER || LottoConfig::MAX_NUMBER < n ) { return false; }
        if ( (marks & (1 << n)) != 0 ) { return false; }
        marks |= (1 << n);
    }
    return true;
}

void Lotto::CalculatePrizes()
{
    INT64 funds = GetCurrentTotalAccum();
    
    // 고정 금액 먼저 처리
    for (int i = config_.GetNumPrizes(); i > 0; --i)
    {
        const LottoPrize* prizeSetting = config_.GetPrize(i);
        ASSERT(prizeSetting != NULL);
        if (prizeSetting == NULL) { continue; }

        if (prizeSetting->type == LottoPrize::TYPE_AMOUNT)
        {
            int numWinners = numWinners_[i - 1];
            INT64 rankTotalPrize = prizeSetting->value.amount * numWinners;
            prizes_[i - 1] = prizeSetting->value.amount;

            // 기금보다 고정 금액 당첨금이 많으면 적자
            funds = max(funds - rankTotalPrize, 0);

            SUNLOG(eFULL_LOG, "[Lotto] %d Prize %I64d x %d", i, prizes_[i - 1], numWinners);
        }
    }

    if (funds > 0)
    {
        // 기금이 남았으면 비율에 의한 가변 금액 처리
        double bonusRate = 0.0f;
        for (int i = config_.GetNumPrizes(); i > 0; --i)
        {
            const LottoPrize* prizeSetting = config_.GetPrize(i);
            ASSERT(prizeSetting != NULL);
            if (prizeSetting == NULL) { continue; }

            if (prizeSetting->type == LottoPrize::TYPE_RATIO)
            {
                int numWinners = numWinners_[i - 1];
                INT64 rankTotalPrize;
                if (i > 1) { // 1등 미만
                    rankTotalPrize = (INT64)(funds * prizeSetting->value.ratio);
                    if (numWinners > 0) {
                        prizes_[i - 1] = (INT64)(rankTotalPrize / numWinners);
                    }
                    else {
                        prizes_[i - 1] = rankTotalPrize;
                        bonusRate += prizeSetting->value.ratio; // 1등 당첨금에 합산
                    }
                }
                else { // 1등
                    rankTotalPrize = (INT64)(funds * (prizeSetting->value.ratio + bonusRate));
                    if (numWinners > 0) {
                        prizes_[i - 1] = (INT64)(rankTotalPrize / numWinners);
                    }
                    else {
                        prizes_[i - 1] = rankTotalPrize;
                    }

                    // 현재 회차 결과 레코드에 저장
                    ASSERT(numResults_ > 0);
                    if (numResults_ > 0) {
                        results_[0].numWinners = numWinners;
                        results_[0].jackpot = prizes_[i - 1];
                    }
                }

                SUNLOG(eFULL_LOG, "[Lotto] %d Prize %I64d x %d", i, prizes_[i - 1], numWinners);
            }
        }
    }
}

void Lotto::TransitState(Lotto::State newState)
{
    ASSERT(newState < Lotto::NUM_STATES);
    if (newState >= Lotto::NUM_STATES) { return; }
    if (currentState_ != NULL) { currentState_->OnExit(); } // 기존 상태 탈출 액션
    LottoState* state = states_[newState];
    state->OnEnter();                                       // 새로운 상태 진입 액션
    currentState_ = state;                                  // 상태 전이
}

void Lotto::GetCurrentWinningNum(BYTE* numbers) const
{
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        numbers[i] = 0;
    }
    if (numResults_ < 1) { return; }
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        numbers[i] = results_[0].winningNum[i];
    }
}

void Lotto::ForceRollOver()
{
    // 강제로 추첨 단계로 진행
    TransitState(Lotto::DRAWING_STATE);
}

static int compareByte(const void* arg1, const void* arg2)
{
    return (*(BYTE*)arg1 - *(BYTE*)arg2);
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET

void TicketActivationXactions::Add(CHARGUID char_guid, SLOTIDX index, POSTYPE pos)
{
    if (Find(char_guid) != NULL) {
        return;
    }
    TicketActivationXactionInfo info;
    info.index = index;
    info.pos   = pos;
    map_.insert(std::pair<CHARGUID, TicketActivationXactionInfo>(char_guid, info));
}

void TicketActivationXactions::Remove(CHARGUID char_guid)
{
    map_.erase(char_guid);
}

void TicketActivationXactions::Clear()
{
    map_.clear();
}

const TicketActivationXactionInfo* TicketActivationXactions::Find(CHARGUID char_guid) const
{
    XactionInfoMap::const_iterator it = map_.find(char_guid);
    if (it == map_.end()) {
        return NULL;
    }
    return &(it->second);
}

#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

