//=======================================================================================================================
/** ����â Ŭ����
	@author
			���翵
	@since
			
	@remarks
*/

#pragma once
#include <SCQuickSlotContainer.h>

class QuickSlotContainer : public SCQuickSlotContainer
{
public:
	QuickSlotContainer(void);
	virtual ~QuickSlotContainer(void);

	SCSlot *				CreateSlot();
	virtual VOID			Init( POSTYPE MaxSlotSize, SLOTIDX idx = SI_QUICK);
	virtual BOOL			IsEmpty( POSTYPE AtPos );

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// ���Կ� ��ü �߰�
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );				//< ������ ��ü ����
};
