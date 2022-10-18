#include "SunClientPrecompiledHeader.h"

#include "uiAskSocketIdentifyDialog/uiAskSocketIdentifyDialog.h"
#include "Interfacemanager.h"
#include "iteminfoparser.h"
#include "globalfunc.h"
#include "GameConst_Color.h"

AskSocketIdentifyDialog::AskSocketIdentifyDialog(InterfaceManager* ui_manager_ptr)
:AskDialog(ui_manager_ptr)
{
	Initialize();
    is_socket_identify_ = false;
}

AskSocketIdentifyDialog::~AskSocketIdentifyDialog(void)
{
}

void AskSocketIdentifyDialog::Process(DWORD tick_count)
{
}

void AskSocketIdentifyDialog::Initialize()
{
	this->item_code_ = 0;
	this->Identify_item_positeion_ = INVALID_POSTYPE_VALUE;
	packet_status_ = NONE;
}

void AskSocketIdentifyDialog::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
		if (!BIT_CHECK(this->packet_status_, IDENTIFY_SOCKET))
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


void AskSocketIdentifyDialog::MessageProc(SI_MESSAGE* message)
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

void AskSocketIdentifyDialog::OnShowWindow(BOOL show_value)
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

void AskSocketIdentifyDialog::SetTexts()
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

    m_pListCtrl->DeleteItemAll();
    m_pListCtrl->SetBeginDrawIndex(0);
	m_pListCtrl->SetUnuseSelBar(TRUE);
    m_pListCtrl->OnFocusOut();

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(item_code_);

	if ( pItemInfo )
	{
		//6484 : %s 아이템의 소켓 등급을 확인하시겠습니까?
        g_InterfaceManager.GetInterfaceString(6484, szText, INTERFACE_STRING_LENGTH);
        g_InterfaceManager.GetItemName(pItemInfo->m_Code, szMessage, INTERFACE_STRING_LENGTH);
        Sprintf(szItemName, szText, szMessage);

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

VOID AskSocketIdentifyDialog::DoIt()
{
	BIT_ADD(this->packet_status_, IDENTIFY_SOCKET);

	MSG_CG_ITEM_IDENTIFY_SOCKET_CMD ItemIdentifySocketCMD;
    
    ItemIdentifySocketCMD.unidentified_item_pos_ = target_position_;
    ItemIdentifySocketCMD.identify_item_pos_ = Identify_item_positeion_;
    if (Identify_item_positeion_ == INVALID_POSTYPE_VALUE)
    {
	    ItemIdentifySocketCMD.use_identify_item_ = false;
    }
    else
    {
        ItemIdentifySocketCMD.use_identify_item_ = true;
    }
    is_socket_identify_ = true;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ItemIdentifySocketCMD, sizeof(ItemIdentifySocketCMD));	
	ShowInterface(FALSE);
}

void AskSocketIdentifyDialog::Cancel()
{
	BIT_REMOVE(packet_status_, IDENTIFY_SOCKET);
	// cancel
	ShowInterface(FALSE);
	Initialize();
}

void AskSocketIdentifyDialog::SetInfo(CODETYPE item_code, POSTYPE Identify_item_positeion, POSTYPE target_position)
{
	this->item_code_	= item_code;
	this->Identify_item_positeion_	= Identify_item_positeion;
	this->target_position_	= target_position;
}
