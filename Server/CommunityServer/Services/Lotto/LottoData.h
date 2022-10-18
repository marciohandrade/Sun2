/*
 * LottoData.h
 */
#ifndef _LOTTO_DATA_H_
#define _LOTTO_DATA_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _LOTTO_CONFIG_H_
#include "LottoConfig.h"
#endif

// 하임 로또 복권 용지의 메모리 표현
struct LottoTicket
{
    LottoTicket() : drawNum(0), status(0), pickedNum1(0), pickedNum2(0), pickedNum3(0), pickedNum4(0) {}

    DWORD drawNum    : 16;
    DWORD status     : 16;
    DWORD pickedNum1 : 8;
    DWORD pickedNum2 : 8;
    DWORD pickedNum3 : 8;
    DWORD pickedNum4 : 8;
};
typedef std::deque<LottoTicket> LottoTicketList;

// 하임 로또 응모 내역의 메모리 표현
struct LottoEntry
{
    LottoEntry() : payment(0), numRemainingTickets(0), numUsedTickets(0), paymentOrg(0) {}

    DWORD payment;              // 복권 용지 자동 발급을 위한 소비 하임 추적 변수
    WORD  numRemainingTickets;  // 응모 가능한 남은 복권 용지 개수
    WORD  numUsedTickets;       // 현재 회차에 이미 응모한 응모 복권 용지 개수
    LottoTicketList tickets;    // 현재와 지난 회차를 모두 포함하는 응모 내역
    DWORD paymentOrg;           // 로딩 시점의 유효 소비 하임 백업 변수
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    LottoTicket temp;           // 용지 직접 구매시 사용되는 임시 저장 공간
#endif
};
typedef stdext::hash_map<CHARGUID, LottoEntry> LottoEntryMap;

// 하임 로또 당첨 건
struct LottoWinner
{
    INT64    idx;
    CHARGUID charGuid;
    BYTE     status;
};
typedef std::vector<LottoWinner> LottoWinnerList;

// 하임 로또 1회분의 현황/결과를 표현하는 구조체
struct LottoResult
{
    int   drawNum;     // 회차
    INT64 totalAccum;  // 총 적립액
    int   numWinners;  // 1등 당첨자 수 (-1:진행중, 0:이월)
    INT64 jackpot;     // 1등 당첨금

    BYTE winningNum[LottoConfig::NUM_PICKS];    // 당첨 번호

    void Reset()
    {
        ZeroMemory(this, sizeof(LottoResult));
        numWinners = -1; // 진행 중임을 표시
    }
};

#endif // _LOTTO_DATA_H_
