/*
 * InitialState.h
 */
#ifndef _LOTTO_INITIAL_STATE_H_
#define _LOTTO_INITIAL_STATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Timer.h"

#ifndef _LOTTO_STATE_H_
#include "../LottoState.h"
#endif

// 하임 로또 초기 상태 클래스
class LottoInitialState : public LottoState
{
public:
    LottoInitialState() : LottoState((BYTE)0) {}
    virtual ~LottoInitialState() {}

    virtual void OnEnter();
    virtual void OnExit();

    virtual void OnTick();

    virtual void OnLoad(MSG_BASE* packet);
    virtual void OnWinnerList(MSG_BASE* packet);
    virtual void OnRollOver(MSG_BASE* packet);

private:
    BYTE nextWinningNum_[LottoConfig::NUM_PICKS];   // 차회 당첨 번호

    util::Timer initRetryTimer_;                    // 초기화 재시도 타이머

    int page_;
};

#endif // _LOTTO_INITIAL_STATE_H_
