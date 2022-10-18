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
    // ���� ���� 1���� ���� ���� (����)
    // ����� ĳ�� �Ǹ� ���(_NA_20101011_HEIM_LOTTO_CASH_TICKET)�� Ȱ��ȭ�Ǹ� ������ �ʴ´�.
#if defined(_CHINA)
    ticketPrice_ = 1000000; // ���� ���� 1���� ���� ����
#elif defined(_TAIWAN)
    ticketPrice_ = 2000000; // ���� ���� 1�Ÿ� ȹ���ϱ� ���� 2,000,000���� ���� �ʿ�
#else
    ticketPrice_ = 10000; // ���� ���� 1�Ÿ� ȹ���ϱ� ���� 10,000���� ���� �ʿ�
#endif

    // ��ȿ ���� �Һ� ����, �۷ι� �������� ���κ� ��������
    // ���ǿ��� ���� ���� ���(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE)�� Ȱ��ȭ�Ǹ� ������� �ʴ´�.

    // ���� ���� ȹ���� ���� �����Ǵ� ��ȿ�� ���� �Һ� ����
    consumptionFlag_ = 0;
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_WAYPOINT);      // ��������Ʈ ���
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_RANKUP);        // ��ũ�� ���
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_COMPOSITION);   // ���� ���
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_NPC_SHOP);      // NPC ���ο��Լ� ����
    consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_REPAIR);        // ������ ����
    //consumptionFlag_ |= (1 << eHEIM_CONSUMPTION_ENCHANT);       // ��æ�� ���

#if defined(_TAIWAN)
    accumulationRate_ = 0.5f; // ��ȿ ������ 50%�� ���������� ��ȯ
#else
    accumulationRate_ = 0.6f; // ��ȿ ������ 60%�� ���������� ��ȯ
#endif

    // ��÷�� ���� ����
    numPrizes_ = 3;
    if (prizes_ != NULL) { delete[] prizes_; }
    prizes_ = new LottoPrize[numPrizes_];
    // Would crash on out-of-memory condition
    // 1�� ��÷��
    LottoPrize& firstPrize = prizes_[0];
    firstPrize.type = LottoPrize::TYPE_RATIO;
    firstPrize.value.ratio = 0.5f; // 50%
    // 2�� ��÷��
    LottoPrize& secondPrize = prizes_[1];
    secondPrize.type = LottoPrize::TYPE_RATIO;
    secondPrize.value.ratio = 0.1f; // 10%
    // 3�� ��÷��
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

    // ��÷ �Ͻ�
    schedule_.ResetAll();
#if defined (_GLOBAL) || defined(_RUSSIA)
    // ���: �� ����� ���� 14�� 00��
    schedule_.SetDayOfWeek(Schedule::SATURDAY);
    schedule_.SetHour(14);
    schedule_.SetMinute(00);
#else
    // ���: �� �ݿ��� ���� 8�� 45��
    schedule_.SetDayOfWeek(Schedule::FRIDAY);
    schedule_.SetHour(20);
    schedule_.SetMinute(45);
#endif
    // ���� �׽�Ʈ��: �� ����
    /*
    schedule_.SetDayOfWeek();
    schedule_.SetHour();
    schedule_.SetMinute(0);
    */

    // �� ������ ī��Ʈ �ٿ� ������
    // ��÷ �ð� üũ�� 1�� �����̹Ƿ�, 1��(60��) �̸����δ� ������ �� ����
    reminders_.clear();
    reminders_.push_back(30 * 60);
    //reminders_.push_back(20 * 60);
    //reminders_.push_back(10 * 60);
    reminders_.push_back(5 * 60);
    //reminders_.push_back(4 * 60);
    //reminders_.push_back(3 * 60);
    //reminders_.push_back(2 * 60);
    //reminders_.push_back(1 * 60);
    std::sort(reminders_.rbegin(), reminders_.rend()); // �������� ����

    return true;
}

const LottoPrize* LottoConfig::GetPrize(int rank) const
{
    ASSERT(1 <= rank && rank <= numPrizes_);
    if (rank < 1 || numPrizes_ < rank) { return NULL; }
    return &prizes_[rank - 1];
}
