/*
 * InactiveState.h
 */
#ifndef _LOTTO_DRAWING_STATE_H_
#define _LOTTO_DRAWING_STATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _LOTTO_STATE_H_
#include "../LottoState.h"
#endif

// 하임 로또 추첨 상태 클래스
class LottoDrawingState : public LottoState
{
public:
    LottoDrawingState() : LottoState((BYTE)0) {}
    virtual ~LottoDrawingState() {}

    virtual void OnEnter();
    virtual void OnExit();

    virtual void OnTick();

    virtual void OnInfoRequest(CHARGUID charGuid, MSG_BASE* packet);

    virtual void OnWinnerList(MSG_BASE* packet);
    virtual void OnRollOver(MSG_BASE* packet);

private:
    BYTE nextWinningNum_[LottoConfig::NUM_PICKS];   // 차회 당첨 번호

    int page_;
};

#endif // _LOTTO_DRAWING_STATE_H_
