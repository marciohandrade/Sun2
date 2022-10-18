//------------------------------------------------------------------------------
/**

    Dummy Slot Container 로서 실제로 데이터가 이동하는것은 아지만 특정위치의 정보를 가지고 있어야할
	필요가 있을때 사용하는 컨테이너

    @author
		유재영 < y2jinc@webzen.co.kr >

    @since

    @remarks
		- 유저의 마음대로 더미 컨테이너 위에 올려져있는 아이템을 이동시킬수 없다.
		- 데이터만 복사하고 어디서 왔는지만 명시해놓는 식이다.
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
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );		// 슬롯의 객체 삭제
};
