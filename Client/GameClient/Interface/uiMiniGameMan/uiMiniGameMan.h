#pragma once

#ifdef _NA_008405_20150716_MINIGAME_BINGO

#include "uiBaseMan.h"

struct IM_MINIGAME_MANAGER
{
	static const DWORD BINGOGAME_DIALOG;
};

class uiBingoGameDialog;
class uiMiniGameMan : public uiBaseMan
{
public:
	uiMiniGameMan(InterfaceManager* ui_manager);
	~uiMiniGameMan(void);

public:
	virtual void OnInitialize();
	virtual void NetworkProc( MSG_BASE* message );

private:
	void OnRecv_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET(MSG_BASE* message);
	void OnRecv_CG_CHARINFO_MINIGAME_NAK(MSG_BASE* message);

public:
	static void HotKeyToggleMiniGameBingoDialog();

public:
	void ToggleBingoGameDialog();

private:
	uiBingoGameDialog* bingo_game_dialog_;
};

#endif // _NA_008405_20150716_MINIGAME_BINGO
