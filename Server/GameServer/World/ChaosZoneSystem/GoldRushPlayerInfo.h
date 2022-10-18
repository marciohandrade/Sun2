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

    //���� ����..
    void IncreasePlayTimes();
    void IncreaseWinTimes();
    void IncreaseDrawTimes();
    void IncreaseLoseTimes();
    void IncreaseDisTimes();
    void DecreaseDisTimes();
    void IncreaseKill(const WORD kill_point = 1); 
    void IncreaseDeath();
    void IncreaseAssistTimes();

    // ��Ȱ�� �̵� ���� Ÿ�̸�
    inline void SetMoveToSafeZoneTimer(DWORD interval) { move_to_safe_zone_timer_.SetTimer(interval); }
    inline bool IsExpiredMoveToSafeZone() { return move_to_safe_zone_timer_.IsExpiredManual(false); }

    // team_key ����
    inline ChaosZoneTeam GetTeamKey() const { return team_key_;}
    inline void SetTeamKey(const ChaosZoneTeam team_key) { team_key_ = team_key;}

    // �÷��̾� ���� ����
    inline void SetPlayerState(BattleGroundPlayerState player_state) { player_state_ = player_state;}
    inline BattleGroundPlayerState GetPlayerState() const { return player_state_;}

    // ����ų��
    inline WORD GetSeriesKill() { return series_kill_; }
    inline WORD GetMaxSeriesKill() { return max_series_kill_; }

    // �������Ʈ
    // gold_point�� gold_grade�� PlayerInfo�� ����������, CharExtraInfo���� ���Ե��� �ʴ´�.
    // �������Ʈ�� 'ȹ��'
    void AddGoldPoint(const WORD gold);
    // �������Ʈ�� '��������'
    void SetGoldPoint(const WORD gold);
    WORD GetGoldPoint();
    //�� ������ ����Ʈ���� �Ű����� ������ŭ ����Ʈ�� �Ҵ´�.
    WORD LoseGoldPointRatio(const BYTE lose_ratio);

    // �����
    void _AdjustGoldGrade();
    const GOLDGRADE GetGoldGrade();

private:
    Player* player_;
    ChaosZoneTeam team_key_;
    BattleGroundPlayerState player_state_; // �÷��̾� ����
    util::Timer move_to_safe_zone_timer_; // �������� �̵� Ÿ�̸�

    GoldRushRecord* goldrush_record_;

    const GoldRushInfo* goldrush_info_;

    WORD series_kill_; //����ų��
    WORD max_series_kill_; // �ִ� ���� ų��
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH