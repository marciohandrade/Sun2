//------------------------------------------------------------------------------
//  uiGuildMenu_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildMenu.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiSystemMan/uiSystemMan.h"
//------------------------------------------------------------------------------
/**
*/
uiGuildMenu::uiGuildMenu(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pGuildMan =
        static_cast<uiGuildMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD));
    assert (m_pGuildMan);

    if (!this->m_pGuildMan)
    {
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pGuildMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

    memset ((CCtrlButtonWZ *)this->m_pBtnControls, NULL, sizeof (this->m_pBtnControls));
    this->m_pListControl = NULL;

	m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiGuildMenu::~uiGuildMenu()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMenu::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMenu::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMenu::InitControls()
{
    this->m_pListControl =
        static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_GUILD_NOTICE));
    assert (this->m_pListControl);

    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_EXIT + i));
        assert (this->m_pBtnControls[i]);

		if(this->m_pBtnControls[i])
			this->m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
    }

	this->m_pBtnControls[DLGPOS_BTN_EXIT]->ShowWindowWZ(WZ_SHOW);
	m_pBtnControls[DLGPOS_BTN_GUILD_CREATE_AND_DESTORY]->ShowWindowWZ(WZ_SHOW);
    this->m_bControlsInitialized = true;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
