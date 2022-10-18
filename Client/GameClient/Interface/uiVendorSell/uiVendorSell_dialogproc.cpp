//------------------------------------------------------------------------------
//  uiVendorSell_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan/uiVendorMan.h"
#include "uiVendorSell.h"
#include "SCItemSlot.h"
#include "ItemInfoParser.h"
#include "ItemFormula.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "ItemManager.h"
#include "ChatDialog.h"
#include "HeroActionInput.h"
#include "slotkeygenerator.h"
#include "CursorChangeTransact.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiVendorTitlePopup/uiVendorTitlePopup.h"
#include "Hero.h"
//------------------------------------------------------------------------------
WzID uiVendorSell::m_wzId[DIALOG_MAX] = 
{ 
#ifdef _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
		StrToWzID("B005"),  // DIALOG_SLOT00
		StrToWzID("B006"),  // DIALOG_SLOT01
		StrToWzID("B007"),  // DIALOG_SLOT02
		StrToWzID("B008"),  // DIALOG_SLOT03
		StrToWzID("B009"),  // DIALOG_SLOT04
		StrToWzID("B010"),  // DIALOG_SLOT05
		StrToWzID("B011"),  // DIALOG_SLOT06
		StrToWzID("B012"),  // DIALOG_SLOT07

		StrToWzID("B020"),  // DIALOG_SLOT10
		StrToWzID("B019"),  // DIALOG_SLOT11
		StrToWzID("B018"),  // DIALOG_SLOT12
		StrToWzID("B017"),  // DIALOG_SLOT13
		StrToWzID("B013"),  // DIALOG_SLOT14
		StrToWzID("B014"),  // DIALOG_SLOT15
		StrToWzID("B015"),  // DIALOG_SLOT16
		StrToWzID("B016"),  // DIALOG_SLOT17

		StrToWzID("B029"),  // DIALOG_SLOT20
		StrToWzID("B030"),  // DIALOG_SLOT21
		StrToWzID("B031"),  // DIALOG_SLOT22
		StrToWzID("B032"),  // DIALOG_SLOT23
		StrToWzID("B033"),  // DIALOG_SLOT24
		StrToWzID("B034"),  // DIALOG_SLOT25
		StrToWzID("B035"),  // DIALOG_SLOT26
		StrToWzID("B036"),  // DIALOG_SLOT27

		StrToWzID("B028"),  // DIALOG_SLOT30
		StrToWzID("B027"),  // DIALOG_SLOT31
		StrToWzID("B026"),  // DIALOG_SLOT32
		StrToWzID("B025"),  // DIALOG_SLOT33
		StrToWzID("B021"),  // DIALOG_SLOT34
		StrToWzID("B022"),  // DIALOG_SLOT35
		StrToWzID("B023"),  // DIALOG_SLOT36
		StrToWzID("B024"),  // DIALOG_SLOT37

		StrToWzID("B037"),  // DIALOG_SLOT30
		StrToWzID("B038"),  // DIALOG_SLOT31
		StrToWzID("B039"),  // DIALOG_SLOT32
		StrToWzID("B040"),  // DIALOG_SLOT33
		StrToWzID("B041"),  // DIALOG_SLOT34
		StrToWzID("B042"),  // DIALOG_SLOT35
		StrToWzID("B043"),  // DIALOG_SLOT46
		StrToWzID("B044"),  // DIALOG_SLOT47

#else
        StrToWzID("B005"),  // DIALOG_SLOT00
        StrToWzID("B006"),  // DIALOG_SLOT01
        StrToWzID("B007"),  // DIALOG_SLOT02
        StrToWzID("B008"),  // DIALOG_SLOT03
        StrToWzID("B009"),  // DIALOG_SLOT04
        StrToWzID("B010"),  // DIALOG_SLOT05

        StrToWzID("B011"),  // DIALOG_SLOT10
        StrToWzID("B012"),  // DIALOG_SLOT11
        StrToWzID("B013"),  // DIALOG_SLOT12
        StrToWzID("B014"),  // DIALOG_SLOT13
        StrToWzID("B015"),  // DIALOG_SLOT14
        StrToWzID("B016"),  // DIALOG_SLOT15

        StrToWzID("B017"),  // DIALOG_SLOT20
        StrToWzID("B018"),  // DIALOG_SLOT21
        StrToWzID("B019"),  // DIALOG_SLOT22
        StrToWzID("B020"),  // DIALOG_SLOT23
        StrToWzID("B021"),  // DIALOG_SLOT24
        StrToWzID("B022"),  // DIALOG_SLOT25

        StrToWzID("B023"),  // DIALOG_SLOT30
        StrToWzID("B024"),  // DIALOG_SLOT31
        StrToWzID("B025"),  // DIALOG_SLOT32
        StrToWzID("B026"),  // DIALOG_SLOT33
        StrToWzID("B027"),  // DIALOG_SLOT34
        StrToWzID("B028"),  // DIALOG_SLOT35
#endif //_NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE

        StrToWzID("T001"),  // DIALOG_TXT_SELLITEM
        StrToWzID("T006"),  // DIALOG_TXT_SELLMSG
        StrToWzID("E000"),  // DIALOG_EDIT_SELLMSG,

        StrToWzID("T002"),  // DIALOG_TXT_ITEMCOUNTS,
        StrToWzID("T004"),  // DIALOG_TXT_ITEMVALUE,
        StrToWzID("T003"),  // DIALOG_TXTVALUE_ITEMCOUNTS,
        StrToWzID("T005"),  // DIALOG_TXTVALUE_ITEMVALUTE,
#ifdef _JBH_ADD_VENDOR_COMMISION
		StrToWzID("T009"),
		StrToWzID("T007"),
#endif

        StrToWzID("TI01"),  // DIALOG_BTN_STARTEND,
        StrToWzID("TI02"),  // DIALOG_BTN_MODIFY,
        StrToWzID("TI03"),  // DIALOG_BTN_SUBITEM,
        StrToWzID("TI00"),  // DIALOG_BTN_INPUTMODIFY,
		StrToWzID("TI04"),	// DIALOG_BTN_PAUSE

        StrToWzID("B000"),  // DIALOG_EXIT

#ifdef _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR
        StrToWzID("S000"),	// DIALOG_TXT_ITEM_UNIT_VALUE,
        StrToWzID("S001"),	// DIALOG_TXTVALUE_ITEM_UNIT_VALUE,
#endif//_DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiVendorSell::OnShowWindow( BOOL val )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	
	if ( ! g_pHero )
		return;

    if (val)
    {
		if( !EnableOpenVendorState())
			return;
        // 이러한 경우에는 개인상점 구매창을 닫아버리고 개인상점을 강제로 열도록 한다.
        InitDataWhenShowWindow();
		POINT pt = m_pVendorMan->GetWinPos();
        GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
		this->GetUIMan()->AttachFirstDialog(InterfaceManager::DIALOG_VENDOR_SELL);
		g_pHero->SetBeHaveState(PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE);

		uiVendorBuy* pVendorBuy = GET_CAST_DIALOG( uiVendorBuy, IM_VENDOR_MANAGER::VENDOR_BUY);
		if ( pVendorBuy->IsVisible() )
		{
			pVendorBuy->Exit();
		}

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_vendor_sell;
		msg.wParam=InterfaceManager::DIALOG_VENDOR_SELL;
		msg.DoSomething=GlobalFunc::DoVentorSell;
		g_KeyQueueManager.PushBack(msg);

		m_bShow			  = TRUE;

        g_pHero->StopMove();
        g_pHero->ResetMoveFlag();
        g_HeroInput.SetAutoRun(FALSE);
        g_HeroInput.SetFollowState(FALSE);
        g_HeroInput.SetAutoAttack(FALSE);
    }
    else
    {
        FlushUnitDraw();
		CloseSubWindow();
		g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
		g_HeroInput.SetAckBeginVendor(FALSE);
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_vendor_sell);
		this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_VENDOR_SELL);
		if(g_pHero->GetCurState() == STATE::SIT)
			g_HeroInput.OnSkill(eACTION_SIT_AND_STAND + eACTION_START_SKILL_CODE);
		EndVendorSyn();

		m_bShow			  = FALSE;
		RECT rc;
		this->GetDialogWZ()->GetClientRect(&rc);
		POINT pt = { rc.left, rc.top };
		this->m_pVendorMan->SetWinPos(pt);
    }

    this->GetUIMan()->ShowWindow(InterfaceManager::DIALOG_EQUIP, val);
}

//------------------------------------------------------------------------------
/**
*/
void          
uiVendorSell::Exit()
{
	if (this->m_bStarted)
	{
		EndVendorSyn();
	}
	else
	{
		this->ShowInterfaceWithSound(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if( pos < POS_MAX_SLOT ) 
            {
                CControlWZ * pControl = getControl(pos);
                if (pControl)
                {
                    RECT rcSlot = pControl->GetSizeRect();
                    MouseHandler::Instance()->ItemTransaction(SI_VENDOR_SELL, pos, &rcSlot);
                }
                // 슬롯에 대한 처리가 끝났으므로 다이얼로그 메시지 처리는 여기서 Break
                break;
            }

            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
                // 닫기
            case DIALOG_EXIT:
                {
					Exit();

                }
                break;

            case DIALOG_EDIT_SELLMSG:
                {
                }
                break;

            case DIALOG_BTN_INPUTMODIFY:
                {
                    if (this->m_bStarted)
                    {
                    }
                    else
                    {
						GET_DIALOG(IM_VENDOR_MANAGER::VENDOR_TITLE_POPUP)->ShowWindow(TRUE);
						uiBase * vendorTitlePopup = GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_TITLE_POPUP );
						if ( vendorTitlePopup )
						{
							vendorTitlePopup->ShowWindow(TRUE);
						}
                    }
                }
                break;

            case DIALOG_BTN_STARTEND:
                {
                    // - 게인상점을 시작할 수 있는 조건
                    // -- 반드시 한개 이상의 아이템이 거래대상으로 올려져야만 한다.
                    if (this->GetSlotNum())
                    {
                        this->VendingStartEnd();
                    }
                }
                break;

            case DIALOG_BTN_MODIFY:
                {
                    if (! m_bStarted)
                    {
						// 시작 전에는 단순하게 가격만 새로 책정
						if (this->GetSlotNum())
						{
							this->OpenItemModifyPopup(this->m_curPos);
						}
                    }
                }
                break;

            case DIALOG_BTN_SUBITEM:
                {
                    if (! m_bStarted)
                    {
						if (this->GetSlotNum())
						{
							this->ItemSub(this->m_curPos);
						}
                    }
                }
                break;
#ifdef _JBH_MOD_MONEY_OUTPUT
			case DIALOG_TXTVALUE_ITEMVALUTE:
			case DIALOG_TXT_COMMISION_VALUE:
				{
					m_bClickMoney = TRUE;
				}
				break;
#endif


			case DIALOG_BTN_PAUSE:
				{
					PauseVendorSyn();
				}
				break;

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
void			
uiVendorSell::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if ( pos >= POS_SLOT_BEGIN && pos < POS_SLOT_END )
			{
				if (!IsEmpty( pos ))
				{
					SCSlot & rSlot = GetSlot( pos );
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

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::OnUpdateSolarDialog()
{
    if (m_pItemUnitRenderer)
        m_pItemUnitRenderer->Update();

    if (g_pHero)
    {
        if (g_pHero->GetBeHaveState() == PLAYER_BEHAVE_IDLE_STATE)
        {
            this->ShowInterface(FALSE);
            return;
        }
    }

    this->updateVendingMsgs();
    this->updateVendingBtns();
    this->updateVendingCurrentItemInfos();

#ifdef _JBH_MOD_MONEY_OUTPUT
#if defined(__KR) || defined(_KOREA)
		this->updateTooltip(DIALOG_TXTVALUE_ITEMVALUTE , m_curMoney);
		this->updateTooltip(DIALOG_TXT_COMMISION_VALUE , m_curCommision);
#	endif //#if defined(__KR) || defined(_KOREA)
#endif

    if (this->m_bStarted)
    {
        if (!this->GetSlotNum())
        {
            this->VendingStartEnd();
        }
    }

	PauseButtonShowNHide();

}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::updateVendingMsgs()
{
    CCtrlStaticWZ * pControl;
    pControl = (CCtrlStaticWZ*)getControl(DIALOG_EDIT_SELLMSG);
	GlobalFunc::SetCaption( pControl, this->m_szVendorMsg );
    pControl->SetTextColor(WzColor_RGBA( 152, 208, 142, 255 ));
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::updateVendingBtns()
{
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];
    POSTYPE pos;
    CCtrlStaticWZ * pControl;

    pos = getCtrlIDToPos( m_wzId[DIALOG_BTN_STARTEND] );
    pControl = (CCtrlStaticWZ*)getControl(pos);

    if (this->m_bStarted)
	{
		//	끝내기
		const int	c_iEnd = 70402;
		g_InterfaceManager.GetInterfaceString( c_iEnd, szMessage, INTERFACE_STRING_LENGTH );
	}
    else
	{
		//	시작하기
		const int	c_iStart = 70401;
		g_InterfaceManager.GetInterfaceString( c_iStart, szMessage, INTERFACE_STRING_LENGTH );
	}

    if (!this->GetSlotNum())
    {
        pControl->SetTextColor(RGBA(100,100,100,255));
    }
    else
    {
        pControl->ShowWindowWZ(WZ_SHOW);
        pControl->SetTextColor(RGBA(227,179,27,255));
    }

	GlobalFunc::SetCaption( pControl, szMessage );

    pos = getCtrlIDToPos( m_wzId[DIALOG_BTN_MODIFY] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
	//	가격 수정
	g_InterfaceManager.GetInterfaceString( eST_CHANGE_MONEY, szMessage, INTERFACE_STRING_LENGTH );
    if (!this->GetSlotNum() || this->m_bStarted)
    {
        pControl->SetTextColor(RGBA(100,100,100,255));
    }
    else
    {
        pControl->ShowWindowWZ(WZ_SHOW);
        pControl->SetTextColor(RGBA(227,179,27,255));
    }
	GlobalFunc::SetCaption( pControl, szMessage );

    pos = getCtrlIDToPos( m_wzId[DIALOG_BTN_SUBITEM] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
	//	아이템 내리기
	g_InterfaceManager.GetInterfaceString( eST_SUBITEM_BUY_CANCEL, szMessage, INTERFACE_STRING_LENGTH );
    if (!this->GetSlotNum() || this->m_bStarted)
    {
        pControl->SetTextColor(RGBA(100,100,100,255));
    }
    else
    {
        pControl->ShowWindowWZ(WZ_SHOW);
        pControl->SetTextColor(RGBA(227,179,27,255));
    }
	GlobalFunc::SetCaption( pControl, szMessage );
    pos = getCtrlIDToPos( m_wzId[DIALOG_BTN_INPUTMODIFY] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
    if (pControl)
    {
        if (this->m_bStarted)
        {
            pControl->SetTextColor(RGBA(100,100,100,255));
        }
        else
        {
            pControl->SetTextColor(RGBA(227,179,27,255));
        }

		//	입력
		g_InterfaceManager.GetInterfaceString( 4535, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::SetCaption( pControl, szMessage );
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::updateVendingCurrentItemInfos()
{
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    POSTYPE pos;
    CCtrlStaticWZ * pControl;
    DWORD money_color;

    pos = getCtrlIDToPos( m_wzId[DIALOG_TXTVALUE_ITEMCOUNTS] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
    Sprintf (szMessage, _T("%d"), m_curAmouts);
    money_color = GlobalFunc::GetMoneyColor(m_curAmouts);
    pControl->SetTextColor(money_color);
	GlobalFunc::SetCaption( pControl, szMessage );

	// 총 판매 금액
    pos = getCtrlIDToPos( m_wzId[DIALOG_TXTVALUE_ITEMVALUTE] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
    setFormatMoney(pControl, m_curMoney);

#ifdef _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR
    CCtrlStaticWZ* text_control = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXTVALUE_ITEM_UNIT_VALUE));
    if (text_control)
    {
        MONEY unit_price = m_curAmouts != 0 ? (m_curMoney / m_curAmouts) : m_curMoney;
        money_color = GlobalFunc::GetMoneyColor(unit_price);
        text_control->SetTextColor(money_color);
        GlobalFunc::SetMoneyText(text_control, unit_price);
    }
#endif //_DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR
	

#ifdef _JBH_ADD_VENDOR_COMMISION
	// 수수료 뿌려주기
	pControl = (CCtrlStaticWZ *) getControl( DIALOG_TXT_COMMISION_VALUE);
	if( pControl )
	{
		m_curCommision =(MONEY) ( m_curMoney * (float)TRADE_COMMISSION_RATE);
		setFormatMoney(pControl , m_curCommision);
	}
#endif

}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::OnRenderSolarDialog()
{
    if (m_pItemUnitRenderer)
        m_pItemUnitRenderer->Render();

	// 슬롯에 선택된 아이템 표시하기 
	if(this->GetSlotNum() >0)
	{
		SCSlot& rSlot = this->GetSlot(m_curPos);
		if(rSlot.GetCode())
		{
			CControlWZ* pControl = this->getControl(m_curPos);
			RECT rt = pControl->GetSizeRect();
			g_InterfaceManager.RenderActivateMark( rt.left - 13, rt.top - 13);
		}
	}

	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
            if (m_MouseOverObject.pSlot && m_MouseOverObject.pSlot->GetCode())
            {
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{
					pUIMan->RegisterTooltipItem(m_MouseOverObject.pSlot, FALSE);
					pUIMan->SetTooltipDialogID(GetDialogKey());

				}

				if( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
				{
					uiToolTipMan* pUIMan2 = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager( UIMAN_TOOLTIP2 );
					if( pUIMan2 )
					{
						pUIMan2->RegisterHeroItemInfoTooltipFromSlot( m_MouseOverObject.pSlot );
					}
				}
            }

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = POS_SLOT_BEGIN; a < POS_SLOT_END; a++)
	{
		CControlWZ * pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}

//------------------------------------------------------------------------------
/**
    pSlot for not allocated in heap
*/
RC::eSLOT_INSERT_RESULT		uiVendorSell::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

    CControlWZ * pCtrl = getControl(AtPos);
    RECT rect = pCtrl->GetSizeRect();

	// about locking 
	ItemUp(rSlot.GetSlotIdx(), SI_VENDOR_SELL,rSlot.GetPos() ,AtPos, 0);
	SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
	rItemSlot.SetLock(TRUE);

    // 클라에서 따로 시리얼을 발급한다.
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rRealSlot = GetSlot(AtPos);
	rRealSlot.SetSerial(dwSerial);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rRealSlot.GetSerial(), rRealSlot.GetCode(), &rect, &rRealSlot ,pCtrl);
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

    ReFreshRenderSlot();

	return eInsertSlotResult;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
uiVendorSell::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    SCSlot& slot = this->GetSlot(AtPos);

    if (m_pItemUnitRenderer)
	{
        m_pItemUnitRenderer->RemoveItemUnit( slot.GetSerial() );
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

	g_SlotKeyGenerator.Restore( (DWORD)slot.GetSerial() );

	SCSlot& toSlot = this->GetSlot(AtPos);
    uiSCItemSlot& rToItemSlot = (uiSCItemSlot&)toSlot;

	if ( !IsEmpty(AtPos) )
	{
		SCSlotContainer * pInvenContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
		assert(pInvenContainer);
		if(pInvenContainer)
		{
			SCItemSlot & rFromSlot = (SCItemSlot &)pInvenContainer->GetSlot( rToItemSlot.GetFromPos() );
			rFromSlot.SetLock(FALSE);
		}
	}
	
    SCSlotContainer::DeleteSlot( AtPos, pSlotOut );

    this->ReFreshRenderSlot();

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit *	pRender = NULL;
    RECT				rcSlot;
    CControlWZ *		pCtrl = NULL;

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        SCItemSlot & rSlot = (SCItemSlot &)SCSlotContainer::GetSlot(i);

        // 보여줘야할 목록

        if ( !IsEmpty(i) )
        {	
			if(!m_pItemUnitRenderer)
				continue;

            pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

            if (pRender == NULL)
            {
                pCtrl = getControl(i);
                rcSlot = pCtrl->GetSizeRect();
                m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
            }

            if (pRender)
            {
                pRender->SetShow(TRUE);
            }
        }
    }
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------


#ifdef _JBH_MOD_MONEY_OUTPUT
void uiVendorSell::updateTooltip(POSTYPE atPos , MONEY money)
{
	if( m_MouseOverPostype )
	{
		if( m_MouseOverPostype != atPos)
		{
			return;
		}
	}
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	
	// 효율을 위하여 DIALOG_TXT_ITEMVALUE 컨트롤만 검사함.
	CControlWZ * pControl = getControl(atPos);

	if(pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				if( m_dwPendingTick == 0 )
				{
					m_dwPendingTick =  clock_function::GetTickCount();
				}
				bMouseOver = TRUE;
				m_MouseOverPostype = atPos;
			}
			else
			{
				m_dwPendingTick = 0;
				m_bClickMoney = FALSE;
				m_MouseOverPostype = NULL;
			}
		}
		else
		{
			m_dwPendingTick = 0;
			m_bClickMoney = FALSE;
			m_MouseOverPostype = NULL;
		}
	}
	
	DWORD dwTick = 0;
	if(  m_dwPendingTick > 0 )
	{
		dwTick = clock_function::GetTickCount() - m_dwPendingTick;
	}

	if ( ( bMouseOver  && dwTick > 2000) || (bMouseOver && m_bClickMoney) )
	{
		if( money > 0 )
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			memset(szMessage , 0 , sizeof( szMessage ) );
			GlobalFunc::ChangeMoneyToStringMoney( money , szMessage );
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if( pUIMan )
			{
				pUIMan->InitTooltip();
				pUIMan->RegisterTooltip(szMessage ,_tcslen(szMessage) , 25 );
				pUIMan->SetTooltipDialogID( GetDialogKey() );

			}
		}
	}
}
#endif