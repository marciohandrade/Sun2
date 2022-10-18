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
    DWORD strongpoint_id; // Ʈ���� ���̵�
    DWORD sealing_player_key; // Ʈ���� �۵� �÷��̾� Ű
    DWORD sealing_success_player_key; // ���� ������ �÷��̾� Ű
    bool is_start_sealing; // ���� ���� ����
    bool is_sealing; // ���� ���� ����
    bool is_conquer; // ���� ��������
    ChaosZoneTeam conquer_wait_team_key; // ���� ��⿡ ������ �� Ű
    ChaosZoneTeam conquer_team_key; // ���� ������ �� Ű
    BattleGroundStrongpointState trigger_state; // Ʈ���� ����
    util::Timer sealing_timer; // ���� �ð�
    util::Timer conquer_wait_timer; // ���ɴ�� �ð�

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
        kAllEnterWait,  // �÷��̾ �� ������ ��ٸ��� ����
        kPrepareWait,   // �غ� ��� ����
        kMatch,         // ���� ���� ����
        kResult,        // ��� ����
        kEnd            // ���� ����
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

    //pvp ����
    void DeadCharacter(Character* const dead_character, Character* const kill_character);
    eUSER_RELATION_TYPE	IsFriendPVP(Player* attacker,Player* target);

    // ���� ����
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
    PLAYER_HASH player_hash_; // ������ �������� ���

    // BattleGround Info
    struct 
    { 
        BattleGroundState battle_ground_state_;
        ChaosZoneTeam winner_team_key_;
    };

    // Time Info
    struct 
    {	
        util::Timer start_wait_timer_; // �������� ��� �ð� �� ���� �� �غ�ð�.
        ITimerBase resurrection_timer_; // ��Ȱ �����ٸ�
        ITimerBase victory_point_timer_; // ���� �����ٸ�
        bool is_count_message_;
    };

    BattleGroundTeam* battle_ground_team_[kTeamMax]; // ���� �� ��
    STRONGPOINT_TRIGGER_HASH strongpoint_trigger_hash_; // ���峻 ����

    // �����ļ�
    const BattleGroundInfo* battle_ground_Info_;

    // �������â
    BattleGroundBattleResult* battle_ground_battle_result_;

};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
