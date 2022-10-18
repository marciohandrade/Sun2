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

    initRetryTimer_.SetTimer(0); // ���� 1ȸ�� ������ Expired ���·�
    initRetryTimer_.EnableCheckTime();

    lotto_.winners_.clear();

    page_ = 1; // ��÷�� ��� ������ �� �ʱ�ȭ
}

void LottoInitialState::OnExit()
{
    SUNLOG(eFULL_LOG, "[Lotto] Exiting INITIAL_STATE");

    initRetryTimer_.DisableCheckTime();
}

void LottoInitialState::OnTick()
{
    // �ʱ�ȭ �õ�
    if (initRetryTimer_.IsExpired())
    {
        SUNLOG(eFULL_LOG, "[Lotto] Initializing Lotto : Loading...");

        RequestLoad();

        // �ʱ�ȭ ��õ� Ÿ�̸� ����
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
            // ���� ���� ���� ȸ���� ��� �� �������� �о���δ�
            result->totalAccum = pRecvMsg->i64TotalAccum;
        }
        else {
            // ���� ȸ�� ���� ���� ���� ȸ�� ��ȣ�� ���� ��÷ ȸ���� ����
            if (lotto_.lastDrawNum_ == 0) { lotto_.lastDrawNum_ = result->drawNum; }
        }
        result->numWinners = record.iNumWinners;
        result->jackpot = record.i64Jackpot;
        for (int j = 0; j < LottoConfig::NUM_PICKS; ++j) {
            result->winningNum[j] = record.bWinningNum[j];
        }
    }

    // ���� ���� ���� ȸ���� �ִٸ�
    if (lotto_.numResults_ > 0)
    {
        SUNLOG(eFULL_LOG, "[Lotto] Loaded %d results(s). Current draw # = %d, Total accumulation = %I64d", 
            lotto_.numResults_, lotto_.GetCurrentDrawNum(), lotto_.GetCurrentTotalAccum());

        lotto_.TransitState(Lotto::ACTIVE_STATE);
        
        // Ȱ��ȭ�ϱ� ���� ���� ȸ�� ������ �Ϸ�Ǿ����� �����ؾ� �Ѵ�
        //RequestWinnerList(page_);
    }
    else
    {
        SUNLOG(eFULL_LOG, "[Lotto] Initializing the 1st draw...");

        // ���� 1ȸ���� �ʱ�ȭ�Ѵ�.
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
        
        RequestRollOver(0, 0, 0, 0, 0, nextWinningNum_); // ��õ�
        return;
    }

    lotto_.RollOver(nextWinningNum_);

    lotto_.TransitState(Lotto::ACTIVE_STATE);
}
