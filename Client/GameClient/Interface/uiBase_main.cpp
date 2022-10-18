//------------------------------------------------------------------------------
//  uiBase_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiBase.h"
#include "ChatDialog.h"
#include "Application.h"
//------------------------------------------------------------------------------
/**
*/
uiBase::uiBase(InterfaceManager *pUIMan)
: m_bClone( FALSE )
{
    m_ResourceInfo.m_dwDialogKey = 0xFFFFFFFF;
    memset(m_ResourceInfo.m_szDialogResName, 0, sizeof(char)*MAX_PATH );

    this->m_pUIMan = pUIMan;

    this->m_isLock = false;
    this->m_isPending = false;

	m_pDraw = NULL;
    m_pMan = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiBase::~uiBase()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBase::Init(CDrawBase* pDrawBase)
{
    this->m_pDraw = pDrawBase;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBase::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBase::ShowInterface( BOOL val, BOOL showOff/* =FALSE  */)
{
    if( m_ResourceInfo.m_dwDialogKey == 0xFFFFFFFF )
    {
        WzAssert(!"���ҽ� ������ �Է��� �� ShowInterface�� ����Ͻʼ�.");
        return false;
    }

    bool ret = true;

    if (val)
    {
        ret = this->LoadInterface();
    }

    if (this->IsActivated() && ret && (showOff==FALSE))
	{
        this->ShowWindow(val);
	}

    return ret;
}

//----------------------------------------------------------------------------
/** ShowInterfaceWithSound
 * Desc: ShowInterface �Լ��� ������ ������ â ���� ������ �Ҹ� ���
 * Author : ���翵
*/	
bool                    
uiBase::ShowInterfaceWithSound( BOOL val, BOOL showOff/*=FALSE*/ )
{
    if( m_ResourceInfo.m_dwDialogKey == 0xFFFFFFFF )
    {
        WzAssert(!"���ҽ� ������ �Է��� �� ShowInterface�� ����Ͻʼ�.");
        return false;
    }

    bool ret = true;

    if (val)
    {
        ret = this->LoadInterface();
    }

    if (this->IsActivated() && ret && (showOff==FALSE))
	{
        this->ShowDialog(val);
	}

    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool uiBase::LoadInterface()
{
	if (IsLoaded() == FALSE)
	{
        if (!g_InterfaceManager.LoadInterface( (SolarDialog *)this,
            m_ResourceInfo.m_szDialogResName,
            m_ResourceInfo.m_dwDialogKey,
            FALSE, !m_bClone ))
        {
            assert (!"�������̽� �ҷ����� �����Դϴ�.");
            SetActivated(false);
            return false;
        }
    }

    SetActivated( true );

    return true;
}
//------------------------------------------------------------------------------ 
BOOL	uiBase::CloneInterface()
{
	if (IsLoaded() == FALSE)
	{
		if( !g_InterfaceManager.CloneInterface( this, m_ResourceInfo.m_szDialogResName, m_ResourceInfo.m_dwDialogKey ) )
        {
            assert (!"�������̽� �ҷ����� �����Դϴ�.");
            SetActivated(false);
            return FALSE;
        }
	}

    SetActivated(true);

	return TRUE;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
