#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "GuildSystem/uiGuildPvE.h"
#include "GuildSystem/uiGuildInfo.h"
#include "InterfaceManager.h"
#include "MultiVScrollDialog.h"
#include "CastDialog.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/GuildMissionTreeParser.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "SCItemSlot.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "GlobalFunc.h"
#include "Map.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
  #include "Hero.h"
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#include "ChaosZoneInfoParser.h"

const DWORD uiGuildPvE::kDialog_ID = StrToWzID("7349");

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
const int refresh_member_list_time = 5;
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION



//----------------------------------------------------------------------------
/**
*/
// 길드원 정렬 함수
template <class T, T GUILD_MEMBER_INFO::*p>
bool sortGuildMemberByTypeHigh(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO& rhw)
{
    return lhw.*p < rhw.*p;
}

template <class T, T GUILD_MEMBER_INFO::*p>
bool sortGuildMemberByTypeLow(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO& rhw)
{
    return lhw.*p > rhw.*p;
}
//스트링 StrCmp이용한 정렬
bool sortGuildMemberByNameHigh(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO & rhw)
{
    int returnValue =  StrnCmp(lhw.m_tszCharName , rhw.m_tszCharName, MAX_CHARNAME_LENGTH);
    if( returnValue < 0 )
        return true;
    else
        return false;
}

bool sortGuildMemberByNameLow(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO & rhw)
{
    int returnValue =  StrnCmp(lhw.m_tszCharName , rhw.m_tszCharName, MAX_CHARNAME_LENGTH);
    if( returnValue > 0 )
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------
/**
*/
uiGuildPvE::uiGuildPvE(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
    //set_mission_info_vscroll(NULL);
    set_member_vscroll(NULL);

    init_mission_tree_ = false;

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    select_zone_type_ = kChaosZoneTypeNone;
    onedays_mission_end_time_ = 0;
    onedays_mission_event_state_ = 0;
    list_refresh_timer_.On(refresh_member_list_time);
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
}


//----------------------------------------------------------------------------
/**
*/
uiGuildPvE::~uiGuildPvE(void) 
{
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildPvE::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);


    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    

    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    //RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    //RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("V002"), kControl_VScroll_V002);

    RegisterControlPosition(StrToWzID("S003"), kControl_Static_S003);

    //set_mission_info_vscroll(new MultiVScrollDialog(GetControlWZ_byPosition(kControl_VScroll_V001),
        //GetControlWZ_byPosition(kControl_List_L001)));
    set_member_vscroll(new MultiVScrollDialog(GetControlWZ_byPosition(kControl_VScroll_V002),
        GetControlWZ_byPosition(kControl_List_L002)));

    

    // 미션내용
 /*
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
     if (list_control)
     {
         RECT list_rect = list_control->GetSizeRect();
         ftext_render_.Init(draw_base, list_rect);
 
         ftext_render_.SetMargin(0,0,0,-5);
         ftext_render_.SetGap(0,0);
 
         ftext_render_.convertLineInfo();
     }*/
 

    // 멤버 리스트
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L002));
    if (list_control)
    {
        list_control->SetUseReport(true);
        list_control->SetSellHeight(19);
        list_control->SetReportUseCnt(3);
        list_control->SetHideReportRender(TRUE);
        list_control->SetReportButtonInfo(0, 144, NULL); // 길드원이름
        list_control->SetReportButtonInfo(1, 46, NULL); // 레벨         // 2016.1.22 / [SUNJP-1810] 일본 폰트가 공간이 작아서 크기 조절
        list_control->SetReportButtonInfo(2, 108, NULL); // 클래스
        //list_control->SetReportButtonInfo(1, 60, NULL); // 레벨
        //list_control->SetReportButtonInfo(2, 94, NULL); // 클래스
    }

    // 트리 생성
    InitMissionTree();
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildPvE::Release()
{
/*
    if (mission_info_vscroll())
    {
        delete(mission_info_vscroll());
        set_mission_info_vscroll(NULL);
    }*/

    if (member_vscroll())
    {
        delete(member_vscroll());
        set_member_vscroll(NULL);
    }

    tree_menu().Release();
    select_tree_.menu_index = 0;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildPvE::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnVScrollUp(message);
            }           
            /*
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
                        {
                            OnVScrollUp_MissionInfo(message);
                        }*/
            
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002 && member_vscroll())
            {
                member_vscroll()->OnVScrollUp(message);
            }
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnVScrollDown(message);
            }
            /*
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
                        {
                            OnVScrollDown_MissionInfo(message);
                        }*/
            
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002 && member_vscroll())
            {
                member_vscroll()->OnVScrollDown(message);
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnVScrollThumb(message);
            }
            /*
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
                        {
                            OnVScrollThumb_MissionInfo(message);
                        }*/
            
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002 && member_vscroll())
            {
                member_vscroll()->OnVScrollThumb(message);
            }
        }
        break;

    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
bool uiGuildPvE::OnLButtonClick(SI_MESSAGE* message)
{
    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true)
    {
        SelectIndex(tree_object);
    }

    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C008:
        {
            uiGuildInfo* ui_guild_info = GET_CAST_DIALOG(uiGuildInfo, uiGuildInfo::kDialog_ID);
            if (ui_guild_info != NULL)
            {
                ui_guild_info->tab_button_controller().OnClickTab(
                    control_position - kControl_CheckBT_C000);
            }
            RefreshTab();
        }
        break;
    case kControl_Button_B009:
        {
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            ClickStart();
#else
            if (g_pMap->IsField() || g_pMap->IsVill())
            {
                ChaosSystem::PacketFunction::SEND_CG_SONNENSCHEIN_CREATE_SYN();
            }
            else
            {
                // 6415 마을과 필드에서만 입장이 가능 합니다.
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(6415, string, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, string);
            }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        }
        break;
    case kControl_Button_B000:
    case kControl_Button_B001:
    case kControl_Button_B002:
        {
            GuildSystem::GuildSystemData::GuildMemberSortType sort_type = 
                static_cast<GuildSystem::GuildSystemData::GuildMemberSortType>(control_position - kControl_Button_B000);
            SortGuildMember(sort_type);
            UpdateGuildMemberList();
        }
        break;
    case kControl_Button_B004:
        {
            RequestGuildMemberList();
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildPvE::OnShowWindow(BOOL val)
{
    if (val)
    {
        RefreshTab();

        if (select_tree_.menu_index == 0)
        {
            TreeMenuGroup::TreeObject* tree_object = NULL;
            tree_object = tree_menu().Find(1,1,0,0);
            if (tree_object != NULL)
            {
                tree_menu().Select(tree_object);
                SelectIndex(tree_object);
            }
        }

        // 미션참여 버튼 활성화
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_B009));
        if (g_pMap && control)
        {
            if (g_pMap->IsField() || g_pMap->IsVill())
            {
                control->EnableWindowWZ(true);
            }
            else
            {
                control->EnableWindowWZ(false);
            }
        }

        RequestGuildMemberList();
    }
    else
    {
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildPvE::NetworkProc(MSG_BASE* recv_packet)
{/*
    if(recv_packet->m_byCategory == CG_GUILD){
        switch (recv_packet->m_byProtocol)
        {
       
             case CG_GUILD_WAREHOUSE_OPEN_ACK:
                        {
                            / *MSG_CG_GUILD_WAREHOUSE_OPEN_ACK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_OPEN_ACK*)recv_packet;
            
                            ShowInterfaceWithSound(TRUE);
            
                            FlushUnitDraw();
            
                            BitStream bitstream(warehouse_packet->m_BitStreamBuffer, 
                            warehouse_packet->m_Size, 
                            FALSE);
                            SCItemSlotContainer::Serialize(bitstream, eSLOT_SERIALIZE_CLIENT_SAVE);
                            set_max_tab(warehouse_packet->m_SlotTabNum);
                            set_balance(warehouse_packet->m_WarehouseMoney);
            
                            set_current_tab(0);
                            ReFreshRenderSlot();
            
                            control_flag().insert(kUseStart);* /
                        }
                        break;
            

        default:
            {
            }
            break;
        }
    }*/
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildPvE::OnUpdateSolarDialog()
{
    VScrollDialog::Update();

    /*
    if (mission_info_vscroll())
        {
            RECT area;
            if (mission_info_vscroll()->GetScrollArea(area))
            {
                BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);
                switch (wheel_msg)
                {
                case MOUSE_WHEEL_UP:
                    {
                        OnVScrollUp_MissionInfo(NULL);  break;
                    }
                case MOUSE_WHEEL_DOWN:
                    {
                        OnVScrollDown_MissionInfo(NULL);  break;
                    }
                } 
            }
        }*/
    

    if (member_vscroll())
    {
        member_vscroll()->Update();
    }
}

void uiGuildPvE::OnRenderSolarDialog()
{
/*
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    if (list_control)
    {
        RECT list_rect = list_control->GetSizeRect();
        ftext_render_.SetRect(list_rect);

        ftext_render_.Render();
    }*/


    RenderTooltip();
}


void uiGuildPvE::Process(DWORD tick_count)
{
    if (wait_timer_.Check() != TIMER_STAT_NONE)
    {
        if (wait_timer_.Check() == TIMER_STAT_FINISH)
        {
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            // 미션 종류 
            switch (select_zone_type_)
            {
            case eCHAOSZONE_TYPE_SONNENSCHEIN:
                {
                    // 길드PVE - 소넨샤인
                    TreeMenuGroup::TreeObject* find_tree = tree_menu().Find(select_tree_.menu_index);
                    if (find_tree)
                    {
                        MAPCODE map_code = static_cast<MAPCODE>(find_tree->info->user_data);
                        ChaosSystem::PacketFunction::SEND_CG_SONNENSCHEIN_ENTER_CHECK_SYN(GetRequireItem(map_code));
                    }
                }
                break;
            case eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION:
                {
                    // 길드일일미션
                    TreeMenuGroup::TreeObject* find_tree = tree_menu().Find(select_tree_.menu_index);
                    if (find_tree)
                    {
                        MAPCODE map_code = static_cast<MAPCODE>(find_tree->info->user_data);
                        ChaosSystem::PacketFunction::SEND_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN(GetRequireItem(map_code));
                    }
                }
                break;
            default:
                {
                    ChaosSystem::GetChaosZoneData().set_is_wait(false);
                    if (g_InterfaceManager.IsLockDlg())
                    {
                        g_InterfaceManager.UnLockDlgs();
                    }
                    break;
                }
            }

            WaitGague(false);
#else
            TreeMenuGroup::TreeObject* find_tree = tree_menu().Find(select_tree_.menu_index);
            if (find_tree)
            {
                MAPCODE map_code = static_cast<MAPCODE>(find_tree->info->user_data);

                const ChaosZoneInfo* find_info = ChaosZoneInfoParser::Instance()->FindData(map_code);
                if (find_info == NULL)
                {
                    return;
                }

                // 미션 종류 
                switch (find_info->kind_of_room)
                {
                case eCHAOSZONE_TYPE_SONNENSCHEIN:
                    {
                        // 길드PVE - 소넨샤인
                        ChaosSystem::PacketFunction::SEND_CG_SONNENSCHEIN_ENTER_CHECK_SYN(GetRequireItem(map_code));
                    }
                    break;
                default:
                    //
                    ChaosSystem::GetChaosZoneData().set_is_wait(false);
                    if (g_InterfaceManager.IsLockDlg())
                    {
                        g_InterfaceManager.UnLockDlgs();
                    }
                    break;
                }

                WaitGague(false);
            }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        }
        else
        {
            CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));
            if (cast_dialog)
            {
                cast_dialog->SetGage((wait_timer_.SpentTime()/50.0f));
            }
        }
    }
}
//========= 스크롤바 처리 =======================
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiGuildPvE::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiGuildPvE::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
    
}
//------------------------------------------------------------------------------ 
bool uiGuildPvE::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;

}
//------------------------------------------------------------------------------ 
bool uiGuildPvE::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildPvE::OnVScrollDown(SI_MESSAGE* msg)
{
    
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
/*
bool uiGuildPvE::OnVScrollThumb_MissionInfo(SI_MESSAGE* msg)
{
    if (mission_info_vscroll() && mission_info_vscroll()->OnVScrollThumb(msg) == true)
    {
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
        if (scroll_control)
        {
            ftext_render_.SetBeginLineIndex(scroll_control->GetScrollPosWZ());
        }
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildPvE::OnVScrollUp_MissionInfo(SI_MESSAGE* msg)
{
    if (mission_info_vscroll() && mission_info_vscroll()->OnVScrollUp(msg) == true)
    {
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
        if (scroll_control)
        {
            ftext_render_.SetBeginLineIndex(scroll_control->GetScrollPosWZ());
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildPvE::OnVScrollDown_MissionInfo(SI_MESSAGE* msg)
{
    if (mission_info_vscroll() && mission_info_vscroll()->OnVScrollDown(msg) == true)
    {
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
        if (scroll_control)
        {
            ftext_render_.SetBeginLineIndex(scroll_control->GetScrollPosWZ());
        }
        return true;
    }
    return false;
}*/

//------------------------------------------------------------------------------ 
void uiGuildPvE::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C002 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((position == kControl_CheckBT_C008) ? (true) : (false));
    }
}
//------------------------------------------------------------------------------ 
/*
void uiGuildPvE::UpdateGuildMissionInfo(MAPCODE map_code)
{
    // 미션 내용
    const GuildMissionInfo* guild_mission_info = GuildMissionTreeParser::Instance()->FindData(map_code);
    if (guild_mission_info)
    {
        // 전장 설명
        ftext_render_.ClearSentence();

        FTextParser parser;
        FTextSentence sentence;
        //FTextSentence temp_sentence;

        TCHAR temp_string[INTERFACE_STRING_LENGTH]= " ";
        //TCHAR str_format[INTERFACE_STRING_LENGTH]={0,};
        BOOL result = false;

        // 빈줄
        //parser.Parser(temp_string, temp_sentence);

        if (guild_mission_info->mission_string_code != 0)
        {
            // 스트링에 줄바꿈은 # 대신 <return=1/> 으로 넣어주세요
            g_InterfaceManager.GetInterfaceString(guild_mission_info->mission_string_code, temp_string, INTERFACE_STRING_LENGTH);
            result = parser.Parser(temp_string, sentence);
            if (result == TRUE)
            {
                //ftext_render_.Append(temp_sentence); //빈줄
                ftext_render_.Append(sentence);
            }
        }

        ftext_render_.convertLineInfo();
       
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
        if (list_control)
        {
            RECT list_rect = list_control->GetSizeRect();
            ftext_render_.SetRect(list_rect);
        }
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
        if (scroll_control)
        {
            int scroll_range = ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect();
            scroll_control->SetScrollRangeWZ(0,scroll_range);
            scroll_control->SetScrollPosWZ(0);
        }
        
        
    }
}*/

//------------------------------------------------------------------------------ 
void uiGuildPvE::RequestGuildMemberList()
{
    member_container_.clear();
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    disconnect_member_list_.clear();
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

    UpdateGuildMemberList();

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    // 미션 종류 
    switch (select_zone_type_)
    {
    case eCHAOSZONE_TYPE_SONNENSCHEIN:
        // 길드PVE - 소넨샤인
        ChaosSystem::PacketFunction::SEND_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN();
        break;
    case eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION:
        // 길드일일미션
        ChaosSystem::PacketFunction::SEND_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN();
        list_refresh_timer_.Off();
        list_refresh_timer_.On(refresh_member_list_time);
        break;
    default:
        break;
    }
#else
    TreeMenuGroup::TreeObject* find_tree = tree_menu().Find(select_tree_.menu_index);
    if (find_tree)
    {
        MAPCODE map_code = static_cast<MAPCODE>(find_tree->info->user_data);

        const ChaosZoneInfo* find_info = ChaosZoneInfoParser::Instance()->FindData(map_code);
        if (find_info == NULL)
        {
            return;
        }

        // 미션 종류 
        switch (find_info->kind_of_room)
        {
        case eCHAOSZONE_TYPE_SONNENSCHEIN:
            // 길드PVE - 소넨샤인
            ChaosSystem::PacketFunction::SEND_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN();
            break;
        default:
            break;
        }
    }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    
}
//------------------------------------------------------------------------------ 
void uiGuildPvE::SetGuildMemberList(MSG_BASE* recv_message)
{
    if (recv_message == NULL)
    {
        return;
    }

    if (select_zone_type_ != eCHAOSZONE_TYPE_SONNENSCHEIN)
    {
        return;
    }

    MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK* recv_packet =
        static_cast<MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK*>(recv_message);

    member_container_.clear();

    for (int i = 0; i < recv_packet->enter_player_count; ++i)
    {
        CHARGUID character_guid = recv_packet->enter_player_list[i];
        if (character_guid == 0)
        {
            break;
        }
        
        GUILD_MEMBER_INFO* member_info =
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(character_guid);
        if (member_info == NULL)
        {
            break;
        }
        
        member_container_.push_back(*member_info);
    }
    
    SortGuildMember(GuildSystemData::kSortByName, true);
    UpdateGuildMemberList();
}
//------------------------------------------------------------------------------ 
void uiGuildPvE::UpdateGuildMemberList()
{
    // 멤버 리스트
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L002));
    if (list_control == NULL)
    {
        return;
    }

    list_control->DeleteItemAll();

    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    const DWORD kTextColor[3] = {RGBA(255,255,255,255), RGBA(240,200,120,255), RGBA(128, 128, 128, 255)};
    int color_index = 0;
    int list_index = 0;
    size_t member_number = member_container_.size();
    for (size_t i = 0; i < member_number; ++i)
    {
        if (i >= MAX_GUILD_MEMBER_NUM)
        {
            break;
        }
        
        GUILD_MEMBER_INFO& member_info = member_container_[i];
        
        color_index = 0;
        
        if ((g_pHero != NULL) && 
            (StrnCmp(g_pHero->GetName(), member_info.m_tszCharName, MAX_CHARNAME_LENGTH) == 0))
        {
            color_index = 1;
        }
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        if (select_zone_type_ == eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
        {
            for (size_t i = 0; i < disconnect_member_list_.size(); ++i)
            {
                if (member_info.m_CharGuid == disconnect_member_list_[i])
                {
                    // 튕긴사람 리스트에 있으면 회색
                    color_index = 2;
                }
            }
        }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION


        list_index = list_control->InsertItemBack();

        // 길드원 이름
        list_control->SetItemWZ(list_index, 
            0,
            member_info.m_tszCharName,
            StrToWzID("st11"),
            DT_CENTER | DT_VCENTER,
            RGBA(0,0,0,0),
            kTextColor[color_index],
            kTextColor[color_index]);

        // 레벨
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%d", member_info.m_Level);
        list_control->SetItemWZ(list_index, 
            1, 
            temp_string,
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            kTextColor[color_index], 
            kTextColor[color_index]);

        // 클래스
        list_control->SetItemWZ(list_index, 
            2, 
            GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(member_info.m_byClass)), 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            kTextColor[color_index], 
            kTextColor[color_index]);
    }

    if (member_vscroll())
    {
        int max_line_count = list_control->GetListRowCnt();
        int draw_line_count = list_control->GetDrawRowCount();
        int scroll_max_num = max(0, max_line_count - draw_line_count);
        member_vscroll()->ResizeingVScrollControl(scroll_max_num, 0);
    }

    // 참여 인원
    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Static_S003));
    if (static_control)
    {
        TCHAR string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
        sprintf(temp_string,"%d",member_number);

        g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 3537, temp_string);

        // 3537 %s 명
        static_control->SetTextWZ(string);
    }
    

}
//------------------------------------------------------------------------------ 
void uiGuildPvE::WaitGague(bool is_show, int second /* = 5 */)
{
    CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));
    if (is_show)
    {
        wait_timer_.On(second);
        wait_second_ = second;

        if (cast_dialog)
        {
            g_InterfaceManager.HideAllWindows();
            g_InterfaceManager.ShowGameInterface(TRUE);

            // 5116	잠시만 기다려 주십시오.
            int string_code = 5116;

            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(string_code, message);
            cast_dialog->SetText(message);
            cast_dialog->ShowWindow(TRUE);
        }
    }
    else
    {
        wait_timer_.Off();
        if (cast_dialog)
        {
            cast_dialog->ShowWindow(FALSE);
            cast_dialog->SetText("");
        }
    }

}
//------------------------------------------------------------------------------ 
SLOTCODE uiGuildPvE::GetRequireItem(const MAPCODE map_code)
{
    SLOTCODE item_code = 0;
    
    const ChaosZoneInfo* find_info = ChaosZoneInfoParser::Instance()->FindData(map_code);
    if (find_info)
    {
        for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
        {
            if (find_info->require_item[i] != 0)
            {
                BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(find_info->require_item[i]);
                if (item_info)
                {
                    //아이템 찾기
                    SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY,
                        static_cast<SLOTCODE>(find_info->require_item[i]));
                    if (item_slot && item_slot->GetNum() > 0)
                    {
                        //nop
                        item_code = static_cast<SLOTCODE>(find_info->require_item[i]);
                        break;
                    }
                }
            }
        }
    }

    return item_code;
}
//------------------------------------------------------------------------------ 
void uiGuildPvE::InitMissionTree()
{
    if (init_mission_tree_ == true)
    {
        return;
    }
    init_mission_tree_ = true;

    select_tree_.menu_index = 0;
    
    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_SonnenSchein_MissionList,
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000)),
        GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000)),
        false, 
        NULL);
    tree_menu().set_menu_height_gab(20);


    const int limit_depth = 100;
    const GuildMissionInfo* find_info;

    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_SonnenSchein_MissionList);

    for (BYTE LGroupCount = 1; LGroupCount <= limit_depth; ++LGroupCount)
    {
        find_info = GuildMissionTreeParser::Instance()->FindData(LGroupCount, 1);
        if (find_info == NULL)
        {
            break;
        }

        for (BYTE SGroupCount = 1; SGroupCount <= limit_depth; ++SGroupCount)
        {
            find_info = GuildMissionTreeParser::Instance()->FindData(LGroupCount, SGroupCount);
            if (find_info == NULL)
            {
                break;
            }

            if (SGroupCount == 1)
            {
                MakeGuildPvETree(LGroupCount, 0, 0, find_info->large_group_string, NULL, find_info->font_color, find_info->map_code);
            }

            MakeGuildPvETree(LGroupCount, SGroupCount, 1, find_info->sub_group_string, NULL, find_info->font_color, find_info->map_code);
        }
    }

    tree_menu().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_SonnenSchein_MissionList);
    tree_menu().Fill();
}
//------------------------------------------------------------------------------ 
void uiGuildPvE::MakeGuildPvETree(BYTE depth_3, BYTE depth_2, BYTE depth, DWORD string_code, TCHAR* string, const DWORD* font_colors, MAPCODE map_code)
{
    BASE_TreeMenuInfo tree_menu_info;
    ZeroMemory(&tree_menu_info, sizeof(tree_menu_info));

    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Enable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Disable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Over] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Select] = StrToWzID("st10");


    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Enable] = font_colors[BASE_TreeMenuInfo::kFontType_Enable];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = font_colors[BASE_TreeMenuInfo::kFontType_Disable];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Over] = font_colors[BASE_TreeMenuInfo::kFontType_Over];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Select] = font_colors[BASE_TreeMenuInfo::kFontType_Select];


    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = 10007;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = 10002;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = 10003;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Cursor] = 10008;


    tree_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_SonnenSchein_MissionList;
    tree_menu_info.menu_depth[3] = depth_3;
    tree_menu_info.menu_depth[2] = depth_2;
    tree_menu_info.menu_depth[1] = 0;
    tree_menu_info.menu_depth[0] = 0;
    tree_menu_info.string_code = string_code;

    if (string_code == 0)
    {
        strncpy(tree_menu_info.string_name, string, sizeof(tree_menu_info.string_name));
    }

    switch (depth)
    {
    case 0:
        tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
        break;
    case 1:
        tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
        tree_menu_info.menu_depth_width = 20;

        tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 10006;
        tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 10005;
        break;
    default:
        tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_None;
        break;
    }

    tree_menu_info.user_data = map_code;

    TreeMenuInfoParser::Instance()->Insert(tree_menu_info);

}
//------------------------------------------------------------------------------
bool uiGuildPvE::SelectIndex(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object == NULL)
    {
        return false;
    }

    if (select_tree_.menu_index == tree_object->info->menu_index)
    {
        tree_object->is_check = true;
        return false; // 이미 선택한 항목이다
    }

    if (tree_object->info->menu_depth[2] <= 0)
    {
        return false;//항목이 아니다. 폴더이다.
    }

    /*if (tree_object->is_enable == false)
    {
        return false;//비활성 항목 이다. 
    }*/

    TreeMenuGroup::TreeObject* old_tree_object = tree_menu().Find(select_tree_.menu_index);
    if (old_tree_object)
    {
        old_tree_object->is_check = false;
    }

    //선택한것은 항상 체크
    tree_object->is_check = true;

    select_tree_.menu_index = tree_object->info->menu_index;

    //
    // 내용 갱신
/*
    MAPCODE map_code = static_cast<MAPCODE>(tree_object->info->user_data);
    UpdateGuildMissionInfo(map_code);*/


#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    // 선택한 트리의 카오스존 타입 저장
    MAPCODE map_code = static_cast<MAPCODE>(tree_object->info->user_data);
    const ChaosZoneInfo* find_info = ChaosZoneInfoParser::Instance()->FindData(map_code);
    if (find_info)
    {
        select_zone_type_ = static_cast<ChaosZoneType>(find_info->kind_of_room);
    }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION


    // 길원리스트 목록 요청
    RequestGuildMemberList();

    return true;
}

void uiGuildPvE::SortGuildMember(GuildSystemData::GuildMemberSortType sort_type, bool force_high_case)
{
    static bool high_case = true; // default 오름 차순
    static bool low_case = false; // default 내림 차순

    // 다른 소팅 탭 눌렀을 때, 기본 소팅 방식으로 셋팅되도록 함.
    static GuildSystemData::GuildMemberSortType prev_sort_type = GuildSystemData::kMaxType;
    if ((prev_sort_type != sort_type) || (force_high_case == true))
    {
        high_case = true;
        low_case = false;
    }
    prev_sort_type = sort_type;

    switch (sort_type)
    {
    case GuildSystemData::kSortByName:
        {
            if (high_case)
            {
                sort(member_container_.begin(),member_container_.end(), sortGuildMemberByNameHigh);
            }
            else
            {
                sort(member_container_.begin(),member_container_.end(), sortGuildMemberByNameLow);
            }
            high_case ^= 1;
        }
        break;

    case GuildSystemData::kSortByLevel:
        {
            if (low_case)
            {
                sort(member_container_.begin(),
                    member_container_.end(), 
                    sortGuildMemberByTypeHigh<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
            }
            else
            {
                sort(member_container_.begin(),
                    member_container_.end(), 
                    sortGuildMemberByTypeLow<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
            }

            low_case ^= 1;
        }
        break;

    case GuildSystemData::kSortByClass:
        {
            if (high_case)
            {
                sort(member_container_.begin(), 
                    member_container_.end(), 
                    sortGuildMemberByTypeHigh<BYTE, &GUILD_MEMBER_INFO::m_byClass>);	
            }
            else
            {
                sort(member_container_.begin(), 
                    member_container_.end(), 
                    sortGuildMemberByTypeLow<BYTE, &GUILD_MEMBER_INFO::m_byClass>);	
            }
            high_case ^= 1;
        }
        break;
    }
}

void uiGuildPvE::RenderTooltip()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tree_menu().list_control() == NULL || 
        tooltip_manager == NULL)
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    RECT position = tree_menu().list_control()->GetSizeRect();
    if (position.left < mouse_x && position.right > mouse_x &&
        position.top < mouse_y && position.bottom > mouse_y)
    {
        TreeMenuGroup::TreeObject* tree_object = tree_menu().OverTreeObject();
        if (tree_object)
        {
            MAPCODE map_code = static_cast<MAPCODE>(tree_object->info->user_data);
            const GuildMissionInfo* find_info = GuildMissionTreeParser::Instance()->FindData(map_code);
            if (find_info)
            {
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
                const ChaosZoneInfo* chaos_info = ChaosZoneInfoParser::Instance()->FindData(map_code);
                if (chaos_info)
                {
                    switch (chaos_info->kind_of_room)
                    {
                    case eCHAOSZONE_TYPE_SONNENSCHEIN:
                        {
                            // 길드PVE - 소넨샤인
                            tooltip_manager->RegisterTooltipGuildMissionTree(find_info);
                            tooltip_manager->SetTooltipDialogID(GetDialogKey());
                        }
                        break;
                    case eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION:
                        {
                            // 길드일일미션
                            if (list_refresh_timer_.Check() != TIMER_STAT_NONE)
                            {
                                if (list_refresh_timer_.Check() == TIMER_STAT_FINISH)
                                {
                                    list_refresh_timer_.Off();
                                    list_refresh_timer_.On(refresh_member_list_time);
                                    ChaosSystem::PacketFunction::SEND_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN();
                                }
                            }
                            tooltip_manager->RegisterTooltipGuildMissionTree_OneDayMission(
                                find_info, onedays_mission_end_time_, onedays_mission_event_state_);
                            tooltip_manager->SetTooltipDialogID(GetDialogKey());
                        }
                        break;
                    default:
                        break;
                    }
                }
#else
                tooltip_manager->RegisterTooltipGuildMissionTree(find_info);
                tooltip_manager->SetTooltipDialogID(GetDialogKey());
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            }
        }
    }
}

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
void uiGuildPvE::ClickStart()
{
    if (g_pMap->IsField() || g_pMap->IsVill())
    {
        // 미션 종류 
        switch (select_zone_type_)
        {
        case eCHAOSZONE_TYPE_SONNENSCHEIN:
            // 길드PVE - 소넨샤인
            ChaosSystem::PacketFunction::SEND_CG_SONNENSCHEIN_CREATE_SYN();
            break;
        case eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION:
            // 길드일일미션
            ChaosSystem::PacketFunction::SEND_CG_ONEDAYSGUILDMISSION_CREATE_SYN();
            break;
        default:
            break;
        }
    }
    else
    {
        // 6415 마을과 필드에서만 입장이 가능 합니다.
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(6415, string, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, string);
    }
}

void uiGuildPvE::SetGuildMemberList_OneDaysGuildMission(MSG_BASE* recv_message)
{
    if (recv_message == NULL)
    {
        return;
    }

    MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK* recv_packet =
        static_cast<MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK*>(recv_message);

    if (select_zone_type_ != eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
    {
        onedays_mission_end_time_ = GetTickCount() + recv_packet->remain_time_;
        onedays_mission_event_state_ = recv_packet->event_state_;
        return;
    }

    member_container_.clear();
    disconnect_member_list_.clear();

    for (int i = 0; i < recv_packet->enter_player_count; ++i)
    {
        CHARGUID character_guid = recv_packet->enter_player_list[i];
        if (character_guid == 0)
        {
            break;
        }

        GUILD_MEMBER_INFO* member_info =
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(character_guid);
        if (member_info == NULL)
        {
            break;
        }

        member_container_.push_back(*member_info);
    }

    for (int i = 0; i < recv_packet->stand_by_user_list_cnt; ++i)
    {
        CHARGUID character_guid = recv_packet->stand_by_user_list[i];
        if (character_guid == 0)
        {
            break;
        }

        // 스타트업멤버가 멤버리스트에 없으면 넣어줌
        BOOL is_find = FALSE;
        for (size_t j = 0; j < member_container_.size(); ++j)
        {
            GUILD_MEMBER_INFO& member_info = member_container_[j];
            if (member_info.m_CharGuid == character_guid)
            {
                is_find = TRUE;
                break;
            }
        }

        if (is_find == FALSE)
        {
            // 튕긴 사람으로 추가
            disconnect_member_list_.push_back(character_guid);

            GUILD_MEMBER_INFO* member_info =
                GuildSystem::GetGuildSystemData().GetGuildMemberInfo(character_guid);
            if (member_info)
            {
                member_container_.push_back(*member_info);
            }
        }
    }

    SortGuildMember(GuildSystemData::kSortByName, true);
    UpdateGuildMemberList();

    onedays_mission_end_time_ = GetTickCount() + recv_packet->remain_time_;
    onedays_mission_event_state_ = recv_packet->event_state_;
}
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION


#endif //_NA_008334_20150608_SONNENSCHEIN
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL


