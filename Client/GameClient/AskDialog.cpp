//------------------------------------------------------------------------------
//  CharSelectDialog.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "AskDialog.h"
#include "GameFramework.h"
#include "Interfacemanager.h"
#include "Globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GameConst_Color.h"
WzID AskDialog::m_wzId[ASK_MAX] =
{
    StrToWzID("TI00"),  // ok
    StrToWzID("TI01"),  // cancel
    StrToWzID("L000"),  // text list 
};

//------------------------------------------------------------------------------
/**
*/
AskDialog::AskDialog(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
    this->m_status = ASK_STATUS_CANCEL;
}

//------------------------------------------------------------------------------
/**
*/
AskDialog::~AskDialog(void)
{
	_asm nop;
}

//------------------------------------------------------------------------------
/**
*/
VOID AskDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_pOKButton = NULL;
	m_pCancelButton = NULL;
	m_pListCtrl = NULL;

	GetControls();

	this->m_status = ASK_STATUS_NORMAL;
}

//------------------------------------------------------------------------------
/**
*/
void AskDialog::OnShowWindow(BOOL val)
{
    if (val)
    {
        this->m_status = ASK_STATUS_NORMAL;
    }
    else
    {
        this->m_status = ASK_STATUS_CANCEL;
    }
}

//------------------------------------------------------------------------------
/**
*/
VOID AskDialog::Release()
{
}

//------------------------------------------------------------------------------
/** 텍스트를 개행처리한다.
*/
void AskDialog::InsertMessage(TCHAR	*szMessage,DWORD bkColor,DWORD TxtColor)
{
	if(m_pListCtrl)
	{
		m_pListCtrl->DeleteItemAll();
		m_pListCtrl->SetBeginDrawIndex(0);
		m_pListCtrl->SetUnuseSelBar(TRUE);
		m_pListCtrl->OnFocusOut();

		std::vector<util::_tstring> vstrMsg;
		vstrMsg.clear();
	#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int iListWidth = m_pListCtrl->GetSize().right - 15;
	#else
		int iListWidth = m_pListCtrl->GetSize().right - 5;
	#endif	

	#ifdef _INTERNATIONAL_UI  
		DWORD dwFontID  =StrToWzID("st10");
	#else
		DWORD dwFontID  =StrToWzID("mn12");
	#endif

		GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, dwFontID, _tcslen(szMessage) ,iListWidth );

		for( unsigned int i =0; i < vstrMsg.size(); ++i )
		{
			int index = m_pListCtrl->InsertItemBack();
			GlobalFunc::SetItem(m_pListCtrl, index , 0, (TCHAR *)vstrMsg[i].c_str() ,dwFontID,
				DT_VCENTER | DT_LEFT, bkColor,TxtColor , c_InterfaceSelColor);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
VOID AskDialog::MessageProc( SI_MESSAGE * pMessage )
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
                    ShowWindow( FALSE );
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

//------------------------------------------------------------------------------
/**
*/
VOID AskDialog::NetworkProc( MSG_BASE * pMsg )
{
	_asm nop;
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ * AskDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < ASK_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE AskDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < ASK_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return ASK_MAX;
}


//------------------------------------------------------------------------------
/**
*/
VOID AskDialog::GetControls()
{
	m_pOKButton = static_cast<CCtrlImageStaticWZ*>(getControl(ASK_OK));
	SUN_ASSERT(m_pOKButton);
	m_pCancelButton = static_cast<CCtrlImageStaticWZ*>(getControl(ASK_CANCEL));
	SUN_ASSERT(m_pCancelButton);
	m_pListCtrl = static_cast<CCtrlListWZ*>(getControl(ASK_LIST));
	SUN_ASSERT(m_pListCtrl);
}