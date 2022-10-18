//------------------------------------------------------------------------------
//  uiBattleZoneReadyCHuntingConfig_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneReadyCHuntingConfig.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
*/
uiBattleZoneReadyCHuntingConfig::uiBattleZoneReadyCHuntingConfig(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
    m_bShowFieldList = false;
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneReadyCHuntingConfig::~uiBattleZoneReadyCHuntingConfig()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::InitControls()
{
    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    m_pLstControl = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_FIELDLIST));
    assert (m_pLstControl);

    m_pTxtControl = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_FIELD));
    assert (m_pTxtControl);
    GlobalFunc::SetCaption(m_pTxtControl, _T(""));

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::Release()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
