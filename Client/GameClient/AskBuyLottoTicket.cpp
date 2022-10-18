#include "SunClientPrecompiledHeader.h"
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#include "AskBuyLottoTicket.h"
#include "InterfaceManager.h"
#include "GameConst_Color.h"
#include "uiLottoMan/uiLottoMan.h"
#include "uiLottoNumInput/uiLottoNumInput.h"

AskBuyLottoTicket::AskBuyLottoTicket(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
	Initialize();
}

AskBuyLottoTicket::~AskBuyLottoTicket()
{

}


VOID AskBuyLottoTicket::Process(DWORD dwTick)
{

}

VOID AskBuyLottoTicket::Initialize()
{

}

void AskBuyLottoTicket::OnUpdateSolarDialog()
{
	if (this->GetStatus() == ASK_STATUS_OK)
	{
		this->DoIt();
		SetStatus(ASK_STATUS_NORMAL);
	}
	else if (this->GetStatus() == ASK_STATUS_CANCEL)
	{
		this->Cancel();
		SetStatus(ASK_STATUS_NORMAL);
	}
}
void AskBuyLottoTicket::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case ASK_OK:
				{
					this->m_status = ASK_STATUS_OK;
				}
				break;

			case ASK_CANCEL:
				{
					this->m_status = ASK_STATUS_CANCEL;
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

VOID AskBuyLottoTicket::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();
		SetTexts();
	}
}

VOID AskBuyLottoTicket::SetTexts()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	
	//5284: 복권사용시 XXXXXX 하임이 소비됩니다.
	g_InterfaceManager.GetInterfaceString(5284,szMessage);
    InsertMessage(szMessage,c_InterfaceBKColor,c_InterfaceTextColor);

}

VOID AskBuyLottoTicket::DoIt()
{
    uiLottoNumInput* pLottoNumInput= GET_CAST_DIALOG( uiLottoNumInput, IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG );

    if(pLottoNumInput)
    {
        pLottoNumInput->SEND_LOTTO_ENTRY_SYN(bAutoNumber);
		ShowInterface(FALSE);
    }
}
VOID AskBuyLottoTicket::Cancel()
{
	ShowInterface(FALSE);
	Initialize();
}
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE