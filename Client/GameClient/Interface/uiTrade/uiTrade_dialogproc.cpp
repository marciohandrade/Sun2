//------------------------------------------------------------------------------
//  uiTrade_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiTrade.h"
#include "ItemUnitRenderer.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "uiTradeMan/uiTradeMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiVendorMan/uiVendorMan_def.h"

//------------------------------------------------------------------------------
WzID uiTrade::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("B001"),  // DLGPOS_SLOT00 = 0,
        StrToWzID("B002"),  // DLGPOS_SLOT01,
        StrToWzID("B003"),  // DLGPOS_SLOT02,
        StrToWzID("B004"),  // DLGPOS_SLOT03,
        StrToWzID("B005"),  // DLGPOS_SLOT04,
        StrToWzID("B006"),  // DLGPOS_SLOT05,
        StrToWzID("B007"),  // DLGPOS_SLOT06,
        StrToWzID("B008"),  // DLGPOS_SLOT07,
        StrToWzID("B009"),  // DLGPOS_SLOT08,
        StrToWzID("B010"),  // DLGPOS_SLOT09,

        StrToWzID("B011"),  // DLGPOS_SLOT10,
        StrToWzID("B012"),  // DLGPOS_SLOT11,
        StrToWzID("B013"),  // DLGPOS_SLOT12,
        StrToWzID("B014"),  // DLGPOS_SLOT13,
        StrToWzID("B015"),  // DLGPOS_SLOT14,
        StrToWzID("B016"),  // DLGPOS_SLOT15,
        StrToWzID("B017"),  // DLGPOS_SLOT16,
        StrToWzID("B018"),  // DLGPOS_SLOT17,
        StrToWzID("B019"),  // DLGPOS_SLOT18,
        StrToWzID("B020"),  // DLGPOS_SLOT19,

        StrToWzID("B900"),  // DLGPOS_BTN_1_MONEY,
        StrToWzID("B901"),  // DLGPOS_BTN_2_MONEY,

        StrToWzID("B100"),  // DLGPOS_BTN_COMPLETE,    // 거래 수락
        StrToWzID("B101"),  // DLGPOS_BTN_ACCEPT,      // 거래 제시
        StrToWzID("B555"),  // DLGPOS_BTN_CANCEL,      // 거래 취소

        StrToWzID("T001"),  // DLGPOS_TXT_1_ITEM_NAME,
        StrToWzID("T003"),  // DLGPOS_TXT_2_ITEM_NAME,

        StrToWzID("T002"),  // DLGPOS_TXT_1_STATUS,
        StrToWzID("T004"),  // DLGPOS_TXT_2_STATUS,

        StrToWzID("T008"),  // DLGPOS_TXT_1_MONEY,
        StrToWzID("T009"),  // DLGPOS_TXT_2_MONEY,

#ifdef _NA_000000_20130114_RENEWER_UI
#else
        StrToWzID("T005"),  // DLGPOS_TXT_COMPLETE,    // 거래 수락
        StrToWzID("T006"),  // DLGPOS_TXT_ACCEPT,      // 거래 제시
        StrToWzID("T007"),  // DLGPOS_TXT_CANCEL,      // 거래 취소
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

        StrToWzID("B999"),  // DLGPOS_BTN_EXIT,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiTrade::OnShowWindow( BOOL val )
{
    //this->m_bShow = val;

    if (val)
    {
        uiBase* pDlg = GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL );
		if ( pDlg->IsVisible() )
		{
			pDlg->ShowWindow(FALSE);
		}
        SolarDialog * system_dialog = g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SYSTEM);
        if ((system_dialog != NULL) && (system_dialog->IsVisible() == TRUE))
        {
            system_dialog->ShowWindow(FALSE);
        }

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_trade;
        msg.wParam = InterfaceManager::DIALOG_TRADE;
        msg.DoSomething = GlobalFunc::DoTrade;
        g_KeyQueueManager.PushBack(msg);

        ClearCheckItem();

        //MoveWindowWZ( -1.0f, 120.0f );

        this->GetUIMan()->ShowDialog(InterfaceManager::DIALOG_EQUIP, TRUE);

        this->InitItemRender();

        this->Unlock();
    }
    else
    {
        FlushUnitDraw();

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_trade);
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		MouseHandler::Instance()->RollbackItemAtHand();
        MouseHandler::Instance()->RemoveItemAtHand();

#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
		GetManager()->ShowNumTradeUIDialog( FALSE );
#else
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_INPUT_NUM, FALSE);
#endif
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::MessageProc( SI_MESSAGE * pMessage )
{

	if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
	    {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if( pos < (DLGPOS_SLOT09+1)) 
            {
				//거래중 이라면 정상작동, 거래제시 했다면 피킹 금지
				if(TRADE_STATUS_IDLE == this->GetManager()->GetTradeInfo1().m_status)
				{
					CControlWZ * pControl = getControl(pos);
					if (pControl)
					{
						RECT rcSlot = pControl->GetSizeRect();
						MouseHandler::Instance()->ItemTransaction(SI_TRADE, pos, &rcSlot);
					}
				}
            }

            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
                // 거래 수락
            case DLGPOS_BTN_ACCEPT:
                {
                    this->GetManager()->TEST_Accept();
                }
                break;

                // 거래 제시
            case DLGPOS_BTN_PROPOSAL:
                {
                    this->GetManager()->TEST_Proposal();
                }
                break;

                // 돈 입력창 버튼
            case DLGPOS_BTN_1_MONEY:
                {
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
					GetManager()->ShowNumTradeUIDialog( TRUE );
#else
					this->GetManager()->PopupTradeMoney();
#endif
                }
                break;

                // 거래 취소
            case DLGPOS_BTN_CANCEL:
            case DLGPOS_BTN_EXIT:
                {
                    this->GetManager()->SendTradeCancel();
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
uiTrade::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos >= DLGPOS_SLOT00 && pos < (DLGPOS_SLOT19+1))
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
uiTrade::OnUpdateSolarDialog()
{
    if (m_pItemUnitRenderer)
        m_pItemUnitRenderer->Update();

    this->updateStatus();

    this->GetManager()->CheckEndTradingCauseRanged();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::updateStatus()
{
    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    uicTradeInfo tradeInfo1 = this->GetManager()->GetTradeInfo1();
    uicTradeInfo tradeInfo2 = this->GetManager()->GetTradeInfo2();

    //
    // 누가 제시했는가?
    //
    if (tradeInfo1.m_isSet)
    {
        //	%s 이(가) 제시한 아이템
        g_InterfaceManager.GetInterfaceString( eST_NAMECODE_TRADE_STATE_REQUEST, szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, szText, tradeInfo1.m_strName.c_str() );
    }
    else
    {
        //	?? 이(가) 제시한 아이템
        g_InterfaceManager.GetInterfaceString( eST_NAMECODE_TRADE_STATE_REQUEST, szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, szText, _T("??") );
    }
    
    this->m_pTxtItemNames[0]->SetTextColor(RGBA(255, 133, 50, 255));
	GlobalFunc::SetCaption( this->m_pTxtItemNames[0], szMessage );

    if (tradeInfo2.m_isSet)
    {
        //	%s 이(가) 제시한 아이템
        g_InterfaceManager.GetInterfaceString( eST_NAMECODE_TRADE_STATE_REQUEST, szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, szText, tradeInfo2.m_strName.c_str() );
    }
    else
    {
        //	?? 이(가) 제시한 아이템
        g_InterfaceManager.GetInterfaceString( eST_NAMECODE_TRADE_STATE_REQUEST, szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, szText, _T("??") );
    }

    this->m_pTxtItemNames[1]->SetTextColor(RGBA(255, 133, 50, 255));
	GlobalFunc::SetCaption( this->m_pTxtItemNames[1], szMessage );

    //
    // 거래 관련 버튼들의 문자 출력
    //
    //	거래 완료
    g_InterfaceManager.GetInterfaceString( eST_TRADE_STATE_DONE_TEXT, szMessage, INTERFACE_STRING_LENGTH );
    if (tradeInfo1.m_status == TRADE_STATUS_PROPOSAL
     && tradeInfo2.m_status >= TRADE_STATUS_PROPOSAL)
    {
#ifdef _NA_000000_20130114_RENEWER_UI
        this->m_pBtnComplete->SetTextColor(RGBA(227,179,27,255));
#else
		this->m_pTxtComplete->SetTextColor(RGBA(227,179,27,255));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
    }
    else
	{
#ifdef _NA_000000_20130114_RENEWER_UI
        this->m_pBtnComplete->SetTextColor(RGBA(100,100,100,255));
#else
		this->m_pTxtComplete->SetTextColor(RGBA(100,100,100,255));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}

#ifdef _NA_000000_20130114_RENEWER_UI
	GlobalFunc::SetCaption( this->m_pBtnComplete, szMessage );
#else
	GlobalFunc::SetCaption( this->m_pTxtComplete, szMessage );
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	
    if (tradeInfo1.m_status == TRADE_STATUS_IDLE)
	{
		g_InterfaceManager.GetInterfaceString( eST_TRADE_STATE_REQUEST_TEXT, szMessage, INTERFACE_STRING_LENGTH );	//거래제시
#ifdef _NA_000000_20130114_RENEWER_UI			
		this->m_pBtnAccept->SetTextColor(RGBA(227,179,27,255));
#else
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}
    else if (tradeInfo1.m_status == TRADE_STATUS_PROPOSAL)
	{
		g_InterfaceManager.GetInterfaceString( 886, szMessage, INTERFACE_STRING_LENGTH );	// 물품수정
#ifdef _NA_000000_20130114_RENEWER_UI
		this->m_pBtnAccept->SetTextColor(RGBA(227,179,27,255));
#else
		this->m_pTxtAccept->SetTextColor(RGBA(227,179,27,255));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}
	else
	{
		g_InterfaceManager.GetInterfaceString( eST_TRADE_STATE_REQUEST_TEXT, szMessage, INTERFACE_STRING_LENGTH );
#ifdef _NA_000000_20130114_RENEWER_UI
		this->m_pBtnAccept->SetTextColor(RGBA(100,100,100,255));
#else
		this->m_pTxtAccept->SetTextColor(RGBA(100,100,100,255));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}
#ifdef _NA_000000_20130114_RENEWER_UI
	GlobalFunc::SetCaption( this->m_pBtnAccept, szMessage );
#else
	GlobalFunc::SetCaption( this->m_pTxtAccept, szMessage );
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    //	취소
    g_InterfaceManager.GetInterfaceString( eST_CANCEL, szMessage, INTERFACE_STRING_LENGTH );
#ifdef _NA_000000_20130114_RENEWER_UI
    this->m_pBtnCancel->SetTextColor( RGBA(227,179,27,255) );
	GlobalFunc::SetCaption( this->m_pBtnCancel, szMessage );
#else
	this->m_pTxtCancel->SetTextColor( RGBA(227,179,27,255) );
	GlobalFunc::SetCaption( this->m_pTxtCancel, szMessage );
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
    //
    // 거래 상태 출력
    //
    if (tradeInfo1.m_status < (TRADE_STATUS_ACCEPT+1))
    {
        g_InterfaceManager.GetInterfaceString( (eST_TRADE_STATE + tradeInfo1.m_status), szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, _T("%s"), szText );
    }
    else
    {
        Sprintf( szMessage, _T("??") );
    }
    this->m_pTxtStatus[0]->SetTextColor(RGBA(200, 100, 25, 150));
	GlobalFunc::SetCaption( this->m_pTxtStatus[0], szMessage );

    if (tradeInfo2.m_status < (TRADE_STATUS_ACCEPT+1))
    {
        g_InterfaceManager.GetInterfaceString( (eST_TRADE_STATE + tradeInfo2.m_status), szText, INTERFACE_STRING_LENGTH );
        Sprintf( szMessage, _T("%s"), szText );
    }
    else
    {
        Sprintf( szMessage, _T("??") );
    }

    this->m_pTxtStatus[1]->SetTextColor(RGBA(200, 100, 25, 150));
	GlobalFunc::SetCaption( this->m_pTxtStatus[1], szMessage );

    //
    // 거래중인 금액 표시
    //
	TCHAR szMoney[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMoneyFinal[INTERFACE_STRING_LENGTH] = {0,};

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

	Sprintf(szMoney, _T("%I64u"), tradeInfo1.m_DealMoney);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMoneyFinal, sizeof(szMoneyFinal));
    DWORD money_color;
    money_color = GlobalFunc::GetMoneyColor(tradeInfo1.m_DealMoney);
    this->m_pTxtMoneys[0]->SetTextColor(money_color);
	GlobalFunc::SetCaption( this->m_pTxtMoneys[0], szMoneyFinal );

	Sprintf(szMoney, _T("%I64u"), tradeInfo2.m_DealMoney);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMoneyFinal, sizeof(szMoneyFinal));
    money_color = GlobalFunc::GetMoneyColor(tradeInfo2.m_DealMoney);
    this->m_pTxtMoneys[1]->SetTextColor(money_color);
	GlobalFunc::SetCaption( this->m_pTxtMoneys[1], szMoneyFinal );
}

//------------------------------------------------------------------------------
/**
*/
void uiTrade::RenderCheckItem()
{
    if (m_pItemUnitRenderer == NULL)
    {
        return;
    }   

    for (POSTYPE idx = 0; idx < GetSlotSize2(); ++idx)
    {
        if (IsEmpty(idx + GetSlotSize1()))
        {
            continue;
        }

        if (GetCheckItem(idx) == false)
        {
            continue;
        }
        
        if (m_pSlotBtnControls2[idx])
        {
            g_InterfaceManager.RenderCheckItem(m_pSlotBtnControls2[idx]->GetSizeRect());
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void
uiTrade::OnRenderSolarDialog()
{
    if (m_pItemUnitRenderer)
    {
        m_pItemUnitRenderer->Render();
        RenderCheckItem();
    }

    this->renderToolTip();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::renderToolTip()
{
	if ( this->m_MouseOverObject.pSlot && 
		 eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
	{
        if (m_MouseOverObject.pSlot->GetCode() != 0)
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterTooltipItem(this->m_MouseOverObject.pSlot, TRUE);
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
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = DLGPOS_SLOT00; a < (DLGPOS_SLOT19+1); a++)
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
//  EOF
//------------------------------------------------------------------------------
