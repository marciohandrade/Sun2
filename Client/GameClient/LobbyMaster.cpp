#include "SunClientPrecompiledHeader.h"
#include ".\lobbymaster.h"
#include "CreateBattleDialog.h"
#include "BattleRoomListDialog.h"
#include "InterfaceManager.h"
#include "BattleScene.h"

#if !defined ( _ZONE_TEST )

WzID LobbyMaster::m_wzId[Dialog_Max] =
{
	StrToWzID("B000"),
	StrToWzID("B001"),

};

LobbyMaster::LobbyMaster(void)
{
}


LobbyMaster::~LobbyMaster(void)
{
}


VOID LobbyMaster::Init( CDrawBase * pDrawBase )
{
	// window size
	RECT* rcWindow = g_InterfaceManager.GetWindowRC();

	long wWidth, wHeight;
	if (rcWindow)
	{
		wWidth = (rcWindow->right - rcWindow->left);
		wHeight = (rcWindow->bottom - rcWindow->top);
	}
	else
	{
		wWidth = 1024;
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


VOID LobbyMaster::Release()
{

}


VOID LobbyMaster::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			switch (pos)
			{
			case LOBBY_MASTER_START:
				{
					// TODO
					// ���� ��Ʋ ����Ʈ�� �̵��ؾ� �� �ʹ�.
					// �ӽ������� ������ �κ� �� ���¿��� ���� �����Ѵ�.
					// �κ������ �̵�����.
					BattleRoomListDialog * pBattleDlg = 
						static_cast<BattleRoomListDialog *>(g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_BATTLE_ROOM ));

					CreateBattleDialog * pCreateBattleDlg = 
						static_cast<CreateBattleDialog *>(g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_BATTLE_CREATE));

					BattleRoomEntry * pBattleRoom = 
						pBattleDlg->GetBattleEntryTable()->findRoomKey(pCreateBattleDlg->GetCreateRoomInfo()->m_dwSerial);	

					pCreateBattleDlg->CreateMission();	
					//ASSERT( pBattleDlg->GetBattleEntryTable()->Delete(pBattleRoom->m_dwSerial) );				
				}
				break;

			case LOBBY_MASTER_EXIT:
				{
					MSG_CG_ZONE_LEAVELOBBYREQ_SYN MsgLeaveLobbySyn;

					MsgLeaveLobbySyn.m_LobbyKey = BattleScene::GetCurRoomKey();;

					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &MsgLeaveLobbySyn, sizeof(MsgLeaveLobbySyn));					
				}
				break;
			}
		}
		break;
	}
}


VOID LobbyMaster::NetworkProc( MSG_BASE * pMsg )
{

}


CControlWZ * LobbyMaster::getControl( POSTYPE AtPos )
{
	if( AtPos > Dialog_Max )
	{
		return NULL;
	}

	return GetControlWZ( m_wzId[AtPos] );
}


POSTYPE	LobbyMaster::getCtrlIDToPos( WzID wzId )
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
// #if !defined ( _ZONE_TEST )