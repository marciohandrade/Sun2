#include "StdAfx.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

#include ".\AccumulatePoint.h"
#include ".\Player.h"
#include <AccumulatePointParser.h>
#include <AccumulatePointData.h>


#define ONE_SECOND  1000

class TempTimer : public util::Timer
{
public:
    TempTimer(uint interval_time)
    {
        SetTimer(interval_time);
    }

    ~TempTimer() {}

    uint Update()
    {
        uint progress_time = GetProcessTime() / GetIntervalTime();
        IsExpired(TRUE);
        return progress_time;
    }
};


struct GlobalData
{
    GlobalData():
    year(0),
    dayofweek(0),
    increase_second(0),
    event_time(false)
    {
    }

    ushort year;
    ushort dayofweek;
    bool event_time;
    uint increase_second;
};


GlobalData global_data;

void AccumulatePoint::SyncMatchDate(ushort year, ushort dayofweek, uint secondofday)
{
    global_data.year = year;
    global_data.dayofweek = dayofweek;
    global_data.event_time = false;

#ifdef _NA_0_20100528_ACCUMULATE_POINT_EVENT
    const AccumulatePointParser::RewardGroup& reward_group = 
        AccumulatePointParser::Instance()->get_reward_group(global_data.dayofweek);

    const AccumulatePointParser::RewardGroup::RewardEvent& reward_event = reward_group.reward_event;
    if ((reward_event.start_time == 0) && (reward_event.end_time == 0))
    {
        global_data.event_time = false;
    }
    else
    {
        uint second = secondofday;

        if ((reward_event.start_time <= second) && (reward_event.end_time > second))
            global_data.event_time = true;
        else
            global_data.event_time = false;
    }
#endif
}


// ���� �⵵ �� �÷��̾��� ���� �÷��� ���� �ð�(�� ����)�� ���� �Ѵ�.
// ������ ����Ǵ� ���� �ش� �⵵�� �ٲ� �� �ֱ� ������ Update ���� ȣ���Ѵ�.
// �⵵ �� �÷��̾��� ���� �ð��� ���� AccumulatePoint ��ü�� �������� ��� �ϱ⿡ �������� �����Ѵ�. 
// �� Update���� ƽ ī��Ʈ�� ������ 1�� �̻��� ��� AccumulatePoint::increase_second_�� ����.
// �� �÷��̾��� �ð� ����� ���������� 1���� �ϱ� ���� �����̴�.
void AccumulatePoint::Calculate_Current_Year(uint delta_tick_count)
{
    static TempTimer second_timer(ONE_SECOND);

    // ������ �ʰ��� ���������� �����س��´�. �÷��̾� �� �Ź� ����ϴ� �� ���� ��� ������ ������ 
    // ����ϹǷ� ���������� ����� 1�� �ϴ°� ����.
    global_data.increase_second = second_timer.Update();
}


AccumulatePoint::AccumulatePoint(Player* player) :
player_(player),
playerinfo_(NULL)
{
    ASSERT(player_);
}

AccumulatePoint::~AccumulatePoint()
{
}

// Player �޸� Ǯ���� �����ڹ� �Ҹ��ڸ� ȣ���� �Ͼ�� �ʾ� �ʱ�ȭ �κ��� ������.
void AccumulatePoint::Init(Player* player)
{
    ASSERT(player);
    if (player == NULL)
        return;

    ASSERT(player->GetCharInfo());
    if (player->GetCharInfo() == NULL)
        return;

    const AccumulatePointParser::RewardGroup& reward_group = 
                    AccumulatePointParser::Instance()->get_reward_group(global_data.dayofweek);

    player_ = player;
    playerinfo_ = player->GetCharInfo();

    generated_ = false;

    reserved_data_.dayofweek = global_data.dayofweek;
    reserved_data_.event_time = global_data.event_time;
    reserved_data_.require_point = reward_group.require_point;
    reserved_data_.reward_code = 0;

    current_data_ = reserved_data_;
}

void AccumulatePoint::Release()
{
    player_ = NULL;
    playerinfo_ = NULL;
}

// Player::Update() ���ο��� ȣ��ȴ�.
// Player�� ���ӽð��� (�� ����)�� ���� ��Ű��, Ư�� �ð� ���� ���ϸ��� ������ 1 ���� ��Ų��.
// ����, �ų� 1�� 1��(�� �ش� ������ �ٲ� ���)�϶� �����ð��� �ʱ�ȭ ��Ű�� ������ �״�� �����Ѵ�.
void AccumulatePoint::Update()
{
    // �÷��̾ ZONE �� ���� ���� �ʴ� ��� Update()�� �������� �ʴ´�.
    // ZONE �� ���� ���� �Ǻ��� class PlayerManager::Update() ���� üũ�ϰ��ִ�.
    // ����, ZONE�� �Ǻ��� ����ó�� �ؾ��� ��� �Ʒ��� ������ ����Ѵ�.

    //GameZone* pEnterZone = NULL;
    //MAPCODE EnterFieldCode = 0;
    //const eZONE_STATE eRoomState = pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
    //if ( !(eRoomState & ePRS_AT_ZONE) )
    //    return;



    // ���� EnterField ���̸� Update�� �������� �ʴ´�.
    // EnterField �Ǻ����� class Player::Update() ���۽� üũ�ϰ��ִ�.
    // ����, EnterField�� �Ǻ��� ����ó�� �ؾ��� ��� �Ʒ��� ������ ����Ѵ�.

    //GameField* pField = player_pointer_->GetField();
    //if ( !(pField && pField->IsReady()) )
    //    return;

    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return;


    static MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD send_message;


    // �� ������ ������ ���� �����Ҷ��� üũ�Ѵ�.
    if (global_data.increase_second == 0)
        return;

    const AccumulatePointParser::RewardGroup& reward_group = 
                    AccumulatePointParser::Instance()->get_reward_group(global_data.dayofweek);

    // 0���� ������ ���� ���ϱ�
    // �ش� ���� 0���� �����ϴ� ������ �뷮���� �αװ� ���� ��찡 �����Ͽ� �ּ� ó����.
    // ASSERT(reward_group.generate_time);
    if (reward_group.generate_time == 0)
        return;

    // �⵵�� �ٲ��� �����ð��� �ʱ�ȭ ��Ų��.
    if (global_data.year != playerinfo_->m_wAccumulateYear)
    {
        playerinfo_->m_wAccumulateYear = global_data.year;
        playerinfo_->m_dwAccumulateTime = 0;
        GAMELOG->LogAccumulatePoint(ACCUM_CHANGE_YEAR, player_, 0, 0);

        send_message.m_dwReason = RC::RC_ACCUMULATE_POINT_INITIALIZE;
        send_message.m_dwAccumulateTime = playerinfo_->m_dwAccumulateTime;
        send_message.m_wAccumulatePoint = playerinfo_->m_wAccumulatePoint;
        send_message.m_byRequirePoint = reward_group.require_point;
        player_->SendPacket(&send_message, sizeof(send_message));
    }

    // ���� �ʿ��� �䱸 ������ ����� ���.
    if (reward_group.require_point != reserved_data_.require_point)
    {
        reserved_data_.require_point = reward_group.require_point;

        send_message.m_dwReason = RC::RC_ACCUMULATE_POINT_CHANGE_REQUIRE_POINT;
        send_message.m_dwAccumulateTime = playerinfo_->m_dwAccumulateTime;
        send_message.m_wAccumulatePoint = playerinfo_->m_wAccumulatePoint;
        send_message.m_byRequirePoint = reward_group.require_point;
        player_->SendPacket(&send_message, sizeof(send_message));
    }


    // ������ ������ ����� ���
    reserved_data_.dayofweek = global_data.dayofweek;

    // �̺�Ʈ �ð��� ����� ���
    reserved_data_.event_time = global_data.event_time;



    // ���ϸ��� �ð��� ������Ų��.(�� ����)
    // �ð� ��ȭ : ���� �� �ð� - ���� �� �ð�
    int before_second = playerinfo_->m_dwAccumulateTime / reward_group.generate_time;

    playerinfo_->m_dwAccumulateTime += global_data.increase_second;

    if ((playerinfo_->m_dwAccumulateTime / reward_group.generate_time) - before_second)
    {
        if (PlusAccumulatePoint(reward_group.generate_point))
        {
            GAMELOG->LogAccumulatePoint(ACCUM_ACQUISITION_TIME, player_, reward_group.generate_point, 0);

            send_message.m_dwReason = RC::RC_ACCUMULATE_POINT_SUCCESS;
            send_message.m_dwAccumulateTime = playerinfo_->m_dwAccumulateTime;
            send_message.m_wAccumulatePoint = playerinfo_->m_wAccumulatePoint;
            send_message.m_byRequirePoint = reward_group.require_point;
            player_->SendPacket(&send_message, sizeof(send_message));
        }
    }
}

bool AccumulatePoint::GenerateReward(bool first)
{
    bool change_data = false;

    // �̺�Ʈ�� ����Ǿ��� ���.
    if (current_data_.event_time != reserved_data_.event_time)
        change_data = true;

    // ������ ����Ǿ��� ���.
    if (current_data_.dayofweek != reserved_data_.dayofweek)
        change_data = true;

    if (first)
    {
        if (!generated_)
            change_data = true;
    }

    if (change_data)
    {
        current_data_ = reserved_data_;
        generated_ = false;
    }

    if (!generated_)
    {
        const AccumulatePointParser::RewardGroup& reward_group = 
              AccumulatePointParser::Instance()->get_reward_group(current_data_.dayofweek);

#ifdef _NA_0_20100528_ACCUMULATE_POINT_EVENT
        if (current_data_.event_time)
        {
            current_data_.reward_code = reward_group.reward_event.reward_code;
            current_data_.require_point = reward_group.require_point;
        }
        else
#endif
        {
            uint seed = random(0, (AccumulatePointParser::RewardGroup::Max_Reward - 1));
            current_data_.reward_code = reward_group.reward_array[seed].reward_code;
            current_data_.require_point = reward_group.require_point;
        }
    }

    generated_ = ((current_data_.reward_code > 0) && (current_data_.require_point > 0)) ? true : false;

    return generated_;
}

void AccumulatePoint::ResetReward()
{
    generated_ = false;
}

void AccumulatePoint::SendPacketAccumulatePoint()
{
    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return;

    MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD send_message;
    send_message.m_dwReason = RC::RC_ACCUMULATE_POINT_SUCCESS;
    send_message.m_dwAccumulateTime = playerinfo_->m_dwAccumulateTime;
    send_message.m_wAccumulatePoint = playerinfo_->m_wAccumulatePoint;
    send_message.m_byRequirePoint = current_data_.require_point;
    player_->SendPacket(&send_message, sizeof(send_message));
}

bool AccumulatePoint::PlusAccumulatePoint(ushort accumulate_point)
{
    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return false;

    uint max_point = 65000;
    uint next_point = playerinfo_->m_wAccumulatePoint + accumulate_point;

#ifdef _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION
    const AccumulatePointParser::RewardGroup& reward_group = 
        AccumulatePointParser::Instance()->get_reward_group(global_data.dayofweek);
    
    // ��������Ʈ�� �ޱ����� �ּ� �������� ���� ���( 0�� ��� �ּ� ���� ���Ѿ���)
    if ((reward_group.limit_option.receptible_min_level != 0) && (playerinfo_->m_LV < reward_group.limit_option.receptible_min_level))
        return false;

    // ��������Ʈ�� �ޱ����� �ִ� �������� ���� ���( 0�� ��� �ִ� ���� ���Ѿ���)
    if ((reward_group.limit_option.receptible_max_level != 0) && (playerinfo_->m_LV > reward_group.limit_option.receptible_max_level))
        return false;

    // ��������Ʈ �ִ�ġ ����
    if (reward_group.limit_option.limit_max_point != 0)
        max_point = reward_group.limit_option.limit_max_point;
#endif

    playerinfo_->m_wAccumulatePoint = min(next_point, max_point);

    return (next_point <= max_point) ? true : false;
}

void AccumulatePoint::MinusAccumulatePoint(ushort accumulate_point)
{
    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return;

    playerinfo_->m_wAccumulatePoint = max((playerinfo_->m_wAccumulatePoint - accumulate_point), 0);
}

ushort AccumulatePoint::get_accumulate_point() const
{
    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return 0;

    return playerinfo_->m_wAccumulatePoint;
}

uint AccumulatePoint::get_accumulate_time() const
{
    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return 0;

    return playerinfo_->m_dwAccumulateTime;
}


#endif //_NA_1419_20100121_ACCUMULATE_POINT
