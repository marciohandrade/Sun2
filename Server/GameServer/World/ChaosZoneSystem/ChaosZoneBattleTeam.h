#pragma once

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
typedef STLX_VECTOR<Player*>			PLAYER_SORT_VECTOR;

class ChaosZoneBattleTeam
{
protected:
    enum ChaosZoneTeamState {
        eCHAOSTEAM_STATE_NONE = 0,
        eCHAOSTEAM_STATE_MATCHED,
        eCHAOSTEAM_STATE_END,
        eCHAOSTEAM_MAX
        
    };
public:
    ChaosZoneBattleTeam() :
    team_key_(kChaosZoneTeamNone),
    team_state_(eCHAOSTEAM_STATE_NONE),
    is_win_(false)
    {}
    virtual ~ChaosZoneBattleTeam() {}

    virtual void Update()=0;
    virtual void Start()=0;

    //Team Member Control
    virtual void AddMember(Player* const player)=0;
    void RemoveMember(const DWORD char_guid);
    Player* FindMember(const DWORD char_guid);

    //Agent의 MatchingSystem에서 받아온 유저들 정보를 ReadyPlayer로 저장해둔다.
    //GameServer에서 유저가 실제로 Zone에 입장할 경우 실질적으로 팀 객체에 등록된다.
    //About ReadyPlayer
    void SetReadyPlayer(Player* const player);
    void SetReadyPlayer(const BYTE player_count, DWORD* user_key_list);
    bool IsReadyPlayer(const DWORD user_key);
    bool IsAllPlayerEntered();

    virtual RC::eSTATUS_RESULT OnMoveToSafezone(Player* const player)=0;

    virtual void OnResult(const ChaosZoneResultType result_type)=0;

    //packet send
    void SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);

    //inline function
    inline BYTE GetMemberCount() const { return player_hash_map_.size(); }
    inline void SetTeamKey(ChaosZoneTeam team_key) { team_key_ = team_key; }
    inline ChaosZoneTeam GetTeamKey() const { return team_key_; }
    inline bool	IsExpired() const { return team_state_ == eCHAOSTEAM_STATE_END; }
    inline bool GetIsWin() { return is_win_;}

protected:
    inline void _SetWinFlag(bool is_win) {is_win_ = is_win;}
protected:
    ChaosZoneTeam team_key_; // 팀 키
    ChaosZoneTeamState team_state_; //팀의 상태
    PLAYER_HASH player_hash_map_; // 팀 멤버

    BYTE ready_player_count_; // 입장하려는 플레이어 수
    STLX_SET<DWORD> ready_user_keys_; // 입장 대기 중인유저키 리스트

    bool is_win_;   //해당 팀의 승리여부를 기억한다
};