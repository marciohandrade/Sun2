#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "BattleGroundInfoParser.h"
#include "InterfaceManager.h"
#include "Map.h"
//#include "ObjectManager.h"
//#include "Hero.h"

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "ObjectManager.h"
HANDLE ChaosSystem::ChaosZoneData::mutex_handle = NULL;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

namespace ChaosSystem
{
    //-----------------------------------------------------------------------------------
    //  카오스존 Data
    //-----------------------------------------------------------------------------------
    ChaosZoneData choas_zone_data;
    ChaosZoneData& GetChaosZoneData()
    {
        return choas_zone_data;
    }

    ChaosZoneData::ChaosZoneData()
    {
        is_wait_ = false;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        if (NULL == mutex_handle)
        {
            mutex_handle = CreateMutex(NULL, FALSE, _T("SUN_CHAOSZONE_RESERVATION"));
        }
#endif // 
    }

    ChaosZoneData::~ChaosZoneData()
    {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        if (mutex_handle != NULL)
        {
            CloseHandle(mutex_handle);
            mutex_handle = NULL;
        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    BOOL ChaosZoneData::CheckMutexHandle()
    {
        if (GENERALPARAM->GetBattleGRoundMultiReservation() == 1)
        {
            return TRUE;
        }

        if (mutex_handle == NULL)
        {
            return FALSE;
        }

        DWORD wait_result = WaitForSingleObject(mutex_handle, 100);
        if (wait_result == WAIT_TIMEOUT)
        {
            return FALSE;
        }

        if ((wait_result == WAIT_OBJECT_0) || (wait_result == WAIT_ABANDONED))
        {
            return TRUE;
        }        

        return FALSE;
    }
    void ChaosZoneData::ReleaseMutexHandle()
    {
        if (mutex_handle != NULL)
        {
            ReleaseMutex(mutex_handle);
        }
    }
    void ChaosZoneData::Clear()
    {
        ChaosSystem::ChaosZoneData::ReleaseMutexHandle();
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    //-----------------------------------------------------------------------------------
    //  전장 Data
    //-----------------------------------------------------------------------------------
    BattleGroundData battle_ground_data;
    BattleGroundData& GetBattleGroundData()
    {
        return battle_ground_data;
    }

    BattleGroundData::BattleGroundData()
    {
        team_score_info[0].team_key = kBlue;
        team_score_info[0].strongpoint_count = 0;
        team_score_info[0].victory_point = 0;
        team_score_info[1].team_key = kRed;
        team_score_info[1].strongpoint_count = 0;
        team_score_info[1].victory_point = 0;

        reservation_enter_time_ = 0;
        battle_ground_progress_ = PROGRESS_ENTER_ZONE;
        battle_ground_start_tick_ = 0;
        battle_ground_end_tick_ = 0;
        reservation_member_count_ = 0;

        battle_ground_open_type_ = kNone;
        battle_ground_open_time_ = 0;
        battle_ground_open_recv_time_ = 0;

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        mutex_handle = CreateMutex(NULL, FALSE, _T("SUN_BATTLE_GROUND_RESERVATION"));
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
 }

    BattleGroundData::~BattleGroundData()
    {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        if (mutex_handle != NULL)
        {
            CloseHandle(mutex_handle);
            mutex_handle = NULL;
        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }

    void BattleGroundData::Clear()
    {
        member_info_vector_.clear();
        
        team_score_info[0].team_key = kBlue;
        team_score_info[0].strongpoint_count = 0;
        team_score_info[0].victory_point = 0;
        team_score_info[1].team_key = kRed;
        team_score_info[1].strongpoint_count = 0;
        team_score_info[1].victory_point = 0;
        
        reservation_enter_time_ = 0;
        battle_ground_progress_ = PROGRESS_ENTER_ZONE;
        battle_ground_start_tick_ = 0;
        battle_ground_end_tick_ = 0;
        reservation_member_count_ = 0;

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
        ChaosSystem::GetBattleGroundData().ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }

    void BattleGroundData::SetBattleGroundMemberInfo(BYTE member_count, BATTLE_GROUND_TEAM_MEMBER_INFO* member_info)
    {
        member_info_vector().clear();

        for (int i = 0; i < member_count; ++i)
        {
            member_info_vector().push_back(*(member_info+i));
        }

        ChaosSystem::Function::RefreshBattleGroundTeam();
    }

    void BattleGroundData::AddBattleGroundMemberInfo(BATTLE_GROUND_TEAM_MEMBER_INFO* member_info)
    {
        BattleGroundMemberContainer::iterator itr = member_info_vector().begin();
        BattleGroundMemberContainer::iterator end_itr = member_info_vector().end();
        for ( ; itr != end_itr; ++itr)
        {
            if (_tcscmp((*itr).character_name, member_info->character_name) == 0)
            {
                return;
            }
        }
        member_info_vector().push_back(*member_info);

    }
    
    WORD BattleGroundData::GetBattleGroundTeamForPlayerName(const TCHAR* player_name)
    {
        BattleGroundMemberContainer::iterator itr = member_info_vector().begin();
        BattleGroundMemberContainer::iterator end_itr = member_info_vector().end();
        for ( ; itr != end_itr; ++itr)
        {
            if (_tcscmp((*itr).character_name, player_name) == 0)
            {
                return (*itr).team_key;
            }
        }
        return 0;
    }

    void BattleGroundData::set_team_score_info(const BATTLE_GROUND_TEAM_SCORE_INFO* const score_info)
    {
        if ((score_info->team_key <= kChaosZoneTeamNone) ||
            (score_info->team_key > kBlue))
        {
            return;
        }
        //BATTLE_GROUND_TEAM_NONE = 0,
        //kBlue = 1,
        //kRed = 2
        team_score_info[score_info->team_key-1].strongpoint_count = score_info->strongpoint_count;
        team_score_info[score_info->team_key-1].victory_point = score_info->victory_point;

    }
    BATTLE_GROUND_TEAM_SCORE_INFO* const BattleGroundData::GetTeamScore(ChaosZoneTeam team)
    {
        if ((team <= kChaosZoneTeamNone) ||
            (team > kBlue))
        {
            return NULL;
        }

        return  &(team_score_info[team-1]);
    }

    void BattleGroundData::GetEnterWaitTime(OUT TCHAR* message_string)
    {
        if (is_on_reservation() == false)
        {
            message_string = NULL;
            return;
        }

        int wait_time = (clock_function::GetTickCount() - reservation_enter_time()) / 1000;
        if (wait_time < 0)
        {
            wait_time = 0;
        }

        //hour = wait_time / 3600;
        wait_time %= 3600;
        int minute = wait_time / 60;
        wait_time %= 60;
        int second = wait_time % 60;

        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR minute_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR second_string[INTERFACE_STRING_LENGTH] = {0,};
        // 2596 소요대기시간:(%s 분 %s 초 경과)
        g_InterfaceManager.GetInterfaceString(2596, temp_string, INTERFACE_STRING_LENGTH);
        _itot(minute, minute_string, 10);
        _itot(second, second_string, 10);
        Snprintf(message_string, INTERFACE_STRING_LENGTH-1, temp_string, minute_string, second_string);
    }

    void BattleGroundData::StartBattleGround()
    {
        // 전장 대기시간 돌입
        battle_ground_progress_ = PROGRESS_PLAYING;
        battle_ground_start_tick_ = clock_function::GetTickCount();
    }
    void BattleGroundData::EndBattleGround()
    {
        const BattleGroundInfo* battle_ground_info =
            BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(g_pMap->GetCurrentFieldID()));
        if (battle_ground_info)
        {
            // 전장 종료 시간
            battle_ground_end_tick_ = clock_function::GetTickCount();
        }

        battle_ground_progress_ = PROGRESS_END;
    }
    DWORD BattleGroundData::GetBattleTime()
    {
        switch (battle_ground_progress_)
        {
        case PROGRESS_ENTER_ZONE:
            {
                return 0;
            }
            break;
        case PROGRESS_PLAYING:
            {
                return (clock_function::GetTickCount() - battle_ground_start_tick_);
            }
            break;
        case PROGRESS_END:
            {
                return (battle_ground_end_tick_ - battle_ground_start_tick_);
            }
            break;
        };

        return 0;
    }

    void BattleGroundData::GetBattleGroundOpenTime(OUT TCHAR* ouput_string)
    {
        if (battle_ground_open_type_ == kNone)
        {
            ouput_string = "\0";
            return;
        }

        int hour = 0;
        int min = 0;
        int sec = 0;
        int check_time = 0;
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

        check_time = (int)(battle_ground_open_time_ + (battle_ground_open_recv_time_ - clock_function::GetTickCount())) / 1000;

        if (check_time < 0)
        {
            ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN();
            ouput_string = "\0";
            return;
        }

        hour = check_time / 3600;
        check_time %= 3600;
        min = check_time / 60;
        check_time %= 60;
        sec = check_time % 60;

        if (battle_ground_open_type_ == kJoinStateNonadmission)
        {
            // 2724 이스라필의 대지:(입장까지 남은 시간 : %d시간 %d분 %d초)
            g_InterfaceManager.GetInterfaceString(2724, temp_string, INTERFACE_STRING_LENGTH);
            Snprintf(ouput_string, INTERFACE_STRING_LENGTH-1, temp_string, hour, min, sec);
        }
        else if (battle_ground_open_type_ == kJoinStateAdmission)
        {
            // 2726 이스라필의 대지:(입장종료까지 남은 시간 : %d시간 %d분 %d초)
            g_InterfaceManager.GetInterfaceString(2726, temp_string, INTERFACE_STRING_LENGTH);
            Snprintf(ouput_string, INTERFACE_STRING_LENGTH-1, temp_string, hour, min, sec);
        }
    }
    void BattleGroundData::UpdateBattleGroundOpenTime()
    {
        if (battle_ground_open_type_ == kNone)
        {
            return;
        }

        int check_time = (int)(battle_ground_open_time_ + (battle_ground_open_recv_time_ - clock_function::GetTickCount())) / 1000;

        if (check_time < 0)
        {
            ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN();
        }
    }

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
    BOOL BattleGroundData::CheckMutexHandle()
    {
        if (GENERALPARAM->GetBattleGRoundMultiReservation() == 1)
        {
            return TRUE;
        }

        if (mutex_handle == NULL)
        {
            return FALSE;
        }

        DWORD wait_result = WaitForSingleObject(mutex_handle, 100);
        if (wait_result == WAIT_TIMEOUT)
        {
            return FALSE;
        }

        if ((wait_result == WAIT_OBJECT_0) || (wait_result == WAIT_ABANDONED))
        {
            return TRUE;
        }

        //if ( wait_result == WAIT_FAILED )
        {
        }

        return FALSE;
    }
    void BattleGroundData::ReleaseMutexHandle()
    {
        if (mutex_handle != NULL)
        {
            ReleaseMutex(mutex_handle);
        }
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    //-----------------------------------------------------------------------------------
    //  자유 전투 지역 Data
    //-----------------------------------------------------------------------------------
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    FreePVPData free_pvp_data;
    FreePVPData& GetFreePVPData()
    {
        return free_pvp_data;
    }
    FreePVPData::FreePVPData()
    {
        free_pvp_room_vector_.clear();
        ZeroMemory(&my_room_info_, sizeof(my_room_info_));
    }
    FreePVPData::~FreePVPData()
    {

    }
    void FreePVPData::Clear()
    {
        free_pvp_room_vector_.clear();
        ZeroMemory(&my_room_info_, sizeof(my_room_info_));
    }
    void FreePVPData::SetFreePVPRoomList(BYTE count_of_room, FreePVPRoomInfo* room_infos)
    {
        if (count_of_room > 100)
        {
            assert(!"count_of_room is over count");
            return;
        }
        free_pvp_room_vector_.clear();

        for (BYTE i = 0; i < count_of_room; ++i)
        {
            free_pvp_room_vector_.push_back(*(room_infos+i));
        }

        std::sort(free_pvp_room_vector_.begin(), free_pvp_room_vector_.end(), SortRoomNumber);
    }
    
    bool SortRoomNumber(const FreePVPRoomInfo &a, const FreePVPRoomInfo &b)
    {
        return  (a.room_key < b.room_key);
    }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    //-----------------------------------------------------------------------------------
    //  골드 러쉬 지역 Data
    //-----------------------------------------------------------------------------------
    GoldRushData gold_rush_data;
    GoldRushData& GetGoldRushData()
    {
        return gold_rush_data;
    }
    GoldRushData::GoldRushData()
    {
        member_info_vector_.clear();        
        
        team_score_info[0].team_gold_point = 0;
        team_score_info[1].team_gold_point = 0;

        reservation_enter_time_ = 0;
        gold_rush_progress_ = PROGRESS_ENTER_ZONE;
        gold_rush_start_tick_ = 0;
        gold_rush_end_tick_ = 0;
        reservation_member_count_ = 0;
    }

    GoldRushData::~GoldRushData()
    {

    }

    void GoldRushData::Clear()
    {
         member_info_vector_.clear();

         team_score_info[0].team_gold_point = 0;
         team_score_info[1].team_gold_point = 0;

        reservation_enter_time_ = 0;
        gold_rush_progress_ = PROGRESS_ENTER_ZONE;
        gold_rush_start_tick_ = 0;
        gold_rush_end_tick_ = 0;
        reservation_member_count_ = 0;
    }

    void GoldRushData::GetEnterWaitTime(OUT TCHAR* message_string)
    {
        if (is_on_reservation() == false)
        {
            message_string = NULL;
            return;
        }

        int wait_time = (clock_function::GetTickCount() - reservation_enter_time()) / 1000;
        if (wait_time < 0)
        {
            wait_time = 0;
        }

        //hour = wait_time / 3600;
        wait_time %= 3600;
        int minute = wait_time / 60;
        wait_time %= 60;
        int second = wait_time % 60;

        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR minute_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR second_string[INTERFACE_STRING_LENGTH] = {0,};
        // 2596 소요대기시간:(%s 분 %s 초 경과)
        g_InterfaceManager.GetInterfaceString(2596, temp_string, INTERFACE_STRING_LENGTH);
        _itot(minute, minute_string, 10);
        _itot(second, second_string, 10);
        Snprintf(message_string, INTERFACE_STRING_LENGTH-1, temp_string, minute_string, second_string);
    }

    void GoldRushData::SetGoldRushMemberInfo(BYTE member_count, GOLDRUSH_TEAM_MEMBER_INFO* member_info)
    {
        member_info_vector().clear();

        for (int i = 0; i < member_count; ++i)
        {
            member_info_vector().push_back(*(member_info+i));
        }
        
        ChaosSystem::Function::RefreshGoldRushTeam();
    }

    void GoldRushData::AddGoldRushMemberInfo(GOLDRUSH_TEAM_MEMBER_INFO* member_info)
    {
        GoldRushMemberContainer::iterator itr = member_info_vector().begin();
        GoldRushMemberContainer::iterator end_itr = member_info_vector().end();
        for ( ; itr != end_itr; ++itr)
        {
            if (_tcscmp((*itr).character_name, member_info->character_name) == 0)
            {
                return;
            }
        }
        member_info_vector().push_back(*member_info);

    }

    WORD GoldRushData::GetGoldRushTeamForPlayerName(const TCHAR* player_name)
    {
        GoldRushMemberContainer::iterator itr = member_info_vector().begin();
        GoldRushMemberContainer::iterator end_itr = member_info_vector().end();
        for ( ; itr != end_itr; ++itr)
        {
            if (_tcscmp((*itr).character_name, player_name) == 0)
            {
                return (*itr).team_key;
            }
        }
        return 0;
    }
    
    void GoldRushData::set_team_score_info(const GOLDRUSH_TEAM_SCORE_INFO* const score_info)
    {
        if ((score_info->team_key <= kChaosZoneTeamNone) ||
            (score_info->team_key > kBlue))
        {
            return;
        }
        //BATTLE_GROUND_TEAM_NONE = 0,
        //kBlue = 1,
        //kRed = 2

        // 골드량 추가
        team_score_info[score_info->team_key-1].team_gold_point = score_info->team_gold_point;

        // 모든 팀정보를 전달한다.
        for (int i = 0; i < score_info->team_member_count; ++i)
        {
            GoldRushMemberContainer::iterator itr = member_info_vector().begin();
            GoldRushMemberContainer::iterator end_itr = member_info_vector().end();
            for ( ; itr != end_itr; ++itr)
            {
                if (_tcscmp((*itr).character_name, score_info->team_player_info[i].character_name) == 0)                
                {
                    (*itr).gold_grade = static_cast<BYTE>(score_info->team_player_info[i].gold_grade);                    
                }
            }
        }
    }


    void GoldRushData::set_team_score( ChaosZoneTeam team_key, WORD total_gold_point )
    {
        if ((team_key <= kChaosZoneTeamNone) ||
            (team_key > kBlue))
        {
            return;
        }

        team_score_info[team_key-1].team_gold_point = total_gold_point;
    }
    
    GOLDRUSH_TEAM_SCORE_INFO* const GoldRushData::GetTeamScore(ChaosZoneTeam team)
    {
        if ((team <= kChaosZoneTeamNone) ||
            (team > kBlue))
        {
            return NULL;
        }

        return  &(team_score_info[team-1]);
    }    

    void GoldRushData::StartGoldRush()
    {
        // 전장 대기시간 돌입
        gold_rush_progress_ = PROGRESS_PLAYING;
        gold_rush_start_tick_ = clock_function::GetTickCount();
    }
    void GoldRushData::EndGoldRush()
    {        
        const BattleGroundInfo* gold_rush_info =
            BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(g_pMap->GetCurrentFieldID()));
        if (gold_rush_info)
        {
            // 골드러쉬 종료 시간
            gold_rush_end_tick_ = clock_function::GetTickCount();
        }        

        gold_rush_progress_ = PROGRESS_END;
    }
    DWORD GoldRushData::GetGoldRushTime()
    {
        switch (gold_rush_progress_)
        {
        case PROGRESS_ENTER_ZONE:
            {
                return 0;
            }
            break;
        case PROGRESS_PLAYING:
            {
                return (clock_function::GetTickCount() - gold_rush_start_tick_);
            }
            break;
        case PROGRESS_END:
            {
                return (gold_rush_end_tick_ - gold_rush_start_tick_);
            }
            break;
        };

        return 0;
    }

    WORD GoldRushData::get_gold_grade( const TCHAR* character_name )
    {
        GoldRushMemberContainer::iterator itr = member_info_vector().begin();
        GoldRushMemberContainer::iterator end_itr = member_info_vector().end();
        for ( ; itr != end_itr; ++itr)
        {
            if (_tcscmp((*itr).character_name, character_name) == 0)                
            {
                return ((*itr).gold_grade);
            }
        }
        return 0;
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}; //namespace ChaosSystem

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND