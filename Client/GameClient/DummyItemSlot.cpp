#include "SunClientPrecompiledHeader.h"
#include "dummyitemslot.h"

DummyItemSlot::DummyItemSlot(void)
{
}

DummyItemSlot::~DummyItemSlot(void)
{
}

VOID DummyItemSlot::Clear()
{
	m_FromSlotSerial	= 0;
	m_FromContainerIdx	= SI_MAX;
	m_FromPos			= 0;
	m_pSrcSlot			= NULL;
}

VOID DummyItemSlot::Copy( SCSlot & IN slot )
{
	DummyItemSlot & rDummySlot	= (DummyItemSlot & )slot;
	m_FromSlotSerial			= rDummySlot.GetSerial();
	m_FromContainerIdx			= rDummySlot.GetFromContainerIdx();
	m_FromContainerIdx			= rDummySlot.GetFromPosition();
	m_pSrcSlot					= rDummySlot.GetOrgSlot();
}