/*
 * FinalState.h
 */
#ifndef _LOTTO_FINAL_STATE_H_
#define _LOTTO_FINAL_STATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _LOTTO_STATE_H_
#include "Services/Lotto/LottoState.h"
#endif

// ���� �ζ� ���� ���� Ŭ����
class LottoFinalState : public LottoState
{
public:
    LottoFinalState() : LottoState((BYTE)0) {}
    virtual ~LottoFinalState() {}

    virtual void OnEnter();
    virtual void OnExit();
};

#endif // _LOTTO_FINAL_STATE_H_
