#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/ChaosZoneBattleTeam.h"

class GoldRush;
class GoldRushPlayerInfo;
struct BattleGroundInfo;

class GoldRushTeam : public ChaosZoneBattleTeam
{
public:
    explicit GoldRushTeam(GoldRush* const goldrush) :
    goldrush_(goldrush),
    battle_ground_Info_(NULL),
    is_initialte_vote_(false)
    {
        initialte_vote_timer_.SetTimer(60 * 1000);
    }
    virtual ~GoldRushTeam() {}

    void Update();
    void Start();

    void AddMember(Player* const player);

    RC::eSTATUS_RESULT OnMoveToSafezone(Player* const player);

    void OnResult(const ChaosZoneResultType result_type);

    //vote system
    bool IsInitialteTime();
    bool OnInitiateVote(Player* initiate_player, Player* van_payer);
    void OnVote(CHARGUID char_guid, bool vote_result);
    bool CheckVoteResult();
    void OnVoteResult(bool van_result);
    void OnVanPlayer();

    void AllDeathMemberResurrectionAtSafezone();

    // 팀정보 채워넣기
    void GetTeamMemberInfo(OUT GOLDRUSH_TEAM_SCORE_INFO& team_info);

    // 획득골드를 팀원들에게 분배
    // 적플레이어를 죽인 플레이어가 있을경우 (보너스 골드포인트를 받을 플레이어)
    WORD DistributeGoldPoint(WORD gold, const CHARGUID bonus_player = 0);
    // 전장 시작 시 사용되는 함수. 각 멤버들의 골드값을 매개변수값 만큼 더해준다
    void ObtainGoldPoint(WORD gold/*개개인이 획득하는 골드값*/);
    const WORD GetTotalTeamGoldPoint();

    inline void SetBattleGroundInfo(const BattleGroundInfo* battle_ground_Info) { battle_ground_Info_ = battle_ground_Info; }
    inline bool GetIsInitiateVote() { return is_initialte_vote_; }

private:
    void _OnReward(Player* const player, const ChaosZoneResultType result_type);
    void _OnFinishResult(Player* const player, const ChaosZoneResultType result_type);

private:
    GoldRush* goldrush_;
    const BattleGroundInfo* battle_ground_Info_; // 전장파서
    //goldrushinfo

    WORD team_gold_point_;

    //vote system
    bool is_initialte_vote_;
    TCHAR van_player_name_[MAX_CHARNAME_LENGTH];
    PLAYER_HASH vote_player_hash_map_; // 투표를 진행한 멤버 리스트
    util::Timer voting_timer_; // 투표 진행 시간.. 30초..
    util::Timer initialte_vote_timer_; // 투표 발의 가능 시간. 1분..
};

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH