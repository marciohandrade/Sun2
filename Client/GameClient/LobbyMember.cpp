#include "SunClientPrecompiledHeader.h"
#include ".\lobbymember.h"
#include "PacketStruct_CG.h"
#include "InterfaceManager.h"
#include "BattleRoomListDialog.h"
#include "CreateBattleDialog.h"
#include "Hero.h"

#if !defined ( _ZONE_TEST )

WzID LobbyMember::m_wzId[] =
{
	StrToWzID("B000"),
};

LobbyMember::LobbyMember(void)
{
}


LobbyMember::~LobbyMember(void)
{
}


VOID LobbyMember::Init( CDrawBase * pDrawBase )
{
	// window size
	RECT * rcWindow = g_InterfaceManager.GetWindowRC();

	long wWidth, wHeight;
	if (rcWindow)
	{
		wWidth  = (rcWindow->right - rcWindow->left);
		wHeight = (rcWindow->bottom - rcWindow->top);
	}
	else
	{
		wWidth  = 1024;
		wHeight = 768;
	}

	// dialog size
	RECT rcDialog;
	GetDialogWZ()->GetDlgSize(&rcDialog);

	int detWidth, detHeight;
	detWidth = (rcDialog.right - rcDialog.left) /2;
	detHeight = (rcDialog.bottom - rcDialog.top) / 2;

	int iMidX = wWidth/2;
	int iMidY = wHeight/2;

	MoveWindowWZ(iMidX-detWidth, iMidY-detHeight);
}


VOID LobbyMember::Release()
{

}


VOID LobbyMember::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch (getCtrlIDToPos( pMessage->dwCtrlID ))
			{
			case LOBBYMEMBER_EXIT:
				{
					if (!m_bSendFlag)
					{
						MSG_CG_ZONE_LEAVELOBBYREQ_SYN MsgLeaveLobbySyn;

						BattleRoomListDialog * pBattleDlg = (BattleRoomListDialog *)
							g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_ROOM);

						CreateBattleDialog * pCreateDlg = (CreateBattleDialog * )
							g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_CREATE);

						MsgLeaveLobbySyn.m_LobbyKey = BattleScene::GetCurRoomKey();

						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &MsgLeaveLobbySyn, sizeof(MsgLeaveLobbySyn));
					}
				}
				break;

			default:
				{
				}
				break;
			}
		}
		break;
	}
}


VOID LobbyMember::NetworkProc( MSG_BASE * pMsg )
{

}


CControlWZ * LobbyMember::getControl( POSTYPE AtPos )
{
	if( AtPos > Dialog_Max )
	{
		return NULL;
	}

	return GetControlWZ( m_wzId[AtPos] );
}


POSTYPE	LobbyMember::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

#endif 