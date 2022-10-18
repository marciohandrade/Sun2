//------------------------------------------------------------------------------
//  uiVendorBuy_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiVendorBuy.h"
#include "uiVendorSell/uiVendorSell_def.h"
#include "ItemUnitRenderer.h"
#include "uivendorman/uiVendorMan.h"

//------------------------------------------------------------------------------
/**
*/
uiVendorBuy::uiVendorBuy(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_pItemUnitRenderer(NULL)
{
	m_pVendorMan = GET_DIALOG_MANAGER(uiVendorMan, UIMAN_VENDOR);
	assert(m_pVendorMan);
    ClearCheckItem();

    this->Init( MAX_PERSONAL_VENDOR_SLOT_NUM );
}

//------------------------------------------------------------------------------
/**
*/
uiVendorBuy::~uiVendorBuy()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
SCSlot * uiVendorBuy::CreateSlot()
{
	return new uiSCItemSlot;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::Init(POSTYPE MaxSlotSize)
{
	SCItemSlotContainer::Init(MaxSlotSize,0);

#ifdef _JBH_MOD_MONEY_OUTPUT
	m_dwPendingTick = 0;
	m_bClickMoney = FALSE;
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	// Todo:생성위치 맞출것 
	SolarDialog::MoveCenter();
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::Release()
{
    SAFE_RELEASENDELETE( m_pItemUnitRenderer );
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer *
uiVendorBuy::GetSlotContainer()
{
    return static_cast<SCSlotContainer *>(this);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
