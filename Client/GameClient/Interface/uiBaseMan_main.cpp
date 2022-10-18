//------------------------------------------------------------------------------
// uiBaseMan_main.cpp
// (C) 2005 ckbang
// 2012 01 12 milky
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiBaseMan.h"

//------------------------------------------------------------------------------
uiBaseMan::uiBaseMan(InterfaceManager *pUIMan)
{
    assert(pUIMan);

    m_pUIMan = pUIMan;
    m_pDraw = NULL;
}

//------------------------------------------------------------------------------
uiBaseMan::~uiBaseMan()
{
    // uiBaseMan::Release가 정상적으로 호출되지 않았다 -_-+
    assert( m_pUIMan == NULL );
}

//------------------------------------------------------------------------------
BOOL uiBaseMan::Init(CDrawBase* pDrawBase)
{
    m_pDraw = pDrawBase;

    InitPKBooleans();

    OnInitialize();

    //if( OnInitialize() == FALSE )
    //{
    //    return FALSE;
    //}

    return TRUE;
}

//------------------------------------------------------------------------------
void uiBaseMan::Release()
{
    UnLoadUIAll();

    OnRelease();

    m_pUIMan = NULL;
    m_pDraw = NULL;
}

//------------------------------------------------------------------------------
void uiBaseMan::Reset()
{
    OnReset();
}


//------------------------------------------------------------------------------
void uiBaseMan::Update()
{
}

//------------------------------------------------------------------------------
void uiBaseMan::Process(DWORD dwTick)
{
    HandlerPKBooleans();
}


//------------------------------------------------------------------------------
BOOL	uiBaseMan::ReLoadUI( DWORD dwKey )
{
	if( UnLoadUI( dwKey ) == FALSE )
	{
		assert( !"Dialog Unload Failed" );
		return FALSE;
	}

    OnInitialize();

	return TRUE;
}

//------------------------------------------------------------------------------
BOOL	uiBaseMan::InsertDialogKey( DWORD dwKey )
{
	m_DialogIdList.insert( dwKey );

	return TRUE;
}

//------------------------------------------------------------------------------
BOOL uiBaseMan::UnLoadUIAll()
{
	if( m_DialogIdList.empty() )
		return FALSE;

	DIALOG_ID_SET_ITR_TYPE	itr = m_DialogIdList.begin();
	DIALOG_ID_SET_ITR_TYPE	end = m_DialogIdList.end();

	for( ; itr != end; ++itr )
	{
		DWORD dwKey = *itr;

		SolarDialog * pDlg = GetUIMan()->GetDialog( dwKey );

		if( pDlg )
		{
			pDlg->Release();
			GetUIMan()->RemoveDialog( dwKey );
			delete pDlg;
		}
		else
		{
			assert( !"cannot find dialog key in SolarManager" );
		}

		if( FALSE == GetUIMan()->GetDialogManager()->RemoveDialogWZ( dwKey ) )
		{
			assert( !"dialog remove failed" );
		}
	}

	m_DialogIdList.clear();

	return TRUE;
}

//------------------------------------------------------------------------------
BOOL uiBaseMan::UnLoadUI( DWORD dwKey )
{
	SolarDialog * pDlg = GetUIMan()->GetDialog( dwKey );

	if( pDlg )
	{
		pDlg->Release();
		GetUIMan()->RemoveDialog( dwKey );
		delete pDlg;
	}
	else
	{
		assert( !"cannot find dialog key in SolarManager" );
		return FALSE;
	}

	if( FALSE == GetUIMan()->GetDialogManager()->RemoveDialogWZ( dwKey ) )
    {
        assert( !"dialog remove failed" );
		return FALSE;
    }

	DIALOG_ID_SET_ITR_TYPE	itr = m_DialogIdList.find( dwKey );
	DIALOG_ID_SET_ITR_TYPE	end = m_DialogIdList.end();

	if( itr != end )
	{
		m_DialogIdList.erase( itr );
	}
	else
	{
		assert( !"cannot find dialog key in DialogIdList" );
		return FALSE;
	}

	return TRUE;
}
