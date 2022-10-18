#ifndef __TEMP_STORE_SLOT_CONTAINER_H__
#define __TEMP_STORE_SLOT_CONTAINER_H__

#pragma once

#if SUN_CODE_BACKUP

#include "SCItemSlotContainer.h"

#include <ResultCode.h>

class CTempStoreSlotContainer : public SCItemSlotContainer
{
public:
	CTempStoreSlotContainer();
	virtual ~CTempStoreSlotContainer(void);

	virtual RC::eSLOT_INSERT_RESULT		InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	virtual BOOL						DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

	inline VOID							SetPlayer( Player * pPlayer ) { m_pPlayer = pPlayer; }

	//인벤토리락
	virtual BOOL						ValidState();
protected:
	Player *							m_pPlayer;
};

#endif //SUN_CODE_BACKUP

#endif // __TEMP_STORE_SLOT_CONTAINER_H__