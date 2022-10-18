//------------------------------------------------------------------------------
//  uiSystemIdNumberConfirm_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemIdNumberConfirm.h"

#if defined(_CHINA) 
	#define MAX_IDENTIFY_NUM	12	
#elif defined( _JAPAN )
	#define MAX_IDENTIFY_NUM	8
#else
	#ifdef _MOD_IDENTITY_NUM__FOR_CHANNELING
		#define MAX_IDENTIFY_NUM	6
	#else
		#define MAX_IDENTIFY_NUM	7
	#endif
#endif


//------------------------------------------------------------------------------
/**
*/
uiSystemIdNumberConfirm::uiSystemIdNumberConfirm(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
    m_pMessageMesh = NULL;

    m_vstr.clear();
}

//------------------------------------------------------------------------------
/**
*/
uiSystemIdNumberConfirm::~uiSystemIdNumberConfirm()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemIdNumberConfirm::Clear()
{
    this->m_bLockingEffect = false;

    ClearToken();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemIdNumberConfirm::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	CCtrlDxEditBox * pEdit= (CCtrlDxEditBox *)getControl( DLGPOS_EDIT );
	pEdit->SetLimitText( MAX_IDENTIFY_NUM );
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemIdNumberConfirm::Release()
{
	ClearToken();
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemIdNumberConfirm::ClearToken()
{
    m_vstr.clear();
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
