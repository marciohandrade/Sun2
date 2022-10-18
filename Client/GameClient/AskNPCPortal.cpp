//---------------------------------------------------------------------------
//
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------


#include "SunClientPrecompiledHeader.h"
#include "asknpcportal.h"
#include "NPCPortalDialog.h"
#include "MapInfoParser.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//----------------------------------------------------------------------------
/**
*/
AskNPCPortal::AskNPCPortal(InterfaceManager *pUIMan)
:AskDialog(pUIMan), m_MapCode(0)
{
	m_packetStatus = 0;
}

//----------------------------------------------------------------------------
/**
*/
AskNPCPortal::~AskNPCPortal(void)
{

}

//----------------------------------------------------------------------------
/**
*/
VOID			
AskNPCPortal::Initialize()
{
	m_MapCode = 0;
}

//----------------------------------------------------------------------------
/**
*/
VOID			
AskNPCPortal::SetMoveVillage( MAPCODE MapCode, MONEY fare )
{
	m_MapCode = MapCode;
	m_Fare = fare;
}

//----------------------------------------------------------------------------
/**
*/
void	
AskNPCPortal::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
		if (!BIT_CHECK(this->m_packetStatus, MOVE_PORTAL))
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

//----------------------------------------------------------------------------
/**
*/
void    
AskNPCPortal::MessageProc( SI_MESSAGE * pMessage )
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

//----------------------------------------------------------------------------
/**
*/
VOID	
AskNPCPortal::OnShowWindow( BOOL val )
{
    if (val)
    {
		SolarDialog::MoveCenter();

		assert(m_pListCtrl);

		SetTexts();
	}
	else
	{
		m_packetStatus = NONE;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
AskNPCPortal::SetTexts()
{
	TCHAR	szMapName[INTERFACE_STRING_LENGTH], 
			szMessage[INTERFACE_STRING_LENGTH],
			szResult[INTERFACE_STRING_LENGTH];

	assert(m_pListCtrl);

	DWORD	dwTextColor	= RGBA( 240, 201, 121, 255 );
	DWORD	dwWideTextColor	= RGBA( 245, 166, 46, 255 );
	DWORD	dwSelectColor = RGBA( 0, 0, 255,255 );
	DWORD	dwBGColor = RGBA( 32, 32, 32, 255 );
	DWORD	dwSetColorSelBarOnText = RGBA( 255, 255, 255, 255 );

    m_pListCtrl->DeleteItemAll();
    m_pListCtrl->SetBeginDrawIndex(0);
	m_pListCtrl->SetUnuseSelBar(TRUE);
    m_pListCtrl->OnFocusOut();

	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( this->m_MapCode );

	if ( pMapInfo )
	{
		g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH);
		g_InterfaceManager.GetInterfaceString(51201, szMessage, INTERFACE_STRING_LENGTH);
		Sprintf(szResult, szMessage, szMapName);
		int index = m_pListCtrl->InsertItemBack();
		GlobalFunc::SetItemText( m_pListCtrl, index, 0, szResult );
		m_pListCtrl->SetItemFontID( index, 0, StrToWzID("mn12") );
		m_pListCtrl->SetItemTextColor( index, 0, dwTextColor );
		m_pListCtrl->SetItemBKColor( index, 0, dwBGColor );		

		g_InterfaceManager.GetInterfaceString(eST_PAY_SOME_FARE_FOR_PORTAL, szMessage, INTERFACE_STRING_LENGTH);
		Sprintf(szResult, szMessage, m_Fare);
		index = m_pListCtrl->InsertItemBack();
		GlobalFunc::SetItemText( m_pListCtrl, index, 0, szResult );
		m_pListCtrl->SetItemFontID( index, 0, StrToWzID("mn12") );
		m_pListCtrl->SetItemTextColor( index, 0, dwTextColor );
		m_pListCtrl->SetItemBKColor( index, 0, dwBGColor );		
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
AskNPCPortal::DoIt()
{
	BIT_ADD(this->m_packetStatus, MOVE_PORTAL);

	NPCPortalDialog * pPortalDlg = (NPCPortalDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_NPC_PORTAL);
	if ( pPortalDlg )
		pPortalDlg->SEND_CG_ZONE_VILLAGE_MOVE_SYN(this->m_MapCode, this->m_Fare);

    ShowInterface(FALSE);
}

//----------------------------------------------------------------------------
/**
*/
VOID			
AskNPCPortal::Cancel()
{
	BIT_REMOVE(this->m_packetStatus, MOVE_PORTAL);
	ShowInterface(FALSE);
}