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

    // ���� �ζ� ���� �ε�
    config_.Init();

    // ��÷ ���� �ڷ� ���� �Ҵ�
    numWinners_ = new int[config_.GetNumPrizes()];
    prizes_ = new INT64[config_.GetNumPrizes()];

    // �� ���� ��ü �ʱ�ȭ
    states_[Lotto::INITIAL_STATE]   = new LottoInitialState();
    states_[Lotto::ACTIVE_STATE]    = new LottoActiveState();
    states_[Lotto::DRAWING_STATE]   = new LottoDrawingState();
    states_[Lotto::INACTIVE_STATE]  = new LottoInactiveState();
    states_[Lotto::FINAL_STATE]     = new LottoFinalState();

    // �ʱ� ����
    currentState_ = NULL;
    TransitState(Lotto::INITIAL_STATE);
}

Lotto::~Lotto()
{
    if (currentState_ != NULL) {
        currentState_->OnExit();
        currentState_ = NULL;
    }
    
    // ���� ��ü ����
    for (int i = 0; i < Lotto::NUM_STATES; ++i) {
        if (states_[i] != NULL) { delete states_[i]; }
    }

    // ��÷ ���� �ڷ� ���� ����
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

    // ���� ������ ����� ���ÿ� ��÷ ���� �Ǵ�
    // [TODO] ��÷ �Ǵ� ���� ��÷ �������� �ű�� �� ����
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
    // ���� �Ǹ� ��� ����
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

    // ������ ����
    float rate;
    if (percentage > -1) {
        // �������� �����Ǿ� ������ �ش� ������ ���
        rate = (float)percentage * 0.01f;
    }
    else {
        // �ƴϸ� �۷ι� ������ ���
        rate = config_.GetAccumulationRate();
    }

    // �� ������ ����
    results_[0].totalAccum += (INT64)(amount * rate);

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // ����� ĳ�� �Ǹ� ����� Ȱ��ȭ�Ǹ� ���� ������ ���� �ڵ� ������ ������ �ʴ´�.
    return -1;
#else
    // ĳ���� ���� ���⿡ ���� ���� ���� �ڵ� ����
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
    // ��� ���
    for (int i = 0; i < min(numResults_, LottoConfig::NUM_RECENT_DRAWS - 1); ++i) {
        results_[i + 1] = results_[i];
    }

    LottoResult& currentDrawResult = results_[0];
    currentDrawResult.Reset();
    if (numResults_ > 0) {
        currentDrawResult.drawNum = results_[1].drawNum + 1;
        // ��ȸ 1�� ��÷�ڰ� ������ 1�� ��÷�� �̿� ó��
        if (results_[1].numWinners == 0) {
            currentDrawResult.totalAccum = results_[1].jackpot;
            results_[1].jackpot = 0;
        }
    }
    else {
        currentDrawResult.drawNum = 1;
    }
    // ��ȸ ��÷ ��ȣ ����
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        currentDrawResult.winningNum[i] = nextWinningNum[i];
    }
    numResults_ = min(numResults_ + 1, LottoConfig::NUM_RECENT_DRAWS);
    ASSERT(numResults_ > 0);
    if (numResults_ <= 0) { return; }

    // ���� ���� ���� ���� �� ���ο� ���� ȸ�� ���� ���� �ʱ�ȭ
    int minDrawNum = results_[numResults_ - 1].drawNum;
    LottoEntryMap::iterator it(entries_.begin());
    LottoEntryMap::iterator end(entries_.end());
    for ( ; it != end; ++it) {
        LottoEntry& entry = it->second;
        entry.payment = 0;
        // ���� ���� ���� ������ ����
        entry.numUsedTickets = 0;
        LottoTicketList& tickets = entry.tickets;
        while (!tickets.empty() && (int)tickets.back().drawNum < minDrawNum) {
            tickets.pop_back();
        }
    }
}

int Lotto::CheckMatches(BYTE* numbers) const
{
    // ���� ���� ���� ȸ���� ������ -1�� ����
    if (numResults_ < 1) { return -1; }
    // [TODO] ���� �ʿ�
    int n = 0;
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        for (int j = 0; j < LottoConfig::NUM_PICKS; ++j) {
            register BYTE refNum = results_[0].winningNum[j];
            if (numbers[i] == refNum) {
                ++n;
                break;
            }
            // ������������ ���ĵǾ� �ֱ� ������
            else if (numbers[i] < refNum) {
                break;
            }
        }
    }
    return n;
}

void Lotto::PickRandomNumbers(BYTE* numbers)
{
    // [TODO] ���� �ʿ� : �ϴ��� �����ϰ� �ܼ��� ����
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
    
    // ���� �ݾ� ���� ó��
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

            // ��ݺ��� ���� �ݾ� ��÷���� ������ ����
            funds = max(funds - rankTotalPrize, 0);

            SUNLOG(eFULL_LOG, "[Lotto] %d Prize %I64d x %d", i, prizes_[i - 1], numWinners);
        }
    }

    if (funds > 0)
    {
        // ����� �������� ������ ���� ���� �ݾ� ó��
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
                if (i > 1) { // 1�� �̸�
                    rankTotalPrize = (INT64)(funds * prizeSetting->value.ratio);
                    if (numWinners > 0) {
                        prizes_[i - 1] = (INT64)(rankTotalPrize / numWinners);
                    }
                    else {
                        prizes_[i - 1] = rankTotalPrize;
                        bonusRate += prizeSetting->value.ratio; // 1�� ��÷�ݿ� �ջ�
                    }
                }
                else { // 1��
                    rankTotalPrize = (INT64)(funds * (prizeSetting->value.ratio + bonusRate));
                    if (numWinners > 0) {
                        prizes_[i - 1] = (INT64)(rankTotalPrize / numWinners);
                    }
                    else {
                        prizes_[i - 1] = rankTotalPrize;
                    }

                    // ���� ȸ�� ��� ���ڵ忡 ����
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
    if (currentState_ != NULL) { currentState_->OnExit(); } // ���� ���� Ż�� �׼�
    LottoState* state = states_[newState];
    state->OnEnter();                                       // ���ο� ���� ���� �׼�
    currentState_ = state;                                  // ���� ����
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
    // ������ ��÷ �ܰ�� ����
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

