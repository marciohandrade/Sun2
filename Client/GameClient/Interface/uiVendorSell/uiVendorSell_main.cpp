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
		// ���� ������ ���� 9556
		g_InterfaceManager.GetInterfaceString(eST_VILLAGE_MANAGER_NO_EXIST , szMessage ,INTERFACE_STRING_LENGTH );
		GlobalFunc::SetCaption(pControl ,szMessage ); 
	}
#endif


	m_pButtonPause = (CCtrlButtonWZ *) getControl(DIALOG_BTN_PAUSE);
	assert(m_pButtonPause);

	m_bShow			  = FALSE;

	// Todo:������ġ ����� 
	SolarDialog::MoveCenter();

	// �Ŵ����� ��ġ�� �����Ͽ� (���� ���� ����/�Ǹ� ��� ���� ��ġ�� �����ϰ� �Ѵ�. �ʱ�ȭ�� ���⿡���� �Ѵ�)
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
