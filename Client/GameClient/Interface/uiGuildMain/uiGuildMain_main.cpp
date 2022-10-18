//------------------------------------------------------------------------------
//  uiGuildMain_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildMain.h"
#include "BattleRoomEntry.h"
#include "BattleEntryTable.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiSystemMan/uiSystemMan.h"
//------------------------------------------------------------------------------
/**
*/
uiGuildMain::uiGuildMain(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pGuildMan =
        static_cast<uiGuildMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD));
    assert (m_pGuildMan);

    if (!this->m_pGuildMan)
    {
#ifdef _DEBUG

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pGuildMan이 없습니다."));

#endif
        this->ShowInterface(FALSE);
        return;
    }

    memset ((CCtrlButtonWZ *)this->m_pBtnControls, NULL, sizeof (this->m_pBtnControls));

    m_pLstControl = NULL;
    m_pScrollControl = NULL;
    last_scrollbar_position_ = NULL;
	m_strAttemptChageMasterName.clear();
	m_bClickMemberRightInfoButton = false;
}

//------------------------------------------------------------------------------
/**
*/
uiGuildMain::~uiGuildMain()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::Release()
{
}

//------------------------------------------------------------------------------
void uiGuildMain::Reset()
{
    SolarDialog::Reset();
    set_last_scrollbar_position(0);
}


//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::InitControls()
{
    int i;

    for (i=0; i<POS_CHKBTN_SIZE; ++i)
    {
        this->m_pChkBtnControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(DLGPOS_CHKBTN_MEMBER_INFOS + i));
        assert (this->m_pChkBtnControls[i]);
    }

    this->m_pLstControl =
        static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_MEMBERS));
    assert (this->m_pLstControl);

    this->m_pScrollControl =
        static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_SCR_VERTICAL));
    assert (this->m_pScrollControl);

    for (i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_EXIT + i));
        assert (this->m_pBtnControls[i]);
    }

	m_pListCorps = (CCtrlListWZ *) getControl(DLGPOS_LST_CORP);
	assert(m_pListCorps);

	m_pListCorps->SetStartLineGapLeft(7);
	m_pListCorps->SetStartLineGapTop(6); 
	m_pListCorps->ShowWindowWZ(WZ_HIDE);

	m_iCurSelCorps = 0;
	m_pCaptinCurSel = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_CURRENT_SELECT);
	assert(m_pCaptinCurSel);
	m_pStaticNumGuildMember = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_NUMBER_OF_GUILDMEMBER);
	assert(m_pStaticNumGuildMember);

    this->m_bControlsInitialized = true;

	RECT rc = m_pLstControl->GetSize();
	int ReportHeight = m_pLstControl->GetReportHeight();
	m_pLstControl->SetHideReportRender( true );
	m_MaxPage = 0;
	if( ReportHeight > 0 )
	{
		m_MaxPage = rc.bottom / ReportHeight ;
	}
	assert(ReportHeight);

	m_pSortName		= getControl( DLGPOS_BTN_SORT_NAME );
	m_pSortLevel	= getControl( DLGPOS_BTN_SORT_LEVEL );
	m_pSortClass	= getControl( DLGPOS_BTN_SORT_CLASS );
	m_pSortDuty		= getControl( DLGPOS_BTN_SORT_DUTY );
	m_pSortConnect	= getControl( DLGPOS_BTN_SORT_CONNECT );

}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
