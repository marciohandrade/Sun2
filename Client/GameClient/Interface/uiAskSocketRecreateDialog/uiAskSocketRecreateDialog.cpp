#include "SunClientPrecompiledHeader.h"

#include "uiAskSocketRecreateDialog/uiAskSocketRecreateDialog.h"
#include "Interfacemanager.h"
#include "iteminfoparser.h"
#include "globalfunc.h"
#include "GameConst_Color.h"
#include "ItemManager.h"
#include <SCItemSlot.h>
#include <SCSlotContainer.h>

AskSocketRecreateDialog::AskSocketRecreateDialog(InterfaceManager* ui_manager_ptr)
:AskDialog(ui_manager_ptr)
{
	Initialize();
    is_socket_recreate_ = false;
}

AskSocketRecreateDialog::~AskSocketRecreateDialog(void)
{
}

void AskSocketRecreateDialog::Process(DWORD tick_count)
{
}

void AskSocketRecreateDialog::Initialize()
{
	this->item_code_ = 0;
	this->recreate_item_positeion_ = INVALID_POSTYPE_VALUE;
	packet_status_ = NONE;
}

void AskSocketRecreateDialog::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
		if (!BIT_CHECK(this->packet_status_, RECREATE_SOCKET))
		{
			this->DoIt();
		}
			
		SetStatus(ASK_STATUS_NORMAL);
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
		this->Cancel();
		SetStatus(ASK_STATUS_NORMAL);
    }
}


void AskSocketRecreateDialog::MessageProc(SI_MESSAGE* message)
{
	switch(message->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch(getCtrlIDToPos(message->dwCtrlID))
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
			SolarDialog::MessageProc(message);
		}
		break;
    }
}

void AskSocketRecreateDialog::OnShowWindow(BOOL show_value)
{
    if (show_value)
    {
        SolarDialog::MoveCenter();

		assert(m_pListCtrl);
		SetTexts();
	}
	else
	{
		packet_status_ = NONE;
	}
}

void AskSocketRecreateDialog::SetTexts()
{
	TCHAR	szItemName[INTERFACE_STRING_LENGTH], 
			szText[INTERFACE_STRING_LENGTH], 
			szMessage[INTERFACE_STRING_LENGTH];
	assert(m_pListCtrl);

	DWORD	dwTextColor	= RGBA( 235, 192, 38, 255 );
	DWORD	dwWideTextColor	= RGBA( 245, 166, 46, 255 );
	DWORD	dwSelectColor = RGBA( 0, 0, 255,255 );
	DWORD	dwBGColor = RGBA( 32, 32, 32, 255 );
	DWORD	dwSetColorSelBarOnText = RGBA( 255, 255, 255, 255 );
    DWORD string_code = 0;
    SLOTCODE exchanged_item = 0;

    m_pListCtrl->DeleteItemAll();
    m_pListCtrl->SetBeginDrawIndex(0);
	m_pListCtrl->SetUnuseSelBar(TRUE);
    m_pListCtrl->OnFocusOut();

	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(item_code_);

    SCSlotContainer* inventory_container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if(inventory_container)
    {
        SCItemSlot& item_slot = (SCItemSlot&)inventory_container->GetSlot(recreate_item_positeion_);
        const BASE_ITEMINFO* recreate_Item_Info = item_slot.GetItemInfo();
        
        if (recreate_Item_Info && recreate_Item_Info->m_ExchangedItem)
        {
            exchanged_item = recreate_Item_Info->m_ExchangedItem;
        }

        if (exchanged_item > 0)
        {
            //48310 : %s 장비에 %d개의 소켓을 생성시킵니다. 계속하시겠습니까?
            string_code = 48310;
        }
        else
        {   //48282 : %s 장비에 소켓 생성 확률을 1번 더 부여하시겠습니까?
            string_code = 48282;
        }
    }

	if ( pItemInfo )
	{
        g_InterfaceManager.GetInterfaceString(string_code, szText, INTERFACE_STRING_LENGTH);
        g_InterfaceManager.GetItemName(pItemInfo->m_Code, szMessage, INTERFACE_STRING_LENGTH);
        Sprintf(szItemName, szText, szMessage, exchanged_item);

		std::vector<util::_tstring> vstrMsg;
		vstrMsg.clear();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int iListWidth = m_pListCtrl->GetSize().right - 15;
#else
		int iListWidth = m_pListCtrl->GetSize().right - 5;
#endif	
		
		GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szItemName, StrToWzID("mn12"), _tcslen(szItemName) ,iListWidth );
		for( unsigned int i =0; i < vstrMsg.size(); ++i )
		{
			int index = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl, index , 0, (TCHAR *)vstrMsg[i].c_str() ,StrToWzID("mn12") ,
							DT_VCENTER | DT_LEFT, c_InterfaceBKColor,dwTextColor , dwTextColor);
		}	
	}
}

VOID AskSocketRecreateDialog::DoIt()
{
	BIT_ADD(this->packet_status_, RECREATE_SOCKET);

	MSG_CG_ITEM_RECREATE_SOCKET_SYN ItemSocketRecreateSYN;
    
    ItemSocketRecreateSYN.Init();
    ItemSocketRecreateSYN.request_item_pos = recreate_item_positeion_;
    ItemSocketRecreateSYN.target_item_pos = target_position_;

    is_socket_recreate_ = true;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ItemSocketRecreateSYN, ItemSocketRecreateSYN.GetSize());	
	ShowInterface(FALSE);
}

void AskSocketRecreateDialog::Cancel()
{
	BIT_REMOVE(packet_status_, NONE);
	// cancel
	ShowInterface(FALSE);
	Initialize();
}

void AskSocketRecreateDialog::SetInfo(CODETYPE item_code, POSTYPE recreate_item_positeion, POSTYPE target_position)
{
	this->item_code_	= item_code;
	this->recreate_item_positeion_	= recreate_item_positeion;
	this->target_position_	= target_position;
}
