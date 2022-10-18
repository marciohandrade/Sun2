#include "SunClientPrecompiledHeader.h"
#include "quickslotcontainer.h"
#include "quickslot.h"

QuickSlotContainer::QuickSlotContainer(void)
{
}

QuickSlotContainer::~QuickSlotContainer(void)
{
}


VOID QuickSlotContainer::Init( POSTYPE MaxSlotSize, SLOTIDX idx /* = SI_QUICK */)
{
	SCQuickSlotContainer::Init( MaxSlotSize, idx );
}
SCSlot * QuickSlotContainer::CreateSlot()
{
	return new QuickSlot;
}


BOOL QuickSlotContainer::IsEmpty( POSTYPE AtPos )
{
	QuickSlot & rQuickSlot = (QuickSlot &) GetSlot(AtPos);

	return BOOL(rQuickSlot.GetOrgCode() == 0);
}

RC::eSLOT_INSERT_RESULT		QuickSlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	return SCQuickSlotContainer::InsertSlot(AtPos, rSlot);
}


BOOL QuickSlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	return SCQuickSlotContainer::DeleteSlot(AtPos, pSlotOut);
}
