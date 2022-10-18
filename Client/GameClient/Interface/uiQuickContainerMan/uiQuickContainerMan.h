#pragma once
#include "uibaseman.h"

struct IM_QUICK_CONTAINER_MANAGER
{
	static const DWORD QUICK_CONTAINER;
	static const DWORD STYLE_CONTAINER;
};

class QuickContainer;
class StyleContainer;

class uiQuickContainerMan : public uiBaseMan
{
public:
	uiQuickContainerMan(InterfaceManager *pUIMan);
//--------------------------------------------------------------------------
// init/release
//
public:
    void                    OnInitialize();

	void					SetQuickContainerCurIndex(int iIndex);
	void					PrevQuickContainerTab();
	void					NextQuickContainerTab();
    void                    ToggleSlot();
    void                    ToggleChangeSlotSize();

private:
	QuickContainer *		m_pQuickDlg;
	StyleContainer *		m_pStyleDlg;

};
