#include "SunClientPrecompiledHeader.h"
#include ".\dummyslotset.h"
#include "dummyitemslot.h"

DummySlotSet::DummySlotSet(void) : m_pItemSlotArray(0)
{
}

DummySlotSet::~DummySlotSet(void)
{
		ASSERT( m_pItemSlotArray == NULL );
}


VOID DummySlotSet::DestorySlotSet()
{
	setSlotSetSize( 0 );
	if( m_pItemSlotArray )
	{
		delete [] m_pItemSlotArray;
		m_pItemSlotArray = NULL;
	}
}


VOID DummySlotSet::CreateSlotSet( DWORD dwSlotSetSize )
{
	setSlotSetSize( dwSlotSetSize );
	m_pItemSlotArray	= new DummyItemSlot[dwSlotSetSize];
}


VOID DummySlotSet::ClearSlotSet()
{
	for( DWORD i = 0 ; i < GetSlotSetSize() ; ++i )
	{
		m_pItemSlotArray[i].Clear();
	}
}


BOOL DummySlotSet::IsEmpty( POSTYPE pos )
{
	return ( m_pItemSlotArray[pos].GetSerial() == 0 );
}


VOID DummySlotSet::ClearSlot( POSTYPE pos )
{
	ASSERT( pos < GetSlotSetSize() );
	m_pItemSlotArray[pos].Clear();
}


VOID DummySlotSet::SetSlot( POSTYPE pos, const SCSlot * pSlot ) const
{
	ASSERT( pos < GetSlotSetSize() );

	DummyItemSlot * pDummySlot = (DummyItemSlot *)pSlot;

	m_pItemSlotArray[pos].SetSerial( pDummySlot->GetSerial() );
	m_pItemSlotArray[pos].SetDura( pDummySlot->GetDura() );
	m_pItemSlotArray[pos].SetCode( pDummySlot->GetCode() );
	m_pItemSlotArray[pos].SetQuickPos( pos );
	m_pItemSlotArray[pos].SetFromContainerIdx(pDummySlot->GetFromContainerIdx() );
	m_pItemSlotArray[pos].SetFromPosition( pDummySlot->GetFromPosition() );
}


SCSlot * DummySlotSet::getSlot( POSTYPE pos )
{
	return &m_pItemSlotArray[pos];	
}

