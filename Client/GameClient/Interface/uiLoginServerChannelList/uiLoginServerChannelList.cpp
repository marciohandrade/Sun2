#include "SunClientPrecompiledHeader.h"

#include "uiLoginServerChannelList.h"
#include "uiLoginMan/uiLoginMan_def.h"
#include "uiLoginMan/uiLoginMan.h"
#include "GlobalFunc.h"
#include "uiAssistMan/uiAssistMan.h"
#ifdef _DH_LAST_SELECTED_CHARACTER
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#endif//
//------------------------------------------------------------------------------ 
uiLoginServerChannelList::uiLoginServerChannelList(InterfaceManager* ui_man) : uiBase(ui_man)
{
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
    RegisterControlPosition(StrToWzID("T002"), kControl_Text_T002);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("V003"), kControl_VScroll_V003);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
    RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
    RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
    RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
    RegisterControlPosition(StrToWzID("P026"), kControl_Picture_P026);
    RegisterControlPosition(StrToWzID("P027"), kControl_Picture_P027);
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P028);
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032);
    RegisterControlPosition(StrToWzID("P033"), kControl_Picture_P033);
    RegisterControlPosition(StrToWzID("P034"), kControl_Picture_P034);
    RegisterControlPosition(StrToWzID("P035"), kControl_Picture_P035);
    RegisterControlPosition(StrToWzID("P036"), kControl_Picture_P036);
    RegisterControlPosition(StrToWzID("P037"), kControl_Picture_P037);
    RegisterControlPosition(StrToWzID("P038"), kControl_Picture_P038);
    RegisterControlPosition(StrToWzID("P039"), kControl_Picture_P039);

    RegisterControlPosition(StrToWzID("P040"), kControl_Picture_P040);
    RegisterControlPosition(StrToWzID("P041"), kControl_Picture_P041);
    RegisterControlPosition(StrToWzID("P042"), kControl_Picture_P042);
    RegisterControlPosition(StrToWzID("P043"), kControl_Picture_P043);
    RegisterControlPosition(StrToWzID("P044"), kControl_Picture_P044);
    RegisterControlPosition(StrToWzID("P045"), kControl_Picture_P045);
    RegisterControlPosition(StrToWzID("P046"), kControl_Picture_P046);
    RegisterControlPosition(StrToWzID("P047"), kControl_Picture_P047);
    RegisterControlPosition(StrToWzID("P048"), kControl_Picture_P048);
    RegisterControlPosition(StrToWzID("P049"), kControl_Picture_P049);

    m_pNewServerIcon = NULL;
}
//------------------------------------------------------------------------------ 
uiLoginServerChannelList::~uiLoginServerChannelList(void)
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::Init(CDrawBase* drawbase)
{
    uiBase::Init(drawbase);

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    if (list_control != NULL)
    {
        list_control->SetHideReportRender(true);
    }

	

    CControlWZ* button_control = GetControlWZ(GetControlID_byPosition(kControl_Button_B005));
    if (button_control != NULL)
    {
#ifndef _CHINA //#if'N'def
        button_control->ShowWindowWZ(WZ_HIDE);
#endif //_CHINA
    }

    UpdateChannelList();

    m_pNewServerIcon = ICONMGR()->Add(71005);

	hide_check_mouse_ = false;
	m_fTrans = 0.0f;
	m_dwTransAcc = 0;
	m_dwTransEnd = (SEC_PER_TICK);
}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::Release(void)
{
    uiBase::Release();
}
//------------------------------------------------------------------------------ 
uiLoginMan* uiLoginServerChannelList::GetManager()
{
    return GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
}
//------------------------------------------------------------------------------
AuthGroupData* uiLoginServerChannelList::GetCurrentSelectedAuthGroup()
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    if (list_control == NULL)
    {
        return NULL;
    }
    if (list_control->GetCurSel() == -1)
    {
        return NULL;
    }

    int select_index = list_control->GetCurSel();
    return reinterpret_cast<AuthGroupData*>(list_control->GetItemData(select_index));
}

//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
//        int a = 0;
//#ifdef _DH_LAST_SELECTED_CHARACTER
//        //마지막 접속 정보 요청
//        uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
//        if (select_manager)
//        {
//            select_manager->SEND_SELECT_CHARACTER_DATA_GAMEOPTION_LOAD_SYN();
//        }
//#endif//_DH_LAST_SELECTED_CHARACTER
		m_fTrans = 0.0f;
		m_dwTransAcc = 0;

		if(uiLogineAsigned::is_show_count_Account_ <= 0)
		{
			m_dwTransEnd = uiLogineAsigned::is_show_coint_ServerList_ > 1 ? (SEC_PER_TICK) :3500 ;
		}
		else
		{
			m_dwTransEnd = (SEC_PER_TICK);
		}
		
		hide_check_mouse_ = false;;
		uiLogineAsigned::is_show_coint_ServerList_++;
    }
    else
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
        if (list_control != NULL)
        {
            list_control->SetCurSel(-1);
            list_control->SetBeginDrawIndex(0);
            UpdateChannelList();
        }
    }

}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::Process(DWORD tick)
{
	if (!this->IsVisible())
		return;

	m_dwTransAcc += tick;

	if (m_dwTransAcc >= m_dwTransEnd)
	{
		float fPercent =
			((float)(m_dwTransAcc - m_dwTransEnd) / (float)m_dwTransEnd);

		float fTrans = 1.0f - fPercent;
		if( fTrans < 0.0f )
			fTrans = 0.0f;

		m_fTrans = 1.0f - (fTrans * 1.0f);
	}

// 	if (m_fTrans >= .2f)
// 	{
// 		CCtrlStaticWZ* pctrl;
// 
// 		pctrl = (CCtrlStaticWZ *)GetControlWZ(StrToWzID("T001"));
// 		if (pctrl)
// 		{
// 			pctrl->ShowWindowWZ(WZ_SHOW);
// 		}
// 
// 		pctrl = (CCtrlStaticWZ *)GetControlWZ(StrToWzID("T002"));
// 		if (pctr
// 		{
// 			pctrl->ShowWindowWZ(WZ_SHOW);
// 		}
// 
// 		m_pBtnControl->ShowWindowWZ(WZ_SHOW);
// 		m_pEdtControls[0]->ShowWindowWZ(WZ_SHOW);
// 		m_pEdtControls[1]->ShowWindowWZ(WZ_SHOW);
// 	}
// 	else
// 	{
// 		CCtrlStaticWZ* pctrl;
// 
// 		pctrl = (CCtrlStaticWZ *)GetControlWZ(StrToWzID("T001"));
// 		if (pctrl)
// 		{
// 			pctrl->ShowWindowWZ(WZ_HIDE);
// 		}
// 
// 		pctrl = (CCtrlStaticWZ *)GetControlWZ(StrToWzID("T002"));
// 		if (pctrl)
// 		{
// 			pctrl->ShowWindowWZ(WZ_HIDE);
// 		}
// 
// 		m_pBtnControl->ShowWindowWZ(WZ_HIDE);
// 		m_pEdtControls[0]->ShowWindowWZ(WZ_HIDE);
// 		m_pEdtControls[1]->ShowWindowWZ(WZ_HIDE);
// 	}

	if(hide_check_mouse_)
	{

		float SetAlpah = 1.0f - m_fTrans;
		SetAlpah = SetAlpah <= 0.0f ? 0.0f : SetAlpah;

		GetDialogWZ()->SetTransparency(SetAlpah);

		if(SetAlpah == 0.0f)
		{
			this->ShowDialog(false);
			hide_check_mouse_ = false;
		}

		return;
	}

	GetDialogWZ()->SetTransparency(m_fTrans);
}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::OnRenderSolarDialog()
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    CCtrlVScrollWZ* vscroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_V000)));

    if (list_control == NULL || vscroll_control == NULL)
    {
        return;
    }

    const int c_ListHeight = 20;
    RECT rtSrc = {0,0,63,16};

    set<int>& newServer = PROGRAMPARAM->GetNewServerSet();

    int start = vscroll_control->GetScrollPosWZ();
    int end = min (start + kServerNumPerList, GetManager()->GetAuthGroupNum());

    for (int i = start;i < end;++i)
    {
        AuthGroupData* pAuthGroupData = GetManager()->GetAuthGroupByIndex(i);

        if (newServer.find(pAuthGroupData->GetAuthGroupID()) != newServer.end())
        {
            RECT rt = list_control->GetSizeRect();
            rt.left += 127;
            rt.right = rt.left + rtSrc.right;
            rt.top += (i - start) * c_ListHeight;
            rt.top += 2;
            rt.bottom = rt.top + rtSrc.bottom;	

            if( m_pNewServerIcon )
            {
                m_pNewServerIcon->Render(&rt, &rtSrc);
            }
        }		
    }	

}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::UpdateServerList()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    CCtrlVScrollWZ* vscroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_V000)));

    if ((GetManager() == NULL) || 
        (GetManager()->GetAuthGroupNum() == 0) || 
        (list_control == NULL) || 
        (vscroll_control == NULL))
    {
        return;
    }

    list_control->DeleteItemAll();
    list_control->SetCurSel(-1);

    for (int i = 0; i < GetManager()->GetAuthGroupNum(); ++i)
    {
        AuthGroupData* auth_group_ata = GetManager()->GetAuthGroupByIndex(i);
        if (auth_group_ata == NULL)
        {
            break;
        }

        int idx = list_control->InsertItemBack();
        Sprintf (szMessage, _T("%s"), auth_group_ata->GetAuthGroupName());
        GlobalFunc::SetItemText(list_control, idx, szMessage);
        list_control->SetItemTextColor(idx, 0, RGBA(235,192,38, 255));
        list_control->SetItemFontID(idx, 0, StrToWzID("m216"));
        list_control->SetItemBKColor(idx, 0, RGBA(0,0,0,0));   
#ifdef _NA_000000_20130114_RENEWER_UI
		list_control->SetColorSelBar(RGBA(46,58,72,255));
#else
		list_control->SetColorSelBar(RGBA(59, 47, 31, 255));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

        //list_control->SetItemData(idx,(VOID *)i);
        list_control->SetItemData(idx, reinterpret_cast<VOID*>(auth_group_ata));
    }

    int max_num = max(0, GetManager()->GetAuthGroupNum() - kServerNumPerList);
    vscroll_control->SetScrollRangeWZ(0, max_num);
    vscroll_control->SetScrollPosWZ(0);

    int thumb_size = 100 / (max_num + 1);
    vscroll_control->SetChangeSizeThumb(thumb_size);

    //! 채널 리스트 스크롤바 크기 셋팅
    CCtrlVScrollWZ* channel_vscroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_V003)));
    if (channel_vscroll_control != NULL)
    {
        channel_vscroll_control->SetScrollRangeWZ(0, 0);
        channel_vscroll_control->SetScrollPosWZ(0);
        channel_vscroll_control->SetChangeSizeThumb(100);
    }

    UpdateChannelList();
}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::UpdateChannelList()
{
    //------------------------------------------------------------------------------ 
    CCtrlVScrollWZ* channel_vscroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_V003)));
    if (channel_vscroll_control == NULL)
    {
        return;
    }

    int start_channelno = channel_vscroll_control->GetScrollPosWZ();

    AuthGroupData* auth_group_data = GetCurrentSelectedAuthGroup();

    for (int i = 0; i < kChannelNumPerList; ++i)
    {
        AuthSetverData* auth_setver_data = NULL;
        int data_index = i + start_channelno;
        if ((auth_group_data != NULL) && (data_index < auth_group_data->GetAuthServerDataCount()))
        {
            auth_setver_data = auth_group_data->GetAuthServerDatabyIndex(data_index);
        }

        UpdateChannel(i, auth_setver_data);
    }
}

//------------------------------------------------------------------------------ 
int uiLoginServerChannelList::GetPictureImageCount(BYTE crowd_grade)
{
#ifdef _NA_000000_20130903_CHANNEL_GAGE_INCREASE
    const int picture_count[kCrowdGradePictureNum] = {5, 6, 7, 7, 8, 8, 9, 9, 9, 10};
#else
    const int picture_count[kCrowdGradePictureNum] = {5, 5, 5, 5, 7, 7, 7, 9, 9, 10};
#endif //_NA_000000_20130903_CHANNEL_GAGE_INCREASE

    if (crowd_grade >= kCrowdGradePictureNum)
    {
        return 0;
    }

    return picture_count[crowd_grade];
}
//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::UpdateChannel(int index, AuthSetverData* auth_setver_data)
{
    int picture_start_index = (index * kCrowdGradePictureNum) + kControl_Picture_P000;

    CCtrlPictureWZ* picture_control = NULL;
    CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B000 + index)));
    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S000 + index)));
    if (static_control == NULL || button_control == NULL)
    {
        return;
    }

    static_control->ShowWindowWZ(WZ_HIDE);
    button_control->ShowWindowWZ(WZ_HIDE);
    button_control->SetUserData(NULL);

    for (int i = 0; i < kCrowdGradePictureNum; ++i)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(picture_start_index + i)));
        picture_control->ShowWindowWZ(WZ_HIDE);
    }

    if ((auth_setver_data == NULL) || (auth_setver_data->m_CrowdGrade == eAUTH_SERVER_STAT_NONE))
    {
        return;
    }

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {NULL, };

    Sprintf (szMessage, _T("%s"), auth_setver_data->m_ServerName.c_str());

    static_control->SetFontID(StrToWzID("mn12"));
    static_control->SetTextColor( RGBA(240, 201, 121, 255) );
    static_control->SetTextWZ(szMessage);
    static_control->ShowWindowWZ(WZ_SHOW);

    button_control->ShowWindowWZ(WZ_SHOW);
    button_control->SetUserData(reinterpret_cast<DWORD>(auth_setver_data));

    int show_count = GetPictureImageCount(auth_setver_data->m_CrowdGrade);

    for (int i = 0; i < show_count; ++i)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(picture_start_index + i)));
        if (picture_control != NULL)
        {
            picture_control->ShowWindowWZ(WZ_SHOW);
        }
    }
}

//------------------------------------------------------------------------------ 
void uiLoginServerChannelList::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetManager() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(message);
        break;
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(message);
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        OnVScrollThumb(message);

    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------ 
bool uiLoginServerChannelList::OnVScrollThumb(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));
    if (scroll_control == NULL)
    {
        return true;
    }

    int pos2 = scroll_control->GetScrollPosWZ();
    int pos = pos2;
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);

    pos = max(mmin, pos);
    pos = min(mmax, pos);

    if(pos != pos2)
    {
        scroll_control->SetScrollPosWZ(pos);
    }

    //------------------------------------------------------------------------------ 
    if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));

        if (list_control != NULL)
        {
            list_control->SetBeginDrawIndex(pos);
        }
    }
    else if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V003)
    {
        UpdateChannelList();
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiLoginServerChannelList::OnVScrollUp(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));
    if (scroll_control == NULL)
    {
        return true;
    }

    int pos = scroll_control->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {
        --pos;
        scroll_control->SetScrollPosWZ(pos);
    }

    //------------------------------------------------------------------------------ 
    if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));

        if (list_control != NULL)
        {
             list_control->SetBeginDrawIndex(pos);
        }
    }
    else if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V003)
    {
        UpdateChannelList();
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiLoginServerChannelList::OnVScrollDown(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));
    if (scroll_control == NULL)
    {
        return true;
    }

    int pos = scroll_control->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    scroll_control->GetScrollRangeWZ(&mmin, &mmax);

    if (pos < mmax)
    {
        ++pos;
        scroll_control->SetScrollPosWZ(pos);
    }

    //------------------------------------------------------------------------------ 
    if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));

        if (list_control != NULL)
        {
            list_control->SetBeginDrawIndex(pos);
        }
    }
    else if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V003)
    {
        UpdateChannelList();
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiLoginServerChannelList::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B000:
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
        {
            CControlWZ* button_control = GetControlWZ(message->dwCtrlID);
            if (button_control != NULL)
            {
                AuthSetverData* auth_server_data = NULL;

                auth_server_data = reinterpret_cast<AuthSetverData*>(button_control->GetUserData());

                if (auth_server_data)
                {
#ifdef _SCJ_PK_POPUP_DIALOG
                    //! pk안내 팝업창 뛰움
                    uiAssistMan * assist_man = GET_DIALOG_MANAGER(uiAssistMan, UIMAN_ASSIST);
                    if (assist_man != NULL)
                    {
                        assist_man->ShowAskPkPopupDialog(true, auth_server_data);
                    }
#else
    #ifdef _DEV_VER
                    TCHAR szTitle[MAX_PATH];
                    TCHAR temp_string[MAX_PATH];
                    GetWindowText(g_hWndMain, temp_string, sizeof(szTitle));
                    sprintf(szTitle, "%s@%s", strtok(temp_string, "@"), auth_server_data->m_ServerName.c_str());
                    ::SetWindowText(g_hWndMain, szTitle);
    #endif
                    GetManager()->SEND_AUTH_SERVER_SELECT_SYN(auth_server_data);
#endif // _SCJ_PK_POPUP_DIALOG
                }
            }
        }
        break;

    case kControl_Button_B005:
        {
            GetManager()->resetTryReconnect();
        }
        break;

    case kControl_List_L001:
        {
            CCtrlVScrollWZ* channel_vscroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_V003)));
            if (channel_vscroll_control != NULL)
            {
                int max_num = 0;
                AuthGroupData* auth_group_data = GetCurrentSelectedAuthGroup();
                if (auth_group_data != NULL)
                {
                    max_num = max(0, auth_group_data->GetAuthServerDataCount() - kChannelNumPerList);
                }
                channel_vscroll_control->SetScrollRangeWZ(0, max_num);
                channel_vscroll_control->SetScrollPosWZ(0);

                int thumb_size = 100 / (max_num + 1);
                channel_vscroll_control->SetChangeSizeThumb(thumb_size);

                UpdateChannelList();
            }
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiLoginServerChannelList::hideCheckonShow( bool hide )
{
	hide_check_mouse_ = hide;
	m_fTrans = 0.0f;
	m_dwTransAcc = 0;
	m_dwTransEnd = (SEC_PER_TICK);
}