//------------------------------------------------------------------------------
//  StateAir.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "solardialog.h"
#include "soundEffect.h"
#include "DlgPosInfoParser.h"
#include "GraphicResourceList.h"
#include "ObjectManager.h"
#include "NPC.h"
#include "mouse.h"
#include "InterfaceManager.h"

#include "CursorChangeTransact.h"
#include "application.h"
#include "GlobalFunc.h"

SolarDialog::SolarDialog()
:m_packetStatus( 0 )
,m_bSkipSuspendFocusCheck( FALSE )
,m_bLoaded( FALSE )
,m_bShow(FALSE)
,is_ani_playing_(false)
,ani_start_tick_(0)
{
    m_bActivate = false;

	m_pDialogWZ = NULL;

	m_dwOwnerKey = 0;
	m_bDynamicWindow=FALSE;
	
    m_bCheckDistance_NPCMEET = true;

    m_bDestory=FALSE;

    control_position_map_.clear();
    control_id_map_.clear();

    is_blink_grid_ = false;

    Unlock();
}

SolarDialog::~SolarDialog()
{
    control_position_map_.clear();
    control_id_map_.clear();
}

BOOL SolarDialog::InterSectRect( POINT point )
{
	return m_pDialogWZ->InterSectRect(point);
}


void  SolarDialog::SetDialogWZ( CDialogWZ * pDialogWZ )
{
	m_pDialogWZ = pDialogWZ;
}

void  SolarDialog::Update()
{
	if( GetDialogWZ() )
	{
		if( GetDialogWZ()->IsState( WZ_HIDE ) ) return;

		OnUpdateSolarDialog();
	}
}

void SolarDialog::OnRenderSolarDialog()
{

}

void  SolarDialog::Render()
{
	if( GetDialogWZ() )
	{
		if( GetDialogWZ()->IsState( WZ_HIDE ) ) return;
	}

	OnRenderSolarDialog();

#if _DEV_VER//#ifdef _DH_SHOW_UI_NAME
    if (CGeneralGameParam::Instance()->IsShowUIName())
    {
        ShowFileName();
    }
#endif//_DH_SHOW_UI_NAME
    static bool fade_in = false;
    if (is_blink_grid_)
    {
        float x_position = GetDialogWZ()->GetDlgX();
        float y_position = GetDialogWZ()->GetDlgY();
        float fade_value = GetFadeValue(&grid_timer_, 700, fade_in);
        BYTE alpha = static_cast<BYTE>(fade_value * 255);

        WzColor colors[4] = {0,};
        WzColor color = WzColor_RGBA(255, 255, 255, alpha);
        for (int i = 0; i < 4; ++i)
        {
            colors[i] = color;
        }

        if(g_InterfaceManager.grid_effect_texture() != INVALID_HANDLE_VALUE)
        {
            for (int i = 0; i < 8; ++i)
            {
                g_pSunRenderer->RenderTexture(g_InterfaceManager.grid_effect_texture(), 
                    control_rectangles_[i].left + x_position, control_rectangles_[i].top + y_position, 
                    control_rectangles_[i].right, control_rectangles_[i].bottom, 
                    g_InterfaceManager.grid_rectangles()[i].left, 
                    g_InterfaceManager.grid_rectangles()[i].top, 
                    g_InterfaceManager.grid_rectangles()[i].right, 
                    g_InterfaceManager.grid_rectangles()[i].bottom, colors);
            }
        }

        if ((fade_value >= 1.0f && fade_in) || (fade_value == 0.0f && !fade_in))
        {
            fade_in = !fade_in;
            grid_timer_.SetTimer(700);
            //grid_timer_.Reset();
        }
    }    
}

#if _DEV_VER//#ifdef _DH_SHOW_UI_NAME
char* SolarDialog::GetFileName()
{
    return NULL;
}

void SolarDialog::ShowFileName()
{
    g_pSunRenderer->x_pManagerTextOut->StartToUseFont("m216");
    char dialog_name[INTERFACE_STRING_LENGTH] = {0,};
    char temp[INTERFACE_STRING_LENGTH] = {0,};
    if (GetFileName() != NULL)
    {
        Sprintf(temp, "%s", GetFileName());
        strtok(temp, "\\");//Data
        strtok(NULL, "\\");//Interface
        Sprintf(dialog_name, strtok(NULL, "\\"));
    }
    else
    {
        BASE_DlgPosInfo* dialog_info =
            DlgPosInfoParser::Instance()->GetDlgPosInfo(GetDialogWZ()->GetDlgID());
        if (dialog_info != NULL)
        {
            Sprintf(dialog_name, dialog_info->m_szdlgFileName);
        }
        else
        {
            WzIDToString(temp,GetDialogWZ()->GetDlgID());
            Sprintf(dialog_name, "ID:%s", temp);
        }
    }
    
    RECT dialog_rect;
    GetDialogWZ()->GetClientRect(&dialog_rect);
    g_pSunRenderer->x_pManagerTextOut->DrawText(_T(dialog_name), 
        (dialog_rect.left+9), (dialog_rect.bottom - 21), WzColor_RGB(0, 0, 0));
    g_pSunRenderer->x_pManagerTextOut->DrawText(_T(dialog_name), 
        (dialog_rect.left+11), (dialog_rect.bottom - 19), WzColor_RGB(0, 0, 0));
	g_pSunRenderer->x_pManagerTextOut->DrawText(_T(dialog_name), 
        (dialog_rect.left+10), (dialog_rect.bottom - 20), WzColor_RGB(192, 192, 0));
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}
#endif//_DH_SHOW_UI_NAME

VOID 
SolarDialog::Process(DWORD dwTick)
{

}


VOID			
SolarDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch (pMessage->eResultMsg)
    {
	
	    case RT_MSG_MOUSEOVER:
            {
				MouseOver_MsgProc(pMessage);
			}
			break;

		default:
			break;
	}	
}


VOID			
SolarDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
}

//------------------------------------------------------------------------------ 
BOOL SolarDialog::IsVisibleWindow()
{
	if (GetDialogWZ())
    {
		return GetDialogWZ()->IsState(WZ_SHOW);
	}
    return FALSE;
}

//------------------------------------------------------------------------------ 
BOOL SolarDialog::HaveFocus()
{
    if (GetDialogWZ() != NULL)
    {
        return GetDialogWZ()->HaveFocus();
    }
    return FALSE;
}

//------------------------------------------------------------------------------ 
void SolarDialog::PlayOpenDialogSound()
{
    g_SoundEffect.PlayUI( getOpenDlgSoundIndex() );
}

//------------------------------------------------------------------------------ 
void SolarDialog::PlayCloseDialogSound()
{
    g_SoundEffect.PlayUI( getCloseDlgSoundIndex() );
}

//------------------------------------------------------------------------------ 
VOID SolarDialog::ShowDialog( BOOL val )
{
	ShowWindow( val );
    m_packetStatus=0;

	// 사운드 출력
    if (TRUE == val)
	{
        PlayOpenDialogSound();
        if (is_blink_grid_)
        {
            SetBlinkGrid(grid_control_id_, is_blink_grid_);
        }
	}
	else
	{
		if (m_dwOwnerKey) 
        {
			Object *pObject = g_ObjectManager.GetObject(m_dwOwnerKey);
			if (pObject && pObject->IsKindOfObject(NPC_OBJECT))
			{
				NPC *pNPC = (NPC *)pObject;
				pNPC->PlayLeaveSound();
				
			}

			m_dwOwnerKey = 0;
		}
        is_blink_grid_ = false; //해당창을 닫으면 초기화
	}
}

//------------------------------------------------------------------------------ 
VOID SolarDialog::ShowWindow( BOOL val )
{
	if (GetDialogWZ()) 
	{
		//if (val)
		//{
		//	UpdatePosition();
		//}

		GetDialogWZ()->ShowWindowWZ((val == TRUE ? WZ_SHOW : WZ_HIDE));

		m_bShow = val;

		OnShowWindow( val );

        if (IsVisible())
        {
            POINT app_size = g_pApplication->Get2DModeMapping();
            RECT dialog_rect;
            GetDialogWZ()->GetClientRect(&dialog_rect);
            if ((dialog_rect.left > app_size.x) ||
                (dialog_rect.top > app_size.y) ||
                (dialog_rect.right < 0) ||
                (dialog_rect.bottom < 0))
            {
                MoveDefaultWindowPos();
            }
        }

		// 오픈시 현재 다이얼로그에 마우스커서가 위치하는지
		if ((val == TRUE) && 
            (IsLocked() == FALSE))
		{
            BOOL is_intersect = InterSectRect(Mouse::Instance()->CheckMousePos());
			g_Input.m_bUIMouseInput |= is_intersect;
		}

        //------------------------------------------------------------------------------
        //! 사운드 처리
        int sound_index = 0;
        CControlWZ* controlwz = GetDialogWZ()->GetClientWZ();
        if (controlwz != NULL)
        {
            if (IsVisible())
            {
                sound_index = controlwz->GetSoundIndex(kControlSoundEventControlShow);
            }
            else
            {
                sound_index = controlwz->GetSoundIndex(kControlSoundEventControlHide);
            }
        }
        if (sound_index > 0)
        {
            g_SoundEffect.PlayUI(sound_index);
        }

        //------------------------------------------------------------------------------ 
	}
}
//------------------------------------------------------------------------------ 
VOID SolarDialog::OnShowWindow( BOOL val )
{
    set_is_ani_playing(false);
    if (val == TRUE)
    {
        BASE_DlgPosInfo* info = DlgPosInfoParser::Instance()->GetDlgPosInfo(GetDialogWZ()->GetDlgID());
        if ((info != NULL) && (info->is_ani() == true))
        {
            set_is_ani_playing(true);
            set_ani_start_tick(clock_function::GetTickCount());
        }
        if (is_blink_grid_)
        {
            SetBlinkGrid(grid_control_id_, is_blink_grid_);
        }
    }
    else 
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
        is_blink_grid_ = false; //해당창을 닫으면 초기화
	}

    UpdateData(val);
}
//------------------------------------------------------------------------------ 
bool SolarDialog::MoveWindowWZ(float fx, float fy)
{
    //if (IsVisibleWindow() == FALSE)
    //{
    //    return false;
    //}

	//POINT pt;
	//WzID DlgID = GetDialogWZ()->GetDlgID();

	//if ( DlgPosInfoParser::Instance()->GetDlgPoint(DlgID, &pt) )
	//{
	//	GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
	//}
	//else
	{
		GetDialogWZ()->MoveWindowWZ(fx, fy);
	}

	return true;
}
//------------------------------------------------------------------------------ 
bool SolarDialog::MoveWindowWZ(int x, int y)
{
    //if (IsVisibleWindow() == FALSE)
    //{
    //    return false;
    //}

	//POINT pt;
	//WzID DlgID = GetDialogWZ()->GetDlgID();

	//if ( DlgPosInfoParser::Instance()->GetDlgPoint(DlgID, &pt) )
	//{
	//	GetDialogWZ()->MoveWindowWZ( (float)pt.x, (float)pt.y );
	//}
	//else
	{
		GetDialogWZ()->MoveWindowWZ((float)x, (float)y);
	}

	return true;
}
//------------------------------------------------------------------------------ 
bool SolarDialog::MoveWindowWZ(float fx, float fy, float fWidth, float fHeight)
{
	//if (IsVisibleWindow() == FALSE)
	//{
	//	return false;
	//}

	//POINT pt;
	//WzID DlgID = GetDialogWZ()->GetDlgID();
	//if ( DlgPosInfoParser::Instance()->GetDlgPoint(DlgID, &pt) )
	//{
	//	GetDialogWZ()->MoveWindowWZ( (float)pt.x, (float)pt.y, fWidth, fHeight );
	//}
	//else
	{
		GetDialogWZ()->MoveWindowWZ(fx, fy, fWidth, fHeight);
	}

	return true;
}
////------------------------------------------------------------------------------ 
//bool SolarDialog::MoveWindowPos()
//{	
//	POINT pt;
//	WzID DlgID = GetDialogWZ()->GetDlgID();
//
//	if ( DlgPosInfoParser::Instance()->GetDlgPoint(DlgID, &pt) )
//	{
//		GetDialogWZ()->MoveWindowWZ( (float)pt.x, (float)pt.y );
//		return true;
//	}
//	
//	return false;	// DialogPosInfo.txt 에 정보가 없으면 FALSE 리턴
//}
//------------------------------------------------------------------------------ 
void SolarDialog::MoveDefaultWindowPos()
{
	POINT pt;

	if (DlgPosInfoParser::Instance()->GetDlgPoint(GetDialogWZ()->GetDlgID(), &pt))
	{
		GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
		return;
	}
	
	MoveCenter();	// DialogPosInfo.txt 에 정보가 없으면 중앙으로
}
//------------------------------------------------------------------------------ 
void SolarDialog::MoveCenter()
{
    RECT rc;
    GetDialogWZ()->GetDlgSize(&rc);

    int width = rc.right;
    int height = rc.bottom;
    int mw = width >> 1;
    int mh = height >> 1;

    POINT pt = g_pApplication->Get2DModeMapping();
    long pw = (pt.x >> 1) - mw;
    long ph = (pt.y >> 1) - mh;

    GetDialogWZ()->MoveWindowWZ((float)pw, (float)ph);
}
////------------------------------------------------------------------------------ 
//void SolarDialog::UpdatePosition()
//{
//    //MoveDefaultWindowPos();
//	//MoveWindowWZ(0.0f,0.0f);
//}

//------------------------------------------------------------------------------ 
DWORD SolarDialog::getOpenDlgSoundIndex()
{
	return UI_SOUND_DEFAULT_WIN_OPEN;
}


DWORD			
SolarDialog::getCloseDlgSoundIndex()
{
	return UI_SOUND_DEFAULT_WIN_CLOSE;
}

CControlWZ* SolarDialog::GetControlWZ(DWORD control_id) 
{ 
	if (m_pDialogWZ) 
	{
		return m_pDialogWZ->GetControlWZ(control_id); 
	}
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlButtonCheckWZ* SolarDialog::GetCheckButtonControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_CHECKBT) == true)
        {
            return static_cast<CCtrlButtonCheckWZ*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlButtonWZ* SolarDialog::GetButtonControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_BUTTON) == true)
        {
            return static_cast<CCtrlButtonWZ*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlListWZ* SolarDialog::GetListControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_LIST) == true)
        {
            return static_cast<CCtrlListWZ*>(control); 
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* SolarDialog::GetVScrollControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_VSCROLL) == true)
        {
            return static_cast<CCtrlVScrollWZ*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlStaticWZ* SolarDialog::GetStaticControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_TEXT) == true)
        {
            return static_cast<CCtrlStaticWZ*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlPictureWZ* SolarDialog::GetPictureControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_PICTURE) == true)
        {
            return static_cast<CCtrlPictureWZ*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlImageStaticWZ* SolarDialog::GetImageStaticControlWZ(DWORD control_id) 
{ 
    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_TEXTIMAGE) == true)
        {
            return static_cast<CCtrlImageStaticWZ*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlDxEditBox* SolarDialog::GetDxEditBoxControlWZ(DWORD control_id) 
{ 
    if (g_InterfaceManager.GetDialogManager()->IsUseDx() == false)
    {
        return NULL;
    }

    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        if ((control != NULL) && control->IsKind(CTRL_KIND_EDIT) == true)
        {
            return static_cast<CCtrlDxEditBox*>(control); 
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
CCtrlMultilineEditWZ* SolarDialog::GetMultilineEditControlWZ(DWORD control_id)
{ 
    if (g_InterfaceManager.GetDialogManager()->IsUseDx() == false)
    {
        return NULL;
    }

    if (m_pDialogWZ) 
    {
        CControlWZ* control = m_pDialogWZ->GetControlWZ(control_id);

        // 멀티라인도 에디터 콘트롤이다
        if ((control != NULL) && control->IsKind(CTRL_KIND_EDIT) == true)
        {
            if ((control->GetCtrlStyle() & CTRL_ES_MULTILINE) == CTRL_ES_MULTILINE)
            {
                return static_cast<CCtrlMultilineEditWZ*>(control); 
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
void SolarDialog::SetEnable(DWORD control_index, bool enable)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_index));
    if (control)
    {
        control->EnableWindowWZ(enable);
    }
}
//------------------------------------------------------------------------------ 
void SolarDialog::SetVisible(DWORD control_index, ENUM_STATEWZ state)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_index));
    if (control)
    {
        control->ShowWindowWZ(state);
    }
}
//------------------------------------------------------------------------------ 
bool SolarDialog::IsChecked(DWORD button_check_control_index)
{
    CCtrlButtonCheckWZ* button_check_control = GetCheckButtonControlWZ(GetControlID_byPosition(button_check_control_index));

    if (button_check_control != NULL)
    {
        return button_check_control->GetCheckState();
    }
    return false;
}
//------------------------------------------------------------------------------ 
void SolarDialog::SetChecked(DWORD button_check_control_index, bool is_check)
{
    CCtrlButtonCheckWZ* button_check_control = GetCheckButtonControlWZ(GetControlID_byPosition(button_check_control_index));

    if (button_check_control != NULL)
    {
        button_check_control->SetCheckState(is_check);
    }
}
//------------------------------------------------------------------------------ 
void SolarDialog::SetTextColor(DWORD text_control_index, COLORREF color)
{
    CCtrlStaticWZ* text_check_control = GetStaticControlWZ(GetControlID_byPosition(text_control_index));

    if (text_check_control != NULL)
    {
        text_check_control->SetTextColor(color);
    }
}

//------------------------------------------------------------------------------ 
void SolarDialog::SetTextInt(DWORD control_index, int val)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_index));

    if ((control != NULL) && (control->IsKind(CTRL_KIND_TEXT) || control->IsKind(CTRL_KIND_EDIT)))
    {
        TCHAR text[INTERFACE_STRING_LENGTH] = {NULL, };
        Snprintf(text, INTERFACE_STRING_LENGTH, "%d", val);

        control->SetTextWZ(text);
    }
}
//------------------------------------------------------------------------------ 
int SolarDialog::GetTextInt(DWORD control_index)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_index));

    if ((control != NULL) && (control->IsKind(CTRL_KIND_TEXT) || control->IsKind(CTRL_KIND_EDIT)))
    {
        TCHAR text[INTERFACE_STRING_LENGTH] = {NULL, };
        control->GetTextWZ(text, INTERFACE_STRING_LENGTH);

        return atoi(text);
    }
    return 0;
}


//------------------------------------------------------------------------------ 
void SolarDialog::ReleaseDialogTexture()
{	
	m_pDialogWZ->ReleaseDialogTexture();
}

//------------------------------------------------------------------------------
void SolarDialog::RegisterControlPosition(WzID control_id, DWORD position)
{
    control_position_map_.insert(std::make_pair(control_id, position));
    control_id_map_.insert(std::make_pair(position, control_id));
}
//------------------------------------------------------------------------------
DWORD SolarDialog::GetControlPosition_byID(WzID control_id)
{
    std::map<WzID, DWORD>::iterator iter = control_position_map_.find(control_id);
    if (iter != control_position_map_.end())
    {
        return (iter->second);
    }
    return 0xffffffff;
}
//------------------------------------------------------------------------------
WzID SolarDialog::GetControlID_byPosition(DWORD position)
{
    std::map<DWORD, WzID>::iterator iter = control_id_map_.find(position);
    if (iter != control_id_map_.end())
    {
        return (iter->second);
    }
    return 0xffffffff;
}

//------------------------------------------------------------------------------
CControlWZ* SolarDialog::GetControlWZ_byPosition(DWORD position)
{
    WzID control_id = GetControlID_byPosition(position);
    if (control_id == 0xffffffff)
    {
        return NULL;
    }

    return GetControlWZ(control_id);
}

//------------------------------------------------------------------------------
void SolarDialog::SetControlImage(WzID control_id, 
                                  CODETYPE image_code, 
                                  int image_src_x, 
                                  int image_src_y, 
                                  int image_height, 
                                  int image_width,
                                  BYTE image_alpha/* = 255*/)
{
    CControlWZ* control = GetControlWZ(control_id);
    if (control == NULL)
    {
        return;
    }

    // 이미지세팅 - 해당 이미지코드를 출력/0이면 초기화
    char* image_filename = NULL;
    RESOURCE_INFO* resource_info = GraphicResourceList::Instance()->GetItemInfo(image_code);
    if ((image_code == 0) ||
        (resource_info == NULL))
    {
        control->ReleaseImage();
    }
    else
    {
        image_filename = resource_info->szFilename;
    }

    SetControlImageFilename(control_id, 
        image_filename, 
        image_src_x, 
        image_src_y, 
        image_height, 
        image_width, 
        image_alpha);
}

//------------------------------------------------------------------------------ 
void SolarDialog::SetControlImageFilename(WzID control_id, 
                                          char* image_filename, 
                                          int image_src_x, 
                                          int image_src_y, 
                                          int image_height, 
                                          int image_width, 
                                          BYTE image_alpha/* = 255*/)
{
    CControlWZ* control = GetControlWZ(control_id);
    if (control == NULL)
    {
        return;
    }

    SI_TFORMAT control_image_format;
    g_InterfaceManager.GetDialogManager()->SetResourceFolderPath("");

    for (int draw_state = E_BASE; draw_state < (E_CLICK + 1); ++draw_state)
    {
        if (image_filename == NULL)
        {
            control_image_format.m_sImageState[draw_state].m_blIs = FALSE;
            control_image_format.m_sImageState[draw_state].m_color = RGBA(255,255,255,0);
        }
        else
        {
            control_image_format.m_sImageState[draw_state].m_blIs = TRUE;
            control_image_format.m_sImageState[draw_state].m_sImageName = image_filename;
            control_image_format.m_sImageState[draw_state].m_color = RGBA(255,255,255,image_alpha);
        }        
        control_image_format.m_sImageState[draw_state].m_sFRect.fTopX = static_cast<float>(image_src_x);
        control_image_format.m_sImageState[draw_state].m_sFRect.fTopY = static_cast<float>(image_src_y);
        control_image_format.m_sImageState[draw_state].m_sFRect.fHeight = static_cast<float>(image_height);
        control_image_format.m_sImageState[draw_state].m_sFRect.fWidth = static_cast<float>(image_width);
        control_image_format.m_sImageState[draw_state].m_nType = 1;
    }
    control->SetCtrlImageFormat(&control_image_format);

    g_InterfaceManager.GetDialogManager()->SetResourceFolderPath("Data\\Interface\\");
}
//------------------------------------------------------------------------------ 
VOID SolarDialog::OnUpdateSolarDialog()
{
    if (is_ani_playing())
    {
        BASE_DlgPosInfo* info = DlgPosInfoParser::Instance()->GetDlgPosInfo(GetDialogWZ()->GetDlgID());
        if (info != NULL)
        {
            POINT start_pot, dest_pos;
            DlgPosInfoParser::Instance()->GetAniStartPos(GetDialogWZ()->GetDlgID(), &start_pot);
            DlgPosInfoParser::Instance()->GetDlgPoint(GetDialogWZ()->GetDlgID(), &dest_pos);

            DWORD cur_tick = clock_function::GetTickCount();
            DWORD end_tick = ani_start_tick() + info->ani_until_millisecond();

            float ratio = float(cur_tick - ani_start_tick()) / (float)info->ani_until_millisecond();
            ratio = min(ratio, 1.0f);

            if (info->ani_speed_ratio() > 0.0f)
            {
                ratio = pow(ratio, info->ani_speed_ratio());
                ratio = min(ratio, 1.0f);
            }

            float x = start_pot.x + ((dest_pos.x - start_pot.x) * ratio);
            float y = start_pot.y + ((dest_pos.y - start_pot.y) * ratio);

            float transparency = info->ani_start_alpha() + ((1 - info->ani_start_alpha()) * ratio);
            transparency = max(ratio, 0.0f);
            transparency = min(ratio, 1.0f);

            MoveWindowWZ(x, y);

            GetDialogWZ()->SetTransparency(transparency);

            if (cur_tick >= end_tick)
            {
                set_is_ani_playing(false);
            }
        }
    }
}

bool SolarDialog::IsInRect(float left, float top, float width, float height)
{
    //uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    FRECT static_rect = {left, top, (left + width), (top + height)};

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    if (mouse_x > static_rect.left && mouse_x < static_rect.right
        && mouse_y > static_rect.top && mouse_y < static_rect.bottom)
    {
        //tooltip_manager->RegisterTooltip(string);
        return true;
    }
    return false;
}

void SolarDialog::SetBlinkGrid(DWORD control_id, bool is_blink)
{ 
    is_blink_grid_ = is_blink;
    grid_control_id_ = control_id;

    if (is_blink == false)
    {
        grid_timer_.Clear();
        return;
    }

    RECT grid_rect = {0,0,0,0};
    if (control_id == 0)
    {
        if (this->GetDialogWZ())
        {
            this->GetDialogWZ()->GetClientRect(&grid_rect);
            ControlSize(grid_rect);
        }
    }
    else
    {
        CControlWZ* control_ptr = GetControlWZ(control_id);
        if (control_ptr)
        {
            ControlSize(control_ptr->GetSize());
        }
    }
    grid_timer_.SetTimer(700);
}

void SolarDialog::ControlSize(RECT control_rect)
{
    const float kWIDHT = 11.0f;
    const float kHEIGHT = 11.0f;
    float x_postion = static_cast<float>(control_rect.left) - GetDialogWZ()->GetDlgX();
    float y_postion = static_cast<float>(control_rect.top) - GetDialogWZ()->GetDlgY();
    float width = static_cast<float>(control_rect.right);// - control_rect.left);
    float height = static_cast<float>(control_rect.bottom);// - control_rect.top);
    if (grid_control_id_ == 0)
    {
        width = width - control_rect.left;
        height = height - control_rect.top;
    }
    float center_width = width - (kWIDHT * 2);
    float center_height = height - (kHEIGHT * 2);
    float half_width = (width/2);
    float half_height = (width/2);

    if (half_width <= kWIDHT)
    {
        control_rectangles_[0].left = x_postion;    //┏─┐
        control_rectangles_[0].right = half_width;  //└─┘

        control_rectangles_[1].left = x_postion;    //┌─┐
        control_rectangles_[1].right = half_width;  //┃  │
                                                    //└─┘
        control_rectangles_[2].left = x_postion;    //┌─┐
        control_rectangles_[2].right = half_width;  //┗─┘

        control_rectangles_[3].left = x_postion + half_width;   //┌━┐
        control_rectangles_[3].right = 0;                       //└─┘

        control_rectangles_[4].left = x_postion + half_width;   //┌─┐
        control_rectangles_[4].right = 0;                       //└━┘

        control_rectangles_[5].left = x_postion + half_width;   //┌─┓
        control_rectangles_[5].right = half_width;              //└─┘

        control_rectangles_[6].left = x_postion + half_width;   //┌─┐
        control_rectangles_[6].right = half_width;              //│  ┃
                                                                //└─┘
        control_rectangles_[7].left = x_postion + half_width;   //┌─┐
        control_rectangles_[7].right = half_width;              //└─┛
    }
    else
    {
        control_rectangles_[0].left = x_postion;    //┏─┐
        control_rectangles_[0].right = kWIDHT;      //└─┘

        control_rectangles_[1].left = x_postion;    //┌─┐
        control_rectangles_[1].right = kWIDHT;      //┃  │
                                                    //└─┘
        control_rectangles_[2].left = x_postion;    //┌─┐
        control_rectangles_[2].right = kWIDHT;      //┗─┘

        control_rectangles_[3].left = x_postion + kWIDHT;   //┌━┐
        control_rectangles_[3].right = center_width;        //└─┘
        control_rectangles_[4].left = x_postion + kWIDHT;   //┌─┐
        control_rectangles_[4].right = center_width;        //└━┘

        control_rectangles_[5].left = x_postion + kWIDHT + center_width;    //┌─┓
        control_rectangles_[5].right = kWIDHT;                              //└─┘

        control_rectangles_[6].left = x_postion + kWIDHT + center_width;    //┌─┐
        control_rectangles_[6].right = kWIDHT;                              //│  ┃
                                                                            //└─┘
        control_rectangles_[7].left = x_postion + kWIDHT + center_width;    //┌─┐
        control_rectangles_[7].right = kWIDHT;                              //└─┛
    }

    if (half_height <= kHEIGHT)
    {
        control_rectangles_[0].top = y_postion;     //┏─┐
        control_rectangles_[0].bottom = half_height;//└─┘
        control_rectangles_[3].top = y_postion;     //┌━┐
        control_rectangles_[3].bottom = half_height;//└─┘
        control_rectangles_[5].top = y_postion;     //┌─┓
        control_rectangles_[5].bottom = half_height;//└─┘

        control_rectangles_[1].top = y_postion + half_height;   //┌─┐
        control_rectangles_[1].bottom = 0;                      //┃  │
                                                                //└─┘
        control_rectangles_[6].top = y_postion + half_height;   //┌─┐
        control_rectangles_[6].bottom = 0;                      //│  ┃
                                                                //└─┘
        control_rectangles_[2].top = y_postion + half_height;   //┌─┐
        control_rectangles_[2].bottom = half_height;            //┗─┘
        control_rectangles_[4].top = y_postion + half_height;   //┌─┐
        control_rectangles_[4].bottom = half_height;            //└━┘
        control_rectangles_[7].top = y_postion + half_height;   //┌─┐
        control_rectangles_[7].bottom = half_height;            //└─┛
    }
    else
    {
        control_rectangles_[0].top = y_postion;     //┏─┐
        control_rectangles_[0].bottom = kHEIGHT;    //└─┘
        control_rectangles_[3].top = y_postion;     //┌━┐
        control_rectangles_[3].bottom = kHEIGHT;    //└─┘
        control_rectangles_[5].top = y_postion;     //┌─┓
        control_rectangles_[5].bottom = kHEIGHT;    //└─┘

        control_rectangles_[1].top = y_postion + kHEIGHT;   //┌─┐
        control_rectangles_[1].bottom = center_height;      //┃  │
                                                            //└─┘
        control_rectangles_[6].top = y_postion + kHEIGHT;   //┌─┐
        control_rectangles_[6].bottom = center_height;      //│  ┃
                                                            //└─┘
        control_rectangles_[2].top = y_postion + center_height + kHEIGHT; //┌─┐
        control_rectangles_[2].bottom = kHEIGHT;                          //┗─┘
        control_rectangles_[4].top = y_postion + center_height + kHEIGHT; //┌─┐
        control_rectangles_[4].bottom = kHEIGHT;                          //└━┘
        control_rectangles_[7].top = y_postion + center_height + kHEIGHT; //┌─┐
        control_rectangles_[7].bottom = kHEIGHT;                          //└─┛
    }
}
