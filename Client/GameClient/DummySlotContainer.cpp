#include "SunClientPrecompiledHeader.h"
#include "dummyslotcontainer.h"
#include "DummyItemSlot.h"


DummySlotContainer::DummySlotContainer(void)
{
}

DummySlotContainer::~DummySlotContainer(void)
{
}

VOID DummySlotContainer::Init( POSTYPE MaxSlotSize )
{
	SCSlotContainer::Init(MaxSlotSize,0);
}

SCSlot * DummySlotContainer::CreateSlot()
{
	return new DummyItemSlot;
}


BOOL DummySlotContainer::IsEmpty( POSTYPE AtPos )
{
	return BOOL( ((DummyItemSlot&)GetSlot(AtPos)).GetOrgSlot() == NULL);
}


// rSlot은 DummyItemSlot 으로 
// 각각의 부가정보를 채워서 Insert해줘야하한다.!!
RC::eSLOT_INSERT_RESULT		DummySlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DummyItemSlot & rDummyItemSlot = (DummyItemSlot &)rSlot;

	assert(rDummyItemSlot.GetOrgSlot());

	DummyItemSlot & rAtDummySlot = (DummyItemSlot &)GetSlot(AtPos);
	rAtDummySlot.SetSerial(rDummyItemSlot.GetOrgSlot()->GetSerial());
	rAtDummySlot.SetOrgSlot( rDummyItemSlot.GetOrgSlot() );
	rAtDummySlot.SetFromContainerIdx(rDummyItemSlot.GetFromContainerIdx() );
	rAtDummySlot.SetFromPosition(rDummyItemSlot.GetFromPosition() );

	return eInsertSlotResult;
}


BOOL DummySlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	// 이놈을 지울때
	// 실제 슬롯에서 찾아서 지워줘야지

	DummyItemSlot & rSlot = (DummyItemSlot &)GetSlot(AtPos);
	
	if( pSlotOut )
	{
		((DummyItemSlot *)pSlotOut)->SetSerial( rSlot.GetSerial() );
		((DummyItemSlot *)pSlotOut)->SetFromContainerIdx( rSlot.GetFromContainerIdx() );
		((DummyItemSlot *)pSlotOut)->SetFromPosition( rSlot.GetFromPosition() );
		((DummyItemSlot *)pSlotOut)->SetOrgSlot( rSlot.GetOrgSlot() );
	}

	return SCSlotContainer::DeleteSlot(AtPos, NULL);
}
