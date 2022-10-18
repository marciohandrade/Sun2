#include "SunClientPrecompiledHeader.h"

#include "uiAskCharAddSlotDialog/uiAskCharAddSlotDialog.h"
#include "Interfacemanager.h"
#include "iteminfoparser.h"
#include "globalfunc.h"
#include "GameConst_Color.h"
#include "ItemManager.h"

AskCharAddSlotDialog::AskCharAddSlotDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{

}

AskCharAddSlotDialog::~AskCharAddSlotDialog()
{

}

void AskCharAddSlotDialog::OnUpdateSolarDialog()
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
void AskCharAddSlotDialog::MessageProc( SI_MESSAGE * pMessage )
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

void AskCharAddSlotDialog::OnShowWindow( BOOL val )
{
    if( val )
    {
        SetTexts();
    }
}

void AskCharAddSlotDialog::SetTexts()
{
    TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
    // 5991 : 캐릭터 슬롯 1칸을 추가 하시겠습니까?
    g_InterfaceManager.GetInterfaceString(5991, message);
    InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);
}

void AskCharAddSlotDialog::DoIt()
{
    MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN packet;
    
    ItemManager::Instance()->SendItemUseMsg(SI_INVENTORY, item_position_);
    ShowInterface(FALSE);
}
void AskCharAddSlotDialog::Cancel()
{
    ShowInterface(FALSE);
}
