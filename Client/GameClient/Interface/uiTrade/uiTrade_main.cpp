//------------------------------------------------------------------------------
//  uiTrade_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiTrade.h"
#include "ItemUnitRenderer.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Hero.h"
#include "uiTradeMan/uiTradeMan.h"
#include "uiSystemMan/uiSystemMan.h"
//------------------------------------------------------------------------------
/**
*/
uiTrade::uiTrade(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_pItemUnitRenderer(NULL),
    m_pBtnComplete(NULL),
    m_pBtnAccept(NULL),
    m_pBtnCancel(NULL)
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	,m_pTxtComplete(NULL),
    m_pTxtAccept(NULL),
    m_pTxtCancel(NULL)
#endif
{
    ZeroMemory (&m_pSlotBtnControls1, sizeof (m_pSlotBtnControls1));
    ZeroMemory (&m_pSlotBtnControls2, sizeof (m_pSlotBtnControls2));
    ZeroMemory (&m_pBtnMoneys, sizeof (m_pBtnMoneys));
    ZeroMemory (&m_pTxtItemNames, sizeof (m_pTxtItemNames));
    ZeroMemory (&m_pTxtStatus, sizeof (m_pTxtStatus));
    ZeroMemory (&m_pTxtMoneys, sizeof (m_pTxtMoneys));
    ClearCheckItem();

    SCItemSlotContainer::Init( MAX_TRADE_SLOT_NUM, SI_TRADE );

    this->m_pTradeMan =
        static_cast<uiTradeMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_TRADE));
    assert (m_pTradeMan);

    if (!this->m_pTradeMan)
    {
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pTradeMan이 없습니다."));

#endif
        this->ShowInterface(FALSE);
        return;
    }
}

//------------------------------------------------------------------------------
/**
*/
uiTrade::~uiTrade()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::Release()
{
    SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer *
uiTrade::GetSlotContainer()
{
    return static_cast<SCSlotContainer *>(this);
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::InitControls()
{
    int i;

    for (i=0; i<POS_1_SLOT_SIZE; ++i)
    {
        this->m_pSlotBtnControls1[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_SLOT00 + i));
        assert (this->m_pSlotBtnControls1[i]);
    }

    for (i=0; i<POS_2_SLOT_SIZE; ++i)
    {
        this->m_pSlotBtnControls2[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_SLOT10 + i));
        assert (this->m_pSlotBtnControls2[i]);
    }

    for (i=0; i<2; ++i)
    {
        this->m_pBtnMoneys[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_1_MONEY + i));
        assert (this->m_pBtnMoneys[i]);
    }

    this->m_pBtnMoneys[1]->ShowWindowWZ(WZ_HIDE);

    for (i=0; i<2; ++i)
    {
        this->m_pTxtStatus[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_1_STATUS + i));
        assert (this->m_pTxtStatus[i]);
    }

    for (i=0; i<2; ++i)
    {
        this->m_pTxtItemNames[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_1_ITEM_NAME + i));
        assert (this->m_pTxtItemNames[i]);
    }

    for (i=0; i<2; ++i)
    {
        this->m_pTxtMoneys[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_1_MONEY + i));
        assert (this->m_pTxtMoneys[i]);
    }

    this->m_pBtnComplete =
        static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_ACCEPT));
    assert (this->m_pBtnComplete);

    this->m_pBtnAccept =
        static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_PROPOSAL));
    assert (this->m_pBtnAccept);

    this->m_pBtnCancel =
        static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_CANCEL));
    assert (this->m_pBtnCancel);

#ifdef _NA_000000_20130114_RENEWER_UI
#else

    this->m_pTxtComplete =
        static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_COMPLETE));
    assert (this->m_pTxtComplete);

    this->m_pTxtAccept =
        static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_ACCEPT));
    assert (this->m_pTxtAccept);

    this->m_pTxtCancel =
        static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_CANCEL));
    assert (this->m_pTxtCancel);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
