//------------------------------------------------------------------------------
//  uiBattleZoneList_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiBattleZoneList.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneList::uiBattleZoneList(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
    for (int i = 0; i < NUM_POS_CHKBTN; ++i)
    {
        m_pChkBtnViewControls[i] = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneList::~uiBattleZoneList()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();

#ifdef __LTJ_DISABLE_HUNTINGZONE_BUTTON_JP
	CCtrlButtonCheckWZ* pHButtonCheck = (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C004"));
	if (pHButtonCheck)
	{			
		pHButtonCheck->EnableWindowWZ(false);
		pHButtonCheck->SetCheckState(true);
	}
	CControlWZ* pHControl = GetControlWZ(StrToWzID("TI08"));
	if (pHControl)
	{
		pHControl->EnableWindowWZ(false);
	}
#endif //__LTJ_DISABLE_HUNTINGZONE_BUTTON_JP
#ifdef __LTJ_DISABLE_PVPZONE_BUTTON_GSP
	CCtrlButtonCheckWZ* pPVPButtonCheck = (CCtrlButtonCheckWZ*)GetControlWZ(StrToWzID("C005"));
	if (pPVPButtonCheck)
	{			
		pPVPButtonCheck->EnableWindowWZ(false);
		pPVPButtonCheck->SetCheckState(true);
	}
	CControlWZ* pPVPControl = GetControlWZ(StrToWzID("TI01"));
	if (pPVPControl)
	{
		pPVPControl->EnableWindowWZ(false);
	}
#endif //__LTJ_DISABLE_PVPZONE_BUTTON_GSP
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::InitControls()
{
    for (int i=0; i<NUM_POS_BTN; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    for (int i=0; i<NUM_POS_CHKBTN; ++i)
    {
        m_pChkBtnViewControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_BEGIN + i));
        assert (m_pChkBtnViewControls[i]);
    }

    for (int i=0; i<NUM_POS_CHKBTN_SEARCH; ++i)
    {
        m_pChkBtnSearchControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_SEARCH_BEGIN + i));
        assert (m_pChkBtnSearchControls[i]);
    }

    m_pEdtControl = static_cast<CCtrlDxIMEEditBox *>(this->getControl(DLGPOS_EDT_SEARCH));
    m_pVScrollControl = static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_SCR_VERTICAL));
    m_pLstControl = static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_ROOMLIST));
    m_pTabControl = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TAB_ROOMLIST));

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneList::Release()
{
    
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
