#include "stdafx.h"

#include "PacketStruct_DW.h"
#include "ServerSession.h"
#include "ServerSessionManager.h"

#include "../Lotto.h"

#include "InitialState.h"

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

void LottoInitialState::OnEnter()
{
    SUNLOG(eFULL_LOG, "[Lotto] Entering INITIAL_STATE");

    initRetryTimer_.SetTimer(0); // 최초 1회는 강제로 Expired 상태로
    initRetryTimer_.EnableCheckTime();

    lotto_.winners_.clear();

    page_ = 1; // 당첨자 목록 페이지 수 초기화
}

void LottoInitialState::OnExit()
{
    SUNLOG(eFULL_LOG, "[Lotto] Exiting INITIAL_STATE");

    initRetryTimer_.DisableCheckTime();
}

void LottoInitialState::OnTick()
{
    // 초기화 시도
    if (initRetryTimer_.IsExpired())
    {
        SUNLOG(eFULL_LOG, "[Lotto] Initializing Lotto : Loading...");

        RequestLoad();

        // 초기화 재시도 타이머 리셋
        if (initRetryTimer_.GetBaseIntervalTime() > 0) {
            initRetryTimer_.Reset();
        }
        else { 
            initRetryTimer_.SetTimer(60 * 1000);
        }
    }
}

void LottoInitialState::OnLoad(MSG_BASE* packet)
{
    MSG_DW_LOTTO_LOAD_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_LOAD_ACK*>(packet);

    lotto_.numResults_ = pRecvMsg->bNumResultRecords;
    int currentDrawNum = pRecvMsg->wCurrentDrawNum;

    for (int i = 0; i < lotto_.numResults_; ++i)
    {
        LottoResult* result = lotto_.GetResult(i);
        ASSERT(result != NULL);
        if (result == NULL) { continue; }

        result->Reset();

        ServerLottoResult& record = pRecvMsg->results[i];

        result->drawNum = record.wDrawNum;
        if ( (currentDrawNum > 0) && (record.wDrawNum == currentDrawNum) ) {
            // 진행 중인 현재 회차의 경우 총 적립액을 읽어들인다
            result->totalAccum = pRecvMsg->i64TotalAccum;
        }
        else {
            // 현재 회차 전의 가장 높은 회차 번호를 최종 추첨 회차로 선택
            if (lotto_.lastDrawNum_ == 0) { lotto_.lastDrawNum_ = result->drawNum; }
        }
        result->numWinners = record.iNumWinners;
        result->jackpot = record.i64Jackpot;
        for (int j = 0; j < LottoConfig::NUM_PICKS; ++j) {
            result->winningNum[j] = record.bWinningNum[j];
        }
    }

    // 현재 진행 중인 회차가 있다면
    if (lotto_.numResults_ > 0)
    {
        SUNLOG(eFULL_LOG, "[Lotto] Loaded %d results(s). Current draw # = %d, Total accumulation = %I64d", 
            lotto_.numResults_, lotto_.GetCurrentDrawNum(), lotto_.GetCurrentTotalAccum());

        lotto_.TransitState(Lotto::ACTIVE_STATE);
        
        // 활성화하기 전에 직전 회차 보상이 완료되었는지 점검해야 한다
        //RequestWinnerList(page_);
    }
    else
    {
        SUNLOG(eFULL_LOG, "[Lotto] Initializing the 1st draw...");

        // 최초 1회차를 초기화한다.
        lotto_.PickRandomNumbers(nextWinningNum_);
        RequestRollOver(0, 0, 0, 0, 0, nextWinningNum_);
    }
}

void LottoInitialState::OnWinnerList(MSG_BASE* packet)
{
    MSG_DW_LOTTO_WINNER_LIST_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_WINNER_LIST_ACK*>(packet);

    int page = pRecvMsg->iPage;
    int maxPage = pRecvMsg->iMaxPage;
    int numRecs = pRecvMsg->iNumRecords;
    
    ASSERT(numRecs <= LOTTO_NUM_WINNERS_PER_PAGE);
    if (numRecs > LOTTO_NUM_WINNERS_PER_PAGE) { numRecs = LOTTO_NUM_WINNERS_PER_PAGE; }

    for (int i = 0; i < numRecs; ++i)
    {
        LottoWinner winner;

        winner.idx = pRecvMsg->winners[i].i64Idx;
        winner.charGuid = pRecvMsg->winners[i].iCharGuid;
        winner.status = pRecvMsg->winners[i].bStatus;

        lotto_.winners_.push_back(winner);

        ASSERT(0 < winner.status && winner.status <= lotto_.config_.GetNumPrizes());
        ++lotto_.numWinners_[winner.status - 1];

        SUNLOG(eFULL_LOG, "[Lotto] Winner [%d] : %d", winner.charGuid, winner.status);
    }

    if (page < maxPage) {
        RequestWinnerList(++page_);
        return;
    }

    if (lotto_.winners_.empty()) {
        lotto_.TransitState(Lotto::ACTIVE_STATE);
    }
    else {
        lotto_.TransitState(Lotto::INACTIVE_STATE);
    }
}

void LottoInitialState::OnRollOver(MSG_BASE* packet)
{
    MSG_DW_LOTTO_ROLLOVER_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_ROLLOVER_ACK*>(packet);

    if (pRecvMsg->iResult != 0) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] Failed to roll-over!");
        
        RequestRollOver(0, 0, 0, 0, 0, nextWinningNum_); // 재시도
        return;
    }

    lotto_.RollOver(nextWinningNum_);

    lotto_.TransitState(Lotto::ACTIVE_STATE);
}
