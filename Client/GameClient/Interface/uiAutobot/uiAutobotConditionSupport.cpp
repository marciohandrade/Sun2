#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotConditionSupport.h"
#include "uiAutobotMan.h"
#include "uiAutobotConditionBattle.h"

#include "Autobot/Condition/AutobotConditionManager.h"

#include "Map.h"
#include "GlobalFunc.h"
#include "AutomaticInfoParser.h"
#include "AutomaticComboInfoParser.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"

//------------------------------------------------------------------------------ 
const DWORD uiAutobotConditionSupport::kDialog_ID = StrToWzID("9520");;

//------------------------------------------------------------------------------ 
uiAutobotConditionSupport::uiAutobotConditionSupport(InterfaceManager* ui_man) : uiAutobotDialog(ui_man)
{
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C010"), kControl_CheckBT_C010);
    RegisterControlPosition(StrToWzID("C024"), kControl_CheckBT_C024);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009);
    RegisterControlPosition(StrToWzID("C011"), kControl_CheckBT_C011);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C012"), kControl_CheckBT_C012);
    RegisterControlPosition(StrToWzID("C013"), kControl_CheckBT_C013);
    RegisterControlPosition(StrToWzID("C014"), kControl_CheckBT_C014);
    RegisterControlPosition(StrToWzID("C015"), kControl_CheckBT_C015);
    RegisterControlPosition(StrToWzID("C016"), kControl_CheckBT_C016);
    RegisterControlPosition(StrToWzID("C017"), kControl_CheckBT_C017);
    RegisterControlPosition(StrToWzID("C018"), kControl_CheckBT_C018);
    RegisterControlPosition(StrToWzID("C019"), kControl_CheckBT_C019);
    RegisterControlPosition(StrToWzID("C020"), kControl_CheckBT_C020);
    RegisterControlPosition(StrToWzID("C021"), kControl_CheckBT_C021);
    RegisterControlPosition(StrToWzID("C022"), kControl_CheckBT_C022);
    RegisterControlPosition(StrToWzID("C025"), kControl_CheckBT_C025);
    RegisterControlPosition(StrToWzID("C023"), kControl_CheckBT_C023);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B034"), kControl_Button_B034);
    RegisterControlPosition(StrToWzID("B035"), kControl_Button_B035);
    RegisterControlPosition(StrToWzID("B036"), kControl_Button_B036);
    RegisterControlPosition(StrToWzID("B037"), kControl_Button_B037);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("T101"), kControl_Text_T101);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("T006"), kControl_Text_T006);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S026"), kControl_Text_S026);
    RegisterControlPosition(StrToWzID("S031"), kControl_Text_S031);
    RegisterControlPosition(StrToWzID("S032"), kControl_Text_S032);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S033"), kControl_Text_S033);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("S021"), kControl_Text_S021);
    RegisterControlPosition(StrToWzID("S022"), kControl_Text_S022);
    RegisterControlPosition(StrToWzID("S023"), kControl_Text_S023);
    RegisterControlPosition(StrToWzID("S024"), kControl_Text_S024);
    RegisterControlPosition(StrToWzID("S025"), kControl_Text_S025);
    RegisterControlPosition(StrToWzID("S027"), kControl_Text_S027);
    RegisterControlPosition(StrToWzID("S028"), kControl_Text_S028);
    RegisterControlPosition(StrToWzID("S029"), kControl_Text_S029);
    RegisterControlPosition(StrToWzID("S030"), kControl_Text_S030);
    RegisterControlPosition(StrToWzID("S034"), kControl_Text_S034);
    RegisterControlPosition(StrToWzID("S035"), kControl_Text_S035);
    RegisterControlPosition(StrToWzID("S036"), kControl_Text_S036);
    RegisterControlPosition(StrToWzID("S037"), kControl_Text_S037);
    RegisterControlPosition(StrToWzID("S038"), kControl_Text_S038);
    RegisterControlPosition(StrToWzID("S041"), kControl_Text_S041);
    RegisterControlPosition(StrToWzID("S042"), kControl_Text_S042);
    RegisterControlPosition(StrToWzID("S044"), kControl_Text_S044);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);

    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("L003"), kControl_List_L003);
    RegisterControlPosition(StrToWzID("L004"), kControl_List_L004);
    RegisterControlPosition(StrToWzID("L005"), kControl_List_L005);
    RegisterControlPosition(StrToWzID("L006"), kControl_List_L006);
    RegisterControlPosition(StrToWzID("L007"), kControl_List_L007);
    RegisterControlPosition(StrToWzID("L008"), kControl_List_L008);
    RegisterControlPosition(StrToWzID("L009"), kControl_List_L009);
    RegisterControlPosition(StrToWzID("L010"), kControl_List_L010);
    RegisterControlPosition(StrToWzID("L011"), kControl_List_L011);

    RegisterControlPosition(StrToWzID("VS00"), kControl_VScroll_VS00);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
    RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);
    RegisterControlPosition(StrToWzID("P055"), kControl_Picture_P055);
    RegisterControlPosition(StrToWzID("P056"), kControl_Picture_P056);
    RegisterControlPosition(StrToWzID("P057"), kControl_Picture_P057);
    RegisterControlPosition(StrToWzID("P058"), kControl_Picture_P058);
    RegisterControlPosition(StrToWzID("P059"), kControl_Picture_P059);
    RegisterControlPosition(StrToWzID("P060"), kControl_Picture_P060);
    RegisterControlPosition(StrToWzID("P061"), kControl_Picture_P061);
    RegisterControlPosition(StrToWzID("P062"), kControl_Picture_P062);
    RegisterControlPosition(StrToWzID("P063"), kControl_Picture_P063);
    RegisterControlPosition(StrToWzID("P064"), kControl_Picture_P064);
    RegisterControlPosition(StrToWzID("P065"), kControl_Picture_P065);
    RegisterControlPosition(StrToWzID("P067"), kControl_Picture_P067);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
    RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
    RegisterControlPosition(StrToWzID("PI00"), kControl_Picture_PI00);
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
    RegisterControlPosition(StrToWzID("P026"), kControl_Picture_P026);

    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("P027"), kControl_Picture_P027);
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P028);
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032);

    RegisterControlPosition(StrToWzID("TI07"), kControl_TextImage_TI07);
    RegisterControlPosition(StrToWzID("TI06"), kControl_TextImage_TI06);

    //! 2011.9.16 / i4u4me /  
    RegisterControlPosition(StrToWzID("C026"), kControl_CheckBT_C026);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("P033"), kControl_Picture_P033);
}

//------------------------------------------------------------------------------ 
uiAutobotConditionSupport::~uiAutobotConditionSupport( void )
{
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::Init(CDrawBase* drawbase)
{
    GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C006))->SetCheckState(true);
    GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C006))->SetDiscardMessage(true);

    int combo_index = 0;
    std::vector<BASE_AutomaticComboInfo*> combo_vector;

    //! 귀환 : 인벤토리
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_RETURNSCROLL_INVEN)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L002, combo_vector);

    //! 귀환 : hp물약
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_RETURNSCROLL_POTION)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L005, combo_vector);

    //! 귀환 : 사료
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_RETURNSCROLL_PET_FEED)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L006, combo_vector);

    //! 귀환 : 장비류
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_RETURNSCROLL_DURATION)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L007, combo_vector);

    //! 자동구매 : 귀환 스크롤
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_RETURNSCROLL)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L000, combo_vector);

    //! 자동구매 : 사료
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_PET_FEED)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L001, combo_vector);

    //! 자동구매 : 회복 hp 레벨
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_POTION_LEVEL)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L003, combo_vector);

    //! 자동구매 : 회복 hp 갯수
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_POTION_COUNT)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L004, combo_vector);

    //! 자동채널 
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_COMMUNITY_CHANNEL)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L010, combo_vector);
    {
        CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(kControl_List_L010));
        CCtrlVScrollWZ* scroll_control = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_VS00));
        if (scroll_control != NULL && list_control != NULL)
        {
            scroll_control->SetScrollRangeWZ(0, list_control->GetListRowCnt() - 7/*리스트박스 한페이지에 보여줄 갯수*/);
        }
    }

    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_COMMUNITY_CHANNEL)->combo_index[1];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L011, combo_vector);
    {
        CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(kControl_List_L011));
        CCtrlVScrollWZ* scroll_control = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
        if (scroll_control != NULL && list_control != NULL)
        {
            scroll_control->SetScrollRangeWZ(0, list_control->GetListRowCnt() - 7/*리스트박스 한페이지에 보여줄 갯수*/);
        }
    }

    //! 외치기 
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_COMMUNITY_SHOUT)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L008, combo_vector);

    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_COMMUNITY_SHOUT)->combo_index[1];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    InsertItemToDropdown(kControl_List_L009, combo_vector);
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::Release(void)
{

}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Dialog_Autobot;
        msg.wParam = uiAutobotConditionSupport::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Autobot);
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiAutobotConditionSupport::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        OnUpScroll(message);
        break;
    case RT_MSG_SB_LINEBOTTOM:
        OnDownScroll(message);
        break;
    case RT_MSG_SB_THUMBPOSITION:
        OnThumbPosition(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
bool uiAutobotConditionSupport::OnThumbPosition(SI_MESSAGE* message)
{
    UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_pos)
    {
    case kControl_VScroll_VS00:
        {
            //자동체널 레벨
            int index = GetVScrollControlWZ(message->dwCtrlID)->GetScrollPosWZ();
            GetListControlWZ(GetControlID_byPosition(kControl_List_L010))->SetBeginDrawIndex(index);
        }
        break;
    case kControl_VScroll_V000:
        {
            // 자동 체널 이름
            int index = GetVScrollControlWZ(message->dwCtrlID)->GetScrollPosWZ();
            GetListControlWZ(GetControlID_byPosition(kControl_List_L011))->SetBeginDrawIndex(index);
        }
        break;
    default:
        {
        }
        break;
    }

    return true;
}
//------------------------------------------------------------------------------
bool uiAutobotConditionSupport::OnDownScroll(SI_MESSAGE* message)
{
    UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_pos)
    {
    case kControl_VScroll_VS00:
        {
            //자동체널 레벨
            OnDropdownScrollDown(kControl_List_L010, control_pos);
        }
        break;
    case kControl_VScroll_V000:
        {
            // 자동 체널 이름
            OnDropdownScrollDown(kControl_List_L011, control_pos);
        }
        break;
    default:
        {
        }
        break;
    }

    return true;
}
//------------------------------------------------------------------------------
bool uiAutobotConditionSupport::OnUpScroll(SI_MESSAGE* message)
{
    UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_pos)
    {
    case kControl_VScroll_VS00:
        {
            //자동체널 레벨
            OnDropdownScrollUp(kControl_List_L010, control_pos);
        }
        break;
    case kControl_VScroll_V000:
        {
            // 자동 체널 이름
            OnDropdownScrollUp(kControl_List_L011, control_pos);
        }
        break;
    default:
        {
        }
        break;
    }

    return true;
}
//------------------------------------------------------------------------------
bool uiAutobotConditionSupport::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_pos)
    {
    case kControl_CheckBT_C005:
        {
            static_cast<uiAutobotMan*>(GetMan())->ChangeTab(uiAutobotConditionBattle::kDialog_ID);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C005))->SetCheckState(false);
        }
        break;
    case kControl_CheckBT_C006:
        {
        }
        break;
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C007:
    case kControl_CheckBT_C010:
        {
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C001))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C007))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C010))->SetCheckState(false);

            GetCheckButtonControlWZ(message->dwCtrlID)->SetCheckState(true);
        }
        break;
    case kControl_CheckBT_C024:
        {
            //! 자동판매
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C000))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C002))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C003))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C012))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C013))->SetCheckState(false);
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C014))->SetCheckState(false);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C012:
    case kControl_CheckBT_C013:
    case kControl_CheckBT_C014:
        {
            //! 자동판매
            if (GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C000))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C002))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C003))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C012))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C013))->GetCheckState() || 
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C014))->GetCheckState())
            {
                GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C024))->SetCheckState(false);
            }
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_CheckBT_C020:
    case kControl_CheckBT_C021:
        {
            if (IsChecked(control_pos))
            {
                if (control_pos == kControl_CheckBT_C020)
                {
                    SetChecked(kControl_CheckBT_C021, false);
                }
                else if (control_pos == kControl_CheckBT_C021)
                {
                    SetChecked(kControl_CheckBT_C020, false);
                }
            }
            OnDropdownEnable();
        }
        break;
    case kControl_CheckBT_C004:
    case kControl_CheckBT_C008:
    case kControl_CheckBT_C009:
    case kControl_CheckBT_C011:
    case kControl_CheckBT_C015:
    case kControl_CheckBT_C016:
    case kControl_CheckBT_C017:
    case kControl_CheckBT_C018:
    case kControl_CheckBT_C019:
    case kControl_CheckBT_C022:
    case kControl_CheckBT_C025:
    case kControl_CheckBT_C023:
        {
            OnDropdownEnable();
        }
        break;

    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B034:
        {
            //!< 마을귀환 : 인벤토리
            OnDropdownShow(kControl_List_L002, kControl_Picture_P063);
        }
        break;
    case kControl_Button_B035:
        {
            //!< 마을귀환 : 사료
            OnDropdownShow(kControl_List_L006, kControl_Picture_P065);
        }
        break;
    case kControl_Button_B036:
        {
            //!< 마을귀환 : hp 회복
            OnDropdownShow(kControl_List_L005, kControl_Picture_P064);
        }
        break;
    case kControl_Button_B037:
        {
            //!< 마을귀환 : 장비류
            OnDropdownShow(kControl_List_L007, kControl_Picture_P067);
        }
        break;
    case kControl_Button_B000:
        {
            //!< 자동 구매 : 귀환
            OnDropdownShow(kControl_List_L000, kControl_Picture_P016);
        }
        break;
    case kControl_Button_B003:
        {
            //!< 자동 구매 : 일반사료
            OnDropdownShow(kControl_List_L001, kControl_Picture_P017);
        }
        break;
    case kControl_Button_B004:
        {
            //!< 자동 구매 : hp 회복 레벨
            OnDropdownShow(kControl_List_L003, kControl_Picture_P018);
        }
        break;
    case kControl_Button_B005:
        {
            //!< 자동 구매 : hp 회복 갯수
            OnDropdownShow(kControl_List_L004, kControl_Picture_P019);
        }
        break;
    case kControl_Button_B007:
        {
            //!< 커뮤니티 : 외치기 타입
            OnDropdownShow(kControl_List_L008, kControl_Picture_P020);
        }
        break;
    case kControl_Button_B008:
        {
            //!< 커뮤니티 : 외치기 타입
            OnDropdownShow(kControl_List_L009, kControl_Picture_P021);
        }
        break;
    case kControl_Button_B006:
        {
            //!< 커뮤니티 : 자동채널 : 레벨
            OnDropdownShow(kControl_List_L010, kControl_Picture_P023);
        }
        break;
    case kControl_Button_B010:
        {
            //!< 커뮤니티 : 자동채널 : 이름
            OnDropdownShow(kControl_List_L011, kControl_Picture_P025);
        }
        break;
    case kControl_Edit_E000:
        {
        }
        break;
    case kControl_List_L000:    //!< 자동구매 : 귀환스크롤
        {
            ChangeDropdownItem(control_pos, kControl_Text_S023);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L001:    //!< 자동구매 : 사료
        {
            ChangeDropdownItem(control_pos, kControl_Text_S025);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L002:    //!< 귀환:인벤토리
        {
            ChangeDropdownItem(control_pos, kControl_Text_S008);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L003:    //!< 자동구매 : hp 회복
        {
            ChangeDropdownItem(control_pos, kControl_Text_S028);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L004:    //!< 자동구매 : 마나
        {
            ChangeDropdownItem(control_pos, kControl_Text_S030);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L005:    //!< 귀환:hp 물약
        {
            ChangeDropdownItem(control_pos, kControl_Text_S009);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L006:    //!< 귀환:사료
        {
            ChangeDropdownItem(control_pos, kControl_Text_S010);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L007:     //!< 귀환:장비
        {
            ChangeDropdownItem(control_pos, kControl_Text_S033);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L008:    //!< 커뮤니티기능 : 외치기( 타입)
        {
            ChangeDropdownItem(control_pos, kControl_Text_S042);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L009:    //!< 커뮤니티기능 : 외치기(시간:초)
        {
            ChangeDropdownItem(control_pos, kControl_Text_S044);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L010:    //!< 커뮤니티기능 : 자동채널 레벨
        {
            ChangeDropdownItem(control_pos, kControl_Text_S013);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_List_L011:    //!< 커뮤니티기능 : 자동채널 이름
        {
            ChangeDropdownItem(control_pos, kControl_Text_S017);
            OnDropdownShow(kControl_Size, kControl_Size);
        }
        break;
    case kControl_TextImage_TI07:
        {
            //! 확인
            static_cast<uiAutobotMan*>(GetMan())->UpdateCondition();
            ShowInterface(FALSE);
        }
        break;
    case kControl_TextImage_TI06:
        {
            //! 초기화
            DefaultUI();
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
void uiAutobotConditionSupport::OnDropdownShow(UIControlPos list_index, UIControlPos picture_index)
{
    //! 리스트 박스 배경은 다 히든 시킨다.
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P063))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P064))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P065))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P067))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P016))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P017))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P018))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P019))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P020))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P021))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P023))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P025))->ShowWindowWZ(WZ_HIDE); 

    GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_VScroll_V000))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_PI00))->ShowWindowWZ(WZ_HIDE); 
    GetControlWZ(GetControlID_byPosition(kControl_Picture_P026))->ShowWindowWZ(WZ_HIDE); 

    for (int i = (int)kControl_List_L000; i <= (int)kControl_List_L011; ++i)
    {
        if ((static_cast<UIControlPos>(i) == list_index) && 
            (GetListControlWZ(GetControlID_byPosition(list_index))->GetState() == WZ_HIDE))
        {
            GetListControlWZ(GetControlID_byPosition(list_index))->ShowWindowWZ(WZ_SHOW);
            GetControlWZ(GetControlID_byPosition(picture_index))->ShowWindowWZ(WZ_SHOW);

             if (static_cast<UIControlPos>(i) == kControl_List_L010)
             {
                 GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00))->ShowWindowWZ(WZ_SHOW);
                 GetControlWZ(GetControlID_byPosition(kControl_Picture_PI00))->ShowWindowWZ(WZ_SHOW);
             }
             if (static_cast<UIControlPos>(i) == kControl_List_L011)
             {
                 GetControlWZ(GetControlID_byPosition(kControl_VScroll_V000))->ShowWindowWZ(WZ_SHOW);
                 GetControlWZ(GetControlID_byPosition(kControl_Picture_P026))->ShowWindowWZ(WZ_SHOW);
             }
        }
        else
        {
            GetListControlWZ(GetControlID_byPosition(static_cast<UIControlPos>(i)))->ShowWindowWZ(WZ_HIDE);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::OnDropdownScrollDown(UIControlPos list_index, UIControlPos scroll_index)
{
    CCtrlListWZ* listbox = GetListControlWZ(GetControlID_byPosition(list_index));
    CCtrlVScrollWZ* scroll = GetVScrollControlWZ(GetControlID_byPosition(scroll_index));

    int mmin = 0;
    int mmax = 0;
    scroll->GetScrollRangeWZ(&mmin, &mmax);  // 스크롤 값의 최대 최소를 얻어 온다.

    int pos = scroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
    if (pos < mmax)
    {
        ((pos+1) > mmax ? (pos = mmax) : (pos += 1));

        scroll->SetScrollPosWZ(pos);
        listbox->SetBeginDrawIndex(pos);
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::OnDropdownScrollUp(UIControlPos list_index, UIControlPos scroll_index)
{
    CCtrlListWZ* listbox = GetListControlWZ(GetControlID_byPosition(list_index));
    CCtrlVScrollWZ* scroll = GetVScrollControlWZ(GetControlID_byPosition(scroll_index));

    int mmin = 0;
    int mmax = 0;
    scroll->GetScrollRangeWZ(&mmin, &mmax);  // 스크롤 값의 최대 최소를 얻어 온다.

    int pos = scroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
    if ( pos > mmin)
    {
        ((pos - 1) < mmin ? (pos = mmin) : (pos -= 1));

        scroll->SetScrollPosWZ(pos);
        listbox->SetBeginDrawIndex(pos);
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::DefaultUI()
{
    //! 아이템 종류별로 옵션 활성 여부 판대기
    SetVisible(kControl_Picture_P008, WZ_HIDE); 
    SetVisible(kControl_Picture_P024, WZ_HIDE); 
    SetVisible(kControl_Picture_P027, WZ_HIDE); 
    SetVisible(kControl_Picture_P028, WZ_HIDE); 
    SetVisible(kControl_Picture_P032, WZ_HIDE); 
    SetVisible(kControl_Picture_P033, WZ_HIDE); 

    //! 기존 중국나간것 때문에. 현재 소스는 지우지 않는다.
    SetEnable(kControl_CheckBT_C001, (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH) != NULL));           // 자동부활_제자리 부활
    SetEnable(kControl_CheckBT_C007, (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH_VILIAGE) != NULL));   // 자동부활_마을부활
    SetEnable(kControl_CheckBT_C010, (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH_WAITING) != NULL));   // 자동부활_부활대기


    //! 기능 별로 check button control 활성화 여부
    SLOTCODE item_code = 0;
    g_pHero->GetHeroEquipContainer()->IsEquipItem(eITEMTYPE_AUTO_HUNTING, &item_code);

    //------------------------------------------------------------------------------ 
    SetEnableByItem(kControl_CheckBT_C001, true, item_code, kControl_Text_S001);    // 자동부활_마을부활
    SetEnableByItem(kControl_CheckBT_C007, true, item_code, kControl_Text_S002);    // 자동부활_부활대기
    SetEnableByItem(kControl_CheckBT_C010, true, item_code, kControl_Text_T006);    // 자동부활_제자리 부활
    SetEnableByItem(kControl_CheckBT_C004, true, item_code, kControl_Text_S004);    // 마을이동_인벤토리
    SetEnableByItem(kControl_CheckBT_C009, true, item_code, kControl_Text_S006);    // 마을이동_회복물약
    SetEnableByItem(kControl_CheckBT_C008, true, item_code, kControl_Text_S005);    // 마을이동_사료
    SetEnableByItem(kControl_CheckBT_C011, true, item_code, kControl_Text_S007);    // 마을이동_내구도
    SetEnableByItem(kControl_CheckBT_C026, true, item_code, kControl_Text_S015);    // 마을이동_필드로돌아가기
    SetEnableByItem(kControl_CheckBT_C024, true, item_code, kControl_Text_S019);    // 자동판매_모든아이템             
    SetEnableByItem(kControl_CheckBT_C000, true, item_code, kControl_Text_S000);    // 자동판매_일반아이템             
    SetEnableByItem(kControl_CheckBT_C002, true, item_code, kControl_Text_S003);    // 자동판매_퀘스트아이템           
    SetEnableByItem(kControl_CheckBT_C003, true, item_code, kControl_Text_S014);    // 자동판매_스톤아이템
    SetEnableByItem(kControl_CheckBT_C012, true, item_code, kControl_Text_S020);    // 자동판매_무기아이템
    SetEnableByItem(kControl_CheckBT_C013, true, item_code, kControl_Text_S021);    // 자동판매_방어구아이템
    SetEnableByItem(kControl_CheckBT_C014, true, item_code, kControl_Text_S022);    // 자동판매_AC아이템
    SetEnableByItem(kControl_CheckBT_C015, true, item_code, kControl_Text_S024);    // 자동구매_귀환스크롤
    SetEnableByItem(kControl_CheckBT_C016, true, item_code, kControl_Text_S027);    // 자동구매_일반사료
    SetEnableByItem(kControl_CheckBT_C017, true, item_code, kControl_Text_S029);    // 자동구매_회복물약
    SetEnableByItem(kControl_CheckBT_C018, true, item_code, kControl_Text_S034);    // 자동구매_회복물약
    SetEnableByItem(kControl_CheckBT_C019, true, item_code, kControl_Text_S035);    // 자동구매_내구도전체수리
    SetEnableByItem(kControl_CheckBT_C023, true, item_code, kControl_Text_S011);    // 커뮤니티_채널참가
    SetEnableByItem(kControl_CheckBT_C025, true, item_code, kControl_Text_S041);    // 커뮤니티_외치기
    SetEnableByItem(kControl_CheckBT_C020, true, item_code, kControl_Text_S036);    // 커뮤니티_파티수락
    SetEnableByItem(kControl_CheckBT_C021, true, item_code, kControl_Text_S037);    // 커뮤니티_파티거부
    SetEnableByItem(kControl_CheckBT_C022, true, item_code, kControl_Text_S038);    // 커뮤니티_채팅내용저장
    //------------------------------------------------------------------------------ 

    GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000))->SetTextWZ("");

    //! 콤보박스 첫번째것으로 셋팅
    SelectDropdownItem(kControl_List_L000, kControl_Text_S023, 0);
    SelectDropdownItem(kControl_List_L001, kControl_Text_S025, 0);
    SelectDropdownItem(kControl_List_L002, kControl_Text_S008, 0);
    SelectDropdownItem(kControl_List_L003, kControl_Text_S028, 0);
    SelectDropdownItem(kControl_List_L004, kControl_Text_S030, 0);
    SelectDropdownItem(kControl_List_L005, kControl_Text_S009, 0);
    SelectDropdownItem(kControl_List_L006, kControl_Text_S010, 0);
    SelectDropdownItem(kControl_List_L007, kControl_Text_S033, 0);
    SelectDropdownItem(kControl_List_L008, kControl_Text_S042, 0);
    SelectDropdownItem(kControl_List_L009, kControl_Text_S044, 0);
    SelectDropdownItem(kControl_List_L010, kControl_Text_S013, 0);
    SelectDropdownItem(kControl_List_L011, kControl_Text_S017, 0);

    //사냥 설정
    SetCheckedByItem(kControl_CheckBT_C001, false, item_code);  // 자동부활_제자리 부활
    SetCheckedByItem(kControl_CheckBT_C007, false, item_code);  // 자동부활_마을부활
    SetCheckedByItem(kControl_CheckBT_C010, true , item_code);   // 자동부활_부활대기

    //마을 귀환
    SetCheckedByItem(kControl_CheckBT_C004, false, item_code);    // 마을이동_인벤토리
    SetCheckedByItem(kControl_CheckBT_C009, false, item_code);    // 마을이동_회복물약
    SetCheckedByItem(kControl_CheckBT_C008, false, item_code);    // 마을이동_사료
    SetCheckedByItem(kControl_CheckBT_C011, false, item_code);    // 마을이동_내구도
    SetCheckedByItem(kControl_CheckBT_C026, false, item_code);    // 마을이동_내구도

    //! 자동판매
    SetCheckedByItem(kControl_CheckBT_C024, false, item_code);    // 자동판매_모든아이템             
    SetCheckedByItem(kControl_CheckBT_C000, false, item_code);    // 자동판매_일반아이템             
    SetCheckedByItem(kControl_CheckBT_C002, false, item_code);    // 자동판매_퀘스트아이템           
    SetCheckedByItem(kControl_CheckBT_C003, false, item_code);    // 자동판매_스톤아이템
    SetCheckedByItem(kControl_CheckBT_C012, false, item_code);    // 자동판매_무기아이템
    SetCheckedByItem(kControl_CheckBT_C013, false, item_code);    // 자동판매_방어구아이템
    SetCheckedByItem(kControl_CheckBT_C014, false, item_code);    // 자동판매_AC아이템

    //! 자동구매
    SetCheckedByItem(kControl_CheckBT_C015, false, item_code);    // 자동구매_귀환스크롤
    SetCheckedByItem(kControl_CheckBT_C016, false, item_code);    // 자동구매_일반사료
    SetCheckedByItem(kControl_CheckBT_C017, false, item_code);    // 자동구매_회복물약
    SetCheckedByItem(kControl_CheckBT_C018, false, item_code);    // 자동구매_회복물약
    SetCheckedByItem(kControl_CheckBT_C019, false, item_code);    // 자동구매_내구도전체수리

    //! 커뮤니티
    SetCheckedByItem(kControl_CheckBT_C023, false, item_code);    // 커뮤니티_채널참가
    SetCheckedByItem(kControl_CheckBT_C025, false, item_code);    // 커뮤니티_외치기
    SetCheckedByItem(kControl_CheckBT_C020, false, item_code);    // 커뮤니티_파티수락
    SetCheckedByItem(kControl_CheckBT_C021, false, item_code);    // 커뮤니티_파티거부
    SetCheckedByItem(kControl_CheckBT_C022, false, item_code);    // 커뮤니티_채팅내용저장


    OnDropdownEnable();
}
//------------------------------------------------------------------------------ 
bool uiAutobotConditionSupport::UpdateUI()
{
    if (AutobotConditionManager::Instance()->IsLoaded() == false)
    {
        return false;
    }

    int option_value = 0;

    //------------------------------------------------------------------------------ 
    //사냥 설정
    SetChecked(kControl_CheckBT_C001, (AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Resurrection) && 
                                      (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH) != NULL));
    SetChecked(kControl_CheckBT_C007, (AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Village) && 
                                      (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH_VILIAGE) != NULL));
    SetChecked(kControl_CheckBT_C010, (AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Stay) && 
                                      (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH_WAITING) != NULL));

    //------------------------------------------------------------------------------ 
    //! 마을 이동
    SetChecked(kControl_CheckBT_C004, AutobotConditionManager::Instance()->recall().inventory_full_percent() > 0.0f);
    SetChecked(kControl_CheckBT_C009, AutobotConditionManager::Instance()->recall().heal_item_num() > 0);
    SetChecked(kControl_CheckBT_C008, AutobotConditionManager::Instance()->recall().feed_item_num() > 0);
    SetChecked(kControl_CheckBT_C011, AutobotConditionManager::Instance()->recall().warning_dura() > 0);
    SetChecked(kControl_CheckBT_C026, AutobotConditionManager::Instance()->recall().is_return_field());

    if (IsChecked(kControl_CheckBT_C004))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recall().inventory_full_percent());
        SelectDropdownItemByComboValue(kControl_List_L002, kControl_Text_S008, option_value);
    }
    if (IsChecked(kControl_CheckBT_C009))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recall().heal_item_num());
        SelectDropdownItemByComboValue(kControl_List_L005, kControl_Text_S009, option_value);
    }
    if (IsChecked(kControl_CheckBT_C008))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recall().feed_item_num());
        SelectDropdownItemByComboValue(kControl_List_L006, kControl_Text_S010, option_value);
    }
    if (IsChecked(kControl_CheckBT_C011))
    {
        option_value = static_cast<int>(AutobotConditionManager::Instance()->recall().warning_dura());
        SelectDropdownItemByComboValue(kControl_List_L007, kControl_Text_S033, option_value);
    }

    //------------------------------------------------------------------------------ 
    //! 자동 판매
    SetChecked(kControl_CheckBT_C024, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_All));
    SetChecked(kControl_CheckBT_C000, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_Normal));
    SetChecked(kControl_CheckBT_C002, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_Quest));
    SetChecked(kControl_CheckBT_C003, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_Stone));
    SetChecked(kControl_CheckBT_C012, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_Weapon));
    SetChecked(kControl_CheckBT_C013, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_Armor));
    SetChecked(kControl_CheckBT_C014, AutobotConditionManager::Instance()->shop().IsSellItem(AutobotCondition::kItem_AC));

    //------------------------------------------------------------------------------ 
    //! 자동구매
    SLOTCODE item_code = 0;
    int combo_index = 0;
    std::vector<BASE_AutomaticComboInfo*> combo_vector;
    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_RETURNSCROLL)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    if (combo_vector.size() > 0)
    {
        //! 귀환 스크롤(7241)
        BASE_AutomaticComboInfo* info = combo_vector[0];
        item_code = info->item_code;
        SetChecked(kControl_CheckBT_C015, AutobotConditionManager::Instance()->shop().GetBuyItemCount(item_code) > 0);

        if (IsChecked(kControl_CheckBT_C015))
        {
            AutobotCondition_Shop::ConditionBuyItemMap::iterator iter = AutobotConditionManager::Instance()->shop().buyitem_map().find(item_code);
            if (iter != AutobotConditionManager::Instance()->shop().buyitem_map().end())
            {
                option_value = iter->second;
                SelectDropdownItemByComboValue(kControl_List_L000, kControl_Text_S023, option_value);
            }
        }
    }

    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_PET_FEED)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    if (combo_vector.size() > 0)
    {
        //! 사료
        BASE_AutomaticComboInfo* info = combo_vector[0];
        item_code = info->item_code;
        SetChecked(kControl_CheckBT_C016, AutobotConditionManager::Instance()->shop().GetBuyItemCount(item_code) > 0);

        if (IsChecked(kControl_CheckBT_C016))
        {
            AutobotCondition_Shop::ConditionBuyItemMap::iterator iter = AutobotConditionManager::Instance()->shop().buyitem_map().find(item_code);
            if (iter != AutobotConditionManager::Instance()->shop().buyitem_map().end())
            {
                option_value = iter->second;
                SelectDropdownItemByComboValue(kControl_List_L001, kControl_Text_S025, option_value);
            }
        }
    }

    combo_vector.clear();
    combo_index = AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_BUY_POTION_LEVEL)->combo_index[0];
    AutomaticComboInfoParser::Instance()->GetAutomaticComboInfo(combo_vector, combo_index);
    std::vector<BASE_AutomaticComboInfo*>::iterator begin = combo_vector.begin();
    std::vector<BASE_AutomaticComboInfo*>::iterator end = combo_vector.end();
    for (; begin != end; ++ begin)
    {
        BASE_AutomaticComboInfo* info = (*begin);
        item_code = info->item_code;

        int option_value = AutobotConditionManager::Instance()->shop().GetBuyItemCount(item_code);

        if (option_value > 0)
        {
            SetChecked(kControl_CheckBT_C017, true);
            SetChecked(kControl_CheckBT_C018, true);
            SelectDropdownItemByComboItemCode(kControl_List_L003, kControl_Text_S028, item_code);
            SelectDropdownItemByComboValue(kControl_List_L004, kControl_Text_S030, option_value);
            break;
        }
        else
        {
            SetChecked(kControl_CheckBT_C017, false);
            SetChecked(kControl_CheckBT_C018, false);
        }

    }

    //내구도
    SetChecked(kControl_CheckBT_C019, AutobotConditionManager::Instance()->shop().IsRepair());


    //------------------------------------------------------------------------------ 
    //! 커뮤니티
    TCHAR* channel_level = AutobotConditionManager::Instance()->community().channel_level();
    TCHAR* channel_name = AutobotConditionManager::Instance()->community().channel_name();

    SetChecked(kControl_CheckBT_C023, (_tcslen(channel_level) && _tcslen(channel_name)));
    if (IsChecked(kControl_CheckBT_C023))
    {
        //! 자동 채널 참가
        SelectDropdownItem(kControl_List_L010, kControl_Text_S013, (const char*)channel_level);
        SelectDropdownItem(kControl_List_L011, kControl_Text_S017, (const char*)channel_name);
    }

    AutobotCondition_Community::ShoutingType shouting_type = AutobotConditionManager::Instance()->community().shouting_type();
    int shouting_second = AutobotConditionManager::Instance()->community().shouting_second();
    TCHAR* shouting_message = AutobotConditionManager::Instance()->community().shouting_message();
    bool is_checked = (shouting_type != AutobotCondition_Community::kShouting_None) && 
                        (shouting_type != AutobotCondition_Community::kShouting_Max) && 
                        (shouting_second > 0) && 
                        _tcslen(shouting_message);

    SetChecked(kControl_CheckBT_C025, is_checked);
    if (IsChecked(kControl_CheckBT_C025))
    {
        SelectDropdownItemByComboValue(kControl_List_L008, kControl_Text_S042, static_cast<int>(shouting_type));
        SelectDropdownItemByComboValue(kControl_List_L009, kControl_Text_S044, shouting_second);

        GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000))->SetTextWZ(shouting_message);
    }

    SetChecked(kControl_CheckBT_C020, AutobotConditionManager::Instance()->community().party_type() == AutobotCondition_Community::kParty_Agree);
    SetChecked(kControl_CheckBT_C021, AutobotConditionManager::Instance()->community().party_type() == AutobotCondition_Community::kParty_Ignore);
    SetChecked(kControl_CheckBT_C022, AutobotConditionManager::Instance()->community().is_save_chat_message());

    //콤보 박스 활성화
    OnDropdownEnable();

    return true;
}
//------------------------------------------------------------------------------ 
bool uiAutobotConditionSupport::UpdateCondition()
{
    //------------------------------------------------------------------------------ 
    //! 부활
    if (IsChecked(kControl_CheckBT_C001) == true)
    {
        AutobotConditionManager::Instance()->revival().set_revival_type(AutobotCondition_Revival::kRevival_Resurrection);
    }
    else if (IsChecked(kControl_CheckBT_C007) == true)
    {
        AutobotConditionManager::Instance()->revival().set_revival_type(AutobotCondition_Revival::kRevival_Village);
    }
    else //if (IsChecked(kControl_CheckBT_C010) == true)
    {
        AutobotConditionManager::Instance()->revival().set_revival_type(AutobotCondition_Revival::kRevival_Stay);
    }

    //------------------------------------------------------------------------------ 
    //! 귀환
    BASE_AutomaticComboInfo* info = NULL;
    if (IsChecked(kControl_CheckBT_C004))
    {
        //! 인벤토리 풀
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L002));
        AutobotConditionManager::Instance()->recall().set_inventory_full_percent(static_cast<float>(info->option_value));
    }
    if (IsChecked(kControl_CheckBT_C009))
    {
        //! 물약 갯수 이하
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L005));
        AutobotConditionManager::Instance()->recall().set_heal_item_num(info->option_value);
    }
    if (IsChecked(kControl_CheckBT_C008))
    {
        //! 사료 갯수 이하
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L006));
        AutobotConditionManager::Instance()->recall().set_feed_item_num(info->option_value);
    }
    if (IsChecked(kControl_CheckBT_C011))
    {
        //! 장비 내구도
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L007));
        AutobotConditionManager::Instance()->recall().set_warning_dura(info->option_value);
    }
    //! 마을이동_필드로돌아가기
    AutobotConditionManager::Instance()->recall().set_return_field(IsChecked(kControl_CheckBT_C026));

    //------------------------------------------------------------------------------ 
    //! 자동판매
    if (IsChecked(kControl_CheckBT_C024))
    {
        AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_All);
    }
    else
    {
        if (IsChecked(kControl_CheckBT_C000))
        {
            AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_Normal);
        }
        if (IsChecked(kControl_CheckBT_C002))
        {
            AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_Quest);
        }
        if (IsChecked(kControl_CheckBT_C003))
        {
            AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_Stone);
        }
        if (IsChecked(kControl_CheckBT_C012))
        {
            AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_Weapon);
        }
        if (IsChecked(kControl_CheckBT_C013))
        {
            AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_Armor);
        }
        if (IsChecked(kControl_CheckBT_C014))
        {
            AutobotConditionManager::Instance()->shop().InsertSellItem(AutobotCondition::kItem_AC);
        }
    }

    //------------------------------------------------------------------------------ 
    //! 자동구매
    if (IsChecked(kControl_CheckBT_C015))
    {
        //! 귀환 스크롤(7241)
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L000));
        AutobotConditionManager::Instance()->shop().InsertBuyItem(info->item_code, info->option_value);
    }
    if (IsChecked(kControl_CheckBT_C016))
    {
        //! 일반사료(3301)
        info = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L001));
        AutobotConditionManager::Instance()->shop().InsertBuyItem(info->item_code, info->option_value);
    }
    if (IsChecked(kControl_CheckBT_C017) && IsChecked(kControl_CheckBT_C018))
    {
        //! 물약
        BASE_AutomaticComboInfo* combo_info1 = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L003));
        BASE_AutomaticComboInfo* combo_info2 = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L004));

        AutobotConditionManager::Instance()->shop().InsertBuyItem(combo_info1->item_code, combo_info2->option_value);
    }
    //! 내구도 수리
    AutobotConditionManager::Instance()->shop().SetRepair(IsChecked(kControl_CheckBT_C019));

    //------------------------------------------------------------------------------ 
    //! 커뮤니티
    if (IsChecked(kControl_CheckBT_C023))
    {
        //! 자동 채널 참가
        const char* combo_text1 = GetCurDropdownItemText(kControl_List_L010);
        const char* combo_text2 = GetCurDropdownItemText(kControl_List_L011);
        AutobotConditionManager::Instance()->community().set_channel((TCHAR*)combo_text1, (TCHAR*)combo_text2);
    }
    if (IsChecked(kControl_CheckBT_C025))
    {
        //! 외치기
        BASE_AutomaticComboInfo* combo_info1 = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L008));
        BASE_AutomaticComboInfo* combo_info2 = static_cast<BASE_AutomaticComboInfo*>(GetCurDropdownItemData(kControl_List_L009));

        TCHAR chat_message[MAX_CHAT_LEN] = {NULL, };
        GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000))->GetTextWZ(chat_message, MAX_CHAT_LEN);

        AutobotConditionManager::Instance()->community().set_shouting(static_cast<AutobotCondition_Community::ShoutingType>(combo_info1->option_value), 
                                        combo_info2->option_value,
                                        chat_message);
    }

    if (IsChecked(kControl_CheckBT_C020))
    {
        //! 파티 자동 수락
        AutobotConditionManager::Instance()->community().set_party_type(AutobotCondition_Community::kParty_Agree);
    }
    else if (IsChecked(kControl_CheckBT_C021))
    {
        //! 파티 거부
        AutobotConditionManager::Instance()->community().set_party_type(AutobotCondition_Community::kParty_Ignore);
    }
    else
    {
        AutobotConditionManager::Instance()->community().set_party_type(AutobotCondition_Community::kParty_Stay);
    }

    //! 채팅내용 저장
    AutobotConditionManager::Instance()->community().set_save_chat_message(IsChecked(kControl_CheckBT_C022));

    return true;
}

//------------------------------------------------------------------------------ 
void uiAutobotConditionSupport::OnDropdownEnable()
{
    SetEnable(kControl_Button_B034, IsChecked(kControl_CheckBT_C004));
    SetEnable(kControl_Button_B035, IsChecked(kControl_CheckBT_C008));
    SetEnable(kControl_Button_B036, IsChecked(kControl_CheckBT_C009));
    SetEnable(kControl_Button_B037, IsChecked(kControl_CheckBT_C011));
    SetEnable(kControl_Button_B000, IsChecked(kControl_CheckBT_C015));
    SetEnable(kControl_Button_B003, IsChecked(kControl_CheckBT_C016));
    SetEnable(kControl_Button_B004, IsChecked(kControl_CheckBT_C017));
    SetEnable(kControl_Button_B005, IsChecked(kControl_CheckBT_C018));
    SetEnable(kControl_Button_B007, IsChecked(kControl_CheckBT_C025));
    SetEnable(kControl_Button_B008, IsChecked(kControl_CheckBT_C025));
    SetEnable(kControl_Button_B006, IsChecked(kControl_CheckBT_C023));
    SetEnable(kControl_Button_B010, IsChecked(kControl_CheckBT_C023));

    //! 외치기 텍스트 입력 박스
    SetEnable(kControl_Edit_E000, IsChecked(kControl_CheckBT_C025));

    OnDropdownShow(kControl_Size, kControl_Size);
}
//------------------------------------------------------------------------------ 
eAUTO_HUNTING_TYPE uiAutobotConditionSupport::GetHuntingTypeFromControlIndex(DWORD control_index)
{
    switch (control_index)
    {
    case kControl_CheckBT_C001:    return eAUTO_REBIRTH;                    
    case kControl_CheckBT_C007:    return eAUTO_REBIRTH_VILIAGE;            
    case kControl_CheckBT_C010:    return eAUTO_REBIRTH_WAITING;            
    case kControl_CheckBT_C004:    return eAUTO_RETURNSCROLL_INVEN;         
    case kControl_CheckBT_C009:    return eAUTO_RETURNSCROLL_POTION;        
    case kControl_CheckBT_C008:    return eAUTO_RETURNSCROLL_PET_FEED;      
    case kControl_CheckBT_C011:    return eAUTO_RETURNSCROLL_DURATION;      
    case kControl_CheckBT_C026:    return eAUTO_RETURNSCROLL_RETURNFIELD;      
    case kControl_CheckBT_C024:    return eAUTO_SELL_ALL;                            
    case kControl_CheckBT_C000:    return eAUTO_SELL_NORMAL;                         
    case kControl_CheckBT_C002:    return eAUTO_SELL_QUEST;                          
    case kControl_CheckBT_C003:    return eAUTO_SELL_STONE;                 
    case kControl_CheckBT_C012:    return eAUTO_SELL_WEAPON;                
    case kControl_CheckBT_C013:    return eAUTO_SELL_ARMOR;                 
    case kControl_CheckBT_C014:    return eAUTO_SELL_AC;                    
    case kControl_CheckBT_C015:    return eAUTO_BUY_RETURNSCROLL;           
    case kControl_CheckBT_C016:    return eAUTO_BUY_PET_FEED;               
    case kControl_CheckBT_C017:    return eAUTO_BUY_POTION_LEVEL;           
    case kControl_CheckBT_C018:    return eAUTO_BUY_POTION_COUNT;           
    case kControl_CheckBT_C019:    return eAUTO_BUY_DURATION;               
    case kControl_CheckBT_C023:    return eAUTO_COMMUNITY_CHANNEL;          
    case kControl_CheckBT_C025:    return eAUTO_COMMUNITY_SHOUT;            
    case kControl_CheckBT_C020:    return eAUTO_COMMUNITY_PARTY_JOIN;       
    case kControl_CheckBT_C021:    return eAUTO_COMMUNITY_PARTY_BLOCK;      
    case kControl_CheckBT_C022:    return eAUTO_COMMUNITY_SAVE_CHAT;        
    }

    return eAUTO_TYPE_INVALID;
}
#endif //_YMS_AUTOBOT_GAME