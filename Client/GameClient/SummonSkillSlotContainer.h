//----------------------------------------------------------------------------
/** 소환 스킬  창 클래스
	@author
			김지완
	@since
			
	@remarks
*/

#pragma once
#include "SkillSlotContainer.h"


class SummonSkillSlotContainer :
	public SCSkillSlotContainer
{
public:
	SummonSkillSlotContainer(void);
	virtual ~SummonSkillSlotContainer(void);

	virtual SCSlot *		CreateSlot();
	virtual VOID			Init( POSTYPE MaxSlotSize );
	virtual BOOL			IsEmpty( POSTYPE AtPos );

	virtual RC::eSLOT_INSERT_RESULT		InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );				//< 슬롯에 객체 추가
	virtual BOOL						DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );				//< 슬롯의 객체 삭제
};
