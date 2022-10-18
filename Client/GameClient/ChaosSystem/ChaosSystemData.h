#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


const int free_pvp_base_room_key = 21000;

namespace ChaosSystem
{
    //-----------------------------------------------------------------------------------
    //  카오스존 Data
    //-----------------------------------------------------------------------------------
    class ChaosZoneData
    {
    public:
        ChaosZoneData();
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        virtual ~ChaosZoneData();
#else
        ~ChaosZoneData();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

        void set_is_wait(bool is_wait) { is_wait_ = is_wait; }
        bool is_wait(){ return is_wait_; }

        void set_chaos_zone_record(CHAOS_ZONE_RECORD_INFOS* record_info)
        {
            memcpy(&chaos_zone_record_, record_info, sizeof(CHAOS_ZONE_RECORD_INFOS));
        }
        CHAOS_ZONE_RECORD_INFOS& chaos_zone_record() { return chaos_zone_record_; }

    private:
        bool is_wait_; // 입장 캐스팅중.
        CHAOS_ZONE_RECORD_INFOS chaos_zone_record_;

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    protected:
        DWORD select_mapcode_;  // 선택된 맵코드
        bool is_on_reservation_; // 예약 중인지
        DWORD reservation_enter_time_; // 입장 대기 시작 시간
        WORD reservation_member_count_; // 예약 대기중 인원

        JoinBattleGroundState battle_ground_open_type_; // 전장 오픈 타입
        DWORD battle_ground_open_time_;  // 전장 오픈 남은 시간
        DWORD battle_ground_open_recv_time_;   // 패킷 받은 시간

    public:
        const DWORD select_mapcode() { return select_mapcode_; }
        void set_select_mapcode(DWORD mapcode) { select_mapcode_= mapcode; }
        bool is_on_reservation() { return is_on_reservation_; }
        void set_is_on_reservation(bool is_reservation) { is_on_reservation_ = is_reservation; }
        void set_reservation_member_count(WORD member_count) { reservation_member_count_ = member_count; }
        WORD reservation_member_count() { return reservation_member_count_; }
        void set_battle_ground_open_type(JoinBattleGroundState open_type) { battle_ground_open_type_ = open_type; }
        JoinBattleGroundState battle_ground_open_type() { return battle_ground_open_type_; }
        void set_battle_ground_open_time(DWORD time_tick) { battle_ground_open_time_ = time_tick; }
        void set_battle_ground_open_recv_time(DWORD time_tick) { battle_ground_open_recv_time_ = time_tick; }
        void Clear();        

        // 다중 접속 잠금
    protected:
        static HANDLE mutex_handle;
    public:
        static BOOL CheckMutexHandle();
        static void ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    };
    ChaosZoneData& GetChaosZoneData();

    //-----------------------------------------------------------------------------------
    //  전장 Data
    //-----------------------------------------------------------------------------------
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    class BattleGroundData : public ChaosZoneData
#else
    class BattleGroundData
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
    public:
        enum { MAX_TEAM_COUNT = 2 };

        enum BATTLE_GROUND_PROGRESS
        {
            PROGRESS_ENTER_ZONE, // 존 입장
            PROGRESS_PLAYING,
            PROGRESS_END,
        };

        typedef STLX_VECTOR<BATTLE_GROUND_TEAM_MEMBER_INFO> BattleGroundMemberContainer;

        BattleGroundData();
        ~BattleGroundData();

        void Clear();
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        const DWORD select_mapcode() { return select_mapcode_; }
        void set_select_mapcode(DWORD mapcode) { select_mapcode_= mapcode; }
        bool is_on_reservation() { return is_on_reservation_; }
        void set_is_on_reservation(bool is_reservation) { is_on_reservation_ = is_reservation; }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        BattleGroundPlayerState player_state() { return player_state_; }
        void set_player_state(BattleGroundPlayerState state) { player_state_ = state; }

        // 전장 맴버 관련
        BattleGroundMemberContainer& member_info_vector() { return member_info_vector_; }
        void SetBattleGroundMemberInfo(BYTE member_count, BATTLE_GROUND_TEAM_MEMBER_INFO* member_info);
        void AddBattleGroundMemberInfo(BATTLE_GROUND_TEAM_MEMBER_INFO* member_info);
        //void DeleteBattleGroundMemberInfo();
        //void GetBattleGroundMemberInfo();
        WORD GetBattleGroundTeamForPlayerName(const TCHAR* player_name);

        void set_team_score_info(const BATTLE_GROUND_TEAM_SCORE_INFO* const score_info);
        BATTLE_GROUND_TEAM_SCORE_INFO* const GetTeamScore(ChaosZoneTeam team);

        // 예약 대기 시간
        void GetEnterWaitTime(OUT TCHAR* message_string);
        void set_reservation_enter_time(DWORD tick_time) { reservation_enter_time_ = tick_time; }
        DWORD reservation_enter_time() { return reservation_enter_time_; }

        // 예약 맴버 수
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        void set_reservation_member_count(WORD member_count) { reservation_member_count_ = member_count; }
        WORD reservation_member_count() { return reservation_member_count_; }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

        // 전장 진행 시간
        void StartBattleGround(); // 전장 시작
        void EndBattleGround(); // 전장 종료
        DWORD GetBattleTime(); // 전장 소요 시간
        BATTLE_GROUND_PROGRESS battle_ground_progress() { return battle_ground_progress_; }

        // 전장 오픈 시간
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        void set_battle_ground_open_type(JoinBattleGroundState open_type) { battle_ground_open_type_ = open_type; }
        JoinBattleGroundState battle_ground_open_type() { return battle_ground_open_type_; }
        void set_battle_ground_open_time(DWORD time_tick) { battle_ground_open_time_ = time_tick; }
        void set_battle_ground_open_recv_time(DWORD time_tick) { battle_ground_open_recv_time_ = time_tick; }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void GetBattleGroundOpenTime(OUT TCHAR* ouput_string);
        void UpdateBattleGroundOpenTime();

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        BattleGroundPlayerState player_state_; // 플레이어 상태
        BattleGroundMemberContainer member_info_vector_;  // 참가 멤버의 팀정보
        BATTLE_GROUND_TEAM_SCORE_INFO team_score_info[MAX_TEAM_COUNT]; // 점수

        BATTLE_GROUND_PROGRESS battle_ground_progress_; // 전장 진행 상태
        DWORD battle_ground_start_tick_;  // 전장 시작 시간
        DWORD battle_ground_end_tick_;  // 전장 종료 시간
#else
    private:
        DWORD select_mapcode_;  // 선택된 맵코드
        bool is_on_reservation_; // 예약 중인지
        DWORD reservation_enter_time_; // 입장 대기 시작 시간
        WORD reservation_member_count_; // 예약 대기중 인원

        BattleGroundPlayerState player_state_; // 플레이어 상태
        BattleGroundMemberContainer member_info_vector_;  // 참가 멤버의 팀정보
        BATTLE_GROUND_TEAM_SCORE_INFO team_score_info[MAX_TEAM_COUNT]; // 점수

        BATTLE_GROUND_PROGRESS battle_ground_progress_; // 전장 진행 상태
        DWORD battle_ground_start_tick_;  // 전장 시작 시간
        DWORD battle_ground_end_tick_;  // 전장 종료 시간

        JoinBattleGroundState battle_ground_open_type_; // 전장 오픈 타입
        DWORD battle_ground_open_time_;  // 전장 오픈 남은 시간
        DWORD battle_ground_open_recv_time_;   // 패킷 받은 시간


        // 다중 접속 잠금
    private:
        HANDLE mutex_handle;
    public:
        BOOL CheckMutexHandle();
        void ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    };
    BattleGroundData& GetBattleGroundData();

    //-----------------------------------------------------------------------------------
    //  자유 전투 지역 Data
    //-----------------------------------------------------------------------------------
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    class FreePVPData
    {
    public:
        typedef STLX_VECTOR<FreePVPRoomInfo> FreePVPRoomContainer;

    public:
        FreePVPData();
        ~FreePVPData();

        void Clear();

        FreePVPRoomContainer* free_pvp_room_vector() {return &free_pvp_room_vector_; }
        void SetFreePVPRoomList(BYTE count_of_room, FreePVPRoomInfo* room_infos);

        FreePVPRoomInfo* my_room_info() { return &my_room_info_; }
        void set_my_room_info(FreePVPRoomInfo* room_info) { memcpy(&my_room_info_,room_info,sizeof(*room_info)); }

    private:
        FreePVPRoomContainer free_pvp_room_vector_;
        FreePVPRoomInfo my_room_info_;
    };
    FreePVPData& GetFreePVPData();
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    bool SortRoomNumber(const FreePVPRoomInfo &a, const FreePVPRoomInfo &b);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    //-----------------------------------------------------------------------------------
    //  골드 러쉬 Data
    //-----------------------------------------------------------------------------------
    typedef BattleGroundPlayerState GoldRushPlayerState;
    typedef JoinBattleGroundState   JoinGoldRushState;
    class GoldRushData : public ChaosZoneData
    {
    public:
        enum { MAX_TEAM_COUNT = 2 };

        enum GOLD_RUSH_PROGRESS
        {
            PROGRESS_ENTER_ZONE, // 존 입장
            PROGRESS_PLAYING,
            PROGRESS_END,
        };
    
        typedef STLX_VECTOR<GOLDRUSH_TEAM_MEMBER_INFO> GoldRushMemberContainer;

    public:
        GoldRushData();
        ~GoldRushData();

        GoldRushPlayerState player_state() { return player_state_; }
        void set_player_state(GoldRushPlayerState state) { player_state_ = state; }        

        // 골드러쉬 맴버 관련
        GoldRushMemberContainer& member_info_vector() { return member_info_vector_; }
        void SetGoldRushMemberInfo(BYTE member_count, GOLDRUSH_TEAM_MEMBER_INFO* member_info);
        void AddGoldRushMemberInfo(GOLDRUSH_TEAM_MEMBER_INFO* member_info);
        WORD GetGoldRushTeamForPlayerName(const TCHAR* player_name);
        WORD get_gold_grade(const TCHAR* character_name);

        void set_team_score_info(const GOLDRUSH_TEAM_SCORE_INFO* const score_info);
        void set_team_score(ChaosZoneTeam team_key, WORD total_gold_point);
        GOLDRUSH_TEAM_SCORE_INFO* const GetTeamScore(ChaosZoneTeam team);

        void Clear();
        // 예약 대기 시간
        void GetEnterWaitTime(OUT TCHAR* message_string);
        void set_reservation_enter_time(DWORD tick_time) { reservation_enter_time_ = tick_time; }
        DWORD reservation_enter_time() { return reservation_enter_time_; }

        // 골드러쉬 진행 시간
        void StartGoldRush(); // 골드러쉬 시작
        void EndGoldRush(); // 골드러쉬 종료
        DWORD GetGoldRushTime(); // 골드러쉬 소요 시간
        GOLD_RUSH_PROGRESS gold_rush_progress() { return gold_rush_progress_; }

    private:
        GoldRushPlayerState player_state_; // 플레이어 상태
        GoldRushMemberContainer member_info_vector_;  // 참가 멤버의 팀정보
        GOLDRUSH_TEAM_SCORE_INFO team_score_info[MAX_TEAM_COUNT]; // 점수 -> 새로운 자료형이 필요하다.

        GOLD_RUSH_PROGRESS gold_rush_progress_; // 골드러쉬 진행 상태
        DWORD gold_rush_start_tick_;  // 골드러쉬 시작 시간
        DWORD gold_rush_end_tick_;  // 골드러쉬 종료 시간
    };
    GoldRushData& GetGoldRushData();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}; //namespace ChaosSystem

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND