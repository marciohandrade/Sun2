#include "SunClientPrecompiledHeader.h"

#include "AskRiderWaxGloss.h"
#include "GameConst_Color.h"
#include "InterfaceManager.h"

AskRiderWaxGloss::AskRiderWaxGloss(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
	Initialize();
}

AskRiderWaxGloss::~AskRiderWaxGloss()
{

}

VOID AskRiderWaxGloss::SetRiderInfo(CODETYPE ItemCode, POSTYPE RiderPartsPos, POSTYPE WaxItemPos)
{
	m_RiderPartsPos = RiderPartsPos;
	m_WaxItemPos = WaxItemPos;
	m_ItemCode =ItemCode;
}

VOID AskRiderWaxGloss::Process(DWORD dwTick)
{

}

VOID AskRiderWaxGloss::Initialize()
{
	m_RiderPartsPos = 0;
	m_WaxItemPos = 0;
	m_ItemCode =0;
}

void AskRiderWaxGloss::OnUpdateSolarDialog()
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
void AskRiderWaxGloss::MessageProc( SI_MESSAGE * pMessage )
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

VOID AskRiderWaxGloss::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();
		SetTexts();
	}
}

VOID AskRiderWaxGloss::SetTexts()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szItemName[INTERFACE_STRING_LENGTH] ={0,};
	g_InterfaceManager.GetItemName(m_ItemCode, szItemName);
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	
    //6446 %s 아이템을 적용하시겠습니까?
	g_InterfaceManager.GetInterfaceString(6446,szTemp);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , szItemName);

    InsertMessage(szMessage,c_InterfaceBKColor,c_InterfaceTextColor);
}

VOID AskRiderWaxGloss::DoIt()
{
    //광택바르기요청패킷
    MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN	 SendMsg;
    SendMsg.header.request = SendMsg.header.eReq_WaxGloss;
    SendMsg.header.args.wax_gloss.from_wax = m_WaxItemPos;
    SendMsg.header.args.wax_gloss.to_parts = m_RiderPartsPos;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	ShowInterface(FALSE);
}
VOID AskRiderWaxGloss::Cancel()
{
	ShowInterface(FALSE);
	Initialize();
}
