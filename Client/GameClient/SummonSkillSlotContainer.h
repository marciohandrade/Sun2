//----------------------------------------------------------------------------
/** ��ȯ ��ų  â Ŭ����
	@author
			������
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

	virtual RC::eSLOT_INSERT_RESULT		InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );				//< ���Կ� ��ü �߰�
	virtual BOOL						DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );				//< ������ ��ü ����
};
