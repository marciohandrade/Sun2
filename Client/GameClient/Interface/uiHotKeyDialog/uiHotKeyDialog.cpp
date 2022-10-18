#include "SunClientPrecompiledHeader.h"

#include "uiHotKeyDialog.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "HotKeyManager.h"
#include "input/HotKeyInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "Interface/uiMenuMan/uiMenuMan.h"
#include "MainDialog.h"
#include "MainDialog2.h"
#include "MainMenuOptionDlg.h"
#endif

#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
#include "RadarFullDialog.h"
#include "uiRadarMan/uiRadarMan.h"

#include "uiInventoryMan/uiInventoryMan.h"
#include "InventoryDialog.h"
#endif

uiHotKeyDialog::uiHotKeyDialog(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{

}

uiHotKeyDialog::~uiHotKeyDialog()
{

}

void uiHotKeyDialog::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031);
    RegisterControlPosition(StrToWzID("B032"), kControl_Button_B032);
    RegisterControlPosition(StrToWzID("B033"), kControl_Button_B033);
    RegisterControlPosition(StrToWzID("B034"), kControl_Button_B034);
    RegisterControlPosition(StrToWzID("B035"), kControl_Button_B035);
    RegisterControlPosition(StrToWzID("B036"), kControl_Button_B036);
    RegisterControlPosition(StrToWzID("B037"), kControl_Button_B037);
    RegisterControlPosition(StrToWzID("B038"), kControl_Button_B038);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("S021"), kControl_Text_S021);
    RegisterControlPosition(StrToWzID("V002"), kControl_VScroll_V002);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckButton_C000);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S016);

    // ui_bind_info
    HotKeyInfoParser::Instance()->GetIndexList(function_index_vector());
}

void uiHotKeyDialog::Release()
{

}

void uiHotKeyDialog::OnUpdateSolarDialog()
{
    VScrollDialog::Update();
}

void uiHotKeyDialog::OnRenderSolarDialog()
{
    if (HotKeyManager::Instance()->is_user_setting_mode() == true)
    {
        if ((input_key_info().list_index >= current_ui_index()) && 
            (input_key_info().list_index < current_ui_index() + kNumOfDisplay))
        {
            // 출력
            RenderInputHighLight();
        }
    }
}

void uiHotKeyDialog::RenderInputHighLight()
{
    POSTYPE control_position = kMainHotKeyControlBegin;
    if (input_key_info().bind_pair_index == KEY_BIND_PAIR::kSubHotKey)
    {
        control_position = kSubHotKeyControlBegin;
    }
    control_position += (input_key_info().list_index - current_ui_index());

    CCtrlButtonWZ* buttton_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(control_position));
    if (buttton_control == NULL)
    {
        return;
    }

    SI_TFORMAT* image_format = buttton_control->GetCtrlImageFormat();
    if (image_format == NULL)
    {
        return;
    }

    SI_TSUBINFO& sub_info = image_format->m_sImageState[E_CHECK];
    if ((sub_info.m_blIs == false) || 
        (sub_info.m_pVoid == NULL) || 
        (sub_info.m_pVoid == INVALID_HANDLE_VALUE))
    {
        return;
    }

    const int kCornerSize = 10;
    RECT control_rect = buttton_control->GetSizeRect();
    // 상단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.left + kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.top) + 0.5f, 
        static_cast<float>(control_rect.right - control_rect.left - (kCornerSize * 2)), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopX + static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopY, 
        sub_info.m_sFRect.fWidth - static_cast<float>(kCornerSize * 2), 
        static_cast<float>(kCornerSize));
    // 좌측중단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.left) + 0.5f, 
        static_cast<float>(control_rect.top + kCornerSize) + 0.5f, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(control_rect.bottom - control_rect.top - (kCornerSize * 2)), 
        sub_info.m_sFRect.fTopX, 
        sub_info.m_sFRect.fTopY + static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fHeight - static_cast<float>(kCornerSize * 2));

    // 중앙중단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.left + kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.top + kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.right - control_rect.left - (kCornerSize * 2)), 
        static_cast<float>(control_rect.bottom - control_rect.top - (kCornerSize * 2)), 
        sub_info.m_sFRect.fTopX + static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopY + static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fWidth - static_cast<float>(kCornerSize * 2), 
        sub_info.m_sFRect.fHeight - static_cast<float>(kCornerSize * 2));

    // 우측중단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.right - kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.top + kCornerSize) + 0.5f, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(control_rect.bottom - control_rect.top - (kCornerSize * 2)), 
        sub_info.m_sFRect.fTopX + sub_info.m_sFRect.fWidth - static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopY + static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fHeight - static_cast<float>(kCornerSize * 2));

    // 하단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.left + kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.bottom - kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.right - control_rect.left - (kCornerSize * 2)), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopX + static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopY + sub_info.m_sFRect.fHeight - static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fWidth - static_cast<float>(kCornerSize * 2), 
        static_cast<float>(kCornerSize));

    // 좌측상단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.left) + 0.5f, 
        static_cast<float>(control_rect.top) + 0.5f, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopX, 
        sub_info.m_sFRect.fTopY, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize));
    // 우측상단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.right - kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.top) + 0.5f, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopX + sub_info.m_sFRect.fWidth - static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopY, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize));
    // 좌측하단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.left) + 0.5f, 
        static_cast<float>(control_rect.bottom - kCornerSize) + 0.5f, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopX, 
        sub_info.m_sFRect.fTopY + sub_info.m_sFRect.fHeight - static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize));
    // 우측하단
    g_pSunRenderer->RenderTexture(HANDLE(sub_info.m_pVoid), 
        static_cast<float>(control_rect.right - kCornerSize) + 0.5f, 
        static_cast<float>(control_rect.bottom - kCornerSize) + 0.5f, 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopX + sub_info.m_sFRect.fWidth - static_cast<float>(kCornerSize), 
        sub_info.m_sFRect.fTopY + sub_info.m_sFRect.fHeight - static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize), 
        static_cast<float>(kCornerSize));
}

void uiHotKeyDialog::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        // 임시 세팅 정보에 현재 세팅 정보를 복사
        HotKeyManager::Instance()->temp_key_bind_info() = 
            HotKeyManager::Instance()->current_using_key_bind_info();

        CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckButton_C000));
        if (check_button != NULL)
        {
            check_button->SetCheckState(HotKeyManager::Instance()->is_character_setting());
        }

        set_current_ui_index(0);
        ShowNoticeString(" ");
        RefreshBindInfo();
        ResizeingVScrollControl(function_index_vector().size() - kNumOfDisplay, current_ui_index());

        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_HotKeyDialog;
        msg.wParam = IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_HotKeyDialog);
        EnableControls(true);
        HotKeyManager::Instance()->EndUserSettingMode(0);
        input_key_info().Clear();
    }
}

void uiHotKeyDialog::MessageProc(SI_MESSAGE* message)
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
    default:
        SolarDialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}

bool uiHotKeyDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B999:
    case kControl_Button_B005:
        {
            // 종료/취소 버튼
            ShowInterfaceWithSound(FALSE);
        }
        break;
    case kControl_Button_B001:
        {
            // 초기화 버튼
            InitializeKeyBindInfo();
#ifdef _NA_000000_20130114_RENEWER_UI
			uiMenuMan* MainMenuMan = static_cast<uiMenuMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_MENU));

			if(MainMenuMan)
			{
				MainMenuMan->KeyModifyChange();
			}
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
            RadarFullDialog*  pfullRadar = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
            if (pfullRadar)
            {
                pfullRadar->CheckModifyOption();
            }

            InventoryDialog*  pInventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
            if (pInventory)
            {
                pInventory->CheckModifyOption();
            }
#endif
#endif
        }
        break;
    case kControl_Button_B003:
        {
            // 해제 버튼
            HotKeyManager::Instance()->EndUserSettingMode(0);
            if (input_key_info().function_index != kNone_Function_Index)
            {
                ReleaseKeyBindInfo(input_key_info().function_index, input_key_info().bind_pair_index);
                input_key_info().Clear();
            }
            input_key_info().Clear();
            RefreshBindInfo();
        }
        break;
    case kControl_Button_B004:
        {
            // 확인(적용) 버튼
            ApplayHotKey();
#ifdef _NA_000000_20130114_RENEWER_UI
			uiMenuMan* MainMenuMan = static_cast<uiMenuMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_MENU));

			if(MainMenuMan)
			{
				MainMenuMan->KeyModifyChange();
			}
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
            RadarFullDialog*  pfullRadar = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
            if (pfullRadar)
            {
                pfullRadar->CheckModifyOption();
            }

            InventoryDialog*  pInventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
            if (pInventory)
            {
                pInventory->CheckModifyOption();
            }
#endif
#endif
        }
        break;
        
        // 메인키 버튼
    case kControl_Button_B006:
    case kControl_Button_B007:
    case kControl_Button_B011:
    case kControl_Button_B009:
    case kControl_Button_B020:
    case kControl_Button_B017:
    case kControl_Button_B015:
    case kControl_Button_B013:
    case kControl_Button_B035:
    case kControl_Button_B033:
    case kControl_Button_B031:
    case kControl_Button_B029:
    case kControl_Button_B028:
    case kControl_Button_B025:
    case kControl_Button_B023:
    case kControl_Button_B021:
    case kControl_Button_B037:
        {
            // 입력 받을 인덱스 저장
            int list_index = current_ui_index() + (control_position - kControl_Button_B006);
            if (list_index == input_key_info().list_index)
            {
                HotKeyManager::Instance()->EndUserSettingMode(0);
                input_key_info().Clear();
            }
            else
            {
                DWORD function_index = function_index_vector()[list_index];
                input_key_info().function_index = function_index;
                input_key_info().list_index = list_index;
                input_key_info().bind_pair_index = KEY_BIND_PAIR::kMainHotKey;

                // ... 컨트롤 비활성화
                EnableControls(false);
                HotKeyManager::Instance()->StartUserSettingMode();
            }
        }
        break;

        // 서브키 버튼
    case kControl_Button_BT00:
    case kControl_Button_B008:
    case kControl_Button_B012:
    case kControl_Button_B010:
    case kControl_Button_B019:
    case kControl_Button_B018:
    case kControl_Button_B016:
    case kControl_Button_B014:
    case kControl_Button_B036:
    case kControl_Button_B034:
    case kControl_Button_B032:
    case kControl_Button_B030:
    case kControl_Button_B027:
    case kControl_Button_B026:
    case kControl_Button_B024:
    case kControl_Button_B022:
    case kControl_Button_B038:
        {
            // 입력 받을 인덱스 저장
            int list_index = current_ui_index() + (control_position - kControl_Button_BT00);
            if (list_index == input_key_info().list_index)
            {
                HotKeyManager::Instance()->EndUserSettingMode(0);
                input_key_info().Clear();
            }
            else
            {
                DWORD function_index = function_index_vector()[list_index];
                input_key_info().function_index = function_index;
                input_key_info().list_index = list_index;
                input_key_info().bind_pair_index = KEY_BIND_PAIR::kSubHotKey;

                // ... 컨트롤 비활성화
                EnableControls(false);
                HotKeyManager::Instance()->StartUserSettingMode();
            }
        }
        break;

    default:
        {
        }
    }

    return result;
}

bool uiHotKeyDialog::OnVScrollDown(SI_MESSAGE* msg)
{
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        set_current_ui_index(vscroll_control()->GetScrollPosWZ());
        RefreshBindInfo();

        return true;
    }
    return false;
}

bool uiHotKeyDialog::OnVScrollUp(SI_MESSAGE* msg)
{
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        set_current_ui_index(vscroll_control()->GetScrollPosWZ());
        RefreshBindInfo();

        return true;
    }
    return false;
}

bool uiHotKeyDialog::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        set_current_ui_index(vscroll_control()->GetScrollPosWZ());
        RefreshBindInfo();

        return true;
    }
    return false;
}

void uiHotKeyDialog::NetworkProc(MSG_BASE* recv_packet)
{

}

void uiHotKeyDialog::RecvUserKeySetting(WORD hot_key_code)
{
    // 입력 받은 단축키 저장
    input_key_info().hot_key_code = hot_key_code;

    KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();

    HOT_KEY_CODE_ASSOCIATION_MAP::iterator found_itr = 
        temp_info.hot_key_code_association_map().find(hot_key_code);
    if (found_itr != temp_info.hot_key_code_association_map().end())
    {
        // 키입력이 중복됨(이미 사용하고 있는 단축키)
        // 중복된 단축키 해제
        KEY_BIND_PAIR& key_bind_pair = temp_info.key_bind_pair_map().find(found_itr->second)->second;
        if (key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey) == hot_key_code)
        {
            ReleaseKeyBindInfo(found_itr->second, KEY_BIND_PAIR::kMainHotKey);
        }
        else if (key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey) == hot_key_code)
        {
            ReleaseKeyBindInfo(found_itr->second, KEY_BIND_PAIR::kSubHotKey);
        }
    }

    EnableControls(true);
    RegisterKeyBindInfo(input_key_info().function_index, 
        input_key_info().bind_pair_index, 
        input_key_info().hot_key_code);
    RefreshBindInfo();
    input_key_info().Clear();
}

bool uiHotKeyDialog::GetScrollArea(RECT& area)
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        CControlWZ* control_ptr = dialog_ptr->GetClientWZ();
        if (control_ptr != NULL)
        {
            area = control_ptr->GetSizeRect();
            return true;
        }
    }
    
    return false;
}

CCtrlVScrollWZ* uiHotKeyDialog::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V002));
}

void uiHotKeyDialog::InitializeKeyBindInfo()
{
    // 초기화
    HotKeyManager::Instance()->temp_key_bind_info() = 
        HotKeyManager::Instance()->defalut_key_bind_info();
    RefreshBindInfo();
}

void uiHotKeyDialog::ApplayHotKey()
{
    HotKeyManager::Instance()->current_using_key_bind_info() = HotKeyManager::Instance()->temp_key_bind_info();

    // 서버 저장
    CCtrlButtonCheckWZ* check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckButton_C000));
    bool is_check = false;
    if (check_button != NULL)
    {
        is_check = check_button->GetCheckState();
    }
    HotKeyManager::Instance()->SaveHotKeyInfoToServer(is_check);

    ShowInterfaceWithSound(FALSE);
}

void uiHotKeyDialog::RefreshBindInfo()
{
    CCtrlStaticWZ* caption_control = NULL;
    CCtrlButtonWZ* key_button_control = NULL;
    STLX_STRING hot_key_string;
    DWORD string_code = 0;

    KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();
    for (WORD control_index = 0; control_index < kNumOfDisplay; ++control_index)
    {
        // 각 기능 정보 얻음
        HOT_KEY_FUNCTION_INDEX function_index = 
            static_cast<HOT_KEY_FUNCTION_INDEX>(function_index_vector()[control_index + current_ui_index()]);
        KEY_BIND_PAIR_MAP::iterator found_itr = temp_info.key_bind_pair_map().find(function_index);
        
        if (found_itr == temp_info.key_bind_pair_map().end())
        {
            //! 컨트롤 초기화 해야함.
            continue;
        }

        // 기능명 출력
        caption_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kCaptionControlBegin + control_index));
        if (caption_control != NULL)
        {
            string_code = HotKeyInfoParser::Instance()->GetDescriptionCodeByIndex(function_index);
            caption_control->SetTextID(string_code);
        }

        // 키조합 출력
        // 메인
        key_button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kMainHotKeyControlBegin + control_index));
        if (key_button_control != NULL)
        {
            KEY_BIND_PAIR& key_bind_pair = found_itr->second;

            hot_key_string.clear();
            
            HOT_KEY_CODE hot_ket_code(key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey));
            if (hot_ket_code.key_code_ != 0)
            {
                if (hot_ket_code.ctrl_key_ == 1)
                {
                    hot_key_string += g_InterfaceManager.GetInterfaceString(8218);
                    hot_key_string += " + ";
                }

                if (hot_ket_code.alt_key_ == 1)
                {
                    hot_key_string += g_InterfaceManager.GetInterfaceString(8219);
                    hot_key_string += " + ";
                }

                if (hot_ket_code.shift_key_ == 1)
                {
                    hot_key_string += g_InterfaceManager.GetInterfaceString(8217);
                    hot_key_string += " + ";
                }

                string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);
                hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
            }
            key_button_control->SetTextWZ(hot_key_string.c_str());
        }

        // 서브
        key_button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kSubHotKeyControlBegin + control_index));
        if (key_button_control != NULL)
        {
            KEY_BIND_PAIR& key_bind_pair = found_itr->second;

            hot_key_string.clear();

            HOT_KEY_CODE hot_ket_code(key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey));
            if (hot_ket_code.key_code_ != 0)
            {
                if (hot_ket_code.ctrl_key_ == 1)
                {
                    hot_key_string += g_InterfaceManager.GetInterfaceString(8218);
                    hot_key_string += " + ";
                }

                if (hot_ket_code.alt_key_ == 1)
                {
                    hot_key_string += g_InterfaceManager.GetInterfaceString(8219);
                    hot_key_string += " + ";
                }

                if (hot_ket_code.shift_key_ == 1)
                {
                    hot_key_string += g_InterfaceManager.GetInterfaceString(8217);
                    hot_key_string += " + ";
                }

                string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);
                hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
            }
            key_button_control->SetTextWZ(hot_key_string.c_str());
        }
    }
}

void uiHotKeyDialog::ShowNoticeString(const TCHAR* notice_string)
{
    CCtrlStaticWZ* notice_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S002));
    if (notice_text != NULL)
    {
        notice_text->SetTextWZ(notice_string);
    }
}

void uiHotKeyDialog::EnableControls(bool is_enable)
{
    //EnableControl(, is_enable);
}

void uiHotKeyDialog::EnableControl(UIControlPos control_position, bool is_enable)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);
    if (control_ptr != NULL)
    {
        control_ptr->EnableWindowWZ(is_enable);
    }
}

void uiHotKeyDialog::RegisterKeyBindInfo(DWORD index, BYTE bind_pair_index, WORD hot_key_code)
{
    HOT_KEY_FUNCTION_INDEX function_index = static_cast<HOT_KEY_FUNCTION_INDEX>(index);
    KEY_BIND_PAIR key_bind_pair;
    KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();
    KEY_BIND_PAIR_MAP::iterator found_itr = 
        temp_info.key_bind_pair_map().find(function_index);
    if (found_itr != temp_info.key_bind_pair_map().end())
    {
        key_bind_pair = found_itr->second;
    }

    key_bind_pair.SetHotKeyCode(static_cast<KEY_BIND_PAIR::KeyPairIndex>(bind_pair_index), 
        HOT_KEY_CODE(hot_key_code));

    temp_info.SetKeyBindInfo(function_index, key_bind_pair);

    if (hot_key_code != 0)
    {
        // 등록 메시지 출력
        ShowNoticeString(g_InterfaceManager.GetInterfaceString(8360));
    }
}

void uiHotKeyDialog::ReleaseKeyBindInfo(DWORD index, BYTE bind_pair_index)
{
    RegisterKeyBindInfo(index, bind_pair_index, 0);

    // 해제 메시지 출력
    TCHAR release_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR function_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR pair_index_string[INTERFACE_STRING_LENGTH] = {'\0', };

    HOT_KEY_FUNCTION_INDEX function_index = static_cast<HOT_KEY_FUNCTION_INDEX>(index);
    g_InterfaceManager.GetInterfaceString(
        HotKeyInfoParser::Instance()->GetDescriptionCodeByIndex(function_index), 
        function_string);
    g_InterfaceManager.GetInterfaceString(2311 + bind_pair_index, pair_index_string);

    g_InterfaceManager.GetInterfaceStringFormat(release_string, INTERFACE_STRING_LENGTH - 1, 
        8361, function_string, pair_index_string);

    ShowNoticeString(release_string);
}

