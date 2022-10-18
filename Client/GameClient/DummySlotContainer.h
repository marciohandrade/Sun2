//------------------------------------------------------------------------------
/**

    Dummy Slot Container �μ� ������ �����Ͱ� �̵��ϴ°��� ������ Ư����ġ�� ������ ������ �־����
	�ʿ䰡 ������ ����ϴ� �����̳�

    @author
		���翵 < y2jinc@webzen.co.kr >

    @since

    @remarks
		- ������ ������� ���� �����̳� ���� �÷����ִ� �������� �̵���ų�� ����.
		- �����͸� �����ϰ� ��� �Դ����� ����س��� ���̴�.
*/

#pragma once
#include <SCItemSlotContainer.h>

class DummyItemSlot;

class DummySlotContainer : public SCSlotContainer
{
public:
	DummySlotContainer(void);
	virtual ~DummySlotContainer(void);
	
	virtual SCSlot *		CreateSlot();
	virtual VOID			Init( POSTYPE MaxSlotSize );
	virtual VOID			Init( POSTYPE MaxSlotSize, SLOTIDX SlotIndex ) {};
	virtual BOOL			IsEmpty( POSTYPE AtPos );


	VOID					InsertSlot( POSTYPE AtPos, ITEMSTREAMEX & stream );
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// ���Կ� ��ü �߰�
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );		// ������ ��ü ����
};
