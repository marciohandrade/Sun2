//------------------------------------------------------------------------------
//  uiLoginNotice_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#ifdef _CK_MOD_UI_LOGIN
#else
#include "uiLoginNotice.h"

//------------------------------------------------------------------------------
/**
*/
uiLoginNotice::uiLoginNotice(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiLoginNotice::~uiLoginNotice()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginNotice::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginNotice::Release()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

#endif