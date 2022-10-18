#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildManagement.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/uiGuildDominationAreaInfo.h"
#include "GuildSystem/uiGuildRelation.h"
#include "GuildRightInfoParser.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"
#include "InterfaceManager.h"
#include "globalfunc.h"
#include "MultiVScrollDialog.h"

const DWORD uiGuildManagement::kDialog_ID = StrToWzID("7344");

//----------------------------------------------------------------------------
/**
*/
uiGuildManagement::uiGuildManagement(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
    set_enemy_vscroll(NULL);
    set_wait_vscroll(NULL);
}


//----------------------------------------------------------------------------
/**
*/
uiGuildManagement::~uiGuildManagement(void) 
{
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildManagement::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
#ifdef _NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
#endif //_NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("V002"), kControl_VScroll_V002);
    RegisterControlPosition(StrToWzID("V003"), kControl_VScroll_V003);
    RegisterControlPosition(StrToWzID("I000"), kControl_TextImage_I000);
    RegisterControlPosition(StrToWzID("I001"), kControl_TextImage_I001);
    RegisterControlPosition(StrToWzID("I003"), kControl_TextImage_I003);
    RegisterControlPosition(StrToWzID("I002"), kControl_TextImage_I002);
    RegisterControlPosition(StrToWzID("I005"), kControl_TextImage_I005);
    RegisterControlPosition(StrToWzID("I004"), kControl_TextImage_I004);
    RegisterControlPosition(StrToWzID("I006"), kControl_TextImage_I006);

    set_enemy_vscroll(new MultiVScrollDialog(GetControlWZ_byPosition(kControl_VScroll_V003),
        GetControlWZ_byPosition(kControl_List_L000)));

    set_wait_vscroll(new MultiVScrollDialog(GetControlWZ_byPosition(kControl_VScroll_V002),
        GetControlWZ_byPosition(kControl_List_L002)));
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildManagement::Release()
{
    if (enemy_vscroll())
    {
        delete(enemy_vscroll());
        set_enemy_vscroll(NULL);
    }
    if (wait_vscroll())
    {
        delete(wait_vscroll());
        set_wait_vscroll(NULL);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildManagement::MessageProc(SI_MESSAGE* message)
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
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollUp(message);
            }           
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V003 && enemy_vscroll())
            {
                enemy_vscroll()->OnVScrollUp(message);
            }
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002 && wait_vscroll())
            {
                wait_vscroll()->OnVScrollUp(message);
            }
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollDown(message);
            }
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V003 && enemy_vscroll())
            {
                enemy_vscroll()->OnVScrollDown(message);
            }
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002 && wait_vscroll())
            {
                wait_vscroll()->OnVScrollDown(message);
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollThumb(message);
            }
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V003 && enemy_vscroll())
            {
                enemy_vscroll()->OnVScrollThumb(message);
            }
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002 && wait_vscroll())
            {
                wait_vscroll()->OnVScrollThumb(message);
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
bool uiGuildManagement::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
    case kControl_CheckBT_C002:
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case kControl_CheckBT_C005:
#endif //_NA_008334_20150608_SONNENSCHEIN
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
    case kControl_Button_B005:
        {
            OnClickGuildRelationButton();
        }
        break;
    case kControl_Button_B001:
        {
            uiGuildDominationAreaInfo* ui_guild_domination_info = 
                GET_CAST_DIALOG(uiGuildDominationAreaInfo, uiGuildDominationAreaInfo::kDialog_ID);
            if (ui_guild_domination_info != NULL)
            {
                ui_guild_domination_info->ShowInterface(TRUE);
            }
        }
        break;
    case kControl_Button_B002:
        {
            // 점령전관리기능
            OnClickDominationManagementButton();
        }
        break;
    case kControl_List_L001:
        {
        }
        break;
    case kControl_List_L000:
        {
        }
        break;
    case kControl_List_L002:
        {
        }
        break;
    case kControl_VScroll_V001:
        {
        }
        break;
    case kControl_VScroll_V002:
        {
        }
        break;
    case kControl_VScroll_V003:
        {
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
void uiGuildManagement::OnShowWindow(BOOL val)
{
    if (val)
    {
        RefreshTab();
    }
    else
    {
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildManagement::NetworkProc(MSG_BASE* recv_packet)
{
    if(recv_packet->m_byCategory == CG_GUILD){
        switch (recv_packet->m_byProtocol)
        {
        case CG_GUILD_WAREHOUSE_OPEN_ACK:
            {
                /*MSG_CG_GUILD_WAREHOUSE_OPEN_ACK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_OPEN_ACK*)recv_packet;

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

                control_flag().insert(kUseStart);*/
            }
            break;

        default:
            {
            }
            break;
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildManagement::OnUpdateSolarDialog()
{
    // 점령전 정보 출력
    UpdateDominationInfo();

    // 동맹/적대/적대대기 리스트 출력
    UpdateGuildRelationList();

    VScrollDialog::Update();
    if (enemy_vscroll())
    {
        enemy_vscroll()->Update();
    }
    if (wait_vscroll())
    {
        wait_vscroll()->Update();
    }
}
//========= 스크롤바 처리 =======================
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiGuildManagement::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
}
//------------------------------------------------------------------------------ 
bool uiGuildManagement::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L001)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L001)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildManagement::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L001))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildManagement::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L001))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildManagement::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L001))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void uiGuildManagement::OnClickGuildRelationButton()
{
    BYTE guild_grade = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGrade;
    bool have_tight = 
        GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, guild_grade);
    if (have_tight == true)
    {
        uiGuildRelation* relation_dialog = 
            GET_CAST_DIALOG(uiGuildRelation, uiGuildRelation::kDialog_ID);
        if (relation_dialog != NULL)
        {
            relation_dialog->ShowInterface(TRUE);
        }
    }
    else
    {
        // 7215 길드 레벨이 낮아 관계를 맺을 수 없습니다.
        GlobalFunc::VerifyMessage(7215);
    }
}
//------------------------------------------------------------------------------ 
void uiGuildManagement::OnClickDominationManagementButton()
{
    ContinentMapDlg* continent_map_dialog = 
        GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
    if (continent_map_dialog != NULL)
    {
        BOOL is_visible = continent_map_dialog->IsVisible();
        if (is_visible == FALSE)
        {
            continent_map_dialog->set_continent_map_mode(kContinentDominationManagerMode);
            continent_map_dialog->ShowInterface(TRUE);
        }
        else
        {
            if (continent_map_dialog->continent_map_mode() == kContinentNormalMode)
            {
                continent_map_dialog->set_continent_map_mode(kContinentDominationManagerMode);
            }
        }
    }
}
//------------------------------------------------------------------------------ 
void uiGuildManagement::RefreshTab()
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
        check_button->SetCheckState((position == kControl_CheckBT_C004) ? (true) : (false));
    }
}
//------------------------------------------------------------------------------ 
void uiGuildManagement::UpdateDominationInfo()
{
    TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };

    GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
    CCtrlImageStaticWZ* text_image_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I002));
    if (text_image_control != NULL)
    {
        // 점령지역 수
        g_InterfaceManager.GetInterfaceStringFormat(
            info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            3394, 
            guild_info.m_DominationMapCount);
        text_image_control->SetTextWZ(info_string);
    }

    text_image_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I005));
    if (text_image_control != NULL)
    {
        // 점령전 참여 횟수
        g_InterfaceManager.GetInterfaceStringFormat(
            info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            3395, 
            guild_info.m_DominationJoinCount);
        text_image_control->SetTextWZ(info_string);
    }

    text_image_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I004));
    if (text_image_control != NULL)
    {
        // 점령전 방어 성공 횟수
        g_InterfaceManager.GetInterfaceStringFormat(
            info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            3396, 
            guild_info.m_DominationDefenseCount);
        text_image_control->SetTextWZ(info_string);
    }

    text_image_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I006));
    if (text_image_control != NULL)
    {
        // 점령전 보스 사냥 횟수
        g_InterfaceManager.GetInterfaceStringFormat(
            info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            3397, 
            guild_info.m_DominationHuntingCount);
        text_image_control->SetTextWZ(info_string);
    }    
}
//------------------------------------------------------------------------------ 
void uiGuildManagement::UpdateGuildRelationList()
{
    // 동맹리스트
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
    if ((list_control != NULL) && 
        (scroll_control != NULL))
    {
        int current_select = list_control->GetCurSel();
        int begin_index = scroll_control->GetScrollPosWZ();

        list_control->DeleteItemAll();

        GuildSystem::GuildSystemData::GuildRelationContainer& relation_container = 
            GuildSystem::GetGuildSystemData().alliance_container();
        int list_count = relation_container.size();
        for (int list_index = 0; list_index < list_count; ++list_index)
        {
            GUILD_RELATION& guild_relation = relation_container[list_index];
            
            list_control->InsertItemBack();
            list_control->SetItemWZ(list_index, 
                0, 
                guild_relation.m_GuildName, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0, 0, 0, 0), 
                RGBA(100, 100, 250, 255), 
                RGBA(100, 100, 250, 255));
        }

        int scroll_range = list_count - list_control->GetDrawRowCount();
        scroll_range = (scroll_range > 0)?scroll_range:0;
        VScrollDialog::ResizeingVScrollControl(scroll_range, begin_index);
        list_control->SetBeginDrawIndex(begin_index);
        list_control->SetCurSel(current_select);

        CCtrlImageStaticWZ* text_image = 
            static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I000));
        if (text_image != NULL)
        {
            TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR number_string[INTERFACE_STRING_LENGTH] = {'\0', };
            _itot(list_count, number_string, 10);
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                INTERFACE_STRING_LENGTH - 1,
                2307, 
                number_string);
            text_image->SetTextWZ(info_string);
        }
    }

    // 적대리스트
    list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V003));
    if ((list_control != NULL) && 
        (scroll_control != NULL))
    {
        int current_select = list_control->GetCurSel();
        int begin_index = scroll_control->GetScrollPosWZ();

        list_control->DeleteItemAll();

        GuildSystem::GuildSystemData::GuildRelationContainer& relation_container = 
            GuildSystem::GetGuildSystemData().hostility_container();
        int list_count = relation_container.size();
        for (int list_index = 0; list_index < list_count; ++list_index)
        {
            GUILD_RELATION& guild_relation = relation_container[list_index];

            list_control->InsertItemBack();
            list_control->SetItemWZ(list_index, 
                0, 
                guild_relation.m_GuildName, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0, 0, 0, 0), 
                RGBA(204, 0, 0, 255), 
                RGBA(204, 0, 0, 255));
        }

        int scroll_range = list_count - list_control->GetDrawRowCount();
        scroll_range = (scroll_range > 0)?scroll_range:0;
        enemy_vscroll()->ResizeingVScrollControl(scroll_range, begin_index);

        list_control->SetBeginDrawIndex(begin_index);
        list_control->SetCurSel(current_select);

        CCtrlImageStaticWZ* text_image = 
            static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I001));
        if (text_image != NULL)
        {
            TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR number_string[INTERFACE_STRING_LENGTH] = {'\0', };
            _itot(list_count, number_string, 10);
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                INTERFACE_STRING_LENGTH - 1,
                2308, 
                number_string);
            text_image->SetTextWZ(info_string);
        }
    }

    // 적대대기리스트
    list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L002));
    scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V002));
    if ((list_control != NULL) && 
        (scroll_control != NULL))
    {
        int current_select = list_control->GetCurSel();
        int begin_index = scroll_control->GetScrollPosWZ();

        list_control->DeleteItemAll();

        GuildSystem::GuildSystemData::GuildRelationContainer& relation_container = 
            GuildSystem::GetGuildSystemData().wait_hostility_container();
        int list_count = relation_container.size();
        for (int list_index = 0; list_index < list_count; ++list_index)
        {
            GUILD_RELATION& guild_relation = relation_container[list_index];

            list_control->InsertItemBack();
            list_control->SetItemWZ(list_index, 
                0, 
                guild_relation.m_GuildName, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0, 0, 0, 0), 
                RGBA(249, 165, 44, 255), 
                RGBA(249, 165, 44, 255));
        }

        int scroll_range = list_count - list_control->GetDrawRowCount();
        scroll_range = (scroll_range > 0)?scroll_range:0;
        wait_vscroll()->ResizeingVScrollControl(scroll_range, begin_index);
        list_control->SetBeginDrawIndex(begin_index);
        list_control->SetCurSel(current_select);

        CCtrlImageStaticWZ* text_image = 
            static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I003));
        if (text_image != NULL)
        {
            TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR number_string[INTERFACE_STRING_LENGTH] = {'\0', };
            _itot(list_count, number_string, 10);
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                INTERFACE_STRING_LENGTH - 1,
                2309, 
                number_string);
            text_image->SetTextWZ(info_string);
        }
    }
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
