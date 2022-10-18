
#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "OptionKeyHelpDialog.h"
#include "ResourceManager.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "Sound.h"
#include "SoundBGM.h"
#include "globalfunc.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else
WzID OptionKeyHelpDialog::m_wzId[OPTION_KEYHELP_MAX] = 
{ 
	StrToWzID("B999"),	//	��ư - ����
};

OptionKeyHelpDialog::OptionKeyHelpDialog( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
}


OptionKeyHelpDialog::~OptionKeyHelpDialog()
{
}


VOID OptionKeyHelpDialog::Init( CDrawBase * pDrawBase )
{
	//UpdatePosition();
}

VOID OptionKeyHelpDialog::Release()
{
}

VOID OptionKeyHelpDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
		//-----------------------------------------------------------------------------------------
		//		���콺 ���� Ŭ�� �̺�Ʈ�� �Ͼ�� ��, ����?
		//-----------------------------------------------------------------------------------------
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
				//---------------------------------------------------------------------------------
				//		X ���� ��ư
				//---------------------------------------------------------------------------------

			case OPTION_KEYHELP_BT_EXIT:			//	���� ��ư - ����� �ǹ�
				{
					ShowDialog(FALSE);
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

VOID OptionKeyHelpDialog::NetworkProc( MSG_BASE * pMsg )
{
}

CControlWZ *OptionKeyHelpDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < OPTION_KEYHELP_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE OptionKeyHelpDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < OPTION_KEYHELP_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return OPTION_KEYHELP_MAX;
}

VOID OptionKeyHelpDialog::OnShowWindow( BOOL val )
{
	if( val == TRUE )
	{
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Option_KeyHelp;
		msg.wParam=InterfaceManager::DIALOG_OPTION_KEYHELP;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_KeyHelp);
	}
}
//
//void OptionKeyHelpDialog::UpdatePosition()
//{
//	//POINT pt = g_pApplication->Get2DModeMapping(); 
//
//	//// dialog size
//	//RECT rcDialog;
//	//GetDialogWZ()->GetDlgSize(&rcDialog);
//
//	//long dWidth, dHeight;
//	//dWidth = rcDialog.right / 2;
//	//dHeight = rcDialog.bottom / 2;
//
//
//	////	ȭ�� ��� ����
//	//MoveWindowWZ( (float)((pt.x / 2) - dWidth), (float)((pt.y / 2) - dHeight) );
//}
#endif