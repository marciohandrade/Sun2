#include "SunClientPrecompiledHeader.h"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "ChaosSystem/uiChaosZonePassword.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"

//------------------------------------------------------------------------------
void uiChaosZonePassword::RegisterControl()
{
    RegisterControlPosition(StrToWzID("E000"), DLGPOS_EDT_PASSWORD); 
    RegisterControlPosition(StrToWzID("TI00"), DLGPOS_BTN_OK); 
    RegisterControlPosition(StrToWzID("TI01"), DLGPOS_BTN_CANCEL); 
}

uiChaosZonePassword::uiChaosZonePassword(InterfaceManager *pUIMan) :
uiBase(pUIMan),
room_key_(0)
{
}

//------------------------------------------------------------------------------
/**
*/
uiChaosZonePassword::~uiChaosZonePassword()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZonePassword::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    RegisterControl();
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZonePassword::Release()
{
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZonePassword::OnShowWindow( BOOL val )
{
    if (val == TRUE)
    {
        CCtrlDxIMEEditBox* edit_control_ =
            static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(DLGPOS_EDT_PASSWORD));
        if (edit_control_)
        {
            edit_control_->SetTextWZ(_T(""));
        }

        GlobalFunc::SetFocus((SolarDialog*)this, edit_control_);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZonePassword::MessageProc(SI_MESSAGE* pMessage)
{
    CCtrlDxIMEEditBox* edit_control_ =
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(DLGPOS_EDT_PASSWORD));
    if (edit_control_ == NULL)
    {
        this->ShowInterface(FALSE);
        return;
    }

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            switch (GetControlPosition_byID(pMessage->dwCtrlID))
            {
            case DLGPOS_EDT_PASSWORD:
                {
                    TCHAR password_string[MAX_ROOMPASSWORD_LENGTH];

                    // 비밀번호의 최대 사이즈(7 byte)를 넘었습니다.
                    if (!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)edit_control_,
                        password_string,
                        MAX_ROOMPASSWORD_LENGTH,
                        5792))
                    {
                        return;
                    }

                    if (_tcslen(password_string) == 0)
                    {
                        //	비밀번호를 입력 하십시오.
                        TCHAR message[INTERFACE_STRING_LENGTH];
                        g_InterfaceManager.GetInterfaceString(eST_INPUT_PASSWORD, message, INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                        GlobalFunc::SetFocus((SolarDialog*)this, edit_control_);
                    }
                    else
                    {
                        ChaosSystem::PacketFunction::SEND_CG_FREE_PVP_JOIN_ROOM_SYN(
                            room_key_,
                            true,
                            password_string);
                        this->ShowInterface(FALSE);
                    }
                }
                break;
            }
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            DWORD pos = GetControlPosition_byID(pMessage->dwCtrlID);
            if (pos >= DLGPOS_EDT_PASSWORD && pos <= DLGPOS_BTN_CANCEL)
            {
                switch (pos)
                {
                case DLGPOS_BTN_OK:
                    {
                        TCHAR password_string[MAX_ROOMPASSWORD_LENGTH];

                        // 비밀번호의 최대 사이즈(7 byte)를 넘었습니다.
                        if (!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)edit_control_,
                            password_string,
                            MAX_ROOMPASSWORD_LENGTH,
                            5792))
                        {
                            return;
                        }

                        if (_tcslen( password_string ) == 0)
                        {
                            TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
                            //	비밀번호를 입력 하십시오.
                            g_InterfaceManager.GetInterfaceString(eST_INPUT_PASSWORD, buffer_string, INTERFACE_STRING_LENGTH);
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, buffer_string);
                            GlobalFunc::SetFocus((SolarDialog*)this, edit_control_);
                        }
                        else
                        {
                            ChaosSystem::PacketFunction::SEND_CG_FREE_PVP_JOIN_ROOM_SYN(
                                room_key_,
                                true,
                                password_string);

                            this->ShowInterface(FALSE);
                        }
                    }
                    break;

                case DLGPOS_BTN_CANCEL:
                    {
                        this->ShowInterface(FALSE);
                    }
                    break;
                }
            }
        }
        break;

    default:
        {
            SolarDialog::MessageProc( pMessage );
        }
        break;
    }
}



//------------------------------------------------------------------------------
/**
*/
void uiChaosZonePassword::OnUpdateSolarDialog()
{
    if(!this->HaveFocus())
        ShowInterface(FALSE);
}

void uiChaosZonePassword::Process(DWORD tick_count)
{
}

void uiChaosZonePassword::MouseOver_MsgProc(SI_MESSAGE* message)
{

}

#endif//_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
