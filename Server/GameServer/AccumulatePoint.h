#ifndef SUN_GAMESERVER_ACCUMULATEPOINT_H_
#define SUN_GAMESERVER_ACCUMULATEPOINT_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

/* Ŭ���� ���̾�׷� : 2010.03.03 by �����

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

/* class AccumulatePoint : 2010.03.03 by �����

- PlayerManager::Update() ���� AccumulatePoint::Calculate_Current_Year()�� ȣ�������� �ؼ�
  �����⵵�� �ʱ�ȭ�� ����Ѵ�.

- Player::Update()���� AccumulatePoint::Update() ȣ�������� �ؼ�
  �ش� �÷��̾��� ���� �ֱ�� ���� ����Ʈ�� �����Ѵ�.

- class AccumulatePointParser�� �̿��� ����׷��ڵ� �� �ʿ� ����Ʈ�� �˾Ƴ���.

*/

class Player;
class AccumulatePointParser;

class AccumulatePoint
{
public:
    // �ش� �⵵ �� ���� �ð�(�� ����)�� �����Ѵ�.
    static void Calculate_Current_Year(uint delta_tick_count);
    static void SyncMatchDate(ushort year, ushort dayofweek, uint secondofday);

public:
    AccumulatePoint(Player* player);
    virtual ~AccumulatePoint();

    // Player�� ���ӽð��� (�� ����)�� ���� ��Ű��, Ư�� �ð� ���� ���ϸ��� ������ 1 ���� ��Ų��.
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
