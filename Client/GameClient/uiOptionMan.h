#pragma once

#include "uiBaseMan.h"

struct IM_OPTION_MANAGER
{
	static const DWORD kOptionView_ID;
    static const DWORD kOptionKeyBindingView_ID;
};

class uiOptionMan 
    : public uiBaseMan
{
public:
	int tap_Page_; // 0 - option , 1 - key
public:
    uiOptionMan(InterfaceManager* manager);

    static void ShowOptionDialog(int ActiveTabIndex );
    static void ShowKeyBindingDialog();

	static void ToggleShowActionTap();

private:
    void OnInitialize();
    void NetworkProc(MSG_BASE* pk);
};
