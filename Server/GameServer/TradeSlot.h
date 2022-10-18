//=======================================================================================================================
/// TradeSlot class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@history
			- 2006. 3. 4 : trade관련 리펙토링
			
*/
//=======================================================================================================================

#pragma once

#include <scslot.h>
#include <SCItemSlot.h>

class TradeSlot : public SCSlot
{
public:
	TradeSlot(void);
	virtual ~TradeSlot(void);

	virtual VOID			Clear(){ if(m_pItemSlot) m_pItemSlot->SetLock(FALSE);  m_pItemSlot = NULL; }
	virtual VOID			Copy( SCSlot & IN slot );

	virtual eSlotType		GetSlotType() const	{ return ST_TRADE; }
	virtual DBSERIAL		GetSerial() const	{ if( !m_pItemSlot) return 0; return m_pItemSlot->GetSerial(); }

	virtual DBWLDSERIAL		GetWorldSerial() const { if( !m_pItemSlot) return 0; return m_pItemSlot->GetWorldSerial();	}

    // (CHANGES) (f100316.1) (WAVERIX) fix up missing null pointer check that old logic is passed by garbage data
	virtual SLOTCODE		GetCode() const {
        if (m_pItemSlot) {
            return m_pItemSlot->GetCode();
        }
        return 0;
    }

	inline VOID SetLinkItem( SCItemSlot * pItem )	{ if(pItem) pItem->SetLock(TRUE); if(m_pItemSlot) m_pItemSlot->SetLock(FALSE);	m_pItemSlot = pItem; }
	inline SCItemSlot * GetLinkItem()				{ return m_pItemSlot; }

private:
	SCItemSlot *			m_pItemSlot;
};
