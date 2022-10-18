//----------------------------------------------------------------------------
/** 스타일 창 클래스
	@author
			유재영 (y2jinc@webzen.co.kr)
	@since
			
	@remarks
*/

#pragma once
#include "SCQuickStyleSlotContainer.h"

class StyleSlotContainer :
	public SCQuickStyleSlotContainer
{
public:
	StyleSlotContainer(void);
	virtual ~StyleSlotContainer(void);

	SCSlot * CreateSlot();
	virtual VOID			Init( POSTYPE MaxSlotSize );
	virtual BOOL			IsEmpty( POSTYPE AtPos );

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL			DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );				//< 슬롯의 객체 삭제
};
