#pragma once
#include <scslotset.h>

class QuickSlot;

class QuickSlotSet :
	public SCSlotSet
{
public:
	QuickSlotSet(void);
	virtual ~QuickSlotSet(void);

	virtual VOID			DestorySlotSet();
	virtual VOID			CreateSlotSet( DWORD dwSlotSetSize );
	virtual VOID			ClearSlotSet();

	virtual BOOL			IsEmpty( POSTYPE pos );

	// delete slot information for pos
	virtual VOID			ClearSlot( POSTYPE pos );
	// insert slot information at pos
	virtual VOID			SetSlot( POSTYPE pos, const SCSlot * pSlot ) const ;

	// derived class new function
//	VOID					CopyStreamToSlotSet( BYTE * pStream, DWORD dwSize );
//	VOID					CopyStreamSlotSetTo( BYTE * pStream, DWORD dwSize );

protected:
	// for using in SCSlotContainer
	virtual SCSlot *		getSlot( POSTYPE pos );
private:

	//ITEMSTREAM	*		m_pItemStream;
	QuickSlot	*			m_pQuickSlotArray;
};
