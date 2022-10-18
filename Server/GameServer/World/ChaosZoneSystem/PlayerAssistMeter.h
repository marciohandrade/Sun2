#ifndef __GAMESERVER_PLAYERASSISTMETER_RECORD_H__
#define __GAMESERVER_PLAYERASSISTMETER_RECORD_H__
#pragma once
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

class Player;
// BattleGround에서 사용
// 플레이어 객체 내에 포함된다.
// 전장에서 자신이 죽었을 때, 자신을 죽인 어시스트 플레이어를 찾기위한 어그로시스템
class PlayerAssistMeter
{
    class Node
    {
    public:
        Node();
        ~Node();

        void    Init();
        //void    Update();

        void    SetCharGuid(const CHARGUID _char_guid);
        DWORD   GetCharGuid() const;

        void    PlusAssistDamagePoint(const DWORD _assist_point);
        void    SetAssistDamagePoint(const DWORD _assist_point);
        DWORD   GetAssistDamagePoint() const;

    private:
        CHARGUID character_guid_;
        DWORD assist_damage_point_;
    };
    //----------------------------------------------------------------------------------------------
    // records를 내림차순정렬하기위한 typedef와 functor
    typedef std::pair<CHARGUID, Node> PairRecord;
    typedef std::vector<PairRecord> VectorRecords;
    class GreaterSortRecords : std::unary_function<PairRecord, bool>
    {
    public:
        bool operator()(const PairRecord& lhs, const PairRecord& rhs)
        {
            return lhs.second.GetAssistDamagePoint() > rhs.second.GetAssistDamagePoint();
        }
    };
    //==============================================================================================
public:
    PlayerAssistMeter(Player* player);
    ~PlayerAssistMeter();

    typedef STLX_MAP<CHARGUID, Node> Records;

    void Init();
    //void Update();

    PlayerAssistMeter::Node* Add(const Player* _attacker);
    PlayerAssistMeter::Node* GetNode(const CHARGUID _char_guid);

    bool Remove(const CHARGUID _char_guid);
    void Clear();

    Player* GetBestAssistPlayer(const CHARGUID _killer_guid);        // Assister를 리턴

    void AddAssistPoint(Character* _attacker, const DWORD _damage);     // 어시스트포인트 증가시킨다.

private:
    Player* player_;
    Records records_;

    __DISABLE_COPY(PlayerAssistMeter);
};
//==================================================================================================
inline PlayerAssistMeter::Node::Node()
{
    ZeroMemory(this, sizeof(*this));
}
inline PlayerAssistMeter::Node::~Node() {}

inline void PlayerAssistMeter::Node::Init()
{
    new (this) Node();
}

inline void PlayerAssistMeter::Node::SetCharGuid(const CHARGUID _char_guid)
{
    character_guid_ = _char_guid;
}
inline DWORD PlayerAssistMeter::Node::GetCharGuid() const
{
    return character_guid_;
}

inline void PlayerAssistMeter::Node::PlusAssistDamagePoint(const DWORD _assist_point)
{
    assist_damage_point_ += _assist_point;
}
inline void PlayerAssistMeter::Node::SetAssistDamagePoint(const DWORD _assist_point)
{
    assist_damage_point_ = _assist_point;
}
inline DWORD PlayerAssistMeter::Node::GetAssistDamagePoint() const
{
    return assist_damage_point_;
}
//--------------------------------------------------------------------------------------------------
inline PlayerAssistMeter::PlayerAssistMeter(Player* player)
: player_(player)
{
}
inline PlayerAssistMeter::~PlayerAssistMeter() {}

inline PlayerAssistMeter::Node* PlayerAssistMeter::GetNode(const CHARGUID _char_guid)
{
    Records::iterator it = records_.find(_char_guid);
    return it != records_.end() ? &it->second : NULL;
}

#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#endif //__GAMESERVER_PLAYERASSISTMETER_RECORD_H__