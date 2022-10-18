//------------------------------------------------------------------------------
//  uiSystemInputMessageConfirm_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemInputMessageConfirm.h"

const DWORD kLimitLength = 16;

//------------------------------------------------------------------------------
/**
*/
uiSystemInputMessageConfirm::uiSystemInputMessageConfirm(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
    m_pMessageMesh = NULL;

    m_vstr.clear();
}

//------------------------------------------------------------------------------
/**
*/
uiSystemInputMessageConfirm::~uiSystemInputMessageConfirm()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemInputMessageConfirm::Clear()
{
    this->m_bLockingEffect = false;

    ClearToken();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemInputMessageConfirm::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
	
	CCtrlDxEditBox *pEdit=(CCtrlDxEditBox *)getControl( DLGPOS_EDIT );
	pEdit->SetLimitText(kLimitLength);	
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemInputMessageConfirm::Release()
{
	ClearToken();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemInputMessageConfirm::ClearToken()
{
    m_vstr.clear();
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
