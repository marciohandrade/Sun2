#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildIntro.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/uiGuildRelationAsk.h"
#include "GuildRankInfoParser.h"
#include "GuildRightInfoParser.h"
#include "GuildRightInfoParser.h"
#include "InterfaceManager.h"
#include "globalfunc.h"
#include "hero.h"
#include "ResultCode.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "uiGuildJoinRequestList.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const DWORD uiGuildIntro::kDialog_ID = StrToWzID("7341");
int scroll_pos = 0;
//----------------------------------------------------------------------------
/**
*/
uiGuildIntro::uiGuildIntro(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
    set_control_gauge_width(0.0f);
    set_gauge_texture_width(1.0f);
}


//----------------------------------------------------------------------------
/**
*/
uiGuildIntro::~uiGuildIntro(void) 
{
    ChangeStateTabButton(kTab_Notice);
    Release();
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
#ifdef _NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);
#endif //_NA_008334_20150608_SONNENSCHEIN
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
    sprintf(now_invite_string_, "");
    SetEditMode(false);
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
    CCtrlButtonCheckWZ* second_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C006));
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    // no operation
#else
    if (second_tab)
    {
        second_tab->EnableWindowWZ(false);
    }
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
    ChangeStateTabButton(kTab_Notice);//현재는 자신의 길드만 열수 있다. 공지사항으로 설정
    SelectStateTabButton(kControl_CheckBT_C005);//양쪽 탭 모두 사용

    CControlWZ* exp_gauge_control = GetControlWZ_byPosition(kControl_Picture_P007);
    if (exp_gauge_control != 0)
    {
        set_control_gauge_width(static_cast<float>(exp_gauge_control->GetSize().right));
        set_gauge_texture_width(exp_gauge_control->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth);
    }
}

void uiGuildIntro::SetEditMode(bool is_mode)
{
    is_edit_mode_ = is_mode;
    CControlWZ* control = GetControlWZ_byPosition(kControl_Edit_E000);
    CCtrlEditWZ* edit_control = static_cast<CCtrlEditWZ*>(control);
    if (control)
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        if (is_mode)
        {
            if (is_select_tab_ == kTab_Notice)
            {
                GlobalFunc::ChangeMultiEditOutput(now_notice_string_, now_notice_wstring_);
                control->SetTextWZW(now_notice_wstring_);
            }
            else
            {
                GlobalFunc::ChangeMultiEditOutput(now_invite_string_, now_invite_wstring_);
                control->SetTextWZW(now_invite_wstring_);
            }
        }
        control->ShowWindowWZ(is_edit_mode_?WZ_SHOW:WZ_HIDE);
    }
    control = GetControlWZ_byPosition(kControl_List_L000);
    if (control)
    {
        control->ShowWindowWZ(is_edit_mode_?WZ_HIDE:WZ_SHOW);
    }
    ChangeWriteButton(is_edit_mode_);
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::Release()
{
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::MessageProc(SI_MESSAGE* message)
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
void uiGuildIntro::OnUpdateSolarDialog()
{
    VScrollDialog::Update();

    UpdateGuildCoin();
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildIntro::SetGuildNotice(TCHAR* string)
{
    CCtrlListWZ* intro_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if(intro_list == NULL)
    {
        return;
    }

    intro_list->DeleteItemAll();
    this->guild_notice_string_.clear();

    /*if (!this->GetManager())
        return;

    if (!this->GetManager()->IsGuildInfo())
        return;

    TCHAR *ptszMsg = GetManager()->GetGuildNotice();

    if(ptszMsg == NULL)
        return;*/

    if (strcmp(string, "") == 0)
    {
        int message_num = (is_select_tab_ == kTab_Notice)?7305:7304;
        g_InterfaceManager.GetInterfaceString(message_num, string, MAX_GUILDNOTICE_LENGTH);
    }
    RECT rc = intro_list->GetSizeRect();
    int iListWidth = rc.right - rc.left - 10;
    GlobalFunc::ProcessWordWrapByFontSize(&guild_notice_string_, string, StrToWzID("m212"), _tcslen(string), iListWidth);
    RefreshGuildNotice();
}
//=====================================================================================
//	1.길드공지 갱신
//=====================================================================================
void uiGuildIntro::RefreshGuildNotice()
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
bool uiGuildIntro::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case kControl_CheckBT_C008:
#endif //_NA_008334_20150608_SONNENSCHEIN
    case kControl_CheckBT_C002:
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
    case kControl_CheckBT_C006:
        {
            SelectStateTabButton(control_position);//양쪽 탭 모두 사용
        }
        break;
    case kControl_Button_B005:
        {
            if (is_edit_mode_)
            {
                //7312 등록을 취소합니다.\n취소 시 마지막으로 등록된 내용으로 변경됩니다.\n 정말 취소하시겠습니까?
                uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                if (ui_system_manager != NULL)
                {
                    TCHAR invite_message[INTERFACE_STRING_LENGTH] = {'\0', };
                    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {'\0', };
                    g_InterfaceManager.GetInterfaceString(7312, temp_message);
                    ui_system_manager->Sys_SystemMessage(temp_message, 
                        NULL, 
                        GuildSystem::Function::Callback_GuildNoticeCancle, 
                        uiSystemMessage::kConfirm_Type, 
                        true);
                }
                //SetEditMode(!is_edit_mode_);
            }
            else
            {
                //7313 작성된 글을 모두 삭제합니다\n정말 삭제하시겠습니까?
                uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                if (ui_system_manager != NULL)
                {
                    TCHAR invite_message[INTERFACE_STRING_LENGTH] = {'\0', };
                    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {'\0', };
                    g_InterfaceManager.GetInterfaceString(7313, temp_message);
                    ui_system_manager->Sys_SystemMessage(temp_message, 
                        NULL, 
                        GuildSystem::Function::Callback_GuildNoticeDelete, 
                        uiSystemMessage::kConfirm_Type, 
                        true);
                }
            }
        }
        break;
    case kControl_Button_B006:
        {
            GuildSystem::Function::ShowConfirmWithdrawGuild();            
        }
        break;
    case kControl_Button_B009:
        {
            uiGuildRelationAsk* guild_ask = 
                GET_CAST_DIALOG(uiGuildRelationAsk, uiGuildRelationAsk::kDialog_ID);
            if (guild_ask != NULL)
            {
                guild_ask->set_guild_ask_type(uiGuildRelationAsk::kGuild_Invite);
                guild_ask->ShowInterface(TRUE);
            }
        }
        break;
    case kControl_Button_B000:
        {
            WCHAR guild_notice[MAX_GUILDNOTICE_LENGTH+1] = {0,};
            TCHAR message[MAX_GUILDNOTICE_LENGTH+3] = {0,};
            if (is_edit_mode_)
            {
                CCtrlEditWZ* control = static_cast<CCtrlEditWZ*>(GetControlWZ_byPosition(kControl_Edit_E000));
                control->GetTextWZW(guild_notice, MAX_GUILDNOTICE_LENGTH);
                GlobalFunc::ChangeMultiEditInput(guild_notice, message);
                
                if (WriteNotice(message))
                {
                    //wsprintfW(now_notice_wstring_, guild_notice);
                    if (is_select_tab_ == kTab_Notice)
                    {
                        sprintf(now_notice_string_, message);
                        SetGuildNotice(now_notice_string_);
                    }
                    else
                    {
                        sprintf(now_invite_string_, message);
                        SetGuildNotice(now_invite_string_);
                    }

                    is_edit_mode_ = !is_edit_mode_;
                    SetEditMode(is_edit_mode_);
                    CCtrlEditWZ* edit_control = static_cast<CCtrlEditWZ*>(GetControlWZ_byPosition(kControl_Edit_E000));
                    if (edit_control)
                    {
                        edit_control->SetFocusWZ();
                    }
                    //7311 작성하신 내용이 정상적으로 등록 되었습니다.
                    GlobalFunc::VerifyMessage(7311);
                }
                else //금지어 있음
                {
                    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                    if (ui_system_manager != NULL)
                    {
                        //7310 작성하신 내용에 사용 금지어가 포함되어 있습니다.\n확인 후 다시 등록하여 주시기 바랍니다.
                        GlobalFunc::VerifyMessage(7310);
                    }
                }
            }
            else
            {
                is_edit_mode_ = !is_edit_mode_;
                SetEditMode(is_edit_mode_);
                CCtrlEditWZ* edit_control = static_cast<CCtrlEditWZ*>(GetControlWZ_byPosition(kControl_Edit_E000));
                if (edit_control)
                {
                    edit_control->SetFocusWZ();
                }
            }
        }
        break;
    case kControl_Text_S000:
        {
        }
        break;
    case kControl_Text_S001:
        {
        }
        break;
    case kControl_List_L000:
        {
        }
        break;
    case kControl_VScroll_V000:
        {
        }
        break;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case kControl_Button_B001:
        {
            uiGuildJoinRequestList* ui_guild_join_request_list_ptr = 
                GET_CAST_DIALOG(uiGuildJoinRequestList, uiGuildJoinRequestList::kDialog_ID);
            if (ui_guild_join_request_list_ptr)
            {
                ui_guild_join_request_list_ptr->ShowInterface(TRUE);
            }
        }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    default:
        {
        }
        break;
    }
    return result;
}

//------------------------------------------------------------------------------ 
void uiGuildIntro::SetDeleteMessage(bool force_delete/* = false*/)
{
    TCHAR message[MAX_GUILDNOTICE_LENGTH+1] = {0,};

    // 7305 길드 공지사항을 입력해 주세요
    // 7304 길드 소개글을 입력해 주세요
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    int message_num = (is_select_tab_ == kTab_Notice)?7305:7304;
#else
    int message_num = (first_tab_num_ == kTab_Notice)?7305:7304;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    g_InterfaceManager.GetInterfaceString(message_num, message, MAX_GUILDNOTICE_LENGTH);
    if ((WriteNotice(message) == true) || 
        (force_delete == true))
    {
        //wsprintfW(now_notice_wstring_, guild_notice);
        if (is_select_tab_ == kTab_Notice)
        {
            sprintf(now_notice_string_, message);
            SetGuildNotice(now_notice_string_);
        }
        else
        {
            sprintf(now_invite_string_, message);
            SetGuildNotice(now_invite_string_);
        }        

        is_edit_mode_ = false;
        SetEditMode(is_edit_mode_);
    }
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiGuildIntro::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiGuildIntro::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiGuildIntro::OnVScrollThumb(SI_MESSAGE* msg)
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
bool uiGuildIntro::OnVScrollUp(SI_MESSAGE* msg)
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
bool uiGuildIntro::OnVScrollDown(SI_MESSAGE* msg)
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
void uiGuildIntro::OnShowWindow(BOOL val)
{
    if (val)
    {
        // 캐릭터 최초 접속시 변경된 길드공지가 있는지 요청
        MSG_CW_GUILD_SELECT_NOTICE_SYN SendNoticePacket;
        GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendNoticePacket, sizeof(SendNoticePacket));

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
        GetGuildIntro();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        ChangeStateTabButton(kTab_Notice);//현재는 자신의 길드만 열수 있다. 공지사항으로 설정
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        SelectStateTabButton(kControl_CheckBT_C005);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        ShowGuildInOutButton();
        ShowWriteButton();
        RefreshTab();
    }
    else
    {
        //취소 하기
        SetEditMode(false);
    }
}

void uiGuildIntro::GetGuildIntro()
{
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR invite_string[MAX_GUILDNOTICE_LENGTH+1] = {0,};
    StrnCopy(invite_string, GuildSystem::GetGuildSystemData().guild_intro(), MAX_GUILDNOTICE_LENGTH);

    if(Strcmp(invite_string, "")==0 || Strcmp(invite_string, now_invite_string_))	// 내용이 없거나, 새로운 공지 사항일때만 출력
    {
        ZeroMemory(now_invite_string_, sizeof(now_invite_string_));
        StrnCopy(now_invite_string_, invite_string, MAX_GUILDNOTICE_LENGTH);

        //5174	길드 공지가 전달되었습니다.
        //g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
        //GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        if (first_tab_num_ == kTab_Invite)
        {
            SetGuildNotice(now_invite_string_);
        }
    }
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::NetworkProc(MSG_BASE* recv_packet)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR notice_string[MAX_GUILDNOTICE_LENGTH+1] = {0,};

    if (recv_packet->m_byCategory == CW_GUILD){
        switch (recv_packet->m_byProtocol)
        {
        case CW_GUILD_SELECT_NOTICE_NAK:
            {
                //7196	길드 공지를 받지 못했습니다.
                g_InterfaceManager.GetInterfaceString(7196, szMessage, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
            break;
        case CW_GUILD_SELECT_NOTICE_ACK:
            {
                MSG_CW_GUILD_SELECT_NOTICE_ACK *recvMsg = (MSG_CW_GUILD_SELECT_NOTICE_ACK *)recv_packet;

                StrnCopy(notice_string , recvMsg->m_ptszNotice, MAX_GUILDNOTICE_LENGTH);
                //safecode:서버에서 쓰레기값날라왔을때를 대비
                if (recvMsg->m_wNoticeLen>MAX_GUILDNOTICE_LENGTH) break;

                notice_string[recvMsg->m_wNoticeLen] = NULL;

                if(Strcmp(notice_string, "")==0 || Strcmp(notice_string, now_notice_string_))	// 내용이 없거나, 새로운 공지 사항일때만 출력
                {
                    ZeroMemory(now_notice_string_, sizeof(now_notice_string_));
                    StrnCopy(now_notice_string_, notice_string, MAX_GUILDNOTICE_LENGTH);

                    //5174	길드 공지가 전달되었습니다.
                    /*g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);*/
                    SetGuildNotice(notice_string);
                }
            }
            break;
        case CW_GUILD_NOTICE_CMD:
            {
                MSG_CW_GUILD_NOTICE_CMD *recvMsg = (MSG_CW_GUILD_NOTICE_CMD *)recv_packet;

                StrnCopy(notice_string , recvMsg->m_ptszNotice, MAX_GUILDNOTICE_LENGTH);
                //safecode:서버에서 쓰레기값날라왔을때를 대비
                if (recvMsg->m_wNoticeLen>MAX_GUILDNOTICE_LENGTH) break;

                notice_string[recvMsg->m_wNoticeLen] = NULL;

                if(Strcmp(notice_string, now_notice_string_))	// 새로운 공지 사항일때만 출력
                {
                    ZeroMemory(now_notice_string_, sizeof(now_notice_string_));
                    StrnCopy(now_notice_string_, notice_string, MAX_GUILDNOTICE_LENGTH);

                    //5174	길드 공지가 전달되었습니다.
                    g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    SetGuildNotice(notice_string);
                }
            }
            break;
        case CW_GUILD_CHANGE_NOTICE_BRD:
            {
                MSG_CW_GUILD_CHANGE_NOTICE_BRD *recvMsg = (MSG_CW_GUILD_CHANGE_NOTICE_BRD *)recv_packet;

                StrnCopy(notice_string , recvMsg->m_ptszNotice, MAX_GUILDNOTICE_LENGTH);
                notice_string[recvMsg->m_wNoticeLen] = NULL;

                if(Strcmp(notice_string, now_notice_string_))	// 새로운 공지 사항일때만 출력
                {
                    ZeroMemory(now_notice_string_, sizeof(now_notice_string_));
                    StrnCopy(now_notice_string_, notice_string, MAX_GUILDNOTICE_LENGTH);

                    //5174	길드 공지가 전달되었습니다.
                    g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    SetGuildNotice(notice_string);
                }
            }
            break;

        case CW_GUILD_CHANGE_NOTICE_NAK:
            {
                MSG_CW_GUILD_CHANGE_NOTICE_NAK* pRecvMsg  =(MSG_CW_GUILD_CHANGE_NOTICE_NAK*)recv_packet;
                //5176	길드 공지 변경에 실패했습니다.	
                g_InterfaceManager.GetInterfaceString(5176, szMessage, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
            break;
        default:
            {
            }
            break;
        }
    }
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    else if (recv_packet->m_byCategory == CG_GUILD)
    {
        switch (recv_packet->m_byProtocol)
        {
        case CG_GUILD_INTRODUCTION_SAVE_ACK:
            {
                MSG_CG_GUILD_INTRODUCTION_SAVE_ACK* pRecvMsg  =(MSG_CG_GUILD_INTRODUCTION_SAVE_ACK*)recv_packet;

                if (pRecvMsg->result_ == RC::RC_GUILD_SUCCESS)
                {
                    
                }
                else
                {
                    //5176	길드 공지 변경에 실패했습니다.	
                    g_InterfaceManager.GetInterfaceString(5176, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
            }
            break;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        case CG_GUILD_INTRODUCTION_CHANGE_BRD:
            {
                MSG_CG_GUILD_INTRODUCTION_CHANGE_BRD* recv_message = static_cast<MSG_CG_GUILD_INTRODUCTION_CHANGE_BRD*>(recv_packet);
                TCHAR invite_string[MAX_GUILD_INTRODUCTION_LENGTH+1] = {0,};
                StrnCopy(invite_string , recv_message->introduction_strings_, MAX_GUILD_INTRODUCTION_LENGTH);
                
                {
                    ZeroMemory(now_invite_string_, sizeof(now_invite_string_));
                    StrnCopy(now_invite_string_, invite_string, MAX_GUILDNOTICE_LENGTH);

                    /* hsh2303 -> 길드 소개가 변경되었습니다.
                    //5174	길드 공지가 전달되었습니다.
                    g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    */
                    // 현재 탭이 길드 소개 일 경우에만 적용
                    if (is_select_tab_ == kTab_Invite)
                    {
                        SetGuildNotice(invite_string);
                    }
                }
            }
            break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
    }
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
}

//------------------------------------------------------------------------------
/**
*/
bool uiGuildIntro::WriteNotice(TCHAR* message)
{
    if (message == NULL)
    {
        return false;
    }

    if (g_pHero == NULL)
    {
        return false;
    }

    if (g_pHero->IsExistGuild() == FALSE)
    {
        return false;
    }

    if(GlobalFunc::IsSlang(message, false) == TRUE)
    {
        return false;
    }

    TCHAR szWideString[MAX_GUILDNOTICE_LENGTH+1] ={0,};
    StrnCopy(szWideString , message, MAX_GUILDNOTICE_LENGTH);
    szWideString[MAX_GUILDNOTICE_LENGTH] = NULL;

    if (is_select_tab_ == kTab_Notice)
    {
        MSG_CW_GUILD_CHANGE_NOTICE_SYN sync;
        ZeroMemory(sync.m_tszGuildNotice, sizeof(sync.m_tszGuildNotice));
        StrnCopy(sync.m_tszGuildNotice, szWideString, MAX_GUILDNOTICE_LENGTH-1);
        sync.m_byNoticeLen = strlen(sync.m_tszGuildNotice);
        GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof(sync));
    }
    else
    {
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        MSG_CG_GUILD_INTRODUCTION_SAVE_SYN sync;
        ZeroMemory(sync.guild_introductions_.introduction_strings_, sizeof(sync.guild_introductions_.introduction_strings_));
        StrnCopy(sync.guild_introductions_.introduction_strings_ , szWideString, MAX_GUILDNOTICE_LENGTH-1);
        sync.guild_introductions_.introduction_length_ = strlen(sync.guild_introductions_.introduction_strings_);
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
        GetGuildIntro();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        //TODO 임시
        /*{
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            //5174	길드 공지가 전달되었습니다.
            g_InterfaceManager.GetInterfaceString(5174, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            SetGuildNotice(now_invite_string_);
        }*/
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
    }

    return true;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::ShowWriteButton()
{
    //글쓰기 권한
    bool is_rank = false;
    
    // TODO: is_rank = 길드_글쓰기_권한_얻기();
    if (GuildSystem::Function::IsGuildMember(g_pHero->GetGuildName()) == true)
    {
        PLAYER_GUILD_RENDER_INFO& guild_render_info = g_pHero->GetGuildRenderInfo();        
        if (GuildRightInfoParser::Instance()->CheckDutyForRight(eGUILD_RIGHTS_NOTICE, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty)) &&
            GuildRightInfoParser::Instance()->CheckDutyForRight(eGUILD_RIGHTS_NOTICE, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty)))
        {
            is_rank = true;
        }
    }

    ShowButton(kControl_Button_B000, is_rank);//글쓰기 버튼
    ShowButton(kControl_Button_B005, is_rank);//삭제 버튼    
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::ChangeStateTabButton(int first_tab_num)
{
    first_tab_num_ = first_tab_num;
    CCtrlButtonCheckWZ* first_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C005));
    CCtrlButtonCheckWZ* second_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C006));

    if (first_tab && second_tab)
    {
        first_tab->SetTextID((first_tab_num==kTab_Notice)?1045:7277);//1045 길드 공지사항
        second_tab->SetTextID((first_tab_num==kTab_Invite)?1545:7277);//7277 길드 소개
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        if (first_tab_num == kTab_Notice)
        {
            first_tab->SetCheckState(true);
            second_tab->SetCheckState(false);
        }
        else
        {
            second_tab->SetCheckState(true);
            first_tab->SetCheckState(false);
        }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::SelectStateTabButton(DWORD control_pos)
{
    CCtrlButtonCheckWZ* first_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C005));
    CCtrlButtonCheckWZ* second_tab = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C006));

    if (!first_tab || !second_tab)
    {
        return;
    }
    int old_select_tab = is_select_tab_;

    if (control_pos == kControl_CheckBT_C005)
    {
        //(first_tab_num_ == kTab_Notice)//순서 맞음
        first_tab->SetCheckState((first_tab_num_ == kTab_Notice)?true:false);
        second_tab->SetCheckState((first_tab_num_ == kTab_Notice)?false:true);
        is_select_tab_ = ((first_tab_num_ == kTab_Notice)?kTab_Notice:kTab_Invite);
    }
    else// if (control_pos == kControl_CheckBT_C006)
    {
        //(first_tab_num_ == kTab_Invite)//순서 맞음
        first_tab->SetCheckState((first_tab_num_ == kTab_Invite)?true:false);
        second_tab->SetCheckState((first_tab_num_ == kTab_Invite)?false:true);
        is_select_tab_ = ((first_tab_num_ == kTab_Invite)?kTab_Notice:kTab_Invite);
    }

    //이전 탭과 다른 경우라면 갱신
    if (is_select_tab_ != old_select_tab)
    {
        SetEditMode(false);
        if (is_select_tab_ == kTab_Notice)
        {
            SetGuildNotice(now_notice_string_);
        }
        else
        {
            SetGuildNotice(now_invite_string_);
        }
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::ChangeWriteButton(bool is_mode)
{
    CCtrlButtonWZ* write_button_ptr = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B000)));
    CCtrlButtonWZ* cancle_button_ptr = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B005)));

    if (is_mode)
    {
        if (write_button_ptr)
        {
            write_button_ptr->SetTextID(1174);//등록
        }
        if (cancle_button_ptr)
        {
            cancle_button_ptr->SetTextID(3030);//취소
        }
    }
    else
    {
        if (write_button_ptr)
        {
            write_button_ptr->SetTextID(2302);//글쓰기
        }
        if (cancle_button_ptr)
        {
            cancle_button_ptr->SetTextID(155533);//삭제
        }
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::ShowGuildInOutButton()
{
    if (g_pHero == NULL)
    {
        return;
    }

    PLAYER_GUILD_RENDER_INFO& guild_render_info = g_pHero->GetGuildRenderInfo();
    eGUILD_DUTY guild_duty = static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty);
    bool right_invite = 
        GuildRightInfoParser::Instance()->CheckDutyForRight(eGUILD_RIGHTS_JOIN, guild_duty);
    // 길드마스터이면 길드탈퇴 비활성
    CControlWZ* button_control = GetControlWZ_byPosition(kControl_Button_B006);
    if (button_control != NULL)
    {
        button_control->ShowWindowWZ(WZ_SHOW);
        button_control->EnableWindowWZ((guild_duty != eGUILD_DUTY_MASTER));
    }
    button_control = GetControlWZ_byPosition(kControl_Button_B009);
    if (button_control != NULL)
    {
        button_control->ShowWindowWZ(WZ_SHOW);
        button_control->EnableWindowWZ(right_invite);
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::ShowButton(DWORD control_id, BOOL is_show)
{
    CCtrlButtonWZ* button_ptr = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(control_id)));

    if (button_ptr)
    {
        button_ptr->EnableWindowWZ(is_show?true:false);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildIntro::RefreshTab()
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
        check_button->SetCheckState((position == kControl_CheckBT_C000) ? (true) : (false));
    }
}

void uiGuildIntro::UpdateGuildCoin()
{
    CCtrlStaticWZ* guild_coin_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));

    BYTE guild_grade = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGrade;
    const GuildRankInfo* current_guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_grade);
    const GuildRankInfo* to_next_guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_grade + 1);

    DWORD need_coin = 0;
    DWORD prev_need_coin = 0;
    DWORD guild_coin = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildCoin;
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

void uiGuildIntro::UpdateGuildEXPGauge( float gauge_rate )
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
