//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "styleslotcontainer.h"
#include "StyleSlot.h"
#include "SlotInsertResultConvert.h"

StyleSlotContainer::StyleSlotContainer(void)
{
}

StyleSlotContainer::~StyleSlotContainer(void)
{
}


VOID StyleSlotContainer::Init( POSTYPE MaxSlotSize )
{
	SCQuickStyleSlotContainer::Init( MaxSlotSize, 0 );
}

SCSlot * StyleSlotContainer::CreateSlot()
{
	return new StyleSlot;
}

BOOL StyleSlotContainer::IsEmpty( POSTYPE AtPos )
{
	StyleSlot & rStyleSlot = (StyleSlot &) GetSlot(AtPos);

	return BOOL(rStyleSlot.GetOrgCode() == 0);
}

RC::eSLOT_INSERT_RESULT		StyleSlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
#ifdef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
	return SCSlotContainer::InsertSlot( AtPos, rSlot );
#else
	assert( AtPos < GetMaxSlotNum() );
	assert( IsEmpty(AtPos) );	
	if( AtPos >= GetMaxSlotNum() )			return RC::RC_INSERT_SLOT_FAILED;
	if( !IsEmpty(AtPos) )					return RC::RC_INSERT_SLOT_FAILED;
	if( GetSlotNum() >= GetMaxSlotNum() )	return RC::RC_INSERT_SLOT_FAILED;

	SCSlot & rStyleSlot = GetSlot(AtPos);

	rStyleSlot.Copy( rSlot );
	rStyleSlot.SetPos( AtPos );
	++m_nSlotNum;
	assert( m_nSlotNum <= GetMaxSlotNum() );

	return RC::RC_INSERT_SLOT_SUCCESS;

#endif
}


BOOL StyleSlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
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