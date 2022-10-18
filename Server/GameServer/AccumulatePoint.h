#ifndef SUN_GAMESERVER_ACCUMULATEPOINT_H_
#define SUN_GAMESERVER_ACCUMULATEPOINT_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

/* 클래스 다이어그램 : 2010.03.03 by 남기봉

 GameScriptParserManager <*>--------------> AccumulatePointParser
                                                    A
                                                    |
                                                    |
 Player <*>----------> AccumulatePoint <>-----------|
                               A
                               |
                               |
 PlayerManager <>--------------|
                               |
                               |
 AccumulatePointImplement <m>--|
            A
            |
            |
           <*>        
 CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN or CG_ITEM_USE_ACCUMULATE_POINT_SYN

*/ 

/* class AccumulatePoint : 2010.03.03 by 남기봉

- PlayerManager::Update() 에서 AccumulatePoint::Calculate_Current_Year()를 호출함으로 해서
  적립년도의 초기화를 담당한다.

- Player::Update()에서 AccumulatePoint::Update() 호출함으로 해서
  해당 플레이어의 일정 주기로 적립 포인트를 누적한다.

- class AccumulatePointParser를 이용해 보상그룹코드 와 필요 포인트를 알아낸다.

*/

class Player;
class AccumulatePointParser;

class AccumulatePoint
{
public:
    // 해당 년도 및 증가 시간(초 단위)을 측정한다.
    static void Calculate_Current_Year(uint delta_tick_count);
    static void SyncMatchDate(ushort year, ushort dayofweek, uint secondofday);

public:
    AccumulatePoint(Player* player);
    virtual ~AccumulatePoint();

    // Player의 접속시간을 (초 단위)로 증가 시키며, 특정 시간 마다 마일리지 코인을 1 증가 시킨다.
    void Update();

    void Init(Player* player);
    void Release();

    void SendPacketAccumulatePoint();

    bool GenerateReward(bool first);
    void ResetReward();

    uint get_reward_code() const;
    uchar get_require_point() const;
    ushort get_accumulate_point() const;
    uint get_accumulate_time() const;

    bool PlusAccumulatePoint(ushort accumulate_point);
    void MinusAccumulatePoint(ushort accumulate_point);

private:
    Player* player_;
    BASE_PLAYERINFO* playerinfo_;

    struct Value
    {
        uint    reward_code;
        uchar   require_point;
        ushort  dayofweek;
        bool    event_time;
    };

    Value   current_data_;
    Value   reserved_data_;
    bool    generated_;
};

inline uchar AccumulatePoint::get_require_point() const
{
    return current_data_.require_point;
}

inline uint AccumulatePoint::get_reward_code() const
{
    return current_data_.reward_code;
}


#endif //_NA_1419_20100121_ACCUMULATE_POINT

#endif //SUN_GAMESERVER_ACCUMULATEPOINT_H_
