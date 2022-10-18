#pragma once

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
typedef STLX_VECTOR<CHARGUID> TeamBattleGroundMemberList;

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
class TeamBattleGroundMatchingSystem;

struct TeamBattleGroundTeamInfo
{
    CHARGUID master_user_guid;          // 파티장 캐릭터 아이디
    ChaosZoneTeam team_division;        // 팀 구분
    MAPCODE map_code;                   // 맵 코드
    TeamBattleGroundMemberList member_list;
    TeamBattleGroundTeamInfo() :
        master_user_guid(0),
        team_division(kChaosZoneTeamNone),
        map_code(0)
    {
    }
};

class TeamBattleGroundManager : public util::Singleton<TeamBattleGroundManager>
{
public:
    typedef util::CMemoryPoolFactory<TeamBattleGroundTeamInfo> TeamBattleGroundTeamInfoPool;
    typedef STLX_VECTOR<TeamBattleGroundTeamInfo*> TeamBattleGroundTeamInfoList;
    typedef STLX_HASH_MAP<ChaosZoneTeam, TeamBattleGroundMemberList> TeamBattleGroundTeamMemberMap;
public:
    TeamBattleGroundManager();
    ~TeamBattleGroundManager();

    void Init();
    void Release();
    void Update();

    bool LeaveRoom(User* const user, const ZONEKEY zone_key);

    bool InsertReservationTeam(const CHARGUID char_guid, const ChaosZoneTeam team_division, const MAPCODE map_code);
    bool RemoveReservationTeam(const CHARGUID char_guid);
    bool InsertReservation(const CHARGUID char_guid, const ChaosZoneTeam team_division, const MAPCODE map_code);
    bool RemoveReservation(const CHARGUID char_guid);
    bool CreateTeam(const MAPCODE map_code);
    bool RemoveReservationTeamAll();
    void SendPacketAllMember(const TeamBattleGroundMemberList* member_list, MSG_BASE* msg, WORD size);

    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    bool CheckMatching(const MAPCODE map_code); //매칭 확인
    bool IsValidZoneKey(const KEYTYPE zone_key) const;
    KEYTYPE GetZoneKey() const;

    inline const int GetTeamSize() const { return team_battle_ground_team_info_list_.size(); }
    inline TeamBattleGroundTeamInfo* GetTeamInfoFront() const { return team_battle_ground_team_info_list_.front(); }
    inline TeamBattleGroundTeamInfo* GetTeamInfoRandom() const 
    {
        int size = team_battle_ground_team_info_list_.size();
        int index = rand()%size;
        if (index > size)
            return NULL;
        return team_battle_ground_team_info_list_[index] ? team_battle_ground_team_info_list_[index] : NULL; 
    }
    const TeamBattleGroundTeamInfoList& GetTeamInfoList() const { return team_battle_ground_team_info_list_; }

    inline bool IsExistTeam(const ChaosZoneTeam team_division) const
    {
        TeamBattleGroundTeamInfoList::const_iterator const_itr = team_battle_ground_team_info_list_.begin();
        for (; const_itr != team_battle_ground_team_info_list_.end(); ++const_itr)
        {
            TeamBattleGroundTeamInfo* const team_info = *const_itr;
            if (team_info->team_division == team_division)
            {
                return true;
            }
        }
        return false;
    }

private:
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    TeamBattleGroundMatchingSystem* matching_system_;

    TeamBattleGroundTeamInfoPool* team_battle_ground_team_info_pool_;
    TeamBattleGroundTeamInfoList team_battle_ground_team_info_list_;
    TeamBattleGroundMemberList red_team_battle_ground_member_list_;
    TeamBattleGroundMemberList blue_team_battle_ground_member_list_;
};
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME