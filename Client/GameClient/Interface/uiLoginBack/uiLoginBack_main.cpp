//------------------------------------------------------------------------------
//  uiLoginBack_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginBack.h"
#include "uiLoginMan/uiLoginMan.h"

//------------------------------------------------------------------------------
/**
*/
uiLoginBack::uiLoginBack(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bInitControls(false)
{
}

//------------------------------------------------------------------------------
/**
*/
uiLoginBack::~uiLoginBack()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginBack::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginBack::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginBack::InitControls()
{
    //m_pLstControl =
    //    static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_VERSION));
    //assert (m_pLstControl);

    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
