#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildLog.h"
#include "GuildSystem/uiGuildInfo.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "GameFunc.h"
#include "MultiVScrollDialog.h"
#include "ItemInfoParser.h"
#include "uiToolTipMan/uiToolTipMan.h"

const DWORD uiGuildLog::kDialog_ID = StrToWzID("7345");

//----------------------------------------------------------------------------
/**
*/
uiGuildLog::uiGuildLog(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
    set_warehouse_log_type(kMaxWareHouseLog);
    set_ware_house_vscroll(NULL);
}


//----------------------------------------------------------------------------
/**
*/
uiGuildLog::~uiGuildLog(void) 
{
    Release();
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildLog::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009);
    RegisterControlPosition(StrToWzID("C010"), kControl_CheckBT_C010);
    RegisterControlPosition(StrToWzID("C012"), kControl_CheckBT_C012);
#ifdef _NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("C011"), kControl_CheckBT_C011);
#endif //_NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);

    VScrollDialog::ResizeingVScrollControl(40, 0);
    set_ware_house_vscroll(new MultiVScrollDialog(GetControlWZ_byPosition(kControl_VScroll_V001),
        GetControlWZ_byPosition(kControl_List_L001)));
    ware_house_vscroll()->ResizeingVScrollControl(0, 0);

    list_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000)));
    assert(list_control_);
    if (list_control_ != NULL)
    {
        list_control_->SetUseReport(true);
        list_control_->SetSellHeight(19);
        list_control_->SetReportUseCnt(2);
        list_control_->SetHideReportRender(TRUE);
        list_control_->SetReportButtonInfo(0, 160, NULL); // 시간
        list_control_->SetReportButtonInfo(1, 441, NULL); // 내용
        list_control_->SetUnuseSelBar(true);
    }

    CCtrlListWZ* log_list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    if (log_list_control != NULL)
    {
        log_list_control->SetUseReport(true);
        log_list_control->SetSellHeight(19);
        log_list_control->SetReportUseCnt(3);
        log_list_control->SetHideReportRender(TRUE);
        log_list_control->SetReportButtonInfo(0, 160, NULL); // 이용시간
        log_list_control->SetReportButtonInfo(1, 175, NULL); // 캐릭터명
        log_list_control->SetReportButtonInfo(2, 264, NULL); // 이용내역
    }

    set_refresh_wait_time(0);
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildLog::Release()
{
    if (ware_house_vscroll())
    {
        delete(ware_house_vscroll());
        set_ware_house_vscroll(NULL);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildLog::MessageProc(SI_MESSAGE* message)
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
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001 && ware_house_vscroll())
            {
                ware_house_vscroll()->OnVScrollUp(message);
            } 
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnVScrollDown(message);
            }
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001 && ware_house_vscroll())
            {
                ware_house_vscroll()->OnVScrollDown(message);
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
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001 && ware_house_vscroll())
            {
                ware_house_vscroll()->OnVScrollThumb(message);
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
bool uiGuildLog::OnLButtonClick(SI_MESSAGE* message)
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
    case kControl_CheckBT_C011:
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
    case kControl_CheckBT_C005:
    case kControl_CheckBT_C007:
    case kControl_CheckBT_C008:
    case kControl_CheckBT_C009:
    case kControl_CheckBT_C006:
        {
            GuildLogInfoType select_type = 
                static_cast<GuildLogInfoType>(control_position - kControl_CheckBT_C005);
            if (select_type != guild_info_log_type())
            {
                InitGuildInfoLog();
                set_guild_info_log_type(select_type);
                SetGuildLogUpList();
            }
        }
        break;
    case kControl_CheckBT_C012:
    case kControl_CheckBT_C010:
        {
            WareHouseLogType select_type = 
                static_cast<WareHouseLogType>(control_position - kControl_CheckBT_C012);
            if (select_type != warehouse_log_type())
            {
                InitGuildWareHouseLog();
                set_warehouse_log_type(select_type);
                UpdateGuildWareHouseLog();
            }
        }
        break;
    }
    return result;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildLog::OnShowWindow(BOOL val)
{
    if (val)
    {
        // 길드 창고 로그 요청
        // [GUILD_RENEWAL] 주기적으로 요청하는 것으로 변경 필요
        GuildSystem::PacketFunction::SEND_CW_GUILD_WAREHOUSE_LOGLIST_REQ();
        GuildSystem::PacketFunction::SEND_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ();

        set_warehouse_log_type(kDepositLog);
        set_guild_info_log_type(kAllLog);
    }
    else
    {
        set_warehouse_log_type(kMaxWareHouseLog);
        set_guild_info_log_type(kMaxGuildLog);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildLog::NetworkProc(MSG_BASE* recv_packet)
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
void uiGuildLog::SetGuildLogUpList()
{
    CCtrlListWZ* log_list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    CCtrlVScrollWZ* log_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if ((log_list_control == NULL) || 
        (log_scroll == NULL))
    {
        assert(log_list_control);
        assert(log_scroll);
        return;
    }

    GuildSystem::GuildSystemData::GuildLogInfoContainer& guild_info_log_container = 
        GuildSystem::GetGuildSystemData().guild_log_info_container();

    int begin_index = log_list_control->GetBeginDrawIndex();

    log_list_control->DeleteItemAll();

    int log_list_index = 0;
    std::vector<util::_tstring> temp_string_container;
    TCHAR log_string[INTERFACE_STRING_LENGTH] = {'\0', };
    GuildSystem::GuildSystemData::GuildLogInfoContainer::iterator log_begin_itr = guild_info_log_container.begin();
    GuildSystem::GuildSystemData::GuildLogInfoContainer::iterator log_end_itr = guild_info_log_container.end();
    for ( ; log_begin_itr != log_end_itr; ++log_begin_itr)
    {
        GuildSystem::GuildSystemData::GuildLogInfo& log_info = log_begin_itr->second;

        if ((guild_info_log_type() != kAllLog) && 
            (guild_info_log_type() != (log_info.guild_log_category + 1)))
        {
            continue;
        }

        log_list_index = log_list_control->InsertItemBack();
        DWORD log_color = GetGuildLogColor(log_info.guild_log_data.log_type_);

        // 시간
        GetLogDateString(log_info.guild_log_data.log_time_, log_string, INTERFACE_STRING_LENGTH);
        log_list_control->SetItemWZ(log_list_index, 
            0, 
            log_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            log_color, 
            log_color);

        // 내용
        GetGuildLogInfoString(log_info.guild_log_data, log_string, INTERFACE_STRING_LENGTH);
        temp_string_container.clear();
        GlobalFunc::ProcessWordWrapByFontSize(&temp_string_container, 
            log_string, 
            StrToWzID("st11"), 
            _tcslen(log_string), 
            425);

        std::vector<util::_tstring>::iterator begin_itr = temp_string_container.begin();
        std::vector<util::_tstring>::iterator end_itr = temp_string_container.end();
        if (begin_itr != end_itr)
        {
            log_list_control->SetItemWZ(log_list_index, 
                1, 
                begin_itr->c_str(), 
                StrToWzID("st11"), 
                DT_LEFT | DT_VCENTER, 
                RGBA(0,0,0,0), 
                log_color, 
                log_color);
            ++begin_itr;

            for ( ; begin_itr != end_itr; ++begin_itr)
            {
                log_list_index = log_list_control->InsertItemBack();
                log_list_control->SetItemWZ(log_list_index, 
                    1, 
                    begin_itr->c_str(), 
                    StrToWzID("st11"), 
                    DT_LEFT | DT_VCENTER, 
                    RGBA(0,0,0,0), 
                    log_color, 
                    log_color);
            }
        }        
    }

    int scroll_range = (log_list_index - log_list_control->GetDrawRowCount());
    scroll_range = max(0, scroll_range);
    //begin_index = max(0, begin_index);
    //begin_index = min(begin_index, scroll_range);

    ResizeingVScrollControl(scroll_range, begin_index);
    log_list_control->SetBeginDrawIndex(begin_index);
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildLog::OnUpdateSolarDialog()
{
    VScrollDialog::Update();
    if (ware_house_vscroll())
    {
        ware_house_vscroll()->Update();
    }

    // 탭버튼의 상태갱신
    CCtrlButtonCheckWZ* check_button = NULL;
    for (DWORD tab_index = kAllLog; tab_index < kMaxGuildLog; ++tab_index)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(tab_index + kControl_CheckBT_C005));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((tab_index == guild_info_log_type()) ? (true) : (false));
    }

    check_button = NULL;
    for (int tab_index = kDepositLog; tab_index < kMaxWareHouseLog; ++tab_index)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(tab_index + kControl_CheckBT_C012));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((tab_index == warehouse_log_type()) ? (true) : (false));
    }
}
//------------------------------------------------------------------------------ 
void uiGuildLog::Process( DWORD dwTick )
{
    if (IsVisible() == TRUE)
    {
        set_refresh_wait_time(refresh_wait_time() + dwTick);
        if (refresh_wait_time() >= kGuildLogRefreshTime)
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ();
            set_refresh_wait_time(0);
        }
    }
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiGuildLog::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiGuildLog::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildLog::OnVScrollThumb(SI_MESSAGE* msg)
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
bool uiGuildLog::OnVScrollUp(SI_MESSAGE* msg)
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
bool uiGuildLog::OnVScrollDown(SI_MESSAGE* msg)
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

void uiGuildLog::RefreshTab()
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
        check_button->SetCheckState((position == kControl_CheckBT_C002) ? (true) : (false));
    }
}

void uiGuildLog::UpdateGuildWareHouseLog()
{
    CCtrlListWZ* log_list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    CCtrlVScrollWZ* log_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
    if ((log_list_control == NULL) || 
        (log_scroll == NULL))
    {
        assert(log_list_control);
        assert(log_scroll);
        return;
    }

    using namespace GuildSystem;
    GuildSystemData::GuildWareHouseLogContainer* warehouse_log_container = NULL;

    switch (warehouse_log_type())
    {
    case kDepositLog:
        {
            warehouse_log_container = &(GuildSystem::GetGuildSystemData().wareshoue_deposit_log());
        }
        break;
    case kWithdrawLog:
        {
            warehouse_log_container = &(GuildSystem::GetGuildSystemData().wareshoue_withdraw_log());
        }
        break;
    }

    if (warehouse_log_container == NULL)
    {
        return;
    }

    int begin_index = log_list_control->GetBeginDrawIndex();

    log_list_control->DeleteItemAll();

    int log_list_index = 0;
    TCHAR log_string[INTERFACE_STRING_LENGTH] = {'\0', };
    GuildSystemData::GuildWareHouseLogContainer::iterator log_begin_itr = warehouse_log_container->begin();
    GuildSystemData::GuildWareHouseLogContainer::iterator log_end_itr = warehouse_log_container->end();
    for ( ; log_begin_itr != log_end_itr; ++log_begin_itr)
    {
        STRUCT_GUILDWAREHOUSE_LOGINFO& log_info = (*log_begin_itr);

        log_list_index = log_list_control->InsertItemBack();

        // 이용시간
        GetLogDateString(log_info.m_UsedTime, log_string, INTERFACE_STRING_LENGTH);
        log_list_control->SetItemWZ(log_list_index, 
            0, 
            log_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 캐릭터명
        log_list_control->SetItemWZ(log_list_index, 
            1, 
            log_info.m_UsedCharName, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 이용내역
        GetWareHouseLogInfoString(log_info, log_string, INTERFACE_STRING_LENGTH);
        log_list_control->SetItemWZ(log_list_index, 
            2, 
            log_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));
    }

    int scroll_range = (log_list_index - log_list_control->GetDrawRowCount());
    scroll_range = max(0, scroll_range);
    begin_index = max(0, begin_index);
    begin_index = min(begin_index, scroll_range);

    ware_house_vscroll()->ResizeingVScrollControl(scroll_range, begin_index);
    log_list_control->SetBeginDrawIndex(begin_index);
}

void uiGuildLog::GetGuildLogInfoString(GuildInGameLogData& IN log_info, 
                                       TCHAR* OUT info_string, 
                                       int size_buffer )
{
    if (info_string == NULL)
    {
        return;
    }

    ZeroMemory(info_string, size_buffer);

    TCHAR temp_string_1[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_string_2[INTERFACE_STRING_LENGTH] = {'\0', };
    switch (log_info.log_type_)
    {
    case GuildCreate:            //길드 창설
        {
            // 7368 %s 길드가 창설되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7368, 
                log_info.string_param_);
        }
        break;
    case GuildLevelUp:           //길드 레벨업
        {
            _itot(log_info.param1_, temp_string_1, 10);
            // 7369 길드의 레벨이 %s로 상승하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7369, 
                temp_string_1);
        }
        break;
    case GuildLevelDown:         //길드 레벨 다운
        {
            _itot(log_info.param1_, temp_string_1, 10);
            // 7370 길드의 레벨이 %s로 하락하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7370, 
                temp_string_1);
        }
        break;
    case GuildEquipmentApply:    //길드 시설 활성화
        {
            g_InterfaceManager.GetInterfaceString(
                GuildSystem::Function::GetInstitutionNameCode(log_info.param1_), temp_string_1);
            // 7373 길드시설 %s가 활성화 되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7373, 
                temp_string_1);
        }
        break;
    case GuildEquipmentLevelup:  //길드 시설 레벨 업
        {
            g_InterfaceManager.GetInterfaceString(
                GuildSystem::Function::GetInstitutionNameCode(log_info.param1_), temp_string_1);
            _itot(log_info.param2_, temp_string_2, 10);
            // 7371 길드시설 %s의 레벨이 %s로 상승되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7371, 
                temp_string_1, 
                temp_string_2);
        }
        break;
    case GuildEquipmentLevelDown://길드 시설 레벨 다운
        {
            g_InterfaceManager.GetInterfaceString(
                GuildSystem::Function::GetInstitutionNameCode(log_info.param1_), temp_string_1);
            _itot(log_info.param2_, temp_string_2, 10);
            // 7372 길드시설 %s의 레벨이 %s로 상승되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7372, 
                temp_string_1, 
                temp_string_2);
        }
        break;
    case GuildGivePoint:         //길드 포인트 기부
        {
            _itot(log_info.param1_, temp_string_1, 10);
            // 7374 %s님이 길드코인을 %s개 기부하셨습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7374, 
                log_info.string_param_, 
                temp_string_1);
        }
        break;
    case GuildChangeName:        //길드 이름 변경
        {
            // 7375 %s로 길드명이 변경되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7375, 
                log_info.string_param_);
        }
        break;
    case GuildRegisterMark:      //길드 마크 등록
        {
            // 7376 길드마크가 등록되었습니다.
            g_InterfaceManager.GetInterfaceString(7376, info_string);
        }
        break;
    case GuildRegisterAnnounce:  //길드 공지 등록
        {
            // 7377 길드공지가 등록되었습니다.
            g_InterfaceManager.GetInterfaceString(7377, info_string);
        }
        break;
    case GuildRegisterIntroduce: //길드 소개 등록
        {
            // 7378 길드소개가 등록되었습니다.
            g_InterfaceManager.GetInterfaceString(7378, info_string);
        }
        break;
    case GuildMemberJoin:	    //가입
        {
            // 7379 %s님이 길드에 가입하셨습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7379, 
                log_info.string_param_);
        }
        break;
    case GuildMemberKick:	    //제명
        {
            // 7381 %s님이 길드에서 제명되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7381, 
                log_info.string_param_);
        }
        break;
    case GuildMemberOut:	    //탈퇴
        {
            // 7380 %s님이 길드를 탈퇴하셨습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7380, 
                log_info.string_param_);
        }
        break;
    case GuildDutyChange:	    //직책 변경
        {
            eGUILD_DUTY guild_duty = static_cast<eGUILD_DUTY>(log_info.param1_);
            g_InterfaceManager.GetInterfaceString(
                GuildSystem::Function::GetGuildDutyStringCode(guild_duty), temp_string_1);
            // 7382 %s님이 %s로 직책이 변경되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7382, 
                log_info.string_param_, 
                temp_string_1);
        }
        break;
    case GuildChangeMaster:    //길드장 변경
        {
            // 7383 길드장이 %s님으로 변경되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7383, 
                log_info.string_param_);
        }
        break;
    case GuildRelationAllienceSet:	    //길드 동맹 관계 설정
        {
            // 7384 %s길드와 동맹길드가 되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7384, 
                log_info.string_param_);
        }
        break;
    case GuildRelationAllienceCancle:	//길드 동맹 관계 해지
        {
            // 7385 %s길드와 동맹관계가 해제되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7385, 
                log_info.string_param_);
        }
        break;
    case GuildSendHostilitySet: //길드 적대 관계 선포
        {
            // 7386 %s길드에게 적대를 선포하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7386, 
                log_info.string_param_);
        }
        break;
    case GuildRecvHostilitySet: //길드 적대 관계 받음
        {
            // 7387 %s길드로부터 적대관계가 선포되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7387, 
                log_info.string_param_);
        }
        break;
    case GuildBothHostilitySet: //길드 적대 관계 설정
        {
            // 7388 %s길드와 적대길드가 되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7388, 
                log_info.string_param_);
        }
        break;
    case GuildHostilityCancel:  //길드 적대 관계 해제
        {
            // 7389 %s길드와 적대관계가 해제되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7389, 
                log_info.string_param_);
        }
        break;
    case GuildSelectedDomiAttack://점령전 공격 길드 선정
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(log_info.param1_));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string_1);
            }            
            // 7390 %s지역에 공격길드로 당첨되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7390, 
                temp_string_1);
        }
        break;
    case GuildNotifyDomiAttack: //점령전 공격 길드 선정 알림
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(log_info.param1_));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string_1);
            }            
            // 7391 점령중인 %s지역에 %s길드가 공격길드로 선정되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7391, 
                temp_string_1,
                log_info.string_param_);
        }
        break;
    case GuildSuccessDomiAttack: //점령전 공격 성공
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(log_info.param1_));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string_1);
            }            
            // 7393 %s지역을 %s길드로부터 획득하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7393, 
                temp_string_1);
        }
        break;
    case GuildSuccessDomiDefense://점령전 방어 성공
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(log_info.param1_));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string_1);
            }            
            // 7392 %s지역 방어에 성공하였습니다
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7392, 
                temp_string_1);
        }
        break;
    case GuildAuctionBatting://점령전 경매 배팅
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(log_info.param1_));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string_1);
            }
            GlobalFunc::SetPointText(temp_string_2, log_info.param2_);
            // 7395 %s 지역에 총 %s개의 왕의 증표를 입찰하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                7395, 
                temp_string_1, 
                temp_string_2);
        }
        break;
    case GuildFailAuction://점령전 입찰 실패
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(log_info.param1_));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string_1);
            }
            GlobalFunc::SetPointText(temp_string_2, log_info.param2_);
            // 3393 %s 지역의 최종입찰 갯수는 %s개입니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                info_string, 
                size_buffer, 
                3393, 
                temp_string_1, 
                temp_string_2);
        }
        break;
    }
}

DWORD uiGuildLog::GetGuildLogColor(BYTE log_type)
{
    DWORD log_color = RGBA(255,255,255,255);
    switch (log_type)
    {
    case GuildLevelUp:	        //길드 레벨업
    case GuildEquipmentApply:    //길드 시설 활성화
    case GuildEquipmentLevelup:  //길드 시설 레벨 업
    case GuildGivePoint:         //길드 포인트 기부
    case GuildMemberJoin:	    //가입
    case GuildRelationAllienceSet:   //길드 관계 설정
    case GuildRelationAllienceCancle://길드 관계 해지
    case GuildSelectedDomiAttack://점령전 공격 길드 선정
    case GuildSuccessDomiAttack: //점령전 공격 성공
    case GuildSuccessDomiDefense://점령전 수비 성공
        {
            log_color = RGBA(100, 100, 250, 255);
        }
        break;
    case GuildLevelDown:         //길드 레벨 다운
    case GuildEquipmentLevelDown://길드 시설 레벨 다운
    case GuildMemberKick:	    //제명
    case GuildMemberOut:	        //탈퇴
    case GuildSendHostilitySet:  //길드 적대 관계 선포
    case GuildRecvHostilitySet:  //길드 적대 관계 받음
    case GuildBothHostilitySet:  //길드 적대 관계 설정
    case GuildHostilityCancel:   //길드 적대 관계 해제
    case GuildNotifyDomiAttack:  //내 지역에 공격 길드가 선정
    case GuildFailAuction:       //점령전 입찰 실패
        {
            log_color = RGBA(204, 0, 0, 255);
        }
        break;

    case GuildAuctionBatting:    //점령전 경매 배팅
        {
        }
        break;
    }

    return log_color;
}

void uiGuildLog::GetWareHouseLogInfoString(STRUCT_GUILDWAREHOUSE_LOGINFO& IN log_info, 
                                           TCHAR* OUT info_string, 
                                           int size_buffer)
{
    if (info_string == NULL)
    {
        return;
    }

    ZeroMemory(info_string, size_buffer);

    if ((log_info.m_LogType == ITEM_IN) || 
        (log_info.m_LogType == ITEM_OUT))
    {
        BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(log_info.m_ItemCode);
        if (item_info != NULL)
        {
            TCHAR item_name[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR temp_item_name[INTERFACE_STRING_LENGTH] = {'\0', };

            GameFunc::GetItemName(NULL, item_info, temp_item_name, INTERFACE_STRING_LENGTH);

            if(item_info->IsArmor() || item_info->IsWeapon())
            {
                if (0 == log_info.m_LimiteCode)
                {
                    Snprintf(item_name, INTERFACE_STRING_LENGTH, _T("%s"), temp_item_name);
                }
                else
                {
                    //11283	리미티드
                    Snprintf(item_name, 
                        INTERFACE_STRING_LENGTH, 
                        _T("%s %s"), 
                        g_InterfaceManager.GetInterfaceString(11283), 
                        temp_item_name);
                }
            }
            else
            {
                g_InterfaceManager.GetInterfaceString(item_info->m_NCode, temp_item_name);
                if (0 == log_info.m_ItemNum)
                    Snprintf(item_name, INTERFACE_STRING_LENGTH, _T("%s"), temp_item_name);
                else
                    Snprintf(item_name, INTERFACE_STRING_LENGTH, _T("%s X %d"), temp_item_name, log_info.m_ItemNum);
            }

            strncpy(info_string, item_name, size_buffer);
        }
    }
    else if ((log_info.m_LogType == MONEY_IN) || 
        (log_info.m_LogType == MONEY_OUT))
    {
        TCHAR money_string[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR temp_stringe[INTERFACE_STRING_LENGTH] = {'\0', };

        Snprintf(temp_stringe, INTERFACE_STRING_LENGTH, _T("%I64u"), log_info.m_Money);
        NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
        ::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, temp_stringe, &nFmt, money_string, sizeof(money_string));

        //999	하임
        Snprintf(info_string, 
            size_buffer, 
            _T("%s %s"), 
            money_string, 
            g_InterfaceManager.GetInterfaceString(999));
    }
}

void uiGuildLog::GetLogDateString(INT64 log_time, TCHAR* OUT info_string, int size_buffer )
{
    if (info_string == NULL)
    {
        return;
    }

    ZeroMemory(info_string, size_buffer);

    TCHAR temp_string[12 + 1] = {'\0', };
    _i64toa(log_time, temp_string, 10); 
#ifdef _INTERNATIONAL_DATE_TOOLTIP
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    TCHAR month_string[INTERFACE_STRING_LENGTH] = {'\0', };
    if (tooltip_manager != NULL)
    {
        month_string[0] = temp_string[4];
        month_string[1] = temp_string[5];
        month_string[2] = '\0';
        //월을 영문스트링으로 교체
        int string_code = tooltip_manager->GetStringMonth(atoi(month_string));
        g_InterfaceManager.GetInterfaceString(string_code, month_string);
    }

    #ifdef _RUSSIA
    // 러시아 july.02.2009 (12:12).
    Snprintf(info_string,size_buffer, _T("%s.%c%c.%c%c%c%c (%c%c:%c%c)."),month_string	//월(july)
        ,temp_string[6], temp_string[7]		//1일
        , temp_string[0], temp_string[1], temp_string[2], temp_string[3] //2009
        , temp_string[8], temp_string[9], temp_string[10], temp_string[11]);//(00:00)
    #else
    // 글로벌 july/02 (12:12), 2009	
    Snprintf(info_string,size_buffer, _T("%s %c%c (%c%c:%c%c), %c%c%c%c"),month_string	//월(july)
        ,temp_string[6], temp_string[7]		//1일
        , temp_string[8], temp_string[9], temp_string[10], temp_string[11]//(00:00)
        , temp_string[0], temp_string[1], temp_string[2], temp_string[3]);//2009
    #endif //_RUSSIA

#else
    Snprintf(info_string, size_buffer, _T("%c%c%c%c/%c%c/%c%c %c%c:%c%c"), temp_string[0], temp_string[1], temp_string[2], temp_string[3]
    , temp_string[4], temp_string[5], temp_string[6], temp_string[7]
    , temp_string[8], temp_string[9], temp_string[10], temp_string[11]);
#endif//_INTERNATIONAL_DATE_TOOLTIP
}

void uiGuildLog::InitGuildWareHouseLog()
{
    CCtrlListWZ* log_list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    CCtrlVScrollWZ* log_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V001));
    if ((log_list_control == NULL) || 
        (log_scroll == NULL))
    {
        assert(log_list_control);
        assert(log_scroll);
        return;
    }

    log_list_control->SetBeginDrawIndex(0);
    log_scroll->SetScrollPosWZ(0);
}

void uiGuildLog::InitGuildInfoLog()
{
    CCtrlListWZ* log_list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    CCtrlVScrollWZ* log_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if ((log_list_control == NULL) || 
        (log_scroll == NULL))
    {
        assert(log_list_control);
        assert(log_scroll);
        return;
    }

    log_list_control->SetBeginDrawIndex(0);
    log_scroll->SetScrollPosWZ(0);
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
