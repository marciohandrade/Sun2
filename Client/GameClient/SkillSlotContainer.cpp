#include "SunClientPrecompiledHeader.h"
#include "skillslotcontainer.h"
#include "SkillSlot.h"

SkillSlotContainer::SkillSlotContainer(void)
{
}

SkillSlotContainer::~SkillSlotContainer(void)
{
}

VOID SkillSlotContainer::Init( POSTYPE MaxSlotSize )
{
	SCSkillSlotContainer::Init( MaxSlotSize, 0 );
}

SCSlot * SkillSlotContainer::CreateSlot()
{
	return new SkillSlot;
}

//BOOL SkillSlotContainer::InsertSlot( POSTYPE AtPos, DBSERIAL Serial, SKILLSTREAM & stream )
//{
//	assert( AtPos < GetMaxSlotNum() );
//	assert( IsEmpty(AtPos) );
//
//	if( AtPos >= GetMaxSlotNum() )			return FALSE;
//	if( !IsEmpty(AtPos) )					return FALSE;
//	if( GetSlotNum() >= GetMaxSlotNum() )	return FALSE;
//
//#ifdef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
//	SCSkillSlot *pSlot = (SCSkillSlot*)CreateSlot();
//	if( !pSlot )	return FALSE;
//
//	m_mapSlot.insert( SLOT_MAP::value_type(AtPos, pSlot) );
//
//	pSlot->Copy( stream );
//	pSlot->SetPos( AtPos );
//	pSlot->SetSlotIdx( GetSlotIdx() );
//	pSlot->OnInsert();
//
//	OnInsert( *pSlot );
//
//	m_pSkillHashTable->Add( pSlot, stream.Part.wCode);
//#else
//	SCSkillSlot & rSlot = (SCSkillSlot&)GetSlot(AtPos);
//	rSlot.Copy(stream);
//	rSlot.SetPos( AtPos );
//	rSlot.SetSerial(Serial);
//
//	++m_nSlotNum;
//	assert( m_nSlotNum <= GetMaxSlotNum() );
//
//	assert( skill_slots_.GetData( stream.Part.wCode ) == NULL );
//
//	skill_slots_.Add( &rSlot, stream.Part.wCode);
//#endif
//
//	return TRUE;
//}
//







