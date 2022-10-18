#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildIntroOther.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/uiGuildRelationAsk.h"
#include "GuildRankInfoParser.h"
#include "GuildRightInfoParser.h"
#include "GuildRightInfoParser.h"
#include "InterfaceManager.h"
#include "globalfunc.h"
#include "hero.h"

const DWORD uiGuildIntroOther::kDialog_ID = g_IDGenerator.GenerateID();
//----------------------------------------------------------------------------
/**
*/
uiGuildIntroOther::uiGuildIntroOther(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
    set_control_gauge_width(0.0f);
    set_gauge_texture_width(1.0f);
    scroll_pos = 0;
}


//----------------------------------------------------------------------------
/**
*/
uiGuildIntroOther::~uiGuildIntroOther(void) 
{
    Release();
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    sprintf(now_notice_string_, "");
    VScrollDialog::ResizeingVScrollControl(0, 0);
    
    CCtrlDxEditBox* edit_control = static_cast<CCtrlDxEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control)
    {
        edit_control->SetLimitText(MAX_GUILDNOTICE_LENGTH);
#if WZENGINEVER >= 420
        edit_control->SetLimitText_MultibyteCheck(true); 
#endif //#if WZENGINEVER >= 410
    }

    //일단 길드소개 사용 불가
    CControlWZ* control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C005));
    if (control_ptr != NULL)
    {
        // 7277 길드 소개
        control_ptr->SetTextID(7277);
        control_ptr->EnableWindowWZ(true);
    }
    control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C006));
    if (control_ptr != NULL)
    {
        // 1045 길드 공지사항
        control_ptr->SetTextID(1045);
        control_ptr->EnableWindowWZ(false);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Picture_P007);
    if (control_ptr != NULL)
    {
        set_control_gauge_width(static_cast<float>(control_ptr->GetSize().right));
        set_gauge_texture_width(control_ptr->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Button_B006);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Button_B005);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Button_B009);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = GetControlWZ(StrToWzID("P003"));
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Edit_E000);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    control_ptr = GetControlWZ_byPosition(kControl_Button_B001);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::Release()
{
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::MessageProc(SI_MESSAGE* message)
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
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnVScrollDown(message);
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
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::OnUpdateSolarDialog()
{
    VScrollDialog::Update();

    UpdateGuildCoin();
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::SetGuildNotice(TCHAR* string)
{
    CCtrlListWZ* intro_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if(intro_list == NULL)
    {
        return;
    }

    intro_list->DeleteItemAll();
    this->guild_notice_string_.clear();

    RECT rc = intro_list->GetSizeRect();
    int iListWidth = rc.right - rc.left - 10;
    GlobalFunc::ProcessWordWrapByFontSize(&guild_notice_string_, string, StrToWzID("m212"), _tcslen(string), iListWidth);
    RefreshGuildNotice();
}
//=====================================================================================
//	1.길드공지 갱신
//=====================================================================================
void uiGuildIntroOther::RefreshGuildNotice()
{
    CCtrlListWZ* intro_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));

    if (intro_list == NULL)
    {
        return;
    }
    //0.컨트롤러 기본셋팅
    intro_list->DeleteItemAll();
    intro_list->SetSellHeight(20);
    intro_list->SetUseReport(false);	
    intro_list->SetReportUseCnt(0);
    intro_list->SetHideReportRender(true);
    intro_list->SetFontID(StrToWzID("m212"));

    //1.스크롤 페이지계산
   /* int iScrollMaxLine = this->guild_notice_string_.size();	
    int iPage = (iScrollMaxLine - SHOW_DEFAULT_NOTICE_LINE);
    if( iPage<=0) iPage =0;
//    m_pScroll->SetScrollRangeWZ(0,iPage);
    //int iCurScrollPos = m_pScroll->GetScrollPosWZ();
*/
    int add_line = 0;
    //2.리스트컨트롤에 데이터 삽입
    for( int iLine=0; static_cast<size_t>(iLine) <this->guild_notice_string_.size() ; ++iLine)
    {
        //2_1.기본출력라인수만 삽입한다
        /*if(add_line>=SHOW_DEFAULT_NOTICE_LINE)
        {
            break;
        }*/

        //2_2.스크롤연산된 리스트의 시작포인트잡기 
        /*if(iLine < iCurScrollPos)
        {
            continue;
        }*/

        int index = intro_list->InsertItemBack();

        TCHAR *pStr = const_cast<TCHAR *>(guild_notice_string_[iLine].c_str());
        GlobalFunc::SetItem(
            intro_list, 
            index, 0, pStr, 
            StrToWzID("m212"),
            DT_VCENTER | DT_LEFT, 
            RGBA(255,255,255,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));
        ++add_line;
    }
    int scroll_range = add_line - intro_list->GetDrawRowCount();
    scroll_range = (scroll_range > 0)?scroll_range:0;
    VScrollDialog::ResizeingVScrollControl(scroll_range, 0);
}
//----------------------------------------------------------------------------
/**
*/
bool uiGuildIntroOther::OnLButtonClick(SI_MESSAGE* message)
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
    case kControl_CheckBT_C005:
    case kControl_CheckBT_C006:
        {
            RefreshTab();
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiGuildIntroOther::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiGuildIntroOther::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildIntroOther::OnVScrollThumb(SI_MESSAGE* msg)
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
bool uiGuildIntroOther::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        vscroll_control()->SetScrollPosWZ(++scroll_pos);
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildIntroOther::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        vscroll_control()->SetScrollPosWZ(--scroll_pos);
        return true;
    }
    return false;
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::OnShowWindow(BOOL val)
{
    if (val)
    {
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
            GuildSystem::GetGuildSystemData().guild_ranking_info_container();
        if (static_cast<size_t>(guild_rank_index()) < guild_ranking_info_container.size())
        {
            GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[guild_rank_index()];
            GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& ranking_guild_info_container = 
                GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();
            GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr = 
                ranking_guild_info_container.find(ranking_info.guild_guid);
            if (found_itr != ranking_guild_info_container.end())
            {
                GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;
                SetGuildNotice(guild_info.guild_info_extra_.introduction_strings_);
            }
        }
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
        RefreshTab();
    }
    else
    {
        //취소 하기
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntroOther::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C006 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        bool check_enable = false;
        if ((position == kControl_CheckBT_C000) ||
            (position == kControl_CheckBT_C005))
        {
            check_enable = true;
        }
        check_button->SetCheckState(check_enable);
        check_button->EnableWindowWZ(check_enable);
    }
}

void uiGuildIntroOther::UpdateGuildCoin()
{
    CCtrlStaticWZ* guild_coin_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));

    BYTE guild_grade = 0;
    const GuildRankInfo* current_guild_rank_info = NULL;
    const GuildRankInfo* to_next_guild_rank_info = NULL;

    DWORD need_coin = 0;
    DWORD prev_need_coin = 0;
    DWORD guild_coin = 0;

    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    if (static_cast<size_t>(guild_rank_index()) < guild_ranking_info_container.size())
    {
        GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[guild_rank_index()];

        current_guild_rank_info = 
            GuildRankInfoParser::Instance()->FindGuildRankInfo(static_cast<BYTE>(ranking_info.guild_level));
        to_next_guild_rank_info = 
            GuildRankInfoParser::Instance()->FindGuildRankInfo(static_cast<BYTE>(ranking_info.guild_level) + 1);

        guild_coin = ranking_info.guild_point;
    }
    if (current_guild_rank_info != NULL)
    {
        need_coin = current_guild_rank_info->need_guild_coin_to_rankup_;
        prev_need_coin = need_coin;
    }

    if (to_next_guild_rank_info != NULL)
    {
        need_coin = to_next_guild_rank_info->need_guild_coin_to_rankup_;
    }

    if (need_coin >= prev_need_coin)
    {
        need_coin -= prev_need_coin;
    }
    if (guild_coin >= prev_need_coin)
    {
        guild_coin -= prev_need_coin;
    }

    float gauge_rate = 0.0f;
    if (need_coin > 0)
    {
        gauge_rate = 
            static_cast<float>((guild_coin) / static_cast<double>(need_coin));
        gauge_rate = max(gauge_rate, 0.0f);
        gauge_rate = min(gauge_rate, 100.0f);
    }

    if (guild_coin_control != NULL)
    {
        TCHAR guild_coin_text[INTERFACE_STRING_LENGTH] = {'\0', };
        Snprintf(guild_coin_text, INTERFACE_STRING_LENGTH - 1, "%d%% (%d / %d)", 
            static_cast<DWORD>(gauge_rate * 100.0f), 
            guild_coin, 
            need_coin);
        guild_coin_control->SetTextWZ(guild_coin_text);
    }
    UpdateGuildEXPGauge(gauge_rate);
}

void uiGuildIntroOther::UpdateGuildEXPGauge( float gauge_rate )
{
    CControlWZ* exp_gauge_control = GetControlWZ_byPosition(kControl_Picture_P007);
    if (exp_gauge_control != 0)
    {
        if (gauge_rate < 0.01)
        {
            exp_gauge_control->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            float control_width = gauge_control_width() * gauge_rate;
            float texture_width = gauge_texture_width() * gauge_rate;

            RECT gauge_rect = exp_gauge_control->GetSizeRect();
            exp_gauge_control->MoveWindowWZ(
                static_cast<float>(gauge_rect.left), 
                static_cast<float>(gauge_rect.top), 
                control_width, 
                static_cast<float>(gauge_rect.bottom - gauge_rect.top));
            exp_gauge_control->GetCtrlImageFormat()->m_sImageState[E_BASE].m_sFRect.fWidth = texture_width;

            exp_gauge_control->ShowWindowWZ(WZ_SHOW);
        }        
    }    
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
