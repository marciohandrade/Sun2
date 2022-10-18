//------------------------------------------------------------------------------
/**

    Dummy Slot Container ���� ����� ��� ��

    @author
		���翵 < y2jinc@webzen.co.kr >

    @since

    @remarks
		- 
		- 
*/

#pragma once
#include "scslotset.h"

class DummyItemSlot;

class DummySlotSet :
	public SCSlotSet
{
public:
	DummySlotSet(void);
	virtual ~DummySlotSet(void);

	virtual VOID			DestorySlotSet();
	virtual VOID			CreateSlotSet( DWORD dwSlotSetSize );
	virtual VOID			ClearSlotSet();

	virtual BOOL			IsEmpty( POSTYPE pos );

	// delete slot information for pos
	virtual VOID			ClearSlot( POSTYPE pos );
	// insert slot information at pos
	virtual VOID			SetSlot( POSTYPE pos, const SCSlot * pSlot ) const;

protected:
	// for using in SCSlotContainer
	virtual SCSlot *		getSlot( POSTYPE pos );

private:
	DummyItemSlot *			m_pItemSlotArray;
};
