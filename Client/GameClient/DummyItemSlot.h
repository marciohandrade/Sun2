//------------------------------------------------------------------------------
/**

    Dummy Slot Container 에서 사용할 재료

    @author
		유재영 < y2jinc@webzen.co.kr >

    @since

    @remarks
		- 
		- 
*/

#pragma once

#include <SCItemSlot.h>

class DummyItemSlot :
	public SCSlot
{
public:
	DummyItemSlot(void);
	virtual ~DummyItemSlot(void);

	virtual eSlotType		GetSlotType() const	{ return ST_DUMMY_ITEM; }

	virtual VOID			Copy( SCSlot & IN slot );
	VOID					Clear();


	virtual VOID			SetSerial( DBSERIAL serial )	{ m_FromSlotSerial = serial; }
	virtual DBSERIAL		GetSerial() const	{ return m_FromSlotSerial; }

	VOID					SetFromContainerIdx(SLOTIDX Index) { m_FromContainerIdx = Index;}
	VOID					SetFromPosition(POSTYPE pos) { m_FromPos = pos; }

	
	SLOTIDX					GetFromContainerIdx() { return m_FromContainerIdx; }
	POSTYPE					GetFromPosition() { return m_FromPos; }

	VOID					SetOrgSlot( SCSlot * pSlot ) { m_pSrcSlot = pSlot;	}
	SCSlot *				GetOrgSlot() { return m_pSrcSlot;	}

	eSlotType				GetOrgSlotType() { return m_pSrcSlot->GetSlotType(); }


protected:
	SCSlot *				m_pSrcSlot;
	DBSERIAL				m_FromSlotSerial;
	SLOTIDX					m_FromContainerIdx;
	POSTYPE					m_FromPos;
};
