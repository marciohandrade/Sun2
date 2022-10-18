/*
 * InactiveState.h
 */
#ifndef _LOTTO_INACTIVE_STATE_H_
#define _LOTTO_INACTIVE_STATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Timer.h"

#ifndef _LOTTO_STATE_H_
#include "../LottoState.h"
#endif

// 하임 로또 비활성화 상태 클래스
class LottoInactiveState : public LottoState
{
public:
    LottoInactiveState() : LottoState((BYTE)0) {}
    virtual ~LottoInactiveState() {}

    virtual void OnEnter();
    virtual void OnExit();

    virtual void OnTick();

    virtual void OnInfoRequest(CHARGUID charGuid, MSG_BASE* packet);

private:
    LottoWinnerList::iterator it_;

    util::Timer rewardTimer_;
};

#endif // _LOTTO_INACTIVE_STATE_H_
