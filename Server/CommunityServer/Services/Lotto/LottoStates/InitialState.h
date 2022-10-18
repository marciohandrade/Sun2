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
#include "Services/Lotto/LottoState.h"
#endif

// ���� �ζ� �ʱ� ���� Ŭ����
class LottoInitialState : public LottoState
{
public:
    LottoInitialState() : LottoState((BYTE)0) {}
    virtual ~LottoInitialState() {}

    virtual void OnEnter();
    virtual void OnExit();

    virtual void OnTick();

    virtual void OnLoad(const MSG_DW_LOTTO_LOAD_ACK* packet);
    virtual void OnWinnerList(const MSG_DW_LOTTO_WINNER_LIST_ACK* packet);
    virtual void OnRollOver(const MSG_DW_LOTTO_ROLLOVER_ACK* packet);

private:
    BYTE nextWinningNum_[LottoConfig::NUM_PICKS];   // ��ȸ ��÷ ��ȣ

    util::Timer initRetryTimer_;                    // �ʱ�ȭ ��õ� Ÿ�̸�

    int page_;
};

#endif // _LOTTO_INITIAL_STATE_H_
