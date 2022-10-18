#include "SunClientPrecompiledHeader.h"
#include "styleslot.h"

StyleSlot::StyleSlot(void)
{
}

StyleSlot::~StyleSlot(void)
{
}

VOID StyleSlot::Clear()
{
	SCQuickStyleSlot::Clear();
	m_pSrcSlot	= NULL;
	m_eOrgSlotType = ST_NONE;
}

//-----------------------------------------------------------
/**
	@pre 
		- NULL != m_pSrcSlot
	@post
		- NULL != m_pSrcSlot

*/
VOID StyleSlot::Copy( SCSlot & IN slot )
{
	SCQuickStyleSlot::Copy(slot);

	// 여기서 클라에서 무언가 할게있다면 하자.
	StyleSlot & rStyleSlot = (StyleSlot &)slot;

	assert(rStyleSlot.GetOrgSlotType() == ST_SKILL);
	assert(rStyleSlot.GetOrgSlot() != NULL);
	
	SetOrgSlotType( rStyleSlot.GetOrgSlotType() );
	SetOrgSlot( rStyleSlot.GetOrgSlot() );
}

