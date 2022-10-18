#include "SunClientPrecompiledHeader.h"
#include "uiDominationBettingList.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"
#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"
#include "DominationInfoParser.h"
#include "GlobalFunc.h"

uiDominationBettingList::uiDominationBettingList(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    is_portal_mode_ = false;
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL
}

uiDominationBettingList::~uiDominationBettingList()
{

}

void uiDominationBettingList::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControl();

    // List 컨트롤 세팅
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    CCtrlStaticWZ* list_tab1 = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    CCtrlStaticWZ* list_tab2 = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
#else
    CCtrlStaticWZ* list_tab1 = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    CCtrlStaticWZ* list_tab2 = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
#endif ///_NA_006826_20130722_DOMINATION_RENEWAL
    if ((list_control != NULL) && 
        (list_tab1 != NULL) && 
        (list_tab2 != NULL))
    {
        const int kColumn = 2;
        const int kListTextGap = 10;
        list_control->DeleteItemAll();
        list_control->SetColorSelBar(0);
        list_control->SetUseReport(true);
        list_control->SetReportUseCnt(kColumn);
        list_control->SetHideReportRender(true);
        int column_width = list_tab1->GetSize().right + kListTextGap;
        GlobalFunc::SetReportButtonInfo(list_control, 0, column_width, NULL);
        column_width = list_tab2->GetSize().right + kListTextGap;
        GlobalFunc::SetReportButtonInfo(list_control, 1, column_width, NULL);
    }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    CControlWZ* join_button = GetControlWZ_byPosition(kControl_Button_BT00);
    if (join_button != NULL)
    {
        join_button->EnableWindowWZ(false);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

void uiDominationBettingList::RegisterControl()
{
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000); // 점령전 신청 탭
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001); // 출전 신청 탭
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001); // 성문관리 버튼
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999); // 닫기 버튼
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000); // 입찰 리스트
    RegisterControlPosition(StrToWzID("V002"), kControl_VScroll_V002); // 리스트 스크롤

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001); // 점령지 관리 목록
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000); // 관리 현황
#else
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000); // 길드명 텍스트
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001); // 입찰금액 텍스트
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00); // 출전 신청 버튼
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01); // 입찰하기 버튼
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

void uiDominationBettingList::Release()
{

}

void uiDominationBettingList::MessageProc( SI_MESSAGE * message )
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
            if (is_portal_mode_ == true)
            {
                OnLButtonClickForPortal(message);
            }
            else
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL
            {
                OnLButtonClick(message);
            }
        }        
        break;
    case RT_MSG_LBUTTONDCLICK:
        {
            OnLButtonDClick(message);
        }        
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiDominationBettingList::OnShowWindow(BOOL val)
{
    if (val)
    {
        CCtrlButtonCheckWZ* betting_list_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C000));
        if (betting_list_tab != NULL)
        {
            betting_list_tab->SetCheckState(true);
        }
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        CCtrlButtonCheckWZ* join_list_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C001));
        if (join_list_tab != NULL)
        {
            join_list_tab->ShowWindowWZ(WZ_HIDE);
        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        //CControlWZ* join_member_button = GetControlWZ_byPosition(kControl_Button_BT00);
        //if (join_member_button == NULL)
        //{
        //    join_member_button->EnableWindowWZ(false);
        //}

        // 해당 창이 열릴 때는 대륙맵열릴 상태/대륙맵 여백에 위치해야함.
        uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
        if (ui_radar_manager)
        {
            ContinentMapNumber continent_map_nember = ui_radar_manager->GetCurrentContinentMapNumber();
            uiBase* current_continent_map = NULL;
            float position_offset_x = 0.0f;
            float position_offset_y = 0.0f;
            if (continent_map_nember == kContinentEP1MapNumber)
            {
                current_continent_map = GET_DIALOG(IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
                position_offset_x = 548.0f - 5.0f;  // 2012.09.28 / 송찬종 / 황세일 차장님 요청으로 Fix
                position_offset_y = 235.0f - 30.0f;
            }
            else if (continent_map_nember == kContinentEP2MapNumber)
            {
                current_continent_map = GET_DIALOG(IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
                position_offset_x = 21.0f;
                position_offset_y = 235.0f - 30.0f; // 2012.09.28 / 송찬종 / 황세일 차장님 요청으로 Fix
            }

            if (current_continent_map != NULL)
            {
                float position_x = current_continent_map->GetDialogWZ()->GetDlgX();
                float position_y = current_continent_map->GetDialogWZ()->GetDlgY();
                position_x += position_offset_x;
                position_y += position_offset_y;
                GetDialogWZ()->MoveWindowWZ(position_x, position_y);
            }
        }

        // 대륙맵 보다 상위로 열려야함.
        g_InterfaceManager.AttachFirstDialog(GetDialogWZ()->GetDlgID());

        // 키큐메시지삽입
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_Domination_Betting_List;
        msg.wParam = IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST;
        msg.DoSomething = uiDominationBettingList::CallBackESCKey;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_InterfaceManager.DeAttachFirstDialog(GetDialogWZ()->GetDlgID());

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_Domination_Betting_List);

        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
        if (list_control != NULL)
        {
            list_control->SetCurSel(-1);
            list_control->DeleteItemAll();
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
            is_portal_mode_ = false;
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL            
        }
    }
}

bool uiDominationBettingList::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    if (is_portal_mode_ == true)
    {
        if (control_position == kControl_Button_B999)
        {
            OnClickCloseButton();
        }

        return result;
    }
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL

    switch (control_position)
    {
    case kControl_CheckBT_C000:
        {
            OnClickBettingListTab();
        }
        break;
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    case kControl_CheckBT_C001:
        {
            OnClickJoinListTab();
        }
        break;
    case kControl_Button_BT01:
        {
            OnClickApplyButton();
        }
        break;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    case kControl_Button_B001:
        {
            OnClickGateManageButton();
        }
        break;
    case kControl_Button_B999:
        {
            OnClickCloseButton();
        }
        break;
    default:
        {

        }
        break;
    }
    return result;
}

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
bool uiDominationBettingList::OnLButtonClickForPortal(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

    if (control_position == kControl_Button_B999)
    {
        OnClickCloseButton();
    }

    return result;
}
#endif//_DEV_VER //_DH_FULL_MAP_PORTAL

bool uiDominationBettingList::OnLButtonDClick(SI_MESSAGE* message)
{
    bool result = true;

    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

    switch (control_position)
    {
    case kControl_List_L000:
        {
            OnDClickList();            
        }
        break;
    }

    return result;
}

void uiDominationBettingList::SetShowButtonFlag(MSG_CG_DOMINATION_NOTICE_ACK* recv_packet)
{
    bool enable_button = false;
    CControlWZ* button_control = GetControlWZ_byPosition(kControl_Button_B001);
    if (button_control != NULL)
    {
        enable_button = (recv_packet->m_ManageFlag == TRUE) ? true : false;
        button_control->EnableWindowWZ(enable_button);
    }

    //button_control = GetControlWZ_byPosition(kControl_Button_BT01);
    //if (button_control != NULL)
    //{
    //    enable_button = (recv_packet->m_AuctionFlag == TRUE) ? true : false;
    //    button_control->EnableWindowWZ(enable_button);
    //}
}

void uiDominationBettingList::SetBettingListData(MSG_CG_DOMINATION_APPLY_ACK* recv_packet)
{
    // 리스트 세팅
    DOMINATION_ACTION_INFO* auction_info = recv_packet->m_ActionList;
    DWORD info_count = recv_packet->m_DataNum;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    // 입찰하기 버튼 활성화 여부 체크
    CCtrlButtonWZ* apply_button = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));
    if (apply_button != NULL)
    {
        bool enable_button = (recv_packet->m_CheckBatting == TRUE) ? true : false;
        apply_button->EnableWindowWZ(enable_button);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control == NULL)
    {
        return;
    }

    list_control->DeleteItemAll();

    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR money_string[INTERFACE_STRING_LENGTH] = {0,};
    NUMBERFMT number_format = {0, 0, 3, _T("."), _T(","), 1};
    MONEY offer_cost = 0;

    DWORD list_index = 0;
#ifdef _INTERNATIONAL_UI
    DWORD text_font = StrToWzID("st10");
#else
    DWORD text_font = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
    DWORD text_color;

    for (DWORD index = 0; index < info_count; ++index)
    {
        if (auction_info[index].m_GuilGuid)
        {
            list_index = list_control->InsertItemBack();
            if ( 0 == list_index )
            {
                text_color = RGBA(76,240,4,255);
            }
            else
            {
                text_color = RGBA(200,200,200,255);
            }

            // 입찰 순위
            Snprintf(result_string, INTERFACE_STRING_LENGTH, "%d", list_index + 1);
            GlobalFunc::SetItem(list_control, list_index, 0, result_string, 
                text_font, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), text_color, text_color);

            // 길드명
            GlobalFunc::SetItem(list_control, list_index, 1, auction_info[index].m_tszGuildName, 
                text_font, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), text_color, text_color);
        }
        else
        {
            break;
        }
    }

    // 지역점령전 필드별로 설정된 기본입찰금액과 비교하여 입찰가능 금액계산
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    // 파서 변경시 수정 - 필요한 함수 추가
    MAPCODE mapcode = ui_domination_manager->GetDominationManagedMapCode();
    const sDOMINATION_INFO* domination_info = 
        DominationInfoParser::Instance()->GetDominationInfo(mapcode);
    offer_cost++;
    if (mapcode != 0 && domination_info != NULL)
    {
        offer_cost = max(offer_cost, (domination_info->m_BaseBattingMoney));
        ui_domination_manager->SetMostOfferHeim(offer_cost);
    }
}

void uiDominationBettingList::OnClickBettingListTab()
{
    CCtrlButtonCheckWZ* betting_list_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C000));
    if (betting_list_tab != NULL)
    {
        betting_list_tab->SetCheckState(true);
    }    
}

void uiDominationBettingList::OnClickJoinListTab()
{
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    CCtrlButtonCheckWZ* join_list_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C001));
    if (join_list_tab != NULL)
    {
        join_list_tab->SetCheckState(false);
    }

    uiBase* join_member_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST2);
    if (join_member_list != NULL)
    {
        join_member_list->ShowInterface(TRUE);
    }

    ShowInterface(FALSE);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

void uiDominationBettingList::OnClickApplyButton()
{
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    ui_domination_manager->SEND_DOMINATION_BATTING_HEIM_SYN();
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

void uiDominationBettingList::OnClickGateManageButton()
{
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    ui_domination_manager->SEND_DOMINATION_MANAGE_SYN();
}

void uiDominationBettingList::OnClickCloseButton()
{
    ShowInterface(FALSE);

    // 대륙맵모드 세팅
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager != NULL)
    {
        ContinentMapMode continent_map_mode = ui_radar_manager->GetCurrentContinentMapMode();
        ContinentMapNumber continent_map_number = ui_radar_manager->GetCurrentContinentMapNumber();
        uiBase* continent_map_dialog = ui_radar_manager->GetCurrentContinentMap();
        if (continent_map_dialog != NULL)
        {
            if (continent_map_number == kContinentEP1MapNumber)
            {
                ContinentMapDlg* continent_map_ep1 = static_cast<ContinentMapDlg*>(continent_map_dialog);
                if (continent_map_mode == kContinentDominationManagerSelectedAreaMode)
                {
                    continent_map_ep1->set_continent_map_mode(kContinentDominationManagerMode);
                }                
            }
            else if (continent_map_number == kContinentEP2MapNumber)
            {
                ContinentEp2MapDlg* continent_map_ep2 = static_cast<ContinentEp2MapDlg*>(continent_map_dialog);
                if (continent_map_mode == kContinentDominationManagerSelectedAreaMode)
                {
                    continent_map_ep2->set_continent_map_mode(kContinentDominationManagerMode);
                }
            }
        }
    }
}

void uiDominationBettingList::CallBackESCKey(DWORD wParam,DWORD lParam)
{
    uiDominationBettingList* betting_list = GET_CAST_DIALOG(uiDominationBettingList, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST);
    if (betting_list != NULL)
    {
        betting_list->OnClickCloseButton();
    }
}

void uiDominationBettingList::OnDClickList()
{
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return;
    }

    bool is_enable_use_portal = false;
    ContinentMapNumber continent_map_number = ui_radar_manager->GetCurrentContinentMapNumber();
    if (continent_map_number == kContinentEP1MapNumber)
    {
        ContinentMapDlg* continent_map = GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
        if (continent_map != NULL)
        {

            if (continent_map->continent_map_mode() == kContinentDominationManagerSelectedAreaMode)
            {
                OnClickGateManageButton();
            }
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
            else if ((continent_map->continent_map_mode() == kContinentNormalMode) && 
                 (continent_map->get_is_actived_map_portal() == true) && 
                 (continent_map->get_is_useable_map_portal() == true))
            {
                is_enable_use_portal = true;
            }
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL
        }
    }
    else if (continent_map_number == kContinentEP2MapNumber)
    {
        ContinentEp2MapDlg* continent_map = GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
        if (continent_map != NULL)
        {
            if (continent_map->continent_map_mode() == kContinentDominationManagerSelectedAreaMode)
            {
                OnClickGateManageButton();
            }
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
            else if ((continent_map->continent_map_mode() == kContinentNormalMode) && 
                (continent_map->get_is_actived_map_portal() == true) && 
                (continent_map->get_is_useable_map_portal() == true))
            {
                is_enable_use_portal = true;
            }
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL
        }
    }

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    if (is_enable_use_portal == true)
    {
        DoubleClickMapPortal();
    }
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL
}

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
void uiDominationBettingList::DoubleClickMapPortal()
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control == NULL)
    {
        return;
    }

    int select_index = list_control->GetCurSel();
    if (select_index >= 0 && select_index < kContinentMapPortalMax)
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        Sprintf(message, "go %d", map_portal_infomations_[select_index]);
        MSG_CG_GM_STRING_CMD_SYN send_packet;
        ZeroMemory(send_packet.m_szStringCmd, MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
        StrnCopy(send_packet.m_szStringCmd, message, MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
}

void uiDominationBettingList::GetMapPortalFromControl(int continent_map_number, int continent_positon)
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control == NULL)
    {
        return;
    }

    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return;
    }

    int count = MapInfoParser::Instance()->FindMapPortalInfo(
        map_portal_infomations_, 
        kContinentMapPortalMax, 
        continent_positon + 1, 
        continent_map_number);

    TCHAR map_name[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    DWORD list_index = 0;
    DWORD text_color = RGBA(255, 255, 255, 255);
    DWORD selected_text_color = RGBA(0,220,0,255);
    DWORD background_color = RGBA(0,0,0,0);

    list_control->DeleteItemAll();

#ifdef _INTERNATIONAL_UI
    DWORD text_font = StrToWzID("st10");
#else
    DWORD text_font = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

    for (int index = 0; index < count; ++index)
    {
        const sMAPINFO *map_info = 
            MapInfoParser::Instance()->FindMapInfo(map_portal_infomations_[index]);
        if (map_info != NULL)
        {
            int control_index = 0;
            list_control->ShowWindowWZ(WZ_SHOW);
            list_control->SetCurSel(-1);
            list_index = list_control->InsertItemBack();

            g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name, INTERFACE_STRING_LENGTH);
            GlobalFunc::SetItem(list_control, list_index, 0, 
                map_name, text_font, DT_VCENTER| DT_CENTER, background_color, text_color, selected_text_color);

            sprintf(buffer, "%d", map_info->MapCode);
            GlobalFunc::SetItem(list_control, list_index, 1, buffer, text_font, 
                DT_VCENTER| DT_CENTER, background_color, text_color, selected_text_color);

            
        }
    }

    if (list_control->GetListRowCnt() > 0)
    {
        // 현재 맵 상태 변경
        if (continent_map_number == kContinentEP1MapNumber)
        {
            ContinentMapDlg* continent_map = GET_CAST_DIALOG(ContinentMapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_DLG);
            if (continent_map != NULL)
            {
                continent_map->set_is_actived_map_portal(true);
            }
        }
        else if (continent_map_number == kContinentEP2MapNumber)
        {
            ContinentEp2MapDlg* continent_map = GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);
            if (continent_map != NULL)
            {
                continent_map->set_is_actived_map_portal(true);
            }
        }

        is_portal_mode_ = true;
    }
}

#endif //_DEV_VER //_DH_FULL_MAP_PORTAL

#ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
void uiDominationBettingList::SetManageListData(MSG_CG_DOMINATION_MANAGE_ACK* recv_packet)
{
    // 리스트 세팅
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control == NULL)
    {
        return;
    }

    list_control->DeleteItemAll();

    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,};

    DWORD list_index = 0;
#ifdef _INTERNATIONAL_UI
    DWORD text_font = StrToWzID("st10");
#else
    DWORD text_font = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

    DWORD selected_text_color = RGBA(0,220,0,255);

    list_index = list_control->InsertItemBack();

    // 성벽 관리 3287
    g_InterfaceManager.GetInterfaceString(3287, result_string, INTERFACE_STRING_LENGTH);
    GlobalFunc::SetItem(list_control, list_index, 0, result_string, 
        text_font, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), RGBA(255,255,255,255), selected_text_color);

    // 성벽 개수
    BYTE current_count = 0;
    for (int i = 0; i < MAX_DOMINATION_DEFENSEWALL_NUM; ++i)
    {
        current_count += recv_packet->m_DefenseWall[i];
    }
    Snprintf(result_string, INTERFACE_STRING_LENGTH, "%d / %d", current_count, recv_packet->m_TotalPoint);
    GlobalFunc::SetItem(list_control, list_index, 1, result_string, 
        text_font, DT_VCENTER| DT_CENTER, RGBA(0,0,0,0), RGBA(255,255,255,255), selected_text_color);
}
#endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL