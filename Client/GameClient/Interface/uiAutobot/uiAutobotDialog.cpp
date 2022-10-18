#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotDialog.h"
#include "uiAutobotMan.h"
#include "uiAutobotConditionSupport.h"
#include "AutomaticInfoParser.h"
#include "AutomaticComboInfoParser.h"

//------------------------------------------------------------------------------ 
uiAutobotDialog::uiAutobotDialog(InterfaceManager* ui_man) : uiBase(ui_man)
{
}

//------------------------------------------------------------------------------ 
uiAutobotDialog::~uiAutobotDialog( void )
{
}
//------------------------------------------------------------------------------ 
bool uiAutobotDialog::InsertItemToDropdown(DWORD list_index, std::vector<BASE_AutomaticComboInfo*>& value_vector)
{
    TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };
    TCHAR format[INTERFACE_STRING_LENGTH] = {NULL, };

    std::vector<BASE_AutomaticComboInfo*>::iterator begin = value_vector.begin();
    std::vector<BASE_AutomaticComboInfo*>::iterator end = value_vector.end();
    for (; begin != end; ++begin)
    {
        BASE_AutomaticComboInfo* info = (*begin);

        g_InterfaceManager.GetInterfaceString(info->stringtable_code, format);
        _snprintf(message, INTERFACE_STRING_LENGTH, format, info->option_value);

        if (InsertItemToDropdown(list_index, message, static_cast<void*>(info)) == false)
        {
            return false;
        }
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool uiAutobotDialog::InsertItemToDropdown(DWORD list_index, char* item_text, IN void* user_data)
{
    static DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
    static DWORD c_dwSelTextColor = RGBA(0,220,0,255);
    static DWORD c_dwBackColor = RGBA(0,0,0,0);

    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));

    if (list_control == NULL)
    {
        return false;
    }


    int index = list_control->InsertItemBack();

    list_control->SetStartLineGapTop(3);
    list_control->SetSellHeight(14);
    list_control->SetFontID(StrToWzID("st10"));
    list_control->SetItemWZA(index, 
                            item_text, 
                            StrToWzID("st10"), 
                            DT_VCENTER | DT_CENTER, 
                            c_dwBackColor, 
                            c_dwTextColor, 
                            c_dwSelTextColor);

    list_control->SetItemData(index, user_data);

    return true;

}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::ChangeDropdownItem(DWORD list_index, DWORD staticbox_index)
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));
    CControlWZ* staticbox_control = GetControlWZ(GetControlID_byPosition(staticbox_index));

    staticbox_control->SetTextWZ("");

    int select_index = list_control->GetCurSel();
    if (select_index >= 0)
    {
        const char* text = list_control->GetItemText(select_index, 0);
        if (text != NULL)
        {
            staticbox_control->SetTextWZ((char*)text);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::SelectDropdownItem(DWORD list_index, DWORD staticbox_index, int select_index)
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));
    if ((select_index >= 0) && (select_index < list_control->GetListRowCnt()))
    {
        list_control->SetCurSel(select_index);
    }
    ChangeDropdownItem(list_index, staticbox_index);
}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::SelectDropdownItem(DWORD list_index, DWORD staticbox_index, const char* item_text)
{
    if (item_text != NULL)
    {
        CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));
        int item_count = list_control->GetListRowCnt();

        for (int i = 0; i < item_count; ++i)
        {
            if (_stricmp(item_text, list_control->GetItemText(i, 0)) == 0)
            {
                list_control->SetCurSel(i);
                ChangeDropdownItem(list_index, staticbox_index);
                return;
            }
        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::SelectDropdownItemByComboValue(DWORD list_index, DWORD staticbox_index, int combo_option_value)
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));
    int item_count = list_control->GetListRowCnt();

    for (int i = 0; i < item_count; ++i)
    {
        BASE_AutomaticComboInfo* info = static_cast<BASE_AutomaticComboInfo*>(list_control->GetItemData(i));
        if (combo_option_value == info->option_value)
        {
            list_control->SetCurSel(i);
            ChangeDropdownItem(list_index, staticbox_index);
            return;
        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::SelectDropdownItemByComboItemCode(DWORD list_index, DWORD staticbox_index, int combo_item_code)
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));
    int item_count = list_control->GetListRowCnt();

    for (int i = 0; i < item_count; ++i)
    {
        BASE_AutomaticComboInfo* info = static_cast<BASE_AutomaticComboInfo*>(list_control->GetItemData(i));
        if (combo_item_code == info->item_code)
        {
            list_control->SetCurSel(i);
            ChangeDropdownItem(list_index, staticbox_index);
            return;
        }
    }
}
//------------------------------------------------------------------------------ 
void* uiAutobotDialog::GetCurDropdownItemData(DWORD list_index)
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));

    int select_index = list_control->GetCurSel();
    if (select_index >= 0)
    {
        return list_control->GetItemData(select_index);
    }

    return NULL;
}
//------------------------------------------------------------------------------ 
const char* uiAutobotDialog::GetCurDropdownItemText(DWORD list_index)
{
    CCtrlListWZ* list_control = GetListControlWZ(GetControlID_byPosition(list_index));

    int select_index = list_control->GetCurSel();

    if (select_index >= 0)
    {
        return list_control->GetItemText(select_index, 0);
    }

    return NULL;
}
//------------------------------------------------------------------------------ 
bool uiAutobotDialog::IsEnableMenu(eAUTO_HUNTING_TYPE hunting_type, SLOTCODE item_code)
{
    if (item_code <= 0)
    {
        return false;
    }

    BASE_AutomaticInfo* info = AutomaticInfoParser::Instance()->GetAutomaticInfo(hunting_type);
    if (info == NULL)
    {
        return false;
    }

    for (int i = 0; i < BASE_AutomaticInfo::kItemCount; ++i)
    {
        if (info->item_code[i] == item_code)
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::SetEnableByItem(DWORD control_index, bool is_enable, SLOTCODE item_code, DWORD text_control_index)
{
    if (is_enable)
    {
        is_enable = IsEnableMenu(GetHuntingTypeFromControlIndex(control_index), item_code);
    }
    SetEnable(control_index, is_enable);
    SetTextColor(text_control_index, (is_enable ? RGBA(255, 255, 255,255) : RGBA(255, 255, 255, 128)));
}
//------------------------------------------------------------------------------ 
void uiAutobotDialog::SetCheckedByItem(DWORD control_index, bool is_checked, SLOTCODE item_code)
{
    if (is_checked)
    {
        is_checked = IsEnableMenu(GetHuntingTypeFromControlIndex(control_index), item_code);
    }
    SetChecked(control_index, is_checked);
}
#endif //_YMS_AUTOBOT_GAME