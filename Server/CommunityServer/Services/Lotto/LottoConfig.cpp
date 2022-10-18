#include "WorldServerPch.hxx"
#include "./LottoConfig.h"

#include <algorithm>

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

LottoConfig::LottoConfig()
  : prizes_(NULL)
{
}

LottoConfig::~LottoConfig()
{
    if (prizes_ != NULL) {
        delete[] prizes_;
    }
}

bool LottoConfig::Init()
{
    // 복권 용지 1매의 구입 가격 (하임)
    // 응모권 캐시 판매 기능(_NA_20101011_HEIM_LOTTO_CASH_TICKET)이 활성화되면 사용되지 않는다.
#if defined(_CHINA)
    ticketPrice_ = 1000000; // 복권 용지 1매의 구입 가격
#elif defined(_TAIWAN)
    ticketPrice_ = 2000000; // 복권 용지 1매를 획득하기 위해 2,000,000하임 지출 필요
#else
    ticketPrice_ = 10000; // 복권 용지 1매를 획득하기 위해 10,000하임 지출 필요
#endif

    // 유효 하임 소비 유형, 글로벌 적립율과 상인별 적립율은
    // 복권용지 직접 구입 기능(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE)이 활성화되면 적용되지 않는다.

    // 복권 용지 획득을 위해 인정되는 유효한 하임 소비 유형
    consumptionFlag_ = 0;
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_WAYPOINT);      // 웨이포인트 비용
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_RANKUP);        // 랭크업 비용
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_COMPOSITION);   // 조합 비용
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_NPC_SHOP);      // NPC 상인에게서 구입
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_REPAIR);        // 아이템 수리
    //consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_ENCHANT);       // 인챈팅 비용

#if defined(_TAIWAN)
    accumulationRate_ = 0.5f; // 유효 지출의 50%를 적립액으로 전환
#else
    accumulationRate_ = 0.6f; // 유효 지출의 60%를 적립액으로 전환
#endif

    // 당첨금 지급 조건
    numPrizes_ = 3;
    if (prizes_ != NULL) { delete[] prizes_; }
    prizes_ = new LottoPrize[numPrizes_];
    // Would crash on out-of-memory condition
    // 1등 당첨금
    LottoPrize& firstPrize = prizes_[0];
    firstPrize.type = LottoPrize::TYPE_RATIO;
    firstPrize.value.ratio = 0.5f; // 50%
    // 2등 당첨금
    LottoPrize& secondPrize = prizes_[1];
    secondPrize.type = LottoPrize::TYPE_RATIO;
    secondPrize.value.ratio = 0.1f; // 10%
    // 3등 당첨금
    LottoPrize& thirdPrize = prizes_[2];
    thirdPrize.type = LottoPrize::TYPE_AMOUNT;
#if defined(_CHINA)
    thirdPrize.value.amount = 3000000;
#elif defined(_TAIWAN)
    thirdPrize.value.amount = 50000;
#elif defined(_GLOBAL) || defined(_RUSSIA)
    thirdPrize.value.amount = 1000000;
#else
    thirdPrize.value.amount = 5000;
#endif

    // 추첨 일시
    schedule_.ResetAll();
#if defined (_GLOBAL) || defined(_RUSSIA)
    // 상용: 매 토요일 오후 14시 00분
    schedule_.SetDayOfWeek(Schedule::SATURDAY);
    schedule_.SetHour(14);
    schedule_.SetMinute(00);
#else
    // 상용: 매 금요일 오후 8시 45분
    schedule_.SetDayOfWeek(Schedule::FRIDAY);
    schedule_.SetHour(20);
    schedule_.SetMinute(45);
#endif
    // 개발 테스트용: 매 정시
    /*
    schedule_.SetDayOfWeek();
    schedule_.SetHour();
    schedule_.SetMinute(0);
    */

    // 초 단위의 카운트 다운 시점들
    // 추첨 시간 체크가 1분 단위이므로, 1분(60초) 미만으로는 설정할 수 없다
    reminders_.clear();
    reminders_.push_back(30 * 60);
    //reminders_.push_back(20 * 60);
    //reminders_.push_back(10 * 60);
    reminders_.push_back(5 * 60);
    //reminders_.push_back(4 * 60);
    //reminders_.push_back(3 * 60);
    //reminders_.push_back(2 * 60);
    //reminders_.push_back(1 * 60);
    std::sort(reminders_.rbegin(), reminders_.rend()); // 내림차순 정렬

    return true;
}

const LottoPrize* LottoConfig::GetPrize(int rank) const
{
    ASSERT(1 <= rank && rank <= numPrizes_);
    if (rank < 1 || numPrizes_ < rank) { return NULL; }
    return &prizes_[rank - 1];
}
