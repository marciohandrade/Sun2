#include "SunClientPrecompiledHeader.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemCube.h"

#include "ItemUnitRenderer.h"
#include "ItemManager.h"
#include "interfacemanager.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "globalfunc.h"
#include "dummyitemSlot.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "CubeSystem/CubeSystemParser.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "Application.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "../SoundEffect.h"
#include "ItemInfoParser.h"
#include "uiToolTipMan/uiToolTipMan.h"

class uiSCItemSlot;
//------------------------------------------------------------------------------
uiItemCube::uiItemCube(InterfaceManager* ui_man): uiBase(ui_man),
item_unit_renderer_(NULL)
{
	RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);
	RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
	RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
	RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
	RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
	RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
	RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
	RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
	RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
	RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
	RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
	RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
	RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);

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
	RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
	RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
	RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
	RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
	RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
	RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
	RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
	RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
	RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
	RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
	RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031);
	RegisterControlPosition(StrToWzID("B032"), kControl_Button_B032);
	RegisterControlPosition(StrToWzID("B033"), kControl_Button_B033);
	RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
	RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
	RegisterControlPosition(StrToWzID("L003"), kControl_List_L003);
	RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);    
	RegisterControlPosition(StrToWzID("L004"), kControl_List_L004);
	RegisterControlPosition(StrToWzID("L005"), kControl_List_L005);
	RegisterControlPosition(StrToWzID("L006"), kControl_List_L006);
	RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
	RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
	RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
	RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
	RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
	RegisterControlPosition(StrToWzID("P500"), kControl_Picture_P500);
	RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
	RegisterControlPosition(StrToWzID("S000"), kControl_Static_S000);

	RegisterControlPosition(StrToWzID("P444"), kControl_Picture_P444);
	RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
	RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
	RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
	RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
	RegisterControlPosition(StrToWzID("P999"), kControl_Picture_P999);

    // 2013.04.03 / 홍성현 / 다이얼로그 전체가 두개로 나뉘어서 각각 컨트롤로 되어 있어서 등록 시킨다.
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
}
//------------------------------------------------------------------------------ 
uiItemCube::~uiItemCube(void)
{
	this->Release();

	SAFE_RELEASENDELETE(item_unit_renderer_);
	CDialogWZ* dialog_ptr = GetDialogWZ();
	if (dialog_ptr != NULL)
	{
		STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
		intersect_list.clear();
	}
}
//------------------------------------------------------------------------------
void uiItemCube::Init(CDrawBase* draw_base)
{
	uiBase::Init(draw_base);

	cube_lock_ = false;

	// 슬롯 초기화
	DummySlotContainer::Init(kControl_Button_B008+1);   

	// 랜더러 초기화
	assert(!item_unit_renderer_);
	item_unit_renderer_ = new ItemUnitRenderer;
	item_unit_renderer_->Init(g_pSunRenderer, SCSlotContainer::GetMaxSlotNum());

	for (int i = 0; i < kControl_Button_B008+1; ++i)
	{
		item_inventory_position[i] = INVALID_POSTYPE_VALUE;
	}

	// 애니메이션용 텍스쳐 초기화
	InitTextures();

	save_inventory_item_info.Clear();
	//is_composite_click_ = true;
	cube_position_ = -1;

	animate_now_tick_ = 0;
	animate_start_tick_ = 0;
	animate_end_tick_ = 0;
	list_current_focus_ = 0;
	list_size_ = 0;
	tooltip_string_code_ = 0;
	is_show_cube_history_ = false;

	CCtrlListWZ* background_control_ptr = 
		static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Description_Background)));

	if (background_control_ptr)
	{
		default_list_background_height_ = 
			background_control_ptr->GetSizeRect().bottom - background_control_ptr->GetSizeRect().top;
	}

	CControlWZ* control_ptr = 
		GetControlWZ(GetControlID_byPosition(kControl_List_L000));

	if (control_ptr)
	{
		control_ptr->SetDiscardMessage(true);
	}

	control_ptr = 
		GetControlWZ(GetControlID_byPosition(kControl_List_L004));

	if (control_ptr)
	{
		control_ptr->SetDiscardMessage(true);
	}

	control_ptr = 
		GetControlWZ(GetControlID_byPosition(kControl_List_L001));

	if (control_ptr)
	{
		control_ptr->SetDiscardMessage(true);
	}

	control_ptr = 
		GetControlWZ(GetControlID_byPosition(kControl_List_L002));

	if (control_ptr)
	{
		control_ptr->SetDiscardMessage(true);
	}

	control_ptr = 
		GetControlWZ(GetControlID_byPosition(kControl_List_L003));

	if (control_ptr)
	{
		control_ptr->SetDiscardMessage(true);
	}

	control_ptr = 
		GetControlWZ(GetControlID_byPosition(kControl_List_L006));

	if (control_ptr)
	{
		control_ptr->SetDiscardMessage(true);
	}

    // 2013.04.03 / 홍성현 / 컨트롤에 가려져서 큐브 이동영역이 제대로 작동하지 않아 컨트롤을 Discard 시킨다.
    control_ptr = 
        GetControlWZ(GetControlID_byPosition(kControl_Picture_P006));

    if (control_ptr)
    {
        control_ptr->SetDiscardMessage(true);
    }
    // 2013.04.03 / 홍성현 / 컨트롤에 가려져서 합성 목록 이동영역이 제대로 작동하지 않아 컨트롤을 Discard 시킨다.
    control_ptr = 
        GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));

    if (control_ptr)
    {
        control_ptr->SetDiscardMessage(true);
    }

#ifdef _CN_000000_20130729_NOT_USE_CUBE
#else
    control_ptr = 
        GetControlWZ(GetControlID_byPosition(kControl_Button_B010));

    if(control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = 
        GetControlWZ(GetControlID_byPosition(kControl_Button_B011));
    
    if(control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
#endif  
}
//------------------------------------------------------------------------------
void uiItemCube::InitTextures()
{
	// 애니메이션 텍스쳐 2장 로드
	effect_texture_[kType_Composite] = g_pSunRenderer->x_pManagerTexture->LoadTexture(
		"Data\\Interface\\etc\\Cube_eff01.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	effect_texture_[kType_Decomposite] = g_pSunRenderer->x_pManagerTexture->LoadTexture(
		"Data\\Interface\\etc\\Cube_eff02.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	// 애니메이션 텍스쳐 위치 (4프레임)
	texture_rectangles_[0].left = 0.0f;
	texture_rectangles_[0].top = 0.0f;
	texture_rectangles_[0].right = 256.0f;
	texture_rectangles_[0].bottom = 256.0f;

	texture_rectangles_[1].left = 256.0f;
	texture_rectangles_[1].top = 0.0f;
	texture_rectangles_[1].right = 256.0f;
	texture_rectangles_[1].bottom = 256.0f;

	texture_rectangles_[2].left = 0.0f;
	texture_rectangles_[2].top = 256.0f;
	texture_rectangles_[2].right = 256.0f;
	texture_rectangles_[2].bottom = 256.0f;

	texture_rectangles_[3].left = 256.0f;
	texture_rectangles_[3].top = 256.0f;
	texture_rectangles_[3].right = 256.0f;
	texture_rectangles_[3].bottom = 256.0f;

	// 애니메이션 시간 지정
	animate_frame_count_ = 0;
	animate_effect_type_ = kType_Composite;
}
//------------------------------------------------------------------------------
void uiItemCube::ResetAnimation()
{
	animate_frame_count_ = 0;
}
//------------------------------------------------------------------------------
void uiItemCube::Release()
{
	for(int i = 0; i < kType_Composite_Max; ++i)
	{
		if(effect_texture_ != INVALID_HANDLE_VALUE)
		{
			g_pSunRenderer->x_pManagerTexture->DestroyTexture(effect_texture_[i]);
			effect_texture_[i] = INVALID_HANDLE_VALUE;
		}
	}
}

//------------------------------------------------------------------------------
uiItemCompositeMan* uiItemCube::GetManager()
{
	uiItemCompositeMan* ui_item_composite_manager =
		static_cast<uiItemCompositeMan*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_ITEM_COMPOSITE));
	assert (ui_item_composite_manager);
	return ui_item_composite_manager;
}
//------------------------------------------------------------------------------
void uiItemCube::MessageProc(SI_MESSAGE* message)
{
	if (IsLock())
		return;

	if (GetManager() == NULL)
		return;

	static BOOL double_clicked = FALSE;

	switch (message->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
        {
		    if (double_clicked && double_clicked == FALSE)
		    {
			    double_clicked = FALSE;
		    }
            click_move_enable_ = false;
            g_InterfaceManager.MouseMoveLock(false);

		    OnLButtonClick(message);
        }
		break;
    case RT_MSG_LBUTTONDOWN:
    {
        if(GetDialogWZ())
        {
            RECT move_rect = GetDialogWZ()->GetMoveRECT();
            move_rect.left += (long)GetDialogWZ()->GetDlgX();
            move_rect.top += (long)GetDialogWZ()->GetDlgY();
            move_rect.right += move_rect.left;
            move_rect.bottom += move_rect.top;

            POINT point;
            point.x = Mouse::Instance()->GetMouseX();
            point.y = Mouse::Instance()->GetMouseY();
            if(DEF_IntersectRect(move_rect,point))
            {
                click_move_enable_ = true;
                g_InterfaceManager.MouseMoveLock(true);
            }
        }
    }
        break;
	case RT_MSG_LBUTTONDCLICK:
		double_clicked = TRUE;
		if (cube_lock_ == false)
		{
			OnDoubleButtonClick(message);
		}
		break;
	case RT_MSG_SB_LINETOP:
	case RT_MSG_SB_LINEBOTTOM:
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
	case RT_MSG_SB_THUMBTRACK:
		{
			OnScroll(message);
			SetListItems();
		}
		break;
	default:
		SolarDialog::MessageProc(message);
		break;
	}
}

//----------------------------------------------------------------------------
void uiItemCube::MouseOver_MsgProc(SI_MESSAGE* message)
{
	switch (message->eResultMsg) 
	{
	case RT_MSG_MOUSEOVER:
		{
			UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
			if (control_pos >= kControl_Button_Slot0 && control_pos < (kControl_Button_Slot8+1))
			{
				if (!IsEmpty(control_pos))
				{
					DummyItemSlot& rSlot = (DummyItemSlot&)GetSlot(control_pos);
					assert( rSlot.GetOrgSlot());

					if(rSlot.GetOrgSlot())
					{
						m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
						m_MouseOverObject.pSlot = rSlot.GetOrgSlot();
					}
				}
				else
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				}
			}
			else
			{
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
			}

			switch (control_pos)
			{
			case kControl_Button_Lock1:
			case kControl_Button_Lock2:
			case kControl_Button_Lock3:
			case kControl_Button_Lock4:
			case kControl_Button_Lock5:
				tooltip_string_code_ = 2244;
				break;
			case kControl_Button_UnLock1:
			case kControl_Button_UnLock2:
			case kControl_Button_UnLock3:
			case kControl_Button_UnLock4:
			case kControl_Button_UnLock5:
				tooltip_string_code_ = 2255;
				break;
			default:
				tooltip_string_code_ = 0;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
void uiItemCube::UpdateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	bool is_mouse_over = false;
	int loop_end = kControl_Max_Size;
	for (POSTYPE position = 0; position < loop_end; ++position)
	{
		CControlWZ* control = GetControlWZ(GetControlID_byPosition(position));
		if(control)
		{
			RECT control_rect = control->GetSizeRect();
			if (control_rect.left <= iMouseX && control_rect.right >= iMouseX)
			{
				if (control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
				{
					is_mouse_over = true;
					break;
				}
			}
		}
	}

	if (!is_mouse_over)
	{
		tooltip_string_code_ = 0;
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
	else
	{
		if (tooltip_string_code_ > 0)
		{
			uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
			if (tooltip_manager)
			{
				tooltip_manager->RegisterTooltipForWareNInventoryLock(tooltip_string_code_);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
}
//------------------------------------------------------------------------------
void uiItemCube::OnShowWindow( BOOL val )
{
	if (val)
	{
		cube_lock_ = false;
		ResetAnimation();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Cube;
		msg.wParam=IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);

		// 사운드 출력
		g_SoundEffect.PlayUI(UI_SOUND_CUBE_OPEN);
		ShowCubeLists(false);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Cube);
		DeleteAllSlot();//아이템 모두 해제
		ResetAnimation();
		SendPacketListLock();// 패킷 락 정보도 전송

		// 사운드 출력
		g_SoundEffect.PlayUI(UI_SOUND_CUBE_CLOSE);
		if (cube_position_ != INVALID_POSTYPE_VALUE)
		{
			ItemManager::Instance()->SetItemLock(SI_INVENTORY, cube_position_, false);
			cube_position_ = INVALID_POSTYPE_VALUE;
		}
	}
}
//------------------------------------------------------------------------------
bool uiItemCube::ShowItemVisible()
{
	DWORD gep_tick = 0;
	if (animate_end_tick_ < animate_start_tick_)
	{
		return true;
	}
	gep_tick = kMax_Animate_Tick - (animate_end_tick_ - animate_now_tick_);

	// 아이템이 깜빡 깜빡
	if (gep_tick >= 2500 && gep_tick < 2700)
	{
		return false;
	}
	else if (gep_tick >= 2900 && gep_tick < 3100)
	{
		return false;
	}
	else if (gep_tick >= 3300 && gep_tick < 3500)
	{
		return false;
	}
	else if (gep_tick >= 3700 && gep_tick < 3900)
	{
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
void uiItemCube::OnRenderSolarDialog()
{
	if (!item_unit_renderer_)
	{
		return;
	}

	if (item_unit_renderer_ && ShowItemVisible())
	{
		item_unit_renderer_->Render();
	}

	if (animate_start_tick_ == 0)//이펙트 중에는 툴팁 표시 안함
	{
		if (this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX &&
			m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT)
		{
			if (m_MouseOverObject.pSlot && m_MouseOverObject.pSlot->GetCode())
			{
				uiToolTipMan* tooltip_manager = 
					static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
				if (tooltip_manager != NULL)
				{
					tooltip_manager->RegisterTooltipItem(m_MouseOverObject.pSlot, FALSE);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
				}
			}
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE pos = kControl_Button_Slot0; pos < (kControl_Button_Slot8+1); ++pos)
	{
		CControlWZ* pControl = GetControlWZ(GetControlID_byPosition(pos));

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if (!bMouseOver)
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}

	animate_now_tick_ = clock_function::GetTickCount();
	if (animate_start_tick_ != 0)
	{
		RECT rc;
		POINT pt = g_pApplication->Get2DModeMapping();
		SetRect(&rc, 0, 0, pt.x, pt.y);
		g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0, 0, static_cast<float>(rc.right), static_cast<float>(rc.bottom));
		GetDialogWZ()->OnDraw(true);

		if (item_unit_renderer_ && ShowItemVisible()) //화면 어두운 상태에서는 다시 랜더 해야만 나옴
		{
			item_unit_renderer_->Render();
		}
		AnimateEffect(animate_effect_type_);
	}
	else
	{
		cube_lock_ = false;
	}

	

	CControlWZ* pControl = GetControlWZ(GetControlID_byPosition(kControl_Button_B011));
	if(pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH];
				memset(szMessage , 0 , sizeof( szMessage ) );
				g_InterfaceManager.GetInterfaceString(2242, szMessage, INTERFACE_STRING_LENGTH);

				uiToolTipMan* tooltip_manager = 
					static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
				if (tooltip_manager != NULL)
				{
					tooltip_manager->RegisterTooltip(szMessage);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
				}
			}
		}
	}
	
}
//------------------------------------------------------------------------------
WzColor uiItemCube::GetAlphaValue()
{
	DWORD gep_tick = 0;
	int alpha = 0x00000000;
	if (animate_end_tick_ < animate_start_tick_)
	{
		return 0x00000000;
	}
	gep_tick = kMax_Animate_Tick - (animate_end_tick_ - animate_now_tick_);

	// 이펙트 알파
	if (gep_tick < 1000)
	{
		float float_alpha = gep_tick / 1000.0f;
		alpha = (int)(float_alpha * 255);
		if (alpha > 255)
		{
			alpha = 255;
		}
	}
	else if (gep_tick < 4000 && gep_tick >= 2500)
	{
		gep_tick -= 2500;
		float float_alpha = ((1500.0f - gep_tick) / 1500.0f);
		alpha = (int)(float_alpha * 255);
		if (alpha > 255)
		{
			alpha = 255;
		}
	}
	else if (gep_tick >= 4000)
	{
		alpha = 0;
	}
	else
	{
		return 0xffffffff;
	}
	return (WzColor_RGBA(255, 255, 255, alpha));
}
//------------------------------------------------------------------------------
void uiItemCube::AnimateEffect(int type)
{
	static WzColor colors[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
	static FRECT img_rect = {0,};
	img_rect.left = 
		(float)GetControlWZ(GetControlID_byPosition(kControl_Button_B004))->GetSizeRect().left - 80;
	img_rect.top = 
		(float)GetControlWZ(GetControlID_byPosition(kControl_Button_B004))->GetSizeRect().top - 80;

	TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};

	if(effect_texture_ != INVALID_HANDLE_VALUE)
	{
		int count = animate_frame_count_;
		if (count >= 4)
		{
			count = 0;
			animate_frame_count_ = 0;
		}

		WzColor color = GetAlphaValue();
		for (int i = 0; i < 4; ++i)
		{
			colors[i] = color;
		}
		g_pSunRenderer->RenderTexture(effect_texture_[type], img_rect.left, img_rect.top, 
			192.0f, 192.0f, texture_rectangles_[count].left, texture_rectangles_[count].top, 
			texture_rectangles_[count].right, texture_rectangles_[count].bottom, colors);

		//++animate_frame_count_;        
		animate_frame_count_ = ((animate_now_tick_ - animate_start_tick_)/50)%4;
	}
	if (animate_start_tick_ != 0 && animate_now_tick_ >= animate_end_tick_)
	{    
		animate_start_tick_ = 0;
		if (save_inventory_item_info.m_InvenCount == 0)
		{
			if (animate_effect_type_ == kType_Composite)
			{   // 3409: 합성에 성공했지만, 어떤 아이템도 생성되지 않았습니다.
				g_InterfaceManager.GetInterfaceString(3409, message_buffer,INTERFACE_STRING_LENGTH);
			}
			else
			{   // 3410: 분해에 성공했지만, 어떤 아이템도 생성되지 않았습니다.
				g_InterfaceManager.GetInterfaceString(3410, message_buffer,INTERFACE_STRING_LENGTH);
			}
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);
		}
		else
		{
			if (animate_effect_type_ == kType_Composite)
			{   // 3406: 합성에 성공했습니다.
				g_InterfaceManager.GetInterfaceString(3406, message_buffer,INTERFACE_STRING_LENGTH);
				SendPacketGetList();
			}
			else
			{   // 3410: 분해에 성공했습니다.
				g_InterfaceManager.GetInterfaceString(3407, message_buffer,INTERFACE_STRING_LENGTH);
			}
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);
			ItemManager::Instance()->InsertInventoryTotalInfo(CUBE, save_inventory_item_info);
		}
		save_inventory_item_info.Clear();
		DeleteAllSlot();
		g_InterfaceManager.UnLockDlgs();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Cube;
		msg.wParam=IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
}
//------------------------------------------------------------------------------
bool uiItemCube::OnLButtonClick(SI_MESSAGE* message)
{
	bool result = true;
	UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
	switch (control_pos)
	{
	case kControl_Button_Slot0:
	case kControl_Button_Slot1:
	case kControl_Button_Slot2:
	case kControl_Button_Slot3:
	case kControl_Button_Slot4:
	case kControl_Button_Slot5:
	case kControl_Button_Slot6:
	case kControl_Button_Slot7:
	case kControl_Button_Slot8:
		{
			if (cube_lock_ == true)
			{
				return false;
			}
			if (MouseHandler::Instance()->IsExistItemAtHand() == TRUE)
			{
				MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();                
				CControlWZ* pControl = GetControlWZ(GetControlID_byPosition(control_pos));
				if (pControl)
				{
					RECT rcSlot = pControl->GetSizeRect();
					MouseHandler::Instance()->ItemCubeTransection(SI_CUBE, control_pos, &rcSlot);
				}
			}
			else
			{
				CControlWZ* pControl = GetControlWZ(GetControlID_byPosition(control_pos));
				if (pControl)
				{
					SCSlotContainer* to_container = ItemManager::Instance()->GetContainer(SI_CUBE); 
					// 비어있지 않으면
					if (!to_container->IsEmpty(control_pos))
					{
						TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
						// 3402: 큐브에 등록되어 있는 아이템을 더블클릭하면 다시 인벤토리로 이동시킬 수 있습니다.
						g_InterfaceManager.GetInterfaceString(3402, message_buffer,INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);
					}                
				}
			}
		}
		break;
	case kControl_Button_Composite:
		{
			if (cube_lock_ == false && ExecuteCube(true))
			{
				cube_lock_ = true;  
				animate_effect_type_ = kType_Composite;
			}
		}
		break;
	case kControl_Button_DeComposite:
		{
			if (cube_lock_ == false && ExecuteCube(false))
			{
				cube_lock_ = true;
				animate_effect_type_ = kType_Decomposite;
			}
		}
		break;
	case kControl_Button_Exit:
		{
			if (cube_lock_ == false)
			{
				this->ShowInterface(FALSE);
			}
		}
		break;
	case kControl_Button_Open:
		{
			SendPacketGetList();
			ShowCubeLists(true);
		}
		break;
	case kControl_Button_ListClose:
		{
			SendPacketListLock();
			ShowCubeLists(false);
		}
		break;
		//kControl_Text_Title
	case kControl_Button_Lock1:
	case kControl_Button_Lock2:
	case kControl_Button_Lock3:
	case kControl_Button_Lock4:
	case kControl_Button_Lock5:
		{
			SetListLock(control_pos - kControl_Button_Lock1, false);
		}
		break;
	case kControl_Button_UnLock1:
	case kControl_Button_UnLock2:
	case kControl_Button_UnLock3:
	case kControl_Button_UnLock4:
	case kControl_Button_UnLock5:
		{
			SetListLock(control_pos - kControl_Button_UnLock1, true);
		}
		break;
	case kControl_Button_List1:
	case kControl_Button_List2:
	case kControl_Button_List3:
	case kControl_Button_List4:
	case kControl_Button_List5:
		{
			OpenDetailList(control_pos - kControl_Button_List1);
		}
		break;
	case kControl_List_L000:
	case kControl_List_L001:
	case kControl_List_L002:
	case kControl_List_L003:
	case kControl_List_L004:
		{
			OpenDetailList(control_pos - kControl_List_L000);
		}
		break;
	case kControl_Select_Background:
	case kControl_Slot_Background:
	case kControl_Description_Background:
	case kControl_List_Background:
	case kControl_List_L005:
	case kControl_List_L006:
		{
			ShowSelectionControls(false);
		}
	default:
		{
		}
		break;
	}
	return result;
}
//------------------------------------------------------------------------------
void uiItemCube::SendPacketListLock()
{
	MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD sync;
	int size = MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD::kMaxConditionListSize;
	int condition_index = 0;
	for (int i = 0; i < list_size_; ++i)
	{
		if ((modify_condition_list_[i]).history_id != 0)
		{
			sync.modify_condition_list[condition_index] = modify_condition_list_[i];
			++condition_index;
		}
	}
	if (condition_index > 0)
	{
		sync.condition_list_size = condition_index;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sync.GetSize());
	}
}
//------------------------------------------------------------------------------
void uiItemCube::SendPacketGetList()
{
#ifdef _CN_000000_20130729_NOT_USE_CUBE
#else
	MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN sync;
	sync.request_type = MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest;// 요청 종류
	sync.is_composition = true; // 합성 여부*/
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
#endif
}
//------------------------------------------------------------------------------
void uiItemCube::SetListLock(int index, bool is_lock)
{
	int current_position = list_current_focus_ + index;
	if (current_position < MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD::kMaxConditionListSize)
	{
		modify_condition_list_[current_position].is_memorize = is_lock;
		SetListLocking(index, list_current_focus_ + index);
	}
}
//------------------------------------------------------------------------------
void uiItemCube::OpenDetailList(int index)
{
	ShowSelectionControls(true);
	int selection_gap = kControl_Button_B023 - kControl_Button_B017;
	SetItemIcon(selection_gap, list_current_focus_ + index);
	SetListItem(selection_gap, list_current_focus_ + index);
	SetDetailListItem(list_current_focus_ + index);
}
//------------------------------------------------------------------------------
bool uiItemCube::OnDoubleButtonClick(SI_MESSAGE* message)
{
	bool result = true;
	UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
	switch (control_pos)
	{
	case kControl_Button_Slot0:
	case kControl_Button_Slot1:
	case kControl_Button_Slot2:
	case kControl_Button_Slot3:
	case kControl_Button_Slot4:
	case kControl_Button_Slot5:
	case kControl_Button_Slot6:
	case kControl_Button_Slot7:
	case kControl_Button_Slot8:
		{
			if (MouseHandler::Instance()->IsExistItemAtHand() != TRUE)
			{
				this->DeleteSlot(control_pos, NULL);
			}
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
bool uiItemCube::OnScroll(SI_MESSAGE* message)
{
	UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));

	if (kControl_List_Scroll != control_pos)
	{
		return false;
	}

	CCtrlVScrollWZ* vscroll_control_ptr = 
		static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_Scroll)));
	switch (message->eResultMsg)
	{
	case RT_MSG_SB_LINETOP:
		{
			//.윗쪽 화살표를 눌렀다.
			int index = vscroll_control_ptr->GetScrollPosWZ();
			--index;
			index = max(0, index);
			vscroll_control_ptr->SetScrollPosWZ(index);
			list_current_focus_ = index;
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.아랫쪽 화살표를 눌렀다
			int index = vscroll_control_ptr->GetScrollPosWZ();
			++index;
			int scroll_range = (list_size_ - (kControl_List_L004 - kControl_List_L000 + 1));
			scroll_range = (scroll_range > 0)? scroll_range: 0;
			index = min(scroll_range, index);
			vscroll_control_ptr->SetScrollPosWZ(index);
			list_current_focus_ = index;
		}
		break;
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
	case RT_MSG_SB_THUMBTRACK:
		{
			//.썸을 드래그 하고 있다.
			int index = vscroll_control_ptr->GetScrollPosWZ();

			int scroll_range = (list_size_ - (kControl_List_L004 - kControl_List_L000 + 1));
			scroll_range = (scroll_range > 0)? scroll_range: 0;

			if (scroll_range < 0)
			{
				list_current_focus_ = 0;
				vscroll_control_ptr->SetScrollPosWZ(0);
			}
			else
			{
				list_current_focus_ = index;
				//vscroll_control_ptr->SetScrollPosWZ(index);
			}
		}
		break;
	default:
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
SCSlotContainer* uiItemCube::GetSlotContainer()
{
	return static_cast<SCSlotContainer*>(this);
}
//------------------------------------------------------------------------------ 
RC::eSLOT_INSERT_RESULT uiItemCube::InsertSlot(POSTYPE at_pos, SCSlot& slot)
{
	if (item_unit_renderer_ == NULL)
	{
		assert(item_unit_renderer_);
		return RC::RC_INSERT_SLOT_FAILED;
	}
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = DummySlotContainer::InsertSlot(at_pos, slot);
	DummyItemSlot & dummy_item_slot = (DummyItemSlot&)slot;

	// 락킹을 걸어라.
	SCSlotContainer* pFromContainer = 
		ItemManager::Instance()->GetContainer(dummy_item_slot.GetFromContainerIdx());
	if (pFromContainer)
	{
		SCItemSlot& rFromItemSlot = 
			(SCItemSlot &)pFromContainer->GetSlot(dummy_item_slot.GetFromPosition());

		rFromItemSlot.SetLock( TRUE );
	}

	/// 인벤토리
	CControlWZ* control = GetControlWZ(GetControlID_byPosition(at_pos));
	RECT rect = control->GetSizeRect();

	DummyItemSlot& real_slot = (DummyItemSlot&)(GetSlot(at_pos));
	assert(real_slot.GetOrgSlot());

	item_unit_renderer_->AddItemUnit(real_slot.GetSerial(), real_slot.GetOrgSlot()->GetCode(), 
		&rect, &real_slot, control);

	if (at_pos >= 0 && at_pos < kControl_Button_B008+1)
	{
		item_inventory_position[at_pos] = real_slot.GetOrgSlot()->GetPos();
	}

	return eInsertSlotResult;
}
//------------------------------------------------------------------------------ 
BOOL uiItemCube::DeleteSlot(POSTYPE at_pos, SCSlot* slotOut)
{
	if (item_unit_renderer_ == NULL)
	{
		assert(item_unit_renderer_);
		return FALSE;
	}
	DummyItemSlot& delete_dummy_slot = (DummyItemSlot&)GetSlot(at_pos);

	// 락킹을 풀어라.
	SCSlotContainer* from_container = 
		ItemManager::Instance()->GetContainer(delete_dummy_slot.GetFromContainerIdx());
	if (from_container != NULL)
	{
		SCItemSlot& from_item_slot = 
			(SCItemSlot&)from_container->GetSlot(delete_dummy_slot.GetFromPosition());

		from_item_slot.SetLock( FALSE );
	}

	if (delete_dummy_slot.GetOrgSlot())
	{
		item_unit_renderer_->RemoveItemUnit(delete_dummy_slot.GetSerial());
		if (at_pos >= 0 && at_pos < kControl_Button_B008+1)
		{
			item_inventory_position[at_pos] = 0;
		}
	}

	item_inventory_position[at_pos] = INVALID_POSTYPE_VALUE;

	return DummySlotContainer::DeleteSlot(at_pos, slotOut);
}
//------------------------------------------------------------------------------
void uiItemCube::DeleteAllSlot()
{
	if (item_unit_renderer_ == NULL)
	{
		assert(item_unit_renderer_);
		return;
	}
	for (int slot_pos = kControl_Button_Slot0; slot_pos <= kControl_Button_Slot8; ++slot_pos)
	{
		this->DeleteSlot(slot_pos, NULL);
	}
}
//------------------------------------------------------------------------------
bool SortByDate(const ItemCompositionOrDecompositionHistory &first, const ItemCompositionOrDecompositionHistory &second)
{
	return (first.history_date_time > second.history_date_time);
}
//------------------------------------------------------------------------------
void uiItemCube::NetworkProc(MSG_BASE* message)
{
	if (message->m_byCategory == CG_ITEM)
	{
		switch (message->m_byProtocol)
		{
		case CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK:
			{
				MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK* cube_history_packet =
					static_cast<MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK*>(message);
				switch(cube_history_packet->result_code)
				{
				case RC::RC_ITEM_SUCCESS_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES:
					{   // 히스토리 요청 시도 성공
						// 히스토리가 있음 cube_history_packet->GetSize() > 0
						WORD size = cube_history_packet->history_list_size; 
						for (int i = 0; i < size; ++i)
						{
							history_list_[i] = cube_history_packet->history_list[i];
						}
						std::sort(history_list_, history_list_+size, SortByDate);//날짜 순 정렬
						for (int i = 0; i < size; ++i)
						{
							modify_condition_list_[i].history_id = history_list_[i].history_id;
							modify_condition_list_[i].is_memorize = history_list_[i].is_memorized;
						}

						if (size > 0)
						{
							CCtrlVScrollWZ* vscroll_control_ptr = 
								static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_Scroll)));
							int scroll_range = size - (kControl_List_L004 - kControl_List_L000 + 1);
							if (vscroll_control_ptr != NULL && size > 0)
							{
								vscroll_control_ptr->SetScrollRangeWZ(0, scroll_range);
								vscroll_control_ptr->SetScrollPosWZ(0);
								list_current_focus_ = 0;
								list_size_ = size;
							}
							SetListItems();
						}
					}
					break;
				case RC::RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES:
					// 히스토리 요청 시도가 실패
				default:
					ItemManager::Instance()->NoticeReasonItemErrorCode(cube_history_packet->result_code);
					break;
				}
			}
			break;
		case CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK:
			{
				MSG_CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK* cube_result_packet =
					static_cast<MSG_CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK*>(message);

				cube_lock_ = true;
				//DeleteAllSlot();
				switch (cube_result_packet->result_code_)
				{
				case RC::RC_ITEM_SUCCESS_TO_COMPOSE_CUBE:
				case RC::RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE:
					{
						save_inventory_item_info = cube_result_packet->added_inventory_items_;
						g_KeyQueueManager.DeleteMsg(eDoType_dialog_Cube);
						animate_frame_count_ = 0;
						animate_start_tick_ = clock_function::GetTickCount();
						animate_end_tick_ = animate_start_tick_ + kMax_Animate_Tick;
						//ItemManager::Instance()->InsertInventoryTotalInfo(CREATE, save_inventory_item_info);
						// 사운드 출력
						if (cube_result_packet->result_code_ == RC::RC_ITEM_SUCCESS_TO_COMPOSE_CUBE)
						{
							if (save_inventory_item_info.m_InvenCount == 0)
							{   // 실패 사운드
								g_SoundEffect.PlayUI(UI_SOUND_CUBE_COMPOSITION_FAIL);
							}
							else
							{   // 성공 사운드
								g_SoundEffect.PlayUI(UI_SOUND_CUBE_COMPOSITION_SUCCESS);
							}
						}
						else
						{
							g_SoundEffect.PlayUI(UI_SOUND_CUBE_DECOMPOSITION);
						}
						break;
					}			
				default:
					g_InterfaceManager.UnLockDlgs();
					ItemManager::Instance()->NoticeReasonItemErrorCode(cube_result_packet->result_code_);
					break;
				}//switch(cube_result_packet->result_code_)
			}//case
			break;
		}//switch
	}
}
//------------------------------------------------------------------------------
bool uiItemCube::ExecuteCube(bool is_composition)
{
	bool is_executeable = false;
	MaterialInventoryPositionList material_inventory_position_list;
	TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};

	g_pHero->StopMove();    // 합성하면서 포탈타는 것 등을 못하게 하기위해 멈춘다
	g_pHero->ResetMoveFlag();
	g_HeroInput.SetAutoRun(FALSE);
	g_HeroInput.SetFollowState(FALSE);

	int i = 0;
	for (i = 0; i < kControl_Button_B008+1; ++i)
	{
		if (item_inventory_position[i] != INVALID_POSTYPE_VALUE)
		{
			material_inventory_position_list.push_back(item_inventory_position[i]);
		}
	}

	if (material_inventory_position_list.size() > 0)
	{
		MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN request;
		SCItemSlotContainer& item_slot_container = 
			static_cast<SCItemSlotContainer&>(*(g_pHero->GetSlotContainer(SI_INVENTORY)));
		is_executeable = CubeSystemParser::Instance()->GetRequest(item_slot_container, 
			is_composition, material_inventory_position_list, cube_position_, request);

		if (is_executeable == true)
		{
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &request, request.GetSize());
			SendPacketListLock();// 패킷 락 정보도 전송
			g_InterfaceManager.LockDlgs(IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE); // 화면 락
		}
		else
		{
			if (is_composition)
			{   // 3403: 아무 변화도 일어나지 않습니다. 합성이 불가능한 재료입니다.
				g_InterfaceManager.GetInterfaceString(3403, message_buffer,INTERFACE_STRING_LENGTH);
			}
			else
			{   // 3411: 아무 변화도 일어나지 않습니다. 분해가 불가능한 재료입니다.
				g_InterfaceManager.GetInterfaceString(3411, message_buffer,INTERFACE_STRING_LENGTH);
			}
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);
		}
	}
	else
	{
		if (is_composition)
		{   // 3404: 합성할 재료를 큐브에 넣은 후, 시도해주세요.
			g_InterfaceManager.GetInterfaceString(3404, message_buffer,INTERFACE_STRING_LENGTH);
		}
		else
		{   // 3405: 합성할 재료를 큐브에 넣은 후, 시도해주세요.
			g_InterfaceManager.GetInterfaceString(3405, message_buffer,INTERFACE_STRING_LENGTH);
		}
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);
	}
	return is_executeable;
}

void uiItemCube::OnItemCube_InventoryRightClick(const SLOTIDX from_container_index, 
												const POSTYPE from_container_position)
{
	MouseHandler::Instance()->ItemCubeInventoryRightClick(from_container_index, SI_CUBE, 
		from_container_position);
}

void uiItemCube::ShowCubeLists(bool is_value)
{
	is_show_cube_history_ = is_value;
	ShowBackgrounds(is_value);
	ShowListControls(is_value);
	ShowListOpenButton(is_value);

	if (is_value == false)
	{
		ShowSelectionControls(is_value);    // 선택된 리스트는 닫을 때만 적용
	}
}

void uiItemCube::ShowBackgrounds(bool is_value)
{
	CControlWZ* control_ptr;
	ENUM_STATEWZ state_value = is_value? WZ_SHOW:WZ_HIDE;

	UIControlPosName control_positions[] = {KControl_Picture_Back,kControl_Picture_Top,kControl_List_Background, kControl_List_Scroll,
		kControl_List_Title, kControl_Max_Size};

	int index = 0;
	while(control_positions[index] != kControl_Max_Size)
	{
		control_ptr = GetControlWZ(GetControlID_byPosition(control_positions[index]));
		++index;
		if (control_ptr != NULL)
		{
			control_ptr->ShowWindowWZ(state_value);
		}
	}
}

void uiItemCube::ShowSelectionControls(bool is_value)
{
	CControlWZ* control_ptr;
	ENUM_STATEWZ state_value = is_value? WZ_SHOW:WZ_HIDE;

	UIControlPosName control_positions[] = {kControl_Button_ListSlot0, kControl_List0,
		kControl_List_Description, kControl_Select_Background, kControl_Slot_Background,
		kControl_Description_Background, kControl_Max_Size};

	int index = 0;
	while(control_positions[index] != kControl_Max_Size)
	{
		control_ptr = GetControlWZ(GetControlID_byPosition(control_positions[index]));
		++index;
		if (control_ptr != NULL)
		{
			control_ptr->ShowWindowWZ(state_value);
		}
	}    
}

void uiItemCube::ShowListControls(bool is_value)
{
	CControlWZ* control_ptr;
	ENUM_STATEWZ state_value = is_value? WZ_SHOW:WZ_HIDE;

	for(int index = kControl_Button_List1; index <= kControl_List5; ++index)
	{
		if (index == kControl_Button_ListSlot0)
		{
			continue;  // 선택된 리스트 컨트롤은 스킵
		}
		control_ptr = GetControlWZ(GetControlID_byPosition(index));
		if (control_ptr != NULL)
		{
			control_ptr->ShowWindowWZ(state_value);
		}
	}

	for (int index = kControl_Picture_P444 ; index <= kControl_Picture_P999 ; ++index)
	{
		control_ptr = GetControlWZ(GetControlID_byPosition(index));
		if (control_ptr != NULL)
		{
			control_ptr->ShowWindowWZ(state_value);
		}
	}
}

void uiItemCube::ShowListOpenButton(bool is_value)
{
	CControlWZ* control_ptr;
	ENUM_STATEWZ state_value;

	control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Button_Open));
	if (control_ptr != NULL)
	{
		state_value = is_value? WZ_HIDE:WZ_SHOW;
		control_ptr->ShowWindowWZ(state_value);
	}
	control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Small_Background));
	if (control_ptr != NULL)
	{
		state_value = is_value? WZ_HIDE:WZ_SHOW;
		control_ptr->ShowWindowWZ(state_value);
	}
	control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Button_ListClose));
	if (control_ptr != NULL)
	{
		state_value = is_value? WZ_SHOW:WZ_HIDE;
		control_ptr->ShowWindowWZ(state_value);
	}
}

void uiItemCube::SetListItems()
{
	int list_size = list_current_focus_ + (kControl_List5 - kControl_List1);
	int index = 0;
	for (int i = list_current_focus_; i <= list_size; ++i)
	{
		SetListItem(index, i);
		SetItemIcon(index, i);
		SetListLocking(index, i);
		++index;
	}
}

void uiItemCube::SetListLocking(int index, int focus)
{
	if (is_show_cube_history_ == false)
	{
		return;
	}

	CCtrlButtonWZ* control_lock = 
		static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_Lock1 + index)));

	CCtrlButtonWZ* control_unlock = 
		static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_UnLock1 + index)));

	if (control_lock && control_unlock)
	{
		if (modify_condition_list_[focus].is_memorize == true)
		{
			control_lock->ShowWindowWZ(WZ_SHOW);
			control_unlock->ShowWindowWZ(WZ_HIDE);
		}
		else
		{
			control_lock->ShowWindowWZ(WZ_HIDE);
			control_unlock->ShowWindowWZ(WZ_SHOW);
		}
	}
}

void uiItemCube::SetItemIcon(int index, int focus)
{
	CODETYPE image_code = 0;

	for (int i = 0; i < history_list_[focus].used_item_list_size; ++i)
	{
		if (history_list_[focus].used_item_list[i].is_material == false)
		{
			CODETYPE item_code = history_list_[focus].used_item_list[i].item_code;
			BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
			if (item_info)
			{
				image_code = item_info->m_wVICode;
				break;
			}
		}
	}

	if (image_code != 0)
	{
		SetControlImage(GetControlID_byPosition(kControl_Button_ListSlot1 + index), image_code, 0, 0, 32, 32);
	}
}

void uiItemCube::SetListItem(int index, int focus)
{
	CCtrlListWZ* control_ptr = 
		static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List1 + index)));

	if (control_ptr != NULL)
	{
		control_ptr->DeleteItemAll();
		control_ptr->SetSellHeight(15);
		control_ptr->InsertItemBack();
		control_ptr->SetItemData(0, (void*)&history_list_[focus]);
		control_ptr->InsertItemBack();
		control_ptr->SetItemData(1, (void*)&history_list_[focus]);

		BASE_ITEMINFO* item_info;
		//=====================================================================================
		//	1.리스트컨트롤 데이터삽입
		//=====================================================================================
		for (int i = 0; i < history_list_[focus].used_item_list_size; ++i)
		{
			item_info = ItemInfoParser::Instance()->GetItemInfo(history_list_[focus].used_item_list[i].item_code);
			if (item_info && history_list_[focus].used_item_list[i].is_material == false) // 결과물
			{
				TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
				TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
				TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetItemName(item_info->m_Code, item_name);

				GlobalFunc::SetItem(control_ptr, 0, 0,
					item_name,
					StrToWzID("m212"),
					DT_LEFT,
					RGBA(255,255,255,0), 
					RGBA(255,255,0,255), 
					RGBA(255,255,0,255));

				SYSTEMTIME systime;
				DWORD64 date_time = history_list_[focus].history_date_time;
				util::YYYYMMDDHHMMSSToSYSTEMTIME(date_time, systime);

				// 2232   %4d 년 %2d월 %2d일 %2d시 %2d분 
				GlobalFunc::GetDateString(tooltip_string, INTERFACE_STRING_LENGTH, 2232, systime);

				GlobalFunc::SetItem(control_ptr, 1, 0,
					tooltip_string,
					StrToWzID("m212"),
					DT_LEFT,
					RGBA(255,255,255,0), 
					RGBA(255,255,255,255), 
					RGBA(255,255,255,255));

				break;
			}
		}
	}
}

void uiItemCube::SetDetailListItem(int focus)
{
	CCtrlListWZ* control_ptr = 
		static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_Description)));

	CCtrlListWZ* background_control_ptr = 
		static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Description_Background)));

	CCtrlListWZ* background_title_control_ptr = 
		static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List0)));

	int row = 0;
	if (control_ptr != NULL)
	{
		control_ptr->DeleteItemAll();
		control_ptr->SetSellHeight(15);

		int list_size = history_list_[focus].used_item_list_size;

		if (list_size > 10) // 11줄 부터는 크기 확장
		{
			RECT list_rect = control_ptr->GetSize();
			int list_height = 16 * list_size;
			control_ptr->SetSizeHeight(list_height);
			if (background_control_ptr)
			{
				background_control_ptr->SetSizeHeight(list_height + background_title_control_ptr->GetSize().bottom + 30);
			}
		}
		else
		{
			if (background_control_ptr)
			{
				background_control_ptr->SetSizeHeight(default_list_background_height_);
			}
		}

		TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
		BASE_ITEMINFO* item_info;
		//=====================================================================================
		//	1.리스트컨트롤 데이터삽입
		//=====================================================================================
		int material_count = 0;
		for (int i = list_size - 1; i >= 0; --i)
		{
			item_info = ItemInfoParser::Instance()->GetItemInfo(history_list_[focus].used_item_list[i].item_code);

			row = control_ptr->InsertItemBack();
			control_ptr->SetItemData(row, (void*)&history_list_[focus]);

			if (item_info == NULL)
			{
				continue;
			}

			if (history_list_[focus].used_item_list[i].is_material == true) // 재료
			{   // 2233 재료 1: %s %d개 ~ 2241 재료 9: %s %d개
				GetUIMan()->GetInterfaceString((2233 + material_count), temp_string);
				++material_count;
				g_InterfaceManager.GetItemName(item_info->m_Code, item_name);
				sprintf(string, temp_string, item_name, history_list_[focus].used_item_list[i].number_of_item);
				GlobalFunc::SetItem(control_ptr, row, 0,
					string,
					StrToWzID("m212"),
					DT_LEFT,
					RGBA(255,255,255,0), 
					RGBA(80,200,80,255), 
					RGBA(80,200,80,255));
			}
			else
			{
				g_InterfaceManager.GetItemName(item_info->m_Code, item_name);
				sprintf(string, "%s (%d)", item_name, history_list_[focus].used_item_list[i].number_of_item);
				if (item_info && history_list_[focus].used_item_list[i].is_material == false) // 결과물
				{
					GlobalFunc::SetItem(control_ptr, row, 0,
						string,
						StrToWzID("m212"),
						DT_LEFT,
						RGBA(255,255,255,0), 
						RGBA(255,255,0,255), 
						RGBA(255,255,0,255));
				}
			}
		}
	}
}

void uiItemCube::OnUpdateSolarDialog()
{
	CDialogWZ* dialog_ptr = GetDialogWZ();
	if (dialog_ptr != NULL)
	{
		// UI Inetersect 영역 설정 - 설정한 영역에서만 UI가 동작한다.
		STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
		intersect_list.clear();        

		RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();
		RECT disable_rect = {0, 0, 0, 0};

		CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Description_Background));

		if (control != NULL)
		{
			disable_rect = control->GetSizeRect();
			enable_rect.left = disable_rect.right;
			intersect_list.push_back(enable_rect);

			// 리스트 열기, 닫기 컨트롤이 열려있으면 해당 영역도 포함
			control = GetControlWZ(GetControlID_byPosition(kControl_Small_Background));
			if (control != NULL)
			{
				enable_rect = control->GetSizeRect();
				intersect_list.push_back(enable_rect);
			}
			// 리스트가 열려있으면 해당 영역도 포함
			control = GetControlWZ(GetControlID_byPosition(kControl_List_Background));
			if ((control != NULL) && (control->GetState() == WZ_SHOW))
			{
				enable_rect = control->GetSizeRect();
				intersect_list.push_back(enable_rect);
			}
		}
	}

	UpdateToolTip();
}
