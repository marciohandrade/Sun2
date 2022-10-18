#include "SunClientPrecompiledHeader.h"

#include "asksocketfilldialog.h"
#include "Interfacemanager.h"
#include "iteminfoparser.h"
#include "globalfunc.h"
#include "GameConst_Color.h"
#include "ItemManager.h"
#include "SCSlotContainer.h"
#include "SCItemSlot.h"

AskSocketFillDialog::AskSocketFillDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
	Initialize();
}

AskSocketFillDialog::~AskSocketFillDialog(void)
{
}

VOID AskSocketFillDialog::Process(DWORD dwTick)
{

}


VOID AskSocketFillDialog::Initialize()
{
	this->m_ItemCode = 0;
	this->m_CrystalPos = 0;
	m_PacketStatus = NONE;
}

void AskSocketFillDialog::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
		if (!BIT_CHECK(this->m_PacketStatus, FILL_SOCKET))
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


void AskSocketFillDialog::MessageProc( SI_MESSAGE * pMessage )
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


VOID AskSocketFillDialog::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

		assert(m_pListCtrl);

		SetTexts();
	}
	else
	{
		m_PacketStatus = NONE;
	}
}


VOID AskSocketFillDialog::SetTexts()
{
	TCHAR szItemName[INTERFACE_STRING_LENGTH];
	TCHAR szText[INTERFACE_STRING_LENGTH];
    TCHAR szMessage[INTERFACE_STRING_LENGTH];

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

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_ItemCode);

	if ( pItemInfo )
	{
        DWORD string_code = 0;
        SCSlotContainer* pAtContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
        SCItemSlot& rItemSlot = (SCItemSlot & )pAtContainer->GetSlot(m_CrystalPos);
        const BASE_ITEMINFO* zard_item = rItemSlot.GetItemInfo();
        if (zard_item != NULL)
        {
            switch (zard_item->zard_grade)
            {
            case kZardGrade1:
                string_code = 65016;//[%s]에 하급 쟈드를 사용하시겠습니까?
                break;
            case kZardGrade2:
                string_code = 65015;// 중급 소켓 옵션은 1개의 장비에 3개까지 부여할 수 있습니다. [%s]에 중급 쟈드를 사용하시겠습니까?
                break;
            case kZardGrade3:
                string_code = 65014;// 상급 소켓 옵션은 1개의 장비에 3개까지 부여할 수 있습니다.  [%s]에 상급 쟈드를 사용하시겠습니까?
                break;
            case kZardGrade4:
                string_code = 65013;// 최상급 소켓 옵션은 1개의 장비에 3개까지 부여할 수 있습니다. [%s]에 최상급 쟈드를 사용하시겠습니까?
                break;
            default:
                string_code = eST_INSERT_SOCKET_ITEM_ASK; // %s 에 소켓 아이템을 삽입 합니까?
                break;
            }

            g_InterfaceManager.GetInterfaceString(string_code, szText, INTERFACE_STRING_LENGTH);
            g_InterfaceManager.GetItemName(m_ItemCode, szMessage, INTERFACE_STRING_LENGTH);
            Sprintf(szItemName, szText, szMessage);
        }        

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

VOID AskSocketFillDialog::DoIt()
{
	BIT_ADD(this->m_PacketStatus, FILL_SOCKET);

	MSG_CG_ITEM_SOCKET_FILL_SYN ItemSocketFillSyn;
	ItemSocketFillSyn.m_SocketItemPos	= this->m_CrystalPos;
	ItemSocketFillSyn.m_TargetPos		= this->m_TargetPos;

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ItemSocketFillSyn, sizeof(ItemSocketFillSyn));	

	ShowInterface(FALSE);
}


VOID AskSocketFillDialog::Cancel()
{
	BIT_REMOVE(m_PacketStatus, FILL_SOCKET);
	// cancel
	ShowInterface(FALSE);
	Initialize();
}

VOID AskSocketFillDialog::SetInfo(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
	this->m_ItemCode	= ItemCode;
	this->m_CrystalPos	= CrystalPos;
	this->m_TargetPos	= TargetPos;
}