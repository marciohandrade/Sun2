//------------------------------------------------------------------------------
//  uiBattleZoneCreateNum_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneCreateNum.h"


//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCreateNum::uiBattleZoneCreateNum(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCreateNum::~uiBattleZoneCreateNum()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateNum::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateNum::Release()
{
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
