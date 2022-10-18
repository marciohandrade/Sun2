#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_FriendList.h"
#include "uiConvenienceStoreMan.h"
#include "InterfaceManager.h"
#include "uiConvenienceStore_Purchase.h"
#include "GlobalFunc.h"
#include "uiConvenienceStore_Gift.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemPacketFunction.h"
    #include "GuildSystem/GuildSystemData.h"
    #include "GuildSystem/uiGuildMan.h"
#else
    #include "uiGuildMan\uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiCharPopupMenu\uiCharPopupMenu_def.h"
#include "uiCommunityMan\uiCommunityMan.h"
#include "uiCommunityInfo\uiCommunityInfo.h"
#include "Hero.h"

//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_FriendList::kDialog_ID = StrToWzID("0479");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_FriendList::uiConvenienceStore_FriendList(InterfaceManager* ui_man) : uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("CT01"), kControl_CheckBT_CT01);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_FriendList::~uiConvenienceStore_FriendList( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_FriendList::Init(CDrawBase* drawbase)
{
    menu_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C001)), uiConvenienceStore_FriendList::kTabMenu_Friend);
    menu_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_CT01)), uiConvenienceStore_FriendList::kTabMenu_Guild);
    menu_tab().Click(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C001)));

    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(kControl_List_L000));
    if (list_control != NULL) 
    {
        GlobalFunc::SetReportButtonInfo(list_control, 0, 175, _T(""));
        GlobalFunc::SetReportButtonInfo(list_control, 1, 75, _T(""));
        list_control->SetHideReportRender(true);
        list_control->DeleteItemAll();
        list_control->SetColorSelBar(RGBA(0,51,102,100));
    }

    ASSERT(true && "uiConvenienceStore_FriendList::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_FriendList::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        //! 정보 요청
        uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
        if (community_man != NULL)
        {
            community_man->SEND_CW_FRIEND_NEWLIST_SYN();
            community_man->SEND_CW_MEMO_LIST_REQ();
            community_man->SEND_CW_GROUP_NEWLIST_SYN(); 
        }

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GuildSystem::PacketFunction::SEND_CG_GUILD_TOTALINFO_REQ_SYN();
#else
        uiGuildMan* guild_man = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        if (guild_man != NULL)
        {
            guild_man->SEND_CG_GUILD_TOTALINFO_REQ_SYN(this);
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER        

        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiConvenienceStore_FriendList::kDialog_ID;
        msg.wParam = uiConvenienceStore_FriendList::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 

        menu_tab().Click(uiConvenienceStore_FriendList::kTabMenu_Friend);

        RefreshFriendList();
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_FriendList::kDialog_ID);

        //------------------------------------------------------------------------------ 
        uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
        if ((community_man != NULL) &&
            ( community_man->GetCommunityinfo_Dialog() != NULL))
        {
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            if (community_man->GetCommunityinfo_Dialog()->request_uimanager_id() == UIMAN_CONVENIENCESTORE)
            {
                community_man->GetCommunityinfo_Dialog()->ShowInterface(val);
            }
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
        }
    }

}
//------------------------------------------------------------------------------
void uiConvenienceStore_FriendList::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetManager() == NULL)
    {
        return;
    }

    //------------------------------------------------------------------------------ 
    if (menu_tab().MessageProc(message))  
    {
        DWORD user_data = 0; 
        if (menu_tab().Current(user_data) != NULL)
        {
            RefreshFriendList();
        }
        return;
    }

    //------------------------------------------------------------------------------ 
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_LBUTTONDCLICK:
        OnLDButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_FriendList::OnLDButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_List_L000:
        {
            CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(kControl_List_L000));
            if (list_control == NULL)
            {
                break;
            }
            LPCTSTR name = list_control->GetItemText(list_control->GetCurSel(), 0);
            if (name == NULL)
            {
                break;
            }

            CHARGUID char_guid = 0;
            uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
            uiGuildMan* guild_man = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
            if (community_man == NULL || guild_man == NULL)
            {
                break;
            }

            if (menu_tab().CurrentUserData() == kTabMenu_Friend)
            {
                char_guid = community_man->GetGUID((TCHAR*)name);
            }
            else if (menu_tab().CurrentUserData() == kTabMenu_Guild)
            {
    #ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                GUILD_MEMBER_INFO* member_info = GuildSystem::GetGuildSystemData().GetGuildMemberInfo(name);
    #else
                stGUILD_MEMBER_INFO* member_info = guild_man->GetGuildMemberInfo(name);
    #endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                if (member_info != NULL)
                {
                    char_guid = member_info->m_CharGuid;
                }

            }
            else
            {
                break;
            }

            community_man->Send_CW_FRIEND_STATUS_REQ((TCHAR*)name, char_guid);
            if (community_man->GetCommunityinfo_Dialog())
            {
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
                community_man->GetCommunityinfo_Dialog()->Unlock();
                community_man->GetCommunityinfo_Dialog()->set_request_uimanager_id(UIMAN_CONVENIENCESTORE);
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
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
//------------------------------------------------------------------------------
bool uiConvenienceStore_FriendList::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_CheckBT_C001:
        {
        }
        break;
    case kControl_CheckBT_CT01:
        {
        }
        break;
    case kControl_Button_B000:
        {
            //!닫기
            ShowInterface(FALSE);
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
    case kControl_Text_S002:
        {
        }
        break;
    case kControl_List_L000:
        {
            CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(kControl_List_L000));
            if (list_control != NULL)
            {
                LPCTSTR name = list_control->GetItemText(list_control->GetCurSel(), 0);
                LPCTSTR str_level = list_control->GetItemText(list_control->GetCurSel(), 1);
                if (name != NULL/* && str_level != NULL*/)
                {
                    //GetManager()->gift()->CheckName(name);
                    int char_level = 0;
                    _stscanf(str_level, _T("%*s %d"), &char_level);

                    TCHAR now_name[256] = {NULL, };
                    GetManager()->gift()->GetRecvCharacterName(now_name, _countof(now_name) - 1);
                    if (Stricmp(now_name, name) == 0)
                    {
                        GetManager()->gift()->SetRecvCharInfo("", 0, "");
                    }
                    else
                    {
                        if (menu_tab().CurrentUserData() == kTabMenu_Friend)
                        {
                            GetManager()->gift()->SetRecvCharInfo(name, char_level, "");
                        }
                        else if (menu_tab().CurrentUserData() == kTabMenu_Guild)
                        {
                            GetManager()->gift()->SetRecvCharInfo(name, char_level, g_pHero->GetGuildName());
                        }
                    }
                }
            }
        }
        break;
    case kControl_Picture_P001:
        {
        }
        break;
    case kControl_Picture_P002:
        {
        }
        break;
    case kControl_Picture_P003:
        {
        }
        break;
    case kControl_Picture_P004:
        {
        }
        break;
    case kControl_Picture_P000:
        {
        }
        break;
    case kControl_Picture_P005:
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
//------------------------------------------------------------------------------ 
void uiConvenienceStore_FriendList::OnUpdateSolarDialog()
{
    VScrollDialog::Update();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_FriendList::RefreshFriendList()
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(kControl_List_L000));
    if (list_control == NULL)
    {
        return; 
    }
    list_control->DeleteItemAll();

    if (menu_tab().CurrentUserData() == kTabMenu_Friend)
    {
        uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
        if (community_man == NULL)
        {
            return;
        }

        for (int i = 0; i < community_man->m_FriendArray.Size(); ++i)
        {
            FRIEND_DATA& friend_data = community_man->m_FriendArray[i];

            AddFriendList(list_control, friend_data.name.Get(), 
                            friend_data.lv, 
                            static_cast<eCHAR_TYPE>(friend_data.bClassCode));
        }
    }
    else //if (menu_tab().CurrentUserData() == kTabMenu_Guild)
    {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GuildSystem::GuildSystemData::GuildMemberContainer::iterator found_itr = 
            GuildSystem::GetGuildSystemData().guild_member_container().begin();
        GuildSystem::GuildSystemData::GuildMemberContainer::iterator end_itr = 
            GuildSystem::GetGuildSystemData().guild_member_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_MEMBER_INFO& guild_member_info = (*found_itr);
            if ((g_pHero != NULL) && 
                (StrnCmp(g_pHero->GetName(), guild_member_info.m_tszCharName, MAX_CHARNAME_LENGTH) == 0))
            {
                continue;
            }

            AddFriendList(list_control, guild_member_info.m_tszCharName, 
                guild_member_info.m_Level, 
                static_cast<eCHAR_TYPE>(guild_member_info.m_byClass));
        }
#else
        uiGuildMan* guild_man = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        if (guild_man == NULL)
        {
            return;
        }

        for (int i = 0; i < static_cast<int>(guild_man->GetGuildMembers().size()); ++i)
        {
            stGUILD_MEMBER_INFO* member_info = guild_man->GetGuildMemberInfo(i);

            if ((g_pHero != NULL) && 
                (StrnCmp(g_pHero->GetName(), member_info->m_tszCharName, MAX_CHARNAME_LENGTH) == 0))
            {
                continue;
            }
            AddFriendList(list_control, member_info->m_tszCharName, 
                member_info->m_Level, 
                static_cast<eCHAR_TYPE>(member_info->m_byClass));
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER        
    }

#if WZENGINEVER >= 400
    int max_line_count = list_control->GetListRowCnt();
    int draw_line_count = list_control->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);
    VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);
#endif //#if WZENGINEVER >= 400
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_FriendList::AddFriendList(CCtrlListWZ* list_control, const TCHAR* char_name, LEVELTYPE level, eCHAR_TYPE char_type)
{
    int index = list_control->InsertItemBack();
    if (index >= 0)
    {
        list_control->SetItemWZA(index, 0, char_name, 
            StrToWzID("st10"), DT_VCENTER | DT_CENTER, c_dwBackColor, c_dwTextColor, c_dwSelTextColor);

        TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
        Snprintf(msg, INTERFACE_STRING_LENGTH-1, _T("Lv. %d"), level);
        list_control->SetItemWZA(index, 1, msg, 
            StrToWzID("st10"), DT_VCENTER | DT_CENTER, c_dwBackColor, c_dwTextColor, c_dwSelTextColor);

        list_control->SetItemWZA(index, 2, GlobalFunc::GetClassString(char_type), 
            StrToWzID("st10"), DT_VCENTER | DT_CENTER, c_dwBackColor, c_dwTextColor, c_dwSelTextColor);
    }
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiConvenienceStore_FriendList::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_FriendList::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_FriendList::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollThumb(msg) == true)
    {
        if (GetControlID_byPosition(kControl_List_L000))
            GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_FriendList::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollUp(msg) == true)
    {
        if (GetControlID_byPosition(kControl_List_L000))
            GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_FriendList::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollDown(msg) == true)
    {
        if (GetControlID_byPosition(kControl_List_L000))
            GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
