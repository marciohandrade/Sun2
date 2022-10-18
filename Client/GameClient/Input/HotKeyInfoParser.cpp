#include "SunClientPrecompiledHeader.h"
#include "HotKeyInfoParser.h"
#include "Input.h"
#include "InterfaceManager.h"

HotKeyInfoParser::HotKeyInfoParser()
{

}

HotKeyInfoParser::~HotKeyInfoParser()
{

}

void HotKeyInfoParser::Release()
{
    Unload();
}

void HotKeyInfoParser::Unload()
{
    // 컨테이너 해제/클리어
    defalut_Info().key_bind_pair_map().clear();
    defalut_Info().hot_key_code_association_map().clear();
    allow_key_code_map().clear();

    HotKeyInfoMap::iterator found_itr = hot_key_info_map().begin();
    HotKeyInfoMap::iterator end_itr = hot_key_info_map().end();
    BASE_HotKeyInfo* hot_key_info = NULL;
    for ( ; found_itr != end_itr; ++found_itr)
    {
        hot_key_info = found_itr->second;
        SAFE_DELETE(hot_key_info);
    }
    hot_key_info_map().clear();
}

void HotKeyInfoParser::BuildAllowKeyCodeList()
{
    allow_key_code_map().clear();

    // 제한했으면 하는 키코드
    //allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_RETURN, 8283));
    //allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPADENTER, 8283));
    
    // 허용 키코드
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_ESCAPE, 8282));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_1, 8202));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_2, 8203));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_3, 8204));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_4, 8205));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_5, 8206));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_6, 8207));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_7, 8208));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_8, 8209));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_9, 8210));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_0, 8211));
    
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_MINUS, 8212));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_EQUALS, 8213));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_BACKSPACE, 8324));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_TAB, 8215));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_CAPSLOCK, 8216));

    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_Q, 8220));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_W, 8221));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_E, 8222));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_R, 8223));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_T, 8224));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_Y, 8225));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_U, 8226));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_I, 8227));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_O, 8228));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_P, 8229));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_LBRACKET, 8246));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_RBRACKET, 8247));

    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_A, 8230));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_S, 8231));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_D, 8232));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F, 8233));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_G, 8234));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_H, 8235));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_J, 8236));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_K, 8237));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_L, 8238));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_SEMICOLON, 8248));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_APOSTROPHE, 8249));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_BACKSLASH, 8323));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_YEN, 8325));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_CIRCUMFLEX, 8326));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_COLON, 8342));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_GRAVE, 8201));

    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_Z, 8239));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_X, 8240));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_C, 8241));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_V, 8242));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_B, 8243));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_N, 8244));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_M, 8245));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_COMMA, 8250));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_PERIOD, 8251));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_SLASH, 8252));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_SPACE, 8350));

    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F1, 8188));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F2, 8189));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F3, 8190));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F4, 8191));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F5, 8192));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F6, 8193));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F7, 8194));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F8, 8195));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F9, 8196));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F10, 8197));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F11, 8198));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_F12, 8199));

    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_SYSRQ, 8259));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_SCROLL, 8260));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_PAUSE, 8261));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_INSERT, 8253));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_DELETE, 8256));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_HOME, 8254));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_END, 8257));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_PGUP, 8255));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_PGDN, 8258));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_UP, 8327));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_DOWN, 8328));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_LEFT, 8329));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_RIGHT, 8330));

    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMLOCK, 8262));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPADSLASH, 8351));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPADSTAR, 8352));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPADMINUS, 8353));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPADPLUS, 8354));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPADPERIOD, 8314));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD1, 8305));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD2, 8306));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD3, 8307));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD4, 8308));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD5, 8309));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD6, 8310));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD7, 8311));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD8, 8312));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD9, 8313));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_NUMPAD0, 8304));

    // 마우스관련 키코드
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(MOUSE_MBUTTON, 8317));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(MOUSE_BUTTON4, 8318));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(MOUSE_BUTTON5, 8319));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(MOUSE_BUTTON6, 8320));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(MOUSE_BUTTON7, 8321));
    allow_key_code_map().insert(AllowKeyCodeMap::value_type(MOUSE_BUTTON8, 8322));

    WORD mouse_wheel_value = MOUSE_WHEEL_BUTTON_UP;
    allow_key_code_map().insert( AllowKeyCodeMap::value_type( mouse_wheel_value , 8315 ) );
    mouse_wheel_value = MOUSE_WHEEL_BUTTON_DOWN;
    allow_key_code_map().insert(AllowKeyCodeMap::value_type( mouse_wheel_value , 8316));
 
    // 큇슬록용 스트링 코드
    quick_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_PGUP, 8355));
    quick_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_PGDN, 8356));
    quick_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_DELETE, 8357));
    quick_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_INSERT, 8362));
    quick_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_SYSRQ, 8358));
    quick_key_code_map().insert(AllowKeyCodeMap::value_type(DIK_CAPSLOCK, 8359));
}

BOOL HotKeyInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
    Unload();
    BuildAllowKeyCodeList();

    int row_size = GetRowSize();
    HOT_KEY_CODE temp_key_code;

    for (int row_index = 0; row_index < row_size; ++row_index)
    {
        DWORD index = GetDataDWORD("Index", row_index);

        if (defalut_Info().key_bind_pair_map().find(static_cast<HOT_KEY_FUNCTION_INDEX>(index)) != defalut_Info().key_bind_pair_map().end())
        {
            // 중복 인덱스
            assert(!"HotKeyInfoParser - duplicated index");
            continue;
        }

        BASE_HotKeyInfo* new_parser_info = new BASE_HotKeyInfo;
        hot_key_info_map().insert(HotKeyInfoMap::value_type(index, new_parser_info));

        new_parser_info->function_index = index;

        new_parser_info->support_setting = (GetDataBYTE("SupportUserSetting", row_index) == 0) ? false : true;

        new_parser_info->description_code = GetDataDWORD("DescriptionCode", row_index);

        temp_key_code.key_code_ = GetKeyCodeByString(GetDataString("KeyCode1", row_index));
        temp_key_code.shift_key_ = (GetDataBYTE("ShiftKey1", row_index) == 0) ? 0 : 1;
        temp_key_code.ctrl_key_ = (GetDataBYTE("CtrlKey1", row_index) == 0) ? 0 : 1;
        temp_key_code.alt_key_ = (GetDataBYTE("AltKey1", row_index) == 0) ? 0 : 1;
        new_parser_info->key_pair.SetHotKeyCode(KEY_BIND_PAIR::kMainHotKey , temp_key_code.hot_key_code_);

        temp_key_code.key_code_ = GetKeyCodeByString(GetDataString("KeyCode2", row_index));
        temp_key_code.shift_key_ = (GetDataBYTE("ShiftKey2", row_index) == 0) ? 0 : 1;
        temp_key_code.ctrl_key_ = (GetDataBYTE("CtrlKey2", row_index) == 0) ? 0 : 1;
        temp_key_code.alt_key_ = (GetDataBYTE("AltKey2", row_index) == 0) ? 0 : 1;
        new_parser_info->key_pair.SetHotKeyCode(KEY_BIND_PAIR::kSubHotKey , temp_key_code.hot_key_code_);

        new_parser_info->action_code = GetDataWORD("ActionCode", row_index);

        defalut_Info().SetKeyBindInfo(static_cast<HOT_KEY_FUNCTION_INDEX>(index), new_parser_info->key_pair);
    }

    return TRUE;
}

bool HotKeyInfoParser::GetDefalutKeyInfo( KEY_BIND_INFO& output_info )
{
    output_info = defalut_Info();

    return true;
}

DWORD HotKeyInfoParser::GetDescriptionCodeByIndex( HOT_KEY_FUNCTION_INDEX function_index )
{
    HotKeyInfoMap::iterator found_itr = hot_key_info_map().find(function_index);
    if (found_itr != hot_key_info_map().end())
    {
        BASE_HotKeyInfo* base_hot_key_info = found_itr->second;
        return base_hot_key_info->description_code;
    }

    return 0;
}

bool HotKeyInfoParser::SupportUserSetting( HOT_KEY_FUNCTION_INDEX function_index )
{
    HotKeyInfoMap::iterator found_itr = hot_key_info_map().find(function_index);
    if (found_itr != hot_key_info_map().end())
    {
        BASE_HotKeyInfo* base_hot_key_info = found_itr->second;
        if (base_hot_key_info->support_setting == 1)
        {
            return true;
        }
    }

    return false;
}

bool HotKeyInfoParser::GetIndexList(STLX_VECTOR<DWORD>& output_list)
{
    output_list.clear();
    
    HotKeyInfoMap::iterator found_itr = hot_key_info_map().begin();
    HotKeyInfoMap::iterator end_itr = hot_key_info_map().end();
    BASE_HotKeyInfo* base_hot_key_info = NULL;
    for ( ; found_itr != end_itr; ++found_itr)
    {
        base_hot_key_info = found_itr->second;
        if (base_hot_key_info->support_setting == 1)
        {
            output_list.push_back(base_hot_key_info->function_index);
        }
    }

    return true;
}

bool HotKeyInfoParser::IsAllowKeyCode( WORD key_code )
{
    AllowKeyCodeMap::iterator found_itr = allow_key_code_map().find(key_code);
    if (found_itr != allow_key_code_map().end())
    {
        return true;
    }

    return false;
}

int HotKeyInfoParser::GetStringCodeByKeyCode( WORD key_code )
{
    AllowKeyCodeMap::iterator found_itr = allow_key_code_map().find(key_code);
    if (found_itr != allow_key_code_map().end())
    {
        return found_itr->second;
    }

    return 0;
}

int HotKeyInfoParser::GetStringCodeForQuickSlot( WORD key_code )
{
    AllowKeyCodeMap::iterator found_itr = quick_key_code_map().find(key_code);
    if (found_itr != quick_key_code_map().end())
    {
        return found_itr->second;
    }

    return GetStringCodeByKeyCode(key_code);
}

WORD HotKeyInfoParser::GetKeyCodeByString(const TCHAR* key_string)
{
    AllowKeyCodeMap::iterator found_itr = allow_key_code_map().begin();
    AllowKeyCodeMap::iterator end_itr = allow_key_code_map().end();
    const TCHAR* found_string = NULL;
    for ( ; found_itr != end_itr; ++found_itr)
    {
        found_string = g_InterfaceManager.GetInterfaceString(found_itr->second);
        if (strcmp(key_string, found_string) == 0)
        {
            return found_itr->first;
        }
    }

    return 0;
}

WORD HotKeyInfoParser::GetUserActionCode(HOT_KEY_FUNCTION_INDEX function_index)
{
    HotKeyInfoMap::iterator found_itr = hot_key_info_map().find(function_index);
    if (found_itr != hot_key_info_map().end())
    {
        BASE_HotKeyInfo* base_hot_key_info = found_itr->second;
        return base_hot_key_info->action_code;
    }

    return 0;
}

