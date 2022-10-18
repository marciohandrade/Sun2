//------------------------------------------------------------------------------
//  uiVendorSell_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "SCItemSlot.h"
#include "uiVendorSell.h"
#include "ItemUnitRenderer.h"
#include "GlobalFunc.h"
#include "uivendorman/uiVendorMan.h"

//------------------------------------------------------------------------------
/**
*/
uiVendorSell::uiVendorSell(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_pItemUnitRenderer(NULL),
    m_bFocus(false),
    m_bStarted(false)
{
    this->Init(MAX_PERSONAL_VENDOR_SLOT_NUM);
	m_pVendorMan = GET_DIALOG_MANAGER(uiVendorMan, UIMAN_VENDOR);
	assert(m_pVendorMan);
	m_bShow			  = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
uiVendorSell::~uiVendorSell()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
SCSlot * uiVendorSell::CreateSlot()
{
	return new uiSCItemSlot;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::Init(POSTYPE MaxSlotSize)
{
	SCItemSlotContainer::Init( MaxSlotSize, 0 );

#ifdef 	_JBH_MOD_MONEY_OUTPUT
	m_bClickMoney = FALSE;
	m_dwPendingTick = 0;
	m_MouseOverPostype = NULL;
#endif	

	m_bShow			  = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
#ifdef _JBH_ADD_VENDOR_COMMISION
	m_curCommision	= 0;
	
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	CCtrlStaticWZ * pControl = ( CCtrlStaticWZ * )  getControl(DIALOG_TXT_COMMISION_OWNER);
	if( pControl )
	{
		pControl->SetTextColor(WzColor_RGBA(0,200,0,255));
		// 마을 관리자 부재 9556
		g_InterfaceManager.GetInterfaceString(eST_VILLAGE_MANAGER_NO_EXIST , szMessage ,INTERFACE_STRING_LENGTH );
		GlobalFunc::SetCaption(pControl ,szMessage ); 
	}
#endif


	m_pButtonPause = (CCtrlButtonWZ *) getControl(DIALOG_BTN_PAUSE);
	assert(m_pButtonPause);

	m_bShow			  = FALSE;

	// Todo:생성위치 맞출것 
	SolarDialog::MoveCenter();

	// 매니저에 위치를 저장하여 (개인 상점 구매/판매 모두 같은 위치를 공유하게 한다. 초기화는 여기에서만 한다)
	RECT rc;
	this->GetDialogWZ()->GetClientRect(&rc);
	POINT pt = { rc.left, rc.top };
	this->m_pVendorMan->SetWinPos(pt);
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::Release()
{
    SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer *
uiVendorSell::GetSlotContainer()
{
    return static_cast<SCSlotContainer *>(this);
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::SetCurrentItemSlotInfo(DURATYPE curAmounts, MONEY curMoney, POSTYPE curPos)
{
    this->m_curAmouts   = curAmounts;
    this->m_curMoney    = curMoney;
    this->m_curPos      = curPos;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorSell::SetVendorMessage(TCHAR* pmsg)
{
    StrCopy (this->m_szVendorMsg, pmsg);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
