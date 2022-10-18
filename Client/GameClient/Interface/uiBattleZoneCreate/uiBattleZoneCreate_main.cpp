//------------------------------------------------------------------------------
//  uiBattleZoneCreate_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiBattleZoneCreate.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "globalfunc.h"
#include "GameConst_Color.h"

const int c_iZONE_CREATE_ROOMTYPES[] =
{
    640,
    641,
    685,
    604,
};

const int c_iZONE_CREATE_ROOMTYPES_DESCS[] =
{
    594,
    595,
    597,
    596,
};

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCreate::uiBattleZoneCreate(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
	m_bInitControls(false)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCreate::~uiBattleZoneCreate()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreate::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreate::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreate::InitControls()
{
    int i;

    for (i=0; i<NUM_POS_BTN; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    for (i=0; i<NUM_POS_CLASS_CHKBTN; ++i)
    {
        m_pChkBtnClassControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_CLASS_BEGIN + i));
        assert (m_pChkBtnClassControls[i]);
    }

	m_pChkBtnMapAll = static_cast<CCtrlButtonCheckWZ *>(this->getControl(DLGPOS_CHKBTN_MAPALL));
	assert (m_pChkBtnMapAll);

    for (i=0; i<NUM_POS_MAP_CHKBTN; ++i)
    {
        m_pChkBtnMapControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_MAP_BEGIN + i));
        assert (m_pChkBtnMapControls[i]);
    }

    for (i=0; i<NUM_POS_TXT; ++i)
    {
        m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(POS_TXT_BEGIN + i));
        assert (m_pTxtControls[i]);
        GlobalFunc::SetCaption (m_pTxtControls[i], _T(""));
    }

    for (i=0; i<NUM_POS_EDT; ++i)
    {
        m_pEdtControls[i] =
            static_cast<CCtrlDxIMEEditBox *>(this->getControl(POS_EDT_BEGIN + i));
        assert (m_pEdtControls[i]);
    }

    for (i=0; i<NUM_POS_LST; ++i)
    {
        m_pLstControls[i] =
            static_cast<CCtrlListWZ *>(this->getControl(POS_LST_BEGIN + i));
        assert (m_pLstControls[i]);
    }

	m_pListSuperMaster = static_cast<CCtrlListWZ *>( getControl(DLGPOS_LIST_SUPER_ROOM) );
		assert(m_pListSuperMaster);


	m_pButtonSuperMaster = static_cast<CCtrlButtonWZ *>(getControl(DLGPOS_BTN_SUPER_ROOM_SELECT) );
		assert(m_pButtonSuperMaster);

	m_pTextSuperMaster = static_cast<CCtrlImageStaticWZ *> ( getControl( DLGPOS_TI_SUPER_ROOM_NAME ) );
		assert(m_pTextSuperMaster);
	m_pTextSuperMaster->SetFontID(StrToWzID("mn12"));

    m_bInitControls = true;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
