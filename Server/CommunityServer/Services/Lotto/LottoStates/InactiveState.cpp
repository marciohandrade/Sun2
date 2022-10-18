#include "WorldServerPch.hxx"
#include "./InactiveState.h"

#include "Services/Lotto/Lotto.h"


// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

#define REWARD_PERIOD_IN_SECS   5   // �� ������ �κ� ���� ���� �ֱ�
#define BATCH_PROCESSING_UNIT   10  // �� �ֱ�� �� ���� ĳ���Ϳ��� ������ ��������

void LottoInactiveState::OnEnter()
{
    SUNLOG(eFULL_LOG, "[Lotto] Entering INACTIVE_STATE");

    it_ = lotto_.winners_.begin();

    rewardTimer_.SetTimer(0); // ���� 1ȸ�� ������ Expired ���·�
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
        lotto_.TransitState(Lotto::ACTIVE_STATE); // ��÷�ڰ� �� �� ���ٸ� �ٷ� ACTIVE_STATE��
        return;
    }

    if (rewardTimer_.IsExpired())
    {
        int i = 0;
        for (; i < BATCH_PROCESSING_UNIT; ++i)
        {
            // ��÷�� ���� ����

            LottoWinner& winner = *it_;
            ASSERT(0 < winner.status && winner.status <= lotto_.config_.GetNumPrizes());

            SUNLOG(eFULL_LOG, "[Lotto] Rewarding [%d] Prize %d : %I64d", winner.charGuid, 
                   winner.status, lotto_.prizes_[winner.status - 1]);

            RequestReward(winner);

            if (++it_ == lotto_.winners_.end()) { break; }
        }
        if (i < BATCH_PROCESSING_UNIT) {
            // ���� �Ϸ�, ��ȸ ����
            lotto_.TransitState(Lotto::ACTIVE_STATE);
            return;
        }

        // �ֱ��� ���� Ÿ�̸� ����
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
