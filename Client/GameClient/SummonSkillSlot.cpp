#include "SunClientPrecompiledHeader.h"
#include ".\SummonSkillslot.h"

SummonSkillSlot::SummonSkillSlot(void)
{
}

SummonSkillSlot::~SummonSkillSlot(void)
{
}

VOID SummonSkillSlot::Clear()
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
VOID SummonSkillSlot::Copy( SCSlot & IN slot )
{
	SCQuickStyleSlot::Copy(slot);

	// 여기서 클라에서 무언가 할게있다면 하자.
	SummonSkillSlot & rStyleSlot = (SummonSkillSlot &)slot;

	ASSERT(rStyleSlot.GetOrgSlotType() == ST_SKILL);
	ASSERT(rStyleSlot.GetOrgSlot() != NULL);
	
	SetOrgSlotType( rStyleSlot.GetOrgSlotType() );
	SetOrgSlot( rStyleSlot.GetOrgSlot() );
}

