#pragma once
#include "scQuickstyleslot.h"

class SCSlot;
class SCStyleSlot;

class StyleSlot :
	public SCQuickStyleSlot
{
public:
	StyleSlot(void);
	virtual ~StyleSlot(void);

	virtual SLOTCODE		GetCode() const		{ return ((SCQuickStyleSlot *)(this))->GetOrgCode(); }	
	virtual eSlotType		GetSlotType() const	{ return ST_STYLE_QUICK; }

	eSlotType				GetOrgSlotType()	{ return m_eOrgSlotType;	}
	VOID					SetOrgSlotType(eSlotType type)	{ m_eOrgSlotType = type;	}
	VOID					SetOrgSlot( SCSlot * pSlot ) { m_pSrcSlot = pSlot;	}
	SCSlot *				GetOrgSlot() { return m_pSrcSlot;	}

	virtual VOID			Clear();
	virtual VOID			Copy( SCSlot & IN slot );


private:
	SCSlot *				m_pSrcSlot;			
	eSlotType				m_eOrgSlotType;	

};
