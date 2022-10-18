#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildMenu.h"
#include "GuildSystem/uiGuildWareHouse.h"
#include "GuildSystem/uiGuildCreate.h"
#include "GuildSystem/uiGuildMarkCombination.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/uiGuildMarkRegister.h"
#include "GuildSystem/uiGuildAuthority.h"
#include "GuildSystem/uiGuildDominationAreaInfo.h"
#include "GuildSystem/uiGuildDutyVest.h"
#include "GuildSystem/uiGuildPoint.h"
#include "GuildSystem/uiGuildRelation.h"
#include "GuildSystem/uiGuildRelationAsk.h"
#include "GuildSystem/uiGuildIntro.h"
#include "GuildSystem/uiGuildRanking.h"
#include "GuildSystem/uiGuildInfoOther.h"
#include "GuildMarkInfoParser.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "ResultCode.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiTargetMan/uiTargetMan.h"
#include "TargetDialog.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "uiGuildJoinRequestList.h"
#include "GuildFacilityImageParser.h"
#include "uiGuildInstitution.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

namespace GuildSystem
{
    
namespace Function
{
    // common functions
    bool IsGuildMember( const TCHAR* guild_name )
    {
        GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();

        if (guild_info.m_GuildGuid == 0)
        {
            return false;
        }

        if (StrnCmp(guild_name, guild_info.m_tszGuildName, MAX_CHARNAME_LENGTH) == 0)
        {
            return true;
        }

        return false;
    }

    void ShowGuildMenu()
    {
        uiBase* guild_menu = GET_DIALOG(uiGuildMenu::kDialog_ID);
        if (guild_menu != NULL)
        {
            guild_menu->ShowInterface(TRUE);
        }
    }

    void ShowGuildRanking()
    {
        //size_t ranking_container_size = 
        //    GuildSystem::GetGuildSystemData().guild_ranking_info_container().size();
        //if (ranking_container_size == 0)
        //{
        //    GuildSystem::PacketFunction::SEND_CG_ETC_GUILD_RANKINGINFO_SYN();

        //    return;
        //}
        GuildSystem::PacketFunction::SEND_CG_ETC_GUILD_RANKINGINFO_SYN();

        uiBase* guild_menu = GET_DIALOG(uiGuildRanking::kDialog_ID);
        if (guild_menu != NULL)
        {
            guild_menu->ShowInterface(TRUE);
        }
    }

    void ShowNoticeRegisterGuildMark()
    {
        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (system_manager != NULL)
        {
            TCHAR title_stinrg[INTERFACE_STRING_LENGTH] = {'\0', };
            // 6266 길드 마크
            g_InterfaceManager.GetInterfaceString(6266, title_stinrg);
            
            system_manager->Sys_SystemMessage(
                g_InterfaceManager.GetInterfaceString(6163), // 6163 길드 마크 등록 안내
                title_stinrg, 
                GuildSystem::Function::Callback_RegisterGuildMark, 
                uiSystemMessage::kConfirm_Type, 
                true, 
                0, 
                100, 
                1194, 
                1197);
        }
    }

    void ShowConfirmDeleteGuildMark()
    {
        // 6163 길드 마크 등록 안내
        GlobalFunc::ConfirmMessage(6168, 0, GuildSystem::Function::Callback_DeleteGuildMark);
    }

    void ShowConfirmKickGuildMember()
    {
        // 선택되어 있는 멤버 정보 얻어옴
        uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
        if (ui_guild_member == NULL)
        {
            return;
        }

        GUILD_MEMBER_INFO* kick_member_info = ui_guild_member->GetSelectedMemberInfo();
        if (kick_member_info == NULL)
        {
            return;
        }
        
        // 안내팝업
        TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
        //7220	길드원을 제명하게 되면 3일간 가입을 권유할 수 없습니다.%s 길드원을 정말 제명하시겠습니까?
        g_InterfaceManager.GetInterfaceStringFormat(message, 
            INTERFACE_STRING_LENGTH - 1, 
            7220, 
            kick_member_info->m_tszCharName);
        GlobalFunc::ConfirmMessage(message, NULL, Callback_KickGuildMember);
    }

    void ShowConfirmWithdrawGuild()
    {
        if (g_pHero == NULL)
        {
            return;
        }

        if (GuildSystem::GetGuildSystemData().is_recv_guild_info() == false)
        {
            // 5165 길드에 가입되어 있지 않습니다.
            GlobalFunc::VerifyMessage(5165);
        }
        else
        {
            // 6209 길드를 탈퇴하게 되면 3일간 길드 가입이 가능하지 않습니다. 정말 탈퇴하시겠습니까?
            GlobalFunc::ConfirmMessage(6209, 0, GuildSystem::Function::Callback_WithdrawGuild);
        }
    }

    void ShowConfirmDelegateMaster()
    {
        if (g_pHero == NULL)
        {
            return;
        }

        uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
        if (ui_guild_member == NULL)
        {
            return;
        }

        GUILD_MEMBER_INFO* member_info = ui_guild_member->GetSelectedMemberInfo();
        if (member_info == NULL)
        {
            return;
        }

        if (StrnCmp(g_pHero->GetName(), member_info->m_tszCharName, MAX_CHARNAME_LENGTH) == 0)
        {
            //5197 자신에게는 이용할수 없는 기능입니다.
            GlobalFunc::VerifyMessage(5197);
            return;
        }

        // 안내팝업
        TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
        //6214	길드장이 1번 변경하면 14일간 길드장 위임이 가능하지 않습니다. \n%s님으로 길드장 위임을 하시겠습니까?
        g_InterfaceManager.GetInterfaceStringFormat(message, 
            INTERFACE_STRING_LENGTH - 1, 
            6214, 
            member_info->m_tszCharName);
        GlobalFunc::ConfirmMessage(message, NULL, Callback_DelegateMaster);
    }

        void RenderGuildMark( DWORD mark_info_index, RECT& draw_rect )
    {
        RenderGuildMark(
            GuildMarkInfoParser::Instance()->GetGuildMarkInfo(mark_info_index), 
            draw_rect);
    }

    void RenderGuildMark( BASE_GuildMarkInfo* mark_info, RECT& draw_rect )
    {
        if (mark_info == NULL)
        {
            return;
        }

        HANDLE guild_mark_texture = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
        if (guild_mark_texture == INVALID_HANDLE_VALUE)
        {
            return;
        }

        ENUM_ALPHABLEND old_alphablend_mode = g_pSunRenderer->GetAlphaBlendMode();
        g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

        if (mark_info)
        {
            g_pSunRenderer->RenderTexture(
                guild_mark_texture , 
                static_cast<float>(draw_rect.left), 
                static_cast<float>(draw_rect.top), 
                static_cast<float>(draw_rect.right - draw_rect.left), 
                static_cast<float>(draw_rect.bottom - draw_rect.top), 
                static_cast<float>(mark_info->iRow) * 32.0f, 
                static_cast<float>(mark_info->iColumn) * 32.0f, 
                32.0f, 
                32.0f);
        }	

        g_pSunRenderer->SetAlphaBlendMode(old_alphablend_mode);
    }

    void ShowNoticeUseGuildWareHouse()
    {
        // 1223 길드 창고 이용 방법
        // 6194 창고 이용 관련 안내문
        GlobalFunc::VerifyMessage(6194, 1223, GuildSystem::Function::Callback_UseGuildWareHouse, true);
    }

    eGUILD_DELCHK GetGuildDestroyFlag()
    {
        eGUILD_DELCHK guild_destroy_flag = 
            GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildDestroyWaitFlag;

        return guild_destroy_flag;
    }

    void ToggleGuildInfoDialog()
    {
        if (g_pHero == NULL)
        {
            return;
        }

        if (g_pHero->IsExistGuild() == TRUE)
        {
            uiGuildInfo* ui_guild_info = GET_CAST_DIALOG(uiGuildInfo, uiGuildInfo::kDialog_ID);
            if (ui_guild_info != NULL)
            {
                if (ui_guild_info->IsVisible() == TRUE)
                {
                    ui_guild_info->ShowInterface(FALSE);
                }
                else
                {
                    if (GuildSystem::Function::GetGuildDestroyFlag() == eGUILD_DELCHK_DESTROY_WAIT)
                    {
                        uiBase* ui_base = GET_CAST_DIALOG(uiBase, GuildSystem::GetGuildSystemData().verify_dialog_key());
                        if (ui_base != NULL)
                        {
                            ui_base->SetDestroyWindow(TRUE);
                        }
                        else
                        {
                            //6236	길드 해산 대기 중입니다.
                            GuildSystem::GetGuildSystemData().verify_dialog_key() = GlobalFunc::VerifyMessage(6236);
                        }
                    }
                    else if (GuildSystem::GetGuildSystemData().is_recv_total_info() == true)
                    {
                        ui_guild_info->ShowInterface(TRUE);
                    }
                    else if (g_pHero->IsExistGuild() == TRUE)
                    {
                        ui_guild_info->set_show_flag(true);
                        GuildSystem::PacketFunction::SEND_CG_GUILD_TOTALINFO_REQ_SYN();
                    }
                }
            }
        }
        else
        {
            uiBase* ui_guild_ranking = GET_DIALOG(uiGuildRanking::kDialog_ID);
            if (ui_guild_ranking != NULL)
            {
                if (ui_guild_ranking->IsVisible() == FALSE)
                {
                    GuildSystem::Function::ShowGuildRanking();
                }
                else
                {
                    ui_guild_ranking->ShowInterface(FALSE);
                }
            }
            
        }
    }

    void ClearPlayerRelation()
    {
        //길드를 탈퇴했으므로 관계를 초기화 한다.
        Object_Map_Itr object_itr = g_ObjectManager.GetBegin();
        while (g_ObjectManager.GetEnd() != object_itr)
        {
            Object* object = object_itr->second;

            if ((object != NULL) && 
                (object->IsKindOfObject(PLAYER_OBJECT) == TRUE))
            {
                Player* player = static_cast<Player*>(object);
                if (player != NULL)
                {
                    player->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_NONE;
                }
            }

            ++object_itr;
        }
    }

    void CloseGuildWareHouse()
    {
        uiGuildWareHouse* guild_warehouse = GET_CAST_DIALOG(uiGuildWareHouse, uiGuildWareHouse::kDialog_ID);
        if (guild_warehouse != NULL)
        {
            guild_warehouse->SendWareHouseEnd();
        }
    }

    void CloseAllGuildDialog()
    {
        uiBase* guild_dialog = GET_DIALOG(uiGuildMenu::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildInfo::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildCreate::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildMarkCombination::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildMarkRegister::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildAuthority::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildDominationAreaInfo::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildDutyVest::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildPoint::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildRelation::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }

        guild_dialog = GET_DIALOG(uiGuildRelationAsk::kDialog_ID);
        if (guild_dialog != NULL)
        {
            guild_dialog->ShowInterface(FALSE);
        }
    }

    void InvitePlayer( const TCHAR* player_name )
    {
        if (GuildSystem::GetGuildSystemData().is_recv_guild_info() == false)
        {
            // 5165 길드에 가입되어 있지 않습니다.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(5165));

            return;
        }

        GuildSystem::PacketFunction::SEND_CG_GUILD_INVITE_SYN(player_name);
    }

    void InviteTargetPlayer()
    {
        if (g_pHero == NULL)
        {
            return;
        }

        if (GuildSystem::GetGuildSystemData().is_recv_guild_info() == false)
        {
            // 5165 길드에 가입되어 있지 않습니다.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(5165));

            return;
        }

        TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        if (target_dialog == NULL)
        {
            return;
        }

        if (target_dialog->IsVisible() == FALSE)
        {
            // 70158 선택된 플레이어가 없습니다.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(70158));

            return;
        }

        DWORD target_key = target_dialog->GetTargetKey();
        Object* target_object = g_ObjectManager.GetObject(target_key);
        if ((target_object == NULL) || 
            (target_object->IsKindOfObject(PLAYER_OBJECT) == FALSE))
        {
            return;
        }

        GuildSystem::PacketFunction::SEND_CG_GUILD_INVITE_SYN(target_object->GetName());
    }

    void SetPlayerGuildRelation( GUILDGUID guild_guid, eGUILD_RELATION guild_relation )
    {
        Object_Map_Itr object_itr = g_ObjectManager.GetBegin();
        Object_Map_Itr object_end_itr = g_ObjectManager.GetEnd();
        for ( ; object_itr != object_end_itr; ++object_itr)
        {
            Object* object_ptr = (object_itr->second);

            if (object_ptr == NULL)
            {
                continue;
            }

            if (object_ptr->IsKindOfObject(PLAYER_OBJECT) == FALSE)
            {
                continue;
            }

            Player* player_ptr = static_cast<Player*>(object_ptr);

            if (player_ptr->GetPlayerGuildID() == guild_guid)
            {
                player_ptr->GetGuildRenderInfo().m_RelationType = guild_relation;

                player_ptr->CheckDominationRelation();

            }
        }
    }

    void ClearGuildRelation()
    {
        Object_Map_Itr object_itr = g_ObjectManager.GetBegin();
        Object_Map_Itr object_end_itr = g_ObjectManager.GetEnd();
        for ( ; object_itr != object_end_itr; ++object_itr)
        {
            Object* object_ptr = (object_itr->second);

            if (object_ptr == NULL)
            {
                continue;
            }

            if (object_ptr->IsKindOfObject(PLAYER_OBJECT) == FALSE)
            {
                continue;
            }

            Player* player_ptr = static_cast<Player*>(object_ptr);
            player_ptr->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_NONE;
        }
    }

    void ProcessErrorCode( DWORD error_code, bool is_popup /*= true*/ )
    {
        using namespace RC;
        int string_code = 0;

        switch (error_code)
        {
        case RC_GUILD_CANNOT_CREAT_GUILD_STATE:
            //5151	길드를 창설할 수 없습니다.
            string_code = 5151;
            break;

        case RC_GUILD_CANNOT_CREAT_LEVEL_STATE:
            //5152	레벨이 낮아 길드를 창설할 수 없습니다.
            string_code = 5152;
            break;

        case RC_GUILD_CANNOT_CREAT_MONEY_STATE:
            //5153	하임이 부족하여 길드를 창설할 수 없습니다.
            string_code = 5153;
            break;

        case RC_GUILD_CANNOT_DESTROY_STATE:
            //5154	길드를 해체할 수 없습니다.
            string_code = 5154;
            break;

        case RC_GUILD_DESTROY_WAIT_CANNOT_NONE_RIGHT:
        case RC_GUILD_CANNOT_DESTROY_NOT_MASTER_STATE:
            //5154	길드를 해체할 수 없습니다.
            string_code = 5154;
            break;

        case RC_GUILD_DESTROY_WAIT_CANNOT_MEMBER_EXIST:
        case RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE:
            //5155	길드원이 존재할 경우 길드를 해체할 수 없습니다.
            string_code = 5155;
            break;

        case RC_GUILD_NOT_EXIST_TARGET_STATE:
            //7318 길드초대에 실패하였습니다.\n캐릭터명 확인 후 다시 시도해 주십시오.
            string_code = 7318;
            break;

        case RC_GUILD_CANNOT_INVITE_STATE:
            //5157	길드에 초대할 수 없는 상태입니다.
            string_code = 5157;
            break;

        case RC_GUILD_DONT_HAVE_RIGHTS_STATE:
            //5158	길드 초대 권한이 없습니다.
            string_code = 5158;
            break;

        case RC_GUILD_FULL_MEMBER_STATE:
            //5159	더 이상 길드원을 추가할 수 없습니다. 길드 레벨을 확인해 주십시오.
            string_code = 5159;
            break;

        case RC_GUILD_NOT_EXIST_HOST_STATE:
            //5157	길드에 초대할 수 없는 상태입니다.
            string_code = 5157;
            break;

        case RC_GUILD_CANNOT_WITHDRAW_STATE:
            //5164	길드 관련한 알 수 없는 결과를 받았습니다.
            string_code = 5164;
            break;

        case RC_GUILD_NOT_EXIST_GUILD:
            //5160	존재하지 않는 길드입니다.
            string_code = 5160;
            break;

        case RC_GUILD_CANNOT_JOIN_STATE:
            //5161	길드에 가입할 수 없습니다.
            string_code = 5161;
            break;

        case RC_GUILD_CANNOT_SELECT_GUILD_STATE:
            //5162	길드 정보를 불러오지 못했습니다.
            string_code = 5162;
            break;

        case RC_GUILD_WAIT_A_WHILE_THEN_TRY_AGAIN:
            //5163	잠시 후 다시 시도해 주시기 바랍니다.
            string_code = 5163;
            break;

        case RC_GUILD_CANNOT_KICK_STATE:
            //5168	길드원 제명에 실패했습니다.
            string_code = 5168;
            break;

        case RC_GUILD_CANNOT_KICK_RIGHT:
            //5167	길드원 제명 권한이 없습니다.	
            string_code = 5167;
            break;

        case RC_GUILD_NOT_ENOUGH_UP:
            // UP(유저포인트)가 부족합니다. 5185
            string_code = eST_NOT_ENOUGH_UP;
            break;

        case RC_GUILD_NOT_ENOUGH_GP:
            // GP(길드포인트)가 부족합니다. 5186
            string_code = eST_NOT_ENOUGH_GP;
            break;

        case RC_GUILD_CANNOT_RANKUP_MONEY_STATE:
            // 하임이 부족 합니다. : 551
            string_code = eST_NOT_ENOUGH_MONEY;
            break;

        case RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE:
            // 더이상 랭크 업 할수 없습니다.
            string_code = 5195;
            break;

        case RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE:
            // 5275	카오 상태에서는 이용이 가능하지 않습니다.
            string_code = 5275;
            break;

        case RC_GUILD_MARK_INVALID_TYPE:
            // 7397 길드 마크 형식에 어긋남
            string_code = 7397;
            break;

        case RC_GUILD_MARK_CREATE_FAILED:
            // 1200 길드 마크 생성 실패
            string_code = 1200;
            break;

        case RC_GUILD_CURRENT_NONE_MASTER:
            //6175 길드 마스터가 존재하지 않습니다.
            string_code = 6175;
            break;

        case RC_GUILD_RELATION_REMAIN_GUILD:
        case RC_GUILD_DESTROY_WAIT_CANNOT_RELATION_REMAIN:
            //7195	길드관계가 남아있어서 길드를 해체할 수 없습니다.
            string_code = 7195;
            break;

        case RC_GUILD_RELATION_LIST_ERROR:
            //7176	길드관계의 정보가 잘못되었습니다.
            string_code = 7176;
            break;

        case RC_GUILD_RELATION_ME_FULL:
            //7177	해당 길드관계의 제한 수치를 초과하여 더 이상 해당 길드관계를 맺을 수 없습니다.
            string_code = 7177;
            break;

        case RC_GUILD_RELATION_OTHER_FULL:
            //7178	상대방 길드가 해당 길드관계의 제한 수치를 초과하여 더이상 해당 길드관계를 맺을 수 없습니다.
            string_code = 7178;
            break;

        case RC_GUILD_RELATION_CHANGE_RIGHT_NOT:
            //7179	길드 관계를 설정할 권한이 없습니다.
            string_code = 7179;
            break;

        case RC_GUILD_RELATION_ALEADY_SET:
            //7180	길드 관계를 맺으려는 길드와 이미 다른 관계가 성립되어 있습니다.
            string_code = 7180;
            break;

        case RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED:
            //7181	동맹을 맺으려는 길드가 자신의 길드와 적대 관계를 맺고 있는 길드와 동맹관계라 동맹 관계를 맺을 수 없습니다.
            string_code = 7181;
            break;

        case RC_GUILD_RELATION_ALLIENCE_TYPE2_FAILED:
            //7182	동맹을 맺으려는 길드가 자신의 길드와 동맹을 맺고 있는 길드와 적대 관계라 동맹 관계를 맺을 수 없습니다.
            string_code = 7182;
            break;

        case RC_GUILD_RELATION_HOSTILITY_FAILED:
            //7194	적대를 맺으려는 길드가 자신의 길드와 동맹 관계를 맺고 있는 길드와 동맹 관계여서 적대 관계를 맺을 수 없습니다.
            string_code = 7194;
            break;

        case RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED:
            //7183	상대 길드와 자신의 길드가 일방적 적대 상태 또는 적대 선포 관계가 아니기 때문에 쌍방 적대 관계를 맺을 수 없습니다.
            string_code = 7183;
            break;

        case RC_GUILD_RELATION_NONE_DELETE_NOT:
            //7184	상대 길드와 어떠한 관계도 맺고 있지 않기 때문에 관계를 삭제를 할 수 없습니다.
            string_code = 7184;
            break;

        case RC_GUILD_RELATION_GUILD_DESTROY_WAIT:
            //7185	길드 해산 대기중이라 길드 관계를 맺을 수 없습니다.
            string_code = 7185;
            break;

        case RC_GUILD_RELATION_GUILD_GRADE_LOW_ME:
            //7215	길드 레벨이 낮아 관계를 맺을 수 없습니다.
            string_code = 7215;
            break;

        case RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER:
            //7216	상대방 길드의 레벨이 낮아 관계를 맺을 수 없습니다.
            string_code = 7216;
            break;

        case RC_GUILD_RELATION_DELETE_FAILED: //길드 관계 삭제 실패
            //7219	길드 관계 삭제에 실패했습니다.
            string_code = 7219;
            break;

        case RC_GUILD_RELATION_DELETE_FAILED_BY_TIME:
            //7239 적대관계를 신청한 후 1시간이 지나야 해제가 가능합니다.
            string_code = 7239;
            break;
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        case RC_GUILD_RELATION_CHANGE_FAILED_DOMINATION:
            //7398
            string_code = 7398;
            break;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        case RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE:
            //7186	길드 등급이 낮아 길드 창고를 이용할 수 없습니다.
            string_code = 7186;
            break;

        case RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER:
            //7187	길드 창고를 사용중인 유저가 있습니다.
            string_code = 7187;
            break;

        case RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD:
            //7188	유저의 길드창고 정보와 실제 길드의 길드 창고 정보가 일치하지 않습니다.
            string_code = 7188;
            break;

        case RC_GUILD_WAREHOUSE_INVENMONEY_LOW:
            ////551 하임이 부족합니다.
            string_code = 551;
            break;

        case RC_GUILD_WAREHOUSE_MONEYOUT_NOENRIGHT:
            //7192	길드 창고의 하임을 출금할 권한이 없습니다.
            string_code = 7192;
            break;

        case RC_GUILD_WAREHOUSE_INVENMONEY_LIMIT:
            //7193	하임 한계치를 초과하였습니다.
            string_code = 7193;
            break;

        case RC_GUILD_DESTROY_WAIT_CANNOT_GUILDWAREHOUSE_ITEM_REMAIN:
            //7237	길드창고에 아이템 또는 하임이 남아있어 길드 해산 신청을 할 수 없습니다.
            string_code = 7237;
            break;

        case RC_GUILD_CANNOT_WITHDRAW_BY_WAREHOUSE_USEING:	
            //7243	길드 창고 사용중이여서 탈퇴/제명을 할 수가 없다.
            string_code = 7243;
            break;


        case RC_GUILD_DESTROY_PENALTY_TIME:
            //6213	길드가 해산된 후 3일이 지나지 않아 길드 가입 및 창설을 할 수 없습니다.
            string_code = 6213;
            break;

        case RC_GUILD_WITHDRAW_PENALTY_TIME:
        case RC_GUILD_KICK_PENALTY_TIME:
            //6210	길드를 탈퇴하거나 제명 당한 지 3일이 지나지 않아 길드에 가입할 수 없습니다.
            string_code = 6210;
            break;

        case RC_GUILD_RECOVERY_FAILED:
            //6235	길드 해산 해제 신청에 실패하였습니다.
            string_code = 6235;
            break;

        case RC_GUILD_DELCHK_DESTROY_WAIT:
            //6236	길드 해산 대기 중입니다.
            string_code = 6236;
            break;

        case RC_GUILD_NOTICE_CHNAGE_NONE_RIGHT:
            //7206 길드등급 레벨이 낮거나,공지 변경 권한이 없습니다
            string_code = 7206;
            break;

        case RC_GUILD_ALREADY_EXIST_DUTY:
            //6253	이미 해당 직책을 가지고 있는 길드원이 있습니다.
            string_code = 6253;
            break;
        case RC_GUILD_HAVE_NOT_RIGHT:
            //6252	길드 권한이 없습니다.
            string_code = 6252;
            break;
        case RC_GUILD_CANNOT_COMMAND_MYSELF:
            //5197	자신에게는 이용할 수 없는 기능입니다.
            string_code = 5197;
            break;
        case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY:
            //6251	현재 직책과 바꾸려는 직책이 같아서 직책을 바꿀수 없습니다.
            string_code = 6251;
            break;
        case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_INVALID_DUTY:
            //6195	상위 직책에 있는 길드원에게는 직책을 부여할 수 없습니다.
            string_code = 6195;
            break;
        case RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER:
            //6271 해당 기능을 이용할 권한이 없습니다.
            string_code = 6271;
            break;
        case RC_GUILD_INVALID_DELEGATE_RIGHT:
            //6286	정상적인 권한 위임이 아닙니다.
            string_code = 6286;
            break;
        case RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER:
        case RC_GUILD_PLAYER_ALEADY_DUTY:
            //6223 해당 직책을 가진 길드원이 모두 존재하여 더 이상 변경할 수 없습니다.
            string_code = 6223;
            break;
        case RC_GUILD_CHANGE_MEMBER_DUTY_FAILED:
            //6287	직책 변경에 실패하였습니다. 다시 시도해 주시기 바랍니다.
            string_code = 6287;
            break;
        case RC_GUILD_CHANGE_MEMBER_RIGHT_FAILED:
            //6288	권한 위임에 실패하였습니다.
            string_code = 6288;
            break;
        case RC_GUILD_CANNOT_CHANGE_MASTER_IN_DAY_14:
            //6222	길드장을 변경한 지 14일이 경과하지 않아 길드장 위임이 가능하지 않습니다.
            string_code = 6222;
            break;
        case RC_GUILD_CANNOT_CHANGE_DUTY_BY_WAREHOUSE_USEING:
            //7242	길드 창고를 사용 중인 길드원은 직책 변경이 가능하지 않습니다.
            string_code = 7242;
            break;

        case RC_GUILD_SELECT_CHAR_NO_GUILD_MEMBER:
            //6200	입력한 아이디가 정확하지 않습니다. 다시 확인 후 입력하시기 바랍니다.
            string_code = 6200;
            break;
        case RC_GUILD_CREATE_CORPS_FAILED:
            //6289	길드 군단 생성에 실패하였습니다.
            string_code = 6289;
            break;
        case RC_GUILD_CORPS_NUM_OVERFLOW:
            //6199	더 이상 군단을 창설할 수 없습니다.
            string_code = 6199;
            break;
        case RC_GUILD_MEMBER_NOT_HEAD_CORPS:
            //6290	해당 길드원은 직할 군단 소속이 아닙니다.
            string_code = 6290;
            break;
        case RC_GUILD_NOT_SOLDIER:
        case RC_GUILD_MEMBER_NOT_SOLDIER:
            //7161	해당 길드원은 정예군이 아닙니다.
            string_code = 7161;
            break;
        case RC_GUILD_CORPS_NOT_FOUND:
            //7163	해당 길드에 군단이 생성되지 않았습니다.	
            string_code = 7163;
            break;
        case RC_GUILD_ALREADY_GUILD_MEMBER:
            //7164	이미 길드에 가입되어 있습니다.
            string_code = 7164;
            break;
        case RC_GUILD_ALREADY_EXIST_COMMANDER:
            //7166	이미 길드에 해당 군단장이 존재합니다.
            string_code = 7166;
            break;
        case RC_GUILD_ALREADY_EXIST_TRAINING_LEADER:
            //7167	이미 길드에 해당 훈련대장이 존재합니다.
            string_code = 7167;
            break;
        case RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER:
            //7168	동일한 직책으로는 변경할 수 없습니다.
            string_code = 7168;
            break;
        case RC_GUILD_CORPS_FULL_MEMBER:
            //7169	해당 군단에 인원이 꽉 차 더 이상 이동 및 가입할 수 없습니다.
            string_code = 7169;
            break;
        case RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER:
            //7170	서버 연결이 해제되어 해당 길드 작업에 실패하였습니다. 다시 시도해 주시기 바랍니다.
            string_code = 7170;
            break;
        case RC_GUILD_INVALID_DUTY_IN_BASE_CORPS:
            //7171	직할 군단에는 존재하지 않는 직책입니다.	
            string_code = 7171;
            break;
        case RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS:
            //6237	선택한 군단은 이미 길드원이 소속되어 있는 군단입니다.
            string_code = 6237;
            break;
        case RC_GUILD_ALREADY_EXIST_REAR_CAMP:
            //6201	군단 애 육성캠프가 존재하여 더 이상 창설할 수 없습니다.
            string_code = 6201;
            break;
        case RC_GUILD_NOT_EXIST_REAR_CAMP:
            //7172	해당 군단에 육성캠프가 존재하지 않습니다.
            string_code = 7172;
            break;
        case RC_GUILD_CANNOT_LEAVE_MASTER:
            //7173	길드장은 길드 탈퇴를 할 수 없습니다.
            string_code = 7173;
            break;
        case RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS:
            //7174	직할군단에서는 사용할 수 없는 기능입니다.
            string_code = 7174;
            break;
        case RC_GUILD_EXIST_MEMBER:
            //6203	군단 내 길드원이 있으면 해산할 수 없습니다.
            string_code = 6203;
            break;
        case RC_GUILD_ALREADY_EXIST_GUILD:
            //7214	이미 길드가 존재합니다.
            string_code = 7214;
            break;
        case RC_GUILD_CANNOT_LEVELUP:
            //5182	길드 랭크업에 필요한 조건을 확인해 주시기 바랍니다.
            string_code = 5182;
            break;
        case RC_GUILD_NOT_CORPS_MEMBER:
            //7212	군단에 속한 멤버가 아닙니다.
            string_code = 7212;
            break;
        case RC_GUILD_NOT_BASE_CORPS:
            //7213	직할군단이 아니여서 이용할 수 없습니다.
            string_code = 7213;
            break;

        case RC_GUILD_CANNOT_MOVE_CORPS_BY_CORPS_PENALTY:
            //7240 해당군단은 길드원 제명 패널티로 인해 군단간 이동이 가능하지 않습니다.
            string_code = 7240;
            break;


        case RC_GUILD_NOT_GUILD_MEMBER:
            //7205	길드 멤버가 아닙니다.
            string_code = 7205;
            break;

        case RC_GUILD_NOT_EXIST_GUILD_SEAL:
            //6161 길드 인장이 인벤토리에 존재하지 않습니다.
            string_code = 6161;
            break;

        case RC_GUILD_CHANGE_MARK_NOT_RIGHT:
            //7202	길드 마크를 설정할 수 있는 권한이 없습니다.
            string_code = 7202;
            break;

        case RC_GUILD_CHANGE_MARK_NOT_DUTY_STATE:
            //7203	해당 기능을 이용할 수 없는 직책입니다.
            string_code = 7203;
            break;
        case RC_GUILD_MARK_LEVEL_LOW:
            //7201	길드 레벨이 낮아 길드 마크를 등록할수 없습니다.
            string_code = 7201;
            break;
        case RC_GUILD_MARK_ALEADY_EXIST:
            //7200	이미 등록되어 있는 길드 마크입니다.
            string_code = 7200;
            break;

        case RC_GUILD_MARK_DELETE_FAILED:
            //7218	길드 마크 삭제에 실패했습니다.
            string_code = 7218;
            break;

        case RC_GUILD_CANNOT_CREAT_SAMENAME_STATE:
            // 7175 동일한 이름의 길드가 있습니다. 다시 입력해 주십시오.
            string_code = 7175;
            break;

        case RC_GUILD_KICK_GUILD_BY_LEVELUP:
            //31레벨에 달성하여 육성 캠프에서 강제 탈퇴되었습니다.
            string_code = 7197;
            break;
        case RC_GUILD_NOT_JOIN_GUILD:
            //5165	길드에 가입되어 있지 않습니다.
            string_code = 5165;
            break;
        case RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL:
            //7232	16레벨 이상의 캐릭터는 훈련병으로 가입할 수 없습니다.
            string_code = 7232;
            break;
        case RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL:
            //7211 15레벨 이하만 훈련병으로 변경할 수 있습니다
            string_code = 7211;
            break;
        case RC_GUILD_CORPS_PENALTY_TIME:
            //7238 길드원을 제명하여 해당 군단의 가입 권유를 3일간 할 수 없습니다.
            string_code = 7238;
            break;

        case RC_GUILD_NAME_CHANGE_CANNOT_GRADE:
            //7250 길드명 변경 권한이 없습니다.
            string_code = 7250;
            break;

        case RC_GUILD_NAME_CHANGE_CANNOT_DB:
        case RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM:
            //7251 길드명 변경에 실패 했습니다.
            string_code = 7251;
            break;

        case RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME:
            // 7175 동일한 이름의 길드가 있습니다. 다시 입력해 주십시오.
            string_code = 7175;
            break;

        case RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE:
        case RC_GUILD_NAME_BAN_WORD:
            //70022 사용이 금지된 단어입니다.
            string_code = eST_CANNOT_USE_WORD;
            break;

        case RC_GUILD_CANNOT_WITHDRAW_BY_DOMINATIONFIELD:
            // 3343	피의 성전 내부에서는 길드탈퇴가 불가능하여 피의성전 입구로 이동합니다. 재시도 바랍니다.
            string_code = 3343;
            break;
        case RC_GUILD_CANNOT_KICK_BY_DOMINATIONFIELD:
            //3378 현재 해당 길드원이 피의성전 내부에 있어 제명을 위하여 피의성전 입구로 강제 이동 시킵니다. 재시도 바랍니다.
            string_code = 3378;
            break;
        case RC_GUILD_DOMINATION_ACTIONINFO_CANNOT_DESTROY:
            // 3373	'피의 성전' 점령전 입찰 기록이 존재하여 길드해체가 불가능 합니다.
            string_code = 3373;
            break;
        case RC_GUILD_DOMINATION_INFO_CANNOT_DESTROY:
            // 3381	피의 성전' 점령전에 참여 중인 길드는 길드해체가 불가능 합니다.
            string_code = 3381;
            break;

        case RC_GUILD_WAREHOUSE_GET_CANNOT_DB:
        default:
            {
                //5164	길드 관련한 알 수 없는 결과를 받았습니다.
                string_code = 5164;
            }
            break;
        }

        if (is_popup == true)
        {
            GlobalFunc::VerifyMessage(string_code, 0);
        }
        else
        {
            GlobalFunc::OutputMsg_Notice(
                eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(string_code));
#ifdef _DEV_VER
            TCHAR error_message[INTERFACE_STRING_LENGTH] = {'\0', };
            Snprintf(error_message, INTERFACE_STRING_LENGTH-1, _T("[GUILD] ERROR_CODE [%d]") , error_code);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, error_message);
#endif
        }
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void ProcessGuildFacilityResultCode( DWORD result_code, bool is_popup /*= true*/ )
    {        
        int string_index = 0;
        switch (result_code)
        {
        case RC::RC_GUILD_SUCCESS:
            {
                // 7109 : 업그레이드가 완료되었습니다.
                string_index = 7109;
            }
            break;
        case RC::RC_GUILD_NOT_EXIST_GUILD:
            {
                // 7300 : 길드정보가 없습니다.
                string_index = 7300;
            }
            break;
        case RC::RC_GUILD_HAVE_NOT_RIGHT:
            {
                // 3347 : 해당 기능에 대한 권한이 없습니다.
                string_index = 3347;
            }
            break;
        case RC::RC_GUILD_NOT_ENOUGH_GP:
            {
                // 7111 : 길드포인트가 없어 업그레이드를 할 수 없습니다.
                string_index = 7111;
            }
            break;
        case RC::RC_GUILD_NOT_EXIST_FACILITY:
            {
                // 7116 : 해당시설은 개발되어 있지 않습니다.
                string_index = 7116;
            }
            break;
        case RC::RC_GUILD_NOT_ENOUGH_UPGRADE_ITEM:
            {
                // 7117 : 길드 시설 업그레이드 아이템이 부족하다.
                string_index = 7117;
            }
            break;
        case RC::RC_GUILD_NOT_EXIST_PLAYER:
            {
                // 7123 : 플레이어를 찾을 수 없습니다.
                string_index = 7123;
            }
            break;
        case RC::RC_GUILD_NOT_EXIST_FACILITY_ACTIVE:
            {
                // 7124 : 존재하지 않는 길드시설입니다.
                string_index = 7124;
            }
            break;
        case RC::RC_GUILD_NOT_ENOUGH_ACTIVE_ITEM:
            {
                // 7125 : 활성화에 필요한 아이템이 부족합니다.
                string_index = 7125;
            }
            break;
		case RC::RC_GUILD_NOT_BE_ACTIVATED_IN_RIDE:
        case RC::RC_GUILD_FACILITY_CANNOT_ACTIVATED_FLYING:
			{
				// 7093 : 탈 것을 탄 상태에서는 길드시설을 활성화 할 수 없습니다.
				string_index = 7093;
			}
			break;
        }

        if (string_index != 0)
        {
            TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(string_index, result_message);
            if (is_popup)
            {
                GlobalFunc::VerifyMessage(result_message, "");
            }
        }
    }

    void Process_Guild_Join_Request( const eGUILD_JOIN_REQUEST& request_type, const GUILD_JOIN_REQUEST_INFO& request_info )
    {
        //eGUILD_JOIN_REQUEST_OK = 0, //가입요청 하기, 가입요청 버튼상태, 가입승인
        //eGUILD_JOIN_REQUEST_CANCEL, //가입취소 하기, 가입취소 버튼상태, 가입거절
        //eGUILD_JOIN_REQUEST_UPDATE, //가입 요청 내용 변경

        GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
            GuildSystem::GetGuildSystemData().guild_join_request_info_container();
        switch (request_type)
        {
        case eGUILD_JOIN_REQUEST_OK: // 가입요청이 왔을 때
            {
                bool is_find = false;
                for (unsigned int i = 0; i < guild_join_request_info_container.size(); ++i)
                {
                    if (request_info.m_char_guid == guild_join_request_info_container[i].m_char_guid)
                    {
                        is_find = true;
                        guild_join_request_info_container[i] = request_info;
                        break;
                    }
                }

                if (is_find == false)
                {
                    guild_join_request_info_container.push_back(request_info);
                }
            }
            break;
        default:
            {

            }
        }
        uiGuildJoinRequestList* ui_guild_join_request_list_ptr = 
            GET_CAST_DIALOG(uiGuildJoinRequestList, uiGuildJoinRequestList::kDialog_ID);
        if (ui_guild_join_request_list_ptr)
        {
            ui_guild_join_request_list_ptr->UpdateJoinRequestList();
        }
    }


    void Delete_Element_Guild_Join_Request_List( const CHARGUID& char_guid )
    {
        GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
            GuildSystem::GetGuildSystemData().guild_join_request_info_container();

        GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer::iterator iter;
        for (iter = guild_join_request_info_container.begin();
            iter != guild_join_request_info_container.end();
            )
        {
            if (char_guid == (*iter).m_char_guid)
            {
                iter = guild_join_request_info_container.erase(iter);
                break;
            }
            else
            {
                ++iter;
            }
        }
        uiGuildJoinRequestList* ui_guild_join_request_list_ptr = 
            GET_CAST_DIALOG(uiGuildJoinRequestList, uiGuildJoinRequestList::kDialog_ID);
        if (ui_guild_join_request_list_ptr)
        {
            ui_guild_join_request_list_ptr->UpdateJoinRequestList();
        }
    }

#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION


    DWORD GetGuildDutyStringCode( eGUILD_DUTY guild_duty )
    {
        DWORD string_code = 0;
        switch (guild_duty)
        {
        case eGUILD_DUTY_MASTER:
            {
                string_code = 11324;
            }
            break;
        case eGUILD_DUTY_SUBMASTER:
            {
                string_code = 11327;
            }
            break;
        case eGUILD_DUTY_LEADER:
            {
                string_code = 11330;
            }
            break;
        case eGUILD_DUTY_REGULAR_SOLDIER:
            {
                string_code = 11339;
            }
            break;
        }

        return string_code;
    }

    DWORD GetInstitutionNameCode( int Institution_index )
    {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        DWORD string_code = 0;
        BASE_GuildFacilityImageInfo* info_ptr = 
            GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(Institution_index);
        if (info_ptr)
        {
            return info_ptr->facility_name_code_;
        }

        return string_code;
#else
        const DWORD kInstitutionNumber = 5;
        const DWORD kInstitutionNameCode[kInstitutionNumber] = {
            7319, 
            7320, 
            7321, 
            7322, 
            7323};
        
        DWORD string_code = 0;

        if (Institution_index >= 0 && 
            Institution_index <= kInstitutionNumber)
        {
            string_code = kInstitutionNameCode[Institution_index];
        }        

        return string_code;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }

    // callback functions
    // 길드 초대/탈퇴/제명
    void Callback_GuildInvite( uiSystemMessage::ButtonIndex button_index )
    {
        bool invite_answer = false;
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            invite_answer = true;
        }

        GuildSystem::PacketFunction::SEND_CG_GUILD_INVITE_ANSWER_SYN(invite_answer, 
            uiSystemMessage::GetUserData());
    }

    void Callback_KickGuildMember( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_KICKPLAYER_SYN();
        }
    }

    void Callback_WithdrawGuild( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_WITHDRAW_SYN();
        }
    }


    void Callback_DelegateMaster( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_CHANGE_MASTER_SYN();
        }
    }

    void Callback_GuildRelationAnswerAllince( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_ANSWER_SYN(
                eGUILD_ANSWER_YES, 
                eGUILD_RELATION_ALLIENCE, 
                uiSystemMessage::GetUserData());
        }
        else
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_ANSWER_SYN(
                eGUILD_ANSWER_NO, 
                eGUILD_RELATION_ALLIENCE, 
                uiSystemMessage::GetUserData());
        }
    }

    void Callback_GuildRelationAnswerHostility( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_ANSWER_SYN(
                eGUILD_ANSWER_YES, 
                eGUILD_RELATION_CANCEL_BOTH_HOSTILITY, 
                uiSystemMessage::GetUserData());
        }
        else
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_ANSWER_SYN(
                eGUILD_ANSWER_NO, 
                eGUILD_RELATION_CANCEL_BOTH_HOSTILITY, 
                uiSystemMessage::GetUserData());
        }
    }

    void Callback_RegisterGuildMark( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            uiBase* ui_guild_mark_combination = GET_DIALOG(uiGuildMarkCombination::kDialog_ID);
            if (ui_guild_mark_combination != NULL)
            {
                ui_guild_mark_combination->ShowInterface(TRUE);
            }
        }
        else if (button_index == uiSystemMessage::kRejection_Button)
        {
            GuildSystem::Function::ShowConfirmDeleteGuildMark();
        }
    }

    void Callback_DeleteGuildMark( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_GUILDMARK_DELETE_SYN();
        }
        else if (button_index == uiSystemMessage::kRejection_Button)
        {
            // 
        }
    }

    void Callback_UseGuildWareHouse(uiSystemMessage::ButtonIndex button_index)
    {
        if (button_index == uiSystemMessage::kVerify_Button)
        {
            uiGuildWareHouse* guild_warehouse = 
                GET_CAST_DIALOG(uiGuildWareHouse, uiGuildWareHouse::kDialog_ID);
            if (guild_warehouse != NULL)
            {
                guild_warehouse->SendWareHouseStart();
            }
        }
    }

    void Callback_GuildCreate( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            uiGuildCreate* ui_guild_create = GET_CAST_DIALOG(uiGuildCreate, uiGuildCreate::kDialog_ID);
            if (ui_guild_create != NULL)
            {
                TCHAR guild_name[MAX_GUILDNAME_LENGTH + 1];
                ui_guild_create->GetGuildName(guild_name, MAX_GUILDNAME_LENGTH + 1);
                GuildSystem::PacketFunction::SEND_CG_GUILD_CREATE_SYN(guild_name);
            }            
        }
    }

    void Callback_GuildDestroy( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_DESTROY_SYN();
        }
        else
        {
            // 70491 길드 해산을 취소 하셨습니다.
            GlobalFunc::VerifyMessage(70491, 0, NULL, true);
        }
    }

    void Callback_GuildDestroyWait(uiSystemMessage::ButtonIndex button_index)
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_RECOVERY_GUILD_SYN();
        }
        else
        {
            // 6235 길드 해산 해제 신청에 실패하였습니다.
            GlobalFunc::VerifyMessage(6235, 0, NULL, true);
        }
    }

    void Callback_GuildRename(uiSystemMessage::ButtonIndex button_index)
    {
        uiGuildRelationAsk* ui_guild_relation_ask = 
            GET_CAST_DIALOG(uiGuildRelationAsk, uiGuildRelationAsk::kDialog_ID);
        if (ui_guild_relation_ask == NULL)
        {
            return;
        }

        if (ui_guild_relation_ask->guild_ask_type() != uiGuildRelationAsk::kGuild_Rename)
        {
            return;
        }

        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_NAME_CHANGE_SYN(
                ui_guild_relation_ask->temp_string().c_str(), 
                ui_guild_relation_ask->item_slot_position());
        }
    }

    void Callback_GuildNoticeCancle( uiSystemMessage::ButtonIndex button_index )
    {
        bool invite_answer = false;
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            invite_answer = true;
            uiGuildIntro* ui_guild_intro = GET_CAST_DIALOG(uiGuildIntro, uiGuildIntro::kDialog_ID);
            if (ui_guild_intro)
            {
                ui_guild_intro->SetEditMode(false);
            }
        }
    }

    void Callback_GuildNoticeDelete( uiSystemMessage::ButtonIndex button_index )
    {
        bool invite_answer = false;
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            invite_answer = true;
            uiGuildIntro* ui_guild_intro = GET_CAST_DIALOG(uiGuildIntro, uiGuildIntro::kDialog_ID);
            if (ui_guild_intro)
            {
                ui_guild_intro->SetDeleteMessage();
            }
        }
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void Callback_GuildJoinRequestSend( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            uiGuildInfoOther* ui_guild_info_other = GET_CAST_DIALOG(uiGuildInfoOther, uiGuildInfoOther::kDialog_ID);
            if (ui_guild_info_other)
            {
                ui_guild_info_other->OnClickRequestJoin();
            }
        }
    }

    void Callback_GuildFacilityUpgrade( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_FACILITY_UPGRADE_SYN((FCODE)uiSystemMessage::GetUserData());
        }
    }

    void Callback_GuildFacilityActive( uiSystemMessage::ButtonIndex button_index )
    {
        if (button_index == uiSystemMessage::kConfirm_Button)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_FACILITY_ACTIVE_SYN((FCODE)uiSystemMessage::GetUserData());
        }
    }

    void UpdateAllGuildInstitution()
    {
        uiGuildInstitution* ui_guild_institution = GET_CAST_DIALOG(uiGuildInstitution, uiGuildInstitution::kDialog_ID);
        if (ui_guild_institution)
        {
            ui_guild_institution->UpdateAll();
        }
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

}; //namespace Function

}; //namespace GuildSystem
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL