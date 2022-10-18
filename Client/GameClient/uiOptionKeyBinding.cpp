#include "SunClientPrecompiledHeader.h"
#include "uiOptionKeyBinding.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

#include "HotKeyManager.h"
#include "Input/HotKeyInfoParser.h"
#include "uiMenuMan/uiMenuMan.h"
#include "uiOptionMan.h"

#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
#include "uiRadarMan/uiRadarMan.h"
#include "RadarFullDialog.h"

#include "uiInventoryMan/uiInventoryMan.h"
#include "InventoryDialog.h"
#endif

uiOptionKeyBinding::uiOptionKeyBinding(InterfaceManager* ui_man )
: uiBase(ui_man)
{
    m_Initialized = FALSE;
}

uiOptionKeyBinding::~uiOptionKeyBinding()
{
    for( size_t i=0;i!=m_ControlArray.size();++i)
    {
        delete m_ControlArray[i];
		m_ControlArray[i] = NULL;
    }
    m_ControlArray.clear();
}

void uiOptionKeyBinding::OnShowWindow( BOOL val )
{
    if( val == TRUE && m_Initialized == FALSE )
    {
        m_Initialized = TRUE;
        InitializeControl();

    }
    if( val == TRUE )
    {
        HotKeyManager::Instance()->temp_key_bind_info() = HotKeyManager::Instance()->current_using_key_bind_info();
        KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();
        int count = (int)temp_info.key_bind_pair_map().size();
        m_pScroll->SetScrollRangeWZ( 0, max(count-10,0) );

        m_ResultArray.clear();
        UpdateControlState();
    }


    if( val == TRUE )
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_Option_KeyBinding;
        msg.wParam=InterfaceManager::DIALOG_OPTION_KEYBINDING;
        msg.DoSomething=GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_KeyBinding);
    }
}

void uiOptionKeyBinding::OnRenderSolarDialog()
{

}

void uiOptionKeyBinding::OnUpdateSolarDialog()
{
    VScrollDialog::Update();
}

void uiOptionKeyBinding::MessageProc( SI_MESSAGE * pMessage )
{
    if (IsLock())
        return;


    if( OnClickHotKeyButton(pMessage) == TRUE )
        return;

    if( OnClickButton(pMessage) == TRUE )
        return;

    SolarDialog::MessageProc(pMessage);
    VScrollDialog::MessageProc(pMessage);

}

void uiOptionKeyBinding::InitializeControl()
{
    //
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S000"), StrToWzID("S002"), StrToWzID("B038"), StrToWzID("B039") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S021"), StrToWzID("S030"), StrToWzID("B062"), StrToWzID("B063") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S022"), StrToWzID("S031"), StrToWzID("B064"), StrToWzID("B065") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S023"), StrToWzID("S032"), StrToWzID("B066"), StrToWzID("B067") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S024"), StrToWzID("S033"), StrToWzID("B068"), StrToWzID("B069") ) );
                                                          
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S025"), StrToWzID("S034"), StrToWzID("B070"), StrToWzID("B071") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S026"), StrToWzID("S035"), StrToWzID("B072"), StrToWzID("B073") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S027"), StrToWzID("S036"), StrToWzID("B074"), StrToWzID("B075") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S028"), StrToWzID("S037"), StrToWzID("B076"), StrToWzID("B077") ) );
    m_ControlArray.push_back( new sKeyBindingControlItem( StrToWzID("S029"), StrToWzID("S038"), StrToWzID("B078"), StrToWzID("B079") ) );


    //
    m_pScroll = (CCtrlVScrollWZ*)GetControlWZ(StrToWzID("VS02"));
    m_pScroll->SetScrollPosWZ(0.0f);

    //
    m_CheckCharacterSetting = (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C000") );

    //
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C001") ) );
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C002") ) );
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C003") ) );
    m_CheckButtonArray.push_back( (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C004") ) );

    //HotKeyManager::Instance()->temp_key_bind_info() = HotKeyManager::Instance()->current_using_key_bind_info();
    //KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();
    HotKeyInfoParser::Instance()->GetIndexList( m_IndexArray );
}

BOOL uiOptionKeyBinding::OnClickButton( SI_MESSAGE* pMessage )
{
    if( pMessage->eResultMsg != RT_MSG_LBUTTONCLICK )
        return FALSE;

    for( size_t i=0;i!=m_CheckButtonArray.size();++i)
    {
        if( m_CheckButtonArray[i]->GetCtrlID() == pMessage->dwCtrlID )
        {
            if( i != 3 )
            {
                uiOptionMan::ShowOptionDialog(i);
            }

            return TRUE;
        }
    }

    if( pMessage->dwCtrlID == StrToWzID("B043") )
    {
        // 초기화 버튼
        InitializeKeyBindInfo();
        UpdateControlState();
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
        return TRUE;
    }
    else if( pMessage->dwCtrlID == StrToWzID("B080") )
    {
        // 해제 버튼
        HotKeyManager::Instance()->EndUserSettingMode(0);
        if( m_InputKeyInfo.function_index != kNone_Function_Index )
        {
            ReleaseKeyBindInfo(m_InputKeyInfo.function_index, m_InputKeyInfo.bind_pair_index);
        }
        m_InputKeyInfo.Clear();
        UpdateControlState();

        return TRUE;
    }
    else if( pMessage->dwCtrlID == StrToWzID("B042") )
    {
        // 확인(적용) 버튼
        ApplyHotKey();
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

        return TRUE;
    }
    else if( pMessage->dwCtrlID == StrToWzID("B005") )
    {
        // 확인(적용) 버튼
        ApplyHotKey();
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
        ShowInterfaceWithSound(FALSE);

        return TRUE;
    }
    else if( pMessage->dwCtrlID == StrToWzID("B006") )
    {
        // 종료/취소 버튼
        ShowInterfaceWithSound(FALSE);
        return TRUE;
    }
    else if( pMessage->dwCtrlID == StrToWzID("B999") )
    {
        // 종료/취소 버튼
        ShowInterfaceWithSound(FALSE);
        return TRUE;
    }


    return FALSE;
}

BOOL uiOptionKeyBinding::OnClickHotKeyButton( SI_MESSAGE* pMessage )
{
    if( pMessage->eResultMsg != RT_MSG_LBUTTONCLICK )
        return FALSE;

    int ScrollPosition = m_pScroll->GetScrollPosWZ();

    for( size_t i=0;i!=m_ControlArray.size();++i)
    {
        int ListIndex = ScrollPosition + i;

        //if( ListIndex == m_InputKeyInfo.list_index )
        //{
        //    HotKeyManager::Instance()->EndUserSettingMode(0);
        //    m_InputKeyInfo.Clear();
        //    return TRUE;
        //}

        if( pMessage->dwCtrlID == m_ControlArray[i]->m_Left )
        {
            HOT_KEY_FUNCTION_INDEX function_index = (HOT_KEY_FUNCTION_INDEX)m_IndexArray[ListIndex];

            m_InputKeyInfo.function_index = function_index;
            m_InputKeyInfo.list_index = ListIndex;
            m_InputKeyInfo.bind_pair_index = KEY_BIND_PAIR::kMainHotKey;

            HotKeyManager::Instance()->StartUserSettingMode();
            UpdateControlState();
            return TRUE;
        }
        else if( pMessage->dwCtrlID == m_ControlArray[i]->m_Right )
        {
            HOT_KEY_FUNCTION_INDEX function_index = (HOT_KEY_FUNCTION_INDEX)m_IndexArray[ListIndex];

            m_InputKeyInfo.function_index = function_index;
            m_InputKeyInfo.list_index = ListIndex;
            m_InputKeyInfo.bind_pair_index = KEY_BIND_PAIR::kSubHotKey;

            HotKeyManager::Instance()->StartUserSettingMode();
            UpdateControlState();

            return TRUE;
        }
    }

    return FALSE;
}

void uiOptionKeyBinding::RecvUserKeySetting(WORD hot_key_code)
{
    // 입력 받은 단축키 저장
    m_InputKeyInfo.hot_key_code = hot_key_code;

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

    //EnableControls(true);

    RegisterKeyBindInfo(m_InputKeyInfo.function_index, m_InputKeyInfo.bind_pair_index, m_InputKeyInfo.hot_key_code);
    m_InputKeyInfo.Clear();

    UpdateControlState();
}

void uiOptionKeyBinding::InitializeKeyBindInfo()
{
    // 초기화
    HotKeyManager::Instance()->temp_key_bind_info() = HotKeyManager::Instance()->defalut_key_bind_info();

    m_ResultArray.clear();

    UpdateControlState();
}

void uiOptionKeyBinding::ApplyHotKey()
{
    HotKeyManager::Instance()->current_using_key_bind_info() = HotKeyManager::Instance()->temp_key_bind_info();

    // 서버 저장
    bool is_check = m_CheckCharacterSetting->GetCheckState();
    HotKeyManager::Instance()->SaveHotKeyInfoToServer(is_check);

}

void uiOptionKeyBinding::RegisterKeyBindInfo(DWORD index, BYTE bind_pair_index, WORD hot_key_code)
{
    HOT_KEY_FUNCTION_INDEX function_index = static_cast<HOT_KEY_FUNCTION_INDEX>(index);
    KEY_BIND_PAIR key_bind_pair;
    KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();
    KEY_BIND_PAIR_MAP::iterator found_itr = temp_info.key_bind_pair_map().find(function_index);
    if (found_itr != temp_info.key_bind_pair_map().end())
    {
        key_bind_pair = found_itr->second;
    }

    key_bind_pair.SetHotKeyCode(static_cast<KEY_BIND_PAIR::KeyPairIndex>(bind_pair_index), HOT_KEY_CODE(hot_key_code));

    temp_info.SetKeyBindInfo(function_index, key_bind_pair);

    if (hot_key_code != 0)
    {
        if( bind_pair_index == KEY_BIND_PAIR::kMainHotKey )
            AddResultInfo( sInputKeyResultInfo::RESULT_STATE_SETTED_MAINKEY, function_index );
        else if( bind_pair_index == KEY_BIND_PAIR::kSubHotKey )
            AddResultInfo( sInputKeyResultInfo::RESULT_STATE_SETTED_SUBKEY, function_index );
    }
}

void uiOptionKeyBinding::ReleaseKeyBindInfo(DWORD index, BYTE bind_pair_index)
{
    RegisterKeyBindInfo(index, bind_pair_index, 0);

    HOT_KEY_FUNCTION_INDEX function_index = static_cast<HOT_KEY_FUNCTION_INDEX>(index);

    if( bind_pair_index == KEY_BIND_PAIR::kMainHotKey )
        AddResultInfo( sInputKeyResultInfo::RESULT_STATE_RELEASED_MAINKEY, function_index );
    else if( bind_pair_index == KEY_BIND_PAIR::kSubHotKey )
        AddResultInfo( sInputKeyResultInfo::RESULT_STATE_RELEASED_SUBKEY, function_index );
}

void uiOptionKeyBinding::AddResultInfo( sInputKeyResultInfo::eResultState ResultState, DWORD FunctionIndex )
{
    for( size_t i=0;i!=m_ResultArray.size();++i)
    {
        if( m_ResultArray[i].m_FunctionIndex == FunctionIndex )
        {
            m_ResultArray[i].m_ResultState = ResultState;
            return;
        }
    }

    sInputKeyResultInfo Result;
    Result.m_FunctionIndex = FunctionIndex;
    Result.m_ResultState = ResultState;
    m_ResultArray.push_back( Result );
}

void uiOptionKeyBinding::UpdateControlState()
{
    for( size_t i=0;i!=m_CheckButtonArray.size();++i)
    {
        m_CheckButtonArray[i]->SetCheckState( i == 3 );
    }

    m_CheckCharacterSetting->SetCheckState( HotKeyManager::Instance()->is_character_setting() );


    KEY_BIND_INFO& temp_info = HotKeyManager::Instance()->temp_key_bind_info();

    int ScrollPosition = m_pScroll->GetScrollPosWZ();
    for( size_t i=0;i!=m_IndexArray.size();++i)
    {
        int index = i - ScrollPosition;
        if( index < 0 )
            continue;

        if( index >= 10 )
            continue;

        CCtrlStaticWZ* pTitleControl = (CCtrlStaticWZ*)GetControlWZ(m_ControlArray[index]->m_Title);
        CCtrlStaticWZ* pTextControl = (CCtrlStaticWZ*)GetControlWZ(m_ControlArray[index]->m_Text);
        CCtrlButtonWZ* pLeftButtonControl = (CCtrlButtonWZ*)GetControlWZ(m_ControlArray[index]->m_Left);
        CCtrlButtonWZ* pRightBUttonControl = (CCtrlButtonWZ*)GetControlWZ(m_ControlArray[index]->m_Right);

        HOT_KEY_FUNCTION_INDEX function_index = (HOT_KEY_FUNCTION_INDEX)m_IndexArray[i];

        DWORD TextID = HotKeyInfoParser::Instance()->GetDescriptionCodeByIndex( function_index );


        if( m_InputKeyInfo.function_index == function_index )
        {
            // 입력중
            char text[1024];
            LPCTSTR name = g_InterfaceManager.GetInterfaceString(TextID);

            if( m_InputKeyInfo.bind_pair_index == KEY_BIND_PAIR::kMainHotKey )
                g_InterfaceManager.GetInterfaceStringFormat( text, 1024, 2355, name );  // %s 주 단축키를 입력하세요.
            else
                g_InterfaceManager.GetInterfaceStringFormat( text, 1024, 2356, name );  // %s 보조 단축키를 입력하세요.

            pTextControl->SetTextWZA(text);

            pTitleControl->SetTextWZA("");
        }
        else
        {
            // 기본
            pTextControl->SetTextWZA("");
            pTitleControl->SetTextID(TextID);


            //변경사항이 있는경우
            for( size_t i=0;i!=m_ResultArray.size();++i)
            {
                if( m_ResultArray[i].m_FunctionIndex == function_index )
                {
                    pTitleControl->SetTextWZA("");

                    char text[1024];
                    LPCTSTR name = g_InterfaceManager.GetInterfaceString(TextID);

                    if( m_ResultArray[i].IsSettedMainKey() )
                        g_InterfaceManager.GetInterfaceStringFormat( text, 1024, 2357, name );  // %s 주 단축키가 설정되었습니다.
                    else if( m_ResultArray[i].IsSettedSubKey() )
                        g_InterfaceManager.GetInterfaceStringFormat( text, 1024, 2358, name );  // %s 보조 단축키가 설정되었습니다..
                    else if( m_ResultArray[i].IsReleasedMainKey() )
                        g_InterfaceManager.GetInterfaceStringFormat( text, 1024, 2359, name );  // %s 주 단축키가 해제되었습니다..
                    else if( m_ResultArray[i].IsReleasedSubKey() )
                        g_InterfaceManager.GetInterfaceStringFormat( text, 1024, 2360, name );  // %s 보조 단축키가 해제되었습니다.
                    pTextControl->SetTextWZA(text);
                }
            }
        }

        // Find KeyValue
        KEY_BIND_PAIR_MAP::iterator it = temp_info.key_bind_pair_map().find(function_index);
        if( it == temp_info.key_bind_pair_map().end() ) 
        {
            pLeftButtonControl->SetTextWZ("");
            pRightBUttonControl->SetTextWZ("");
            continue;
        }

        KEY_BIND_PAIR& key_bind_pair = it->second;


        // parse left button text
        {
            STLX_STRING hot_key_string;
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

                DWORD string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);
                hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
            }

            pLeftButtonControl->SetTextWZ(hot_key_string.c_str());
        }

        {
            STLX_STRING hot_key_string;
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

                DWORD string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);
                hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
            }

            pRightBUttonControl->SetTextWZ(hot_key_string.c_str());
        }


    }
}

bool uiOptionKeyBinding::GetScrollArea(RECT& area)
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


bool uiOptionKeyBinding::OnVScrollDown(SI_MESSAGE* msg)
{
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        UpdateControlState();

        return true;
    }
    return false;
}

bool uiOptionKeyBinding::OnVScrollUp(SI_MESSAGE* msg)
{
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        UpdateControlState();

        return true;
    }
    return false;
}

bool uiOptionKeyBinding::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        UpdateControlState();

        return true;
    }
    return false;
}