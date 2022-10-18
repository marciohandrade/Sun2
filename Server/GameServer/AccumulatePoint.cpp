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


// 현재 년도 및 플레이어의 게임 플레이 증가 시간(초 단위)을 설정 한다.
// 서버가 실행되는 동안 해당 년도가 바뀔 수 있기 때문에 Update 동안 호출한다.
// 년도 및 플레이어의 증가 시간은 여러 AccumulatePoint 객체가 공통으로 사용 하기에 전역으로 선언한다. 
// 매 Update마다 틱 카운트를 누적해 1초 이상인 경우 AccumulatePoint::increase_second_에 설정.
// 각 플레이어의 시간 계산을 전역적으로 1번만 하기 위한 방편이다.
void AccumulatePoint::Calculate_Current_Year(uint delta_tick_count)
{
    static TempTimer second_timer(ONE_SECOND);

    // 누적된 초값을 전역적으로 저장해놓는다. 플레이어 당 매번 계산하는 것 보다 어느 정도의 오차를 
    // 허용하므로 전역적으로 계산을 1번 하는게 낫다.
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

// Player 메모리 풀에서 생성자및 소멸자를 호출이 일어나지 않아 초기화 부분을 정의함.
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

// Player::Update() 내부에서 호출된다.
// Player의 접속시간을 (초 단위)로 증가 시키며, 특정 시간 마다 마일리지 코인을 1 증가 시킨다.
// 만약, 매년 1월 1일(즉 해당 연도가 바뀔 경우)일때 적립시간을 초기화 시키며 코인은 그대로 유지한다.
void AccumulatePoint::Update()
{
    // 플레이어가 ZONE 에 존재 하지 않는 경우 Update()를 실행하지 않는다.
    // ZONE 에 존재 유무 판별은 class PlayerManager::Update() 에서 체크하고있다.
    // 만약, ZONE의 판별을 직접처리 해야할 경우 아래의 구문을 사용한다.

    //GameZone* pEnterZone = NULL;
    //MAPCODE EnterFieldCode = 0;
    //const eZONE_STATE eRoomState = pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
    //if ( !(eRoomState & ePRS_AT_ZONE) )
    //    return;



    // 아직 EnterField 전이면 Update를 실행하지 않는다.
    // EnterField 판별문은 class Player::Update() 시작시 체크하고있다.
    // 만약, EnterField의 판별을 직접처리 해야할 경우 아래의 구문을 사용한다.

    //GameField* pField = player_pointer_->GetField();
    //if ( !(pField && pField->IsReady()) )
    //    return;

    ASSERT(player_ && playerinfo_);
    if (player_ == NULL || playerinfo_ == NULL)
        return;


    static MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD send_message;


    // 초 단위로 증가된 값이 존재할때만 체크한다.
    if (global_data.increase_second == 0)
        return;

    const AccumulatePointParser::RewardGroup& reward_group = 
                    AccumulatePointParser::Instance()->get_reward_group(global_data.dayofweek);

    // 0으로 나누기 오류 피하기
    // 해당 값을 0으로 세팅하는 국가에 대량으로 로그가 남는 경우가 존재하여 주석 처리함.
    // ASSERT(reward_group.generate_time);
    if (reward_group.generate_time == 0)
        return;

    // 년도가 바뀐경우 적립시간을 초기화 시킨다.
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

    // 보상에 필요한 요구 코인이 변경된 경우.
    if (reward_group.require_point != reserved_data_.require_point)
    {
        reserved_data_.require_point = reward_group.require_point;

        send_message.m_dwReason = RC::RC_ACCUMULATE_POINT_CHANGE_REQUIRE_POINT;
        send_message.m_dwAccumulateTime = playerinfo_->m_dwAccumulateTime;
        send_message.m_wAccumulatePoint = playerinfo_->m_wAccumulatePoint;
        send_message.m_byRequirePoint = reward_group.require_point;
        player_->SendPacket(&send_message, sizeof(send_message));
    }


    // 현재의 요일이 변경된 경우
    reserved_data_.dayofweek = global_data.dayofweek;

    // 이벤트 시간이 변경된 경우
    reserved_data_.event_time = global_data.event_time;



    // 마일리지 시간을 누적시킨다.(초 단위)
    // 시간 변화 : 증가 전 시간 - 증가 후 시간
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

    // 이벤트가 변경되었을 경우.
    if (current_data_.event_time != reserved_data_.event_time)
        change_data = true;

    // 요일이 변경되었을 경우.
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
    
    // 적립포인트를 받기위한 최소 레벨보다 작은 경우( 0인 경우 최소 레벨 제한없음)
    if ((reward_group.limit_option.receptible_min_level != 0) && (playerinfo_->m_LV < reward_group.limit_option.receptible_min_level))
        return false;

    // 적립포인트를 받기위한 최대 레벨보다 작은 경우( 0인 경우 최대 레벨 제한없음)
    if ((reward_group.limit_option.receptible_max_level != 0) && (playerinfo_->m_LV > reward_group.limit_option.receptible_max_level))
        return false;

    // 적립포인트 최대치 설정
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
