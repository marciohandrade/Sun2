#include "SunClientPrecompiledHeader.h"

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
#include "uiVendorBuyPopup.h"
#include "InterfaceManager.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "GlobalFunc.h"
#include "uiVendorSell/uiVendorSell_def.h"
#include "Hero.h"

WzID uiVendorBuyPopup::m_WzId[DLGPOS_MAX] = 
{
	StrToWzID("B005"),	//	DLGPOS_BTN_OK,
	StrToWzID("B006"),	//	DLGPOS_BTN_CANCLE,
	StrToWzID("B002"),	//	DLGPOS_BTN_LEFT,
	StrToWzID("B003"),	//	DLGPOS_BTN_RIGHT,
	StrToWzID("B001"),	//	DLGPOS_BTN_EXIT,
	StrToWzID("B004"),	//	DLGPOS_BTN_MAX,
	StrToWzID("E001"),	//	DLGPOS_EDIT_INPUT,
};

//------------------------------------------------------------------------------
/**
*/
uiVendorBuyPopup::uiVendorBuyPopup(InterfaceManager * pUIMan ) : uiBase(pUIMan)
{

}

//------------------------------------------------------------------------------
/**
*/
uiVendorBuyPopup::~uiVendorBuyPopup()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiVendorBuyPopup::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pEdit = static_cast<CCtrlDxIMEEditBox *> ( getControl( DLGPOS_EDIT_INPUT ) );
	assert( m_pEdit );
	m_iNum = 0;
	m_iMaxNum = 0;
	m_curSlotPos = 0;
	m_pEdit->SetFontID( StrToWzID("mn12") );
}

//------------------------------------------------------------------------------
/**
*/
void uiVendorBuyPopup::Release()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiVendorBuyPopup::OnShowWindow(BOOL val )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	if ( val )
	{
		GetUIMan()->AttachFirstDialog(InterfaceManager::DIALOG_VENDOR_BUY_POPUP);
		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg) );
		msg.dwType = eDoType_dialog_vendor_buy_popup;
		msg.wParam = InterfaceManager::DIALOG_VENDOR_BUY_POPUP;
		msg.DoSomething = GlobalFunc::DoVendorBuyPopup;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_VENDOR_BUY_POPUP );
		m_pEdit->ClearText();
		m_iNum = 0;

		g_KeyQueueManager.DeleteMsg( eDoType_dialog_vendor_buy_popup );
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiVendorBuyPopup::Process(DWORD dwTick)
{

}

//------------------------------------------------------------------------------
/**
*/
void uiVendorBuyPopup::MessageProc(SI_MESSAGE * pMessage )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_BTN_OK:
				{
					TCHAR szInputMessage[128] = {0,};
					int	  inputNum = 0;

					// 금액의 최대 사이즈(128 byte)를 넘었습니다.
					if(!GlobalFunc::CompareLimitText(m_pEdit, szInputMessage, 5, 5794))
						return;

					szInputMessage[5] = NULL;
					if( IsDigit( szInputMessage ) )
					{
						inputNum = (int ) _ttoi( szInputMessage );
						if( inputNum > 0 )
						{
							if( m_iMaxNum < inputNum )
							{
								m_iNum = m_iMaxNum;
							}
							else
							{
								m_iNum = inputNum;
							}
						
							uiVendorBuy * pVendorBuy = ( uiVendorBuy *) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_VENDOR_BUY);
							if( pVendorBuy)
							{
							
								SCSlot & rSlot = pVendorBuy->GetSlot(m_curSlotPos);
								uiSCItemSlot & rItemSlot = static_cast<uiSCItemSlot &>(rSlot); 
								 if ( g_pHero->GetMoney() >= rItemSlot.GetMoneyPerOne() * m_iNum )
								 {
									 pVendorBuy->SendItemBuySync(m_curSlotPos , m_iNum );
								 }								
								 else
								 {
									 //	현재 보유한 하임이 부족하여 아이템을 구매할 수 없습니다.
									 const int	c_iNotEnoughMoney = 70408;
									 g_InterfaceManager.GetInterfaceString( c_iNotEnoughMoney, szMessage, INTERFACE_STRING_LENGTH );
									 GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
								 }
								ShowInterface(FALSE);
							}
						}
					}
					else
					{
						// 숫자를 입력하세요.
						g_InterfaceManager.GetInterfaceString( 70363 , szMessage , INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT , szMessage );
						m_pEdit->ClearText();
					}
					
				}
				break;

			case DLGPOS_BTN_MAX:
				{
					m_iNum = m_iMaxNum;
					sprintf( szMessage , "%d" , m_iNum );
					GlobalFunc::SetText( m_pEdit , szMessage );
				}
				break;

			case DLGPOS_BTN_LEFT:
				{
					if( m_iNum > 0 )
					{
						m_iNum -= 1;
					}
					sprintf( szMessage , "%d" , m_iNum );
					GlobalFunc::SetText( m_pEdit , szMessage );
				}
				break;

			case DLGPOS_BTN_RIGHT:
				{
					if( m_iMaxNum > m_iNum)
					{
						m_iNum += 1;
					}
					sprintf( szMessage , "%d" , m_iNum );
					GlobalFunc::SetText( m_pEdit , szMessage );
				}
				break;

			case DLGPOS_BTN_EXIT:
			case DLGPOS_BTN_CANCLE:
				{
					ShowInterface(FALSE);
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
void uiVendorBuyPopup::NetworkProc(MSG_BASE * pMsg )
{

}


BOOL uiVendorBuyPopup::IsDigit(const TCHAR * pszMessage)
{
	int len = lstrlen(pszMessage);
	int i;

	for (i=0; i<len; ++i)
	{
		if (pszMessage[i]<_T('0') || pszMessage[i]>_T('9'))
		{
			return FALSE;
		}
	}

	return TRUE;
}

#endif