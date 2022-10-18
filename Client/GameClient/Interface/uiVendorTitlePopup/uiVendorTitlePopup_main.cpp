//------------------------------------------------------------------------------
//  uiVendorTitlePopup_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiVendorTitlePopup.h"

//------------------------------------------------------------------------------
/**
*/
uiVendorTitlePopup::uiVendorTitlePopup(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bFocus(false)
{
}

//------------------------------------------------------------------------------
/**
*/
uiVendorTitlePopup::~uiVendorTitlePopup()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorTitlePopup::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorTitlePopup::Release()
{

}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
