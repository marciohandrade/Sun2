//----------------------------------------------------------------------------
/** ��Ÿ�� â Ŭ����
	@author
			���翵 (y2jinc@webzen.co.kr)
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

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// ���Կ� ��ü �߰�
	virtual BOOL			DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );				//< ������ ��ü ����
};
