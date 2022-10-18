#pragma once

#include ".\BaseImplement.h"

class RandomItemImplement : public BaseImplement
{
public:
    RandomItemImplement( void );
	virtual ~RandomItemImplement( void );

protected:
    virtual BOOL OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult );
    virtual VOID OnImplement( MSG_BASE* pRecvMessage );
    virtual VOID OnFailed( DWORD dwResult );


private:
    RC::eITEM_RESULT GenerateRandomEquipment( SCItemSlot& IN ItemSlot, SCItemSlot& OUT Output );
};
