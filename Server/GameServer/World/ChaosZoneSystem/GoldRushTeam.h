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

    // ������ ä���ֱ�
    void GetTeamMemberInfo(OUT GOLDRUSH_TEAM_SCORE_INFO& team_info);

    // ȹ���带 �����鿡�� �й�
    // ���÷��̾ ���� �÷��̾ ������� (���ʽ� �������Ʈ�� ���� �÷��̾�)
    WORD DistributeGoldPoint(WORD gold, const CHARGUID bonus_player = 0);
    // ���� ���� �� ���Ǵ� �Լ�. �� ������� ��尪�� �Ű������� ��ŭ �����ش�
    void ObtainGoldPoint(WORD gold/*�������� ȹ���ϴ� ��尪*/);
    const WORD GetTotalTeamGoldPoint();

    inline void SetBattleGroundInfo(const BattleGroundInfo* battle_ground_Info) { battle_ground_Info_ = battle_ground_Info; }
    inline bool GetIsInitiateVote() { return is_initialte_vote_; }

private:
    void _OnReward(Player* const player, const ChaosZoneResultType result_type);
    void _OnFinishResult(Player* const player, const ChaosZoneResultType result_type);

private:
    GoldRush* goldrush_;
    const BattleGroundInfo* battle_ground_Info_; // �����ļ�
    //goldrushinfo

    WORD team_gold_point_;

    //vote system
    bool is_initialte_vote_;
    TCHAR van_player_name_[MAX_CHARNAME_LENGTH];
    PLAYER_HASH vote_player_hash_map_; // ��ǥ�� ������ ��� ����Ʈ
    util::Timer voting_timer_; // ��ǥ ���� �ð�.. 30��..
    util::Timer initialte_vote_timer_; // ��ǥ ���� ���� �ð�. 1��..
};

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH