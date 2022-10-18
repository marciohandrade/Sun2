//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "summonskillslotcontainer.h"
#include "skillslot.h"

SummonSkillSlotContainer::SummonSkillSlotContainer(void)
{
}

SummonSkillSlotContainer::~SummonSkillSlotContainer(void)
{
}


VOID SummonSkillSlotContainer::Init( POSTYPE MaxSlotSize )
{
	SCSkillSlotContainer::Init( MaxSlotSize, 0 );
}

SCSlot * SummonSkillSlotContainer::CreateSlot()
{
	return new SkillSlot;
}

BOOL SummonSkillSlotContainer::IsEmpty( POSTYPE AtPos )
{
	SkillSlot & rSkillSlot = (SkillSlot &) GetSlot(AtPos);

	if(rSkillSlot.GetCode() == 0)
		return TRUE;
	else
		return FALSE;

}

RC::eSLOT_INSERT_RESULT		SummonSkillSlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
#ifdef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
	return SCSlotContainer::InsertSlot( AtPos, rSlot );
#else //__NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
	assert( AtPos < GetMaxSlotNum() );
	assert( IsEmpty(AtPos) );	

	SCSlot & rSkillSlot = GetSlot(AtPos);

	rSkillSlot.Copy( rSlot );
	rSkillSlot.SetPos( AtPos );

	++m_nSlotNum;
	assert( m_nSlotNum <= GetMaxSlotNum() );

	return RC::RC_INSERT_SLOT_SUCCESS;
#endif //__NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
}


BOOL SummonSkillSlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
#ifdef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
	return SCSlotContainer::DeleteSlot( AtPos, pSlotOut );
#else
	assert( AtPos < GetMaxSlotNum() );

	if( pSlotOut )
	{
		pSlotOut->Copy( GetSlot(AtPos) );
	}

    if (m_nSlotNum>0)
        --m_nSlotNum;

	GetSlot(AtPos).Clear();
	assert( m_nSlotNum <= GetMaxSlotNum() );

	return TRUE;
#endif
}