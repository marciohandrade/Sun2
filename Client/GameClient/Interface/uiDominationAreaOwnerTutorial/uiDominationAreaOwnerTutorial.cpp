#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include ".\uidominationareaownertutorial.h"
#include "uiDominationTowerMan\uiDominationTowerMan.h"
#include "GlobalFunc.h"

void uiDominationAreaOwnerTutorial::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P002"), eControl_Picture_P002);
    RegisterControlPosition(StrToWzID("BT01"), eControl_Button_BT01);//닫기버튼
    RegisterControlPosition(StrToWzID("S000"), eControl_Text_S000);//타이틀
    RegisterControlPosition(StrToWzID("S001"), eControl_Text_S001);//내용
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    RegisterControlPosition(StrToWzID("S002"), eControl_Text_S002);//동맹
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
}

uiDominationAreaOwnerTutorial::uiDominationAreaOwnerTutorial(InterfaceManager* ui_manager_ptr) : 
uiBase(ui_manager_ptr),
explanation_control_ptr_(NULL)
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
,allience_text_control_(NULL)
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
{
}

uiDominationAreaOwnerTutorial::~uiDominationAreaOwnerTutorial(void)
{
    explanation_control_ptr_ = NULL;
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    allience_text_control_ = NULL;
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
}

void uiDominationAreaOwnerTutorial::Init( CDrawBase* draw_base_ptr )
{
    RegisterControl();

    ui_domination_manager_ptr_ = static_cast<uiDominationTowerMan*>(GetMan());    

    
    CControlWZ* control_ptr = GetControlWZ_byPosition(eControl_Text_S001);
    if (control_ptr)
    {
        control_ptr->SetTextWZ(_T(""));        
        explanation_control_ptr_ = static_cast<CCtrlStaticWZ*>(control_ptr);        
    }
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    control_ptr = GetControlWZ_byPosition(eControl_Text_S002);
    if (control_ptr)
    {
        control_ptr->SetTextWZ(_T(""));
        allience_text_control_ = static_cast<CCtrlStaticWZ*>(control_ptr);
        allience_text_control_->SetTextReducing(true);
    }
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
}

void uiDominationAreaOwnerTutorial::Release()
{    
    explanation_control_ptr_ = NULL;
}

void uiDominationAreaOwnerTutorial::OnRenderSolarDialog()
{
    if (NULL == explanation_control_ptr_)
        return;

#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    text_render_.Render();
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
}

void uiDominationAreaOwnerTutorial::OnShowWindow( BOOL val )
{
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_Domination_Area_Owner_Tutorial_Dialog;
        msg.wParam = IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_AREA_OWNER_TUTORIAL_DIALOG;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Domination_Area_Owner_Tutorial_Dialog);
    }
}

void uiDominationAreaOwnerTutorial::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
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

bool uiDominationAreaOwnerTutorial::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case eControl_Button_BT01://닫기
        {            
            this->ShowInterface(FALSE);
        }
        break;
    default:
        {

        }
    }
    return result;
}

#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
void uiDominationAreaOwnerTutorial::SetAreaOwnerGuildName( TCHAR* area_owner_guild_name, 
                                                          const sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO* allience_guild_infos, 
                                                          const BYTE& allience_guild_count )
{
    if (NULL == area_owner_guild_name)
        return;

    // 내용
    CCtrlStaticWZ* control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(eControl_Text_S001));
    if (!control_ptr)
        return;

    RECT rect = control_ptr->GetSizeRect();
    FTextParser parser;
    FTextSentence sentence;
    FTextParam param;

    text_render_.Init(GetDrawBase(), rect);
    text_render_.SetVCenterAlign(false);
    text_render_.SetHCenterAlign(true);
    text_render_.ClearSentence();
    
    TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };

    FTextSentence ftext_sentence;
    g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 3499, area_owner_guild_name);    
    if (parser.Parser(result_string, ftext_sentence))
    {
        text_render_.Append(ftext_sentence);
        text_render_.convertLineInfo();
    }

    if (allience_text_control_ && allience_guild_count > 0)
    {
        SUN_STRING friend_guild_names;
        memset(result_string, 0, INTERFACE_STRING_LENGTH);
        // 길드 리스트를 모두 분리 한다.
        for (int i = 0; i < allience_guild_count; ++i)
        {
            if (NULL == allience_guild_infos)
                break;

            friend_guild_names += allience_guild_infos[i].allience_guild_name;
            if (i < allience_guild_count - 1)
            {
                friend_guild_names += ", ";
            }
        }

        // 3504 : 동맹 : %s
        g_InterfaceManager.GetInterfaceStringFormat(result_string, INTERFACE_STRING_LENGTH, 3504, friend_guild_names.c_str());
        allience_text_control_->SetTextWZ(result_string);
    }
    else if (allience_text_control_)
    {
        allience_text_control_->SetTextWZ(_T(""));
    }

    this->ShowInterface(TRUE);
}
#else
void uiDominationAreaOwnerTutorial::SetAreaOwnerGuildName( TCHAR* area_owner_guild_name )
{
    if (NULL == area_owner_guild_name)
        return;

    TCHAR total_message[INTERFACE_STRING_LENGTH] = {0, };
    ZeroMemory(total_message, INTERFACE_STRING_LENGTH);    

    // 3499	브라키온 대륙은 대영주 \n %s 길드의 \n영웅들이 지배하고 있는 \n축복받은 대지 입니다.
    // \n그 분들이 계신 곳에서는 \n언행을 조심하고 \n예의를 지켜주시기 \n바랍니다.
    TCHAR string_format[INTERFACE_STRING_LENGTH] = {0, };
    g_InterfaceManager.GetInterfaceString(3499, string_format, INTERFACE_STRING_LENGTH);
    Snprintf(total_message, INTERFACE_STRING_LENGTH, string_format, area_owner_guild_name);
    
    
    // 내용
    CCtrlStaticWZ* control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(eControl_Text_S001));
    if (control_ptr)
    {        
        control_ptr->SetTextWZ(total_message);
    }    

    this->ShowInterface(TRUE);
}
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL