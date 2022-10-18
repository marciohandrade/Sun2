//------------------------------------------------------------------------------
//  uiSystemConfirm_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemConfirm.h"
#include "Command.h"

//------------------------------------------------------------------------------
/**
*/
uiSystemConfirm::uiSystemConfirm(InterfaceManager *pUIMan)
: uiBase( pUIMan )
, m_bLockingEffect( false )
{
    m_pMessageMesh = NULL;
	m_fnSysConfirm = NULL;

    m_pOkCommand = NULL;
    m_pCancelCommand = NULL;

    m_vstr.clear();
    
	m_bDestroyWindow = false;
	m_TimerKey = m_dwDialogKey;

    m_TitleTextGap = 0;
}

//------------------------------------------------------------------------------
/**
*/
uiSystemConfirm::~uiSystemConfirm()
{
    if( m_pOkCommand != NULL )
    {
        delete m_pOkCommand;
    }

    if( m_pCancelCommand != NULL )
    {
        delete m_pCancelCommand;
    }

    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::Clear()
{
    m_bLockingEffect = false;

	m_fnSysConfirm = NULL;
    m_fnSysConfirmCloseButton = NULL;

    ClearToken();

	m_bDestroyWindow = false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    m_pCtrlStatic[0] = (CCtrlStaticWZ *)getControl(DLGPOS_BTN_YES);
	assert(m_pCtrlStatic[0]);
	m_pCtrlStatic[1] = (CCtrlStaticWZ *)getControl(DLGPOS_BTN_NO);
	assert(m_pCtrlStatic[1]);

	m_bShow = FALSE;

	m_bDestroyWindow = false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::Release()
{

    ClearToken();

}

//------------------------------------------------------------------------------
/**
*/
void uiSystemConfirm::SetOkCommand( Command* pCommand )
{
    if( m_pOkCommand != NULL )
    {
        delete m_pOkCommand;
    }
    m_pOkCommand = pCommand;
}
//------------------------------------------------------------------------------
/**
*/
void uiSystemConfirm::SetCancelCommand( Command* pCommand )
{
    if( m_pCancelCommand != NULL )
    {
        delete m_pCancelCommand;
    }
    m_pCancelCommand = pCommand;
}
//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::ClearToken()
{
	this->m_strTitle.clear();
	this->m_vSubject.clear();

	this->m_dwOKStrID		= 100;
	this->m_dwCancleStrID	= 101;

    this->m_vstr.clear();
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
