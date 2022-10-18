//------------------------------------------------------------------------------
//  uiVendorBuy_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan/uiVendorMan.h"
#include "uiVendorBuy.h"
#include "ItemUnitRenderer.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "uiVendorSell/uiVendorSell_def.h"
#include "ChatDialog.h"
#include "ObjectManager.h"
#include "slotkeygenerator.h"
#include "CursorChangeTransact.h"
#include "hero.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "HeroActionInput.h"

#include "uiChatMan/uiChatMan_def.h"

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
#include "uiVendorBuyPopup.h"
#endif
#include "GameFunc.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiSystemMan/uiSystemMan.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
//------------------------------------------------------------------------------

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#include "../uiVendorMan/uiVendorSearch/uiVendorSearch.h"
#endif
WzID uiVendorBuy::m_wzId[DIALOG_MAX] = 
{ 
#ifdef _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
		StrToWzID("B005"),  // DIALOG_SLOT00
		StrToWzID("B006"),  // DIALOG_SLOT01
		StrToWzID("B008"),  // DIALOG_SLOT02
		StrToWzID("B007"),  // DIALOG_SLOT03
		StrToWzID("B011"),  // DIALOG_SLOT04
		StrToWzID("B012"),  // DIALOG_SLOT05
		StrToWzID("B010"),  // DIALOG_SLOT06
		StrToWzID("B009"),  // DIALOG_SLOT07

		StrToWzID("B020"),  // DIALOG_SLOT10
		StrToWzID("B019"),  // DIALOG_SLOT11
		StrToWzID("B018"),  // DIALOG_SLOT12
		StrToWzID("B017"),  // DIALOG_SLOT13
		StrToWzID("B015"),  // DIALOG_SLOT14
		StrToWzID("B016"),  // DIALOG_SLOT15
		StrToWzID("B014"),  // DIALOG_SLOT16
		StrToWzID("B013"),  // DIALOG_SLOT17

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
		StrToWzID("B023"),  // DIALOG_SLOT34
		StrToWzID("B024"),  // DIALOG_SLOT35
		StrToWzID("B022"),  // DIALOG_SLOT36
		StrToWzID("B021"),  // DIALOG_SLOT37

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

        StrToWzID("T006"),  // DIALOG_TXT_SELLITEM,
        StrToWzID("e000"),  // DIALOG_EDIT_SELLMSG
        StrToWzID("T001"),  // DIALOG_TXT_SELLMSG,

        StrToWzID("T002"),  // DIALOG_TXT_ITEMCOUNTS,
        StrToWzID("T004"),  // DIALOG_TXT_ITEMVALUE,
        StrToWzID("T003"),  // DIALOG_TXTVALUE_ITEMCOUNTS,
        StrToWzID("T005"),  // DIALOG_TXTVALUE_ITEMVALUTE,
        StrToWzID("S001"),  // DIALOG_TXT_ITEM_UNIT_PRICE,

        StrToWzID("TI01"),  // DIALOG_BTN_ITEMBUY,

        StrToWzID("B000"),  // DIALOG_EXIT,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiVendorBuy::OnShowWindow( BOOL val )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (val)
    {
        // 이러한 경우에는 개인상점 구매창을 열 수 없다.
        if (g_pHero == NULL)
        {   
            this->ShowInterface(FALSE);
            return;
        }

        if (g_pHero->GetBeHaveState() != PLAYER_BEHAVE_IDLE_STATE)
        {
            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                

            if (pChatDlg)
            {
                pChatDlg->SetFocusChatEdit();
                pChatDlg->ShowWindow(TRUE);
            }

			//	현재 다른 용무 중으로 인하여 개인 상점을 시작할 수 없습니다.
			g_InterfaceManager.GetInterfaceString( eST_CANNOT_CREATE_VENDOR, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }

        if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_CREATE))
        {
            this->ShowInterface(FALSE);
            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                

            if (pChatDlg)
            {
                pChatDlg->SetFocusChatEdit();
                pChatDlg->ShowWindow(TRUE);
            }
			//	배틀 존 생성중에는 개인상점 구매창을 열 수 없습니다.
			const int	c_iNotOpenVendorInCreateBattleZone = 70413;
			g_InterfaceManager.GetInterfaceString( c_iNotOpenVendorInCreateBattleZone, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            this->GetUIMan()->ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
            return;
        }

        if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST))
        {
            this->ShowInterface(FALSE);
            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                

            if (pChatDlg)
            {
                pChatDlg->SetFocusChatEdit();
                pChatDlg->ShowWindow(TRUE);
            }
			//	배틀 존 목록을 보고계시는 동안 개인상점 구매창을 열 수 없습니다.
			const int	c_iNotOpenVendorInBattleZoneList = 70412;
			g_InterfaceManager.GetInterfaceString( c_iNotOpenVendorInBattleZoneList, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            this->GetUIMan()->ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
            return;
        }

        if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLE_LOBBY))
        {
            this->ShowInterface(FALSE);
            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                

            if (pChatDlg)
            {
                pChatDlg->SetFocusChatEdit();
                pChatDlg->ShowWindow(TRUE);
            }
			//	배틀 존 대기실에서는 개인상점 구매창을 열 수 없습니다.
			const int	c_iNotOpenVendorInBattleZone = 70411;
			g_InterfaceManager.GetInterfaceString( c_iNotOpenVendorInBattleZone, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            this->GetUIMan()->ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
            return;
        }

        if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_SHOP))
        {
            this->ShowInterface(FALSE);
            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                

            if (pChatDlg)
            {
                pChatDlg->SetFocusChatEdit();
                pChatDlg->ShowWindow(TRUE);
            }
			//	상점에서 용무를 마치기 전에는 개인상점 구매창을 열 수 없습니다.
			const int	c_iNotOpenVendor = 70410;
			g_InterfaceManager.GetInterfaceString( c_iNotOpenVendor, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            this->GetUIMan()->ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
            return;
        }
        ClearCheckItem();

		POINT pt = m_pVendorMan->GetWinPos();
        GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
        
        
		this->m_curPos = 0;
		this->m_curMoney = 0;
		this->m_curAmouts = 0;

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
		uiVendorSearch * pVendorSearch = GET_CAST_DIALOG( uiVendorSearch, IM_VENDOR_MANAGER::VENDOR_SEARCH );
		if(pVendorSearch && pVendorSearch->IsVisibleWindow())
		{
			m_curPos = pVendorSearch->GetVendorSelectItem().BuySlotPos_;
			m_curMoney = pVendorSearch->GetVendorSelectItem().price * pVendorSearch->GetVendorSelectItem().count;
			m_curAmouts = pVendorSearch->GetVendorSelectItem().count;

		}
		
		//보호코드
		this->m_curPos = this->m_curPos > 40 ? 0 : this->m_curPos ;

#endif

        this->OffLock();

		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString( eST_VENDOR_TEXT, szMessage, INTERFACE_STRING_LENGTH );
		StrnCopy(this->m_szVendorMsg, szMessage, MAX_VENDOR_TITLE_LENGTH);

        CCtrlStaticWZ * pControl;
        pControl = (CCtrlStaticWZ*)getControl(DIALOG_EDIT_SELLMSG);
        if (pControl)
        {
			GlobalFunc::SetCaption( pControl, this->m_szVendorMsg );
            pControl->SetTextColor(WzColor_RGBA( 152, 208, 142, 255 ));
        }

        this->ResetItemRenderer();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_vendor_buy;
		msg.wParam=InterfaceManager::DIALOG_VENDOR_BUY;
		msg.DoSomething=GlobalFunc::DoVentorBuy;
		g_KeyQueueManager.PushBack(msg);

        this->GetUIMan()->AttachFirstDialog(InterfaceManager::DIALOG_VENDOR_BUY);
        g_pHero->StopMove();
        g_pHero->ResetMoveFlag();
        g_HeroInput.SetAutoRun(FALSE);
        g_HeroInput.SetFollowState(FALSE);
        g_HeroInput.SetAutoAttack(FALSE);
    }
    else
    {
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
		uiBase* pVendorDlg	= GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_BUY_POPUP );
		if( pVendorDlg)
		{
			pVendorDlg->ShowWindow(FALSE);
		}
#endif
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_vendor_buy);

        ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

        if (pChatDlg)
        {
            pChatDlg->SetFocusChatEdit();
            pChatDlg->ShowWindow(FALSE);
        }

        if (g_pHero)
		{
            g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
		}

        FlushUnitDraw();

        this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_VENDOR_BUY);

		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

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
uiVendorBuy::Exit()
{
    this->ShowInterface(FALSE);
    FlushUnitDraw();
    this->QueryViewEnd();
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if( pos < POS_MAX_SLOT ) 
            {
                SCSlot& rSlot = this->GetSlot(pos);

                if (rSlot.GetCode())
                {
                    // 슬롯에 채워져 있는 아이템을 클릭했을 경우

                    if (MouseHandler::Instance()->IsExistItemAtHand())
                    {
                        MouseHandler::Instance()->RollbackItemAtHand();
                    }

                    uiSCItemSlot& rItemSlot = (uiSCItemSlot&)rSlot;

                    if (rItemSlot.CanEquip())
                    {
                        this->m_curAmouts   = 1;
                    }
                    else
                    {
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
						if( rItemSlot.GetNum() > 1 &&  g_Input.GetState(DIK_LSHIFT, KS_KEY) )
						{
							uiVendorBuyPopup* pVendorBuyPopup	= GET_CAST_DIALOG( uiVendorBuyPopup, IM_VENDOR_MANAGER::VENDOR_BUY_POPUP );
							if ( pVendorBuyPopup )
							{
								pVendorBuyPopup->ShowWindow(TRUE);
								pVendorBuyPopup->SetMaxNum( rItemSlot.GetNum() );
								pVendorBuyPopup->SetCurSlotPos( pos );
							}
						}
						else
						{
							this->m_curAmouts   = rItemSlot.GetNum();
						}
#else
                        this->m_curAmouts   = rItemSlot.GetNum();
                 
#endif
                    }
                    this->m_curMoney    = rItemSlot.GetMoney();
                    this->m_curPos      = pos;
                }
            }
        
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
                // 닫기
            case DIALOG_EXIT:
                {
                    this->ShowInterface(FALSE);
                    this->QueryViewEnd();
                }
                break;

            case DIALOG_BTN_ITEMBUY:
                {
                    if (!this->m_curMoney)
                    {
						//	우선, 구입하려는 아이템을 선택하세요.
						const int	c_iSelectItem = 70409;
						g_InterfaceManager.GetInterfaceString( c_iSelectItem, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
                    }
                    else
                    if (this->m_curMoney <= g_pHero->GetMoney())
                    {
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
						if (GlobalFunc::HeimTradingvolumeCheck(this->m_curMoney))
						{
							uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
							if (ui_system_manager_ptr)
							{
								ui_system_manager_ptr->Sys_SystemHeimTradingvolumeVendorBuy(GetDialogKey(), this->m_curPos, this->m_curMoney, true);
							}
						}
						else
						{
							this->SendItemBuySync(this->m_curPos);
						}
#else
                        this->SendItemBuySync(this->m_curPos);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
                    }
                    else
                    {
						//	현재 보유한 하임이 부족하여 아이템을 구매할 수 없습니다.
						const int	c_iNotEnoughMoney = 70408;
						g_InterfaceManager.GetInterfaceString( c_iNotEnoughMoney, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
                    }
                }
                break;

#ifdef _JBH_MOD_MONEY_OUTPUT
			case DIALOG_TXTVALUE_ITEMVALUTE:
				{
					m_bClickMoney = TRUE;
				}
				break;
#endif
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
uiVendorBuy::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			if (pos >= POS_SLOT_BEGIN && pos < POS_SLOT_END)
			{
				if (!IsEmpty( pos ))
				{
					SCSlot & rSlot = GetSlot( pos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;

                    SetCheckItem(pos, false);
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
uiVendorBuy::OnUpdateSolarDialog()
{
    if (m_pItemUnitRenderer)
        m_pItemUnitRenderer->Update();

    this->updateVendingCurrentItemInfos();

    CCtrlStaticWZ * pControl;
    pControl = (CCtrlStaticWZ*)getControl(DIALOG_EDIT_SELLMSG);
    if (pControl)
    {
		GlobalFunc::SetCaption( pControl, this->m_szVendorMsg );
        pControl->SetTextColor(WzColor_RGBA( 152, 208, 142, 255 ));
    }


#ifdef _JBH_MOD_MONEY_OUTPUT
#if defined(__KR) || defined(_KOREA)
		updateToolTip();
#	endif //#if defined(__KR) || defined(_KOREA)
#endif	

}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::updateVendingCurrentItemInfos()
{
    TCHAR string_value[INTERFACE_STRING_LENGTH];
    DWORD money_color;

    CCtrlStaticWZ* text_control = NULL;;
    text_control = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXTVALUE_ITEMCOUNTS));
    if (text_control)
    {
        Sprintf(string_value, _T("%d"), this->m_curAmouts);
        text_control->SetTextColor(RGBA(255,255,255,255));
        GlobalFunc::SetCaption(text_control, string_value);
    }    

    text_control = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXTVALUE_ITEMVALUTE));
    if (text_control)
    {
        money_color = GlobalFunc::GetMoneyColor(m_curMoney);
        text_control->SetTextColor(money_color);
        GlobalFunc::SetMoneyText(text_control, m_curMoney);
    }

    text_control = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXT_ITEM_UNIT_PRICE));
    if (text_control)
    {
        MONEY unit_price = m_curAmouts != 0 ? (m_curMoney / m_curAmouts) : m_curMoney;
        money_color = GlobalFunc::GetMoneyColor(unit_price);
        text_control->SetTextColor(money_color);
        GlobalFunc::SetMoneyText(text_control, unit_price);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiVendorBuy::RenderCheckItem()
{
    if (m_pItemUnitRenderer == NULL)
    {
        return;
    }   

    for (POSTYPE idx = 0; idx < GetMaxSlotNum(); ++idx)
    {
        if (IsEmpty(idx))
        {
            continue;
        }
        
        CControlWZ* slot_control = getControl(idx);
        if (GetCheckItem(idx) &&
            slot_control)
        {
            g_InterfaceManager.RenderCheckItem(slot_control->GetSizeRect());
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::OnRenderSolarDialog()
{
    if (m_pItemUnitRenderer)
    {
        m_pItemUnitRenderer->Render();
        RenderCheckItem();
    }
	
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
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
            {
				pUIMan->RegisterTooltipItem(m_MouseOverObject.pSlot, FALSE, kLimitStatCompositeNormal);
                pUIMan->RegisterTooltipVendorItemUnitPrice(m_MouseOverObject.pSlot);
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
RC::eSLOT_INSERT_RESULT		uiVendorBuy::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	if(!this->IsVisible())
		return RC::RC_INSERT_SLOT_FAILED;
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

    // 클라에서 따로 시리얼을 발급한다.
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rRealSlot = GetSlot(AtPos);
	rRealSlot.SetSerial(dwSerial);

    CControlWZ * pCtrl = getControl(AtPos);
    RECT rect = pCtrl->GetSizeRect();

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
uiVendorBuy::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
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

    SCSlotContainer::DeleteSlot( AtPos, pSlotOut );

    this->ReFreshRenderSlot();

	return TRUE;
}
void uiVendorBuy::Serialize( POSTYPE pos, BitStream & bitstream, eSLOT_SERIALIZE eType )
{
	SCItemSlotContainer::Serialize( pos , bitstream, eType );

	if( eType == eSLOT_SERIALIZE_CLIENT_SAVE )
	{
		// 클라에서 따로 시리얼을 발급한다.
		DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
		SCSlot & rRealSlot = GetSlot(pos);
		rRealSlot.SetSerial(dwSerial);

		CControlWZ * pCtrl = getControl(pos);
		RECT rect = pCtrl->GetSizeRect();

		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->AddItemUnit( rRealSlot.GetSerial(), rRealSlot.GetCode(), &rect, &rRealSlot ,pCtrl);
		}
		else
		{
			assert(!"m_pItemUnitRenderer == NULL");
		}

		ReFreshRenderSlot();
	}
}	
//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit *	pRender = NULL;
    RECT				rcSlot;
    CControlWZ *		pCtrl = NULL;

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        SCSlot & rSlot = SCSlotContainer::GetSlot(i);

        // 보여줘야할 목록

        if ( !IsEmpty(i) )
        {	
			if(!m_pItemUnitRenderer)
				continue;

            pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );
            SetCheckItem(i, GameFunc::IsCheckItem(rSlot));
            if (pRender = NULL)
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
/**
*/
#ifdef _JBH_MOD_MONEY_OUTPUT
void uiVendorBuy::updateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;

	// 효율을 위하여 DIALOG_TXT_ITEMVALUE 컨트롤만 검사함.
	CControlWZ * pControl = getControl(DIALOG_TXTVALUE_ITEMVALUTE);

	if(pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
				if( m_dwPendingTick == 0)
				{
					m_dwPendingTick = clock_function::GetTickCount();
				}
			}
			else
			{
				m_dwPendingTick = 0;
				m_bClickMoney = FALSE;
			}
		}
		else
		{
			m_dwPendingTick = 0;
			m_bClickMoney = FALSE;
		}
	}

	DWORD dwTick = 0;
	if( m_dwPendingTick > 0 )
	{
		dwTick = clock_function::GetTickCount() - m_dwPendingTick;
	}

	if ( (bMouseOver && dwTick > 2000 ) || m_bClickMoney )
	{
		if( m_curMoney > 0 )
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			memset(szMessage , 0 , sizeof( szMessage ) );
			GlobalFunc::ChangeMoneyToStringMoney( m_curMoney , szMessage );
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

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
