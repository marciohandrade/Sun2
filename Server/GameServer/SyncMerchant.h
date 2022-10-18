#pragma once

#ifndef SUN_GAMESERVER_SYNCMERCHANT_H_
#define SUN_GAMESERVER_SYNCMERCHANT_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Monster.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_REGEN

class SyncMerchant : public Monster
{
public:
    SyncMerchant();
    virtual	~SyncMerchant();

    virtual	VOID OnDead();
    virtual	eUSER_RELATION_TYPE IsFriend(Character* pTarget);

    virtual VOID OnEnterObject(const Sector& IN rSector);
	virtual	VOID OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle=-1 );
};

#endif // SUN_GAMESERVER_SYNCMERCHANT_H_