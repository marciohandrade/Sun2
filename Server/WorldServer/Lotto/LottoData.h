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

// ���� �ζ� ���� ������ �޸� ǥ��
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

// ���� �ζ� ���� ������ �޸� ǥ��
struct LottoEntry
{
    LottoEntry() : payment(0), numRemainingTickets(0), numUsedTickets(0), paymentOrg(0) {}

    DWORD payment;              // ���� ���� �ڵ� �߱��� ���� �Һ� ���� ���� ����
    WORD  numRemainingTickets;  // ���� ������ ���� ���� ���� ����
    WORD  numUsedTickets;       // ���� ȸ���� �̹� ������ ���� ���� ���� ����
    LottoTicketList tickets;    // ����� ���� ȸ���� ��� �����ϴ� ���� ����
    DWORD paymentOrg;           // �ε� ������ ��ȿ �Һ� ���� ��� ����
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    LottoTicket temp;           // ���� ���� ���Ž� ���Ǵ� �ӽ� ���� ����
#endif
};
typedef stdext::hash_map<CHARGUID, LottoEntry> LottoEntryMap;

// ���� �ζ� ��÷ ��
struct LottoWinner
{
    INT64    idx;
    CHARGUID charGuid;
    BYTE     status;
};
typedef std::vector<LottoWinner> LottoWinnerList;

// ���� �ζ� 1ȸ���� ��Ȳ/����� ǥ���ϴ� ����ü
struct LottoResult
{
    int   drawNum;     // ȸ��
    INT64 totalAccum;  // �� ������
    int   numWinners;  // 1�� ��÷�� �� (-1:������, 0:�̿�)
    INT64 jackpot;     // 1�� ��÷��

    BYTE winningNum[LottoConfig::NUM_PICKS];    // ��÷ ��ȣ

    void Reset()
    {
        ZeroMemory(this, sizeof(LottoResult));
        numWinners = -1; // ���� ������ ǥ��
    }
};

#endif // _LOTTO_DATA_H_
