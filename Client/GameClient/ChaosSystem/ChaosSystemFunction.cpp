#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "ChaosSystem/uiChaosZoneBattle.h"
#include "ObjectManager.h"
#include "GlobalFunc.h"
#include "Map.h"
#include "BattleGroundFlagInfoParser.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/uiChaosZoneGoldRush.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

namespace ChaosSystem
{
    namespace Function
    {
        // common functions
        //----------------------------------------------------------------------------
        // 예약 패킷 전송
        void SendReservationPacket()
        {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            // 전장 활성화
            if (ChaosSystem::GetBattleGroundData().is_on_reservation() == false && 
                ChaosSystem::GetGoldRushData().is_on_reservation() == false)
            {
                if (ChaosSystem::ChaosZoneData::CheckMutexHandle() == TRUE)
                {
                    if (!GENERALPARAM->IsNet())
                    {
                        DWORD mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                        ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_SYN(mapcode);
                    }

                    {
                        // 맵코드를 기준으로 kind에 따라서 다르게 입장예약 패킷을 보낼 수 있도록 한다.                        
                        // 현재 열려져 있는 카오스존에 한해서 mapcode를 뽑아서 전송
                        uiChaosSystemMan* chaos_system_manager_ptr = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);

                        if (chaos_system_manager_ptr)
                        {
                            DWORD dialog_id = chaos_system_manager_ptr->get_pending_dialog_id();
                            if (dialog_id == 0)
                            {
                                // 잘못됬다.
                                assert(!"열려있지 않다.");
                                return;
                            }

                            if (dialog_id == IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE) // 전장
                            {
                                DWORD mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                                ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_SYN(mapcode);
                            }
                            else if (dialog_id == IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH) // 골드 러쉬
                            {
                                DWORD mapcode = ChaosSystem::GetGoldRushData().select_mapcode();
                                ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_RESERVATION_SYN(mapcode);
                            }
                        }
                    }
                }
                else
                {
                    // 2746 다중 접속 에러
                    TCHAR message[INTERFACE_STRING_LENGTH ] = {0,};
                    g_InterfaceManager.GetInterfaceString(2746 , message ,INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
                }
            }
            else
            {
                // 어느 전장이 현재 예약이 되어있는지 판별해서 예약취소 패킷을 전송
                if (ChaosSystem::GetBattleGroundData().is_on_reservation() == true) // 전장
                {
                    ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN();
                }
                else if (ChaosSystem::GetGoldRushData().is_on_reservation() == true) // 골드러쉬
                {
                    ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_RESERVATION_CANCEL_SYN();
                }                
            }            
#else // GOLDRUSH #else
#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
            // 전장 활성화
            if (ChaosSystem::GetBattleGroundData().is_on_reservation() == false)
            {
                if (ChaosSystem::GetBattleGroundData().CheckMutexHandle() == TRUE)
                {
                    if (!GENERALPARAM->IsNet())
                    {
                        DWORD mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                        ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_SYN(mapcode);
                    }

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
                    if (ChaosSystem::GetBattleGroundData().battle_ground_open_type() == kJoinStateNonadmission)
                    {
                        // 입장 시간이 아닙니다.
                        TCHAR message[INTERFACE_STRING_LENGTH ] = {0,};
                        g_InterfaceManager.GetInterfaceString(2753 , message ,INTERFACE_STRING_LENGTH);
                        GlobalFunc::SysMessageBox(message);
                    }
                    else
#endif //_NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
                    {
                        DWORD mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                        ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_SYN(mapcode);
                    }
                }
                else
                {
                    // 2746 다중 접속 에러
                    TCHAR message[INTERFACE_STRING_LENGTH ] = {0,};
                    g_InterfaceManager.GetInterfaceString(2746 , message ,INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
                }
            }
            else
            {
                ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN();
            }
#else
            // 전장 비활성화
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        };
        //----------------------------------------------------------------------------
        // 전장 예약 버튼 세팅
        void SetBattleGroundReservation(bool is_reservation) 
        {
            ChaosSystem::GetBattleGroundData().set_is_on_reservation(is_reservation);

            uiChaosZoneBattle* chaos_zone_battle = GET_CAST_DIALOG(uiChaosZoneBattle, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE);
            if (chaos_zone_battle)
            {
                chaos_zone_battle->SetReservationButton(is_reservation);
            }

            ChaosSystem::GetBattleGroundData().set_reservation_enter_time(
                is_reservation ? clock_function::GetTickCount() : 0);
        };
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        //----------------------------------------------------------------------------
        // 골드러쉬 / 전장 예약 버튼 세팅
        void SetGoldRushReservation(const bool& is_reservation)
        {
            ChaosSystem::GetGoldRushData().set_is_on_reservation(is_reservation);

            uiChaosZoneGoldRush* chaos_zone_goldrush_ptr = GET_CAST_DIALOG(uiChaosZoneGoldRush, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH);
            if (chaos_zone_goldrush_ptr)
            {
                chaos_zone_goldrush_ptr->SetReservationButton(is_reservation);
            }            

            ChaosSystem::GetGoldRushData().set_reservation_enter_time(
                is_reservation ? clock_function::GetTickCount() : 0);
        };
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        //----------------------------------------------------------------------------
        // 전장 팀 갱신
        void RefreshBattleGroundTeam()
        {
            ChaosSystem::BattleGroundData::BattleGroundMemberContainer::iterator itr =
                ChaosSystem::GetBattleGroundData().member_info_vector().begin();
            ChaosSystem::BattleGroundData::BattleGroundMemberContainer::iterator end_itr =
                ChaosSystem::GetBattleGroundData().member_info_vector().end();
            for ( ; itr != end_itr; ++itr)
            {
                Player* player = static_cast<Player*>(g_ObjectManager.GetObject((*itr).character_name));
                if (player != NULL)
                {
                    player->SetTeam((*itr).team_key);
                }
            }
        }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        // 골드러쉬 팀 갱신
        void RefreshGoldRushTeam()
        {
            ChaosSystem::GoldRushData::GoldRushMemberContainer::iterator itr =
                ChaosSystem::GetGoldRushData().member_info_vector().begin();
            ChaosSystem::GoldRushData::GoldRushMemberContainer::iterator end_itr =
                ChaosSystem::GetGoldRushData().member_info_vector().end();
            for ( ; itr != end_itr; ++itr)
            {
                Player* player = static_cast<Player*>(g_ObjectManager.GetObject((*itr).character_name));
                if (player != NULL)
                {
                    player->SetTeam((*itr).team_key);                    
                }
            }
        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        //----------------------------------------------------------------------------
        // 점령 중 이펙트
        void CreateBattleGroundCollectEffect(Player* player, MapObject* map_object)
        {
            // 거점 오브젝트를 채집 하는 중의 이펙트 출력
            // 거점 오브젝트에 이펙트를 입히는게 아니라, 애니메이션을 변경해 주는 방식이여서
            // 채집하는 상황에 따라 거점 오브젝트와 거점 애니메이션을 찾아 바꿔줘야함
            if (GlobalFunc::IsBattleGroundScene() == FALSE)
            {
                return;
            }

            BattleGroundFlagInfo* flag_info =
                BattleGroundFlagInfoParser::Instance()->GetBattleGroundFlagInfo(g_pMap->GetCurrentFieldID(),
                player->GetCollectID());

            if (flag_info == NULL) // flag_info 가 NULL 이 아니면 거점오브젝트로 판단
            {
                return;
            }
            
            if (player->GetTeam() == INVALID_TEAM)
            {
                // 팀 설정이 안되있으면 전장인포에서 가져와서 저장한다.
                WORD team_code = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(player->GetName());
                if (team_code != 0)
                {
                    player->SetTeam(team_code);
                }
            }

            WzID object_id = map_object->GetBasePart()->GetWzUnit()->GetType();
            if (object_id == battle_ground_object_type_id[battle_ground_object_red_waypoint]) // 빨 거점이고
            {
                if (player->GetTeam() == kBlue) // 파 팀이 땡겼으면
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_red_waypoint_redfield_blueconquering); // 빨 오오라 + 파 점령중 이펙트
                }
                else
                {
#ifdef _DEV_VER
                    assert(!"자기편 거점을 당겼다!? 확인필요");
#endif //_DEV_VER
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_blue_waypoint]) // 파 거점이고
            {
                if (player->GetTeam() == kRed) // 빨 팀이 땡겼으면
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_blue_waypoint_bluefield_redconquering); // 빨 오오라 + 파 점령중 이펙트
                }
                else
                {
#ifdef _DEV_VER
                    assert(!"자기편 거점을 당겼다!? 확인필요");
#endif //_DEV_VER
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_normal_waypoint]) // 흰 거점이고
            {
                WzID animation_id = map_object->GetCurrentAnimation();
                if (player->GetTeam() == kRed) // 빨 팀이 땡겼으면
                {
                    if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield])  // 흰색 오오라면
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield_redconquering); // 흰 오오라 + 빨 점령중 이펙트
                    }
                    else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield]) ||
                        (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_blueconquered])) // 파란 오오라면
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_bluefield_redconquering); // 파 오오라 + 빨 점령중 이펙트
                    }
                }
                else if (player->GetTeam() == kBlue) // 파 팀이 땡겼으면
                {
                    if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield]) // 흰색 오오라면
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield_blueconquering); // 흰 오오라 + 파 점령중 이펙트
                    }
                    else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield]) ||
                        (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_redconquered])) // 빨간 오오라면
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_redfield_blueconquering); // 빨 오오라 + 파 점령중 이펙트
                    }
                }
            }
        }
        void CreateBattleGroundConqeredEffect(Player* player, MapObject* map_object)
        {
            // 거점 오브젝트의 채집 이펙트 제거
            if (GlobalFunc::IsBattleGroundScene() == FALSE)
            {
                return;
            }

            WzID object_id = map_object->GetBasePart()->GetWzUnit()->GetType();
            WzID animation_id = map_object->GetCurrentAnimation();
            if (object_id == battle_ground_object_type_id[battle_ground_object_red_waypoint]) // 빨 거점이고
            {
                if (animation_id == battle_ground_waypoint_animation_id[battle_ground_red_waypoint_redfield_blueconquering]) // 빨 오오라 + 파 점령중 이펙트면
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_red_waypoint_redfield); // 빨 오오라
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_blue_waypoint]) // 파 거점이고
            {
                if (animation_id == battle_ground_waypoint_animation_id[battle_ground_blue_waypoint_bluefield_redconquering]) // 파 오오라 + 빨 점령중 이펙트면
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_blue_waypoint_bluefield); // 파 오오라
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_normal_waypoint]) // 흰 거점이고
            {
                if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_redconquering]) || // 흰 오오라 + 빨/파 점령중 이펙트
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_blueconquering]))
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield); // 흰 오오라
                }
                else if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_blueconquering])  // 빨 오오라 + 파 점령중 이펙트
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_redfield); // 빨 오오라
                }
                else if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_redconquering])  // 파 오오라 + 빨 점령중 이펙트
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_bluefield); // 파 오오라
                }
            }
        }

        void InitStrongPointAnimaion(const MAPCODE map_code)
        {
            BATTLE_GROUND_FLAG_INFO_MAP flag_info_hash_map =
                BattleGroundFlagInfoParser::Instance()->GetBattleGroundFlagInfoHashMap();

            BATTLE_GROUND_FLAG_INFO_MAP_ITER itr;
            std::pair<BATTLE_GROUND_FLAG_INFO_MAP_ITER, BATTLE_GROUND_FLAG_INFO_MAP_ITER> range;

            range = flag_info_hash_map.equal_range(map_code);
            for (itr = range.first; itr != range.second; ++itr)
            {
                BattleGroundFlagInfo* info_map = itr->second;
                if (info_map)
                {
                    MapObject* map_object = static_cast<MapObject*>(g_ObjectManager.GetObject(info_map->strong_point_key));
                    if (map_object)
                    {
                        //map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_normal_waypoint); // 흰 거점
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield); // 흰 오오라
                    }
                }
            }
        }

        int GetStrongPointRadarColor(MapObject* map_object)
        {
            /*
            eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL = 111,     // 전장 거점 아이콘(흰)
            eRADAR_BATTLE_GROUND_STRONGPOINT_HALFRED = 112,    // 전장 거점 아이콘(흐린빨)
            eRADAR_BATTLE_GROUND_STRONGPOINT_RED = 113,        // 전장 거점 아이콘(빨)
            eRADAR_BATTLE_GROUND_STRONGPOINT_HALFBLUE = 114,   // 전장 거점 아이콘(흐린파)
            eRADAR_BATTLE_GROUND_STRONGPOINT_BLUE = 115,       // 전장 거점 아이콘(파)
            */
            BYTE radar_color_index = 0;

            WzID object_id = map_object->GetBasePart()->GetWzUnit()->GetType();
            WzID animation_id = map_object->GetCurrentAnimation();

            if (object_id == battle_ground_object_type_id[battle_ground_object_normal_waypoint]) // 흰 거점이고
            {
                if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield]) || // 흰 오오들이면
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_blueconquering]) ||
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_redconquering]))
                {
                    radar_color_index = 0;
                }
                else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield]) || // 빨 오오라들이면
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_blueconquering]) ||
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_redconquered]))
                {
                    radar_color_index = 1;
                }
                else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield]) || // 파 오오라들이면
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_redconquering]) ||
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_blueconquered]))
                {
                    radar_color_index = 3;
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_red_waypoint]) // 빨 거점이면
            {
                radar_color_index = 2;
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_blue_waypoint]) // 파 거점이고
            {
                radar_color_index = 4;
            }

            return radar_color_index;
        }

        void OutputResultStringBattleGround(DWORD result_code) // 전장 에러메세지 출력
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

            switch (result_code)
            {
            case RC::RC_BATTLE_GROUND_RESERVATION_FAIL:
                {
                    // 2705 전장 입장 예약을 할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2705, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_RESERVATION_ALREADY:
                {
                    // 2706 이미 전장 입장 예약 상태입니다.
                    g_InterfaceManager.GetInterfaceString(2706, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL:
                {
                    // 2707 전장에 입장하기 위한 레벨 조건이 맞지 않습니다.
                    g_InterfaceManager.GetInterfaceString(2707, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_ITEM_SCORE:
                {
                    // 2708 전장에 입장하기 위한 장비평점 조건이 맞지 않습니다.
                    g_InterfaceManager.GetInterfaceString(2708, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_EXIST_STATE_FUGITIVE_BUF:
                {
                    // 2710 도망자 상태에서는 전장 입장 예약을 할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2710, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_PLAYER_IS_CHAO_STATE:
                {
                    // 2711 카오 상태에서는 전장 입장 예약을 할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2711, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_ALREADY_EXIST_PARTY:
                {
                    // 2712 파티상태에서는 전장 입장 예약을 할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2712, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_REWARD_WINDOW_OPEN:
                {
                    // 2748 미션 클리어 보상이나, 전장 보상을 받지 않은 상태에서는 전장 예약이 불가능 합니다.
                    g_InterfaceManager.GetInterfaceString(2748, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_NONADMISSION_TIME:
                {
                    // 2753 입장 가능 시간이 아닙니다.
                    g_InterfaceManager.GetInterfaceString(2753, message, INTERFACE_STRING_LENGTH);
                }
                break;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL:
                {
                    // 2784 투표를 할 수 없는 상황이거나, 대상이 올바르지 않습니다.
                    g_InterfaceManager.GetInterfaceString(2784, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SAME_TEAM:
                {
                    // 2784 투표를 할 수 없는 상황이거나, 대상이 올바르지 않습니다.
                    g_InterfaceManager.GetInterfaceString(2784, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_VICTORYPOINT:
                {
                    // 2779 지금은 추방투표를 발의 할 수 없습니다. 추방투표는 팀에 관계없이 1000점이 득점되는 순간부터 가능합니다.
                    g_InterfaceManager.GetInterfaceString(2779, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT:
                {
                    // 2778 추방투표에 필요한 최소인원이 부족하여 추방투표를 진행 할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2778, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_LIMIT_TIME:
                {
                    // 2777 추방투표 종료 후 1분간은 해당기능을 사용 할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2777, message, INTERFACE_STRING_LENGTH);
                }
                break;
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
            case RC::RC_BATTLE_GROUND_RESERVATION_NOT_AVAILABLE_CHANNEL:
                {
                    // 70676 점령전 채널에서만 예약 가능
                    g_InterfaceManager.GetInterfaceString(70676, message, INTERFACE_STRING_LENGTH);
                }
                break;
#endif //_NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL

            default:
                {
                    Snprintf(message, INTERFACE_STRING_LENGTH-1, "ChaosZone Error(ErrorCode : %d)", result_code);
                }
                break;
            }
            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
        }

        //
        //---------------------------------------------------------------------------
        // 자유전투실 
        //---------------------------------------------------------------------------
        // Result code 메세지 출력
        void OutputResultStringFreePVP(DWORD result_code)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

            switch (result_code)
            {
            case RC::RC_FREE_PVP_CREATE_FAIL:
                {
                    // 2616 방 생성에 실패했습니다.
                    g_InterfaceManager.GetInterfaceString(2616, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_MAX_ROOM:
                {
                    //2615 최대 개설할 수 있는 자유전투실 개수를 초과하였습니다.
                    g_InterfaceManager.GetInterfaceString(2615, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_ZONE:
            case RC::RC_FREE_PVP_CREATE_FAIL_TRANSACTION:
            case RC::RC_FREE_PVP_CREATE_FAIL_WAIT_MOVE_ZONE:
                {
                    // 2618 현재 방 생성을 할 수 없는 상태입니다.
                    g_InterfaceManager.GetInterfaceString(2618, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_INACTIVE:
            case RC::RC_FREE_PVP_JOIN_FAIL_INACTIVE:
                {
                    // 2614 자유전투실은 현재 이용할 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(2614, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_JOIN_FAIL:
                {
                    // 2617 방 입장에 실패했습니다.
                    g_InterfaceManager.GetInterfaceString(2617, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM:
                {
                    // 2619 방이 존재하지 않습니다.
                    g_InterfaceManager.GetInterfaceString(2619, message, INTERFACE_STRING_LENGTH);
                }
            case RC::RC_FREE_PVP_JOIN_FAIL_MAX_USER:
                {
                    // 2477 입장 인원이 초과 되었습니다.
                    g_InterfaceManager.GetInterfaceString(2477, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_JOIN_FAIL_DIFFER_PASSWORD:
                {
                    // 2494 비밀 번호를 잘못 입력하였습니다.
                    g_InterfaceManager.GetInterfaceString(2494, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_ALREADY_BATTLE_GROUND_RESERVATION:
                {
                    // 2703 전장 예약 중에는 해당 동작을 수행 할 수 없습니다.\n전장 예약을 취소하신 후 다시 시도해 주십시오
                    g_InterfaceManager.GetInterfaceString(2703, message, INTERFACE_STRING_LENGTH);
                }
                break;
            default:
                {
                    // 알수 없는 에러코드
                    Snprintf(message, INTERFACE_STRING_LENGTH, "ChaosZone PVP - Invalid Error Code = %d", result_code);
                }
                break;
            }

            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
            //GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, message);
        }

#ifdef _NA_008334_20150608_SONNENSCHEIN
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        // 소넨샤인 Result code 메세지 출력

        void OutputResultStringSonnenSchein(DWORD result_code)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

            switch (result_code)
            {
            case RC::RC_SONNENSCHEIN_FAILED:
            case RC::RC_SONNENSCHEIN_ROOM_CREATE_FAILED:
            case RC::RC_SONNENSCHEIN_ROOM_NO_EXIST:
            case RC::RC_SONNENSCHEIN_NOTEXIST_MISSIONINFO:
            case RC::RC_SONNENSCHEIN_ALREADY_ENTERED:
                {
                    // 2617 방 입장에 실패 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2617, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_STATE_NOT_READY:
            case RC::RC_SONNENSCHEIN_STATE_END:
                {
                    // 5597 입장 가능 시간이 아닙니다.
                    g_InterfaceManager.GetInterfaceString(5597, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_FAIL_TRANSACTION:
            case RC::RC_SONNENSCHEIN_BATTLEGROUND_RESERVATION_STATE:
                {
                    // 6429 해당 맵의 입장이 불가능 합니다.
                    g_InterfaceManager.GetInterfaceString(6429, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_NOTEXIST_ENTERANCE_ITEM:
            case RC::RC_SONNENSCHEIN_CHECK_ENTRANCE_ITEM_FAILED:
                {
                    // 2495 방 입장에 필요한 아이템이 없습니다.
                    g_InterfaceManager.GetInterfaceString(2495, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_NOT_DOMINATION_CHANNEL:
                {
                    // 3538 현재 채널에서 참여할 수 없습니다. 1채널에서 시도해 주세요.
                    g_InterfaceManager.GetInterfaceString(3538, message, INTERFACE_STRING_LENGTH);
                }
                break;
            default:
                {
                    // 알수 없는 에러코드
                    Snprintf(message, INTERFACE_STRING_LENGTH, "Guild PVE - Invalid Error Code = %d", result_code);
                }
                break;
            }

            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, message);
            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
        }
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        // 길드일일미션 Result code 메세지 출력

        void OutputResultStringOneDaysGuildMission(DWORD result_code)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

            switch (result_code)
            {
            case RC::RC_ONEDAYSGUILDMISSION_FAILED:
            case RC::RC_ONEDAYSGUILDMISSION_ROOM_CREATE_FAILED:
            case RC::RC_ONEDAYSGUILDMISSION_ROOM_NOT_EXIST:
            case RC::RC_ONEDAYSGUILDMISSION_NOTEXIST_MISSIONINFO:
            case RC::RC_ONEDAYSGUILDMISSION_ALREADY_ENTERED:
                {
                    // 2617 방 입장에 실패 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2617, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_STATE_END:
                {
                    // 5597 입장 가능 시간이 아닙니다.
                    g_InterfaceManager.GetInterfaceString(5597, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION:
            case RC::RC_ONEDAYSGUILDMISSION_BATTLEGROUND_RESERVATION_STATE:
                {
                    // 6429 해당 맵의 입장이 불가능 합니다.
                    g_InterfaceManager.GetInterfaceString(6429, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_NOTEXIST_ENTERANCE_ITEM:
            case RC::RC_ONEDAYSGUILDMISSION_CHECK_ENTRANCE_ITEM_FAILED:
                {
                    // 2495 방 입장에 필요한 아이템이 없습니다.
                    g_InterfaceManager.GetInterfaceString(2495, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_NOT_DOMINATION_CHANNEL:
                {
                    // 3538 현재 채널에서 참여할 수 없습니다. 1채널에서 시도해 주세요.
                    g_InterfaceManager.GetInterfaceString(3538, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_NOT_GUILD_USER:
                {
                    // 5165 길드에 가입되어 있지 않습니다.
                    g_InterfaceManager.GetInterfaceString(5165, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_OVER_MAX_LIMIT_ENTERED_CNT:
                {
                    // 3340 참여 제한 인원수가 초과되어 입장하실 수 없습니다.
                    g_InterfaceManager.GetInterfaceString(3340, message, INTERFACE_STRING_LENGTH);
                }
                break;
            default:
                {
                    // 알수 없는 에러코드
                    Snprintf(message, INTERFACE_STRING_LENGTH, "Guild PVE - Invalid Error Code = %d", result_code);
                }
                break;
            }
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, message);
            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
        }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

    };
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND