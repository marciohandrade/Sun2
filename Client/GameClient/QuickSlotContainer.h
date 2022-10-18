//=======================================================================================================================
/** 단축창 클래스
	@author
			유재영
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

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );				//< 슬롯의 객체 삭제
};
