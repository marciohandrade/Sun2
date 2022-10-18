#pragma once

#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

class BattleRecorder;
struct BattleGroundPoints
{
    USERGUID	user_guid;
    RANKINGNO	ranking_no;

    CHARGUID char_guid; //player->GetCharGuid();
    BOOL		is_ready;

    RANKINGKEY win_db_key;
    DWORD win_point;
    RANKINGKEY kill_db_key;
    DWORD kill_point;
    RANKINGKEY assist_db_key;
    DWORD assist_point;

    BattleGroundPoints() : win_db_key(0), win_point(0), kill_db_key(0), kill_point(0), assist_db_key(0),
                           assist_point(0), user_guid(0), ranking_no(0), char_guid(0), is_ready(FALSE)
    {}
   
};

class BattleGroundRankingManager
{
    typedef STLX_HASH_MAP<USERGUID, BattleGroundPoints>	BATTLEGROUND_POINT_HASH;
    enum eBATTLEGROUND_RANKING_TYPE {
        eBATTLEGROUND_RANKING_TYPE_WIN = 1,
        eBATTLEGROUND_RANKING_TYPE_KILL = 2,
        eBATTLEGROUND_RANKING_TYPE_ASSIST = 3,
        eBATTLEGROUND_RANKING_TYPE_MAX = 4,
    };
public:
    VOID OnCreate() {}
    VOID OnDestroy() {}

    VOID EnterPlayer(Player* player, MAPCODE map_code);     //�÷��̾��� ���� ������� ��ŷ����ó��
    VOID BattleGround_LeavePlayer(Player* player, MAPCODE map_code, const BattleRecorder::Record* record, 
                                  bool is_win); //�÷��̾��� ���� ������� ��ŷ����ó��
    
    VOID SetPlayerPoint( USERGUID user_guid, DWORD point, RANKINGKEY db_key, RANKINGTYPE ranking_type );

    BattleGroundRankingManager() {}
    ~BattleGroundRankingManager() {}
private:
    // �� ��ü�� enum�� eRanking_System_Type�� ��Ī��Ų��.
    // ������ �� ��ü �������� eBattleground_Ranking_Type�� ����ϵ����Ѵ�.
    // ��������� �����԰� ���� eRanking_System_Type�� Ÿ���߰��� ����� �۾�.
    RANKINGTYPE ConvertRankingType(eBATTLEGROUND_RANKING_TYPE temp_type);

    BATTLEGROUND_POINT_HASH	hash_player_points_;
};

#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif // _NA_008012_20150130_RANKING_SYSTEM