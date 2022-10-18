#include "SunClientPrecompiledHeader.h"

#include "AskExtraStoneUse.h"
#include "Interfacemanager.h"
#include "iteminfoparser.h"
#include "MouseHandler.h"
#include "globalfunc.h"
#include "GameConst_Color.h"
AskExtraStoneUse::AskExtraStoneUse(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
	Initialize();
}

AskExtraStoneUse::~AskExtraStoneUse(void)
{
}

VOID AskExtraStoneUse::Process(DWORD dwTick)
{
}


VOID AskExtraStoneUse::Initialize()
{
	this->m_ItemCode = 0;
	this->m_CrystalPos = 0;
	m_PacketStatus = NONE;
}

void AskExtraStoneUse::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
		if (!BIT_CHECK(this->m_PacketStatus, USE_EXTRASTONE))
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


void AskExtraStoneUse::MessageProc( SI_MESSAGE * pMessage )
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


VOID AskExtraStoneUse::OnShowWindow( BOOL val )
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


VOID AskExtraStoneUse::SetTexts()
{
	TCHAR	szItemName[INTERFACE_STRING_LENGTH], 
			szText[INTERFACE_STRING_LENGTH], 
			szMessage[INTERFACE_STRING_LENGTH];

	assert(m_pListCtrl);


	DWORD	dwTextColor	= RGBA( 235, 192, 38, 255 );
	DWORD	dwBGColor = RGBA( 32, 32, 32, 255 );

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_ItemCode);

	if ( pItemInfo )
	{	
		g_InterfaceManager.GetItemName( pItemInfo->m_Code, szItemName, INTERFACE_STRING_LENGTH );
		// %s 에 엑스트라 스톤을 사용합니까? : 5305
		const int c_iIsApplyExtraStone = 5305;        
		g_InterfaceManager.GetInterfaceString(c_iIsApplyExtraStone, szText, INTERFACE_STRING_LENGTH);
		Sprintf(szMessage, szText, szItemName);
		
        InsertMessage(szMessage,dwBGColor,dwTextColor);

	}
}

VOID AskExtraStoneUse::DoIt()
{
	if ( !BIT_CHECK(this->m_PacketStatus, USE_EXTRASTONE) )
	{
		BIT_ADD(this->m_PacketStatus, USE_EXTRASTONE);

		MSG_CG_ITEM_EXTRASTONE_USE_SYN ItemUseExtraStoneSyn;
		ItemUseExtraStoneSyn.m_ExtraStonePos = this->m_CrystalPos;
		ItemUseExtraStoneSyn.m_TargetPos	 = this->m_TargetPos;
		MouseHandler::Instance()->SetPendingTargetPos( this->m_TargetPos );

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ItemUseExtraStoneSyn, sizeof(ItemUseExtraStoneSyn));	
	}

	ShowInterface(FALSE);
}


VOID AskExtraStoneUse::Cancel()
{
	BIT_REMOVE(m_PacketStatus, USE_EXTRASTONE);
	// cancel
	ShowInterface(FALSE);
	Initialize();
}

VOID AskExtraStoneUse::SetInfo(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
	this->m_ItemCode	= ItemCode;
	this->m_CrystalPos	= CrystalPos;
	this->m_TargetPos	= TargetPos;
}