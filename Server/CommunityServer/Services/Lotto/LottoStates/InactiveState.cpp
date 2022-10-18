#include "WorldServerPch.hxx"
#include "./InactiveState.h"

#include "Services/Lotto/Lotto.h"


// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

#define REWARD_PERIOD_IN_SECS   5   // 초 단위의 부분 보상 실행 주기
#define BATCH_PROCESSING_UNIT   10  // 매 주기당 몇 개의 캐릭터에게 보상을 지급할지

void LottoInactiveState::OnEnter()
{
    SUNLOG(eFULL_LOG, "[Lotto] Entering INACTIVE_STATE");

    it_ = lotto_.winners_.begin();

    rewardTimer_.SetTimer(0); // 최초 1회는 강제로 Expired 상태로
    rewardTimer_.EnableCheckTime();
}

void LottoInactiveState::OnExit()
{
    SUNLOG(eFULL_LOG, "[Lotto] Exiting INACTIVE_STATE");

    rewardTimer_.DisableCheckTime();
}

void LottoInactiveState::OnTick()
{
    if (it_ == lotto_.winners_.end()) {
        lotto_.TransitState(Lotto::ACTIVE_STATE); // 당첨자가 한 명도 없다면 바로 ACTIVE_STATE로
        return;
    }

    if (rewardTimer_.IsExpired())
    {
        int i = 0;
        for (; i < BATCH_PROCESSING_UNIT; ++i)
        {
            // 당첨자 개별 보상

            LottoWinner& winner = *it_;
            ASSERT(0 < winner.status && winner.status <= lotto_.config_.GetNumPrizes());

            SUNLOG(eFULL_LOG, "[Lotto] Rewarding [%d] Prize %d : %I64d", winner.charGuid, 
                   winner.status, lotto_.prizes_[winner.status - 1]);

            RequestReward(winner);

            if (++it_ == lotto_.winners_.end()) { break; }
        }
        if (i < BATCH_PROCESSING_UNIT) {
            // 보상 완료, 차회 시작
            lotto_.TransitState(Lotto::ACTIVE_STATE);
            return;
        }

        // 주기적 보상 타이머 리셋
        if (rewardTimer_.GetBaseIntervalTime() > 0) {
            rewardTimer_.Reset();
        }
        else {
            rewardTimer_.SetTimer(REWARD_PERIOD_IN_SECS * 1000);
        }
    }
}

void LottoInactiveState::OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet)
{
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eMIDDLE_LOG, "[Lotto] WARNING : Entry not found for character [%d]", charGuid);
        RequestEntryLoad(charGuid, packet->bFlag);
        return;
    }
    SendInfoResponse(charGuid, packet->bFlag);
}
