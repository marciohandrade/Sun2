//------------------------------------------------------------------------------
//  uiLoginSer(verList_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginServerList.h"
#include "uiLoginBack/uiLoginBack.h"

//------------------------------------------------------------------------------
/**
*/
uiLoginServerList::uiLoginServerList(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bInitControls(false)
{
    ZeroMemory (&m_pTxtControls, POS_TXT_SIZE);
    m_pLstControl = NULL;
    m_pVScrollControl = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiLoginServerList::~uiLoginServerList()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::InitControls()
{
	int i = 0;
#ifdef _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
	// no operation
#else
	for (i=0; i<POS_BTN_SIZE; ++i)
	{
		m_pBtnControls[i] =
			static_cast<CCtrlButtonWZ *>(getControl(POS_BTN_BEGIN + i));
		assert (m_pBtnControls[i]);
	}
#endif    

    for (i=0; i<POS_TXT_SIZE; ++i)
    {
        m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(getControl(POS_TXT_BEGIN + i));
        assert (m_pTxtControls[i]);
    }

    m_pLstControl =
        static_cast<CCtrlListWZ *>(getControl(DLGPOS_LST_SERVERS));
    assert (m_pLstControl);

	m_pLstControl->SetHideReportRender(true);

	m_pNewServerIcon = ICONMGR()->Add(71005);

    m_pVScrollControl =
        static_cast<CCtrlVScrollWZ *>(getControl(DLGPOS_SCR_VERTICAL));
    assert (m_pVScrollControl);

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::Release()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
