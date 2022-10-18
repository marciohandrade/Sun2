#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiPetGauge.h"
#include "GlobalFunc.h"
#include "GameStruct.h"
#include "GameOption.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"

//------------------------------------------------------------------------------
uiPetGauge::uiPetGauge(InterfaceManager* ui_man) : 
    uiBase(ui_man),
    original_fullness_control_size_(0),
    original_intimacy_control_size_(0)
{
    set_dialog_position(0, 0);

    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
}
//------------------------------------------------------------------------------
uiPetGauge::~uiPetGauge()
{
    Release();
}
//------------------------------------------------------------------------------
void uiPetGauge::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    control_ptr_[kControl_Button_B000] = GetControlWZ(GetControlID_byPosition(kControl_Button_B000));
    control_ptr_[kControl_Text_S000] = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));
    control_ptr_[kControl_Text_S001] = GetControlWZ(GetControlID_byPosition(kControl_Text_S001));
    control_ptr_[kControl_Picture_P000] = GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    control_ptr_[kControl_Picture_P001] = GetControlWZ(GetControlID_byPosition(kControl_Picture_P001));
    control_ptr_[kControl_Picture_P002] = GetControlWZ(GetControlID_byPosition(kControl_Picture_P002));
    control_ptr_[kControl_Picture_P003] = GetControlWZ(GetControlID_byPosition(kControl_Picture_P003));

    control_ptr_[kControl_Text_S000]->SetDiscardMessage(true);
    control_ptr_[kControl_Text_S001]->SetDiscardMessage(true);
    control_ptr_[kControl_Picture_P001]->SetDiscardMessage(true);
    control_ptr_[kControl_Picture_P002]->SetDiscardMessage(true);
    control_ptr_[kControl_Picture_P003]->SetDiscardMessage(true);

    //! 원본 사이즈 기억해 놓는다.
    original_fullness_control_size_ = control_ptr_[kControl_Picture_P001]->GetSizeWidth();
    original_intimacy_control_size_ = control_ptr_[kControl_Picture_P002]->GetSizeWidth();
}
//------------------------------------------------------------------------------
void uiPetGauge::Release()
{
    ZeroMemory(control_ptr_, sizeof(control_ptr_));
}
//------------------------------------------------------------------------------
void uiPetGauge::render(WzVector pet_position)
{
    if (IsVisible() == FALSE)
    {
        return;
    }
    if (CGameOption::Instance()->GetPetGaugeShowType() != kPetGaugeShowType_Head)
    {
        return;
    }

    BOOL backup_z_test = g_pSunRenderer->IsEnableZTest();
    BOOL backup_z_write = g_pSunRenderer->IsEnableZWrite();

    g_pSunRenderer->Set2DMode(TRUE);
    g_pSunRenderer->SetZBufferTest(TRUE);
    g_pSunRenderer->SetZBufferWrite(TRUE);

    {
        RECT area = GetDialogWZ()->GetClientWZ()->GetSize();

        float fDepth;
        POINT pt2;
        g_pSunRenderer->GetScreenPos(&pt2, &pet_position, &fDepth);

        float cx = pt2.x - (static_cast<float>(area.right - control(kControl_Button_B000)->GetSizeWidth()) * 0.5f);
        float cy = static_cast<float>(pt2.y - 24.0f);

        GetDialogWZ()->MoveWindowWZ(cx, cy);
        GetDialogWZ()->SetDepth(fDepth);

        control(kControl_Button_B000)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P000)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Text_S000)->ShowWindowWZ(WZ_SHOW);
        control(kControl_Text_S001)->ShowWindowWZ(WZ_SHOW);
        control(kControl_Picture_P001)->ShowWindowWZ(WZ_SHOW);
        control(kControl_Picture_P002)->ShowWindowWZ(WZ_SHOW);
        control(kControl_Picture_P003)->ShowWindowWZ(WZ_SHOW);

#ifdef _NA_000000_20130114_RENEWER_UI
		control(kControl_Picture_P003)->OnDraw(FALSE);
		control(kControl_Picture_P001)->OnDraw(FALSE);
		control(kControl_Picture_P002)->OnDraw(FALSE);
		control(kControl_Text_S000)->OnDraw(FALSE);
		control(kControl_Text_S001)->OnDraw(FALSE);
#else
		GetDialogWZ()->OnDraw(FALSE);
#endif
        control(kControl_Button_B000)->ShowWindowWZ(WZ_SHOW);
        control(kControl_Picture_P000)->ShowWindowWZ(WZ_SHOW);
        control(kControl_Text_S000)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P001)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P002)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P003)->ShowWindowWZ(WZ_HIDE);

        GetDialogWZ()->SetDepth(0);

        //! 위치 복구
        GetDialogWZ()->MoveWindowWZ((float)area.left, (float)area.top);
    }

    g_pSunRenderer->SetZBufferWrite(backup_z_write);	
    g_pSunRenderer->SetZBufferTest(backup_z_test);
    g_pSunRenderer->Set2DMode(FALSE);
}
//------------------------------------------------------------------------------
void uiPetGauge::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        ChangeShowType();
    }
    else
    {
        RECT area = GetDialogWZ()->GetClientWZ()->GetSize();
        set_dialog_position(area.left, area.top);
    }
}
////------------------------------------------------------------------------------
//void uiPetGauge::UpdatePosition()
//{
//    if ((dialog_position().x == 0) || (dialog_position().y == 0))
//    {
//        MoveDefaultWindowPos();
//    }
//    else
//    {
//        GetDialogWZ()->MoveWindowWZ(static_cast<float>(dialog_position().x), static_cast<float>(dialog_position().y));
//    }
//}
//------------------------------------------------------------------------------
void uiPetGauge::MessageProc(SI_MESSAGE * message)
{
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
bool uiPetGauge::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B000:
        {
            NextShowType();
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------
void uiPetGauge::NextShowType()
{
    if (IsVisible() == FALSE)
    {
        return;
    }

    int index = static_cast<int>(CGameOption::Instance()->GetPetGaugeShowType()) + 1;
    
    if (static_cast<PetGauge_ShowType>(index) >= kPetGaugeShowType_Size)
    {
        CGameOption::Instance()->SetPetGaugeShowType(kPetGaugeShowType_Head);
    }
    else
    {
        CGameOption::Instance()->SetPetGaugeShowType(static_cast<PetGauge_ShowType>(index));
    }

    ChangeShowType();
}
//------------------------------------------------------------------------------
void uiPetGauge::ChangeShowType()
{
    if (IsVisible() == FALSE)
    {
        return;
    }

    GetDialogWZ()->SetDepth(0.0f);

    control(kControl_Button_B000)->ShowWindowWZ(WZ_SHOW);
    control(kControl_Picture_P000)->ShowWindowWZ(WZ_SHOW);
    control(kControl_Text_S000)->ShowWindowWZ(WZ_SHOW);
    control(kControl_Text_S001)->ShowWindowWZ(WZ_SHOW);
    control(kControl_Picture_P001)->ShowWindowWZ(WZ_SHOW);
    control(kControl_Picture_P002)->ShowWindowWZ(WZ_SHOW);
    control(kControl_Picture_P003)->ShowWindowWZ(WZ_SHOW);

    if (CGameOption::Instance()->GetPetGaugeShowType() == kPetGaugeShowType_Mini)
    {
        control(kControl_Text_S000)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P001)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P002)->ShowWindowWZ(WZ_HIDE);
        control(kControl_Picture_P003)->ShowWindowWZ(WZ_HIDE);
    }
}
//------------------------------------------------------------------------------
void uiPetGauge::set_fullness(int fullness, int max_value)
{
    if (IsVisible() == FALSE)
    {
        return;
    }

    if (fullness > max_value)
    {
        fullness = max_value;
    }

    TCHAR szText[128] = {NULL, };
    Sprintf(szText, _T("%d/%d"), fullness, max_value);

    float length = original_fullness_control_size() / float(max_value) * float(fullness);

    static_cast<CCtrlStaticWZ*>(control(kControl_Text_S000))->SetTextWZ(szText);
    static_cast<CCtrlPictureWZ*>(control(kControl_Picture_P001))->SetSizeWidth(static_cast<long>(length));
}
//------------------------------------------------------------------------------
void uiPetGauge::set_intimacy(int intimacy, int max_value)
{
    if (IsVisible() == FALSE)
    {
        return;
    }

    if (intimacy > max_value)
    {
        intimacy = max_value;
    }

    TCHAR szText[128] = {NULL, };
    Sprintf(szText, _T("%d/%d"), intimacy, max_value);

    float length = original_intimacy_control_size() / float(max_value) * float(intimacy);

    static_cast<CCtrlStaticWZ*>(control(kControl_Text_S001))->SetTextWZ(szText);
    static_cast<CCtrlPictureWZ*>(control(kControl_Picture_P002))->SetSizeWidth(static_cast<long>(length));
}

void uiPetGauge::OnRenderSolarDialog()
{
	int x	= Mouse::Instance()->GetMouseX();
	int y	= Mouse::Instance()->GetMouseY();

	RECT pos_control;

	CControlWZ* controller = control(kControl_Button_B000);
	if(controller)
	{
		pos_control = controller->GetSizeRect();

		if( x > pos_control.left && x < pos_control.right && 
			y > pos_control.top &&  y < pos_control.bottom )
		{
			uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
			if (tooltip_manager)
			{
				TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
				if(CGameOption::Instance()->GetPetGaugeShowType() == kPetGaugeShowType_Head)
				{
					g_InterfaceManager.GetInterfaceString(11094, tooltip_message);
				}
				if(CGameOption::Instance()->GetPetGaugeShowType() == kPetGaugeShowType_Screen)
				{
					g_InterfaceManager.GetInterfaceString(11093, tooltip_message);
				}
				if(CGameOption::Instance()->GetPetGaugeShowType() == kPetGaugeShowType_Mini)
				{
					g_InterfaceManager.GetInterfaceString(11095, tooltip_message);
				}

				tooltip_manager->RegisterTooltip(tooltip_message);
			}
		}
	}

	if (CGameOption::Instance()->GetPetGaugeShowType() != kPetGaugeShowType_Screen)
	{
		return;
	}

	pos_control = control_ptr_[kControl_Picture_P001]->GetSizeRect();
	pos_control.right = pos_control.left + (int)original_fullness_control_size_;
	
	if( x > pos_control.left && x < pos_control.right && 
		y > pos_control.top &&  y < pos_control.bottom )
	{
		uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
		if (tooltip_manager)
		{
			TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
			g_InterfaceManager.GetInterfaceString(1415, tooltip_message);

			tooltip_manager->RegisterTooltip(tooltip_message);
		}
	}

	pos_control = control_ptr_[kControl_Picture_P002]->GetSizeRect();
	pos_control.right = pos_control.left + (int)original_intimacy_control_size_;

	if( x > pos_control.left && x < pos_control.right && 
		y > pos_control.top &&  y < pos_control.bottom )
	{
		uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
		if (tooltip_manager)
		{
			TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
			g_InterfaceManager.GetInterfaceString(1416, tooltip_message);

			tooltip_manager->RegisterTooltip(tooltip_message);
		}
	}

	
}	


int uiPetGauge::ClickMessage()
{
#if WZENGINEVER >= 590
	CDialogManager* pDialogManager = g_InterfaceManager.GetDialogManager();
	pDialogManager->MoveTail( GetDialogKey() );
#endif // WZENGINEVER >= 590

	return 0;
}