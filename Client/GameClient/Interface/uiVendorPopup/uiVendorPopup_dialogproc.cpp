//------------------------------------------------------------------------------
//  uiVendorPopup_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan/uiVendorMan.h"
#include "uiVendorPopup.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "InventoryDialog.h"
#include "SCItemSlot.h"
#include "ItemInfoParser.h"
#include "ItemFormula.h"
#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "interfacemanager.h"
#include "globalfunc.h"

const BYTE c_btMaxEditNum = 17;

//------------------------------------------------------------------------------
WzID uiVendorPopup::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("T001"),  // DIALOG_TXT_ORG_VALUE
        StrToWzID("T002"),  // DIALOG_TXT_AFTER_VALUE
        StrToWzID("TI00"),  // DIALOG_INPUT_ORG_VALUE
        StrToWzID("E001"),  // DIALOG_EDIT_AFTER_VALUE
        StrToWzID("TI01"),  // DIALOG_OK
        StrToWzID("TI02"),  // DIALOG_CANCEL
};

void _vendorpopup_restoreFocus()
{
	uiVendorPopup* pVendorPopup = GET_CAST_DIALOG( uiVendorPopup, IM_VENDOR_MANAGER::VENDOR_POPUP );
    pVendorPopup->SetFocus();
}

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiVendorPopup::OnShowWindow( BOOL val )
{
    //this->m_bShow = val;

 	uiBase* vendorBuy	= GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_BUY );
	uiBase* vendorSell	= GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL );

    if (val)
    {
        SolarDialog::MoveCenter();

        CCtrlDxIMEEditBox * m_pChatEditCtrl;
        m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( DIALOG_EDIT_AFTER_VALUE );
        m_pChatEditCtrl->ClearText();
		GlobalFunc::SetFocus( (SolarDialog *)this, m_pChatEditCtrl );

		m_pChatEditCtrl->SetColorCaret(RGBA(255,100,100,255));
        m_pChatEditCtrl->SetColorText(RGBA(255,255,255,255));
        m_pChatEditCtrl->SetFontID(StrToWzID("mn12"));
        m_pChatEditCtrl->SetLimitText(c_btMaxEditNum);

        this->m_bModify = false;
        this->m_bRequiredQuery = false;
        this->m_bFocus = true;
        this->m_status = ASK_STATUS_NORMAL;

        vendorBuy->OnLock();
        vendorSell->OnLock();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_vendor_popup;
		msg.wParam=InterfaceManager::DIALOG_VENDOR_POPUP;
		msg.DoSomething=GlobalFunc::DoVentorPopup;
		g_KeyQueueManager.PushBack(msg);


	
    }
    else
    {
        this->m_status = ASK_STATUS_CANCEL;
        
        vendorBuy->OffLock();
        vendorSell->OffLock();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_vendor_popup);

    }
}

//------------------------------------------------------------------------------
/**
*/
void 
uiVendorPopup::Exit()
{
	this->m_status = ASK_STATUS_CANCEL;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorPopup::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DIALOG_EDIT_AFTER_VALUE:
				{
					m_bFocus = false;
					m_status = ASK_STATUS_OK;
				}
				break;
			}
        }
        break;

	case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
                // 닫기
            case DIALOG_CANCEL:
                {
                    Exit();
                }
                break;

            case DIALOG_EDIT_AFTER_VALUE:
                {
                    m_bFocus = true;
                }
                break;

            case DIALOG_OK:
                {
                    m_bFocus = false;
                    m_status = ASK_STATUS_OK;
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

//------------------------------------------------------------------------------
/**
*/
void
uiVendorPopup::OnUpdateSolarDialog()
{
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];
    TCHAR   szMessage2[INTERFACE_STRING_LENGTH];
    POSTYPE pos;
    CCtrlStaticWZ * pControl;


	//	기존 금액
    pos = getCtrlIDToPos( m_wzId[DIALOG_TXT_ORG_VALUE] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
	g_InterfaceManager.GetInterfaceString( eST_EXISTING_MONEY, szMessage, INTERFACE_STRING_LENGTH );
    pControl->SetTextColor(RGBA(227,179,27,255));
	GlobalFunc::SetCaption( pControl, szMessage );

	//	변경 금액
    pos = getCtrlIDToPos( m_wzId[DIALOG_TXT_AFTER_VALUE] );
    pControl = (CCtrlStaticWZ*)getControl(pos);
    g_InterfaceManager.GetInterfaceString( eST_CHANGED_MONEY, szMessage, INTERFACE_STRING_LENGTH );
    pControl->SetTextColor(RGBA(227,179,27,255));
	GlobalFunc::SetCaption( pControl, szMessage );

    if (this->m_rItemSlot)
    {
        if (this->m_bModify)
        {
            uiSCItemSlot* uiItemSlot = (uiSCItemSlot *)this->m_rItemSlot;

            pos = getCtrlIDToPos( m_wzId[DIALOG_INPUT_ORG_VALUE] );
            pControl = (CCtrlStaticWZ*)getControl(pos);
            Sprintf (szMessage, _T("%I64u"), uiItemSlot->GetMoney());
     
            std::deque<char> strReal;

            strReal.clear();

            size_t length = lstrlen (szMessage);

            for (size_t i=0, j=0; i<length; ++i)
            {
                if (!(i%3) && i!=0)
                {
                    strReal.push_front(',');
                }

                strReal.push_front(szMessage[(length-1)-i]);
            }

            for (int i=0; i<strReal.size(); ++i)
            {
                szMessage2[i] = strReal[i];
            }
			int i;
            szMessage2[i] = 0;
            
            pControl->SetTextColor(RGBA(227,179,27,255));
			GlobalFunc::SetCaption( pControl, szMessage2 );
        }
        else
        {
            MONEY PriceOfGoods;

            BASE_ITEMINFO *pInfo = ItemInfoParser::Instance()->GetItemInfo(this->m_rItemSlot->GetCode());
            assert(pInfo);

            // 되팔때

			PriceOfGoods = this->m_rItemSlot->GetPriceForSell();

            pos = getCtrlIDToPos( m_wzId[DIALOG_INPUT_ORG_VALUE] );
            pControl = (CCtrlStaticWZ*)getControl(pos);
            Sprintf (szMessage, _T("%I64u"), PriceOfGoods);

            std::deque<char> strReal;

            strReal.clear();

            size_t length = lstrlen (szMessage);

            for (size_t i=0, j=0; i<length; ++i)
            {
                if (!(i%3) && i!=0)
                {
                    strReal.push_front(',');
                }

                strReal.push_front(szMessage[(length-1)-i]);
            }

            for (int i=0; i<strReal.size(); ++i)
            {
                szMessage2[i] = strReal[i];
            }
			int i;
            szMessage2[i] = 0;

            pControl->SetTextColor(RGBA(227,179,27,255));
			GlobalFunc::SetCaption( pControl, szMessage2 );
        }
    }
    else
    {
        pos = getCtrlIDToPos( m_wzId[DIALOG_INPUT_ORG_VALUE] );
        pControl = (CCtrlStaticWZ*)getControl(pos);
        Sprintf (szMessage, _T("0"));
        pControl->SetTextColor(RGBA(227,179,27,255));
		GlobalFunc::SetCaption( pControl, szMessage );
    }

	CCtrlDxIMEEditBox * m_pChatEditCtrl;
	m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( DIALOG_EDIT_AFTER_VALUE );
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};

	m_pChatEditCtrl->GetTextWZ(m_szMoneyOld, INTERFACE_STRING_LENGTH);
	int iLen = strlen(m_szMoneyOld);
	int j = 0;
	for(int i = 0; i < iLen; ++i, ++j)
	{
		if(',' != m_szMoneyOld[j])
		{
			szTemp[i] = m_szMoneyOld[j];
		}
		else 
		{
			++j;
			szTemp[i] = m_szMoneyOld[j];
		}
	}
	if(isdigit(szTemp[0]))
	{
		if(c_btMaxEditNum > strlen(szTemp)) //에디트 컨트롤 범위 넘지 않게
		{
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szTemp, &nFmt, m_szMoneyNew, sizeof(m_szMoneyNew));
			m_pChatEditCtrl->SetTextWZ(m_szMoneyNew);
		}
		else
		{
			m_pChatEditCtrl->SetTextWZ("");
		}
	}
	else
	{
		m_pChatEditCtrl->SetTextWZ("");
	}

    if (this->m_status == ASK_STATUS_OK)
    {
		TCHAR    m_szAfterValue[64] = {0,};

        CCtrlDxIMEEditBox * m_pChatEditCtrl;
        m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( DIALOG_EDIT_AFTER_VALUE );
		StrnCopy(m_szAfterValue, szTemp, 63);

        if (lstrlen(m_szAfterValue)==0 || (MONEY) (_ttoi64(m_szAfterValue)) <= 0)
        {
			//	금액을 입력하세요.
			g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            
			GlobalFunc::SetFocus( (SolarDialog *)this, m_pChatEditCtrl );
			this->m_status = ASK_STATUS_NORMAL;
        }
        else
        {
            if (this->checkDigit(m_szAfterValue))
            {
                if (this->checkLimit((MONEY)_ttoi64(m_szAfterValue)))
                {
                    MONEY cur_Money = (MONEY)_ttoi64(m_szAfterValue);

					uiVendorSell* pUIVendorSell	= GET_CAST_DIALOG( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );

                    if (this->m_bModify)
                    {
                        pUIVendorSell->ItemModify(this->m_FromPos, cur_Money, this->m_bRequiredQuery);
                    }
                    else
                    {
                        pUIVendorSell->ItemUp(this->m_FromSlotIdx,
                            this->m_ToSlotIdx,
                            this->m_FromPos,
                            this->m_ToPos,
                            cur_Money);
                    }

                    this->ShowInterface(FALSE);
                }
                else
                {
					//	42억하임을 넘지 않도록 입력해 주세요.
					const int	c_iLimit42 = 70407;
					g_InterfaceManager.GetInterfaceString( c_iLimit42, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					GlobalFunc::SetFocus( (SolarDialog *)this, m_pChatEditCtrl );
                    this->m_status = ASK_STATUS_NORMAL;
                }
            }
            else
            {
				//	금액은 숫자로만 입력하십시요.
				g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY_IN_FIGURES, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                m_pChatEditCtrl->ClearText();                

				GlobalFunc::SetFocus( (SolarDialog *)this, m_pChatEditCtrl );

                this->m_status = ASK_STATUS_NORMAL;
            }
        }
    }
    else if (this->m_status == ASK_STATUS_CANCEL)
    {
		uiVendorSell* pUIVendorSell	= GET_CAST_DIALOG( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );

        if (this->m_bModify)
        {
            pUIVendorSell->ItemModifyCancel();
        }
        else
        {
            pUIVendorSell->ItemUpCencel(
				this->m_FromSlotIdx,
                this->m_ToSlotIdx,
                this->m_FromPos,
                this->m_ToPos);
        }

        this->ShowInterface(FALSE);
    }
}


//------------------------------------------------------------------------------
/**
*/
bool
uiVendorPopup::checkDigit(const TCHAR* str)
{
    int len = lstrlen(str);
    int i;

    for (i=0; i<len; ++i)
    {
        if (str[i]<_T('0') || str[i]>_T('9'))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiVendorPopup::checkLimit(MONEY money)
{
    // 액수 제한을 변경
    // UI 컨트롤(Edit, StaticText)의 크기 및 국가별 폰트를 고려할때 이정도가 적당한듯
    // 추후 UI 수정에 따라 제한 변경
    // 4200000000 ==> 9999999999999
    if (money > 9999999999999)
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorPopup::SetFocus()
{
    CCtrlDxIMEEditBox * m_pChatEditCtrl;
    m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( DIALOG_EDIT_AFTER_VALUE );
	GlobalFunc::SetFocus( (SolarDialog *)this, m_pChatEditCtrl );
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
