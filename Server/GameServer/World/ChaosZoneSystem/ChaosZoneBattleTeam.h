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

    //Agent�� MatchingSystem���� �޾ƿ� ������ ������ ReadyPlayer�� �����صд�.
    //GameServer���� ������ ������ Zone�� ������ ��� ���������� �� ��ü�� ��ϵȴ�.
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
    ChaosZoneTeam team_key_; // �� Ű
    ChaosZoneTeamState team_state_; //���� ����
    PLAYER_HASH player_hash_map_; // �� ���

    BYTE ready_player_count_; // �����Ϸ��� �÷��̾� ��
    STLX_SET<DWORD> ready_user_keys_; // ���� ��� ��������Ű ����Ʈ

    bool is_win_;   //�ش� ���� �¸����θ� ����Ѵ�
};