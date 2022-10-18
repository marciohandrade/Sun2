//------------------------------------------------------------------------------
//  uiSystemVerify_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemVerify.h"
#include "uiSystemMan/uiSystemMan.h"

//------------------------------------------------------------------------------
/**
*/
uiSystemVerify::uiSystemVerify(InterfaceManager *pUIMan)
: uiBase(pUIMan)
, m_CloseType( CT_GOTOWINDOW )
, m_bShutdownAfterVerify(false)
{
	this->m_fnSysVerify = NULL;
	m_pMessageMesh = NULL;
    m_pMyMan = NULL;

#ifdef _INTERNATIONAL_UI
	m_vSubstr.clear();
#endif//_INTERNATIONAL_UI
}

//------------------------------------------------------------------------------
/**
*/
uiSystemVerify::~uiSystemVerify()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::Clear()
{
    this->m_bShutdownAfterVerify = false;
    this->m_bLockingEffect = false;

	ClearToken();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    m_pMyMan = static_cast<uiSystemMan *>(this->GetMan());
    assert (m_pMyMan);

	this->m_pCtrlOKStatic = (CCtrlStaticWZ *)this->getControl(DLGPOS_BTN_VERIFY);
	assert(this->m_pCtrlOKStatic);
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::Release()
{
	ClearToken();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::ClearToken()
{
	this->m_strTitle.clear();
	this->m_strSubject.clear();
	this->m_dwOKStrID = 100;	//ok

    m_vstr.clear();

#ifdef _INTERNATIONAL_UI
	m_vSubstr.clear();
#endif//_INTERNATIONAL_UI
}

void uiSystemVerify::ReSetShutDownTime( DWORD dwTime )
{
	m_timerExit.Off();
	m_timerExit.On(dwTime);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
