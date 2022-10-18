//------------------------------------------------------------------------------
//  uiVendorPopup_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiVendorPopup.h"

//------------------------------------------------------------------------------
/**
*/
uiVendorPopup::uiVendorPopup(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bFocus(false),
    m_bModify(false)
{
}

//------------------------------------------------------------------------------
/**
*/
uiVendorPopup::~uiVendorPopup()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorPopup::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorPopup::Release()
{
	
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------