#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiMiniGameMan.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "uiBingoGameDialog/uiBingoGameDialog.h"

const DWORD IM_MINIGAME_MANAGER::BINGOGAME_DIALOG = StrToWzID("5001");

uiMiniGameMan::uiMiniGameMan(InterfaceManager* ui_manager) : 
uiBaseMan(ui_manager), 
bingo_game_dialog_(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
uiMiniGameMan::~uiMiniGameMan(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void uiMiniGameMan::OnInitialize()
{
	bingo_game_dialog_ = CreateUIDialog<uiBingoGameDialog>(IM_MINIGAME_MANAGER::BINGOGAME_DIALOG, "Data\\Interface\\5_1_Bingo.iwz", this, TRUE);
	assert(bingo_game_dialog_);
}

//------------------------------------------------------------------------------
/**
*/
void uiMiniGameMan::ToggleBingoGameDialog()
{
	if ( bingo_game_dialog_ == NULL )
	{
		return;
	}

	if ( bingo_game_dialog_->IsVisibleWindow() == TRUE )
	{
		bingo_game_dialog_->ShowInterface(FALSE);
	}
	else
	{
		bingo_game_dialog_->ShowInterface(TRUE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMiniGameMan::NetworkProc( MSG_BASE* message )
{
	switch ( message->m_byCategory )
	{
	case CG_CHARINFO:
		{
			switch ( message->m_byProtocol )
			{
			case CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET:
				{
					OnRecv_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET(message);
				}
				break;

			case CG_CHARINFO_MINIGAME_NAK:
				{
					OnRecv_CG_CHARINFO_MINIGAME_NAK(message);
				}
				break;
			}
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMiniGameMan::OnRecv_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET( MSG_BASE* message )
{
	MSG_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET* recv_packet = 
		static_cast<MSG_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET*>(message);

	if ( bingo_game_dialog_ )
	{
		bingo_game_dialog_->BingoGameInfoSetting(recv_packet->minigame_bingo_infos);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMiniGameMan::OnRecv_CG_CHARINFO_MINIGAME_NAK( MSG_BASE* message )
{
	MSG_CG_CHARINFO_MINIGAME_NAK* recv_packet = 
		static_cast<MSG_CG_CHARINFO_MINIGAME_NAK*>(message);

	int string_code = 0;
	switch ( recv_packet->result_code )
	{
	case RC::RC_MINIGAME_INVAILD_TYPE:
		{
			// 41859 : 알 수 없는 미니게임 타입니다.
			string_code = 41859;
		}
		break;

	case RC::RC_MINIGAME_NOT_OPEN_TIME:
		{
			// 41860 : 미니게임 오픈 시간이 아닙니다.
			string_code = 41860;
		}
		break;

	case RC::RC_MINIGAME_BINGO_INVALID_ITEM_INFO:
		{
			// 41861 : 빙고 아이템정보가 잘 못 되었습니다.
			string_code = 41861;
		}
		break;

	case RC::RC_MINIGAME_BINGO_ALREADY_FILLED_NUMBER:
		{
			// 41855 : 이미 등록이 완료된 숫자입니다.
			string_code = 41855;
		}
		break;

	case RC::RC_MINIGAME_BINGO_NOT_EXIST_NUMBER:
		{
			// 41854 : 현재 빙고판에 없는 숫자입니다.
			string_code = 41854;
		}
		break;

	case RC::RC_MINIGAME_BINGO_REWARD_FAILED:
		{
			// 41862 : 빙고 보상받기에 실패하였습니다.
			string_code = 41862;
		}
		break;
	}

	if ( string_code != 0 )
	{
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(string_code));
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMiniGameMan::HotKeyToggleMiniGameBingoDialog()
{
	uiBingoGameDialog* ui_bingo_game_dialog = GET_CAST_DIALOG(uiBingoGameDialog, IM_MINIGAME_MANAGER::BINGOGAME_DIALOG);
	if (ui_bingo_game_dialog)
	{
		if ( ui_bingo_game_dialog->get_is_enable_() == false )
		{
			return;
		}
	}

	uiMiniGameMan* ui_mini_game_manager = GET_DIALOG_MANAGER(uiMiniGameMan, UIMAN_MINIGAME);
	if ( ui_mini_game_manager )
	{
		ui_mini_game_manager->ToggleBingoGameDialog();
	}
}

#endif // _NA_008405_20150716_MINIGAME_BINGO
