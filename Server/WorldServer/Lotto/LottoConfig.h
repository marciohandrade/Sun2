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

// 하임 로또 등위별 당첨금 지급 조건을 표현하는 구조체
struct LottoPrize
{
    // 당첨금 조건 설정 유형
    enum PrizeType {
        TYPE_RATIO  // 고정 금액 당첨금 제외 후 적립액에 비율 적용
      , TYPE_AMOUNT // 고정 금액
    };

    PrizeType type;     // 당첨금 조건
    union {
        double ratio;   // 당첨금 비율
        INT64 amount;   // 당첨금액
    } value;
};

typedef std::vector<int> ReminderList;

// 하임 로또 세부 설정을 캡슐화하는 클래스
class LottoConfig
{
public:
    // 서버 코드에서 일방적으로 변경하기 힘든 항목들을 public static 상수로 설정
    
    // 선택 가능한 최소 숫자
    static const BYTE MIN_NUMBER = LOTTO_MIN_NUMBER;
    // 선택 가능한 최대 숫자
    static const BYTE MAX_NUMBER = LOTTO_MAX_NUMBER;
    // 응모/추첨시 몇 개의 숫자를 선택할 것인가
    static const int NUM_PICKS = LOTTO_NUM_PICKS;
    // 한 캐릭터가 보유할 수 있는 복권 용지의 최대 갯수
    static const int MAX_TICKETS = LOTTO_MAX_TICKETS;
    // 진행 중인 현재 회차를 포함해 최근 몇 회차의 하임 로또 데이터가 관리되는가
    static const int NUM_RECENT_DRAWS = LOTTO_NUM_RECENT_DRAWS;
    // 최대 몇 개의 최근 당첨 안내 쪽지를 전송할 것인가
    static const int MAX_NOTICE = LOTTO_MAX_NOTICE;
public:
    LottoConfig();
    ~LottoConfig();

    // 하임 로또 설정을 초기화한다
    bool Init();

    // 주어진 하임 소비 유형이 복권 용지 획득을 위한 유효한 지출인지 판단
    bool IsEffectiveConsumption(eHEIM_CONSUMPTION_TYPE type) const { return ( ((1 << type) & consumptionFlag_) != 0 ); }
    // 로그인 후 복권 용지 1매를 획득하기 위한 최소 지출 하임
    INT64 GetTicketPrice() const { return ticketPrice_; }
    // 유효 지출의 적립액 전환 비율(유효 지출의 몇 %가 적립되는가)
    float GetAccumulationRate() const { return accumulationRate_; }
    // 1등부터 몇 등까지가 당첨금 지급 대상에 포함되는가
    int GetNumPrizes() const { return numPrizes_; }

    // 주어진 등위에 해당하는 당첨금 지급 조건을 얻는다.
    const LottoPrize* GetPrize(int rank) const;

    // 다음 추첨 일정을 찾는다
    bool ScheduleNextDraw(ScheduleToken& token) const { return schedule_.FindNearestAhead(token); }

    const ReminderList& GetReminders() const { return reminders_; }

private:
    DWORD consumptionFlag_;         // 어떤 종류의 하임 소비를 유효하게 인정할지
    INT64 ticketPrice_;             // 복권 용지 가격
    float accumulationRate_;        // 유효 지출의 적립액 전환 비율
    int   numPrizes_;               // 몇 등까지 당첨금 지급 대상에 포함되는가
    
    LottoPrize* prizes_;            // 등위별 당첨급 지급 조건 목록

    Schedule schedule_;             // 추첨 일정

    ReminderList reminders_;
};

#endif // _LOTTO_CONFIG_H_
