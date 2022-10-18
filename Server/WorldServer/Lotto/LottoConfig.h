/*
 * LottoConfig.h
 */
#ifndef _LOTTO_CONFIG_H_
#define _LOTTO_CONFIG_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _SCHEDULE_H_
#include "Schedule.h"
#endif

// ���� �ζ� ������ ��÷�� ���� ������ ǥ���ϴ� ����ü
struct LottoPrize
{
    // ��÷�� ���� ���� ����
    enum PrizeType {
        TYPE_RATIO  // ���� �ݾ� ��÷�� ���� �� �����׿� ���� ����
      , TYPE_AMOUNT // ���� �ݾ�
    };

    PrizeType type;     // ��÷�� ����
    union {
        double ratio;   // ��÷�� ����
        INT64 amount;   // ��÷�ݾ�
    } value;
};

typedef std::vector<int> ReminderList;

// ���� �ζ� ���� ������ ĸ��ȭ�ϴ� Ŭ����
class LottoConfig
{
public:
    // ���� �ڵ忡�� �Ϲ������� �����ϱ� ���� �׸���� public static ����� ����
    
    // ���� ������ �ּ� ����
    static const BYTE MIN_NUMBER = LOTTO_MIN_NUMBER;
    // ���� ������ �ִ� ����
    static const BYTE MAX_NUMBER = LOTTO_MAX_NUMBER;
    // ����/��÷�� �� ���� ���ڸ� ������ ���ΰ�
    static const int NUM_PICKS = LOTTO_NUM_PICKS;
    // �� ĳ���Ͱ� ������ �� �ִ� ���� ������ �ִ� ����
    static const int MAX_TICKETS = LOTTO_MAX_TICKETS;
    // ���� ���� ���� ȸ���� ������ �ֱ� �� ȸ���� ���� �ζ� �����Ͱ� �����Ǵ°�
    static const int NUM_RECENT_DRAWS = LOTTO_NUM_RECENT_DRAWS;
    // �ִ� �� ���� �ֱ� ��÷ �ȳ� ������ ������ ���ΰ�
    static const int MAX_NOTICE = LOTTO_MAX_NOTICE;
public:
    LottoConfig();
    ~LottoConfig();

    // ���� �ζ� ������ �ʱ�ȭ�Ѵ�
    bool Init();

    // �־��� ���� �Һ� ������ ���� ���� ȹ���� ���� ��ȿ�� �������� �Ǵ�
    bool IsEffectiveConsumption(eHEIM_CONSUMPTION_TYPE type) const { return ( ((1 << type) & consumptionFlag_) != 0 ); }
    // �α��� �� ���� ���� 1�Ÿ� ȹ���ϱ� ���� �ּ� ���� ����
    INT64 GetTicketPrice() const { return ticketPrice_; }
    // ��ȿ ������ ������ ��ȯ ����(��ȿ ������ �� %�� �����Ǵ°�)
    float GetAccumulationRate() const { return accumulationRate_; }
    // 1����� �� ������� ��÷�� ���� ��� ���ԵǴ°�
    int GetNumPrizes() const { return numPrizes_; }

    // �־��� ������ �ش��ϴ� ��÷�� ���� ������ ��´�.
    const LottoPrize* GetPrize(int rank) const;

    // ���� ��÷ ������ ã�´�
    bool ScheduleNextDraw(ScheduleToken& token) const { return schedule_.FindNearestAhead(token); }

    const ReminderList& GetReminders() const { return reminders_; }

private:
    DWORD consumptionFlag_;         // � ������ ���� �Һ� ��ȿ�ϰ� ��������
    INT64 ticketPrice_;             // ���� ���� ����
    float accumulationRate_;        // ��ȿ ������ ������ ��ȯ ����
    int   numPrizes_;               // �� ����� ��÷�� ���� ��� ���ԵǴ°�
    
    LottoPrize* prizes_;            // ������ ��÷�� ���� ���� ���

    Schedule schedule_;             // ��÷ ����

    ReminderList reminders_;
};

#endif // _LOTTO_CONFIG_H_
