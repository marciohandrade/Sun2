#include "SunClientPrecompiledHeader.h"

#include "AskEtherDevieDialog.h"
#include "InterfaceManager.h"
#include "GameConst_Color.h"
AskEtherDeviceDialog::AskEtherDeviceDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
	Initialize();
}

AskEtherDeviceDialog::~AskEtherDeviceDialog()
{

}

VOID AskEtherDeviceDialog::SetInfo(CODETYPE ItemCode, POSTYPE WeaponPos, POSTYPE DeviceItemPos)
{
	m_WeaponPos = WeaponPos;
	m_DeviceItemPos = DeviceItemPos;
	m_ItemCode =ItemCode;
}

VOID AskEtherDeviceDialog::Process(DWORD dwTick)
{

}

VOID AskEtherDeviceDialog::Initialize()
{
	m_WeaponPos = 0;
	m_DeviceItemPos = 0;
	m_ItemCode =0;
}

void AskEtherDeviceDialog::OnUpdateSolarDialog()
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
void AskEtherDeviceDialog::MessageProc( SI_MESSAGE * pMessage )
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

VOID AskEtherDeviceDialog::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();
		SetTexts();
	}
}

VOID AskEtherDeviceDialog::SetTexts()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szItemName[INTERFACE_STRING_LENGTH] ={0,};
	g_InterfaceManager.GetItemName(m_ItemCode, szItemName);
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	//6126	 %s 아이템에 에테르 방출 장치를 장착하시겠습니까?
	g_InterfaceManager.GetInterfaceString(6126,szTemp);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , szItemName);
    
    InsertMessage(szMessage,c_InterfaceBKColor,c_InterfaceTextColor);

}

VOID AskEtherDeviceDialog::DoIt()
{
	MSG_CG_ITEM_EQUIP_ETHER_DEVICE_SYN SendMsg;
	SendMsg.m_DeviceItemPos = m_DeviceItemPos;
	SendMsg.m_WeaponItemPos = m_WeaponPos;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	
	ShowInterface(FALSE);
}
VOID AskEtherDeviceDialog::Cancel()
{
	ShowInterface(FALSE);
	Initialize();
}
