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
        // ���� ��Ŷ ����
        void SendReservationPacket()
        {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            // ���� Ȱ��ȭ
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
                        // ���ڵ带 �������� kind�� ���� �ٸ��� ���忹�� ��Ŷ�� ���� �� �ֵ��� �Ѵ�.                        
                        // ���� ������ �ִ� ī�������� ���ؼ� mapcode�� �̾Ƽ� ����
                        uiChaosSystemMan* chaos_system_manager_ptr = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);

                        if (chaos_system_manager_ptr)
                        {
                            DWORD dialog_id = chaos_system_manager_ptr->get_pending_dialog_id();
                            if (dialog_id == 0)
                            {
                                // �߸����.
                                assert(!"�������� �ʴ�.");
                                return;
                            }

                            if (dialog_id == IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE) // ����
                            {
                                DWORD mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                                ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_SYN(mapcode);
                            }
                            else if (dialog_id == IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH) // ��� ����
                            {
                                DWORD mapcode = ChaosSystem::GetGoldRushData().select_mapcode();
                                ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_RESERVATION_SYN(mapcode);
                            }
                        }
                    }
                }
                else
                {
                    // 2746 ���� ���� ����
                    TCHAR message[INTERFACE_STRING_LENGTH ] = {0,};
                    g_InterfaceManager.GetInterfaceString(2746 , message ,INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);
                }
            }
            else
            {
                // ��� ������ ���� ������ �Ǿ��ִ��� �Ǻ��ؼ� ������� ��Ŷ�� ����
                if (ChaosSystem::GetBattleGroundData().is_on_reservation() == true) // ����
                {
                    ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN();
                }
                else if (ChaosSystem::GetGoldRushData().is_on_reservation() == true) // ��巯��
                {
                    ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_RESERVATION_CANCEL_SYN();
                }                
            }            
#else // GOLDRUSH #else
#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
            // ���� Ȱ��ȭ
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
                        // ���� �ð��� �ƴմϴ�.
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
                    // 2746 ���� ���� ����
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
            // ���� ��Ȱ��ȭ
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        };
        //----------------------------------------------------------------------------
        // ���� ���� ��ư ����
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
        // ��巯�� / ���� ���� ��ư ����
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
        // ���� �� ����
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
        // ��巯�� �� ����
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
        // ���� �� ����Ʈ
        void CreateBattleGroundCollectEffect(Player* player, MapObject* map_object)
        {
            // ���� ������Ʈ�� ä�� �ϴ� ���� ����Ʈ ���
            // ���� ������Ʈ�� ����Ʈ�� �����°� �ƴ϶�, �ִϸ��̼��� ������ �ִ� ����̿���
            // ä���ϴ� ��Ȳ�� ���� ���� ������Ʈ�� ���� �ִϸ��̼��� ã�� �ٲ������
            if (GlobalFunc::IsBattleGroundScene() == FALSE)
            {
                return;
            }

            BattleGroundFlagInfo* flag_info =
                BattleGroundFlagInfoParser::Instance()->GetBattleGroundFlagInfo(g_pMap->GetCurrentFieldID(),
                player->GetCollectID());

            if (flag_info == NULL) // flag_info �� NULL �� �ƴϸ� ����������Ʈ�� �Ǵ�
            {
                return;
            }
            
            if (player->GetTeam() == INVALID_TEAM)
            {
                // �� ������ �ȵ������� ������������ �����ͼ� �����Ѵ�.
                WORD team_code = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(player->GetName());
                if (team_code != 0)
                {
                    player->SetTeam(team_code);
                }
            }

            WzID object_id = map_object->GetBasePart()->GetWzUnit()->GetType();
            if (object_id == battle_ground_object_type_id[battle_ground_object_red_waypoint]) // �� �����̰�
            {
                if (player->GetTeam() == kBlue) // �� ���� ��������
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_red_waypoint_redfield_blueconquering); // �� ������ + �� ������ ����Ʈ
                }
                else
                {
#ifdef _DEV_VER
                    assert(!"�ڱ��� ������ ����!? Ȯ���ʿ�");
#endif //_DEV_VER
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_blue_waypoint]) // �� �����̰�
            {
                if (player->GetTeam() == kRed) // �� ���� ��������
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_blue_waypoint_bluefield_redconquering); // �� ������ + �� ������ ����Ʈ
                }
                else
                {
#ifdef _DEV_VER
                    assert(!"�ڱ��� ������ ����!? Ȯ���ʿ�");
#endif //_DEV_VER
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_normal_waypoint]) // �� �����̰�
            {
                WzID animation_id = map_object->GetCurrentAnimation();
                if (player->GetTeam() == kRed) // �� ���� ��������
                {
                    if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield])  // ��� �������
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield_redconquering); // �� ������ + �� ������ ����Ʈ
                    }
                    else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield]) ||
                        (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_blueconquered])) // �Ķ� �������
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_bluefield_redconquering); // �� ������ + �� ������ ����Ʈ
                    }
                }
                else if (player->GetTeam() == kBlue) // �� ���� ��������
                {
                    if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield]) // ��� �������
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield_blueconquering); // �� ������ + �� ������ ����Ʈ
                    }
                    else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield]) ||
                        (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_redconquered])) // ���� �������
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_redfield_blueconquering); // �� ������ + �� ������ ����Ʈ
                    }
                }
            }
        }
        void CreateBattleGroundConqeredEffect(Player* player, MapObject* map_object)
        {
            // ���� ������Ʈ�� ä�� ����Ʈ ����
            if (GlobalFunc::IsBattleGroundScene() == FALSE)
            {
                return;
            }

            WzID object_id = map_object->GetBasePart()->GetWzUnit()->GetType();
            WzID animation_id = map_object->GetCurrentAnimation();
            if (object_id == battle_ground_object_type_id[battle_ground_object_red_waypoint]) // �� �����̰�
            {
                if (animation_id == battle_ground_waypoint_animation_id[battle_ground_red_waypoint_redfield_blueconquering]) // �� ������ + �� ������ ����Ʈ��
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_red_waypoint_redfield); // �� ������
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_blue_waypoint]) // �� �����̰�
            {
                if (animation_id == battle_ground_waypoint_animation_id[battle_ground_blue_waypoint_bluefield_redconquering]) // �� ������ + �� ������ ����Ʈ��
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_blue_waypoint_bluefield); // �� ������
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_normal_waypoint]) // �� �����̰�
            {
                if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_redconquering]) || // �� ������ + ��/�� ������ ����Ʈ
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_blueconquering]))
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield); // �� ������
                }
                else if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_blueconquering])  // �� ������ + �� ������ ����Ʈ
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_redfield); // �� ������
                }
                else if (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_redconquering])  // �� ������ + �� ������ ����Ʈ
                {
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_bluefield); // �� ������
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
                        //map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_normal_waypoint); // �� ����
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield); // �� ������
                    }
                }
            }
        }

        int GetStrongPointRadarColor(MapObject* map_object)
        {
            /*
            eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL = 111,     // ���� ���� ������(��)
            eRADAR_BATTLE_GROUND_STRONGPOINT_HALFRED = 112,    // ���� ���� ������(�帰��)
            eRADAR_BATTLE_GROUND_STRONGPOINT_RED = 113,        // ���� ���� ������(��)
            eRADAR_BATTLE_GROUND_STRONGPOINT_HALFBLUE = 114,   // ���� ���� ������(�帰��)
            eRADAR_BATTLE_GROUND_STRONGPOINT_BLUE = 115,       // ���� ���� ������(��)
            */
            BYTE radar_color_index = 0;

            WzID object_id = map_object->GetBasePart()->GetWzUnit()->GetType();
            WzID animation_id = map_object->GetCurrentAnimation();

            if (object_id == battle_ground_object_type_id[battle_ground_object_normal_waypoint]) // �� �����̰�
            {
                if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield]) || // �� �������̸�
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_blueconquering]) ||
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_whitefield_redconquering]))
                {
                    radar_color_index = 0;
                }
                else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield]) || // �� ��������̸�
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_blueconquering]) ||
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_redfield_redconquered]))
                {
                    radar_color_index = 1;
                }
                else if ((animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield]) || // �� ��������̸�
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_redconquering]) ||
                    (animation_id == battle_ground_waypoint_animation_id[battle_ground_normal_waypoint_bluefield_blueconquered]))
                {
                    radar_color_index = 3;
                }
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_red_waypoint]) // �� �����̸�
            {
                radar_color_index = 2;
            }
            else if (object_id == battle_ground_object_type_id[battle_ground_object_blue_waypoint]) // �� �����̰�
            {
                radar_color_index = 4;
            }

            return radar_color_index;
        }

        void OutputResultStringBattleGround(DWORD result_code) // ���� �����޼��� ���
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

            switch (result_code)
            {
            case RC::RC_BATTLE_GROUND_RESERVATION_FAIL:
                {
                    // 2705 ���� ���� ������ �� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2705, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_RESERVATION_ALREADY:
                {
                    // 2706 �̹� ���� ���� ���� �����Դϴ�.
                    g_InterfaceManager.GetInterfaceString(2706, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL:
                {
                    // 2707 ���忡 �����ϱ� ���� ���� ������ ���� �ʽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2707, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_ITEM_SCORE:
                {
                    // 2708 ���忡 �����ϱ� ���� ������� ������ ���� �ʽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2708, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_EXIST_STATE_FUGITIVE_BUF:
                {
                    // 2710 ������ ���¿����� ���� ���� ������ �� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2710, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_PLAYER_IS_CHAO_STATE:
                {
                    // 2711 ī�� ���¿����� ���� ���� ������ �� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2711, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_ALREADY_EXIST_PARTY:
                {
                    // 2712 ��Ƽ���¿����� ���� ���� ������ �� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2712, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_REWARD_WINDOW_OPEN:
                {
                    // 2748 �̼� Ŭ���� �����̳�, ���� ������ ���� ���� ���¿����� ���� ������ �Ұ��� �մϴ�.
                    g_InterfaceManager.GetInterfaceString(2748, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_NONADMISSION_TIME:
                {
                    // 2753 ���� ���� �ð��� �ƴմϴ�.
                    g_InterfaceManager.GetInterfaceString(2753, message, INTERFACE_STRING_LENGTH);
                }
                break;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL:
                {
                    // 2784 ��ǥ�� �� �� ���� ��Ȳ�̰ų�, ����� �ùٸ��� �ʽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2784, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SAME_TEAM:
                {
                    // 2784 ��ǥ�� �� �� ���� ��Ȳ�̰ų�, ����� �ùٸ��� �ʽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2784, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_VICTORYPOINT:
                {
                    // 2779 ������ �߹���ǥ�� ���� �� �� �����ϴ�. �߹���ǥ�� ���� ������� 1000���� �����Ǵ� �������� �����մϴ�.
                    g_InterfaceManager.GetInterfaceString(2779, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT:
                {
                    // 2778 �߹���ǥ�� �ʿ��� �ּ��ο��� �����Ͽ� �߹���ǥ�� ���� �� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2778, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_LIMIT_TIME:
                {
                    // 2777 �߹���ǥ ���� �� 1�а��� �ش����� ��� �� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2777, message, INTERFACE_STRING_LENGTH);
                }
                break;
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
            case RC::RC_BATTLE_GROUND_RESERVATION_NOT_AVAILABLE_CHANNEL:
                {
                    // 70676 ������ ä�ο����� ���� ����
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
        // ���������� 
        //---------------------------------------------------------------------------
        // Result code �޼��� ���
        void OutputResultStringFreePVP(DWORD result_code)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

            switch (result_code)
            {
            case RC::RC_FREE_PVP_CREATE_FAIL:
                {
                    // 2616 �� ������ �����߽��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2616, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_MAX_ROOM:
                {
                    //2615 �ִ� ������ �� �ִ� ���������� ������ �ʰ��Ͽ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2615, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_ZONE:
            case RC::RC_FREE_PVP_CREATE_FAIL_TRANSACTION:
            case RC::RC_FREE_PVP_CREATE_FAIL_WAIT_MOVE_ZONE:
                {
                    // 2618 ���� �� ������ �� �� ���� �����Դϴ�.
                    g_InterfaceManager.GetInterfaceString(2618, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_INACTIVE:
            case RC::RC_FREE_PVP_JOIN_FAIL_INACTIVE:
                {
                    // 2614 ������������ ���� �̿��� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2614, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_JOIN_FAIL:
                {
                    // 2617 �� ���忡 �����߽��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2617, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM:
                {
                    // 2619 ���� �������� �ʽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(2619, message, INTERFACE_STRING_LENGTH);
                }
            case RC::RC_FREE_PVP_JOIN_FAIL_MAX_USER:
                {
                    // 2477 ���� �ο��� �ʰ� �Ǿ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2477, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_JOIN_FAIL_DIFFER_PASSWORD:
                {
                    // 2494 ��� ��ȣ�� �߸� �Է��Ͽ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2494, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_FREE_PVP_CREATE_FAIL_ALREADY_BATTLE_GROUND_RESERVATION:
                {
                    // 2703 ���� ���� �߿��� �ش� ������ ���� �� �� �����ϴ�.\n���� ������ ����Ͻ� �� �ٽ� �õ��� �ֽʽÿ�
                    g_InterfaceManager.GetInterfaceString(2703, message, INTERFACE_STRING_LENGTH);
                }
                break;
            default:
                {
                    // �˼� ���� �����ڵ�
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
        // �ҳٻ��� Result code �޼��� ���

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
                    // 2617 �� ���忡 ���� �Ͽ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2617, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_STATE_NOT_READY:
            case RC::RC_SONNENSCHEIN_STATE_END:
                {
                    // 5597 ���� ���� �ð��� �ƴմϴ�.
                    g_InterfaceManager.GetInterfaceString(5597, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_FAIL_TRANSACTION:
            case RC::RC_SONNENSCHEIN_BATTLEGROUND_RESERVATION_STATE:
                {
                    // 6429 �ش� ���� ������ �Ұ��� �մϴ�.
                    g_InterfaceManager.GetInterfaceString(6429, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_NOTEXIST_ENTERANCE_ITEM:
            case RC::RC_SONNENSCHEIN_CHECK_ENTRANCE_ITEM_FAILED:
                {
                    // 2495 �� ���忡 �ʿ��� �������� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2495, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_SONNENSCHEIN_NOT_DOMINATION_CHANNEL:
                {
                    // 3538 ���� ä�ο��� ������ �� �����ϴ�. 1ä�ο��� �õ��� �ּ���.
                    g_InterfaceManager.GetInterfaceString(3538, message, INTERFACE_STRING_LENGTH);
                }
                break;
            default:
                {
                    // �˼� ���� �����ڵ�
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
        // ������Ϲ̼� Result code �޼��� ���

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
                    // 2617 �� ���忡 ���� �Ͽ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2617, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_STATE_END:
                {
                    // 5597 ���� ���� �ð��� �ƴմϴ�.
                    g_InterfaceManager.GetInterfaceString(5597, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION:
            case RC::RC_ONEDAYSGUILDMISSION_BATTLEGROUND_RESERVATION_STATE:
                {
                    // 6429 �ش� ���� ������ �Ұ��� �մϴ�.
                    g_InterfaceManager.GetInterfaceString(6429, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_NOTEXIST_ENTERANCE_ITEM:
            case RC::RC_ONEDAYSGUILDMISSION_CHECK_ENTRANCE_ITEM_FAILED:
                {
                    // 2495 �� ���忡 �ʿ��� �������� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(2495, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_NOT_DOMINATION_CHANNEL:
                {
                    // 3538 ���� ä�ο��� ������ �� �����ϴ�. 1ä�ο��� �õ��� �ּ���.
                    g_InterfaceManager.GetInterfaceString(3538, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_NOT_GUILD_USER:
                {
                    // 5165 ��忡 ���ԵǾ� ���� �ʽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(5165, message, INTERFACE_STRING_LENGTH);
                }
                break;
            case RC::RC_ONEDAYSGUILDMISSION_OVER_MAX_LIMIT_ENTERED_CNT:
                {
                    // 3340 ���� ���� �ο����� �ʰ��Ǿ� �����Ͻ� �� �����ϴ�.
                    g_InterfaceManager.GetInterfaceString(3340, message, INTERFACE_STRING_LENGTH);
                }
                break;
            default:
                {
                    // �˼� ���� �����ڵ�
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