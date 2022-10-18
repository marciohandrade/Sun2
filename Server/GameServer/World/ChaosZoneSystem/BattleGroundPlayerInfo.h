#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
class Player;

class BattleGroundPlayerInfo
{
public:
    BattleGroundPlayerInfo(Player* player) : 
      player_(player),
      team_key_(kChaosZoneTeamNone),
      player_state_(kPlayerStateNone),
      battle_ground_record_(NULL),
      series_kill_(0),
      max_series_kill_(0)
    {
    }
    ~BattleGroundPlayerInfo()
    {
    }
    void Init(BattleGroundRecord* const record);
    void Release();

    //���� ����..
    void IncreasePlayTimes();
    void IncreaseWinTimes();
    void IncreaseDrawTimes();
    void IncreaseLoseTimes();
    void IncreaseDisTimes();
    void DecreaseDisTimes();
    void IncreaseKill(const WORD kill_point = 1); 
    void IncreaseDeath();
    void IncreaseStrongpointAttack();
    void IncreaseStrongpointDefense();
    void ClearScore();
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    void IncreaseAssistTimes();
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    WORD GetKillTimes();
    WORD GetAssistTimes();
    WORD GetWintimes();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    // team_key ����
    inline ChaosZoneTeam GetTeamKey() const { return team_key_;}
    inline void SetTeamKey(const ChaosZoneTeam team_key) { team_key_ = team_key;}

    // �÷��̾� ���� ����
    inline void SetPlayerState(BattleGroundPlayerState player_state) { player_state_ = player_state;}
    inline BattleGroundPlayerState GetPlayerState() const { return player_state_;}

    // ��Ȱ������ �̵� ���� Ÿ�̸�..
    inline void SetMoveToSafeZoneTimer(DWORD interval) { move_to_safe_zone_timer_.SetTimer(interval); }
    inline bool IsExpiredMoveToSafeZone() { return move_to_safe_zone_timer_.IsExpiredManual(false); }

    // ����ų��
    inline WORD GetSeriesKill() { return series_kill_; }
    inline WORD GetMaxSeriesKill() { return max_series_kill_; }

private:
    Player* player_;
    ChaosZoneTeam team_key_;
    BattleGroundPlayerState player_state_; // �÷��̾� ����
    util::Timer move_to_safe_zone_timer_; // �������� �̵� Ÿ�̸�

    BattleGroundRecord* battle_ground_record_;

    WORD series_kill_; //����ų��
    WORD max_series_kill_; // �ִ� ���� ų��
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
