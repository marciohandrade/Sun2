//------------------------------------------------------------------------------
//  uiLoginChannelList_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginChannelList.h"
#include "uiLoginBack/uiLoginBack.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
*/
uiLoginChannelList::uiLoginChannelList(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiLoginChannelList::~uiLoginChannelList()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginChannelList::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();

    for (int i=0; i<POS_TXT_SIZE; ++i)
    {
        GlobalFunc::SetCaption (m_pTxtControls[i], _T(""));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginChannelList::InitControls()
{
    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    for (int i=0; i<POS_TXT_SIZE; ++i)
    {
        m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(getControl(POS_TXT_BEGIN + i));
        assert (m_pTxtControls[i]);
    }

#ifdef	_K2P_SERVER_LIST_BY_WORD_000897
	for( i = 0; i < POS_TXT_STATE_SIZE; ++i )
	{
		m_pTxtState[i] = static_cast<CCtrlStaticWZ *>(getControl( DLGPOS_TXT_STATE_1 + i));
		assert( m_pTxtState[i] );
	}
#endif

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginChannelList::Release()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
