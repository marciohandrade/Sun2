//------------------------------------------------------------------------------
//  uiGuildCreate_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildCreate.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"

#include "uiGuildMan/uiGuildMan.h"
#include "uiSystemMan/uiSystemMan.h"
//------------------------------------------------------------------------------
/**
*/
uiGuildCreate::uiGuildCreate(InterfaceManager *pUIMan) :
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
    memset ((CCtrlStaticWZ *)this->m_pTxtControls, NULL, sizeof (this->m_pTxtControls));
}

//------------------------------------------------------------------------------
/**
*/
uiGuildCreate::~uiGuildCreate()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildCreate::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildCreate::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildCreate::InitControls()
{
    this->m_pEdtControl =
        static_cast<CCtrlDxIMEEditBox *>(this->getControl(DLGPOS_EDT_GUILD_NAME));
    assert (this->m_pEdtControl);

    int i;

    for (i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (this->m_pBtnControls[i]);
    }

    for (i=0; i<POS_TXT_SIZE; ++i)
    {
        this->m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(POS_TXT_BEGIN + i));
        assert (this->m_pTxtControls[i]);
    }

    this->m_bControlsInitialized = true;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
