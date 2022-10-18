#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/uiChaosZoneResurrection.h"
#include "ChaosSystem/uiChaosZoneResult.h"
#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "LoadScene.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "BattleGroundFlagInfoParser.h"
#include "BattleGroundInfoParser.h"
#include "Map.h"
#include "ObjectManager.h"
#include "AppearanceManager.h"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "ChaosSystem/uiChaosZonePVP.h"
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "HeroEquipmentContainer.h"
#include "SkillMain.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "uiPartyMan/uiPartyMan.h"
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
  #include "ChaosSystem/uiChaosZoneVote.h"
  #include "ChaosSystem/uiChaosZoneVoteResult.h"
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uiHelpMan/uiHelpMan.h"
#include "NPCInfoParser.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "GuildSystem/uiGuildPvE.h"
  #include "uiDominationStamp/uiDominationStamp.h"
  #include "uiDominationTowerMan/uiDominationTowerMan.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include "uiDominationTimer/uiDominationTimer.h"
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

namespace ChaosSystem
{
    namespace PacketFunction
    {
        //------------------------------------------------------------------------------
        // 전장 관련
        void SEND_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN()
        {
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if ((chaos_system_manager != NULL) &&
                (chaos_system_manager->SafePKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_RESERVATION_CANCLE_SYN)))
            {
                chaos_system_manager->SetPKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_RESERVATION_CANCLE_SYN);

                MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN send_packet;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
        }
        void SEND_CG_BATTLE_GROUND_RESERVATION_SYN(DWORD select_mapcode)
        {
            //
            if (!GENERALPARAM->IsNet())
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_BATTLEGROUND, (MAPID)select_mapcode, 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
            //

            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if ((chaos_system_manager != NULL) &&
                (chaos_system_manager->SafePKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_RESERVATION_SYN)))
            {
                chaos_system_manager->SetPKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_RESERVATION_SYN);

                MSG_CG_BATTLE_GROUND_RESERVATION_SYN send_packet;
                send_packet.map_code = static_cast<MAPCODE>(select_mapcode);
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
                // naklu / 2012.12.12 / 힐러 여부 : 스킬트리의 오른쪽 포인트(왼쪽 에테르, 오른쪽 서클)가 많으면 힐러로 간주함.
                // 대규모 리뉴얼이 아닌 이상 바뀔 일이 없을것 같기에 양쪽 포인트를 직접 대소 비교함
                SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSet();
                send_packet.is_healer = (g_pHero->GetClass() == eCHAR_ELEMENTALIST) &&
                    (skill_set.GetTotalUsedPoint(true) <= skill_set.GetTotalUsedPoint(false));
#else
                send_packet.is_healer = false;
#endif //_NA_0_20120321_SKILLTREE_SECOND_RENEWAL
                
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
        }
        void SEND_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN()
        {
            MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN send_packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        void SEND_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN()
        {
            if (ChaosSystem::GetBattleGroundData().player_state() != kPlayerStateStandbyResurectionRefuse)
            {
                return;
            }

            MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN send_packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        void SEND_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN()
        {
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if ((chaos_system_manager != NULL) &&
                (chaos_system_manager->SafePKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_GET_OPEN_TIME_SYN)))
            {
                chaos_system_manager->SetPKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_GET_OPEN_TIME_SYN);

                MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN send_packet;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
#endif //_NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
        }


        void RECV_CG_BATTLE_GROUND_RESERVATION_ACK(MSG_BASE* recv_message)
        {
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager != NULL)
            {
                chaos_system_manager->UnsetPKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_RESERVATION_SYN);

                MSG_CG_BATTLE_GROUND_RESERVATION_ACK* message =
                    static_cast<MSG_CG_BATTLE_GROUND_RESERVATION_ACK*>(recv_message);

                //message->battle_ground_reservation_key;
                if (message->result_code == RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS)
                {
                    ChaosSystem::Function::SetBattleGroundReservation(true);  // 예약 활성화
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                    // 2704 전장 입장 예약을 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2704, message, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                }
                else
                {
                    ChaosSystem::Function::OutputResultStringBattleGround(message->result_code);
                    ChaosSystem::GetBattleGroundData().ReleaseMutexHandle();
                }
            }
        }
        void RECV_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK(MSG_BASE* recv_message)
        {
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager != NULL)
            {
                chaos_system_manager->UnsetPKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_RESERVATION_CANCLE_SYN);

                MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK* message =
                    static_cast<MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK*>(recv_message);

                if (message->result_code == RC::RC_BATTLE_GROUND_RESERVATION_CANCEL_SUCCESS)
                {
                    ChaosSystem::Function::SetBattleGroundReservation(false);  // 예약 비활성화
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                    // 2709 전장 입장 예약을 취소 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2709, message, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

                    ChaosSystem::GetBattleGroundData().ReleaseMutexHandle();
                }
                else
                {
                    ChaosSystem::Function::OutputResultStringBattleGround(message->result_code);
                }
            }
        }
        void RECV_CG_BATTLE_GROUND_RESERVATION_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_RESERVATION_BRD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_RESERVATION_BRD*>(recv_message);
    
            ChaosSystem::GetBattleGroundData().set_reservation_member_count(message->current_reservation_memeber_count);
        }
        void RECV_CG_BATTLE_GROUND_MATCHING_RESULT_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_MATCHING_RESULT_BRD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_MATCHING_RESULT_BRD*>(recv_message);

            message->result_type;

            // %s초후에 전장이 시작됩니다.
            TCHAR notice_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR time_string[INTERFACE_STRING_LENGTH] = {0,};
            DWORD mapcode = 0;

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
            mapcode = message->map_code;
#else
            mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

            const BattleGroundInfo* battle_ground_info =
                BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(mapcode));

            if (battle_ground_info == NULL)
            {
                return;
            }
            _itot(battle_ground_info->start_time, time_string, 10); // 몇초후 입장
            // 2599 전투가 준비되었습니다.\n%s초 뒤에 전장에 입장합니다.
            g_InterfaceManager.GetInterfaceStringFormat(notice_string, INTERFACE_STRING_LENGTH-1, 2599, time_string);
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager)
            {
                chaos_system_manager->WaitGague(true, battle_ground_info->start_time);
            }
            GlobalFunc::default_OutputMsg(eOUTPUT_MSG_CENTER_SHOW | eOUTPUT_MSG_CHAT, notice_string);
        }
        void RECV_CG_BATTLE_GROUND_TEAM_INFO_CMD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_TEAM_INFO_CMD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_TEAM_INFO_CMD*>(recv_message);

            if (g_pHero != NULL)
            {
                g_pHero->SetTeam(message->team_key);
            }
        }
        void RECV_CG_BATTLE_GROUND_TEAM_INFO_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_TEAM_INFO_BRD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_TEAM_INFO_BRD*>(recv_message);

            ChaosSystem::GetBattleGroundData().SetBattleGroundMemberInfo(message->member_count, message->member_list);
        }
        void RECV_CG_BATTLE_GROUND_STATE_RESURECTION_CMD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD*>(recv_message);

            if (g_pHero && (g_pHero->GetObjectKey() == message->player_object_key))
            {
                ChaosSystem::GetBattleGroundData().set_player_state(message->player_state);
                
                // 부활 대기 상태일때는 시간 세팅
                if (message->player_state == kPlayerStateStandbyResurection)
                {
                    uiChaosZoneResurrection* chaos_zone_resurrection =
                        GET_CAST_DIALOG(uiChaosZoneResurrection, IM_CHAOS_SYSTEM_MANAGER::BATTLE_GROUND_RESURRECTION_POPUP);
                    if (chaos_zone_resurrection)
                    {
                        chaos_zone_resurrection->set_resurrection_tick(message->resurection_time);
                        chaos_zone_resurrection->ShowInterface(TRUE);
                    }
                }
            }

            Player* player = static_cast<Player*>(g_ObjectManager.GetObject(message->player_object_key));
            if (player)
            {
                // 상태에 따른 이동/투명 여부 세팅
                if ((message->player_state == kPlayerStateStandbyResurection) ||
                    (message->player_state == kPlayerStateStandbyResurectionRefuse))
                {
                    // 투명화
                    player->SetBattleGroundHideMode(true);
                }
                else
                {
                    // 투명화 끔
                    player->SetBattleGroundHideMode(false);
                }
            }
        }
        void RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(MSG_BASE* recv_message, bool is_show_message)
        {
            MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD*>(recv_message);

            BattleGroundFlagInfo* flag_info =
                BattleGroundFlagInfoParser::Instance()->GetBattleGroundFlagInfo(g_pMap->GetCurrentFieldID(),
                message->strongpoint_info.strongpoint_id);
            if (flag_info == NULL)
            {
                return;
            }
            int flag_count = flag_info->vector_flag_key.size();
            MapObject* map_object = NULL;
            for(int i = 0; i < flag_count; ++i)
            {
                DWORD flag_object_key = flag_info->vector_flag_key[i];

                // 깃발 오브젝트 색상 변경
                map_object = static_cast<MapObject*>(g_ObjectManager.GetObject(flag_object_key));
                if (map_object != NULL)
                {
                    if (message->strongpoint_info.team_key == kBlue)
                    {
                        if ((message->strongpoint_info.strongpoint_state == kStrongpointStateWait) ||
                            (message->strongpoint_info.strongpoint_state == kStrongpointStateWaitAttack))
                        {
                            map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_half_blue_flag); //파흰
                            map_object->ChangeBattleGroundObjectAnimation(battle_ground_flag_change_animation, true);
                        }
                        else if ((message->strongpoint_info.strongpoint_state == kStrongpointStateConquer) ||
                            (message->strongpoint_info.strongpoint_state == kStrongpointStateDefence))
                        {
                            map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_blue_flag); //파
                            map_object->ChangeBattleGroundObjectAnimation(battle_ground_flag_change_animation, true);
                        }
                    }
                    else if (message->strongpoint_info.team_key == kRed)
                    {
                        if ((message->strongpoint_info.strongpoint_state == kStrongpointStateWait) ||
                            (message->strongpoint_info.strongpoint_state == kStrongpointStateWaitAttack))
                        {
                            map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_half_red_flag); //빨흰
                            map_object->ChangeBattleGroundObjectAnimation(battle_ground_flag_change_animation, true);
                        }
                        else if ((message->strongpoint_info.strongpoint_state == kStrongpointStateConquer) ||
                            (message->strongpoint_info.strongpoint_state == kStrongpointStateDefence))
                        {
                            map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_red_flag); //빨
                            map_object->ChangeBattleGroundObjectAnimation(battle_ground_flag_change_animation, true);
                        }
                    }
                    else
                    {
                        map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_normal_flag); //흰
                    }
                }
            }
            // 거점 오브젝트 색상 변경
            map_object = static_cast<MapObject*>(g_ObjectManager.GetObject(flag_info->strong_point_key));
            if (map_object != NULL)
            {
                TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,}; // 점령 메세지 출력
                TCHAR strong_point_name[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR team_name[INTERFACE_STRING_LENGTH] = {0,};

                // 거점 명칭
                DWORD strong_point_name_code = BattleGroundFlagInfoParser::Instance()->GetStrongPointName(
                    g_pMap->GetCurrentFieldID(), map_object->GetObjectKey());
                g_InterfaceManager.GetInterfaceString(strong_point_name_code, strong_point_name, INTERFACE_STRING_LENGTH-1);

                //팀 명칭
                if (message->strongpoint_info.team_key == kRed)
                {
                    // 2662 레드팀
                    g_InterfaceManager.GetInterfaceString(2662, team_name, INTERFACE_STRING_LENGTH-1);
                }
                else if (message->strongpoint_info.team_key == kBlue)
                {
                    // 2663 블루팀
                    g_InterfaceManager.GetInterfaceString(2663, team_name, INTERFACE_STRING_LENGTH-1);
                }

                //
                if ((message->strongpoint_info.strongpoint_state == kStrongpointStateConquer) ||
                    (message->strongpoint_info.strongpoint_state == kStrongpointStateDefence)) // 점령상태 + 방어 성공
                {
                    if (message->strongpoint_info.team_key == kBlue)
                    {
                        map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_blue_waypoint); // 파 기둥
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_blue_waypoint_bluefield_blueconquerd, true); // 파 오오라 + 파 성공이펙트
                    }
                    else if (message->strongpoint_info.team_key == kRed)
                    {
                        map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_red_waypoint); // 빨 기둥
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_red_waypoint_redfield_redconquered, true); // 빨 오오라 + 빨 성공이펙트
                    }

                    if (message->strongpoint_info.strongpoint_state == kStrongpointStateConquer)
                    {
                        //2641 %s팀이 %s 점령에 성공하였습니다.
                        g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2641, team_name, strong_point_name);
                    }
                    else
                    {
                        // 2644 %s, %s 방어성공!
                        g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2644, strong_point_name, team_name);
                    }
                }
                else if (message->strongpoint_info.strongpoint_state == kStrongpointStateWait) // 대기 상태(선점) - 흰색기둥을 먹을 때
                {
                    // 선점이므로 거점은 흰색이었을 것이라고 가정함
                    if (message->strongpoint_info.team_key == kBlue)
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_bluefield_blueconquered, true); // 파 오오라 + 파 성공이펙트
                    }
                    else if (message->strongpoint_info.team_key == kRed)
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_redfield_redconquered, true); // 빨 오오라 + 빨 성공이펙트
                    }

                    const BattleGroundInfo* battle_ground_info =
                        BattleGroundInfoParser::Instance()->FindData(g_pMap->GetCurrentFieldID());
                    TCHAR second_string[INTERFACE_STRING_LENGTH] = {0, };
                    _itot(battle_ground_info->conquer_wait_time, second_string, 10); // 점령대기시간
                    //2642 %s 선점완료! %s초후에 %s 점령이 완료됩니다.
                    g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH,
                        2642, team_name, second_string, strong_point_name);
                }
                else if (message->strongpoint_info.strongpoint_state == kStrongpointStateWaitAttack) // 대기 상태(공격) - 상대기둥을 먹을때
                {
                    map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_normal_waypoint); // 흰 기둥
                    if (message->strongpoint_info.team_key == kBlue)
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_bluefield_blueconquered, true); // 파 오오라 + 파 성공이펙트
                    }
                    else if (message->strongpoint_info.team_key == kRed)
                    {
                        map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_redfield_redconquered, true); // 빨 오오라 + 빨 성공이펙트
                    }

                    // 2643 %s, %s 공격성공!
                    g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 2643, strong_point_name, team_name);
                }
                else
                {
                    map_object->ChangeBattleGroundObjectAppearance(battle_ground_object_normal_waypoint); // 흰 거점
                    map_object->ChangeBattleGroundObjectAnimation(battle_ground_normal_waypoint_whitefield); // 흰 오오라
                }

                if (is_show_message == true)
                {
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, result_string);
                }
            }
        };
        void RECV_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD* message = 
                static_cast<MSG_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD*>(recv_message);

            for (int i = 0; i < BattleGroundData::MAX_TEAM_COUNT; ++i)
            {
                ChaosSystem::GetBattleGroundData().set_team_score_info(&message->team_score_info[i]);
            }
        };

        void RECV_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN* message =
                static_cast<MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN*>(recv_message);

            // 비정상 종속 복귀자에게 거점정보 세팅
            for (DWORD i = 0; i < message->strongpoint_count; ++i)
            {
                MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
                temp_message.strongpoint_info.strongpoint_id = message->strongpoint_info[i].strongpoint_id;
                temp_message.strongpoint_info.strongpoint_state = message->strongpoint_info[i].strongpoint_state;
                temp_message.strongpoint_info.team_key = message->strongpoint_info[i].team_key;
                RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message, false);
            }
        }

        void RECV_CG_BATTLE_GROUND_FINISH_RESULT_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_FINISH_RESULT_BRD* message =
                static_cast<MSG_CG_BATTLE_GROUND_FINISH_RESULT_BRD*>(recv_message);

            ChaosSystem::GetBattleGroundData().ReleaseMutexHandle();

            uiChaosZoneResult* chaos_zone_result = GET_CAST_DIALOG(uiChaosZoneResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RESULT);
            if (chaos_zone_result)
            {
                chaos_zone_result->ShowChaosZoneResult(message->result_type);
            }

            // 전장 종료
            ChaosSystem::GetBattleGroundData().EndBattleGround();
            uiBattleScoreMan* battle_score_manager = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
            if (battle_score_manager)
            {
                // 전장 나가기 버튼 활성화
                battle_score_manager->EnableBattleGroundExitButton();
            }

            // 누구 팀이 승리하였습니다.
            TCHAR result_message_string[INTERFACE_STRING_LENGTH] = {0,};
            switch (message->result_type)
            {
            case kResultWin:
                {
                    // 2694 전장에서 승리 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2694, result_message_string);
                }
                break;
            case kResultLose:
                {
                    // 2696 전장에서 패배 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2696, result_message_string);
                }
                break;
            case kResultDraw:
                {
                    // 2695 전장에서 승부를 가리지 못하고 무승부가 되었습니다.
                    g_InterfaceManager.GetInterfaceString(2695, result_message_string);
                }
                break;
            case kResultAbuse:
                {
                    // 
                    //g_InterfaceManager.GetInterfaceString(, result_message_string);
                }
                break;
            };
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_message_string);

        }
        void RECV_CG_BATTLE_GROUND_TIME_INFO_CMD(MSG_BASE* recv_message)
        {
            MSG_CG_BATTLE_GROUND_TIME_INFO_CMD* message =
                static_cast<MSG_CG_BATTLE_GROUND_TIME_INFO_CMD*>(recv_message);

            // 전장 대기시간 돌입
            ChaosSystem::GetBattleGroundData().StartBattleGround();

            if (message->elapsed_time == 0)
            {
                TCHAR notice_string[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR time_string[INTERFACE_STRING_LENGTH] = {0,};
                DWORD mapcode;

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
                mapcode = message->map_code;
#else
                mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

                const BattleGroundInfo* battle_ground_info =
                    BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(mapcode));

                if (battle_ground_info != NULL)
                {
                    _itot(battle_ground_info->prepare_time, time_string, 10); // 몇초간 대기시간
                    // 2721 전투준비시간
                    g_InterfaceManager.GetInterfaceStringFormat(notice_string, INTERFACE_STRING_LENGTH-1, 2721, time_string);
                    GlobalFunc::default_OutputMsg(eOUTPUT_MSG_CENTER_SHOW | eOUTPUT_MSG_CHAT, notice_string);
                }
            }
        }
        void RECV_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK(MSG_BASE* recv_message)
        {
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager != NULL)
            {
                chaos_system_manager->UnsetPKBool(uiChaosSystemMan::ePKBOOL_BATTLEGROUND_GET_OPEN_TIME_SYN);

                MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK* message =
                    static_cast<MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK*>(recv_message);

                ChaosSystem::GetBattleGroundData().set_battle_ground_open_type(message->join_state);
                ChaosSystem::GetBattleGroundData().set_battle_ground_open_time(message->join_time);
                ChaosSystem::GetBattleGroundData().set_battle_ground_open_recv_time(clock_function::GetTickCount());
            }
        }

        void RECV_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD(MSG_BASE* recv_message)
        {
#ifdef _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY
            MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD* message =
                static_cast<MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD*>(recv_message);

            if (message == NULL)
            {
                return;
            }

            if (GameFramework::GetCurrentScene() &&
                (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_BATTLEGROUND))
            {
                TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

                WORD kill_player_team = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(message->kill_payer_name);
                WORD death_player_team = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(message->death_payer_name);
                DWORD kill_user_color = (kill_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);
                DWORD death_user_color = (death_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
                BYTE pk_point = message->pk_point; // 추가 득점 점수
#endif // _NA_006599_20130401_BATTLEGROUND_PKPOINT

                if (message->is_series_kill_block == true)
                {
                    // 2751 %s님이 %s님의 연속킬 차단
                    g_InterfaceManager.GetInterfaceString(2751, temp_string, INTERFACE_STRING_LENGTH-1);
                    GameFramework::GetCurrentScene()->InsertKillMessage(
                        message->kill_payer_name,
                        kill_user_color,
                        message->death_payer_name,
                        death_user_color,
                        temp_string);
                }
                else
                {
                    // 2749 %s님이 %s님을 처치했습니다! 
                    g_InterfaceManager.GetInterfaceString(2749, temp_string, INTERFACE_STRING_LENGTH-1);
                    GameFramework::GetCurrentScene()->InsertKillMessage(
                        message->kill_payer_name,
                        kill_user_color,
                        message->death_payer_name,
                        death_user_color,
                        temp_string);
                }

                if (message->is_series_kill == true)
                {
                    TCHAR count_string[INTERFACE_STRING_LENGTH] = {0,};
                    _itot(message->series_kill_count, count_string, 10);
                    // 2750	%s님 %s연속 킬!
                    g_InterfaceManager.GetInterfaceStringFormat(temp_string, INTERFACE_STRING_LENGTH-1, 2750, "%s", count_string);
                    
                    GameFramework::GetCurrentScene()->InsertKillMessage(
                        message->kill_payer_name,
                        kill_user_color,
                        message->death_payer_name,
                        death_user_color,
                        temp_string);
                }

#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
                // 전장 추가 득점 출력과 현재 팀 점수 들 갱신
                // 2752 %s님이 추가점수 %s점을 얻었습니다.
                g_InterfaceManager.GetInterfaceString(2752, temp_string, INTERFACE_STRING_LENGTH-1);
                TCHAR number_char[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR result_char[INTERFACE_STRING_LENGTH] = {0, };
                Snprintf(number_char, INTERFACE_STRING_LENGTH-1, "%d", pk_point);                
                Snprintf(result_char, INTERFACE_STRING_LENGTH-1, temp_string, message->kill_payer_name, number_char);
                
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_char);

                // 점수를 갱신
                BATTLE_GROUND_TEAM_SCORE_INFO* score_message = &(message->team_score_info[0]);

                for (int i = 0; i < BattleGroundData::MAX_TEAM_COUNT; ++i)
                {
                    ChaosSystem::GetBattleGroundData().set_team_score_info(&score_message[i]);
                }
#endif // _NA_006599_20130401_BATTLEGROUND_PKPOINT
            }
#endif //_NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY
        }

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        //------------------------------------------------------------------------------
        // 전장 투표 관련
        void SEND_CG_VOTE(TCHAR* character_name)
        {
            // 투표 발의
            if ((GlobalFunc::IsBattleGroundScene() == false) ||
                (Strcmp(g_pHero->GetName(), character_name) == 0))
            {
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL); // 전장 에러메세지 출력
                return;
            }

            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (party_manager && party_manager->IsPartyMemberForName(character_name) == 0)
            {
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL); // 전장 에러메세지 출력
                return;
            }
            if (party_manager && party_manager->GetPartyNum() < 4)
            {
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT); // 전장 에러메세지 출력
                return;
            }

            if ((ChaosSystem::GetBattleGroundData().GetTeamScore(kRed)->victory_point < 1000) &&
                (ChaosSystem::GetBattleGroundData().GetTeamScore(kBlue)->victory_point < 1000))
            {
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_VICTORYPOINT); // 전장 에러메세지 출력
                return;
            }

            MSG_CG_BATTLE_GROUND_INITIATE_VOTE_SYN send_packet;
            StrnCopy(send_packet.van_payer_name, character_name, MAX_CHARNAME_LENGTH);

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

        }
        void SEND_CG_BATTLE_GROUND_VOTE_SYN(bool vote_result)
        {
            // 전장 투표
            MSG_CG_BATTLE_GROUND_VOTE_SYN send_packet;
            send_packet.vote_result = vote_result;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

        }
        void RECV_CG_BATTLE_GROUND_INITIATE_VOTE_NAK(MSG_BASE* recv_message)
        {
            // 전장 추방 투표 발의 실패
            MSG_CG_BATTLE_GROUND_INITIATE_VOTE_NAK* message =
                static_cast<MSG_CG_BATTLE_GROUND_INITIATE_VOTE_NAK*>(recv_message);

            // 에러 메세지 출력
            ChaosSystem::Function::OutputResultStringBattleGround(message->result); // 전장 에러메세지 출력

        }
        void RECV_CG_BATTLE_GROUND_INITIATE_VOTE_BRD(MSG_BASE* recv_message)
        {
            // 전장 추방 투표 발의 BRD
            MSG_CG_BATTLE_GROUND_INITIATE_VOTE_BRD* message =
                static_cast<MSG_CG_BATTLE_GROUND_INITIATE_VOTE_BRD*>(recv_message);

            uiChaosZoneVote* chaos_zone_vote = GET_CAST_DIALOG(uiChaosZoneVote, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE);
            if (chaos_zone_vote)
            {
                chaos_zone_vote->set_character_name(message->van_payer_name);
                chaos_zone_vote->set_vote_remain_time(30000);
                chaos_zone_vote->ShowInterface(true);
            }
        }
        void RECV_CG_BATTLE_GROUND_VOTE_STATUS_BRD(MSG_BASE* recv_message)
        {
            // 전장 투표 현황
            MSG_CG_BATTLE_GROUND_VOTE_STATUS_BRD* message =
                static_cast<MSG_CG_BATTLE_GROUND_VOTE_STATUS_BRD*>(recv_message);

            uiChaosZoneVoteResult* chaos_zone_vote_result = GET_CAST_DIALOG(uiChaosZoneVoteResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE_RESULT);
            if (chaos_zone_vote_result)
            {
                chaos_zone_vote_result->set_character_name(message->van_payer_name);
                chaos_zone_vote_result->set_vote_yes_count(message->yes_count);
                chaos_zone_vote_result->set_vote_no_count(message->no_count);
                chaos_zone_vote_result->set_vote_remain_time(message->remain_vote_time);

                if (chaos_zone_vote_result->IsVisible() == FALSE)
                {
                    chaos_zone_vote_result->ShowInterface(true);
                }
            }
        }
        
        void RECV_CG_BATTLE_GROUND_VOTE_RESULT_BRD(MSG_BASE* recv_message)
        {
            // 전장 투표 결과
            MSG_CG_BATTLE_GROUND_VOTE_RESULT_BRD* message =
                static_cast<MSG_CG_BATTLE_GROUND_VOTE_RESULT_BRD*>(recv_message);

            TCHAR string_message[INTERFACE_STRING_LENGTH] = {0,};
            if (message->vote_result)
            {
                TCHAR format_message[INTERFACE_STRING_LENGTH] = {0,};
                // 2775 추방투표가 통과되었습니다. %s님이 전장에서 추방되었습니다.
                g_InterfaceManager.GetInterfaceString(2775, format_message, INTERFACE_STRING_LENGTH);

                Snprintf(string_message, INTERFACE_STRING_LENGTH, format_message, message->van_payer_name);

                // 킬메세지 형식으로 투표메세지도 출력해버림.
                WORD van_player_team = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(message->van_payer_name);
                DWORD van_user_color = (van_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);
                GameFramework::GetCurrentScene()->InsertKillMessage(
                    message->van_payer_name,
                    van_user_color,
                    "",
                    0,
                    format_message);
            }
            else
            {
                // 2776 추방투표가 부결되었습니다. 1분간은 동일한 플레이어를 대상으로 추방투표를 할 수 없습니다.
                g_InterfaceManager.GetInterfaceString(2776, string_message, INTERFACE_STRING_LENGTH);
            }

            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, string_message);

            uiChaosZoneVote* chaos_zone_vote = GET_CAST_DIALOG(uiChaosZoneVote, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE);
            if (chaos_zone_vote)
            {
                if (chaos_zone_vote->IsVisible() == TRUE)
                {
                    chaos_zone_vote->ShowInterface(false);
                }
            }
            uiChaosZoneVoteResult* chaos_zone_vote_result = GET_CAST_DIALOG(uiChaosZoneVoteResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE_RESULT);
            if (chaos_zone_vote_result)
            {
                if (chaos_zone_vote_result->IsVisible() == TRUE)
                {
                    chaos_zone_vote_result->ShowInterface(false);
                }
            }

        }
        void RECV_CG_BATTLE_GROUND_VANP_PLAYER_CMD(MSG_BASE* recv_message)
        {
            // 전장 강제 추방 명령
            MSG_CG_BATTLE_GROUND_VANP_PLAYER_CMD* message =
                static_cast<MSG_CG_BATTLE_GROUND_VANP_PLAYER_CMD*>(recv_message);

            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (party_manager)
            {
                party_manager->LeaveParty();
            }

            // 나 강퇴
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager != NULL)
            {
                chaos_system_manager->WaitGague(true, 5, true);

                // 2782 %s님 추방됨
                TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceStringFormat(temp_string, INTERFACE_STRING_LENGTH, 2782, g_pHero->GetName());
                GlobalFunc::VerifyMessage(temp_string, NULL);
            }
        }
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM


        //------------------------------------------------------------------------------
        // 자유전투지역 관련
        void SEND_CG_FREE_PVP_ROOM_LIST_SYN()
        {
            // 방 목록 요청
            MSG_CG_FREE_PVP_ROOM_LIST_SYN send_packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        void SEND_CG_FREE_PVP_CREATE_ROOM_SYN(BaseFreePVPRoomInfo* base_room_info)
        {
            MSG_CG_FREE_PVP_CREATE_ROOM_SYN send_packet;

            memcpy(&(send_packet.base_free_pvp_info), base_room_info, sizeof(*base_room_info));

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        void SEND_CG_FREE_PVP_JOIN_ROOM_SYN(KEYTYPE room_key, bool is_private_room, TCHAR* password)
        {
            MSG_CG_FREE_PVP_JOIN_ROOM_SYN send_packet;

            send_packet.join_room_key = room_key;
            send_packet.is_private_room = is_private_room;
            _tcsncpy(send_packet.password, password, MAX_ROOMPASSWORD_LENGTH);
            send_packet.password[MAX_ROOMPASSWORD_LENGTH] = '\0';

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        void SEND_CG_FREE_PVP_RESURRECTION_SYN()
        {
            MSG_CG_FREE_PVP_RESURRECTION_SYN send_packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }

        void RECV_CG_FREE_PVP_ROOM_LIST_ACK(MSG_BASE* recv_message)
        {
            MSG_CG_FREE_PVP_ROOM_LIST_ACK* message = 
                static_cast<MSG_CG_FREE_PVP_ROOM_LIST_ACK*>(recv_message);

            // 방 목록 삽입
            ChaosSystem::GetFreePVPData().SetFreePVPRoomList(message->count_of_list, message->free_pvp_room_info);

            uiChaosZonePVP* pvp_dialog = GET_CAST_DIALOG(uiChaosZonePVP, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP);
            if (pvp_dialog)
            {
                pvp_dialog->ShowAnimation(false);
                pvp_dialog->SetPVPRoomList();
            }
        }
        void RECV_CG_FREE_PVP_CREATE_ROOM_ACK(MSG_BASE* recv_message)
        {
            MSG_CG_FREE_PVP_CREATE_ROOM_ACK* message = 
                static_cast<MSG_CG_FREE_PVP_CREATE_ROOM_ACK*>(recv_message);

            if (message->result_code == RC::RC_FREE_PVP_CREATE_SUCCESS)
            {
            }
            else
            {
                ChaosSystem::Function::OutputResultStringFreePVP(message->result_code);
            }
        }
        void RECV_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD(MSG_BASE* recv_message)
        {
            MSG_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD* message = 
                static_cast<MSG_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD*>(recv_message);

            // %s초후에 전장이 시작됩니다.
            TCHAR notice_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR time_string[INTERFACE_STRING_LENGTH] = {0,};

            //const BattleGroundInfo* battle_ground_info =
            //BattleGroundInfoParser::Instance()->FindData(g_pMap->GetCurrentFieldID());
            //_itot(battle_ground_info->, time_string, 10); // 저 5초는 스크립트에서 읽어오자  ///_itot(pRecvPacket->betting_item_count_, temp_message, 10);
            //g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH-1, 6767, time_string);
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager)
            {
                chaos_system_manager->WaitGague(true, 10);
            }
        }
        void RECV_CG_FREE_PVP_JOIN_ROOM_ACK(MSG_BASE* recv_message)
        {
            MSG_CG_FREE_PVP_JOIN_ROOM_ACK* message = 
                static_cast<MSG_CG_FREE_PVP_JOIN_ROOM_ACK*>(recv_message);
            
            if (message->result_code == RC::RC_FREE_PVP_JOIN_SUCCESS)
            {
                ChaosSystem::GetFreePVPData().set_my_room_info(&message->free_pvp_room_info);
                uiChaosZonePVP* pvp_dialog = GET_CAST_DIALOG(uiChaosZonePVP, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP);
                if (pvp_dialog)
                {
                    pvp_dialog->ClearEditText();
                }
            }
            else
            {
                ChaosSystem::Function::OutputResultStringFreePVP(message->result_code);
            }

            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager)
            {
                chaos_system_manager->WaitGague(false);
            }

            ChaosSystem::GetChaosZoneData().set_is_wait(false);
            if (g_InterfaceManager.IsLockDlg())
            {
                g_InterfaceManager.UnLockDlgs();
            }
        }

        //------------------------------------------------------------------------------
        // 존 관련
        void RECV_CG_CHAOS_ZONE_ZONE_JOIN_ACK(MSG_BASE* recv_packet)
        {
            MSG_CG_CHAOS_ZONE_ZONE_JOIN_ACK* const recv_msg = static_cast<MSG_CG_CHAOS_ZONE_ZONE_JOIN_ACK*>(recv_packet);

            if (recv_msg->chaos_zone_type == kBattleGround)
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_BATTLEGROUND, recv_msg->map_code, 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
            else if (recv_msg->chaos_zone_type == kFreePVP)
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_FREEPVP, recv_msg->map_code, 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            else if (recv_msg->chaos_zone_type == eCHAOSZONE_TYPE_GOLDRUSH)
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_GOLDRUSH, recv_msg->map_code, 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
            else if (recv_msg->chaos_zone_type == eCHAOSZONE_TYPE_SONNENSCHEIN)
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SONNENSCHEIN, recv_msg->map_code, 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            else if (recv_msg->chaos_zone_type == eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_GUILDMISSION, recv_msg->map_code, 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        }
        //------------------------------------------------------------------------------
        // 골드 러쉬 관련
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void SEND_CG_GOLDRUSH_RESERVATION_SYN(DWORD select_map_code)
        {
            //
            if (!GENERALPARAM->IsNet())
            {
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_GOLDRUSH, 
                    static_cast<MAPID>(select_map_code), 0);
                GameFramework::ChangeScene(SCENE_TYPE_LOAD);
            }
            //

            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if ((chaos_system_manager != NULL) &&
                (chaos_system_manager->SafePKBool(uiChaosSystemMan::ePKBOOL_GOLDRUSH_RESERVATION_SYN)))
            {
                chaos_system_manager->SetPKBool(uiChaosSystemMan::ePKBOOL_GOLDRUSH_RESERVATION_SYN);

                MSG_CG_GOLDRUSH_RESERVATION_SYN send_packet;
                send_packet.map_code = static_cast<MAPCODE>(select_map_code);

                // naklu / 2012.12.12 / 힐러 여부 : 스킬트리의 오른쪽 포인트(왼쪽 에테르, 오른쪽 서클)가 많으면 힐러로 간주함.
                // 대규모 리뉴얼이 아닌 이상 바뀔 일이 없을것 같기에 양쪽 포인트를 직접 대소 비교함
                SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSet();
                send_packet.is_healer = (g_pHero->GetClass() == eCHAR_ELEMENTALIST) &&
                    (skill_set.GetTotalUsedPoint(true) <= skill_set.GetTotalUsedPoint(false));
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
        }
        void SEND_CG_GOLDRUSH_RESERVATION_CANCEL_SYN()
        {
            uiChaosSystemMan* chaos_system_manager_ptr = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if ((chaos_system_manager_ptr != NULL) &&
                (chaos_system_manager_ptr->SafePKBool(uiChaosSystemMan::ePKBOOL_GOLDRUSH_RESERVATION_CANCLE_SYN)))
            {
                chaos_system_manager_ptr->SetPKBool(uiChaosSystemMan::ePKBOOL_GOLDRUSH_RESERVATION_CANCLE_SYN);

                MSG_CG_GOLDRUSH_RESERVATION_CANCEL_SYN send_packet;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            }
        }
        void RECV_CG_GOLDRUSH_RESERVATION_ACK(MSG_BASE* recv_message)
        {
            uiChaosSystemMan* chaos_system_manager_ptr = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager_ptr != NULL)
            {
                chaos_system_manager_ptr->UnsetPKBool(uiChaosSystemMan::ePKBOOL_GOLDRUSH_RESERVATION_SYN);

                MSG_CG_GOLDRUSH_RESERVATION_ACK* message =
                    static_cast<MSG_CG_GOLDRUSH_RESERVATION_ACK*>(recv_message);
                
                if (message->result_code == RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS)
                {
                    ChaosSystem::Function::SetGoldRushReservation(true);  // 예약 활성화
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};                    
                    // 2900 골드러시 입장 예약을 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2900, message, INTERFACE_STRING_LENGTH);                    
                    
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);                    
                }
                else
                {
                    ChaosSystem::Function::OutputResultStringBattleGround(message->result_code);
                    ChaosSystem::ChaosZoneData::ReleaseMutexHandle();
                }
            }
        }
        void RECV_CG_GOLDRUSH_RESERVATION_CANCEL_ACK(MSG_BASE* recv_message)
        {
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager != NULL)
            {
                chaos_system_manager->UnsetPKBool(uiChaosSystemMan::ePKBOOL_GOLDRUSH_RESERVATION_CANCLE_SYN);

                MSG_CG_GOLDRUSH_RESERVATION_CANCEL_ACK* message =
                    static_cast<MSG_CG_GOLDRUSH_RESERVATION_CANCEL_ACK*>(recv_message);

                if (message->result_code == RC::RC_BATTLE_GROUND_RESERVATION_CANCEL_SUCCESS)
                {
                    ChaosSystem::Function::SetGoldRushReservation(false);  // 예약 비활성화
                    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};                    
                    // 2899 골드러시 입장 예약을 취소하였습니다.
                    g_InterfaceManager.GetInterfaceString(2899, message, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

                    ChaosSystem::ChaosZoneData::ReleaseMutexHandle();
                }
                else
                {
                    ChaosSystem::Function::OutputResultStringBattleGround(message->result_code);
                }
            }
        }
        void RECV_CG_GOLDRUSH_RESERVATION_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_GOLDRUSH_RESERVATION_BRD* message = 
                static_cast<MSG_CG_GOLDRUSH_RESERVATION_BRD*>(recv_message);

            ChaosSystem::GetGoldRushData().set_reservation_member_count(message->current_reservation_memeber_count);
        }
        void RECV_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN(MSG_BASE* recv_message)
        {
            MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN* message =
                static_cast<MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN*>(recv_message);            

            for (int i = 0; i < 2; ++i)
            {
                ChaosSystem::GetGoldRushData().set_team_score(message->team_info[i].team_key, 
                    message->team_info[i].total_gold_point);
            }

            // 비정상 종료 접속자에게 트리거 Time을 보여준다.
            uiHelpMan* ui_manager = GET_DIALOG_MANAGER(uiHelpMan, UIMAN_HELP);
            if (ui_manager)
            {
                ui_manager->SetMissionTimer(message->remained_time, eTIMER_CREATE_DECREASE);
            }
        }
        void SEND_CG_GOLDRUSH_VOTE(TCHAR* character_name)
        {
            // 투표 발의
            if ((GlobalFunc::IsGoldRushScene() == false) ||
                (Strcmp(g_pHero->GetName(), character_name) == 0))
            {
                // 이 부분은 기획자 분과 상의해서 result code로 하여금 메시지를 출력 할 수 있도록 상의해야 한다.
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL); // 전장 에러메세지 출력
                return;
            }

            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (party_manager && party_manager->IsPartyMemberForName(character_name) == 0)
            {
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL); // 전장 에러메세지 출력
                return;
            }
            if (party_manager && party_manager->GetPartyNum() < 4)
            {
                ChaosSystem::Function::OutputResultStringBattleGround(RC::RC_BATTLE_GROUND_INITIATE_VOTE_DO_NOT_SATISFY_MEMBER_COUNT); // 전장 에러메세지 출력
                return;
            }            

            MSG_CG_GOLDRUSH_INITIATE_VOTE_SYN send_packet;
            StrnCopy(send_packet.van_payer_name, character_name, MAX_CHARNAME_LENGTH);

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }        
        void SEND_CG_GOLDRUSH_VOTE_SYN(bool vote_result)
        {
            // 골드러쉬 투표
            MSG_CG_GOLDRUSH_VOTE_SYN send_packet;
            send_packet.vote_result = vote_result;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

        }
        void RECV_CG_GOLDRUSH_INITIATE_VOTE_NAK(MSG_BASE* recv_message)
        {
            // 골드 추방 투표 발의 실패
            MSG_CG_GOLDRUSH_INITIATE_VOTE_NAK* message =
                static_cast<MSG_CG_GOLDRUSH_INITIATE_VOTE_NAK*>(recv_message);

            // 에러 메세지 출력
            ChaosSystem::Function::OutputResultStringBattleGround(message->result); // 전장 에러메세지 출력

        }
        void RECV_CG_GOLDRUSH_INITIATE_VOTE_BRD(MSG_BASE* recv_message)
        {
            // 전장 추방 투표 발의 BRD
            MSG_CG_GOLDRUSH_INITIATE_VOTE_BRD* message =
                static_cast<MSG_CG_GOLDRUSH_INITIATE_VOTE_BRD*>(recv_message);

            uiChaosZoneVote* chaos_zone_vote = GET_CAST_DIALOG(uiChaosZoneVote, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE);
            if (chaos_zone_vote)
            {
                chaos_zone_vote->set_character_name(message->van_payer_name);
                chaos_zone_vote->set_vote_remain_time(30000);
                chaos_zone_vote->ShowInterface(true);
            }
        }
        void RECV_CG_GOLDRUSH_VOTE_STATUS_BRD(MSG_BASE* recv_message)
        {
            // 골드러쉬 투표 현황
            MSG_CG_GOLDRUSH_VOTE_STATUS_BRD* message =
                static_cast<MSG_CG_GOLDRUSH_VOTE_STATUS_BRD*>(recv_message);

            uiChaosZoneVoteResult* chaos_zone_vote_result = GET_CAST_DIALOG(uiChaosZoneVoteResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE_RESULT);
            if (chaos_zone_vote_result)
            {
                chaos_zone_vote_result->set_character_name(message->van_payer_name);
                chaos_zone_vote_result->set_vote_yes_count(message->yes_count);
                chaos_zone_vote_result->set_vote_no_count(message->no_count);
                chaos_zone_vote_result->set_vote_remain_time(message->remain_vote_time);

                if (chaos_zone_vote_result->IsVisible() == FALSE)
                {
                    chaos_zone_vote_result->ShowInterface(true);
                }
            }
        }

        void RECV_CG_GOLDRUSH_VOTE_RESULT_BRD(MSG_BASE* recv_message)
        {
            // 전장 투표 결과
            MSG_CG_GOLDRUSH_VOTE_RESULT_BRD* message =
                static_cast<MSG_CG_GOLDRUSH_VOTE_RESULT_BRD*>(recv_message);

            TCHAR string_message[INTERFACE_STRING_LENGTH] = {0,};
            if (message->vote_result)
            {
                TCHAR format_message[INTERFACE_STRING_LENGTH] = {0,};
                // 2775 추방투표가 통과되었습니다. %s님이 전장에서 추방되었습니다.
                g_InterfaceManager.GetInterfaceString(2775, format_message, INTERFACE_STRING_LENGTH);

                Snprintf(string_message, INTERFACE_STRING_LENGTH, format_message, message->van_payer_name);

                // 킬메세지 형식으로 투표메세지도 출력해버림.                
                WORD van_player_team = ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(message->van_payer_name);
                DWORD van_user_color = (van_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);
                GameFramework::GetCurrentScene()->InsertKillMessage(
                    message->van_payer_name,
                    van_user_color,
                    "",
                    0,
                    format_message);                
            }
            else
            {
                // 2776 추방투표가 부결되었습니다. 1분간은 동일한 플레이어를 대상으로 추방투표를 할 수 없습니다.
                g_InterfaceManager.GetInterfaceString(2776, string_message, INTERFACE_STRING_LENGTH);
            }

            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, string_message);

            uiChaosZoneVote* chaos_zone_vote = GET_CAST_DIALOG(uiChaosZoneVote, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE);
            if (chaos_zone_vote)
            {
                if (chaos_zone_vote->IsVisible() == TRUE)
                {
                    chaos_zone_vote->ShowInterface(false);
                }
            }
            uiChaosZoneVoteResult* chaos_zone_vote_result = GET_CAST_DIALOG(uiChaosZoneVoteResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE_RESULT);
            if (chaos_zone_vote_result)
            {
                if (chaos_zone_vote_result->IsVisible() == TRUE)
                {
                    chaos_zone_vote_result->ShowInterface(false);
                }
            }

        }
        void RECV_CG_GOLDRUSH_VANP_PLAYER_CMD(MSG_BASE* recv_message)
        {
            // 골드러쉬 강제 추방 명령
            MSG_CG_GOLDRUSH_VANP_PLAYER_CMD* message =
                static_cast<MSG_CG_GOLDRUSH_VANP_PLAYER_CMD*>(recv_message);

            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (party_manager)
            {
                party_manager->LeaveParty();
            }

            // 나 강퇴
            uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_system_manager != NULL)
            {
                chaos_system_manager->WaitGague(true, 5, true);

                // 2782 %s님 추방됨
                TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceStringFormat(temp_string, INTERFACE_STRING_LENGTH, 2782, g_pHero->GetName());
                GlobalFunc::VerifyMessage(temp_string, NULL);
            }
        }

        void RECV_CG_GOLDRUSH_TEAM_INFO_CMD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_TEAM_INFO_CMD* message = 
                static_cast<MSG_CG_GOLDRUSH_TEAM_INFO_CMD*>(recv_message);

            if (g_pHero != NULL)
            {
                g_pHero->SetTeam(message->team_key);
            }
        }

        void RECV_CG_GOLDRUSH_TEAM_INFO_BRD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_TEAM_INFO_BRD* message = 
                static_cast<MSG_CG_GOLDRUSH_TEAM_INFO_BRD*>(recv_message);

            ChaosSystem::GetGoldRushData().SetGoldRushMemberInfo(message->member_count, message->member_list);
        }


        void RECV_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD* message = 
                static_cast<MSG_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD*>(recv_message);

            for (int i = 0; i < 2; ++i)
            {
                ChaosSystem::GetGoldRushData().set_team_score(message->team_info[i].team_key, 
                    message->team_info[i].total_gold_point);
            }
        }

        void RECV_CG_GOLDRUSH_STATE_RESURECTION_CMD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD* message = 
                static_cast<MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD*>(recv_message);

            if (g_pHero && (g_pHero->GetObjectKey() == message->player_object_key))
            {
                ChaosSystem::GetGoldRushData().set_player_state(message->player_state);

                // 부활 대기 상태일때는 시간 세팅
                if (message->player_state == kPlayerStateStandbyResurection)
                {
                    uiChaosZoneResurrection* chaos_zone_resurrection =
                        GET_CAST_DIALOG(uiChaosZoneResurrection, IM_CHAOS_SYSTEM_MANAGER::BATTLE_GROUND_RESURRECTION_POPUP);
                    if (chaos_zone_resurrection)
                    {
                        chaos_zone_resurrection->set_resurrection_tick(message->resurection_time);
                        chaos_zone_resurrection->ShowInterface(TRUE);
                    }
                }
            }

            Player* player = static_cast<Player*>(g_ObjectManager.GetObject(message->player_object_key));
            if (player)
            {
                // 상태에 따른 이동/투명 여부 세팅
                if ((message->player_state == kPlayerStateStandbyResurection) ||
                    (message->player_state == kPlayerStateStandbyResurectionRefuse))
                {
                    // 투명화
                    player->SetBattleGroundHideMode(true);
                }
                else
                {
                    // 투명화 끔
                    player->SetBattleGroundHideMode(false);
                }
            }
        }

        void RECV_CG_GOLDRUSH_FINISH_RESULT_BRD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_FINISH_RESULT_BRD* message =
                static_cast<MSG_CG_GOLDRUSH_FINISH_RESULT_BRD*>(recv_message);

            ChaosSystem::ChaosZoneData::ReleaseMutexHandle();

            uiChaosZoneResult* chaos_zone_result = GET_CAST_DIALOG(uiChaosZoneResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RESULT);
            if (chaos_zone_result)
            {
                chaos_zone_result->ShowChaosZoneResult(message->result_type);
            }

            // 골드러쉬 종료
            ChaosSystem::GetGoldRushData().EndGoldRush();
            uiBattleScoreMan* battle_score_manager_ptr = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
            if (battle_score_manager_ptr)
            {
                // 전장 나가기 버튼 활성화
                battle_score_manager_ptr->EnableGoldRushExitButton();
            }

            // 누구 팀이 승리하였습니다.
            TCHAR result_message_string[INTERFACE_STRING_LENGTH] = {0,};
            switch (message->result_type)
            {
            case kResultWin:
                {
                    // 2694 전장에서 승리 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2694, result_message_string);
                }
                break;
            case kResultLose:
                {
                    // 2696 전장에서 패배 하였습니다.
                    g_InterfaceManager.GetInterfaceString(2696, result_message_string);
                }
                break;
            case kResultDraw:
                {
                    // 2695 전장에서 승부를 가리지 못하고 무승부가 되었습니다.
                    g_InterfaceManager.GetInterfaceString(2695, result_message_string);
                }
                break;
            case kResultAbuse:
                {
                    // 
                    //g_InterfaceManager.GetInterfaceString(, result_message_string);
                }
                break;
            }
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, result_message_string);
        }

        void RECV_CG_GOLDRUSH_TIME_INFO_CMD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_TIME_INFO_CMD* message =
                static_cast<MSG_CG_GOLDRUSH_TIME_INFO_CMD*>(recv_message);

            // 골드러쉬 대기시간 돌입
            ChaosSystem::GetGoldRushData().StartGoldRush();

            if (message->elapsed_time == 0)
            {
                TCHAR notice_string[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR time_string[INTERFACE_STRING_LENGTH] = {0,};
                DWORD mapcode;

                mapcode = message->map_code;

                const BattleGroundInfo* battle_ground_info =
                    BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(mapcode));

                if (battle_ground_info != NULL)
                {
                    _itot(battle_ground_info->prepare_time, time_string, 10); // 몇초간 대기시간
                    // 2721 전투준비시간
                    g_InterfaceManager.GetInterfaceStringFormat(notice_string, INTERFACE_STRING_LENGTH-1, 2721, time_string);
                    GlobalFunc::default_OutputMsg(eOUTPUT_MSG_CENTER_SHOW | eOUTPUT_MSG_CHAT, notice_string);
                }
            }
        }

        void RECV_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD* message =
                static_cast<MSG_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD*>(recv_message);

            if (message == NULL)
            {
                return;
            }
            
            if (GameFramework::GetCurrentScene() &&
                (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_GOLDRUSH))
            {
                TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

                WORD kill_player_team = ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(message->kill_player_name);
                WORD death_player_team = ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(message->dead_player_name);
                DWORD kill_user_color = (kill_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);
                DWORD death_user_color = (death_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);                

                // 킬러 캐릭터가 몬스터가 아닐 때 killer_team에 정보를 넣는다.
                if (message->is_killer_monster == false)
                {
                    WORD steal_gold = message->steal_gold;
                    if (steal_gold > 0)
                    {                        
                        TCHAR message_string[INTERFACE_STRING_LENGTH] = {0, };
                        TCHAR steal_gold_string[INTERFACE_STRING_LENGTH] = {0,};
                        _itot(message->steal_gold, steal_gold_string, 10);
                        // 2792 %s님이 %s님으로부터 %s골드를 강탈하였습니다.
                        g_InterfaceManager.GetInterfaceStringFormat(message_string, INTERFACE_STRING_LENGTH-1, 2792, "%s", "%s", steal_gold_string);
                        // 스트링 테이블 요청
                        // %s님이 %s님으로부터 %s골드를 획득하였습니다.
                        GameFramework::GetCurrentScene()->InsertKillMessage(
                            message->kill_player_name,
                            kill_user_color,
                            message->dead_player_name,
                            death_user_color,
                            message_string);                     
                    }
                }
                ChaosSystem::GetGoldRushData().set_team_score_info(&(message->killer_team));
                ChaosSystem::GetGoldRushData().set_team_score_info(&(message->dead_team));
            }            
        }

        void RECV_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD( MSG_BASE* recv_message )
        {
            MSG_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD* message =
                static_cast<MSG_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD*>(recv_message);

            if (message == NULL)
            {
                return;
            }

            if (GameFramework::GetCurrentScene() &&
                (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_GOLDRUSH))
            {
                WORD kill_player_team = ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(message->kill_player_name);
                if (message->get_goldpoint > 0)
                {                    
                    if (message->is_boss_monster)
                    {
                        DWORD kill_user_color = (kill_player_team == kRed) ? WzColor_RGBA(255,102,204,255) : WzColor_RGBA(0,204,205,255);
                        DWORD death_user_color = WzColor_RGBA(255,255,255,255);

                        TCHAR message_string[INTERFACE_STRING_LENGTH] = {0, };
                        TCHAR gold_string[INTERFACE_STRING_LENGTH] = {0,};
                        //TCHAR dead_monster_name[MAX_MONSTERNAME_LENGTH+1] = {0, };
                        TCHAR dead_monster_name[INTERFACE_STRING_LENGTH] = {0, };
                        BASE_NPCINFO* pNpcInfo = NPCInfoParser::Instance()->GetNPCInfo(message->dead_monster_code);
                        if (pNpcInfo)
                        {
                            g_InterfaceManager.GetInterfaceString(pNpcInfo->m_NCode, dead_monster_name);
#ifdef _DEV_VER
                            //#ifdef _SCJ_SHOW_NPC_ID
                            if (CGeneralGameParam::Instance()->IsShowNpcId())
                            {
                                TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                                StrnCopy(buffer_string, dead_monster_name, INTERFACE_STRING_LENGTH-1); 
                                Snprintf(dead_monster_name, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                                    buffer_string, pNpcInfo->m_MonsterCode);
                            }
                            //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
                        }
                        _itot(message->get_goldpoint, gold_string, 10);
                        // 2793 %s님이 %s에게서 %s골드를 획득하였습니다.
                        g_InterfaceManager.GetInterfaceStringFormat(message_string, INTERFACE_STRING_LENGTH-1, 2793, "%s", "%s", gold_string);
                        // 스트링 테이블 요청
                        // %s님이 %s님으로부터 %s골드를 획득하였습니다.
                        GameFramework::GetCurrentScene()->InsertMonsterKillMessage(
                            message->kill_player_name,
                            kill_user_color,
                            dead_monster_name,
                            death_user_color,
                            message_string);
                    }
                }
                ChaosSystem::GetGoldRushData().set_team_score_info(&(message->killer_team));
            }
        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

        //------------------------------------------------------------------------------
        // 소넨샤인 관련
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void SEND_CG_SONNENSCHEIN_CREATE_SYN()
        {
            MSG_CG_SONNENSCHEIN_CREATE_SYN packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
        }
        void RECV_CG_SONNENSCHEIN_CREATE_ACK(MSG_BASE* recv_message)
        {
            // 카오스존에서 쓰는 게이지 돌리는게 있었지만, 소넨샤인의 경우
            // 게이지가 완료될 때 패킷을 던지기 위해 별도 게이지를 사용
            // 입장 패킷을 보내는게 생성패킷 보내고 딜레이캐스팅 후 입장패킷 보내게 된 이유는, 
            // 방이 생성되는 시간을 벌기 위함임

            MSG_CG_SONNENSCHEIN_CREATE_ACK* recv_packet =
                static_cast<MSG_CG_SONNENSCHEIN_CREATE_ACK*>(recv_message);

            if (recv_packet->result_code == RC::RC_SONNENSCHEIN_SUCCESS ||
                recv_packet->result_code == RC::RC_SONNENSCHEIN_ALEADY_CREATED)
            {
                uiGuildPvE* ui_guild_pve = GET_CAST_DIALOG(uiGuildPvE, uiGuildPvE::kDialog_ID);
                if (ui_guild_pve)
                {
                    ui_guild_pve->WaitGague(true);
                }
                ChaosSystem::GetChaosZoneData().set_is_wait(true);
                g_InterfaceManager.LockDlgsAll();
            }
            else
            {
                ChaosSystem::Function::OutputResultStringSonnenSchein(recv_packet->result_code);
            }
        }
        void SEND_CG_SONNENSCHEIN_ENTER_CHECK_SYN(SLOTCODE need_item)
        {
            MSG_CG_SONNENSCHEIN_ENTER_CHECK_SYN packet;
            packet.entrance_item_code = need_item;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
        }
        void RECV_CG_SONNENSCHEIN_ENTER_CHECK_ACK(MSG_BASE* recv_message)
        {
            MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK* recv_packet =
                static_cast<MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK*>(recv_message);
            
            if (recv_packet->result_code != RC::RC_SONNENSCHEIN_SUCCESS)
            {
                ChaosSystem::Function::OutputResultStringSonnenSchein(recv_packet->result_code);
            }

            // 성공이든 실패든 락은 풀어줌
            ChaosSystem::GetChaosZoneData().set_is_wait(false);
            if (g_InterfaceManager.IsLockDlg())
            {
                g_InterfaceManager.UnLockDlgs();
            }
        }
        void RECV_CG_SONNENSCHEIN_BEGIN_STAMP_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_SONNENSCHEIN_BEGIN_STAMP_BRD* recv_packet =
                static_cast<MSG_CG_SONNENSCHEIN_BEGIN_STAMP_BRD*>(recv_message);

            uiDominationStamp* stamp_dialog = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
            if (stamp_dialog && g_pHero)
            {
                stamp_dialog->SetStampID(recv_packet->stamp_id);
                stamp_dialog->ShowInterface(TRUE);

                stamp_dialog->set_stamp_guild_name(g_pHero->GetGuildName());
            }
        }
        void RECV_CG_SONNENSCHEIN_CANCEL_STAMP_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_SONNENSCHEIN_CANCEL_STAMP_BRD* recv_packet =
                static_cast<MSG_CG_SONNENSCHEIN_CANCEL_STAMP_BRD*>(recv_message);

            uiDominationStamp* stamp_dialog = GET_CAST_DIALOG(uiDominationStamp, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
            if (stamp_dialog && stamp_dialog->IsVisibleWindow())
            {
                stamp_dialog->ShowInterface(FALSE);
            }
        }
        void SEND_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN()
        {
            MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
        }
        void RECV_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK(MSG_BASE* recv_message)
        {
            uiGuildPvE* ui_guild_pve = GET_CAST_DIALOG(uiGuildPvE, uiGuildPvE::kDialog_ID);
            if (ui_guild_pve)
            {
                ui_guild_pve->SetGuildMemberList(recv_message);
            }
        }
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void SEND_CG_ONEDAYSGUILDMISSION_CREATE_SYN()
        {
            MSG_CG_ONEDAYSGUILDMISSION_CREATE_SYN packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
        }
        void RECV_CG_ONEDAYSGUILDMISSION_CREATE_ACK(MSG_BASE* recv_message)
        {
            MSG_CG_ONEDAYSGUILDMISSION_CREATE_ACK* recv_packet =
                static_cast<MSG_CG_ONEDAYSGUILDMISSION_CREATE_ACK*>(recv_message);

            if (recv_packet->result_code == RC::RC_ONEDAYSGUILDMISSION_SUCCESS ||
                recv_packet->result_code == RC::RC_ONEDAYSGUILDMISSION_ALEADY_CREATED)
            {
                uiGuildPvE* ui_guild_pve = GET_CAST_DIALOG(uiGuildPvE, uiGuildPvE::kDialog_ID);
                if (ui_guild_pve)
                {
                    ui_guild_pve->WaitGague(true);
                }
                ChaosSystem::GetChaosZoneData().set_is_wait(true);
                g_InterfaceManager.LockDlgsAll();
            }
            else
            {
                ChaosSystem::Function::OutputResultStringOneDaysGuildMission(recv_packet->result_code);
            }
        }
        void SEND_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN(SLOTCODE need_item)
        {
            MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN packet;
            packet.entrance_item_code = need_item;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
        }
        void RECV_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK(MSG_BASE* recv_message)
        {
            MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK* recv_packet =
                static_cast<MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK*>(recv_message);

            if (recv_packet->result_code != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
            {
                ChaosSystem::Function::OutputResultStringOneDaysGuildMission(recv_packet->result_code);
            }

            // 성공이든 실패든 락은 풀어줌
            ChaosSystem::GetChaosZoneData().set_is_wait(false);
            if (g_InterfaceManager.IsLockDlg())
            {
                g_InterfaceManager.UnLockDlgs();
            }
        }
        void RECV_CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD(MSG_BASE* recv_message)
        {

        }
        void RECV_CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD(MSG_BASE* recv_message)
        {

        }
        void SEND_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN()
        {
            MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
        }
        void RECV_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK(MSG_BASE* recv_message)
        {
            uiGuildPvE* ui_guild_pve = GET_CAST_DIALOG(uiGuildPvE, uiGuildPvE::kDialog_ID);
            if (ui_guild_pve)
            {
                ui_guild_pve->SetGuildMemberList_OneDaysGuildMission(recv_message);
            }
        }
        void RECV_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD(MSG_BASE* recv_message)
        {
            MSG_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD* recv_packet =
                static_cast<MSG_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD*>(recv_message);

            uiDominationTimer* domination_timer = GET_CAST_DIALOG(uiDominationTimer, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TIMER_DIALOG);
            if (domination_timer)
            {
                if(SONNENSCHEIN_EVENT_STATE_END == recv_packet->event_state_)
                {
                    if (TRUE == domination_timer->IsVisible())
                    {
                        domination_timer->ShowInterface(FALSE);
                    }
                    return;
                }

                if (FALSE == domination_timer->IsVisible())
                    domination_timer->ShowInterface(TRUE);

                domination_timer->SetTimer(recv_packet->remain_time_, true);
                if(SONNENSCHEIN_EVENT_STATE_WAIT == recv_packet->event_state_)
                {
                    domination_timer->SetStartTimer(false);
                }
                else if(SONNENSCHEIN_EVENT_STATE_START == recv_packet->event_state_)
                {
                    domination_timer->SetStartTimer(true);
                }
            }
        }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND