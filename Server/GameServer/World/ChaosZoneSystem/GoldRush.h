#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include <World/ChaosZoneSystem/ChaosZone.h>

class GoldRushTeam;
struct BattleGroundInfo;
struct GoldRushInfo;
class GoldRushMonsterPointParser;

class GoldRushBattleResult;

class GoldRush : public ChaosZone
{
    enum{ kSecond = 1000, };
    enum eGOLDRUSH_STATE
    {
        eGOLDRUSH_NONE = 0, 
        eGOLDRUSH_ENTERWAIT,  // 플레이어가 다 들어오길 기다리는 상태
        eGOLDRUSH_PREPAREWAIT,   // 준비 대기 상태
        eGOLDRUSH_MATCHED,         // 전장 진행 상태
        eGOLDRUSH_RESULT,        // 결과 상태
        eGOLDRUSH_END,            // 종료 상태
        eGOLDRUSH_MAX
    };
    enum { kTeamMax = 3, };
public:
    GoldRush() :
    battle_ground_Info_(NULL),
    goldrush_battle_result_(NULL),
    is_lastboss_dead_(false),
    goldrush_state_(eGOLDRUSH_NONE),
    goldrush_info_(NULL)
    {}
    virtual ~GoldRush() {}

    void Init(GameInstanceDungeon* const instance_dungeon);
    void Update();
    void Release();

    bool EnterPlayer(Player* const player);
    void LeavePlayer(Player* const player);
    void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list);

    void DeadCharacter(Character* const dead_character, Character* const kill_character);
    void DeadPlayer(Player* const dead_player, Character* const kill_character);
    void DeadMonster(Monster* const dead_monster, Character* const kill_character);
    //두 플레이어의 관계를 리턴
    eUSER_RELATION_TYPE IsFriendPVP(Player* attacker,Player* target);

    Player* FindGoldRushMember(const CHARGUID CharGuid);

    //vote system
    RC::eBATTLE_GROUND_RESULT OnInitiateVote(Player* initiate_player, Player* van_payer);
    void OnVote(Player* player, bool vote_result);
    void SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);

    void GoldRushPenalty(Player* player);

    inline GoldRushBattleResult* GetGoldRushBattleResult() {return goldrush_battle_result_;}
    inline DWORD GetRemainTicks() const {return resurrection_timer_.GetRemainTicks(); }
    inline const GoldRushInfo* GetGoldRushInfo() { return goldrush_info_; }
private:
    void _Start();
    void _StartWait();
    void _Result();
    bool _IsAllPlayerEntered();

    void _SentToAllTeamInfo();
    void _SendTotalTeamPoint();

    GoldRushTeam* _CreateTeam(const ChaosZoneTeam team_key);
    GoldRushTeam* _FindTeam(const ChaosZoneTeam team_key);
    GoldRushTeam* _FindReadyTeam(Player* const player);
    Player* _FindPlayer(const DWORD char_guid);
    void _DestroyTeamAll();

    void _AllDeathMemberResurrectionAtSafezone();

    bool _IsGoldRushEnd();

    //packet
    void _SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size);

private:
    PLAYER_HASH player_hash_; // 전장을 진행중인 멤버
    GoldRushTeam* goldrush_team_[kTeamMax]; //골드러시 팀

    struct 
    {
        eGOLDRUSH_STATE goldrush_state_;
        ChaosZoneTeam winner_team_key_;
    };

    // 전장파서
    const BattleGroundInfo* battle_ground_Info_;
    const GoldRushInfo* goldrush_info_;
    const GoldRushMonsterPointParser* monsterpoint_parser_;

    GoldRushBattleResult* goldrush_battle_result_;

    //Time info
    struct
    {
        util::Timer limit_timer_;       // 골드러시 제한 시간.
        util::Timer extra_timer_;       // 엑스트라 타이머, 라스트보스가 죽으면 동작
        util::Timer start_wait_timer_; // 전장입장 대기 시간 및 입장 후 준비시간.
        ITimerBase resurrection_timer_; // 부활 스케줄링
    };
    bool is_count_message_;  // 클라의 x초 남았습니다 메시지를 위한 bool
    bool is_lastboss_dead_;  //최종보스 사망여부 체크
};

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH