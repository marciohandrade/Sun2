#include "SunClientPrecompiledHeader.h"
#include ".\quickslotset.h"
#include "QuickSlot.h"

QuickSlotSet::QuickSlotSet(void) :	m_pQuickSlotArray ( NULL )
{
}

QuickSlotSet::~QuickSlotSet(void)
{
	ASSERT( m_pQuickSlotArray == NULL );
}


VOID QuickSlotSet::DestorySlotSet()
{
	setSlotSetSize( 0 );
	if( m_pQuickSlotArray )
	{
		delete [] m_pQuickSlotArray;
		m_pQuickSlotArray = NULL;
	}
}

VOID QuickSlotSet::CreateSlotSet( DWORD dwSlotSetSize )
{
	setSlotSetSize( dwSlotSetSize );
	m_pQuickSlotArray	= new QuickSlot[dwSlotSetSize];
}

VOID QuickSlotSet::ClearSlotSet()
{
	for( DWORD i = 0 ; i < GetSlotSetSize() ; ++i )
	{
		m_pQuickSlotArray[i].Clear();
	}
}

BOOL QuickSlotSet::IsEmpty( POSTYPE pos )
{
	return ( m_pQuickSlotArray[pos].GetSerial() == 0 );
}

VOID QuickSlotSet::ClearSlot( POSTYPE pos )
{
	ASSERT( pos < GetSlotSetSize() );
	m_pQuickSlotArray[pos].Clear();
}

VOID QuickSlotSet::SetSlot( POSTYPE pos, const SCSlot * pSlot ) const 
{
	ASSERT( pos < GetSlotSetSize() );

	m_pQuickSlotArray[pos].SetSerial( pSlot->GetSerial() );
	m_pQuickSlotArray[pos].SetDura( pSlot->GetDura() );
	m_pQuickSlotArray[pos].SetCode( pSlot->GetCode() );
	m_pQuickSlotArray[pos].SetQuickPos( pos );
}

SCSlot * QuickSlotSet::getSlot( POSTYPE pos ) 
{ 
	return &m_pQuickSlotArray[pos];	
}
