#include "stdafx.h"

#include "PacketStruct_DW.h"

#include "../Lotto.h"

#include "DrawingState.h"

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

void LottoDrawingState::OnEnter()
{
    SUNLOG(eFULL_LOG, "[Lotto] Entering DRAWING_STATE");

    // 하임 로또 종료 공지
    BroadcastLottoFinish();

    // 추첨 정보 초기화
    lotto_.lastDrawNum_ = lotto_.GetCurrentDrawNum();
    lotto_.winners_.clear();
    for (int i = 0; i < lotto_.config_.GetNumPrizes(); ++i) { lotto_.numWinners_[i] = 0; }
    for (int i = 0; i < lotto_.config_.GetNumPrizes(); ++i) { lotto_.prizes_[i] = 0; }

    page_ = 1; // 당첨자 목록 페이지 수 초기화

    RequestWinnerList(page_);
}

void LottoDrawingState::OnExit()
{
    SUNLOG(eFULL_LOG, "[Lotto] Exiting DRAWING_STATE");
}

void LottoDrawingState::OnTick()
{
    // 타임아웃 처리 가능
}

void LottoDrawingState::OnInfoRequest(CHARGUID charGuid, MSG_BASE* packet)
{
    MSG_CW_LOTTO_INFO_SYN* pRecvMsg = static_cast<MSG_CW_LOTTO_INFO_SYN*>(packet);

    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eMIDDLE_LOG, "[Lotto] WARNING : Entry not found for character [%d]", charGuid);
        RequestEntryLoad(charGuid, pRecvMsg->bFlag);
        return;
    }
    SendInfoResponse(charGuid, pRecvMsg->bFlag);
}

void LottoDrawingState::OnWinnerList(MSG_BASE* packet)
{
    MSG_DW_LOTTO_WINNER_LIST_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_WINNER_LIST_ACK*>(packet);

    int page = pRecvMsg->iPage;
    int maxPage = pRecvMsg->iMaxPage;
    int numRecs = pRecvMsg->iNumRecords;

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

    // 당첨자 목록 완성

    lotto_.CalculatePrizes();

    lotto_.PickRandomNumbers(nextWinningNum_);
    RequestRollOver(lotto_.GetCurrentDrawNum(), (INT8)lotto_.numWinners_[0], \
        lotto_.prizes_[0], lotto_.prizes_[1], lotto_.prizes_[2], nextWinningNum_);
}

void LottoDrawingState::OnRollOver(MSG_BASE* packet)
{
    MSG_DW_LOTTO_ROLLOVER_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_ROLLOVER_ACK*>(packet);

    if (pRecvMsg->iResult != 0) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] Failed to roll-over!");

        if (pRecvMsg->iResult < 0) {
            // 다른 개발 서버가 실행 중이면 PK 위반으로 실패할 수 있다
            lotto_.TransitState(Lotto::INITIAL_STATE);
        }
        else {
            RequestRollOver(lotto_.GetCurrentDrawNum(), (INT8)lotto_.numWinners_[0], \
                lotto_.prizes_[0], lotto_.prizes_[1], lotto_.prizes_[2], nextWinningNum_);
        }
        return;
    }

    lotto_.RollOver(nextWinningNum_);

    if (lotto_.winners_.empty()) {
        lotto_.TransitState(Lotto::ACTIVE_STATE);
    }
    else {
        lotto_.TransitState(Lotto::INACTIVE_STATE);
    }
}

