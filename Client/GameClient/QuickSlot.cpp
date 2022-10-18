#include "SunClientPrecompiledHeader.h"
#include "SCQuickSlot.h"
#include "quickslot.h"
#include "ItemUnitRenderer.h"


QuickSlot::QuickSlot()
	:	m_pSrcSlot( NULL ),
		m_wSlotCount( 0 ),
		m_eOrgSlotType(ST_NONE),
		m_eStatus(SLOT_RENDER_STATE_ACTIVATED),
		m_OrgSerial(0)
{

}


QuickSlot::QuickSlot(const QUICKSTREAM & quickStream)
{
	SCQuickSlot::Copy(quickStream);
}

QuickSlot::~QuickSlot()
{
}


VOID QuickSlot::Clear()
{
	SCQuickSlot::Clear();
	m_pSrcSlot	= NULL;
	m_wSlotCount = 0;
	m_eOrgSlotType = ST_NONE;
	m_eStatus = SLOT_RENDER_STATE_ACTIVATED;
	m_OrgSerial = 0;
}


VOID QuickSlot::Copy( SCSlot & IN slot )
{
	SCQuickSlot::Copy(slot);

	// 여기서 클라에서 무언가 할게있다면 하자.
	QuickSlot & rQuickSlot	= (QuickSlot &)slot;

	SetOrgSerial( rQuickSlot.GetOrgSerial() );
	SetOverlapCount( rQuickSlot.GetOverlapCount() );
	SetOrgSlotType( rQuickSlot.GetOrgSlotType() );
	SetStatus( rQuickSlot.GetStatus() );
	SetOrgSlot( rQuickSlot.GetOrgSlot() );
}

