#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
class Player;
struct GoldRushInfo;

class GoldRushPlayerInfo
{
public:
    GoldRushPlayerInfo(Player* player) :
    player_(player),
    team_key_(kChaosZoneTeamNone),
    player_state_(kPlayerStateNone),
    goldrush_record_(NULL),
    goldrush_info_(NULL),
    series_kill_(0),
    max_series_kill_(0)
    {}
    ~GoldRushPlayerInfo() {}

    void Init(GoldRushRecord* const record);
    void Release() {}

    //전적 관련..
    void IncreasePlayTimes();
    void IncreaseWinTimes();
    void IncreaseDrawTimes();
    void IncreaseLoseTimes();
    void IncreaseDisTimes();
    void DecreaseDisTimes();
    void IncreaseKill(const WORD kill_point = 1); 
    void IncreaseDeath();
    void IncreaseAssistTimes();

    // 부활존 이동 관련 타이머
    inline void SetMoveToSafeZoneTimer(DWORD interval) { move_to_safe_zone_timer_.SetTimer(interval); }
    inline bool IsExpiredMoveToSafeZone() { return move_to_safe_zone_timer_.IsExpiredManual(false); }

    // team_key 관련
    inline ChaosZoneTeam GetTeamKey() const { return team_key_;}
    inline void SetTeamKey(const ChaosZoneTeam team_key) { team_key_ = team_key;}

    // 플레이어 상태 관련
    inline void SetPlayerState(BattleGroundPlayerState player_state) { player_state_ = player_state;}
    inline BattleGroundPlayerState GetPlayerState() const { return player_state_;}

    // 연속킬수
    inline WORD GetSeriesKill() { return series_kill_; }
    inline WORD GetMaxSeriesKill() { return max_series_kill_; }

    // 골드포인트
    // gold_point와 gold_grade는 PlayerInfo로 관리하지만, CharExtraInfo에는 포함되지 않는다.
    // 골드포인트값 '획득'
    void AddGoldPoint(const WORD gold);
    // 골드포인트값 '강제설정'
    void SetGoldPoint(const WORD gold);
    WORD GetGoldPoint();
    //총 소지한 포인트에서 매개변수 비율만큼 포인트를 잃는다.
    WORD LoseGoldPointRatio(const BYTE lose_ratio);

    // 골드등급
    void _AdjustGoldGrade();
    const GOLDGRADE GetGoldGrade();

private:
    Player* player_;
    ChaosZoneTeam team_key_;
    BattleGroundPlayerState player_state_; // 플레이어 상태
    util::Timer move_to_safe_zone_timer_; // 안전지대 이동 타이머

    GoldRushRecord* goldrush_record_;

    const GoldRushInfo* goldrush_info_;

    WORD series_kill_; //연속킬수
    WORD max_series_kill_; // 최대 연속 킬수
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH