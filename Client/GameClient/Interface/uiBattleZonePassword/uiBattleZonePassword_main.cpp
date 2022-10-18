//------------------------------------------------------------------------------
//  uiBattleZonePassword_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZonePassword.h"


//------------------------------------------------------------------------------
/**
*/
uiBattleZonePassword::uiBattleZonePassword(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZonePassword::~uiBattleZonePassword()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZonePassword::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZonePassword::Release()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
