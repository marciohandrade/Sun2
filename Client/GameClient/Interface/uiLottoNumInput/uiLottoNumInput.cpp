#include "SunClientPrecompiledHeader.h"
#include "uiLottoMan/uiLottoMan.h"
#include "uiLottoNumInput.h"
#include "interfacemanager.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "ItemManager.h"
#include "Hero.h"
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#include "AskBuyLottoTicket.h"
#include "uiAssistMan/uiAssistMan.h"
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

WzID uiLottoNumInput::m_wzId[LOTTO_MAX] = 
{
	StrToWzID("BT03"),	//LOTTO_BTN_INPUT
	StrToWzID("B002"),	//LOTTO_BTN_AUTO	
	StrToWzID("B001"),	//LOTTO_BTN_EXIT	
	
	StrToWzID("BT04"),	//LOTTO_BTN_UP1	
	StrToWzID("BT05"),	//LOTTO_BTN_UP2	
	StrToWzID("BT06"),  //LOTTO_BTN_UP3	
	StrToWzID("BT07"),	//LOTTO_BTN_UP4	
	
	StrToWzID("BT08"),  //LOTTO_BTN_DOWN1
	StrToWzID("BT09"),	//LOTTO_BTN_DOWN2
	StrToWzID("BT10"),	//LOTTO_BTN_DOWN3
	StrToWzID("BT11"),  //LOTTO_BTN_DOWN4
	
	StrToWzID("ST01"),	//LOTTO_TEXT1	
	StrToWzID("ST02"),  //LOTTO_TEXT2	
	StrToWzID("ST03"),	//LOTTO_TEXT3	
	StrToWzID("ST04"),	//LOTTO_TEXT4	
};

uiLottoNumInput::uiLottoNumInput(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiLottoNumInput::~uiLottoNumInput(void)
{
	Release();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiLottoNumInput::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	InitControls();
}
//------------------------------------------------------------------------------
/** 
*/
void uiLottoNumInput::InitControls()
{
	int i=0;

	for(;i<LOTTO_BTN_MAX;++i)
	{
		m_pLottoBtn[i] = static_cast<CCtrlButtonWZ *>(getControl(i));
		assert(m_pLottoBtn[i]);
	}

	for(i=0;i<LOTTO_TEXT_MAX;++i)
	{
		m_pTxtCtrl[i]	= static_cast<CCtrlStaticWZ *>(getControl(LOTTO_TEXT1+i));
		assert(m_pTxtCtrl[i]);
	}
		
}
//------------------------------------------------------------------------------
/** 
*/
void uiLottoNumInput::OnShowWindow( BOOL val )
{
	
	if(val)
	{
		//uiBase::MoveCenter();
		
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Dialog_Lotto_NumInput_Dlg;
		msg.wParam = IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);

		initlottonumber();
		refreshLottoNumber();

	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Lotto_NumInput_Dlg);
	}
}
//-----------------------------------------------------------------------------
/**	 C-->S 로또응모패킷을 보낸다.
*/
void uiLottoNumInput::SEND_LOTTO_ENTRY_SYN(BOOL bAuto)
{
	if(!g_pHero) return;
	
	//나의 로또응모정보를 요청한다
	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

	if(!pLottoMan) return;
	
	//패킷채운다
	MSG_CW_LOTTO_ENTRY_SYN SendPacket;
	
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
#else
    if (CheckInputNumber(bAuto) == false)
    {
        return;
    }
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET

	//나의 로또응모정보를 요청한다
	SendPacket.bAuto			= bAuto;
	SendPacket.bPickedNum[0]	= m_arLottoNum[0];
	SendPacket.bPickedNum[1]	= m_arLottoNum[1];
	SendPacket.bPickedNum[2]	= m_arLottoNum[2];
	SendPacket.bPickedNum[3]	= m_arLottoNum[3];
	SendPacket.wDrawNum			= pLottoMan->GetLottoInfo()->m_wLottoDrawNum;

	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendPacket, sizeof(SendPacket) );

	this->ShowInterface(FALSE);	
}
//-----------------------------------------------------------------------------
/**	
*/
VOID uiLottoNumInput::MessageProc( SI_MESSAGE * pMessage )
{
	if(pMessage->eResultMsg!=RT_MSG_LBUTTONCLICK) return;	
	
	POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
	
	switch( pos )
	{
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    //중국 현지화작업
    case LOTTO_BTN_INPUT:
    case LOTTO_BTN_AUTO:
        {
            uiAssistMan * pAssistMan = (uiAssistMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST);
            if( pAssistMan )
            {
				this->ShowInterface(FALSE);	
                pAssistMan->ShowAskBuyLottoTicket(pos==LOTTO_BTN_INPUT ? (FALSE):(TRUE));
            }
        }
        break;
#else
    case LOTTO_BTN_INPUT:
        {
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
            uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
            if(pLottoMan && pLottoMan->GetLottoInfo()->m_bNumTickets>0)
            {
                SEND_LOTTO_ENTRY_SYN(FALSE);
            }
            else
            {
                if (CheckInputNumber(FALSE) == true)
                {
                    FindAndUseLotto(FALSE);
                }
            }
#else
            SEND_LOTTO_ENTRY_SYN(FALSE);
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
        }
        break;
    case LOTTO_BTN_AUTO:
        {
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
            uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
            if(pLottoMan && pLottoMan->GetLottoInfo()->m_bNumTickets>0)
            {
                SEND_LOTTO_ENTRY_SYN(TRUE);
            }
            else
            {
                FindAndUseLotto(TRUE);
            }
#else
            SEND_LOTTO_ENTRY_SYN(TRUE);
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
        }
        break;
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
	case LOTTO_BTN_EXIT:
		{
			this->ShowInterface(FALSE);		
		}
		break;
	case LOTTO_BTN_DOWN1:
	case LOTTO_BTN_DOWN2:
	case LOTTO_BTN_DOWN3:
	case LOTTO_BTN_DOWN4:
		{
			int nIndex = pos-LOTTO_BTN_DOWN1;
			int nTemp  = m_arLottoNum[nIndex];

			//1보다작으면 25로 셋팅 돌고돈다
			 m_arLottoNum[nIndex] =  (--nTemp<LOTTO_MIN_NUMBER) ? (LOTTO_MAX_NUMBER):(--m_arLottoNum[nIndex]);
			refreshLottoNumber();
		}
		break;
	case LOTTO_BTN_UP1:
	case LOTTO_BTN_UP2:
	case LOTTO_BTN_UP3:
	case LOTTO_BTN_UP4:
		{
			int nIndex = pos-LOTTO_BTN_UP1;
			int nTemp  = m_arLottoNum[nIndex];

			//25보다 크면 1으로 셋팅 돌고돈다
			 m_arLottoNum[nIndex] = (++nTemp>LOTTO_MAX_NUMBER) ? (LOTTO_MIN_NUMBER):(++m_arLottoNum[nIndex]);
			refreshLottoNumber();
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiLottoNumInput::initlottonumber()
{
	for(int i=0;i<LOTTO_TEXT_MAX;++i)
	{
		m_arLottoNum[i]  = 1;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiLottoNumInput::refreshLottoNumber()
{
	TCHAR szTemp[20] = {0,};

	for(int i=0;i<LOTTO_TEXT_MAX;++i)
	{
		ZeroMemory(szTemp, 20);
		
		if(m_pTxtCtrl[i])
		{
			m_pTxtCtrl[i]->SetTextWZ(_itoa(m_arLottoNum[i], szTemp, 10));
		}
	}
}	
//------------------------------------------------------------------------------
/**
*/
void uiLottoNumInput::OnUpdateSolarDialog()
{
	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

	if(pLottoMan && pLottoMan->CheckDistanceNPC())
	{
		this->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
VOID uiLottoNumInput::Release()
{
	//no operation
}
//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
void uiLottoNumInput::FindAndUseLotto(BOOL auto_select)
{
    //복권아이템(7293)이 있으면 사용
    POSTYPE use_item_position = 0;
    if (ItemManager::Instance()->FindFirstItem(7293, use_item_position) == TRUE)
    {
        auto_select_ = auto_select;
        ItemManager::Instance()->SendItemUseMsg(SI_INVENTORY, use_item_position);
    }
    else
    {
        uiLottoMan* lotto_manager_ptr= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
        //5289	로또복권이 없습니다.
        TCHAR message[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(5289,message);
        lotto_manager_ptr->LottoMessage(message);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiLottoNumInput::SendLottoEntrySyn()
{
    SEND_LOTTO_ENTRY_SYN(auto_select_);
}
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
//------------------------------------------------------------------------------
/**
*/
bool uiLottoNumInput::CheckInputNumber(BOOL bAuto)
{
    if(!bAuto)
    {
        //번호중복인지체크
        for(int i=0;i<LOTTO_TEXT_MAX;++i)
        {
            for(int j=0;j<LOTTO_TEXT_MAX;++j)
            {
                if(i==j) continue;

                if(m_arLottoNum[i] != m_arLottoNum[j]) continue;

                uiLottoMan* pLottoMan = GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

                if(pLottoMan)
                {
                    //5290	로또번호중복입니다.
                    TCHAR	szMessage[INTERFACE_STRING_LENGTH];
                    g_InterfaceManager.GetInterfaceString(5290,szMessage);
                    pLottoMan->LottoMessage(szMessage);		
                    return false;
                }
            }
        }
    }
    return true;
}
