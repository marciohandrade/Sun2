#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
class BattleGround;
struct BattleGroundInfo;

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
#include "World/ChaosZoneSystem/ChaosZoneBattleTeam.h"

class BattleGroundTeam : public ChaosZoneBattleTeam
{
public:
    explicit BattleGroundTeam(BattleGround* const battle_ground) :
        battle_ground_(battle_ground),
        battle_ground_Info_(NULL),
        victory_point_(0),
        strongpoint_count_(0),
        is_initialte_vote_(false)
    {
        initialte_vote_timer_.SetTimer(60 * 1000); 
    }
    virtual ~BattleGroundTeam() {}

    void Update();
    void Start();

    void AddMember(Player* const player);

    void AllDeathMemberResurrectionAtSafezone();
    RC::eSTATUS_RESULT OnMoveToSafezone(Player* const player);

    void OnResult(const ChaosZoneResultType result_type);

    void IncreasePlayerKillPoint(Player* const player, Player* const target_player);
    void IncreaseVictroyPoint(const int increase_victory_point);
    void IncreaseStrongpointCount() { ++strongpoint_count_; }
    void DecreaseStrongpointCount() 
    {
        if (strongpoint_count_ > 0)
            --strongpoint_count_; 
    }
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    bool IsInitialteTime();
    bool OnInitiateVote(Player* initiate_player, Player* van_payer);
    void OnVote(CHARGUID char_guid, bool vote_result);
    bool CheckVoteResult();
    void OnVoteResult(bool van_result);
    void OnVanPlayer();

    inline bool GetIsInitiateVote() { return is_initialte_vote_; }
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

    // inline..
    inline void SetBattleGroundInfo(const BattleGroundInfo* battle_ground_Info) { battle_ground_Info_ = battle_ground_Info; }
    inline BYTE GetStrongpointCount() const { return strongpoint_count_; }
    inline WORD GetVictoryPoint() const { return victory_point_; }

private:
    bool _IsExpired();
    void _End();
    void _OnReward(Player* const player, const ChaosZoneResultType result_type);
    void _OnFinishResult(Player* const player, const ChaosZoneResultType result_type);
private:
    BattleGround* battle_ground_;
    const BattleGroundInfo* battle_ground_Info_; // 전장파서

    WORD victory_point_; // 거점 점령 포인트(승점)
    BYTE strongpoint_count_; // 현재 거점 점령 갯수

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    bool is_initialte_vote_;
    TCHAR van_player_name_[MAX_CHARNAME_LENGTH];
    PLAYER_HASH vote_player_hash_map_; // 투표를 진행한 멤버 리스트
    util::Timer voting_timer_; // 투표 진행 시간.. 30초..
    util::Timer initialte_vote_timer_; // 투표 발의 가능 시간. 1분..
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
};

#else
typedef STLX_VECTOR<Player*>			PLAYER_SORT_VECTOR;

class BattleGroundTeam
{
    enum BattleGroundTeamState
    {
        kTeamStateNone = 0,
        kMatch,
        kEnd
    };
public:
    explicit BattleGroundTeam(BattleGround* const battle_ground) :
      battle_ground_(battle_ground),
      battle_ground_Info_(NULL),
      team_key_(kChaosZoneTeamNone),
      team_state_(kTeamStateNone),
      total_level_(0),
      init_member_count_(0),
      victory_point_(0),
      strongpoint_count_(0),
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
      is_initialte_vote_(false)
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
      ,is_win_(false)
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    {
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        initialte_vote_timer_.SetTimer(60 * 1000); 
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    }
    ~BattleGroundTeam()
    {
    }

    void Update();
    void Start();
    
    void AddMember(Player* const player);
    void RemoveMember(const DWORD char_guid);
    Player* FindMember(const DWORD char_guid);

    void SetReadyPlayer(Player* const player);
    void SetReadyPlayer(const BYTE player_count, DWORD* user_key_list);
    bool IsReadyPlayer(const DWORD user_key);

    void AllDeathMemberResurrectionAtSafezone();
    RC::eSTATUS_RESULT OnMoveToSafezone(Player* const player);

    bool IsAllPlayerEntered();

    void IncreasePlayerKillPoint(Player* const player, Player* const target_player);
    void IncreaseVictroyPoint(const int increase_victory_point);
    void IncreaseStrongpointCount() { ++strongpoint_count_; }
    void DecreaseStrongpointCount() 
    {
        if (strongpoint_count_ > 0)
            --strongpoint_count_; 
    }

    void OnResult(const ChaosZoneResultType result_type);

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    bool IsInitialteTime();
    bool OnInitiateVote(Player* initiate_player, Player* van_payer);
    void OnVote(CHARGUID char_guid, bool vote_result);
    bool CheckVoteResult();
    void OnVoteResult(bool van_result);
    void OnVanPlayer();

    inline bool GetIsInitiateVote() { return is_initialte_vote_; }
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

    //packet
    void SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);


    // inline..
    inline void SetBattleGroundInfo(const BattleGroundInfo* battle_ground_Info) { battle_ground_Info_ = battle_ground_Info; }
    
    inline void SetTeamKey(ChaosZoneTeam team_key) { team_key_ = team_key; }
    inline ChaosZoneTeam GetTeamKey() const { return team_key_; }
    
    inline BYTE GetStrongpointCount() const { return strongpoint_count_; }
    inline WORD GetVictoryPoint() const { return victory_point_; }
    inline BYTE GetMemberCount() const { return player_hash_map_.size(); }

    inline bool	IsExpired() const { return team_state_ == kEnd; }

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    inline bool GetIsWin() { return is_win_;}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

private:
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    inline void _SetWinFlag(bool is_win) {is_win_ = is_win;}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    bool _IsExpired();
    void _End();
    void _OnReward(Player* const player, const ChaosZoneResultType result_type);
    void _OnFinishResult(Player* const player, const ChaosZoneResultType result_type);

private:
    BattleGround* battle_ground_;
    const BattleGroundInfo* battle_ground_Info_; // 전장파서
    PLAYER_HASH player_hash_map_; // 팀 멤버
    BYTE ready_player_count_; // 입장하려는 플레이어 수
    STLX_SET<DWORD> ready_user_keys_; // 입장 대기 중인유저키 리스트
    ChaosZoneTeam team_key_; // 팀 키
    BattleGroundTeamState team_state_; // 팀상태
    LEVELTYPE total_level_; // 초기멤버 전체레벨
    BYTE init_member_count_; // 초기멤버 수
    WORD victory_point_; // 거점 점령 포인트(승점)
    BYTE strongpoint_count_; // 현재 거점 점령 갯수
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    bool is_initialte_vote_;
    TCHAR van_player_name_[MAX_CHARNAME_LENGTH];
    PLAYER_HASH vote_player_hash_map_; // 투표를 진행한 멤버 리스트
    util::Timer voting_timer_; // 투표 진행 시간.. 30초..
    util::Timer initialte_vote_timer_; // 투표 발의 가능 시간. 1분..
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    bool is_win_;   //해당 팀의 승리여부를 기억한다
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
