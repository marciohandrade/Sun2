#include "SunClientPrecompiledHeader.h"

#include "uiLoginBackTop.h"
#include "uiLoginMan/uiLoginMan.h"
#include "GlobalFunc.h"
#include "GraphicResourceList.h"

DWORD uiLoginBackTop::kDialog_ID = StrToWzID("0023");

//------------------------------------------------------------------------------ 
uiLoginBackTop::uiLoginBackTop(InterfaceManager* ui_man) : uiBase(ui_man)
{
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);

    class_type_ = eCHAR_NONE;
}
//------------------------------------------------------------------------------ 
uiLoginBackTop::~uiLoginBackTop(void)
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiLoginBackTop::Init(CDrawBase* drawbase)
{
    uiBase::Init(drawbase);
}
//------------------------------------------------------------------------------ 
void uiLoginBackTop::Release(void)
{
    uiBase::Release();
}
//------------------------------------------------------------------------------ 
uiLoginMan* uiLoginBackTop::GetManager()
{
    return GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
}
//------------------------------------------------------------------------------ 
void uiLoginBackTop::OnShowWindow(BOOL val)
{
    uiBase::OnShowWindow(val);
    if (val)
    {
    }
    else
    {
       class_type_ = eCHAR_NONE;
    }
}

//------------------------------------------------------------------------------ 
void uiLoginBackTop::Process(DWORD tick)
{

}
//------------------------------------------------------------------------------ 
void uiLoginBackTop::OnRenderSolarDialog()
{
    if (class_type_ != eCHAR_NONE)
    {
        RenderClassMark();
    }    
}

//------------------------------------------------------------------------------ 
void uiLoginBackTop::MessageProc(SI_MESSAGE* message)
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
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

//------------------------------------------------------------------------------ 
bool uiLoginBackTop::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
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
    default:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
void uiLoginBackTop::SetSceneTitle(DWORD title_string_code)
{
    if (title_string_code == 0)
    {
        GetControlWZ(StrToWzID("S000"))->ShowWindowWZ(WZ_HIDE);
        GetControlWZ(StrToWzID("S001"))->ShowWindowWZ(WZ_HIDE);
    }
    else
    {
        //! 5002 : 로그인 임시
        //! 50 : 캐릭터 선택

        TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {NULL,};
        g_InterfaceManager.GetInterfaceString(title_string_code, szMessage, INTERFACE_STRING_LENGTH);

        GetControlWZ(StrToWzID("S000"))->ShowWindowWZ(WZ_SHOW);
        GetControlWZ(StrToWzID("S001"))->ShowWindowWZ(WZ_SHOW);
        GetControlWZ(StrToWzID("S000"))->SetTextWZ(szMessage);
        GetControlWZ(StrToWzID("S001"))->SetTextWZ(szMessage);
    }

    class_type_ = eCHAR_NONE;
}

void uiLoginBackTop::SetSceneTitleClassName(BYTE class_type, DWORD class_name_code)
{
    if ((class_type == 0) || 
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        (class_type > eCHAR_TYPE_MAX))
#else
        (class_type > eCHAR_ELEMENTALIST))
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    {
        return;
    }
    
    SetSceneTitle(class_name_code);
    class_type_ = class_type;

}

void uiLoginBackTop::SetSceneTitleString(TCHAR* title_string)
{
    if (title_string == NULL)
    {
        SetSceneTitle(0);
        return;
    }

    GetControlWZ(StrToWzID("S000"))->ShowWindowWZ(WZ_SHOW);
    GetControlWZ(StrToWzID("S001"))->ShowWindowWZ(WZ_SHOW);
    GetControlWZ(StrToWzID("S000"))->SetTextWZ(title_string);
    GetControlWZ(StrToWzID("S001"))->SetTextWZ(title_string);

    class_type_ = eCHAR_NONE;
}

void uiLoginBackTop::RenderClassMark()
{
    CControlWZ* control =  GetControlWZ(StrToWzID("S000"));
    if (control == NULL)
    {
        return;
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    RESOURCE_INFO* resource_info = GraphicResourceList::Instance()->GetItemInfo(ClassMarkInfo::kClassMarkResourceCode);
    if (resource_info == NULL)
    {
        return;
    }

    HANDLE class_mark_handle = GetDrawBase()->x_pManagerTexture->LoadTexture(resource_info->szFilename);
    if (class_mark_handle == INVALID_HANDLE_VALUE)
    {
        return;
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    TCHAR class_name[INTERFACE_STRING_LENGTH] = {'\0', };
    WzID text_font = control->GetFontID();
    RECT control_rect = control->GetSize();

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    float draw_x = static_cast<float>(control_rect.left + (control_rect.right / 2));
    float draw_y = static_cast<float>(control_rect.top + ((control_rect.bottom / 2)));
    float offset_y = static_cast<float>(((class_type_ - 1) * ClassMarkInfo::kClassMarkTextureWidth));
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    control->GetTextWZA(class_name, INTERFACE_STRING_LENGTH -1);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    draw_x -= (GlobalFunc::GetTextWidth(class_name, text_font) / 2);
    draw_x -= ((ClassMarkInfo::kClassMarkTextureWidth) + 10);
    draw_y -= ((ClassMarkInfo::kClassMarkTextureHeight / 2) + 3);


    GetDrawBase()->RenderTexture(class_mark_handle, 
        draw_x, 
        draw_y, 
        static_cast<float>(ClassMarkInfo::kClassMarkTextureWidth), 
        static_cast<float>(ClassMarkInfo::kClassMarkTextureHeight), 
        static_cast<float>(ClassMarkInfo::kClassMarkTextureX), 
        static_cast<float>(ClassMarkInfo::kClassMarkTextureY + offset_y), 
        static_cast<float>(ClassMarkInfo::kClassMarkTextureWidth), 
        static_cast<float>(ClassMarkInfo::kClassMarkTextureHeight));
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}
