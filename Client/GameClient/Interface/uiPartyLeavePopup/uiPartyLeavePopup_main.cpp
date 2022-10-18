//------------------------------------------------------------------------------
//  uiPartyLeavePopup_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiPartyLeavePopup.h"

//------------------------------------------------------------------------------
/**
*/
uiPartyLeavePopup::uiPartyLeavePopup(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiPartyLeavePopup::~uiPartyLeavePopup()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::Release()
{
}
bool uiPartyLeavePopup::ShowInterface( BOOL val, BOOL showOff )
{	
	return uiBase::ShowInterface( val, showOff );
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
