#include "WorldServerPch.hxx"
#include "./DrawingState.h"

#include "PacketStruct_DW.h"

#include "Services/Lotto/Lotto.h"


// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

void LottoDrawingState::OnEnter()
{
    SUNLOG(eFULL_LOG, "[Lotto] Entering DRAWING_STATE");

    // ���� �ζ� ���� ����
    BroadcastLottoFinish();

    // ��÷ ���� �ʱ�ȭ
    lotto_.lastDrawNum_ = lotto_.GetCurrentDrawNum();
    lotto_.winners_.clear();
    for (int i = 0; i < lotto_.config_.GetNumPrizes(); ++i) { lotto_.numWinners_[i] = 0; }
    for (int i = 0; i < lotto_.config_.GetNumPrizes(); ++i) { lotto_.prizes_[i] = 0; }

    page_ = 1; // ��÷�� ��� ������ �� �ʱ�ȭ

    RequestWinnerList(page_);
}

void LottoDrawingState::OnExit()
{
    SUNLOG(eFULL_LOG, "[Lotto] Exiting DRAWING_STATE");
}

void LottoDrawingState::OnTick()
{
    // Ÿ�Ӿƿ� ó�� ����
}

void LottoDrawingState::OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet)
{
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eMIDDLE_LOG, "[Lotto] WARNING : Entry not found for character [%d]", charGuid);
        RequestEntryLoad(charGuid, packet->bFlag);
        return;
    }
    SendInfoResponse(charGuid, packet->bFlag);
}

void LottoDrawingState::OnWinnerList(const MSG_DW_LOTTO_WINNER_LIST_ACK* packet)
{
    int page = packet->iPage;
    int maxPage = packet->iMaxPage;
    int numRecs = packet->iNumRecords;

    for (int i = 0; i < numRecs; ++i)
    {
        LottoWinner winner;

        winner.idx = packet->winners[i].i64Idx;
        winner.charGuid = packet->winners[i].iCharGuid;
        winner.status = packet->winners[i].bStatus;
        
        lotto_.winners_.push_back(winner);

        ASSERT(0 < winner.status && winner.status <= lotto_.config_.GetNumPrizes());
        ++lotto_.numWinners_[winner.status - 1];

        SUNLOG(eFULL_LOG, "[Lotto] Winner [%d] : %d", winner.charGuid, winner.status);
    }
    
    if (page < maxPage) {
        RequestWinnerList(++page_);
        return;
    }

    // ��÷�� ��� �ϼ�

    lotto_.CalculatePrizes();

    lotto_.PickRandomNumbers(nextWinningNum_);
    RequestRollOver(lotto_.GetCurrentDrawNum(), (INT8)lotto_.numWinners_[0], \
        lotto_.prizes_[0], lotto_.prizes_[1], lotto_.prizes_[2], nextWinningNum_);
}

void LottoDrawingState::OnRollOver(const MSG_DW_LOTTO_ROLLOVER_ACK* packet)
{
    if (packet->iResult != 0) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] Failed to roll-over!");

        if (packet->iResult < 0) {
            // �ٸ� ���� ������ ���� ���̸� PK �������� ������ �� �ִ�
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
