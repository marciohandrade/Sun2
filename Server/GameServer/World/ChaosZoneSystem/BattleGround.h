#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/ChaosZone.h>
#include <World/ChaosZoneSystem/BattleGroundTeam.h>
#include <World/ChaosZoneSystem/BattleGroundBattleResult.h>

class Player;
class PVPInfo;
struct BattleGroundInfo;
class BattleGroundTeam;

struct StrongpointTrigger
{
    DWORD strongpoint_id; // 트리거 아이디
    DWORD sealing_player_key; // 트리거 작동 플레이어 키
    DWORD sealing_success_player_key; // 각인 성공한 플레이어 키
    bool is_start_sealing; // 각인 시작 여부
    bool is_sealing; // 각인 성공 여부
    bool is_conquer; // 점령 성공여부
    ChaosZoneTeam conquer_wait_team_key; // 점령 대기에 성공한 팀 키
    ChaosZoneTeam conquer_team_key; // 점령 성공한 팀 키
    BattleGroundStrongpointState trigger_state; // 트리거 상태
    util::Timer sealing_timer; // 각인 시간
    util::Timer conquer_wait_timer; // 점령대기 시간

    StrongpointTrigger() : 
        strongpoint_id(0),
        sealing_player_key(0),
        sealing_success_player_key(0),
        is_start_sealing(false),
        is_sealing(false),
        is_conquer(false),
        conquer_wait_team_key(kChaosZoneTeamNone),
        conquer_team_key(kChaosZoneTeamNone),
        trigger_state(kStrongpointStateNone)
    {
    }
};

typedef STLX_HASH_MAP<DWORD,StrongpointTrigger*> STRONGPOINT_TRIGGER_HASH;

class BattleGround : public ChaosZone
{
    enum{ kSecond = 1000, };
    enum BattleGroundState
    {
        kNone = 0, 
        kAllEnterWait,  // 플레이어가 다 들어오길 기다리는 상태
        kPrepareWait,   // 준비 대기 상태
        kMatch,         // 전장 진행 상태
        kResult,        // 결과 상태
        kEnd            // 종료 상태
    };
    enum { kTeamMax = 3, };

public:
    BattleGround() :
      battle_ground_state_(kNone),
      winner_team_key_(kChaosZoneTeamNone),
      battle_ground_Info_(NULL),
      battle_ground_battle_result_(NULL),
      is_count_message_(false)
    {
    }
    virtual ~BattleGround(){}

    void Init(GameInstanceDungeon* const instance_dungeon);
    void Release();
    void Update();

    //enter
    void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list);
    bool EnterPlayer(Player* const player);
    void LeavePlayer(Player* const player);

    //pvp 관련
    void DeadCharacter(Character* const dead_character, Character* const kill_character);
    eUSER_RELATION_TYPE	IsFriendPVP(Player* attacker,Player* target);

    // 거점 관련
    bool StartSealing(Player* player, WzID collection_id);
    void CancelSealing(Player* player, WzID collection_id);
    void SuccessSealing(Player* player, WzID collection_id);
    bool CanCheckSealing(Player* player, WzID collection_id) const;

    void BattleGroundPenalty(Player* player);

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    bool IsCanVoteByVictoryPoint();
    RC::eBATTLE_GROUND_RESULT OnInitiateVote(Player* initiate_player, Player* van_payer);
    void OnVote(Player* player, bool vote_result);
    void SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);
#endif //_ NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

    inline DWORD GetRemainTicks() const {return resurrection_timer_.GetRemainTicks(); }
    inline BattleGroundBattleResult* GetBattleGroundBattleResult() const { return battle_ground_battle_result_; }
    inline Player* FindBattleGroundMember(const CHARGUID CharGuid)
    {
        PLAYER_HASH::iterator itr = player_hash_.find(CharGuid);
        if (itr == player_hash_.end())
        {
            return NULL;
        }
        return itr->second;
    }
    inline BattleGroundTeam* GetTeam(ChaosZoneTeam team) { return _FindTeam(team); }
private:
    //match
    void _StartWait();
    void _Start();
    bool _IsAllPlayerEntered();
    void _SentToAllTeamInfo();
    void _LoadTrigger();
    void _UpdateStrongpoint();
    void _IncreaseVictoryPoint();
    void _Result();
    
    //team
    BattleGroundTeam* _CreateTeam(const ChaosZoneTeam team_key);
    void _DestroyTeamAll();
    BattleGroundTeam* _FindTeam(const ChaosZoneTeam team_key);
    BattleGroundTeam* _FindReadyTeam(Player* const player);
    Player* _FindPlayer(const DWORD char_guid);

    //resurrect
    RC::eSTATUS_RESULT _ResurrectionAtSafezone(Player* const player);
    void _AllDeathMemberResurrectionAtSafezone();
    bool _IsBattleGroundEnd();

    //trigger
    StrongpointTrigger* _CreateStrongPoint(WzID strongpoint_id);
    void _DestroyStrongPointAll();
    StrongpointTrigger* _FindStrongPoint(WzID strongpoint_id) const;
    void _SendVictoryPointToAll();

    //packet
    void _SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);

private:
    PLAYER_HASH player_hash_; // 전장을 진행중인 멤버

    // BattleGround Info
    struct 
    { 
        BattleGroundState battle_ground_state_;
        ChaosZoneTeam winner_team_key_;
    };

    // Time Info
    struct 
    {	
        util::Timer start_wait_timer_; // 전장입장 대기 시간 및 입장 후 준비시간.
        ITimerBase resurrection_timer_; // 부활 스케줄링
        ITimerBase victory_point_timer_; // 승점 스케줄링
        bool is_count_message_;
    };

    BattleGroundTeam* battle_ground_team_[kTeamMax]; // 전장 내 팀
    STRONGPOINT_TRIGGER_HASH strongpoint_trigger_hash_; // 전장내 거점

    // 전장파서
    const BattleGroundInfo* battle_ground_Info_;

    // 전투결과창
    BattleGroundBattleResult* battle_ground_battle_result_;

};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
