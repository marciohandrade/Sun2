#ifndef __QUICK_SLOT_H__
#define __QUICK_SLOT_H__


//=======================================================================================================================
/** 단축창의 단축 아이템의 클래스 : QuickSlot
	@author
			유재영 	
	@since
			2004. 1. 10
	@remarks
			
*/

#pragma once

#include <SCQuickSlot.h>

enum eSLOT_RENDER_STATE;

class QuickSlot : public SCQuickSlot
{
public:
	QuickSlot();
	virtual ~QuickSlot();

	QuickSlot(const QUICKSTREAM & quickStream);

	virtual eSlotType		GetSlotType() const	{ return ST_QUICK;	}
	virtual DBSERIAL		GetSerial() const	{ return GetOrgSerial();	}

	DBSERIAL				GetOrgSerial() const	{ return m_OrgSerial; }
	VOID					SetOrgSerial(DBSERIAL serial) { m_OrgSerial = serial; }

	eSlotType				GetOrgSlotType()	{ return m_eOrgSlotType;	}
	VOID					SetOrgSlotType(eSlotType type)	{ m_eOrgSlotType = type;	}
	VOID					SetStatus(eSLOT_RENDER_STATE  eStatus) { m_eStatus = eStatus; }
	eSLOT_RENDER_STATE		GetStatus() const	{ return m_eStatus; }
	VOID					SetOverlapCount(WORD wCount) { m_wSlotCount = wCount; }
	WORD 					GetOverlapCount()	{ return m_wSlotCount; }

	VOID					SetOrgSlot( SCSlot * pSlot ) { m_pSrcSlot = pSlot;	}
	SCSlot *				GetOrgSlot() { return m_pSrcSlot;	}

	virtual VOID			Clear();
	virtual VOID			Copy( SCSlot & IN slot );

protected:
	DBSERIAL				m_OrgSerial;
	SCSlot *				m_pSrcSlot;				// 정보 표시를 위한
	WORD					m_wSlotCount;
	eSlotType				m_eOrgSlotType;
	eSLOT_RENDER_STATE		m_eStatus;
};

#endif // __QUICK_SLOT_H__