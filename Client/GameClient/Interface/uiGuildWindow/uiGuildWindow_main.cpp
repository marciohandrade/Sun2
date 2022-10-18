//------------------------------------------------------------------------------
//  uiGuildWindow_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildWindow.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiSystemMan/uiSystemMan.h"
//------------------------------------------------------------------------------
/**
*/
uiGuildWindow::uiGuildWindow(InterfaceManager *pUIMan) :
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

    this->m_pExitBtnControl = NULL;

    for (int i=0; i<POS_TXT_SIZE; ++i)
        this->m_pTxtControls[i] = NULL;

    for (i=0; i<POS_CHKBTN_SIZE; ++i)
        this->m_pChkBtnControls[i] = NULL;

	m_pLstControlNotice = NULL;
	m_pScroll			= NULL;	

	//m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiGuildWindow::~uiGuildWindow()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildWindow::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildWindow::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildWindow::InitControls()
{
    int i;

    for (i=0; i<POS_CHKBTN_SIZE; ++i)
    {
        this->m_pChkBtnControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_BEGIN + i));
        assert (this->m_pChkBtnControls[i]);
    }

    for (i=0; i<POS_TXT_SIZE; ++i)
    {
        this->m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(POS_TXT_BEGIN + i));
        assert (this->m_pTxtControls[i]);
    }

    this->m_pExitBtnControl =
        static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_EXIT));
    assert (this->m_pExitBtnControl);

	m_pLstControlNotice = (CCtrlListWZ *) getControl(DLGPOS_LST_GUILD_NOTICE);
	assert(m_pLstControlNotice);

	m_pScroll	=	(CCtrlVScrollWZ *) getControl(DLGPOS_VSCROLL_NOTICE);
	assert (this->m_pScroll);

    this->m_bControlsInitialized = true;
}


#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
