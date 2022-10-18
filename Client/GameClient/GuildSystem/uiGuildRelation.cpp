#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "uiGuildRelation.h"
#include "InterfaceManager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"

const DWORD uiGuildRelation::kDialog_ID = StrToWzID("7326");

//----------------------------------------------------------------------------
/**
*/
uiGuildRelation::uiGuildRelation(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
}


//----------------------------------------------------------------------------
/**
*/
uiGuildRelation::~uiGuildRelation(void) 
{
    Release();
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildRelation::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B998"), kControl_Button_B998);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("VS02"), kControl_VScroll_VS02);

    VScrollDialog::ResizeingVScrollControl(40, 0);

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control != NULL)
    {
        list_control->SetUseReport(true);
        //list_control->SetUnuseSelBar(false);
        list_control->SetColorSelBar(RGBA(150, 10, 10, 80));
        list_control->SetSellHeight(19);
        list_control->SetReportUseCnt(2);
        list_control->SetHideReportRender(TRUE);
        list_control->SetReportButtonInfo(0, 200, NULL); // 길드원 이름
        list_control->SetReportButtonInfo(1, 260, NULL); // 레벨
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelation::Release()
{
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelation::MessageProc(SI_MESSAGE* message)
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
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_VS02)
            {
                OnVScrollUp(message);
            }           
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_VS02)
            {
                OnVScrollDown(message);
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_VS02)
            {
                OnVScrollThumb(message);
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
bool uiGuildRelation::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C004:    
        {
            SetCurrentListIndex(control_position - kControl_CheckBT_C001);
        }
        break;    
    case kControl_Button_B005:
        {
            OnClickRelationButton();
        }
        break;
    case kControl_Button_B001:
        {
            OnClickDeleteButton();
        }
        break;
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B998:
        {
        }
        break;
    case kControl_Button_B000:
        {
        }
        break;
    
    case kControl_Text_T999:
        {
        }
        break;
    case kControl_Text_S000:
        {
        }
        break;
    case kControl_List_L000:
        {
        }
        break;
    case kControl_VScroll_VS02:
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
void uiGuildRelation::OnShowWindow(BOOL val)
{
    if (val)
    {
        SetCurrentListIndex(kAllienceList);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildRelation::kDialog_ID;
        msg.wParam = uiGuildRelation::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildRelation::kDialog_ID);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelation::NetworkProc(MSG_BASE* recv_packet)
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
void uiGuildRelation::OnUpdateSolarDialog()
{
    // 길드관계 리스트 업데이트
    UpdateGuildRelationList();

    UpdateRelationButtonState();

    UpdateWheelScroll();

    VScrollDialog::Update();
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiGuildRelation::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_VS02));
}
//------------------------------------------------------------------------------ 
bool uiGuildRelation::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildRelation::OnVScrollThumb(SI_MESSAGE* msg)
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
bool uiGuildRelation::OnVScrollUp(SI_MESSAGE* msg)
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
bool uiGuildRelation::OnVScrollDown(SI_MESSAGE* msg)
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
void uiGuildRelation::OnClickRelationButton()
{
    TCHAR guild_name[MAX_GUILDNAME_LENGTH + 1] = {'\0', };
    switch (current_list_index())
    {
    case kAllienceList:
        {
            // 동맹신청
            ShowRelationAsk(uiGuildRelationAsk::kGuild_Alliance_Relation, guild_name);
        }
        break;
    case kHostilityList:
        {
            // 적대신청
            ShowRelationAsk(uiGuildRelationAsk::kGuild_Hostility_Relation, guild_name);
        }
        break;
    case kWaitHostilityList:
        {
            // 적대수락
            GUILD_RELATION* guild_relation = GetSelectedGuildRelation();
            if ((guild_relation != NULL) &&
                (guild_relation->m_eRelationType == eGUILD_RELATION_ONE_SIDE_HOSTILITY))
            {
                strncpy(guild_name, guild_relation->m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_name) - 1] = '\0';
            }
            ShowRelationAsk(uiGuildRelationAsk::kGuild_Answer_Hostility_Relation, guild_name);
        }
        break;
    }
}
//------------------------------------------------------------------------------ 
void uiGuildRelation::OnClickDeleteButton()
{
    TCHAR guild_name[MAX_GUILDNAME_LENGTH + 1] = {'\0', };
    switch (current_list_index())
    {
    case kAllienceList:
        {
            // 동맹해제
            GUILD_RELATION* guild_relation = GetSelectedGuildRelation();
            if ((guild_relation != NULL) &&
                (guild_relation->m_eRelationType == eGUILD_RELATION_ALLIENCE))
            {
                strncpy(guild_name, guild_relation->m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_name) - 1] = '\0';
            }
            ShowRelationAsk(uiGuildRelationAsk::kGuild_Release_Alliance_Relation, guild_name);
        }
        break;
    case kHostilityList:
        {
            // 적대해제
            GUILD_RELATION* guild_relation = GetSelectedGuildRelation();
            if ((guild_relation != NULL) &&
                (guild_relation->m_eRelationType == eGUILD_RELATION_BOTH_HOSTILITY))
            {
                strncpy(guild_name, guild_relation->m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_name) - 1] = '\0';
            }
            ShowRelationAsk(uiGuildRelationAsk::kGuild_Release_Hostility_Relation, guild_name);
        }
        break;
    case kWaitHostilityList:
        {
            // 적대선포취소
            GUILD_RELATION* guild_relation = GetSelectedGuildRelation();
            if ((guild_relation != NULL) &&
                (guild_relation->m_eRelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY))
            {
                strncpy(guild_name, guild_relation->m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_name) - 1] = '\0';
            }
            ShowRelationAsk(uiGuildRelationAsk::kGuild_Cancel_Hostility_Relation, guild_name);
        }
        break;
    }
}
//------------------------------------------------------------------------------ 
void uiGuildRelation::ShowRelationAsk( uiGuildRelationAsk::GuildAskType ask_type, TCHAR* guild_name )
{
    uiGuildRelationAsk* relation_ask = GET_CAST_DIALOG(uiGuildRelationAsk, uiGuildRelationAsk::kDialog_ID);
    if (relation_ask != NULL)
    {
        relation_ask->set_guild_ask_type(ask_type);
        relation_ask->ShowInterface(TRUE);
        relation_ask->SetEditText(guild_name);
    }
}
//------------------------------------------------------------------------------ 
void uiGuildRelation::SetCurrentListIndex( DWORD list_index )
{
    set_current_list_index(list_index);

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control != NULL)
    {
        list_control->SetCurSel(-1);
        list_control->SetBeginDrawIndex(0);
    }
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    if (scroll_control != NULL)
    {
        scroll_control->SetScrollRangeWZ(0, 0);
        scroll_control->SetScrollPosWZ(0);
    }

    // 상태에 따른 버튼명 변경
    CCtrlButtonWZ* relation_list_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B998));
    CCtrlButtonWZ* relation_state_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B000));
    CCtrlButtonWZ* relation_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B005));
    CCtrlButtonWZ* release_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001));
    if ((relation_list_button != NULL) &&
        (relation_state_button != NULL) && 
        (relation_button != NULL) && 
        (release_button != NULL))
    {
        switch (current_list_index())
        {
        case kAllienceList:
            {
                relation_list_button->SetTextID(1071);
                relation_state_button->SetTextID(7348);
                relation_button->SetTextID(1048);
                release_button->SetTextID(1172);
            }
            break;
        case kHostilityList:
            {
                relation_list_button->SetTextID(1072);
                relation_state_button->SetTextID(7349);
                relation_button->SetTextID(1049);
                release_button->SetTextID(1173);
            }
            break;
        case kWaitHostilityList:
            {
                relation_list_button->SetTextID(2306);
                relation_state_button->SetTextID(7350);
                relation_button->SetTextID(7346);
                release_button->SetTextID(7347);
            }
            break;
        }
    }

    RefreshTab();    
}
//------------------------------------------------------------------------------ 
void uiGuildRelation::UpdateGuildRelationList()
{
    DWORD guild_name_color = RGBA(255, 255, 255, 255);
    GuildSystem::GuildSystemData::GuildRelationContainer* relation_container = NULL;
    switch (current_list_index())
    {
    case kAllienceList:
        {
            relation_container = &(GuildSystem::GetGuildSystemData().alliance_container());
            guild_name_color = RGBA(100, 100, 250, 255);
        }
        break;
    case kHostilityList:
        {
            relation_container = &(GuildSystem::GetGuildSystemData().hostility_container());
            guild_name_color = RGBA(204, 0, 0, 255);
        }
        break;
    case kWaitHostilityList:
        {
            relation_container = &(GuildSystem::GetGuildSystemData().wait_hostility_container());
            guild_name_color = RGBA(249, 165, 44, 255);
        }
        break;
    }

    if (relation_container == NULL)
    {
        return;
    }

    STLX_STRING relation_state;
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    if ((list_control != NULL) && 
        (scroll_control != NULL))
    {
        int current_select = list_control->GetCurSel();
        int begin_index = scroll_control->GetScrollPosWZ();

        list_control->DeleteItemAll();

        int list_count = relation_container->size();
        for (int list_index = 0; list_index < list_count; ++list_index)
        {
            GUILD_RELATION& guild_relation = (*relation_container)[list_index];

            list_control->InsertItemBack();
            list_control->SetItemWZ(list_index, 
                0, 
                guild_relation.m_GuildName, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0, 0, 0, 0), 
                guild_name_color, 
                guild_name_color);

            relation_state.clear();
            if (guild_relation.m_eRelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY)
            {
                relation_state += g_InterfaceManager.GetInterfaceString(7351);
            }
            else if (guild_relation.m_eRelationType == eGUILD_RELATION_ONE_SIDE_HOSTILITY)
            {
                relation_state += g_InterfaceManager.GetInterfaceString(7352);
            }

            TCHAR date_string[INTERFACE_STRING_LENGTH] = {'\0', };
            SYSTEMTIME system_time;
            switch (current_list_index())
            {
            case kAllienceList:
                {
                    util::YYYYMMDDHHMMSSToSYSTEMTIME(guild_relation.m_RelationApplyTime, system_time);
                }
                break;
            case kHostilityList:
            case kWaitHostilityList:
                {
                    util::YYYYMMDDHHMMSSToSYSTEMTIME(guild_relation.m_RelationChangeTime, system_time);
                }
                break;
            }

    #ifdef _INTERNATIONAL_DATE_TOOLTIP

            TCHAR month_string[32] = {'\0', };
            int month_string_code = 11071;
            uiToolTipMan* ui_tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (ui_tooltip_manager != NULL)
            {
                month_string_code = ui_tooltip_manager->GetStringMonth(system_time.wMonth);
            }
            g_InterfaceManager.GetInterfaceString(month_string_code, month_string, 32);

        #ifdef _RUSSIA
            Snprintf(date_string, 
                INTERFACE_STRING_LENGTH - 1, 
                "%s.%2d.%4d (%2d:%2d)", 
                month_string,        //달
                system_time.wDay,    //일
                system_time.wYear,   //년도
                system_time.wHour,   //시
                system_time.wMinute);//분
        #else //_RUSSIA
            Snprintf(date_string, 
                INTERFACE_STRING_LENGTH - 1, 
                "%s %02d (%02d:%02d), %4d", 
                month_string,       //달
                system_time.wDay,   //일
                system_time.wHour,  //시
                system_time.wMinute,//분 
                system_time.wYear); //년도
        #endif //_RUSSIA

    #else

            Snprintf(date_string, 
                INTERFACE_STRING_LENGTH - 1, 
                "%4d-%02d-%02d(%02d:%02d)", 
                system_time.wYear, 
                system_time.wMonth, 
                system_time.wDay, 
                system_time.wHour, 
                system_time.wMinute );
    #endif//_INTERNATIONAL_DATE_TOOLTIP
            relation_state += " ";
            relation_state += date_string;

            list_control->SetItemWZ(list_index, 
                1, 
                relation_state.c_str(), 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0, 0, 0, 0), 
                guild_name_color, 
                guild_name_color);
        }

        int scroll_range = list_count - list_control->GetDrawRowCount();
        int range_min = 0;
        int range_max = 0;
        scroll_range = max(0, scroll_range);
        begin_index = max(0, begin_index);
        begin_index = min(begin_index, scroll_range);

        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        if (scroll_range != range_max)
        {
            scroll_control->SetScrollRangeWZ(0, scroll_range);
            scroll_control->SetScrollPosWZ(begin_index);
        }        

        list_control->SetBeginDrawIndex(begin_index);
        list_control->SetCurSel(current_select);
    }
}

void uiGuildRelation::UpdateRelationButtonState()
{
    CCtrlButtonWZ* relation_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B005));
    CCtrlButtonWZ* delete_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001));
    if ((relation_button == NULL) || 
        (delete_button == NULL))
    {
        assert(relation_button);
        assert(delete_button);
        return;
    }

    relation_button->EnableWindowWZ(true);
    delete_button->EnableWindowWZ(true);

    if (current_list_index() != kWaitHostilityList)
    {
        return;
    }

    GUILD_RELATION* guild_relation = GetSelectedGuildRelation();
    if (guild_relation == NULL)
    {
        return;
    }

    if (guild_relation->m_eRelationType == eGUILD_RELATION_ONE_SIDE_HOSTILITY)
    {
        delete_button->EnableWindowWZ(false);
    }
    else if (guild_relation->m_eRelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY)
    {
        relation_button->EnableWindowWZ(false);
    }
}

void uiGuildRelation::UpdateWheelScroll()
{
    CControlWZ* wheel_rect_control = GetControlWZ(StrToWzID("P000"));
    if (wheel_rect_control == NULL)
    {
        return;
    }

    RECT wheel_rect	= wheel_rect_control->GetSizeRect();
    switch (GetMouseWheelStatus(&wheel_rect))
    {
    case MOUSE_WHEEL_UP:
        OnVScrollUp(NULL);
        break; 
    case MOUSE_WHEEL_DOWN:
        OnVScrollDown(NULL);
        break;
    }
}

void uiGuildRelation::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C001; position < (kControl_CheckBT_C004 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((current_list_index() == position) ? (true) : (false));
    }
}

GUILD_RELATION* uiGuildRelation::GetSelectedGuildRelation()
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control == NULL)
    {
        return NULL;
    }

    int selected_index = list_control->GetCurSel();
    if (selected_index < 0)
    {
        return NULL;
    }

    GuildSystem::GuildSystemData::GuildRelationContainer* relation_container = NULL;
    switch (current_list_index())
    {
    case kAllienceList:
        {
            relation_container = &(GuildSystem::GetGuildSystemData().alliance_container());
        }
        break;
    case kHostilityList:
        {
            relation_container = &(GuildSystem::GetGuildSystemData().hostility_container());
        }
        break;
    case kWaitHostilityList:
        {
            relation_container = &(GuildSystem::GetGuildSystemData().wait_hostility_container());
        }
        break;
    }

    if (relation_container == NULL)
    {
        return NULL;
    }

    int list_count = relation_container->size();
    if (selected_index >= list_count)
    {
        return NULL;
    }

    GUILD_RELATION* guild_relation = &(*relation_container)[selected_index];

    return guild_relation;
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
