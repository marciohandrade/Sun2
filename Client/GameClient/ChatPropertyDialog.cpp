#include "SunClientPrecompiledHeader.h"

#include "ChatPropertyDialog.h"
#include "InterfaceManager.h"
#include "uichatman/uiChatMan.h"
#include "ChatDialog.h"

WzID ChatPropertyDialog::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("C001"),		//	DLGPOS_CHECK_SYSTEM,
	StrToWzID("C002"),		//	DLGPOS_CHECK_SHOUT,
	StrToWzID("C003"),		//	DLGPOS_CHECK_GENERAL,
	StrToWzID("C004"),		//	DLGPOS_CHECK_WHISPER,
	StrToWzID("C005"),		//	DLGPOS_CHECK_PARTY,
	StrToWzID("C006"),		//	DLGPOS_CHECK_GUILD,
	StrToWzID("C007"),		//	DLGPOS_CHECK_CHANNEL,
	StrToWzID("B001"),		//	DLGPOS_BTN_EXIT,
};							
//------------------------------------------------------------------------------
/**
*/
ChatPropertyDialog::ChatPropertyDialog(InterfaceManager* pUIMan):
	uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
ChatPropertyDialog::~ChatPropertyDialog()
{

}
//------------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	for( int i = 0 ; i < DLGPOS_CHECK_CHANNEL +1 ; ++i)
	{
		m_pCheck[i] = static_cast<CCtrlButtonCheckWZ *>( getControl(i) );
	}

	m_pChatMan = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
	assert (m_pChatMan);
    MoveDefaultPosition();
}
//------------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_iFilterValue = m_pChatMan->m_pChatDlg->GetCurrentBitFilter();
		SetPropertyDlgFilter(m_iFilterType, m_iFilterValue);
	}
	else
	{
	    ChatDialog* pDlg = (ChatDialog*)GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG);
	    if (pDlg != NULL)
        {
		    pDlg->ChatPropertyDisable();
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getControlIDToPos(pMessage->dwCtrlID);
			switch (pos)
			{
			case DLGPOS_BTN_EXIT:
				{
					this->ShowWindow(FALSE);
				}
				break;
			case DLGPOS_CHECK_SYSTEM:
			case DLGPOS_CHECK_SHOUT:
			case DLGPOS_CHECK_GENERAL:
			case DLGPOS_CHECK_WHISPER:
			case DLGPOS_CHECK_PARTY:
			case DLGPOS_CHECK_GUILD:
			case DLGPOS_CHECK_CHANNEL:
				{
					//변경 불가값 설정.	
					ForceSettingCheckButton();
					// 비트 필드 조작.
					ComposeBitFilter();
					m_pChatMan->m_pChatDlg->SetCurrentBitFilter(m_iFilterValue);
				}
				break;
			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::NetworkProc(MSG_BASE * pMsg)
{

}

void ChatPropertyDialog::updateAllCheckButton()
{
	_updatEachCheckButton(DLGPOS_CHECK_GUILD ,CHAT_FILTER_TYPE_GUILD );
	_updatEachCheckButton(DLGPOS_CHECK_GENERAL , CHAT_FILTER_TYPE_GENERAL);
	_updatEachCheckButton(DLGPOS_CHECK_PARTY , CHAT_FILTER_TYPE_PARTY);
	_updatEachCheckButton(DLGPOS_CHECK_CHANNEL , CHAT_FILTER_TYPE_CHANNEL);
	_updatEachCheckButton(DLGPOS_CHECK_SHOUT , CHAT_FILTER_TYPE_SHOUT);
	_updatEachCheckButton(DLGPOS_CHECK_SYSTEM ,CHAT_FILTER_TYPE_SYSTEM );
	_updatEachCheckButton(DLGPOS_CHECK_WHISPER , CHAT_FILTER_TYPE_WHISPER);
}

void ChatPropertyDialog::_updatEachCheckButton(int id , int bit)
{
	if( m_iFilterValue & bit)
	{
		m_pCheck[id]->SetCheckState(TRUE);
	}
	else
	{
		m_pCheck[id]->SetCheckState(FALSE);
	}
}

void ChatPropertyDialog::ComposeBitFilter()
{
	m_iFilterValue = 0;
	_setEachBitFilter(DLGPOS_CHECK_GUILD ,CHAT_FILTER_TYPE_GUILD );
	_setEachBitFilter(DLGPOS_CHECK_GENERAL , CHAT_FILTER_TYPE_GENERAL);
	_setEachBitFilter(DLGPOS_CHECK_PARTY , CHAT_FILTER_TYPE_PARTY);
	_setEachBitFilter(DLGPOS_CHECK_CHANNEL , CHAT_FILTER_TYPE_CHANNEL);
	_setEachBitFilter(DLGPOS_CHECK_SHOUT , CHAT_FILTER_TYPE_SHOUT);
	_setEachBitFilter(DLGPOS_CHECK_SYSTEM ,CHAT_FILTER_TYPE_SYSTEM );
	_setEachBitFilter(DLGPOS_CHECK_WHISPER , CHAT_FILTER_TYPE_WHISPER);
}

void ChatPropertyDialog::_setEachBitFilter(int id , int bit)
{
	if( m_pCheck[id]->GetCheckState())
	{
		m_iFilterValue |= bit;
	}
}

void ChatPropertyDialog::SetPropertyDlgFilter(int filterType , int filterValue)
{
	m_iFilterType = filterType;
	m_iFilterValue = filterValue;
	updateAllCheckButton();

}

void ChatPropertyDialog::ForceSettingCheckButton()
{
	switch ( m_iFilterType )
	{
	case eLFM_NORMAL:
		{
			m_pCheck[DLGPOS_CHECK_GENERAL]->SetCheckState(TRUE);
		}
		break;

	case eLFM_PARTY:
		{
			m_pCheck[DLGPOS_CHECK_PARTY]->SetCheckState(TRUE);
		}
		break;

	case eLFM_GUILD:
		{
			m_pCheck[DLGPOS_CHECK_GUILD]->SetCheckState(TRUE);
		}
		break;

	case eLFM_CHANNEL:
		{
			m_pCheck[DLGPOS_CHECK_CHANNEL]->SetCheckState(TRUE);
		}
		break;

	case eLFM_SYSTEM:
		{
			m_pCheck[DLGPOS_CHECK_SYSTEM]->SetCheckState(TRUE);
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
void ChatPropertyDialog::MoveDefaultPosition()
{
	RECT rcDialog;
	ChatDialog* pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (pDlg != NULL)
    {
        pDlg->GetDialogWZ()->GetClientRect(&rcDialog);
	    POINT MyInitialPoint;
        MyInitialPoint.x = rcDialog.right + c_iAddXSizeofChatProperty + 5;
        MyInitialPoint.y =  rcDialog.top;
	    this->MoveWindowWZ((float)MyInitialPoint.x, (float)MyInitialPoint.y);
    }
}

//void ChatPropertyDialog::UpdatePosition()
//{
//	// 장착 창 뜰때 포지션 지정해준다.
//	// 스킵~
//    //MoveDefaultPosition();
//}
