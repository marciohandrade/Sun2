#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


const int free_pvp_base_room_key = 21000;

namespace ChaosSystem
{
    //-----------------------------------------------------------------------------------
    //  ī������ Data
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
        bool is_wait_; // ���� ĳ������.
        CHAOS_ZONE_RECORD_INFOS chaos_zone_record_;

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    protected:
        DWORD select_mapcode_;  // ���õ� ���ڵ�
        bool is_on_reservation_; // ���� ������
        DWORD reservation_enter_time_; // ���� ��� ���� �ð�
        WORD reservation_member_count_; // ���� ����� �ο�

        JoinBattleGroundState battle_ground_open_type_; // ���� ���� Ÿ��
        DWORD battle_ground_open_time_;  // ���� ���� ���� �ð�
        DWORD battle_ground_open_recv_time_;   // ��Ŷ ���� �ð�

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

        // ���� ���� ���
    protected:
        static HANDLE mutex_handle;
    public:
        static BOOL CheckMutexHandle();
        static void ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    };
    ChaosZoneData& GetChaosZoneData();

    //-----------------------------------------------------------------------------------
    //  ���� Data
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
            PROGRESS_ENTER_ZONE, // �� ����
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

        // ���� �ɹ� ����
        BattleGroundMemberContainer& member_info_vector() { return member_info_vector_; }
        void SetBattleGroundMemberInfo(BYTE member_count, BATTLE_GROUND_TEAM_MEMBER_INFO* member_info);
        void AddBattleGroundMemberInfo(BATTLE_GROUND_TEAM_MEMBER_INFO* member_info);
        //void DeleteBattleGroundMemberInfo();
        //void GetBattleGroundMemberInfo();
        WORD GetBattleGroundTeamForPlayerName(const TCHAR* player_name);

        void set_team_score_info(const BATTLE_GROUND_TEAM_SCORE_INFO* const score_info);
        BATTLE_GROUND_TEAM_SCORE_INFO* const GetTeamScore(ChaosZoneTeam team);

        // ���� ��� �ð�
        void GetEnterWaitTime(OUT TCHAR* message_string);
        void set_reservation_enter_time(DWORD tick_time) { reservation_enter_time_ = tick_time; }
        DWORD reservation_enter_time() { return reservation_enter_time_; }

        // ���� �ɹ� ��
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        void set_reservation_member_count(WORD member_count) { reservation_member_count_ = member_count; }
        WORD reservation_member_count() { return reservation_member_count_; }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

        // ���� ���� �ð�
        void StartBattleGround(); // ���� ����
        void EndBattleGround(); // ���� ����
        DWORD GetBattleTime(); // ���� �ҿ� �ð�
        BATTLE_GROUND_PROGRESS battle_ground_progress() { return battle_ground_progress_; }

        // ���� ���� �ð�
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
        BattleGroundPlayerState player_state_; // �÷��̾� ����
        BattleGroundMemberContainer member_info_vector_;  // ���� ����� ������
        BATTLE_GROUND_TEAM_SCORE_INFO team_score_info[MAX_TEAM_COUNT]; // ����

        BATTLE_GROUND_PROGRESS battle_ground_progress_; // ���� ���� ����
        DWORD battle_ground_start_tick_;  // ���� ���� �ð�
        DWORD battle_ground_end_tick_;  // ���� ���� �ð�
#else
    private:
        DWORD select_mapcode_;  // ���õ� ���ڵ�
        bool is_on_reservation_; // ���� ������
        DWORD reservation_enter_time_; // ���� ��� ���� �ð�
        WORD reservation_member_count_; // ���� ����� �ο�

        BattleGroundPlayerState player_state_; // �÷��̾� ����
        BattleGroundMemberContainer member_info_vector_;  // ���� ����� ������
        BATTLE_GROUND_TEAM_SCORE_INFO team_score_info[MAX_TEAM_COUNT]; // ����

        BATTLE_GROUND_PROGRESS battle_ground_progress_; // ���� ���� ����
        DWORD battle_ground_start_tick_;  // ���� ���� �ð�
        DWORD battle_ground_end_tick_;  // ���� ���� �ð�

        JoinBattleGroundState battle_ground_open_type_; // ���� ���� Ÿ��
        DWORD battle_ground_open_time_;  // ���� ���� ���� �ð�
        DWORD battle_ground_open_recv_time_;   // ��Ŷ ���� �ð�


        // ���� ���� ���
    private:
        HANDLE mutex_handle;
    public:
        BOOL CheckMutexHandle();
        void ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    };
    BattleGroundData& GetBattleGroundData();

    //-----------------------------------------------------------------------------------
    //  ���� ���� ���� Data
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
    //  ��� ���� Data
    //-----------------------------------------------------------------------------------
    typedef BattleGroundPlayerState GoldRushPlayerState;
    typedef JoinBattleGroundState   JoinGoldRushState;
    class GoldRushData : public ChaosZoneData
    {
    public:
        enum { MAX_TEAM_COUNT = 2 };

        enum GOLD_RUSH_PROGRESS
        {
            PROGRESS_ENTER_ZONE, // �� ����
            PROGRESS_PLAYING,
            PROGRESS_END,
        };
    
        typedef STLX_VECTOR<GOLDRUSH_TEAM_MEMBER_INFO> GoldRushMemberContainer;

    public:
        GoldRushData();
        ~GoldRushData();

        GoldRushPlayerState player_state() { return player_state_; }
        void set_player_state(GoldRushPlayerState state) { player_state_ = state; }        

        // ��巯�� �ɹ� ����
        GoldRushMemberContainer& member_info_vector() { return member_info_vector_; }
        void SetGoldRushMemberInfo(BYTE member_count, GOLDRUSH_TEAM_MEMBER_INFO* member_info);
        void AddGoldRushMemberInfo(GOLDRUSH_TEAM_MEMBER_INFO* member_info);
        WORD GetGoldRushTeamForPlayerName(const TCHAR* player_name);
        WORD get_gold_grade(const TCHAR* character_name);

        void set_team_score_info(const GOLDRUSH_TEAM_SCORE_INFO* const score_info);
        void set_team_score(ChaosZoneTeam team_key, WORD total_gold_point);
        GOLDRUSH_TEAM_SCORE_INFO* const GetTeamScore(ChaosZoneTeam team);

        void Clear();
        // ���� ��� �ð�
        void GetEnterWaitTime(OUT TCHAR* message_string);
        void set_reservation_enter_time(DWORD tick_time) { reservation_enter_time_ = tick_time; }
        DWORD reservation_enter_time() { return reservation_enter_time_; }

        // ��巯�� ���� �ð�
        void StartGoldRush(); // ��巯�� ����
        void EndGoldRush(); // ��巯�� ����
        DWORD GetGoldRushTime(); // ��巯�� �ҿ� �ð�
        GOLD_RUSH_PROGRESS gold_rush_progress() { return gold_rush_progress_; }

    private:
        GoldRushPlayerState player_state_; // �÷��̾� ����
        GoldRushMemberContainer member_info_vector_;  // ���� ����� ������
        GOLDRUSH_TEAM_SCORE_INFO team_score_info[MAX_TEAM_COUNT]; // ���� -> ���ο� �ڷ����� �ʿ��ϴ�.

        GOLD_RUSH_PROGRESS gold_rush_progress_; // ��巯�� ���� ����
        DWORD gold_rush_start_tick_;  // ��巯�� ���� �ð�
        DWORD gold_rush_end_tick_;  // ��巯�� ���� �ð�
    };
    GoldRushData& GetGoldRushData();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}; //namespace ChaosSystem

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND