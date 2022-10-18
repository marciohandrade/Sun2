
#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "OptionMenuDialog.h"
#include "ResourceManager.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "Sound.h"
#include "SoundBGM.h"
#include "globalfunc.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else
WzID OptionMenuDialog::m_wzId[OPTION_MENU_MAX] = 
{ 
	StrToWzID("TI01"),	//OPTION_MENU_BT_SYSTEM_OPTION	//	그래픽 옵션 버튼
	StrToWzID("TI02"),	//OPTION_MENU_BT_GAME_OPTION	//	게임옵션 버튼
	StrToWzID("B999"),	//OPTION_MENU_BT_EXIT			//	버튼 - 종료
	StrToWzID("TI03"),	//OPTION_MENU_BT_SOUND			//  사운드 옵션
	StrToWzID("TI04"),	//OPTION_MENU_BT_KEY_OPTION		//	키설정 버튼
};

OptionMenuDialog::OptionMenuDialog( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
}


OptionMenuDialog::~OptionMenuDialog()
{
}


VOID OptionMenuDialog::Init( CDrawBase * pDrawBase )
{
	//UpdatePosition();
    CCtrlImageStaticWZ* key_option_button = 
        static_cast<CCtrlImageStaticWZ*>(getControl(OPTION_MENU_BT_KEY_OPTION));
    if (key_option_button != NULL)
    {
        key_option_button->SetTextWZ(g_InterfaceManager.GetInterfaceString(8300));
    }
}

VOID OptionMenuDialog::Release()
{
}

VOID OptionMenuDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
		//-----------------------------------------------------------------------------------------
		//		마우스 왼쪽 클릭 이벤트가 일어났을 때, 어디냐?
		//-----------------------------------------------------------------------------------------
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
				//---------------------------------------------------------------------------------
				//		아래 세개 버튼과 X 종료 버튼
				//---------------------------------------------------------------------------------

			case OPTION_MENU_BT_SYSTEM_OPTION:	//	시스템 메뉴 호출
				{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
					//	옵션 다이얼로그
					GET_DIALOG( IM_OPTION_MENU_MANAGER::OPTION_DIALOG )->ShowInterface( TRUE );
					ShowDialog(FALSE);
#endif
				}
				break;

			case OPTION_MENU_BT_GAME_OPTION:	//	게임 옵션 메뉴 호출
				{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
					GET_DIALOG( IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG )->ShowInterface( TRUE );
					ShowDialog(FALSE);
#endif
				}
				break;

			case OPTION_MENU_BT_KEY_OPTION:		//	키 설정 옵션
				{
                    GET_DIALOG(IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG)->ShowInterfaceWithSound(TRUE);
					ShowDialog(FALSE);
				}
				break;

			case OPTION_MENU_BT_EXIT:			//	종료 버튼 - 취소의 의미
				{
					ShowDialog(FALSE);
				}
				break;

			case OPTION_MENU_BT_SOUND:
				{
					GET_DIALOG( IM_OPTION_MENU_MANAGER::OPTION_SOUND_DIALOG )->ShowInterface( TRUE );
					ShowDialog(FALSE);
				}
				break;

			default:
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

VOID OptionMenuDialog::NetworkProc( MSG_BASE * pMsg )
{
}

CControlWZ *OptionMenuDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < OPTION_MENU_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE OptionMenuDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < OPTION_MENU_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return OPTION_MENU_MAX;
}

VOID OptionMenuDialog::OnShowWindow( BOOL val )
{
	if( val == TRUE )
	{
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Option_Menu;
		msg.wParam=InterfaceManager::DIALOG_OPTION_MENU;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_Menu);
	}
}
//
//void OptionMenuDialog::UpdatePosition()
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
//	////	화면 가운데 설정
//	//MoveWindowWZ( (float)((pt.x / 2) - dWidth), (float)((pt.y / 2) - dHeight) );
//}
#endif

