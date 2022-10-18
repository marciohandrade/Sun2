//---------------------------------------------------------------------------
// QuickContainer
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------


#include "SunClientPrecompiledHeader.h"
#include "quickcontainer.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "QuickSlot.h"
#include "ItemManager.h"
#include "InventoryDialog.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"
#include "HeroActionInput.h"
#include "mouse.h"
#include "interfaceManager.h"
#include "ObjectManager.h"
#include "SlotKeyGenerator.h"
#include "CursorChangeTransact.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GlobalFunc.h"
#include "uiChatMan/uichatman.h"
#include "uiSkillQueueMan/uiSkillQueueMan.h"
#include "uiSummonSkillMan/uiSummonSkillMan.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"

#include "HotKeyDefine.h"
#include "Input/HotKeyInfoParser.h"
#include "HotKeyManager.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "GameOptionParser.h"
#endif
//----------------------------------------------------------------------------
/**
*/
WzID QuickContainer::m_wzId[QUICK_MAX] = 
{ 
    StrToWzID("BT14"),
    StrToWzID("BT15"),
    StrToWzID("BT16"),
    StrToWzID("BT17"),
    StrToWzID("BT18"),
    StrToWzID("BT19"),
    StrToWzID("BT20"),
    StrToWzID("BT21"),
    StrToWzID("BT22"),
    StrToWzID("BT23"),//10
    StrToWzID("BT24"),
    StrToWzID("BT25"),
    StrToWzID("BT34"),
    StrToWzID("BT35"),
    StrToWzID("BT36"),
    StrToWzID("BT37"),
    StrToWzID("BT38"),
    StrToWzID("BT39"),
    StrToWzID("BT40"),
    StrToWzID("BT41"),//20

    StrToWzID("BT02"),
    StrToWzID("BT03"),
    StrToWzID("BT04"),
    StrToWzID("BT05"),
    StrToWzID("BT06"),
    StrToWzID("BT07"),
    StrToWzID("BT08"),
    StrToWzID("BT09"),
    StrToWzID("BT10"),
    StrToWzID("BT11"),//30
    StrToWzID("BT12"),
    StrToWzID("BT13"),
    StrToWzID("BT33"),
    StrToWzID("BT32"),
    StrToWzID("BT31"),
    StrToWzID("BT30"),
    StrToWzID("BT29"),
    StrToWzID("BT28"),
    StrToWzID("BT27"),
    StrToWzID("BT26"),//40

    StrToWzID("BT00"),//+
	StrToWzID("BT01"),//-
    StrToWzID("BT42"),//∞ QUICK_CHANGE_SLOT
    StrToWzID("PI02"),//background_of_single
    StrToWzID("PI03"),//background_of_double

#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("P002"),//QUICK_PICTURE_EXPP002
	StrToWzID("P000"),//QUICK_PICTURE_EXPP000
	StrToWzID("T009"),//QUICK_STATIC_TEXT009
#else
    StrToWzID("PI01"),//
#endif
};

static float CalculateExp()
{
	float fExpPercent=0.0f;
	EXPTYPE destexp		= g_pHero->GetNextExp();
	EXPTYPE curaccumexp	= g_pHero->GetAccumExp(g_pHero->GetLevel());
	EXPTYPE curexp		= g_pHero->GetExp();

	__int64 iXpForNextLevel = (__int64)destexp - (__int64)curaccumexp;
	__int64 iCurrentXpForNextLevel = (__int64)curexp - (__int64)curaccumexp;

	if (iXpForNextLevel < 1)
	{
		iXpForNextLevel = 1;
	}

	if (iCurrentXpForNextLevel < 0)
	{
		iCurrentXpForNextLevel = 0;
	}


	fExpPercent = ((float)iCurrentXpForNextLevel / (float)iXpForNextLevel) * 100.0f;

	fExpPercent = min(100.0f,fExpPercent);

	return fExpPercent;

}

//----------------------------------------------------------------------------
/**
*/
QuickContainer::QuickContainer()
	:	m_TabNum ( 0 ),
		m_TabIndex( 0 ),
		m_PrevTabIndex( -1 ),
		m_pItemUnitRenderer ( NULL )
		,is_double_slot_(false)
{
}


//----------------------------------------------------------------------------
/**
*/
QuickContainer::~QuickContainer()
{
	assert( NULL == m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::OnUpdateSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Update();
#ifdef _NA_000000_20130114_RENEWER_UI
	float fExpPercent = CalculateExp();
	SetHeroExp(fExpPercent);
#endif
}


//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::OnRenderSolarDialog()
{
	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render();

#ifdef _NA_000000_20130114_RENEWER_UI
	//RenderText();
#endif

        {
            // 퀵슬록 설정 단축키 출력
            STLX_STRING hot_key_string;
            ENUM_STATEWZ show_state;
            RECT control_rect;
            HOT_KEY_CODE hot_ket_code;
            HOT_KEY_FUNCTION_INDEX function_index;
            KEY_BIND_PAIR_MAP key_bind_pair_map = 
                HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map();
            KEY_BIND_PAIR_MAP::iterator found_itr;
            KEY_BIND_PAIR_MAP::iterator end_itr = key_bind_pair_map.end();
            POSTYPE max_position = QUICK_SLOT_HALF;
            if (double_size() == true)
            {
                max_position += QUICK_SLOT_HALF;
            }
            for (POSTYPE control_position = QUICK_BT00; control_position < max_position; ++control_position)
            {
                CControlWZ* slot_button = getControl(control_position);
                if (slot_button == NULL)
                {
                    continue;
                }

                show_state = slot_button->GetState();
                if (show_state != WZ_SHOW)
                {
                    continue;
                }

                function_index = 
                    static_cast<HOT_KEY_FUNCTION_INDEX>(kUse_QuickSlot1 + (control_position - QUICK_BT00));
                found_itr = key_bind_pair_map.find(function_index);
                if (found_itr == end_itr)
                {
                    continue;
                }

                KEY_BIND_PAIR& key_bind_pair = found_itr->second;
                hot_ket_code.hot_key_code_ = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey);
                if (hot_ket_code.hot_key_code_ == 0)
                {
                    hot_ket_code.hot_key_code_ = key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kSubHotKey);
                }
                if (hot_ket_code.hot_key_code_ == 0)
                {
                    continue;
                }

                hot_key_string.clear();
                if (hot_ket_code.ctrl_key_ == 1)
                {
                    hot_key_string += "C";
                }

                if (hot_ket_code.alt_key_ == 1)
                {
                    hot_key_string += "A";
                }

                if (hot_ket_code.shift_key_ == 1)
                {
                    hot_key_string += "S";
                }

                if (hot_key_string.length() > 0)
                {
                    hot_key_string += ":";
                }

                int string_code = HotKeyInfoParser::Instance()->GetStringCodeForQuickSlot(hot_ket_code.key_code_);
                hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);

                control_rect = slot_button->GetSizeRect();
                const WzColor text_color = WzColor_RGB(195, 171, 122);
                g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("st07"));
                g_pSunRenderer->x_pManagerTextOut->DrawText(hot_key_string.c_str(), 
                    &control_rect, 
                    text_color, 
                    WzColor_RGBA( 0, 0, 0, 0), 
                    TP_HLEFT, 
                    ETS_OUTLINE);
                g_pSunRenderer->x_pManagerTextOut->EndUseFont();
            }
        }

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	POSTYPE pos = 0;
	int string_code = 0;
	for (POSTYPE a = QUICK_BT00; a < QUICK_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
					switch (a)
					{
					case QUICK_CHANGE_SLOT:
						string_code =  2170;
						break;
					case QUICK_UPBT:
					case QUICK_DNBT:
						string_code =  2171;
						break;
					}
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
	else
	{
		ProcessToolTip(!!bMouseOver, string_code);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::Init( CDrawBase * pDrawBase )
{
	m_TabNum = 0;
	QuickSlotContainer::Init(MAX_QUICK_SLOT_NUM);

    button_of_double_ =
        static_cast<CCtrlButtonWZ*>(getControl(QUICK_UPBT));
    assert(button_of_double_);
    button_of_single_ =
    static_cast<CCtrlButtonWZ*>(getControl(QUICK_DNBT));
    assert(button_of_single_);

#ifdef _NA_000000_20130114_RENEWER_UI
#else
    numbers_of_bottom_line_ =
        static_cast<CCtrlPictureWZ*>(getControl(QUICK_NUMBER_BOTTOM_LINE));
    assert(numbers_of_bottom_line_);
#endif //_NA_000000_20130114_RENEWER_UI

    background_of_single_slot_ =
        static_cast<CCtrlPictureWZ*>(getControl(QUICK_BACKGROUND_SINGLE));
    assert(background_of_single_slot_);
    background_of_double_slot_ =
        static_cast<CCtrlPictureWZ*>(getControl(QUICK_BACKGROUND_DOUBLE));
    assert(background_of_double_slot_);

    BYTE result_val = CGameOption::Instance()->GetQuickSlotSize();
    if (result_val == 1)
    {
        double_size_ = false;
    }
    else
    {
        double_size_ = true;
    }
    ChangeSlotLine(double_size_);

    result_val = CGameOption::Instance()->GetQuickSlotIndex();
    SetCurQuickSlot(result_val);

#ifdef _NA_000000_20130114_RENEWER_UI
#else
    if (numbers_of_bottom_line_ != NULL)
    {
        numbers_of_bottom_line_->ShowWindowWZ(WZ_HIDE);
    }

    if (numbers_of_bottom_line_ != NULL)
    {
        rectangles_[RECTANGEL_OF_UI] = numbers_of_bottom_line_->GetSizeRect();
        rectangles_[RECTANGEL_OF_CONTROL] = numbers_of_bottom_line_->GetSize();
        format_of_image_control_ = numbers_of_bottom_line_->GetCtrlImageFormat();
        if (format_of_image_control_ != NULL)
        {
            rectangle_of_texture_ = format_of_image_control_->m_sImageState[E_BASE].m_sFRect;
        }
        else
        {
            rectangle_of_texture_.fTopX = 0;
            rectangle_of_texture_.fTopY = 0;
            rectangle_of_texture_.fWidth = 0;
            rectangle_of_texture_.fHeight = 0;
            assert(!("can not get image control format"));
        }
    }
#endif //_NA_000000_20130114_RENEWER_UI

#ifdef _NA_000000_20130114_RENEWER_UI
	CCtrlPictureWZ* exp_line_Max =
		static_cast<CCtrlPictureWZ*>(getControl(QUICK_PICTURE_EXPP002));

	if(exp_line_Max)
	{
		exp_line_Max->ShowWindowWZ(WZ_HIDE);
	}

	exp_line_ =
		static_cast<CCtrlPictureWZ*>(getControl(QUICK_PICTURE_EXPP000));
	
	exp_gauge_size = 0;
	if (exp_line_)
	{
		exp_gauge_size = (float)exp_line_->GetSize().right;
	}

	Text_EXP_Info_ =
		static_cast<CCtrlStaticWZ*>(getControl(QUICK_STATIC_TEXT009));
	
#endif

    is_item_move_ = false;
}
//----------------------------------------------------------------------------
/**
*/
void QuickContainer::ChangeSlotLine(bool double_size)
{
    if (background_of_single_slot_ == NULL || background_of_double_slot_ == NULL ||
        button_of_double_ == NULL || button_of_single_ == NULL)
    {
        return;
    }
    if (double_size_ == true)
    {
#ifdef _NA_000000_20130114_RENEWER_UI
		background_of_single_slot_->ShowWindowWZ(WZ_SHOW);
#else
        background_of_single_slot_->ShowWindowWZ(WZ_HIDE);
#endif

		background_of_double_slot_->ShowWindowWZ(WZ_SHOW);
        button_of_double_->ShowWindowWZ(WZ_HIDE);
        button_of_single_->ShowWindowWZ(WZ_SHOW);
        CGameOption::Instance()->SetQuickSlotSize(2);
    }
    else
    {
        background_of_single_slot_->ShowWindowWZ(WZ_SHOW);
        background_of_double_slot_->ShowWindowWZ(WZ_HIDE);
        button_of_double_->ShowWindowWZ(WZ_SHOW);
        button_of_single_->ShowWindowWZ(WZ_HIDE);
        CGameOption::Instance()->SetQuickSlotSize(1);
    }
    UpdateDialogSize();
}
//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::InitItemRender()
{
	// 임시
	if (m_pItemUnitRenderer) 
    {
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, GetMaxSlotNum() );
	m_pItemUnitRenderer->ShowCooldown(TRUE);
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
QuickContainer::IsExistSameSerialItem(DBSERIAL Serial)
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if (IsEmpty(i))
		{
			continue;
		}

		QuickSlot & rQuickSlot = (QuickSlot &)GetSlot(i);

		if ( rQuickSlot.GetOrgSerial() == Serial )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
QuickContainer::IsExistSameCodeItem( CODETYPE Code )
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{	
		if (IsEmpty(i))
		{
			continue;
		}

		QuickSlot & rQuickSlot = (QuickSlot &) GetSlot(i);

		if ( rQuickSlot.GetOrgCode() == Code )
		{
			return TRUE;
		}
	}

	return FALSE;

}


//----------------------------------------------------------------------------
/**
*/
POSTYPE 
QuickContainer::GetPosByCode( CODETYPE Code , POSTYPE AfterPos /* = 0 */)
{
	for (int i = AfterPos; i < GetMaxSlotNum(); ++i)
	{	
		if (IsEmpty(i))
		{
			continue;
		}

		QuickSlot & rQuickSlot = (QuickSlot &) GetSlot(i);

		if ( rQuickSlot.GetOrgCode() == Code )
		{
			return i;
		}
	}

	return QUICK_SLOT_NULL_VALUE;
}


//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT 	QuickContainer::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{ 
    if (AtPos > QUICK_SLOT_MAX)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

	RC::eSLOT_INSERT_RESULT eInsertSlotResult = QuickSlotContainer::InsertSlot( AtPos, rSlot );
	
	//QuickSlot & rQuickSlot = (QuickSlot & )GetSlot(AtPos));
    QuickSlot & rQuickSlot = (QuickSlot & )QuickSlotContainer::GetSlot(AtPos);
    CControlWZ * pCtrl = getControl( GetRelativeSlotPos(AtPos) );
	if(!pCtrl)
		return RC::RC_INSERT_SLOT_FAILED;

	RECT rect = pCtrl->GetSizeRect();

	assert( ST_QUICK == rSlot.GetSlotType() );
    
    DBSERIAL Serial = g_SlotKeyGenerator.GetKey();
    assert(Serial);
    rQuickSlot.SetOrgSerial( Serial );
    
	if(m_pItemUnitRenderer)
	{
		// 3D 모델링 Render
		m_pItemUnitRenderer->AddItemUnit( 
			rQuickSlot.GetSerial(), 
			rQuickSlot.GetOrgCode(), 
			&rect, 
			&rQuickSlot ,pCtrl);
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	return eInsertSlotResult;
}



//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::getControlPos( POSTYPE AtPos, RECT & rectOut )
{
	CControlWZ * pCtrl = getControl( AtPos );
	rectOut = pCtrl->GetSizeRect();
}


//----------------------------------------------------------------------------
/**
*/
BOOL
QuickContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	// 소스 퀵 포지션 초기화
	QuickSlot & rSlot = (QuickSlot &)GetSlot(AtPos);
	
	if (rSlot.GetOrgSerial())
	{
		g_SlotKeyGenerator.Restore((DWORD)rSlot.GetOrgSerial() );
		
		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetOrgSerial() );
		}
		else
		{
			assert(m_pItemUnitRenderer);
		}

	}

	return QuickSlotContainer::DeleteSlot( AtPos, pSlotOut );
}


//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
QuickContainer::getControl( POSTYPE AtPos )
{
	assert( AtPos < QUICK_MAX );
	return GetControlWZ( m_wzId[ AtPos % QUICK_MAX ] );
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE 
QuickContainer::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < QUICK_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i % QUICK_MAX;
		}
	}

	return QUICK_MAX;
}


//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer * 
QuickContainer::GetItemUnitRender() const
{
	return m_pItemUnitRenderer;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( pos >= QUICK_BT00  && pos < QUICK_SLOT_MAX ) 
			{
				CControlWZ * pControl = getControl(pos);
				assert(pControl);

				if(pControl)
				{
					RECT rcSlot = pControl->GetSizeRect();
					MouseHandler::Instance()->ItemTransaction(SI_QUICK, GetRealSlotPos(pos), &rcSlot);
				}
			}
			else
			{
				switch (pos)
				{
				case QUICK_UPBT:
					{
                        double_size_ = true;
                        ChangeSlotLine(double_size_);
                        ReFreshRenderSlot();
                        uiChatMan* puiChatMap = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
                        if (puiChatMap != NULL)
                        {
                            puiChatMap->MoveChatDialog(2);
                        }
                        uiSkillQueueMan* ui_skill_queue_manager_ptr = 
                            GET_DIALOG_MANAGER(uiSkillQueueMan, UIMAN_SKILLQUEUE);
                        if (ui_skill_queue_manager_ptr != NULL)
                        {
                            ui_skill_queue_manager_ptr->MoveSkillQueueDialog(2);
                        }

                        uiSummonTargetMan* ui_summon_target_manager_ptr =
                            GET_DIALOG_MANAGER(uiSummonTargetMan, UIMAN_SUMMONTARGET);
                        if (ui_summon_target_manager_ptr != NULL)
                        {
                            ui_summon_target_manager_ptr->MoveSummonTargetDialog(2);
                        }
                        
                        #ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
                        #else
                        uiSummonSkillMan* ui_summon_manager_ptr =
                            GET_DIALOG_MANAGER(uiSummonSkillMan, UIMAN_SUMMONSKILL);
                        if (ui_summon_manager_ptr != NULL)
                        {
                            ui_summon_manager_ptr->MoveSummonSkillDialog(2);
                        }
                        #endif
					}
					break;

				case QUICK_DNBT:
					{
                        double_size_ = false;
                        ChangeSlotLine(double_size_);
                        ReFreshRenderSlot();
                        uiChatMan* puiChatMap = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
                        if (puiChatMap != NULL)
                        {
                            puiChatMap->MoveChatDialog(1);
                        }
                        uiSkillQueueMan* ui_skill_queue_manager_ptr = 
                            GET_DIALOG_MANAGER(uiSkillQueueMan, UIMAN_SKILLQUEUE);
                        if (ui_skill_queue_manager_ptr != NULL)
                        {
                            ui_skill_queue_manager_ptr->MoveSkillQueueDialog(1);
                        }

                        uiSummonTargetMan* ui_summon_target_manager_ptr =
                            GET_DIALOG_MANAGER(uiSummonTargetMan, UIMAN_SUMMONTARGET);
                        if (ui_summon_target_manager_ptr != NULL)
                        {
                            ui_summon_target_manager_ptr->MoveSummonTargetDialog(1);
                        }
                        #ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
                        #else
                        uiSummonSkillMan* ui_summon_manager_ptr =
                            GET_DIALOG_MANAGER(uiSummonSkillMan, UIMAN_SUMMONSKILL);
                        if (ui_summon_manager_ptr != NULL)
                        {
                            ui_summon_manager_ptr->MoveSummonSkillDialog(1);
                        }
                        #endif
					}
					break;
                case QUICK_CHANGE_SLOT:
                    {
                        ToggleSlot();
                    }
                    break;
				}
					

			}
		}
		break;

	//case RT_MSG_RBUTTONCLICK:
	case RT_MSG_RBUTTONDOWN:	// 애니메이션 동작시 Click 메시지가 들어오지 않는 버그가 있어서 down으로 변경
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( pos > QUICK_SLOT_MAX ) return;

			CControlWZ * pControl = getControl(pos);
			assert(pControl);
			
			if(pControl)
			{
				MouseHandler::Instance()->ItemUseTransaction(SI_QUICK, pos);
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


//----------------------------------------------------------------------------
/**
*/
VOID			
QuickContainer::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			POSTYPE RelativePos = GetRealSlotPos(pos);

			if (pos >= 0 && pos < QUICK_SLOT_MAX)
			{
				if (!IsEmpty( RelativePos ))
				{
					SCSlot & rSlot = GetSlot( RelativePos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;
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
		}
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
QuickContainer::NetworkProc( MSG_BASE * pMsg )
{

}


//----------------------------------------------------------------------------
/**
*/
VOID			
QuickContainer::OnShowWindow(BOOL val)
{
	if (val)
	{
		this->ReFreshRenderSlot();
	}
	else
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}

// 실제 슬롯정보관련한 위치
POSTYPE	 QuickContainer::GetRealSlotPos( POSTYPE pos )
{
    if (double_size_)
    {
        if (m_TabNum == 0)
        {
            return (pos + m_TabNum);
        }
        else
        {
            return ((pos+QUICK_SLOT_HALF) % QUICK_SLOT_MAX);
        }
    }
    return ((pos + m_TabNum) % QUICK_SLOT_MAX);
}

// 인터페이스 컨트롤 위치.
POSTYPE	QuickContainer::GetRelativeSlotPos(POSTYPE pos)
{
    if(double_size_)
    {
        if(m_TabNum == 0)
        {
            return (pos % QUICK_SLOT_MAX);
        }
        else
        {
            return ((pos+QUICK_SLOT_HALF) % QUICK_SLOT_MAX);
        }
    }
    else
    {
        return pos%QUICK_SLOT_HALF;
    }	
}

VOID QuickContainer::FlushUnitDraw()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	m_pItemUnitRenderer->FlushUnitDraw();

	QuickSlot FromSlot;

	for (int i = 0; i < GetMaxSlotNum(); i++)
	{
			
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

}

VOID QuickContainer::ReFreshRenderSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;
    int end_of_quick_slot = QUICK_SLOT_HALF;
    int start_of_quick_slot = 0;
    if (double_size_)
    {
        end_of_quick_slot = QUICK_SLOT_MAX;
    }
    else
    {
        end_of_quick_slot = QUICK_SLOT_HALF;
    }

	for (int i = 0; i < GetMaxSlotNum(); i++)//real
	{
        if (double_size_ == true)
        {
            start_of_quick_slot = 0;
            end_of_quick_slot = QUICK_SLOT_MAX;
        }
        else
        {
            start_of_quick_slot = m_TabNum;
            end_of_quick_slot = end_of_quick_slot+m_TabNum;
        }
		QuickSlot & rQuickSlot = (QuickSlot &)SCSlotContainer::GetSlot(i);
		if (!IsEmpty(i))
		{
            if (i >= start_of_quick_slot && i < end_of_quick_slot)
			{
				if(!m_pItemUnitRenderer)
                    continue;

				// 보여줘야할 목록
                rcSlot = getControl(GetRelativeSlotPos(i))->GetSizeRect();
                pCtrl = getControl(GetRelativeSlotPos(i));
                m_pItemUnitRenderer->ChangePositionItemUnit(rQuickSlot.GetOrgSerial(), &rcSlot, pCtrl);
				pRender = m_pItemUnitRenderer->GetItemUnit( rQuickSlot.GetOrgSerial() );
				if (!pRender )
				{
					pCtrl = getControl( GetRelativeSlotPos(i) );
					if(pCtrl)
					{
						rcSlot = pCtrl->GetSizeRect();

						m_pItemUnitRenderer->AddItemUnit( 
							rQuickSlot.GetOrgSerial(), 
							rQuickSlot.GetOrgCode(), 
							&rcSlot, 
							rQuickSlot.GetOrgSlot() ,pCtrl);
					}
				}

				if (pRender && 
					pRender != MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit)
				{
					pRender->SetState(rQuickSlot.GetStatus());
					pRender->SetShow(TRUE);
				}
			}
			else
			{
				if(!m_pItemUnitRenderer)
					continue;

				// 보여지지 말아야 할 목록
				pRender = m_pItemUnitRenderer->GetItemUnit( rQuickSlot.GetOrgSerial() );

				if (!pRender )
				{
					pCtrl = getControl( GetRelativeSlotPos(i) );
					if(pCtrl)
					{
						rcSlot = pCtrl->GetSizeRect();

						m_pItemUnitRenderer->AddItemUnit( 
							rQuickSlot.GetOrgSerial(), 
							rQuickSlot.GetOrgCode(), 
							&rcSlot, 
							rQuickSlot.GetOrgSlot() ,pCtrl);
					}
				}

				if (pRender)
					pRender->SetShow(FALSE);				
			}
		}
	}
}

void QuickContainer::SetCurQuickSlot(int iIndex)
{
	if(iIndex != -1)
		m_TabIndex = iIndex;

    m_TabNum = m_TabIndex * QUICK_SLOT_HALF;
    CGameOption::Instance()->SetQuickSlotIndex(iIndex);
	this->ReFreshRenderSlot();
}

void QuickContainer::UpdateDialogSize()
{
	RECT DialogRect;
	GetDialogWZ()->GetClientRect(&DialogRect);
	RECT rcDisableRect = {DialogRect.left, DialogRect.top, 
        DialogRect.right, DialogRect.bottom};	// 무효화 영역
	
	if(double_size_ == true)// 두줄
    {
        if(background_of_double_slot_ != NULL)
        {
	        rcDisableRect.bottom = background_of_double_slot_->GetSizeRect().top;
            DialogRect.top = background_of_double_slot_->GetSizeRect().top;
        }
    }
    else
    {
        if(background_of_single_slot_ != NULL)
        {
			CControlWZ* pControl = getControl(QUICK_BT20);
	        rcDisableRect.bottom = pControl->GetSizeRect().bottom;

            DialogRect.top = background_of_single_slot_->GetSizeRect().top;
        }
    }
    DisableInterSectRect(rcDisableRect);
	//this->GetDialogWZ()->SetDlgPartSizeRect(DialogRect);
}

void QuickContainer::ToggleSlot()
{
    if(m_TabIndex == 0)
    {
        SetCurQuickSlot(1);
    }
    else
    {
        SetCurQuickSlot(0);
    }
}

void QuickContainer::ToggleChangeSlotSize()
{
    uiChatMan* puiChatMap = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
    uiSkillQueueMan* ui_skill_queue_manager_ptr = 
        GET_DIALOG_MANAGER(uiSkillQueueMan, UIMAN_SKILLQUEUE);
    uiSummonTargetMan* ui_summon_target_manager_ptr =
        GET_DIALOG_MANAGER(uiSummonTargetMan, UIMAN_SUMMONTARGET);
    uiSummonSkillMan* ui_summon_manager_ptr =
        GET_DIALOG_MANAGER(uiSummonSkillMan, UIMAN_SUMMONSKILL);

    if (puiChatMap == NULL || ui_skill_queue_manager_ptr == NULL ||
        ui_summon_target_manager_ptr == NULL || ui_summon_manager_ptr == NULL)
    {
        return;
    }

    if (double_size_ == true)
    {
        double_size_ = false;
        puiChatMap->MoveChatDialog(1);
        ui_skill_queue_manager_ptr->MoveSkillQueueDialog(1);
        ui_summon_target_manager_ptr->MoveSummonTargetDialog(1);
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
        ui_summon_manager_ptr->MoveSummonSkillDialog(1);
#endif
    }
    else
    {
        double_size_ = true;
        puiChatMap->MoveChatDialog(2);
        ui_skill_queue_manager_ptr->MoveSkillQueueDialog(2);
        ui_summon_target_manager_ptr->MoveSummonTargetDialog(2);
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
        ui_summon_manager_ptr->MoveSummonSkillDialog(2);
#endif
    }
    ChangeSlotLine(double_size_);
    ReFreshRenderSlot();
}

VOID					
QuickContainer::PrevQuickTab()
{
	m_TabIndex = ((m_TabIndex) > 0)? --m_TabIndex : 0;
	SetCurQuickSlot();
}

VOID					
QuickContainer::NextQuickTab()
{
	m_TabIndex = (++m_TabIndex > 3)? 3 : m_TabIndex;
	SetCurQuickSlot();
}

void QuickContainer::ProcessToolTip(bool bMouseOver, int tooltip_string_code)
{
	if (!bMouseOver)
	{
		tooltip_string_code = 0;
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
	}
	else
	{
        if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
        {
            if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
            {
                uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
                if(pUIMan)
				{
                    pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
					pUIMan->SetTooltipDialogID(GetDialogKey());
				}

                CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
            }
        }
		if(tooltip_string_code > 0)
		{
			uiToolTipMan* pUIMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterTooltipWithStringCode(tooltip_string_code);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
}

void QuickContainer::UpdateEquip(SLOTIDX slot_index_prev, POSTYPE inven_pos_prev, SLOTCODE slot_code_prev,
                                 SLOTIDX slot_index_next, POSTYPE inven_pos_next, SLOTCODE slot_code_next)// 인벤토리 갱신 때 찾아서 있으면 위치 갱신
{
    if (slot_index_prev == slot_index_next &&
        inven_pos_prev == inven_pos_next &&
        slot_code_prev == slot_code_next)
    {
        return;
    }
#if SOLAR_HASH_TABLE_VERSION >= 0x0200
    ulong quick_slot_key = MakeQuickKey(slot_index_prev, inven_pos_prev, slot_code_prev);
    QuickLinkTable::_Paircc ranges = quick_link_table_->equal_range(quick_slot_key);
    if (ranges.first == ranges.second) {
        return;
    };

    SCSlot& change_slot = g_pHero->GetSlotContainer(slot_index_next)->GetSlot(inven_pos_next);
    for (; ranges.first != ranges.second; ++ranges.first)
    {
        QuickSlot* quick_slot = static_cast<QuickSlot*>(ranges.first->second);
        quick_slot->SetOrgSlot(&change_slot);
    };
#else
    m_pItemHashTable->SetBucketFirst(MakeQuickKey(slot_index_prev, inven_pos_prev, slot_code_prev));
    while (QuickSlot* quick_slot = (QuickSlot*)m_pItemHashTable->GetBucketNext())
    {
        SCSlot& change_slot = g_pHero->GetSlotContainer(slot_index_next)->GetSlot(inven_pos_next);
        quick_slot->SetOrgSlot(&change_slot);
    }
#endif
}

void QuickContainer::MoveDefaultWindowPos()
{
    SolarDialog::MoveDefaultWindowPos();

    ChangeSlotLine(double_size_);
    ReFreshRenderSlot();
}

#ifdef _NA_000000_20130114_RENEWER_UI
VOID QuickContainer::SetHeroExp( float fPercent )
{
	CControlWZ* ctl = getControl(QUICK_PICTURE_EXPP000);
	assert( ctl );

	CCtrlPictureWZ* exp_line_Max =
		static_cast<CCtrlPictureWZ*>(getControl(QUICK_PICTURE_EXPP002));

	if (exp_line_Max)
	{
		if ((g_pHero != NULL) &&
			(GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st() == g_pHero->GetLevel()))
		{
			exp_line_Max->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			exp_line_Max->ShowWindowWZ(WZ_HIDE);
		}
	}
	if( !ctl )
	{
		return;
	}
	assert( fPercent <= 100 );

	if (fPercent >= 100.0f)
	{
		fPercent = 100.0f;
	}

	if (fPercent < 0.0f)
	{
		fPercent = 0.0f;
	}

	RECT rect;
	rect = ctl->GetSizeRect();
	float width ;
	float height = (float)ctl->GetSize().bottom;

	if (exp_gauge_size >= 0.0f )
	{
		width = (exp_gauge_size/100.f) * fPercent;
		ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fHeight =height;
		if(width>0.f)
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =width;
		}
		else
		{
			ctl->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth =0.1f;
		}
		ctl->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
	}
	
	if(Text_EXP_Info_)
	{
		float fExpPercent = CalculateExp();
		Sprintf(m_szRenderInfo, _T("%.2f%%"), fExpPercent);
		Text_EXP_Info_->SetTextWZA(m_szRenderInfo);
	}
	

}


void QuickContainer::RenderText()
{
	DWORD dwCurVal=0;
	DWORD dwMaxVal=0;
	RECT  rc;
	POINT pt;

	if(Text_EXP_Info_)
	{
		float fExpPercent = CalculateExp();

		//exp소수점 앞자리 그리기
		rc = Text_EXP_Info_->GetSizeRect();
		pt =g_InterfaceManager.RenderDrawNumber(rc,(int)fExpPercent,Text_EXP_Info_->GetCtrlStyle());
		pt.x-=1;
		pt = g_InterfaceManager.RenderDrawSign(pt,eIMG_DOT);
		rc.left=pt.x;
		rc.top=pt.y;
		int  arFraction[2];
		char szTemp[2]={0,};

		float fMinor = (fExpPercent-(int)(fExpPercent))*100.f;
		Sprintf(m_szRenderInfo, _T("%02f"), fMinor);

		//소수점 뒷자리 두개값얻기
		szTemp[0] = m_szRenderInfo[0];


		if(fMinor<10.f)
		{
			arFraction[0]=0;
			arFraction[1]=atoi(&szTemp[0]);
		}
		else
		{
			arFraction[0]=atoi(&szTemp[0]);
			arFraction[1]=atoi(&m_szRenderInfo[1]);
		}

		//exp소수점 뒷자리 랜더링(2자리만 그림)
		for(int i=0;i<2;++i)
		{
			pt = g_InterfaceManager.RenderDrawNumber(rc,arFraction[i],DT_LEFT | DT_TOP);
			rc.left=pt.x;
			rc.top=pt.y;
		}

		g_InterfaceManager.RenderDrawSign(pt,eIMG_PERCENT);
	}
}
#endif